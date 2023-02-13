#include <include.h>

void menu::tabs::visual()
{
	im::table::begin("tblVisual", 2, 0);

	if (ImGui::CollapsingHeader("Custom FOV", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &g_cfg.visual_fov_scale_custom);
		ImGui::Checkbox("remove recoil", &g_cfg.visual_remove_recoil);
		ImGui::Checkbox("remove visual recoil", &g_cfg.visual_remove_recoil_visual);
		
		ImGui::SliderFloat("recoil comp scale", &g_cfg.visual_recoil_comp_scale, 0.1f, 100.f);
		ImGui::SliderFloat("camera fov", &g_cfg.visual_fov_scale, 0.1f, 400.f);
		ImGui::SliderFloat("viewmodel fov", &g_cfg.visual_fov_scale_vm, 0.1f, 400.f);
	}
	im::table::right();

	if (ImGui::CollapsingHeader("Other", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Third person view", &g_cfg.visual_third_person);
		ImGui::Checkbox("Anti OBS", &g_cfg.visual_anti_obs);
		ImGui::Checkbox("Disable SE Tab-out Throttle", &g_cfg.visual_disable_se_throttle);
		ImGui::Checkbox("Anonymize: local player", &g_cfg.visual_anonymize_local);
	}
	im::table::down();

	if (ImGui::CollapsingHeader("Window Stick", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("spectator list", &g_cfg.visual_stick_window_spectators);
		ImGui::Checkbox("online list", &g_cfg.visual_stick_window_online_users);
	}
	im::table::right();

	if (ImGui::CollapsingHeader("TD", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
	im::table::down();

	im::table::end();
}
