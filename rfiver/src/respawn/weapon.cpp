#include <include.h>

using namespace respawn;

bool c_weapon::can_fire(c_player* owner) noexcept
{
	auto vm = owner->get_view_model();
	if (!vm) return false;

	auto def = respawn::get_weapon_def(vm, this);
	auto np_attack = next_primary_attack();
	auto current_time = owner->server_time();

	if (ammo_in_clip() <= 0)
		return false;

	if (is_reloading())
		return false;

	if (!cvar::move_use_dt->m_value_numeric)
		return true;

	if (def == weapons::sn_charge && f::aim->is_silent() && g_cfg.aim_psilent)
	{
		if (g_ctx.last_user_cmd.buttons & IN_ATTACK)
		{
			if (g_ctx.packets_choked > 5)
				return false;
		}

		return true;
	}

	return (np_attack <= current_time);
}
