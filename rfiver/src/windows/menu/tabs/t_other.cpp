#include <include.h>

auto get_sqvm() -> u64
{
	auto a1 = MOD_BASE("r5apex.exe") + 0xd4d9700;
	auto a2 = *reinterpret_cast<u64*>(a1);
	if (!a2) return 0;

	return a2;
}

auto get_sqvm_inst() -> u64
{
	auto a1 = MOD_BASE("r5apex.exe") + 0xd4d9700;
	auto a2 = *reinterpret_cast<u64*>(a1);
	if (!a2) return 0;

	auto a3 = *reinterpret_cast<u64*>(a2 + 0x8);
	return a3;
}

void menu::tabs::other()
{
	im::table::begin("tblOther", 2, 0);

	if (ImGui::CollapsingHeader("Main", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Anti Aim", &g_cfg.other_anti_aim);
	}
	im::table::right();

	if (ImGui::CollapsingHeader("Fake Lag", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &g_cfg.other_fake_lag);
		ImGui::SliderInt("Base", &g_cfg.other_fake_lag_base, 1, 15);
	}
	im::table::down();

	if (ImGui::CollapsingHeader("Networking", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("StringCmd: Ignore disconnect", &g_cfg.other_net_ignore_disconnect);
	}
	im::table::right();

	if (ImGui::CollapsingHeader("Legendary Models", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active##N6", &g_cfg.other_legendary_model_enable);
		ImGui::Checkbox("Use fallback on unknown##N6", &g_cfg.other_legendary_model_fallback);
		ImGui::SliderInt("ID", &g_cfg.other_legendary_model_id, 0, 8);
		if (ImGui::Button("update"))
		{
			g_gtc->add(
				[]()
				{
					auto lp = respawn::local_player();

					if (!lp || !lp->is_alive()) return;

					auto lw = lp->get_active_weapon();

					if (!lw) return;

					if (lw->set_legendary_model_idx(g_cfg.other_legendary_model_id))
						__noop();

					INGAME_LOGGER_MSG(5.f, im::color::white, "forced legendary!");
				}
			);
		}
	}
	im::table::down();

	im::table::end();
}
