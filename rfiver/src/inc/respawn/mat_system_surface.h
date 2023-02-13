#pragma once

namespace respawn
{
	class c_mat_system_surface
	{
	public:
		void get_screen_size(int& wide, int& tall)
		{
			return vfunc::call<53, void>(this, &wide, &tall);
		}
	private:
	};
}
