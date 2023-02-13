#pragma once

namespace respawn
{
	class c_view_render
	{
	public:
		d3d::D3DMATRIX* get_view_matrix(char index = 0)
		{
			return vfunc::call<16, d3d::D3DMATRIX*>(this, index);
		}

		char pad_0x0000[0xD0]; //0x0000
		float fov; //0x00D0 
		float fov_vm; //0x00D4 
		char pad_0x00D8[0x8]; //0x00D8
		math::vec3_t camera_pos; //0x00E0
		math::ang3_t eye_angles; //0x00EC
	};
}
