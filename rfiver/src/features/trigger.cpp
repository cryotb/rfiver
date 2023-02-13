#include <include.h>

using namespace ft;

EXTERN_C u64 fn_trace_ray = 0;
EXTERN_C void __trace_ray(void* inst, void* ray, int mask, void* filter, void* trace);

void make_fake_inst(u8* buf, u64* vft, u64* vte, u64 vtl)
{
	*(u64*)(&buf[0]) = BASE_OF(vft);

	for (u64 i = 0; i < vtl; i++)
		vft[i] = vte[i];
}

//
void filter_fallback(void* inst) { __noop(); };
bool filter_should_hit_entity(void* inst, void* ent, int cmask) // 1
{
	if (ent == *(void**)(BASE_OF(inst) + 0x8))
		return false;

	return true;
}
int filter_get_trace_type(void* inst) { return 0; /* TRACE_EVERYTHING */ }
bool filter_unk2(void* inst, void* a1, void* a2) { return false; }

u8 filter_inited = 0;
u8 filter_inst[256];
u64 filter_vft[6];
void c_trigger::move(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (!filter_inited)
	{
		msg("initing filter %p", BASE_OF(&filter_inited));

		u64 filter_vfs[] =
		{
			BASE_OF(filter_fallback),
			BASE_OF(filter_should_hit_entity),
			BASE_OF(filter_get_trace_type),
			BASE_OF(filter_unk2),
		};

		make_fake_inst(filter_inst, filter_vft, filter_vfs, 4);

		filter_inited = 1;
	}

	if (!g_cfg.trigger_active)
		return;

	m_target_found = false;

	if (!me->is_alive())
		return;

	auto vm = me->get_view_model();
	auto wpn = me->get_active_weapon();

	if (!vm || !wpn)
		return;

	auto wpn_def = respawn::get_weapon_def(vm, wpn);
	auto wpn_type = respawn::get_weapon_type(wpn_def);

	if (!g_cfg.trigger_global)
	{
		bool pass = false;

		if (wpn_type == respawn::weapon_types::shotgun && g_cfg.trigger_shotgun)
			pass = true;

		if (wpn_type == respawn::weapon_types::sniper && g_cfg.trigger_sniper)
			pass = true;

		if (!pass) return;
	}

	math::vec3_t aim_position{};
	const math::ang3_t view_angle = me->eye_angles();

	math::AngleVectors(view_angle, aim_position);

	aim_position *= 8192.f;

	auto begin = me->view_offset();
	auto end = begin + aim_position;

	respawn::Ray_t ray;
	respawn::trace_t trace;

	memset(&ray, 0, sizeof(ray));
	memset(&trace, 0, sizeof(trace));

	ray.Init(begin, end);

	*(void**)(BASE_OF(filter_inst) + 0x8) = me;

	ix::engine_trace->trace_ray(ray, 0x46404033, &filter_inst, &trace);

	respawn::debug_draw_box(trace.endpos, { -5.f, -5.f, -5.f }, { 5.f, 5.f, 5.f }, 255, 255, 255, 255, 0.01f);

	if (trace.m_pEnt != nullptr && g_ctx.can_fire)
	{
		if (!(trace.hitgroup == 2 || trace.hitgroup == 3 || trace.hitgroup == 4))
		{
			return;
		}
		auto ent = reinterpret_cast<respawn::c_player*>(trace.m_pEnt);
		auto ent_type = respawn::detect_entity_type((respawn::c_entity*)trace.m_pEnt);

		switch (ent_type)
		{
		case cache::object_t::type::player:
		case cache::object_t::type::dummie:
		{
			m_target_found = true;
			cmd->buttons |= IN_ATTACK;
			break;
		}
		default:
			break;
		}
	}
}
