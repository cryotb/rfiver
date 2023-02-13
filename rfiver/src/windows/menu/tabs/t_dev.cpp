#include <include.h>

void menu::tabs::dev()
{
	im::table::begin(_XS("tblDev"), 2, 0);

	if (ImGui::CollapsingHeader("Drofnet", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox(_XS("ignore users"), &g_cfg.dev_ignore_drofnet_identity);
	}
	im::table::right();

	if (ImGui::CollapsingHeader("B", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}

	im::table::down();

	if (ImGui::CollapsingHeader("C", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
	im::table::right();

	if (ImGui::CollapsingHeader("C", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
	im::table::down();

	im::table::end();
}
