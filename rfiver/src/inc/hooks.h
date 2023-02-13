#pragma once

EXTERN_C void asm_tramp_x_fire_weapon_def();
EXTERN_C extern void* o_fire_weapon_def;

namespace hk
{
	inline long long o_wnd_proc{ };
	inline void* o_create_move_input{ };
	inline void* o_cl_move_send{ };
	inline void* o_frame_stage_notify{ };
	inline void* o_write_ucmd_delta_to_buf{ };
	inline void* o_obs_graphics_d3d11_capture{ };
	inline void* o_gamesdk_draw_imgui{ };
	inline void* o_on_take_damage{ };
	inline void* o_on_take_ammo{ };
	inline void* o_get_recoil{ };
	inline void* o_view_render_start{ };
	inline void* o_level_shutdown{ };
	inline void* o_get_player_info{ };
	inline void* o_net_set_key{ };
	inline void* o_find_mdl{ };
	inline void* o_update_script_ent_data{ };
	inline void* o_set_mdl_idx{ };
	inline void* o_process_playlist_override{ };
	inline void* o_process_string_cmd{ };
	inline void* o_dev_msg{ };

	namespace vmt
	{
		inline vmthk* swap_chain{ };
		inline vmthk* input_system{ };
	}

	namespace srv
	{
		extern bool start();
	}

	extern bool start_early();
	extern bool start();

	extern long long __fastcall wnd_proc(HWND hwnd, UINT xmsg, WPARAM wp, LPARAM lp);
	extern long __fastcall sc_present(IDXGISwapChain* inst, uint32_t sync_int, uint32_t flags);
	extern void __fastcall gamesdk_draw_imgui();

	extern bool __fastcall set_mdl_idx(respawn::c_entity* ent, int idx);
	extern bool __fastcall process_playlist_override(void* inst, void* net_msg);
	extern bool __fastcall process_string_cmd(void* inst, void* net_msg);
	extern void __fastcall dev_msg(u64 a1, const char* fmt, ...);
	extern int64_t __fastcall create_move_input(void* inst, int seq_nr, float input_sample_frame_time, bool active);
	extern void __fastcall cl_move_send();
	extern void __fastcall frame_stage_notify(void* inst, int stage);
	extern bool __fastcall write_ucmd_delta_to_buf(void* inst, int cmd_nr, void* buf, int from, int to);
	extern void __fastcall sleep_until_input(void* inst, int max_sleep_time_ms);
	extern bool __fastcall on_take_damage(respawn::srv_player_t* pl, __int64 a2);
	extern void __fastcall on_take_ammo(void* wpn, unsigned int amount);
	extern float* __fastcall get_recoil(respawn::c_player* player, float* a2);
	extern void __fastcall view_render_start(respawn::c_view_render* inst);
	extern void __fastcall level_shutdown(void* inst);
	extern bool __fastcall get_player_info(void* this_ptr, int index, respawn::player_info_t* buffer);
	extern void __fastcall net_set_key(void* inst, const char* key);
	extern void* __fastcall find_mdl(void* inst, const char* path);
	extern void x_fire_weapon_def(
		void* inst,
		math::vec3_t* pos,
		math::vec3_t* dir,
		__int64 a4,
		int a5,
		__int64 a6,
		char ignore_spread);
	extern u32* __fastcall update_script_ent_data(void* a1, __int64* a2, __int64 a3, __int64 a4, __int64** a5);
}
