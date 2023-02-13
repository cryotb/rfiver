#include <include.h>

using namespace ft;

void c_spectators::paint()
{
	auto render = false;

	if ((g_cfg.visual_stick_window_spectators ||
		!g_cfg.visual_stick_window_spectators && wnds::menu.is_visible()))
		render = true;

	if (!render)
		return;

	auto lp = g_ent_cache.local_object();

	if (!lp || !CE_IS_VALID_PTR(lp))
		return;

	if (!lp->ds->dt.player.alive)
		return;

	ImGui::Begin("spectators");

	for (const auto& obj : g_ent_cache.objects())
	{
		if (!obj.hdr.m_used)
			continue;

		if (obj.hdr.m_type != cache::object_t::type::player)
			continue;

		if (obj.ds->dt.player.obs_mode != 0 && obj.ds->dt.player.obs_target_idx == ix::engine_client->get_local_player_idx())
		{
			ImGui::Text("%s", obj.ds->dt.player.info.name);
		}
	}

	ImGui::End();
}
