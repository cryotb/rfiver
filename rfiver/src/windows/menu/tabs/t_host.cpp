#include <include.h>

void menu::tabs::host()
{
	im::table::begin("tblHosts", 2, 0);

	if (ImGui::CollapsingHeader("Main", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Infinite ammo", &g_cfg.host_infinite_ammo);
		ImGui::Checkbox("Disable player damage", &g_cfg.host_god_mode);
	}
	im::table::right();

	ImGui::Text("3");
	im::table::right();

	ImGui::Text("4");
	im::table::down();

	im::table::end();
}
