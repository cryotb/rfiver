#include <include.h>

using namespace ft;

bool g_hopping{ };
void bunnyhop(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (g_hopping)
	{
		if (!(cmd->buttons & IN_JUMP))
		{
			g_hopping = false;
			return;
		}

		if (!(me->flags() & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
	else
	{
		if (cmd->buttons & IN_DUCK)
			g_hopping = true;
	}
}

void thirdperson()
{
	static bool previous_enabled{ };
	static bool is_toggled{ };
	static int initial_value{ };

	if (!initial_value)
		initial_value = cvar::third_person->m_value_numeric;

	const auto maintain_value = []()
	{
		if (g_cfg.visual_third_person && is_toggled)
			cvar::third_person->m_value_numeric = 1;
		else
			cvar::third_person->m_value_numeric = initial_value;
	};

	if (previous_enabled != g_cfg.visual_third_person)
	{
		maintain_value();
		previous_enabled = g_cfg.visual_third_person;
	}

	if (si::create_move.was_button_down(VK_XBUTTON1))
	{
		is_toggled = !is_toggled;
		maintain_value();
	}
}

void airstuck()
{
	static bool is_toggled{ };
	static int initial_value{ };

	if (!initial_value)
		initial_value = cvar::move_use_dt->m_value_numeric;

	const auto maintain_value = []()
	{
		bool wants_fire = false;

		if (auto lp = respawn::local_player())
		{
			if (lp->is_alive())
			{
				if (auto lw = lp->get_active_weapon())
				{
					wants_fire = lw->can_fire(lp) && (g_ctx.last_user_cmd.buttons & IN_ATTACK);
				}
			}
		}

		if (is_toggled && !wants_fire)
			cvar::move_use_dt->m_value_numeric = 0;
		else
			cvar::move_use_dt->m_value_numeric = initial_value;
	};

	if (si::create_move.was_button_down(VK_NUMPAD4))
	{
		is_toggled = !is_toggled;
		maintain_value();
	}

	maintain_value();
}

void anti_aim(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (!g_cfg.other_anti_aim)
		return;

	auto my_wpn = me->get_active_weapon();

	if (!my_wpn) return;
	if ( (cmd->buttons & IN_ATTACK) && my_wpn->can_fire(me)) return;

	cmd->viewangles.x = cmd->tick_count % 2 ? -89.f : +89.f;
	cmd->viewangles.y = static_cast<float>(cmd->tick_count * 55 % 360);

	math::ClampAngle(cmd->viewangles);
}

void fake_lag(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (!g_cfg.other_fake_lag)
		return;

	auto my_wpn = me->get_active_weapon();

	if (!my_wpn) return;
	if (!my_wpn) return;
	if ((cmd->buttons & IN_ATTACK) && my_wpn->can_fire(me)) return;

	int limit = g_cfg.other_fake_lag_base;

	if ( (me->flags() & FL_ONGROUND) == 0)
		limit = 5;

	if (g_ctx.packets_choked < limit)
		g_ctx.should_choke = true;
}

void c_misc::move(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (me->is_alive())
	{
		airstuck();
		bunnyhop(me, cmd);
	}

	thirdperson();
}

void c_misc::move_unsafe(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	anti_aim(me, cmd);
	fake_lag(me, cmd);
}

void c_misc::paint()
{

}
