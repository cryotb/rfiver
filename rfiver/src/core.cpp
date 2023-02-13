#include <include.h>

bool core::d3d_inited{ };
bool core::is_server{ };

bool core::init()
{
	g::game_base = MOD_BASE(nullptr);
	g::game_length = get_pe_length( PTR_OF(g::game_base) );
	g_gtc = new c_game_thread_callbacks();

	f::mgr = new std::remove_pointer<decltype(f::mgr)>::type();
	f::esp = new std::remove_pointer<decltype(f::esp)>::type();
	f::aim = new std::remove_pointer<decltype(f::aim)>::type();
	f::rapidfire = new std::remove_pointer<decltype(f::rapidfire)>::type();
	f::misc = new std::remove_pointer<decltype(f::misc)>::type();
	f::trigger = new std::remove_pointer<decltype(f::trigger)>::type();
	f::specs = new std::remove_pointer<decltype(f::specs)>::type();

	if (GADGET_TRAMP_BEGIN = find_pattern(g::game_base, g::game_length, "ff e0"))
		msg("found gadget (begin) at -> %p", GADGET_TRAMP_BEGIN);

	if (GADGET_TRAMP_END = find_pattern(g::game_base, g::game_length, "ff e3"))
		msg("found gadget (end) at -> %p", GADGET_TRAMP_END);

	//g_drof_net.start();

	if (!is_server)
	{
		g::sdk_base = MOD_BASE("gamesdk.dll");
		g::sdk_length = get_pe_length(PTR_OF(g::sdk_base));

		ImGui::CreateContext();

		g_ctx.reset();
		g_ent_cache.init();
		wnds::init();

		do
		{
			g::game_hwnd = FindWindowA("Respawn001", NULL);
		} while (!g::game_hwnd);

		if (!g::game_hwnd)
		{
			msg("failed init: game_hwnd");
			return false;
		}

		game_vars();
		iface();

		if (!cvar())
		{
			msg("failed init: cvar");
			return false;
		}

		ix::mat_surface->get_screen_size(g::screen.w, g::screen.h);
		msg("screen width=%i, height=%i", g::screen.w, g::screen.h);

		f::start();

		if (!g::netvar_mgr.initialize())
		{
			msg("failed init: nvmgr");
			return false;
		}

		if (!hk::start())
		{
			msg("failed init: hk");
			return false;
		}

		std::thread(&game_context_t::think, &g_ctx).detach();

		INGAME_LOGGER_MSG(5, im::color::green, "[drofhack] initialized with success, enjoy.");

		cfg::load(&g_cfg, _XS("main.cfg"));
	}
	else
	{
		if (!hk::srv::start())
		{
			msg("failed init: hk");
			return false;
		}
	}

	return true;
}

void core::on_graphics()
{
	if (ix::d3d_dev && ix::d3d_dev_ctx)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		core::render();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void core::game_vars()
{
	g_gv.time = reinterpret_cast<float*>(MOD_BASE("r5apex.exe") + 0xd40bad0);
	g_gv.time_scale = reinterpret_cast<float*>(MOD_BASE("r5apex.exe") + 0x1740d18);
	g_gv.net_chan = reinterpret_cast<respawn::netchan_t**>(MOD_BASE("r5apex.exe") + 0x180A0F0);
	g_gv.game_time_scale = reinterpret_cast<game_time_scale_t*>(MOD_BASE("r5apex.exe") + 0x1843328);
	g_gv.server_list_mgr = *reinterpret_cast<respawn::c_srv_list_mgr**>(
		resolve_addr_to_rel(find_pattern(g::sdk_base, g::sdk_length, 
			"48 89 1D ? ? ? ? 48 8B 5C 24 ? 48 83 C4 20 5F C3"), 
			3, 7));
	g_gv.signon_state = reinterpret_cast<int*>(MOD_BASE("r5apex.exe") + 0x180A128);

	LOG_VAR(g_gv.time);
	LOG_VAR(g_gv.time_scale);
	LOG_VAR(g_gv.net_chan);
	LOG_VAR(g_gv.server_list_mgr);
	LOG_VAR(g_gv.signon_state);
}

void core::iface()
{
	ix::swap_chain = *reinterpret_cast<IDXGISwapChain**>(MOD_BASE("r5apex.exe") + r5::offs::d3d_swap_chain);
	ix::base_client = reinterpret_cast<respawn::c_base_client*>(respawn::create_iface("VClient018", 0));
	ix::ent_list = reinterpret_cast<respawn::c_entity_list*>(respawn::create_iface("VClientEntityList003", 0));
	ix::view_render = *reinterpret_cast<respawn::c_view_render**>(MOD_BASE("r5apex.exe") + r5::offs::view_render);
	ix::engine_client = *reinterpret_cast<respawn::c_engine_client**>(MOD_BASE("r5apex.exe") + r5::offs::engine_client);
	ix::engine_trace = reinterpret_cast<respawn::c_engine_trace*>(respawn::create_iface("EngineTraceClient004", 0));
	ix::mat_surface = reinterpret_cast<respawn::c_mat_system_surface*>(MOD_BASE("r5apex.exe") + r5::offs::mat_surface);
	ix::input_system = *reinterpret_cast<respawn::c_input_system**>(MOD_BASE("r5apex.exe") + r5::offs::input_system);
	ix::model_info = *reinterpret_cast<respawn::c_model_info**>(MOD_BASE("r5apex.exe") + r5::offs::model_info);
	ix::globals = *reinterpret_cast<respawn::c_global_vars**>(MOD_BASE("r5apex.exe") + r5::offs::globals);
	ix::cvar = *reinterpret_cast<respawn::c_convar_mgr**>(MOD_BASE("r5apex.exe") + r5::offs::cvar);

	LOG_VAR(ix::swap_chain);
	LOG_VAR(ix::base_client);
	LOG_VAR(ix::ent_list);
	LOG_VAR(ix::view_render);
	LOG_VAR(ix::engine_client);
	LOG_VAR(ix::engine_trace);
	LOG_VAR(ix::mat_surface);
	LOG_VAR(ix::input_system);
	LOG_VAR(ix::model_info);
	LOG_VAR(ix::globals);
	LOG_VAR(ix::cvar);
}

bool core::cvar()
{
	if ( (cvar::srvcheats = ix::cvar->find(_XS("sv_cheats"))) == nullptr ) return false;
	if ( (cvar::move_use_dt = ix::cvar->find(_XS("cl_move_use_dt"))) == nullptr ) return false;
	if ( (cvar::third_person = ix::cvar->find(_XS("thirdperson_override"))) == nullptr ) return false;

	return true;
}

void core::start()
{
	if(IS_DEBUG_BUILD())
		utl::create_console();

	char proc_path[512];
	is_server = false;

	if (!GetModuleFileName((HMODULE)PTR_OF(g::game_base), proc_path, sizeof(proc_path)))
	{
		MessageBox(0, "failed to get proc path!", 0, 0);
		return;
	}

	if (strstr(proc_path, "r5apex_ds.exe")) is_server = true;
	_msg("%s", proc_path);

	hk::start_early();

	if (!is_server)
	{
		while (!utl::is_module_present(_XS("ncryptsslp.dll")))
		{
			_msg("waiting....");
			Sleep(1000);
		}
	}

	_msg("startup!");

	if (!init())
		return;

	_msg("inited! (base=%p, length=%llx)", g::self.base, g::self.length);
}

void core::render()
{
	g_ctx.is_ingame = ix::engine_client->is_in_game();

	if (g_ctx.is_ingame_prev != g_ctx.is_ingame)
	{
		INGAME_LOGGER_MSG(5, im::color::white, "game state has changed (%i => %i)", g_ctx.is_ingame_prev, g_ctx.is_ingame);

		g_ctx.is_ingame_prev = g_ctx.is_ingame;
	}

	g_input_mgr.update();
	g_input_mgr.sync(&si::paint);

	render::debug_queue();

	if (auto lp = respawn::local_player())
	{
		g::game_time = lp->server_time();
	}

	f::mgr->on_paint();
	g_window_mgr.on_paint();
	c_spec_mgr::single()->on_paint();
}

void core::move(respawn::user_cmd_t* cmd)
{
	g_input_mgr.sync(&si::create_move);

	if (!cmd || !cmd->command_number)
	{
		g_ctx.reset();
		return;
	}

	g_ctx.curr_user_cmd = *cmd;

	auto me = respawn::local_player();

	if (!me)
	{
		g_ctx.reset();
		return;
	}

	g_ctx.my_ent_idx = ix::engine_client->get_local_player_idx();
	g_ctx.my_ent_ptr = me;
	g_ctx.my_studio = me->get_studio_hdr();

	if (me->is_alive())
	{
		g_ctx.fov = reinterpret_cast<float(__thiscall*)(void*)>(MOD_BASE("r5apex.exe") + 0x861560)(me);
	}

	g_ent_cache.update();
	f::mgr->on_move(me, cmd);

	if (g_cfg.visual_fov_scale_custom && g_cfg.visual_remove_recoil)
	{
		if (cmd->buttons & IN_ATTACK)
		{
			auto recoil = me->view_recoil();

			auto comp_scale = (g_cfg.visual_recoil_comp_scale / 100.f);

			cmd->viewangles.x -= recoil.x * comp_scale;
			cmd->viewangles.y -= recoil.y * comp_scale;
			cmd->viewangles.z -= recoil.z * comp_scale;
		}
	}

	g_ctx.last_user_cmd = *cmd;
}