#pragma once

namespace respawn
{
	class c_convar
	{
	public:
		char pad_0x0000[0x18]; //0x0000
		char* m_name; //0x0018 
		char* m_description; //0x0020 
		char pad_0x0028[0x30]; //0x0028
		char* m_value_str; //0x0058 
		char pad_0x0060[0x8]; //0x0060
		float m_value_float; //0x0068 
		__int32 m_value_numeric; //0x006C 
		char pad_0x0070[0x7D0]; //0x0070
	}; //Size: 0x0154


	class c_convar_mgr
	{
	public:
		c_convar* find(const char* name)
		{
			return vfunc::call<14, c_convar*>(this, name);
		}
	private:
	};
}

