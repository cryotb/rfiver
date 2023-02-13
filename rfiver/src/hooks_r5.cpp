#include <include.h>

extern "C"
{
	void* o_fire_weapon_def{ };

	void hk_x_fire_weapon_def(
		void* inst,
		math::vec3_t* pos,
		math::vec3_t* dir,
		__int64 a4,
		int a5,
		__int64 a6,
		char ignore_spread)
	{
		auto self = respawn::local_player();

		if (!self || !self->is_alive())
			return;

		if (g_cfg.aim_sync_bullets && f::aim->m_aiming)
		{
			*pos = f::aim->m_aim_pos;
		}
	}
}

long long __fastcall hk::wnd_proc(HWND hwnd, UINT xmsg, WPARAM wp, LPARAM lp)
{
	ImGuiIO& io = ImGui::GetIO();

	if (wnds::menu.is_visible())
	{
		*(bool*)(MOD_BASE("r5apex.exe") + 0x22c75da) = true;

		if (ImGui_ImplWin32_WndProcHandler(hwnd, xmsg, wp, lp))
			return TRUE;
	}
	else
	{
		*(bool*)(MOD_BASE("r5apex.exe") + 0x22c75da) = false;
	}

	g_input_mgr.window_proc(xmsg, wp, lp);

	return CallWindowProc(reinterpret_cast<WNDPROC>(o_wnd_proc), hwnd, xmsg, wp, lp);
}

long __fastcall hk::sc_present(IDXGISwapChain* inst, uint32_t sync_int, uint32_t flags)
{
	if (!core::d3d_inited)
	{
		msg("==== d3d init ====");

		if (SUCCEEDED(inst->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&ix::d3d_dev))))
		{
			ix::d3d_dev->GetImmediateContext(&ix::d3d_dev_ctx);

			msg("device_ptr => %p", ix::d3d_dev);
			msg("device_ctx => %p", ix::d3d_dev_ctx);

			if (SUCCEEDED(inst->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&ix::d3d_render_target_tex))))
			{
				ix::d3d_dev->CreateRenderTargetView(ix::d3d_render_target_tex, nullptr, &ix::d3d_render_target_view);

				msg("render_target_tex => %p", ix::d3d_render_target_tex);
				msg("render_target_view => %p", ix::d3d_render_target_view);

				ix::d3d_render_target_tex->Release();
				ix::d3d_render_target_tex = nullptr;

				msg("d3d init ok");

				ImGui_ImplWin32_Init(g::game_hwnd);
				ImGui_ImplDX11_Init(ix::d3d_dev, ix::d3d_dev_ctx);
				ImGui_ImplDX11_CreateDeviceObjects();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, { 0.5f, 0.5f });
				ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.f);

				core::d3d_inited = true;
			}
		}

		msg("==================");
	}

	if( !(g_cfg.visual_anti_obs) )
		core::on_graphics();

	return HK_VMT_GET_ORIG(vmt::swap_chain, sc_present, 8)(inst, sync_int, flags);
}

void __fastcall hk::gamesdk_draw_imgui()
{
	if (g_cfg.visual_anti_obs)
		core::on_graphics();

	return reinterpret_cast<decltype(&gamesdk_draw_imgui)>(o_gamesdk_draw_imgui)();
}

namespace hk
{
	FORWARD_DECLARE(bool __fastcall set_mdl_idx(respawn::c_entity* ent, int idx));
	void* last_ent = nullptr;
	int weapon_equipped_ticks = 0;
	bool try_force(respawn::c_entity* ent, decltype(&set_mdl_idx) fn_orig, u64 ret_rva, int idx)
	{
		auto mdl = ix::model_info->get_model(idx);

		if (mdl)
		{
			auto mdl_name = std::string(ix::model_info->get_model_name(mdl));

			if (!mdl_name.empty())
			{
				auto pos = mdl_name.find(".rmdl");

				if (pos != std::string::npos)
				{
					mdl_name.insert(pos, fmt::format("_legendary_0{}", g_cfg.other_legendary_model_id));

					auto custom_idx =
						ix::model_info->get_model_index(mdl_name.c_str());

					if (custom_idx == -1 && g_cfg.other_legendary_model_fallback)
					{
						int tries = 9;

						while (--tries > 0 && custom_idx == -1)
						{
							auto mn = std::string(ix::model_info->get_model_name(mdl));
							mn.insert(pos, fmt::format("_legendary_0{}", tries));

							custom_idx = ix::model_info->get_model_index(mn.c_str());
						}
					}

					if (custom_idx != -1)
					{
						if (auto lp = respawn::local_player())
						{
							if (lp->is_alive())
							{
								if (auto lv = lp->get_view_model())
								{
									if (ent == lv)
									{
										if (!last_ent || last_ent != ent)
										{
											weapon_equipped_ticks = 0;
											last_ent = ent;
										}

										if (last_ent)
										{
											++weapon_equipped_ticks;
										}

										if (weapon_equipped_ticks > 500)
										{
											if (custom_idx != -1)
											{
												fn_orig(ent, custom_idx);
												return false;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return true;
	}
}

bool __fastcall hk::set_mdl_idx(respawn::c_entity* ent, int idx)
{
	static auto fn_orig = reinterpret_cast<decltype(&set_mdl_idx)>(o_set_mdl_idx);
	auto ret_rva = BASE_OF(_ReturnAddress()) - g::game_base;

	if (g_cfg.other_legendary_model_enable && !try_force(ent, fn_orig, ret_rva, idx))
		return false;

	rs::context_t rsc;
	rsc.target = fn_orig;
	rsc.regs.rcx.ptr = ent;
	rsc.regs.rdx.val = idx;

	return rs::invoke<bool>(&rsc);
}

bool __fastcall hk::process_playlist_override(void* inst, void* net_msg)
{
	if (net_msg)
	{
		char* data = reinterpret_cast<char*>(BASE_OF(net_msg) + 0x21);
		std::string data_str;

		if (utl::make_string_unsafe(data, data_str))
		{
			if (respawn::check_watermark(data_str))
			{
				if (!mem::compare(data_str.data(), data, data_str.size()))
					mem::copy(data, data_str.data(), data_str.size());
			}
		}
	}

	return reinterpret_cast<decltype(&process_playlist_override)>
		(o_process_playlist_override)(
			inst,
			net_msg
			);
}

bool __fastcall hk::process_string_cmd(void* inst, void* net_msg)
{
	auto pmsg = reinterpret_cast<respawn::string_cmd_t*>(net_msg);

	if (pmsg)
	{
		std::string cmd(pmsg->m_cmd);
		std::erase(cmd, '\n');

		INGAME_LOGGER_MSG(5.f, im::color::yellow, "[NET] received stringCmd: %s", cmd.c_str());

		if (g_cfg.other_net_ignore_disconnect && cmd == "disconnect")
		{
			INGAME_LOGGER_MSG(5.f, im::color::yellow, "[NET] blocked DC");
			return true;
		}
	}

	return HK_GET_ORIG(process_string_cmd)(inst, net_msg);
}

void __fastcall hk::dev_msg(u64 a1, const char* fmt, ...)
{
	char buf[512];
	va_list vl;

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	printf("    DEV  %s", buf);
}

static void* last_wpn;
static int last_id;
int64_t __fastcall hk::create_move_input(void* inst, int seq_nr, float input_sample_frame_time, bool active)
{
	auto rs = reinterpret_cast<decltype(&create_move_input)>(o_create_move_input)(inst, seq_nr, input_sample_frame_time, active);

	// *(_QWORD *)(a1 + 248) + 0x280i64 * (a2 % 750);
	auto cmd = reinterpret_cast<respawn::user_cmd_t*>( *reinterpret_cast<uintptr_t*>(BASE_OF(inst) + 0xF8) + (0x280 * (seq_nr % 750) ));

	if (!cmd || !cmd->command_number)
		return rs;

	auto me = respawn::local_player();

	if (me)
	{
		auto wpn = me->get_active_weapon();

		if (wpn)
		{
			g_ctx.can_fire = wpn->can_fire(me);

			if (!g_ctx.my_wpn_ptr_old || g_ctx.my_wpn_ptr_old != wpn)
			{
				respawn::exec_script_cl(R"(
local meme = compilestring(" entity me = GetLocalClientPlayer();

if(!IsValid(me) || !IsAlive(me))
return;

entity vmArm = me.GetViewModelArmsAttachment();

if(!IsValid(vmArm))
return;

vector highlightColor = <0.6,0.6,1.0>;
SonarViewModelHighlight(vmArm, highlightColor) ");

meme()
										)");

				g_ctx.my_wpn_ptr_old = wpn;
			}

			g_ctx.my_wpn_ptr = wpn;
		}

		if (g_cfg.aim_silent && g_cfg.aim_psilent)
		{
			if (wpn && !wpn->is_melee())
			{
				if (wpn->can_fire(me) && ( 
					(cmd->buttons & IN_ATTACK) || 
					(g_cfg.aim_auto_fire && f::aim->m_aiming) ||
					(f::trigger->m_target_found)) )
				{
					g_ctx.should_choke = true;
				}
			}
		}

		auto old_angles = cmd->viewangles;
		auto old_ms = cmd->sidemove;
		auto old_mf = cmd->forwardmove;

		core::move(cmd);

		if(ix::engine_client->is_in_game())
			g_gtc->process();

		if (g_cfg.aim_silent && g_cfg.aim_psilent)
		{
			//
			// Server Sielnt
			//

			if (wpn && !wpn->is_melee())
			{
				if (!g_ctx.should_choke && !(old_angles == cmd->viewangles))
				{
					cmd->viewangles = old_angles;
					cmd->sidemove = old_ms;
					cmd->forwardmove = old_mf;

					cmd->buttons &= ~IN_ATTACK;
				}
			}
		}
		else if (g_cfg.aim_silent && !g_cfg.aim_psilent)
		{
			//
			// Client Silent
			//

			if (wpn && !wpn->is_melee())
			{
				if(!wpn->can_fire(me) && cmd->buttons & IN_ATTACK)
				{
					cmd->viewangles = old_angles;
					cmd->sidemove = old_ms;
					cmd->forwardmove = old_mf;

					cmd->buttons &= ~IN_ATTACK;
				}
			}
		}
		else if (g_cfg.aim_tapfire)
		{
			if (!wpn->can_fire(me) && cmd->buttons & IN_ATTACK)
			{
				cmd->viewangles = old_angles;
				cmd->sidemove = old_ms;
				cmd->forwardmove = old_mf;

				cmd->buttons &= ~IN_ATTACK;
			}
		}

		f::mgr->on_move_unsafe(me, cmd);

		math::fix_movement(cmd->forwardmove, cmd->sidemove, cmd->viewangles, old_angles, old_mf, old_ms);
	}

	return rs;
}

void __fastcall hk::cl_move_send()
{
	auto nc = respawn::net_chan();

	if (nc)
	{
		if (g_ctx.should_choke)
		{
			++nc->m_out_sequence_nr;
			++g_ctx.packets_choked;
			g_ctx.should_choke = false;

			return;
		}
	}

	g_ctx.packets_choked = 0;

	return reinterpret_cast<decltype(&cl_move_send)>(o_cl_move_send)();
}

void __fastcall hk::frame_stage_notify(void* inst, int stage)
{
	if (stage == respawn::frame_start)
	{
		static bool last_state = false;
		static unsigned long long last_update = 0;
		
		if (!last_update || (GetTickCount64() - last_update) > 1200)
		{
			last_update = GetTickCount64();

			auto nc = respawn::net_chan();
			bool state = false;

			std::string conn_addr;
			std::string conn_port;
			std::string conn_key;

			if (nc)
			{
				auto remote_addr = std::string(reinterpret_cast<char*>(g::game_base + 0x18223E5 + 0x1));

				if (!remote_addr.empty())
				{
					std::erase(remote_addr, '[');
					std::erase(remote_addr, ']');
					
					auto result = utl::split_string(remote_addr, ":");

					if (result.size() == 2)
					{
						conn_addr = result[0];
						conn_port = result[1];
						conn_key = g::last_net_key;

						state = true;
					}
				}
			}
			else
			{
				state = false;
			}

			if (state != last_state)
			{
				if (state)
				{
					//msg("sending join!");
					//g_drof_net.on_server_connect(conn_addr.c_str(), conn_port.c_str(), conn_key.c_str());
				}
				else
				{
					//msg("sending leave!");
					//g_drof_net.on_level_shutdown();
				}
			}

			if (!ix::engine_client->is_in_game())
				g_gtc->process();

			last_state = state;
		}
	}

	return reinterpret_cast<decltype(&frame_stage_notify)>(o_frame_stage_notify)(inst, stage);
}

struct clc_move_t
{
	DATA_FIELD(backup_commands, this, int, 0x20);
	DATA_FIELD(new_commands, this, int, 0x24);
	DATA_FIELD(data_in, this, void*, 0x30);
};

auto get_user_cmd(void* inst, int seq)
{
	auto cmd = reinterpret_cast<respawn::user_cmd_t*>(*reinterpret_cast<uintptr_t*>(BASE_OF(inst) + 0xF8) + (0x280 * (seq % 750)));
	return cmd;
}

double write_user_cmd(void* buf, const respawn::user_cmd_t* to, const respawn::user_cmd_t* from)
{
	using fn_ = decltype(&write_user_cmd);
	auto fn = (fn_)(MOD_BASE("r5apex.exe") + 0x8A6BA0);

	return fn(buf, to, from);
}

bool __fastcall hk::write_ucmd_delta_to_buf(void* inst, int cmd_nr, void* buf, int from, int to)
{
	auto nc = respawn::net_chan();
	auto move_msg = reinterpret_cast<clc_move_t*>(BASE_OF(_AddressOfReturnAddress()) + 0x38);

	return reinterpret_cast<decltype(&write_ucmd_delta_to_buf)>(o_write_ucmd_delta_to_buf)(inst, cmd_nr, buf, from, to);
}

void __fastcall hk::sleep_until_input(void* inst, int max_sleep_time_ms)
{
	if (g_cfg.visual_disable_se_throttle)
		return;

	return HK_VMT_GET_ORIG(vmt::input_system, sleep_until_input, 27)(inst, max_sleep_time_ms);
}

bool __fastcall hk::on_take_damage(respawn::srv_player_t* pl, __int64 a2)
{
	bool god_mode = g_cfg.host_god_mode && !strcmp(pl->m_identifier, "player");

	auto rs = reinterpret_cast<decltype(&on_take_damage)>(o_on_take_damage)(pl, a2);

	if (god_mode)
		*reinterpret_cast<int*>(BASE_OF(pl) + 0x48C) = 0x100;

	return rs;
}

void __fastcall hk::on_take_ammo(void* wpn, unsigned int amount)
{
	if (g_cfg.host_infinite_ammo)
		return;

	return reinterpret_cast<decltype(&on_take_ammo)>(o_on_take_ammo)(wpn, amount);
}

void __fastcall hk::level_shutdown(void* inst)
{
	g_ent_cache.reset();
	return reinterpret_cast<decltype(&level_shutdown)>(o_level_shutdown)(inst);
}

void __fastcall hk::view_render_start(respawn::c_view_render* inst)
{
	reinterpret_cast<decltype(&view_render_start)>(o_view_render_start)(inst);

	if (g_cfg.visual_fov_scale_custom)
	{
		if (g_cfg.visual_fov_scale > 1.f)
			inst->fov = (g_cfg.visual_fov_scale / 100.f);

		if (g_cfg.visual_fov_scale_vm > 1.f)
			inst->fov_vm = (g_cfg.visual_fov_scale_vm / 100.f);

		if (g_cfg.visual_remove_recoil_visual)
		{
			if (auto lp = respawn::local_player())
			{
				inst->eye_angles = lp->eye_angles();
			}
		}
	}

	c_spec_mgr::single()->on_view_render(inst);
}

float* __fastcall hk::get_recoil(respawn::c_player* player, float* a2)
{
	bool restore = false;
	math::ang3_t original;

	if (g_cfg.visual_fov_scale_custom && g_cfg.visual_remove_recoil)
	{
		if (player == respawn::local_player() && player != nullptr)
		{
			auto comp_scale = (g_cfg.visual_recoil_comp_scale / 100.f);
			auto comp_actual = 1.f - comp_scale;

			original = player->view_recoil();
			player->view_recoil() = player->view_recoil() * comp_actual;
			restore = true;
		}
	}

	auto result = reinterpret_cast<decltype(&get_recoil)>(o_get_recoil)(player, a2);

	if (restore && player != nullptr)
	{
		player->view_recoil() = original;
	}

	return result;
}

bool __fastcall hk::get_player_info(void* this_ptr, int index, respawn::player_info_t* buffer)
{
	auto result = reinterpret_cast<decltype(&get_player_info)>(o_get_player_info)(this_ptr, index, buffer);

	const auto& cached = g_ent_cache.objects()[index];

	if (!RETURNS_WITHIN_SELF())
	{
		if (index == ix::engine_client->get_local_player_idx())
		{
			if(g_cfg.visual_anonymize_local)
				strcpy(buffer->name, _XS(" "));
		}
	}

	return result;
}

void __fastcall hk::net_set_key(void* inst, const char* key)
{
	g::last_net_key = key;

	return reinterpret_cast<decltype(&net_set_key)>(o_net_set_key)(inst, key);
}

void* __fastcall hk::find_mdl(void* inst, const char* path)
{
	return reinterpret_cast<decltype(&find_mdl)>(o_find_mdl)(inst, path);
}

u32* __fastcall hk::update_script_ent_data(void* a1, __int64* a2, __int64 a3, __int64 a4, __int64** a5)
{
	auto rs = reinterpret_cast<decltype(&update_script_ent_data)>(o_update_script_ent_data)(
		a1, a2,
		a3, a4,
		a5
		);

	if (ix::engine_client->is_in_game() && respawn::local_player())
	{
		u64 storage[2];

		storage[0] = a4;
		storage[1] = 0llu;

		if (a5)
		{
			auto entity = reinterpret_cast<respawn::c_entity*>(a5[0]);

			if (entity)
			{
				auto identifier = entity->get_identifier();
				auto pl = reinterpret_cast<respawn::c_player*>(entity);

				if (g_cfg.esp_dummy_always_visible && identifier && !strcmp(identifier, "npc_dummie"))
				{
					if (pl->is_alive())
					{
						auto v1 = respawn::get_entity_var_addr<char**>(storage, "targetTitle", 1);
						*v1[0] = '\0';
					}

					if (auto rs = respawn::get_entity_var_addr<float*>(storage, "lastCrosshairTargetTime", 9))
						*rs = *g_gv.time;
				}
			}
		}
	}

	return rs;
}
