#pragma once

namespace respawn
{
	class c_input_system
	{
	public:
		VIRTUAL_FUNC(is_button_down, bool, 13, (int kc), this, kc);
		VIRTUAL_FUNC(get_cursor_pos, void, 36, (int* x, int* y), this, x, y);
	private:
	};
}
