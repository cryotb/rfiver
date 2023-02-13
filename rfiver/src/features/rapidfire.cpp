#include <include.h>

using namespace ft;

int last_tc;

void c_rapidfire::paint()
{
	if (!g_cfg.aim_rapidfire)
		return;

	ImGui::Begin("rapid fire");

	if (_boost_ticks > 0)
	{
		ImGui::Text("boosting...");
		ImGui::ProgressBar(utl::calc_percent(_boost_ticks, g_cfg.aim_rapidfire_ticks) / 100.f);
	}
	else
	{
		if ((utl::calc_percent((last_tc - _last_boost_stop_tick), 150) / 100.f) < 0.1f)
		{
			ImGui::ProgressBar(1.f);
		}
		else
		{
			ImGui::Text("recharging...");
			ImGui::ProgressBar((utl::calc_percent((last_tc - _last_boost_stop_tick), 150) / 100.f));
		}
	}

	ImGui::End();
}

void c_rapidfire::move(respawn::c_player* me, respawn::user_cmd_t* cmd)
{
	if (!g_cfg.aim_rapidfire)
		return;

	last_tc = cmd->tick_count;

	if (!_boosting)
	{
		if ((cmd->buttons & IN_ATTACK) && can_boost(cmd))
		{
			start_boosting();
			INGAME_LOGGER_MSG(2, im::color::white, "[rapidfire] start boosting!");
		}
	}
	else
	{
		if ( (cmd->buttons & IN_ATTACK) && respawn::can_fire(me))
		{
			++_boost_ticks;
			g_gv.game_time_scale->scale = g_cfg.aim_rapidfire_scale;
		}
		else
		{
			g_gv.game_time_scale->scale = 1.f;
		}

		if (_boost_ticks > g_cfg.aim_rapidfire_ticks)
		{
			_boost_ticks = 0;
			_last_boost_stop_tick = cmd->tick_count;
			stop_boosting();

			INGAME_LOGGER_MSG(2, im::color::white, "[rapidfire] cooldown reached!");
		}
	}
}

bool c_rapidfire::can_boost(respawn::user_cmd_t* cmd)
{
	if (_last_boost_stop_tick != 0 && (cmd->tick_count - _last_boost_stop_tick) < 150)
		return false;

	return !_boosting && _boost_ticks < g_cfg.aim_rapidfire_ticks;
}


void c_rapidfire::start_boosting()
{
	g_gv.game_time_scale->scale = g_cfg.aim_rapidfire_scale;
	_boosting = true;
}

void c_rapidfire::stop_boosting()
{
	g_gv.game_time_scale->scale = 1.f;
	_boosting = false;
}
