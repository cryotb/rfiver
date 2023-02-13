#pragma once

namespace g
{
	struct self_t
	{
		uintptr_t base;
		uintptr_t length;
		u32 version;
	} extern self;

	inline HWND game_hwnd{ };
	inline math::dimension_t<int> screen{ };
	inline float game_time;

	inline u64 game_base{ };
	inline u64 game_length{ };

	inline u64 sdk_base{ };
	inline u64 sdk_length{ };

	inline bool is_dev{ };

	inline std::string last_net_key;

	struct rce_t
	{
		bool force_disable_aim;
		bool force_disable_esp;
	} extern rce;
}
