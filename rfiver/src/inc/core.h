#pragma once

namespace core
{
	extern bool d3d_inited;
	extern bool is_server;

	extern void game_vars();
	extern void iface();
	extern bool cvar();

	extern bool init();

	extern void on_graphics();

	extern void start();
	extern void render();
	extern void move(respawn::user_cmd_t* cmd);
}
