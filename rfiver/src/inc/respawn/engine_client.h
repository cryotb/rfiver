#pragma once

namespace respawn
{
	struct player_info_t
	{
		char name[512];
	};

	class c_engine_client
	{
	public:
		VIRTUAL_FUNC(get_local_player_idx, int, 36, (), this);
		VIRTUAL_FUNC(is_in_game, bool, 45, (), this);
		VIRTUAL_FUNC(get_player_info, bool, 24, (int idx, player_info_t* buf), this, idx, buf);
	};
}
