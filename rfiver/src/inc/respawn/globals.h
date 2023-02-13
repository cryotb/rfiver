#pragma once

namespace respawn
{
	class c_global_vars
	{
	public:
		char pad_0000[16]; //0x0000
		float Curtime; //0x0010
		char pad_0014[12]; //0x0014
		float Frametime; //0x0020
		char pad_0024[12]; //0x0024
		float IntervalPerTick; //0x0030
		int32_t MaxClients; //0x0034
		char pad_0038[8]; //0x0038
		int32_t TickCount; //0x0040
		char pad_0044[1052]; //0x0044
	}; //Size: 0x0460
	static_assert(sizeof(c_global_vars) == 0x460);
}
