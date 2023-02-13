#include <include.h>

void menu::tabs::aim()
{
	im::table::begin("tblAim", 2, 0);

	if (ImGui::CollapsingHeader("Main", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &g_cfg.aim_active);
		ImGui::Checkbox("Ignore key", &g_cfg.aim_ignore_key);
		ImGui::Checkbox("Auto ads", &g_cfg.aim_auto_ads);
		ImGui::Checkbox("Auto fire", &g_cfg.aim_auto_fire);
		ImGui::Checkbox("Silent", &g_cfg.aim_silent);
		ImGui::Checkbox("pSilent", &g_cfg.aim_psilent);
		ImGui::Checkbox("Sync bullets with aimpos", &g_cfg.aim_sync_bullets);
		ImGui::SliderInt("Hit ticks ratio", &g_cfg.aim_spread_ticks, 0, 10);
		ImGui::SliderFloat("Smooth out", &g_cfg.aim_smoothing_ratio, 0.f, 256.f);

		if (g::is_dev)
		{
			ImGui::Checkbox("Tap fire", &g_cfg.aim_tapfire);
			ImGui::Checkbox("Rapid fire", &g_cfg.aim_rapidfire);
		}

		ImGui::Checkbox("Visualize fov", &g_cfg.aim_visualize_fov);
	}
	im::table::right();

	if (g::is_dev)
	{
		if (ImGui::CollapsingHeader("Rapid fire", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderInt("Rapid fire -> ticks", &g_cfg.aim_rapidfire_ticks, 0, 50);
			ImGui::SliderFloat("Rapid fire -> scale", &g_cfg.aim_rapidfire_scale, 0.1f, 12.f);
		}
	}

	if (ImGui::CollapsingHeader("Trigger", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active##N2", &g_cfg.trigger_active);

		ImGui::Checkbox("For all weapons?", &g_cfg.trigger_global);

		if (!g_cfg.trigger_global)
		{
			ImGui::Checkbox("For shotgun", &g_cfg.trigger_shotgun);
			ImGui::Checkbox("For sniper", &g_cfg.trigger_sniper);
		}
	}

	im::table::down();

	if (ImGui::CollapsingHeader("Target selection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static const char* target_sel_methods[] =
		{
			"dist",
			"fov",
		};

		static const char* aim_positions[] =
		{
			"auto",
			"head",
			"body"
		};

		ImGui::ListBox("Method", &g_cfg.aim_target_sel_mode, target_sel_methods, ARRAYSIZE(target_sel_methods));
		ImGui::ListBox("Position", &g_cfg.aim_target_pos, aim_positions, ARRAYSIZE(aim_positions));

		ImGui::SliderFloat("Max distance", &g_cfg.aim_target_sel_max_dist, 0.f, 4096.f);
		ImGui::SliderFloat("Max fov (global)", &g_cfg.aim_target_sel_max_fov, 0.f, 180.f);
		ImGui::SliderFloat("Max fov (shotgun)", &g_cfg.aim_target_sel_max_fov_shotgun, 0.f, 180.f);
		ImGui::SliderFloat("Max fov (sniper)", &g_cfg.aim_target_sel_max_fov_sniper, 0.f, 180.f);
	}
	im::table::right();

	//ImGui::Text("4");
	im::table::down();

	im::table::end();
}