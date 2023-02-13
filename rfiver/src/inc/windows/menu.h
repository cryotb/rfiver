#pragma once

namespace menu::tabs
{
	extern void aim();
	extern void esp();
	extern void visual();
	extern void host();
	extern void other();
	extern void dev();
	extern void config();
}

namespace wnd
{
	class c_menu : public c_base
	{
	public:
		void paint() override;
		void render();

		auto is_visible() { return _visible; }
	private:
		int _tab_id;
		bool _visible;
	};

	class c_player_list : public c_base
	{
	public:
		void paint() override;
		void render();
	};
}
