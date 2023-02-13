#include <include.h>

using namespace wnd;

char script[0x1000 * 4000]; // 4MB
char script_cl[0x1000 * 4000]; // 4MB
char script_cl_buf[sizeof(script_cl) * 2]; // 8MB

struct tab_t
{
	int id;
	std::string name;
	void(*callback)(void);
};

std::vector<tab_t> tabs =
{
	{ 0, "config", menu::tabs::config },
	{ 1, "host", menu::tabs::host },
	{ 2, "aim", menu::tabs::aim },
	{ 3, "esp", menu::tabs::esp },
	{ 4, "visual", menu::tabs::visual },
	{ 5, "other", menu::tabs::other },
};

void render_online_users()
{
	ImGui::Begin("Online users");

	/*for (const auto& user : g_drof_net.users_online)
	{
		if (user.cgs.ingame)
		{
			ImGui::Text(fmt::format("{} GSRV:{}", user.name.c_str(), user.cgs.addr.c_str()).c_str());
			ImGui::SameLine();
			ImGui::TextColored(im::color::green, "ingame!");
			ImGui::SameLine();

			if (ImGui::Button("join"))
			{
				_msg("connect details => %s, %s, %s", user.cgs.addr.c_str(), user.cgs.port.c_str(), user.cgs.key.c_str());
				respawn::connect(user.cgs.addr, user.cgs.port, user.cgs.key);
			}
		}
		else
		{
			ImGui::Text(fmt::format("{}", user.name.c_str()).c_str());
		}
	}

	ImGui::End();*/
}

int __vmctx_curr;
void c_menu::render()
{
	{
		float pos_x = 200.f, pos_y = 200.f, push_y = 0.f;

		render::text({ pos_x, pos_y + push_y }, im::color::blue, _XS("drof.xyz"));
		push_y += 20.f;

		auto version_fmt = fmt::format("build {} {}  [ {:x} ]", _XS(__DATE__), _XS(__TIME__), g::self.version);
		render::text({ pos_x, pos_y + push_y }, im::color::white, version_fmt.c_str());
		push_y += 20.f;
	}

	static bool init = false;

	if (!init)
	{
		if (!strcmp(ix::cvar->find(_XS("nucleus_id"))->m_value_str, _XS("1009151818965")))
		{
			INGAME_LOGGER_MSG(5.f, im::color::yellow, "welcome back, dev!");
			tabs.push_back({ static_cast<int>(tabs.size()), _XS("dev"), menu::tabs::dev });

			g::is_dev = true;
		}

		g::is_dev = true;
		// remove that

		init = true;
	}

	ImGui::Begin("drof.xyz", nullptr, ImGuiWindowFlags_NoCollapse);
	static bool inited = false;

	if (!inited)
	{
		ImGui::SetWindowSize(ImVec2(500, 600));
		inited = true;
	}

	int i = 0;

	ImGui::BeginTabBar("MainTb");

	for (const auto& tab : tabs)
	{
		if (ImGui::BeginTabItem(tab.name.c_str()))
		{
			_tab_id = i;

			ImGui::EndTabItem();
		}

		i++;
	}

	ImGui::EndTabBar();

	for (const auto& tab : tabs)
	{
		if (_tab_id == tab.id)
			tab.callback();
	}

	ImGui::End();

	ImGui::Begin("SQVM");
	ImGui::InputTextMultiline("##Tml_CS", script, sizeof(script), 
		{ ImGui::GetWindowSize().x - 5.f, ImGui::GetWindowSize().y - 150.f });

	static const char* vmctx_curr_items[] =
	{
		"SERVER",
		"CLIENT",
		"UI",
	};

	ImGui::ListBox("VM Context", &__vmctx_curr, vmctx_curr_items, ARRAYSIZE(vmctx_curr_items));

	if (ImGui::Button("RUN"))
	{
		g_gtc->add([]()
			{
				respawn::execute_sqc(script, (respawn::SQCONTEXT)__vmctx_curr);
			});
	}
	ImGui::End();

	ImGui::Begin("scripting [CL]");
	ImGui::InputTextMultiline("##Tml_CCS", script_cl, sizeof(script_cl),
		{ ImGui::GetWindowSize().x - 5.f, ImGui::GetWindowSize().y - 150.f });
	if (ImGui::Button("RUN"))
	{
		g_gtc->add([]()
			{
				std::string tmp( script_cl );
				tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.cend());

				memset(script_cl_buf, 0, sizeof(script_cl_buf));
				sprintf_s(script_cl_buf, "%s", tmp.data());
				
				respawn::execute_sqc(script_cl_buf, respawn::SQCONTEXT::CLIENT);
			});
	}
	ImGui::End();
}

void c_menu::paint()
{
	if (si::paint.was_button_down(VK_INSERT))
		_visible = !_visible;

	if (_visible)
		render();

	if ((g_cfg.visual_stick_window_online_users ||
		!g_cfg.visual_stick_window_online_users && _visible))
	{
		render_online_users();
	}
}
