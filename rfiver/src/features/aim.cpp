#include <include.h>

using namespace ft;

float c_aim::get_max_dist()
{
	if (g_cfg.aim_target_sel_max_dist <= 0.f)
		return FLT_MAX;

	return g_cfg.aim_target_sel_max_dist;
}

float c_aim::get_max_fov()
{
	if (g_cfg.aim_target_sel_max_fov <= 0.f)
		return 180.f;

	if (g_cfg.aim_target_sel_max_fov_shotgun > 0.f &&
		m_wpn_type == respawn::weapon_types::shotgun)
		return g_cfg.aim_target_sel_max_fov_shotgun * 0.3;

	if (g_cfg.aim_target_sel_max_fov_sniper > 0.f &&
		m_wpn_type == respawn::weapon_types::sniper)
		return g_cfg.aim_target_sel_max_fov_sniper * 0.3;

	return g_cfg.aim_target_sel_max_fov * 0.3;
}

bool c_aim::can_activate(respawn::user_cmd_t* cmd)
{
	if (ctx_arc.waiting_for_release || ctx_arc.aiming)
		return true;

	if (g_cfg.aim_auto_fire)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			return false;
	} else
	{
		if (!(cmd->buttons & IN_ATTACK))
			return false;
	}

	return true;
}

bool c_aim::get_target_pos(const cache::object_t& obj, cache::hitbox_t& out)
{
	/* manually whitelist some hitboxes because other ones are not very good... */
	static std::vector<int> viable_hitboxes =
	{
		0, 1, 2, 3, 4, 5, 16, 12, 17, 13
	};

	switch (g_cfg.aim_target_pos)
	{
	case 0: // auto
	{
		float closest_fov = FLT_MAX;
		int closest_hitbox = -1;

		for (int i : viable_hitboxes)
		{
			const auto& hb = obj.ds->m_hitboxes[i];

			if (!hb.visible)
				continue;

			auto fov = math::SE::get_fov_difference(m_ctx.my_angles, m_ctx.my_shoot_pos, hb.pos);

			if (fov < closest_fov)
			{
				closest_hitbox = i;
				closest_fov = fov;
				out = hb;
			}
		}

		return (closest_hitbox != -1);
	}
	case 1:
	{
		out = obj.ds->m_hitboxes[respawn::hitbox_ids::HITBOX_ID_HEAD];
		return out.visible;
	}
	case 2: 
	{
		out = obj.ds->m_hitboxes[respawn::hitbox_ids::HITBOX_ID_SPINE];
		return out.visible;
	}
	default:
		return false;
	}
}

void c_aim::paint()
{
	if (!g_cfg.aim_active) return;
	if (g::rce.force_disable_aim) return;

	if (g_cfg.aim_visualize_fov)
	{
		auto screen = g::screen;

		float local_fov = g_ctx.fov;
		float valid_fov = get_max_fov() / local_fov;

		float scenter_x = (screen.w / 2);
		float scenter_y = (screen.h / 2);

		auto scale = tan(get_max_fov() / 180.f * M_PI) /
			tan((local_fov / 2) / 180.f * M_PI)
			* screen.w / 2.f;

		render::circle({ scenter_x, scenter_y }, scale, 75, im::color::white);
	}

	if (m_aiming)
	{
		math::vec3_t scr;

		if (respawn::world_to_screen(m_aim_pos, scr))
		{
			auto x_center = scr.x;
			auto y_center = scr.y;

			auto width = 5.f;

			render::rect_filled({ x_center - width, y_center - 1.0f }, { x_center + width, y_center + 1.0f }, im::color::red);
			render::rect_filled({ x_center - 1.0f, y_center - width }, { x_center + 1.0f, y_center + width }, im::color::red);

			render::rect({ x_center - width, y_center - 1.0f }, { x_center + width, y_center + 1.0f }, im::color::red);
			render::rect({ x_center - 1.0f, y_center - width }, { x_center + 1.0f, y_center + width }, im::color::red);
		}
	}
}

bool c_aim::think(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	auto my_wpn = me->get_active_weapon();
	auto my_vm = me->get_view_model();
	if (!my_wpn || !my_vm) return false;

	auto my_vm_mdl_name = my_vm->model_name();
	if (!my_vm_mdl_name) return false;

	m_wpn_def = respawn::get_weapon_def( my_vm, my_wpn );
	m_wpn_type = respawn::get_weapon_type( m_wpn_def );

	if (!g_cfg.aim_ignore_key && !can_activate(cmd)) return false;

	auto my_idx = ix::engine_client->get_local_player_idx();
	auto my_origin = me->get_abs_origin();
	auto my_shoot_pos = me->view_offset();
	auto my_va = me->eye_angles();
	auto shots_fired = my_wpn->shots_fired();

	if( is_silent() && g_cfg.aim_spread_ticks > 0 &&
		m_wpn_type != respawn::weapon_types::shotgun &&
		m_wpn_type != respawn::weapon_types::sniper && 
		m_wpn_type != respawn::weapon_types::grenade &&
		(shots_fired % g_cfg.aim_spread_ticks) == 0 )
		return false;

	float max_dist = get_max_dist(), max_fov = get_max_fov();
	float closest_dist = max_dist, closest_fov = max_fov;

	respawn::c_entity* target = nullptr;
	math::vec3_t target_pos{ };

	m_ctx = { };

	m_ctx.my_angles = my_va;
	m_ctx.my_shoot_pos = my_shoot_pos;

	for (const auto& obj : g_ent_cache.objects())
	{
		if (!obj.hdr.m_used) continue;

		auto i = obj.hdr.m_ent_index;
		auto entity = ix::ent_list->GetClientEntity(i);

		if (!entity || entity == me)
			continue;

		if (obj.hdr.m_type == cache::object_t::type::player &&
			obj.ds->dt.player.online_drofnet_identity &&
			g_cfg.dev_ignore_drofnet_identity)
			continue;

		if (obj.hdr.m_type == cache::object_t::type::dummie ||
			obj.hdr.m_type == cache::object_t::type::player)
		{
			auto& pld = obj.ds->dt.player;

			if (!pld.alive) continue;

			cache::hitbox_t hitbox;

			if(!get_target_pos(obj, hitbox))
				continue;

			auto origin = entity->get_abs_origin();
			auto dist = origin.DistanceTo(my_origin) / 4096.f;
			auto fov = math::SE::get_fov_difference(my_va, my_shoot_pos, hitbox.pos);

			if (dist > max_dist || fov > max_fov)
				continue;

			switch (g_cfg.aim_target_sel_mode)
			{
			case c_aim::target_sel_mode::dist:
				if (dist < max_dist && dist < closest_dist)
				{
					target = entity;
					target_pos = hitbox.pos;
					closest_dist = dist;
				}
				break;
			case c_aim::target_sel_mode::fov:
				if (fov < max_fov && fov < closest_fov)
				{
					target = entity;
					target_pos = hitbox.pos;
					closest_fov = fov;
				}
				break;
			}
		}
	}

	if (target == nullptr)
		return false;

	auto velocity = target->get_abs_velocity();

	math::ang3_t aim_ang{ };
	math::VectorAngles((target_pos - my_shoot_pos), aim_ang);

	auto my_angle = me->eye_angles();
	auto my_angle_dyn = me->dynamic_angles();

	auto delta = (my_angle_dyn - my_angle);
	auto length = delta.Length2d();

	if (length > 0.f)
	{
		aim_ang.x -= delta.x;
		aim_ang.y -= delta.y;
	}

	if (!is_silent() && g_cfg.aim_smoothing_ratio > 0.f)
	{
		auto curr_angle = my_angle;
		auto delta = (aim_ang - curr_angle);

		math::NormalizeAngle(curr_angle);
		math::NormalizeAngle(delta);

		auto flMultiplier = g_cfg.aim_smoothing_ratio;
		auto vActualAimAngle = aim_ang;

		if (delta.x >= +89.f || delta.x <= -89.f)
			delta.x = 0.f;

		if (flMultiplier >= 0.01f)
		{
			aim_ang.x = curr_angle.x + ((delta.x + FLT_EPSILON) / flMultiplier);
			aim_ang.y = curr_angle.y + ((delta.y + FLT_EPSILON) / flMultiplier);
			aim_ang.z = 0.f;
		}
	}


	switch (m_wpn_def)
	{
	case respawn::weapons::arc_star:
		if (!ctx_arc.waiting_for_release)
		{
			ctx_arc.waiting_for_release = true;
		}
		else
		{
			if (!ctx_arc.aiming && !(cmd->buttons & IN_ATTACK))
			{
				ctx_arc.waiting_for_release = false;
				ctx_arc.aiming = true;
				ctx_arc.aim_ticks = 0;

				cmd->viewangles = aim_ang;

				if (!g_cfg.aim_silent)
					me->eye_angles() = aim_ang;
			}
		}

		if (ctx_arc.aiming)
		{
			if (ctx_arc.aim_ticks < 75)
			{
				cmd->viewangles = aim_ang;

				if (!g_cfg.aim_silent)
					me->eye_angles() = aim_ang;

				ctx_arc.aim_ticks++;
			}
			else
			{
				ctx_arc = { };
			}
		}

		break;
	default:
		ctx_arc = { };

		if (m_ctx.bt_tick != -1)
			cmd->tick_count = m_ctx.bt_tick;

		cmd->viewangles = aim_ang;

		if (g_cfg.aim_auto_ads)
			cmd->buttons |= 65536;

		if (!g_cfg.aim_silent)
			me->eye_angles() = aim_ang;

		if (g_cfg.aim_auto_fire)
			cmd->buttons |= IN_ATTACK;

		break;
	}

	m_aiming = true;
	m_aim_pos = target_pos;

	return true;
}

void c_aim::move(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (!g_cfg.aim_active || g::rce.force_disable_aim) goto reset;

	if (think(me, cmd))
		return;

reset:
	m_aiming = false;
	m_aim_pos = { };
}
