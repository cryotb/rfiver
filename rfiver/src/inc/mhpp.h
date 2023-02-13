#pragma once

namespace mh
{
	inline bool init() { return MH_Initialize() == MH_OK; }
	inline bool place(void* target, void* handler, void** orig)
	{
		//msg("mh::place(%p)", target);

		if (!target)
			__debugbreak();

		return MH_CreateHook(target, handler, orig) == MH_OK &&
			MH_EnableHook(target) == MH_OK;
	}
}
