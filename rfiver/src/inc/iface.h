#pragma once

namespace ix
{
	inline IDXGISwapChain* swap_chain{ };
	inline ID3D11Device* d3d_dev{ };
	inline ID3D11DeviceContext* d3d_dev_ctx{ };
	inline ID3D11Texture2D* d3d_render_target_tex{ };
	inline ID3D11RenderTargetView* d3d_render_target_view{ };

	inline respawn::c_base_client* base_client{ };
	inline respawn::c_entity_list* ent_list{ };
	inline respawn::c_view_render* view_render{ };
	inline respawn::c_engine_client* engine_client{ };
	inline respawn::c_engine_trace* engine_trace{ };
	inline respawn::c_mat_system_surface* mat_surface{ };
	inline respawn::c_input_system* input_system{ };
	inline respawn::c_model_info* model_info{ };
	inline respawn::c_global_vars* globals{ };
	inline respawn::c_convar_mgr* cvar{ };
}

struct game_time_scale_t
{
	char pad[0x18];
	float scale;
};

struct game_vars_t
{
	int* signon_state;
	float* time;
	float* time_scale;
	game_time_scale_t* game_time_scale;
	respawn::netchan_t** net_chan;
	respawn::c_srv_list_mgr* server_list_mgr;
};

struct game_context_t
{
	void think();
	void reset() { memset(this, 0, sizeof(*this)); }

	int packets_choked;
	bool can_fire;
	int my_ent_idx;
	bool should_choke;
	bool is_ingame;
	bool is_ingame_prev;
	float fov;
	u64 sqvm;
	respawn::c_player* my_ent_ptr;
	respawn::c_weapon* my_wpn_ptr;
	respawn::c_weapon* my_wpn_ptr_old;
	respawn::studiohdr_t* my_studio;
	respawn::user_cmd_t curr_user_cmd;
	respawn::user_cmd_t last_user_cmd;
};

namespace cvar
{
	inline respawn::c_convar* srvcheats{ };
	inline respawn::c_convar* move_use_dt{ };
	inline respawn::c_convar* third_person{ };
}

extern game_vars_t g_gv;
extern game_context_t g_ctx;

namespace si
{
	inline c_synced_input paint{};
	inline c_synced_input create_move{};
	inline c_synced_input game_ctx{};
}
