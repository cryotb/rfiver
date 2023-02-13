#include <include.h>

c_ent_cache g_ent_cache{ };

using e_type = cache::object_t::type;

math::ang3_t* get_eye_angles(void* entity, math::ang3_t* out)
{
	static auto addr = MOD_BASE("r5apex.exe") + 0x7D9A30;
	return reinterpret_cast<math::ang3_t*(__thiscall*)(void*, math::ang3_t*)>(addr)(entity, out);
}

namespace cache
{
	bool is_opp_visible_pos(respawn::c_player* me, respawn::c_player* opponent, const math::vec3_t& pos)
	{
		respawn::Ray_t ray;
		ZeroMemory(&ray, sizeof(ray));
		ray.Init(me->view_offset(), pos);

		respawn::trace_t tr;
		ZeroMemory(&tr, sizeof(tr));

		ix::engine_trace->trace_ray(ray, 0x640400B, &tr);

		return tr.fraction >= 1.f;
	}

	bool is_opp_visible(respawn::c_player* me, respawn::c_player* opponent, int bone_id)
	{
		return is_opp_visible_pos(me, opponent, opponent->get_bone_pos(bone_id));
	}

	struct glow_config_t
	{
		uint8_t general_mode;
		uint8_t border_mode;
		uint8_t border_len;
		uint8_t transp_lvl;
	};

	struct color_t
	{
		float r{}, g{}, b{};
	};

	namespace glow_colors
	{
		constexpr float opacity = 5.f;

		constexpr color_t red = { opacity, 1.f, 1.f };
		constexpr color_t blue = { 1.f, 1.f, opacity };
		constexpr color_t green = { 1.f, opacity, 1.f };
		constexpr color_t yellow = { opacity, opacity, 1.f };
		constexpr color_t white = { opacity, opacity, opacity };
	}

	namespace highlight
	{
		enum class CONTEXT
		{
			HIGHLIGHT_CONTEXT_NONE = -1,
			HIGHLIGHT_CONTEXT_NEUTRAL = 0,
			HIGHLIGHT_CONTEXT_FRIENDLY = 1,
			HIGHLIGHT_CONTEXT_ENEMY = 2,
			HIGHLIGHT_CONTEXT_OWNED = 3,
			HIGHLIGHT_CONTEXT_PINGED = 4,
			HIGHLIGHT_CHARACTER_SPECIAL_HIGHLIGHT = 5,
			HIGHLIGHT_CONTEXT_DEATH_RECAP = 6,
			HIGHLIGHT_CONTEXT_SONAR = 7,
			HIGHLIGHT_MAX_CONTEXTS = 8,
		};

		enum class DRAW_FUNC
		{
			ALWAYS, // always draw the highlight
			OCCLUDED, // draw the highlight when the object is not visible
			LOS, // draw the object only when we can see it
			LOS_LINE, // when visible, uses a line instead of hull trace
			LOS_LINE_ENTSONLYCONTENTSBLOCK,    // only blocked by ents with CONTENTS_BLOCKLOS
			ENT_APPEAR_EFFECT,
			BATTERY_THIEF,
			FW_FRIENDLY_TITAN,
			DON_PANEL,
			SPOT,
			PICKUP,
			SONAR_DETECTED,
			ALWAYS_LONG_FADE,
			CAUSTIC_THREAT,

			LAST_DRAW_FUNC,
		};

		enum OUTLINE
		{
			HIGHLIGHT_OUTLINE_NONE = 0,
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR = 101, // outline_default_custom_color
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR_WEAPON_PICKUP = 110,
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR_PULSE = 120, // outline_custom_color_pulse // set parm1.x to pulse rate
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR_OBEY_Z = 121, // outline_default_custom_obey_z
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR_OCCLUDED_NOSCANLINES = 129,
			HIGHLIGHT_OUTLINE_CUSTOM_COLOR_NOZ_NOSCANLINES = 169, //outline_custom_color_noz_noscanlines
			HIGHLIGHT_OUTLINE_SONAR = 103, // outline_sonar
			HIGHLIGHT_OUTLINE_INTERACT_BUTTON = 105, // outline_interact_button
			HIGHLIGHT_FILL_INTERACT_BUTTON = 103, // inside_interact_button
			HIGHLIGHT_OUTLINE_OBJECTIVE = 125, // outline_objective
		};

		bool enable(respawn::c_entity* ent)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0xB5A1E0;
			return GET_PTYPE(enable)(addr)(ent);
		}

		void set_funcs(
			respawn::c_entity* a1,
			CONTEXT a2,
			unsigned int insideFuncSlot,
			char a4,
			unsigned int outlineFuncSlot,
			float a6,
			unsigned int highlightState,
			char a8)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0x7D53E0;
			return GET_PTYPE(set_funcs)(addr)(
				a1, a2,
				insideFuncSlot, a4,
				outlineFuncSlot, a6,
				highlightState, a8
				);
		}

		void set_param(respawn::c_entity* ent, CONTEXT a2, unsigned int a3, float* col)
		{
			static auto fn = respawn::get_native_func<decltype(&set_param)>("Highlight_SetParam");
			return fn(
				ent, a2, a3, col
				);
		}

		void show_inside(respawn::c_entity* ent, float a2)
		{
			static auto fn = respawn::get_native_func<decltype(&show_inside)>("Highlight_ShowInside");
			return fn(ent, a2);
		}

		void show_outline(respawn::c_entity* ent, float a2)
		{
			static auto fn = respawn::get_native_func<decltype(&show_outline)>("Highlight_ShowOutline");
			return fn(ent, a2);
		}

		void set_curr_ctx(respawn::c_entity* ent, CONTEXT idx)
		{
			static auto fn = respawn::get_native_func<decltype(&set_curr_ctx)>("Highlight_SetCurrentContext");
			return fn(ent, idx);
		}

		void set_life_time(respawn::c_entity* ent, float a2)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0x619DB0;
			return GET_PTYPE(set_life_time)(addr)(ent, a2);
		}

		void set_vis_type(respawn::c_entity* ent, int type)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0x619CD0;
			return GET_PTYPE(set_vis_type)(addr)(ent, type);
		}

		bool start_on(respawn::c_entity* ent)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0x619E30;
			return GET_PTYPE(start_on)(addr)(ent);
		}
		
		u64 get_state(respawn::c_entity* ent, CONTEXT id)
		{
			static auto addr = MOD_BASE("r5apex.exe") + 0x7D5110;
			return GET_PTYPE(get_state)(addr)(ent, id);
		}
	}

	void process_player(object_t& entry, object_t& previous, respawn::c_player* entity, bool pl, bool cache_reset)
	{
		if (!entry.ds)
		{
			entry.ds = new object_t::data_t();
			
			if (!entry.ds)
			{
				entry.hdr.m_used = false;
				return;
			}
		}

		if (!entry.ds_prev)
		{
			entry.ds_prev = new object_t::data_t();

			if (!entry.ds_prev)
			{
				entry.hdr.m_used = false;
				return;
			}
		}

		entry.ds->dt.player.alive = entity->is_alive();

		if (cache_reset)
		{
			if (pl)
			{
				ix::engine_client->get_player_info(entry.hdr.m_ent_index, &entry.ds->dt.player.info);

				entry.ds->dt.player.nucleus_id = entity->get_nucleus_id();
				//entry.ds->dt.player.online_drofnet_identity = g_drof_net.is_online_drofnet_identity(entry.ds->dt.player.nucleus_id);

				sprintf_s(entry.ds->dt.player.anonymized_name, _XS("Player%i"), entry.hdr.m_ent_index + 6000);
			}
			else
			{
				strcpy(entry.ds->dt.player.info.name, _XS("dummie"));
			}
		}

		if (pl)
		{
			entry.ds->dt.player.obs_mode = entity->observer_mode();
			entry.ds->dt.player.obs_target_idx = entity->get_observer_target_idx();
		}

		if (entry.ds->dt.player.alive)
		{
			entry.ds->m_origin = entity->get_abs_origin();
			entry.ds->m_head = entity->get_bone_pos(2);
			get_eye_angles(entity, &entry.ds->dt.player.eye_angles);

			math::vec3_t pos_head, pos_origin = entry.ds->m_origin;
			math::vec3_t scr_head, scr_origin;

			pos_head = entity->get_bone_pos(7);

			entry.ds->dt.player.zooming = entity->is_zooming();

			if (respawn::world_to_screen(pos_head, scr_head) &&
				respawn::world_to_screen(pos_origin, scr_origin))
			{
				entry.ds->dt.player.scr_avail = true;
				entry.ds->dt.player.scr_head = scr_head;
				entry.ds->dt.player.scr_origin = scr_origin;
				entry.ds->dt.player.visible = is_opp_visible(g_ctx.my_ent_ptr, entity, 7);
				entry.ds->dt.player.health = entity->health();
				entry.ds->dt.player.health_max = entity->health_max();
				entry.ds->dt.player.armor = entity->armor();
				entry.ds->dt.player.armor_max = entity->armor_max();
			}

			auto shdr = entity->get_studio_hdr();
			
			if (shdr)
			{
				entry.ds->m_bone_count = min(shdr->num_bones, (int)entry.ds->m_bones.size());

				for (int i = 0; i < entry.ds->m_bone_count; i++)
				{
					auto& bone = entry.ds->m_bones[i];

					bone.pos = entity->get_bone_pos(i);
					bone.available = true;
					bone.visible = is_opp_visible(g_ctx.my_ent_ptr, entity, i);
					bone.onscreen = respawn::world_to_screen(bone.pos, bone.scr);
					
					if (auto obj = shdr->bone(i))
					{
						bone.parent = obj->parent;
						bone.name = obj->name();
					}
				}
			}

			for (int i = 0; i < 20; i++)
			{
				auto& hb = entry.ds->m_hitboxes[i];

				hb.pos = entity->get_hitbox_pos(i);
				hb.visible = is_opp_visible_pos(g_ctx.my_ent_ptr, entity, hb.pos);
				hb.onscreen = respawn::world_to_screen(hb.pos, hb.scr);
			}

			if (entry.ds_prev && entry.ds_prev->m_origin != math::vec3_t{0.f, 0.f, 0.f})
			{
				if (entry.ds_prev->m_origin != entry.ds->m_origin)
				{
					entry.ds->dt.player.time_last_moved = *g_gv.time;
				}
			}

			entry.ds->m_prev_hitboxes.insert( std::make_pair(entry.hdr.m_tick, entry.ds->m_hitboxes) );
		}
	}
}

void c_ent_cache::init()
{
	_objects.resize(0x10000);
	_objects_prev.resize(0x10000);
}

void c_ent_cache::update()
{
	auto current_tick = ix::globals->TickCount;

	for (int i = 0; i < ix::ent_list->GetHighestEntityIndex(); i++)
	{
		auto& entry = _objects[i];
		auto& prev = _objects_prev[i];

		auto entity = ix::ent_list->GetClientEntity(i);

		if (!entity)
		{
			entry.hdr.m_used = false;
			continue;
		}

		auto cache_reset = false;

		entry.hdr.m_used = true;
		entry.hdr.m_ent_index = i;
		entry.hdr.m_type = detect_entity_type(entity);
		entry.hdr.m_tick = entity->simulation_tick();

		if (!entry.hdr.m_pointer || entry.hdr.m_pointer != entity)
		{
			//
			// Invalidate our cached information.
			//

			entry.hdr.m_pointer = entity;
			
			if (entry.ds)
			{
				entry.ds->dt = { };

				delete entry.ds;
				entry.ds = nullptr;
			}

			if (entry.ds_prev)
			{
				entry.ds_prev->dt = { };

				delete entry.ds_prev;
				entry.ds_prev = nullptr;
			}

			cache_reset = true;
		}

		switch (entry.hdr.m_type)
		{
		case e_type::player:
			process_player(entry, prev, (respawn::c_player*)entity, true, cache_reset);
			break;
		case e_type::dummie:
			process_player(entry, prev, (respawn::c_player*)entity, false, cache_reset);
			break;
		default:
			entry.hdr.m_used = false;
			break;
		}

		if (entry.hdr.m_used)
		{
			prev = entry;

			if (entry.ds_prev)
				*entry.ds_prev = *entry.ds;
		}
	}
}

ImColor c_ent_cache::object_color(const cache::object_t& obj)
{
	switch (obj.hdr.m_type)
	{
	case cache::object_t::type::player:
	{
		if (obj.ds && (obj.ds->dt.player.online_drofnet_identity))
			return ImColor(52, 235, 232);

		return im::color::yellow;
	}
	case cache::object_t::type::dummie:
		return im::color::blue;
	default:
		return im::color::white;
	}
}
