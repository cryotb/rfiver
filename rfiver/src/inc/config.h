#pragma once

struct config_t
{
	u32 magic;
	u32 version;

	bool aim_active{ true };
	bool aim_ignore_key{ false };
	bool aim_auto_ads{ false };
	bool aim_silent{ true };
	bool aim_auto_fire{ false };
	bool aim_tapfire{ false };
	bool aim_rapidfire{ false };
	bool aim_visualize_fov{ false };
	bool aim_psilent{ false };
	float aim_smoothing_ratio{ 0.f };
	bool aim_sync_bullets{ false };
	int aim_spread_ticks{ 5 };
	int aim_rapidfire_ticks{ 25 };
	float aim_rapidfire_scale{ 5.f };
	float aim_target_sel_max_dist{ 0.f };
	float aim_target_sel_max_fov{ 5.f };
	float aim_target_sel_max_fov_shotgun{ 25.f };
	float aim_target_sel_max_fov_sniper{ 5.f };
	int aim_target_sel_mode{ 1 };
	int aim_target_pos{};

	bool trigger_active{ false };
	bool trigger_global{ false };
	bool trigger_shotgun{ false };
	bool trigger_sniper{ false };

	bool esp_active{ true };
	bool esp_name{ false };
	bool esp_outline{ true };
	bool esp_health_bar{ true };
	bool esp_head_pos{ false };
	bool esp_sight_line{ true };
	bool esp_bones{ true };
	bool esp_glow{ true };
	bool esp_dummy_always_visible{ true };
	float esp_render_dist{ 8192.f };

	bool visual_disable_se_throttle{ true };
	bool visual_anti_obs{ false };
	bool visual_fov_scale_custom{ true };
	bool visual_remove_recoil{ true };
	bool visual_remove_recoil_visual{ false };
	bool visual_third_person{ true };
	bool visual_anonymize_local{ true };
	bool visual_stick_window_online_users{ false };
	bool visual_stick_window_spectators{ true };
	float visual_fov_scale{ 1.f };
	float visual_fov_scale_vm{ 1.f };
	float visual_recoil_comp_scale{ 50.f };

	bool host_god_mode{ true };
	bool host_infinite_ammo{ true };

	bool dev_ignore_drofnet_identity{ true };

	bool other_anti_aim{ false };
	bool other_fake_lag{ false };
	int other_fake_lag_base{ 10 };

	bool other_net_ignore_disconnect{ false };

	bool other_legendary_model_enable{ false };
	bool other_legendary_model_fallback{ false };
	int other_legendary_model_id{ 2 };
};

namespace cfg
{
	constexpr u32 magic_ = 0xFC061332;
	extern bool save(config_t* p, const char* path);
	extern bool load(config_t* p, const char* path);
}

extern config_t g_cfg;
