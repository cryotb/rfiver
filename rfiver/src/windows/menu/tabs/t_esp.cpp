#include <include.h>

void menu::tabs::esp()
{
	if (ImGui::CollapsingHeader("Main", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &g_cfg.esp_active);
		ImGui::Checkbox("Name", &g_cfg.esp_name);
		ImGui::Checkbox("Outline", &g_cfg.esp_outline);
		ImGui::Checkbox("Health bar", &g_cfg.esp_health_bar);
		ImGui::Checkbox("Head pos", &g_cfg.esp_head_pos);
		ImGui::Checkbox("Sight line", &g_cfg.esp_sight_line);
		ImGui::Checkbox("Bones", &g_cfg.esp_bones);
		ImGui::Checkbox("Always visible dummy", &g_cfg.esp_dummy_always_visible);

		ImGui::SliderFloat("Render Dist", &g_cfg.esp_render_dist, 0.f, 8192.f);
	}

	if (ImGui::CollapsingHeader("Glow", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active ##Glow", &g_cfg.esp_glow);
	}
}
