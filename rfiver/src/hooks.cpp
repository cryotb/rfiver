#include <include.h>

bool hk::start_early()
{
	_msg("early hooks!");

	if (!mh::init())
	{
		msg("failed init: mhpp");
		return false;
	}

	return true;
}

bool hk::start()
{
	vmt::input_system = new vmthk(ix::input_system);
	vmt::swap_chain = new vmthk(ix::swap_chain);

	vmt::swap_chain->place(8, sc_present);
	vmt::input_system->place(27, sleep_until_input);

	vmt::swap_chain->enable();
	vmt::input_system->enable();

	auto gamesdk_base = MOD_BASE("gamesdk.dll");

	if (!gamesdk_base)
	{
		msg("failed to find gamesdk image base!");
		return false;
	}

	auto gamesdk_len = get_pe_length(PTR_OF(gamesdk_base));

	if (!gamesdk_len)
	{
		msg("failed to find gamesdk image length!");
		return false;
	}

	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x703A70)), create_move_input, &o_create_move_input);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x5d4740)), view_render_start, &o_view_render_start);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x8580b0)), get_recoil, &o_get_recoil);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x2C1830)), cl_move_send, &o_cl_move_send);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x5C0740)), frame_stage_notify, &o_frame_stage_notify);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x705330)), write_ucmd_delta_to_buf, &o_write_ucmd_delta_to_buf);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0xCDB260)), on_take_damage, &o_on_take_damage);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x101CD40)), on_take_ammo, &o_on_take_ammo);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x1E5F00)), find_mdl, &o_find_mdl);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x7DBE00)), set_mdl_idx, &o_set_mdl_idx);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x96D940)), update_script_ent_data, &o_update_script_ent_data);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x28DDB0)), process_playlist_override, &o_process_playlist_override);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x28CB00)), process_string_cmd, &o_process_string_cmd);

	if (IS_DEBUG_BUILD())
	{
		// DevMsg causes crashes sometimes on death, so only in dbg mode.
		mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x1057FD0)), dev_msg, &o_dev_msg);
	}
	
	mh::place(PTR_OF(resolve_addr_to_rel(
		find_pattern(gamesdk_base, gamesdk_len,
			"E8 ? ? ? ? 8B 87 ? ? ? ? 33 ED 89 44 24"), 1, 5
	)), gamesdk_draw_imgui, &o_gamesdk_draw_imgui);

	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x5BC830)), level_shutdown, &o_level_shutdown);
	mh::place(utl::get_vtable_func(ix::engine_client, 24), get_player_info, &o_get_player_info);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0xA524F0)), asm_tramp_x_fire_weapon_def, &o_fire_weapon_def);
	mh::place(PTR_OF((MOD_BASE("r5apex.exe") + 0x263E70)), net_set_key, &o_net_set_key);

	if (auto addr = find_pattern(gamesdk_base, gamesdk_len, "0F 86 ? ? ? ? 48 39 75 B7 0F 84"))
	{
		//
		// Allows banned clients to connect on locally hosted servers.
		//

		if (!utl::patch_prot(addr, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }) ||
			!utl::patch_prot(addr + 0xA, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }))
			return false;
	}

	if (!utl::patch_prot(MOD_BASE("r5apex.exe") + 0x7188AF, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }) ||
		!utl::patch_prot(MOD_BASE("r5apex.exe") + 0x7188C1, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }) ||
		!utl::patch_prot(MOD_BASE("r5apex.exe") + 0x7188C9, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }))
	{
		msg("failed to patch client-silent!");
		return false;
	}

	if (auto addr = find_pattern(
		MOD_BASE("r5apex.exe"),
		get_pe_length(PTR_OF(MOD_BASE("r5apex.exe"))), "75 52 48 8B 85 ? ? ? ? 48 85 C0"))
	{
		msg("found multi client addr => %p", addr);

		if (!utl::patch_prot(addr, { 0xEB, 0x52 }))
		{
			msg("failed to patch multi client!");
			return false;
		}
	}

	o_wnd_proc = SetWindowLongPtr(g::game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hk::wnd_proc));

	return true;
}

bool hk::srv::start()
{
	auto dedicated_base = MOD_BASE("dedicated.dll");

	if (!dedicated_base)
	{
		msg("failed to find dedicated image base!");
		return false;
	}

	auto dedicated_len = get_pe_length(PTR_OF(dedicated_base));

	if (!dedicated_len)
	{
		msg("failed to find dedicated image length!");
		return false;
	}

	if (auto addr = find_pattern(dedicated_base, dedicated_len, "0F 86 ? ? ? ? 48 39 75 B7 0F 84")) // Allow Banned Clients #1
	{
		if (!utl::patch_prot(addr, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }) ||
			!utl::patch_prot(addr + 0xA, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }))
			return false;
	}

	if (auto addr = find_pattern(dedicated_base, dedicated_len, "0F 85 ? ? ? ? E8 ? ? ? ? 4C 89 6C 24")) // Allow Banned Clients #2
	{
		if (!utl::patch_prot(addr, { 0xE9, 0xC0, 0x00, 0x00, 0x00 }) ||
			!utl::patch_prot(addr + 0x6, { 0x90 }))
			return false;
	}

	//
	// put these as always on cause server has no cfg/gui.
	//
	g_cfg.host_god_mode = true;
	g_cfg.host_infinite_ammo = true;

	mh::place(PTR_OF((MOD_BASE("r5apex_ds.exe") + 0xCDB260)), on_take_damage, &o_on_take_damage);
	mh::place(PTR_OF((MOD_BASE("r5apex_ds.exe") + 0x101CD40)), on_take_ammo, &o_on_take_ammo);

	return true;
}
