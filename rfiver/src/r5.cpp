#include <include.h>

game_vars_t g_gv{ };
game_context_t g_ctx{ };

void game_context_t::think()
{
	do
	{
		g_input_mgr.sync(&si::game_ctx);

		if (cvar::srvcheats->m_value_numeric != 1)
		{
			INGAME_LOGGER_MSG(5.f, im::color::white, "forced sv_cheats convar on the client.");
			cvar::srvcheats->m_value_numeric = 1;
		}

		if (IS_DEBUG_BUILD())
		{
			if (*g_gv.signon_state == 0 && si::game_ctx.was_button_down(VK_SHIFT))
			{
				INGAME_LOGGER_MSG(5.f, im::color::green, "starting local dev server ...");

				auto gs = (respawn::net_game_server_t*)g_gv.server_list_mgr;

				auto& m_host_name = *reinterpret_cast<std::string*>(BASE_OF(gs));
				auto& m_desc = *reinterpret_cast<std::string*>(BASE_OF(gs) + 0x20);
				auto& m_map = *reinterpret_cast<std::string*>(BASE_OF(gs) + 0x48);
				auto& m_plist = *reinterpret_cast<std::string*>(BASE_OF(gs) + 0x68);

				m_host_name = _XS("drof.xyz | dev");
				m_map = _XS("mp_rr_canyonlands_staging");
				m_plist = _XS("firingrange");

				g_gtc->add([]()
					{
						g_gv.server_list_mgr->launch_server();
					});
			}
		}

		Sleep(1000);
	} while (true);
}
