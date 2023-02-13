#pragma once

namespace respawn
{
	struct netchan_t
	{
		MAKE_PAD(0x4); // 0x00
		int m_out_sequence_nr; // 0x4
		MAKE_PAD(0x8); // 0x10
		int m_choked_packets;

		auto send_datagram(void* dg)
		{
			using fn_ = int(__fastcall*)(void*, void*);
			auto fn = reinterpret_cast<fn_>(MOD_BASE("r5apex.exe") + 0x261D10);

			return fn(this, dg);
		}

		auto transmit()
		{
			return (send_datagram(nullptr) != 0);
		}
	};
}
