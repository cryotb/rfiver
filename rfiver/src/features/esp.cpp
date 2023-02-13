#include <include.h>

using namespace ft;

bool is_within_range(float cap, float distance)
{
	if (cap > 0.f)
		return (distance <= cap);

	return true;
}

void draw_rect_bordered(ImColor sColor, float flX0, float flY0, float flX1, float flY1)
{
	render::rect({ flX0 - 1.0f, flY0 - 1.0f }, { flX1 + 1.0f, flY1 + 1.0f }, ImColor(5, 5, 5));
	render::rect({ flX0, flY0 }, { flX1, flY1 }, sColor);
	render::rect({flX0 + 1.0f, flY0 + 1.0f}, { flX1 - 1.0f, flY1 - 1.0f }, ImColor(5, 5, 5));
}

void draw_outline(ImColor color, ImVec2 scr_origin, float bw, float bh)
{
	draw_rect_bordered(color, scr_origin.x - bw, scr_origin.y - bh, scr_origin.x + bw, scr_origin.y + 5.f);
}

void draw_bar(ImColor color, FLOAT flX, FLOAT flY, FLOAT flW, FLOAT flH, int current, const int max, bool bg = true)
{
	if (current < 0)
		current = 0;

	if (current > max)
		current = max;

	flY -= flH;

	const float base = flH / static_cast<float>(max) * static_cast<float>(current);
	const float delta = (flH - base);

	const float bar_x = flX;

	if(bg) render::rect_filled({ bar_x, flY }, { bar_x + 5.0f, flY + flH }, ImColor( 5, 5, 5 ));

	render::rect_filled({ bar_x, flY + delta }, { bar_x + 5.0f, flY + flH }, color);
	if(bg) render::rect({ bar_x, flY }, { bar_x + 5.0f, flY + flH }, (5, 5, 5));
}

void draw_bar_vert(ImColor color, FLOAT flX, FLOAT flY, FLOAT flW, FLOAT flH, int current, const int max, bool bg = true)
{
	if (current < 0)
		current = 0;

	if (current > max)
		current = max;

	flW *= 2.f;

	const float base = flW / static_cast<float>(max) * static_cast<float>(current);
	const float delta = (flW - base);

	const float bar_x = flX + 5.f;
	const float bar_y = flY + 2.f;
	const float height = 5.f;

	if (bg) render::rect_filled({ bar_x, bar_y }, { bar_x + flW, bar_y + height }, ImColor(5, 5, 5));

	render::rect_filled({ bar_x, bar_y }, { bar_x + flW - delta, bar_y + height }, color);

	if (bg) render::rect({ bar_x, bar_y }, { bar_x + flW, bar_y + height }, (5, 5, 5));
}

const cache::bone_t& get_bone_pos(const cache::object_t& obj, const char* name)
{
	for (int i = 0; i < obj.ds->m_bone_count; i++)
	{
		const auto& bone = obj.ds->m_bones[i];

		if (bone.name == name)
			return bone;
	}

	_dbg("bone has not been found -> %s", name);

	return obj.ds->m_bones.front();
}

auto link_bones(const cache::object_t& obj, const char* namea, const char* nameb)
{
	auto a = get_bone_pos(obj, namea);
	auto b = get_bone_pos(obj, nameb);

	const auto& first = a.scr;
	const auto& second = b.scr;

	if (!a.onscreen || !b.onscreen)
		return;

	render::line(
		{ first.x, first.y },
		{ second.x, second.y },
		im::color::white
	);
}

void c_esp::process_player(const cache::object_t& obj, bool pl)
{
	if (!CE_IS_VALID(obj)) return;
	if (!obj.ds->dt.player.alive) return;
	if (!obj.ds->m_bones[12].onscreen) return;

	math::vec3_t head_ws;

	if (g_cfg.esp_head_pos)
	{
		if (respawn::world_to_screen(obj.ds->m_head, head_ws))
		{
			render::rect({ head_ws.x - 5, head_ws.y - 5 }, { head_ws.x + 5, head_ws.y + 5 }, ImColor(65, 255, 65));
		}
	}

	auto& cache_pl = obj.ds->dt.player;

	const auto bounds_height = fabsf(cache_pl.scr_head.y - cache_pl.scr_origin.y) * 1.5f;
	const auto bounds_width = (bounds_height / 2.5f);

	float distance = 0.f;

	if (auto lp = g_ent_cache.local_object())
	{
		if (CE_IS_VALID_PTR(lp))
		{
			distance = obj.ds->m_origin.DistanceTo(lp->ds->m_origin) / 4096.f;
		}
	}

	if (!is_within_range(g_cfg.esp_render_dist, distance))
		return;

	const auto draw_pos_top_x = cache_pl.scr_origin.x;
	const auto draw_pos_top_y = cache_pl.scr_origin.y - (bounds_height + 20.f);
	float draw_pos_top_push = 0.f;

	if(CE_IS_TYPE_PLAYER(obj))
	{
		if (g_cfg.esp_name)
		{
			render::text_centered_bg(
				{ draw_pos_top_x,
				draw_pos_top_y - draw_pos_top_push },
				ImColor(255, 255, 255),
				obj.ds->dt.player.info.name
			);
			draw_pos_top_push += 15.f;
		}

		if ((*g_gv.time - obj.ds->dt.player.time_last_moved) > 7.f)
		{
			render::text_centered_bg(
				{ draw_pos_top_x,
				draw_pos_top_y - draw_pos_top_push },
				im::color::yellow,
				"AFK"
			);
			draw_pos_top_push += 15.f;
		}

		if (g_cfg.esp_health_bar)
		{
			draw_bar_vert(
				im::color::green, cache_pl.scr_origin.x - bounds_width - 6.f,
				cache_pl.scr_origin.y, bounds_width, bounds_height,
				cache_pl.health, cache_pl.health_max
			);

			if (cache_pl.armor_max != 0 && cache_pl.armor > 0)
			{
				draw_bar_vert(
					im::color::blue, cache_pl.scr_origin.x - bounds_width - 6.f,
					cache_pl.scr_origin.y, bounds_width, bounds_height,
					cache_pl.armor, cache_pl.armor_max, false
				);
			}
		}
	}

	if (g_cfg.esp_outline)
		draw_outline(g_ent_cache.object_color(obj), { cache_pl.scr_origin.x, cache_pl.scr_origin.y }, bounds_width, bounds_height);

	if (g_cfg.esp_sight_line)
	{
		math::vec3_t aim_position{};
		const math::ang3_t view_angle = cache_pl.eye_angles;

		math::AngleVectors(view_angle, aim_position);

		aim_position *= 200.0f;

		auto begin = obj.ds->m_hitboxes[0].pos;
		auto end = begin + aim_position;

		math::vec3_t begin_screen{}, end_screen{};

		if (respawn::world_to_screen(begin, begin_screen) && 
			respawn::world_to_screen(end, end_screen))
		{
			render::line(
				{ begin_screen.x, begin_screen.y }, 
				{ end_screen.x, end_screen.y }, 
				im::color::blue
			);
		}
	}

	if(g_cfg.esp_bones)
	{
		link_bones(obj, "def_c_head", "def_c_neckA");
		link_bones(obj, "def_c_neckA", "def_l_shoulder");
		link_bones(obj, "def_l_elbow", "def_l_wrist");
		link_bones(obj, "def_c_neckA", "def_r_shoulder");
		link_bones(obj, "def_r_shoulder", "def_r_elbow");
		link_bones(obj, "def_r_elbow", "def_r_wrist");
		link_bones(obj, "def_c_neckA", "def_c_spineB");
		link_bones(obj, "def_c_spineB", "def_l_thigh");

		link_bones(obj, "def_l_thigh", "def_l_knee");
		link_bones(obj, "def_c_spineB", "def_r_thigh");
		link_bones(obj, "def_c_spineB", "def_l_thigh");

		link_bones(obj,  "def_c_head",  "def_c_neckA");

		//left arm
		link_bones(obj,  "def_c_neckA",  "def_l_shoulder");
		link_bones(obj,  "def_l_shoulder",  "def_l_elbow");
		link_bones(obj,  "def_l_elbow",  "def_l_wrist");

		//right arm
		link_bones(obj,  "def_c_neckA",  "def_r_shoulder");
		link_bones(obj,  "def_r_shoulder",  "def_r_elbow");
		link_bones(obj,  "def_r_elbow",  "def_r_wrist");

		//spine
		link_bones(obj,  "def_c_neckA",  "def_c_spineB");

		//left leg
		link_bones(obj,  "def_c_spineB",  "def_l_thigh");
		link_bones(obj,  "def_l_thigh",  "def_l_knee");
		link_bones(obj,  "def_l_knee",  "def_l_ankle");

		//right leg
		link_bones(obj,  "def_c_spineB",  "def_r_thigh");
		link_bones(obj,  "def_r_thigh",  "def_r_knee");
		link_bones(obj,  "def_r_knee",  "def_r_ankle");
	}
}

void c_esp::paint()
{
	if(true)
	{
		float pad_x = 5;
		float push_y = 200.f;

		if (wnds::menu.is_visible())
		{
			/*if (g_drof_net.is_synced())
			{
				render::text({ pad_x, 5.f + push_y }, im::color::green , "drofnet: synced (%ims)", g_drof_net.get_latency()); push_y += 20.f;
			} else
				render::text({ pad_x, 5.f + push_y }, im::color::yellow, "drofnet: offline (-1ms)"); push_y += 20.f;*/
		}

		if (IS_DEBUG_BUILD())
		{
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "ingame=%i", ix::engine_client->is_in_game()); push_y += 20.f;
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "base=%p", &__ImageBase); push_y += 20.f;
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "local_player=%p", g_ctx.my_ent_ptr); push_y += 20.f;

			if (g_ctx.my_ent_ptr)
			{
				render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "studio_hdr=%p", g_ctx.my_studio); push_y += 20.f;
			}

			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "local_weapon=%p", g_ctx.my_wpn_ptr); push_y += 20.f;
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "can_fire=%i", g_ctx.can_fire); push_y += 20.f;
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "firing=%i", (g_ctx.last_user_cmd.buttons & IN_ATTACK) == 1); push_y += 20.f;
			auto x = respawn::get_main_thread_id();
			render::text({ pad_x, 5.f + push_y }, ImColor(255, 255, 255), "r5_thr_main=%i", x); push_y += 20.f;
		}
	}

	if (!g_cfg.esp_active || g::rce.force_disable_esp) return;

	for (const auto& obj : g_ent_cache.objects())
	{
		if (!obj.hdr.m_used)
			continue;

		if (obj.hdr.m_type != cache::object_t::type::dummie &&
			obj.hdr.m_type != cache::object_t::type::player)
			continue;

		if (!respawn::is_third_person() && obj.hdr.m_ent_index == g_ctx.my_ent_idx)
			continue;

		process_player(obj, (obj.hdr.m_type == cache::object_t::type::player));
	}
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

bool custom_highlight(respawn::c_entity* ent, float r, float g, float b)
{
	if (!g_cfg.esp_glow)
	{
		reinterpret_cast<void(__fastcall*)(void*, int)>(MOD_BASE("r5apex.exe") + 0x7D5150)(ent, 0);
		return false;
	}

	float clr[] = { r, g, b };

	highlight::show_inside(ent, 1.f);
	highlight::show_outline(ent, 1.f);

	highlight::set_funcs(ent, (highlight::CONTEXT)0, 114, true, 125, 2.0, 2, false);
	highlight::set_param(ent, (highlight::CONTEXT)0, 0, clr);

	reinterpret_cast<void(__fastcall*)(void*, int)>(MOD_BASE("r5apex.exe") + 0x7D5150)(ent, 7);

	auto val = *reinterpret_cast<u32*>(MOD_BASE("r5apex.exe") + 0xd4ec368);

	*reinterpret_cast<float*>(BASE_OF(ent) + 0x2E8) = ix::globals->Curtime;
	*reinterpret_cast<u32*>(BASE_OF(ent) + 0x88) = val;

	return highlight::enable(ent);
}

void c_esp::move(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	for (const auto& obj : g_ent_cache.objects())
	{
		if (!obj.hdr.m_used)
			continue;

		if (obj.hdr.m_type != cache::object_t::type::dummie &&
			obj.hdr.m_type != cache::object_t::type::player)
			continue;

		if (!respawn::is_third_person() && obj.hdr.m_ent_index == g_ctx.my_ent_idx)
			continue;

		if (!obj.ds->dt.player.alive)
			continue;

		custom_highlight((respawn::c_entity*)obj.hdr.m_pointer, 0.f, 0.f, 0.f);
	}
}
