#pragma once

namespace wnd
{
	class c_base
	{
	public:
		virtual void paint() { }
	private:
	};

	class c_mgr
	{
	public:
		void add(c_base* w) { _wl.push_back(w); }

		auto on_paint()
		{
			for (c_base* p : _wl)
			{
				if (!p) continue;

				p->paint();
			}
		}
	private:
		std::vector<c_base*> _wl;
	};
}

extern wnd::c_mgr g_window_mgr;

#include "menu.h"
#include "logger.h"

namespace wnds
{
	inline wnd::c_menu menu{ };
	inline wnd::c_player_list plist{ };
	inline wnd::c_logger logger{ };

	inline auto init()
	{
		g_window_mgr.add(&menu);
		g_window_mgr.add(&plist);
		g_window_mgr.add(&logger);
	}
}
