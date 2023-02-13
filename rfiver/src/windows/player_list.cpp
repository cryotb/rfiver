#include <include.h>

using namespace wnd;

void c_player_list::render()
{
	ImGui::Begin("players", nullptr, ImGuiWindowFlags_NoCollapse);
	static bool inited = false;

	if (!inited)
	{
		ImGui::SetWindowSize(ImVec2(500, 600));
		inited = true;
	}

	im::table::begin("test", 5, ImGuiTableFlags_Borders);

	// ----
	ImGui::Text("Name");
	im::table::right();

	ImGui::Text("Base");
	im::table::right();

	ImGui::Text("Health");
	im::table::right();

	ImGui::Text("Shield");
	im::table::right();

	ImGui::Text("Spec");
	im::table::right();
	// ----

	for (const auto& ent : g_ent_cache.objects())
	{
		if (!ent.hdr.m_used) continue;

		switch (ent.hdr.m_type)
		{
		case cache::object_t::type::player:
		case cache::object_t::type::dummie:
		{
			auto& pl_data = ent.ds->dt.player;

			ImGui::TextColored(g_ent_cache.object_color(ent), "%s", pl_data.info.name);
			im::table::right();

			ImGui::Text("%p", ent.hdr.m_pointer);
			im::table::right();

			ImGui::Text("%i/%i HP", pl_data.health, pl_data.health_max);
			im::table::right();

			ImGui::Text("N/A");
			im::table::right();

			if (!c_spec_mgr::single()->is_spectating())
			{
				if (ImGui::Button(fmt::format("spec##{}", ent.hdr.m_ent_index).c_str()))
				{
					c_spec_mgr::single()->start(ent.hdr.m_ent_index);
				}
			}
			else
			{
				if (ImGui::Button(fmt::format("stop##{}", ent.hdr.m_ent_index).c_str()))
				{
					c_spec_mgr::single()->stop();
				}
			}
			im::table::right();

			break;
		}
		default:
			break;
		}
	}

	im::table::end();

	ImGui::End();
}

void c_player_list::paint()
{
	if(wnds::menu.is_visible())
		render();
}
