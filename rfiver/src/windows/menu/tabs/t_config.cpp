#include <include.h>

void menu::tabs::config()
{
	if (ImGui::Button("load"))
	{
		cfg::load(&g_cfg, _XS("main.cfg"));
	}

	if (ImGui::Button("save"))
	{
		cfg::save(&g_cfg, _XS("main.cfg"));
	}
}
