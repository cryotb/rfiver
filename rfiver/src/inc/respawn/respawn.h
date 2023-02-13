#pragma once

#include "defs.h"
#include "math.h"
#include "dt_recv.h"
#include "raytrace.h"
#include "base_client.h"
#include "view_render.h"
#include "engine_client.h"
#include "engine_trace.h"
#include "globals.h"
#include "mat_system_surface.h"
#include "input_system.h"
#include "cvar.h"
#include "netvar.h"
#include "netchan.h"

namespace g
{
	inline c_netvar_manager netvar_mgr{ };
}

#include "handle.h"
#include "entity.h"
#include "view_model.h"
#include "model_info.h"
#include "weapon.h"
#include "entity_list.h"

#define TICK_INTERVAL			(ix::globals->IntervalPerTick)


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)

#define IN_ATTACK				(1 << 0)
#define IN_JUMP					(1 << 1)
#define IN_DUCK					(1 << 2)
#define IN_FORWARD				(1 << 3)
#define IN_BACK					(1 << 4)
#define IN_USE					(1 << 5)
#define IN_CANCEL				(1 << 6)
#define IN_LEFT					(1 << 7)
#define IN_RIGHT				(1 << 8)
#define IN_MOVELEFT				(1 << 9)
#define IN_MOVERIGHT			(1 << 10)
#define IN_ATTACK2				(1 << 11)
#define IN_RUN					(1 << 12)
#define IN_RELOAD				(1 << 13)
#define IN_ALT1					(1 << 14)
#define IN_ALT2					(1 << 15)
#define IN_SCORE				(1 << 16)
#define IN_SPEED				(1 << 17)
#define IN_WALK					(1 << 18)
#define IN_ZOOM					(1 << 19)
#define IN_WEAPON1				(1 << 20)
#define IN_WEAPON2				(1 << 21)
#define IN_BULLRUSH				(1 << 22)

#define	FL_ONGROUND				(1 << 0)
#define FL_DUCKING				(1 << 1)
#define	FL_WATERJUMP			(1 << 3)
#define FL_ONTRAIN				(1 << 4)
#define FL_INRAIN				(1 << 5)
#define FL_FROZEN				(1 << 6)
#define FL_ATCONTROLS			(1 << 7)
#define	FL_CLIENT				(1 << 8)
#define FL_FAKECLIENT			(1 << 9)
#define	FL_INWATER				(1 << 10)

namespace cache
{
	struct bone_t
	{
		bool available;
		bool visible;
		bool onscreen;
		int parent;

		math::vec3_t pos;
		math::vec3_t scr;
		std::string name;
	};

	struct hitbox_t
	{
		bool visible;
		bool onscreen;

		math::vec3_t pos;
		math::vec3_t scr;
	};

	struct object_t
	{
		enum class type
		{
			none = 0,

			player,
			dummie,
		};

		struct hdr_t
		{
			int m_ent_index;
			int m_tick;
			bool m_used;
			type m_type;
			void* m_pointer;
		} hdr;

		struct data_t
		{
			int m_bone_count;
			math::vec3_t m_origin;
			math::vec3_t m_head;
			std::array<bone_t, 400> m_bones;
			std::array<hitbox_t, 20> m_hitboxes;
			stl::circ_buf_t< std::pair<int, std::array<hitbox_t, 20>>, 12> m_prev_hitboxes;

			union
			{
				struct
				{
					uint64_t nucleus_id;

					int health;
					int health_max;

					int armor;
					int armor_max;
					int obs_mode;
					int obs_target_idx;

					bool alive;
					bool scr_avail;
					bool visible;
					bool online_drofnet_identity;
					bool zooming;
					char anonymized_name[32];
					float time_last_moved;
					math::vec3_t scr_head, scr_origin;
					math::ang3_t eye_angles;
					respawn::player_info_t info;
				} player;
			} dt;
		} *ds;

		data_t* ds_prev;
	};
}

namespace respawn
{
	enum client_frame_stages
	{
		frame_undefined = -1,
		frame_start,

		frame_net_update_start,
		frame_net_update_post_data_update_start,
		frame_net_update_post_data_update_end,
		frame_net_update_end,

		frame_render_start,
		frame_render_end,
		frame_stage_count,
	};

	class string_cmd_t
	{
	public:
		void** m_vtable; //0x0000 
		__int32 m_unk; //0x0008 
		__int32 m_reliable; //0x000C 
		void* m_net_chan; //0x0010 
		void* m_client_state; //0x0018 
		char* m_cmd; //0x0020 
		char m_cmd_buf[1024]; //0x5F27E9C0 

	}; //Size=0x0428

	struct native_t
	{
		char* m_name; //0x0000 
		char* m_name2; //0x0008 
		char* m_comment; //0x0010 
		char pad_0x0018[0x48]; //0x0018
		void* m_func; //0x0060 
	}; //Size=0x0068

	struct user_cmd_t
	{
		int32_t command_number; //0x0000 
		int32_t tick_count; //0x0004 
		float_t curtime; //0x0008 
		math::ang3_t viewangles; //0x000C 
		char pad_0x0018[0xC]; //0x0018
		float_t forwardmove; //0x0024 
		float_t sidemove; //0x0028 
		float_t upmove; //0x002C 
		int32_t buttons; //0x0030 
		char pad_0x0034[0x150]; //0x0034
		int32_t randomseed; //0x0184 
		char pad_0x0188[0x8]; //0x0188
		math::vec3_t headposition; //0x0190 
		float_t maxpitch; //0x019C 
		char pad_0x01A0[0xE0]; //0x01A0
	};//size 0x0280;

	enum hitbox_ids
	{
		HITBOX_ID_HEAD = 0,
		HITBOX_ID_NECK,
		HITBOX_ID_SPINE,

		HITBOX_ID_HIP = 5,
	};

	enum class weapons : ulong
	{
		invalid = 0,

		sg_mastiff = 10,
		sg_eva8 = 11,
		sg_peace_keeper = 7,
		sg_mozam = 12,

		sn_kraber = 13,
		sn_longbow = 5,
		sn_take_triple = 6,
		sn_charge = 42,
		sn_sentinel = 64,
		sn_wingman = 18,

		base = 0x100,
		arc_star,
	};

	enum class weapon_types : ulong
	{
		invalid = 0,

		shotgun,
		sniper,
		smg,
		ar,

		grenade,
	};

	namespace detail
	{
		extern native_t* nt_find(u64 list_base, const char* name);
	}

	extern void debug_draw_line(
		const math::vec3_t& start,
		const math::vec3_t& end,
		int r,
		int g,
		int b,
		bool thru_world,
		float duration
	);
	extern void debug_draw_text(
		const math::vec3_t& pos,
		const char* text,
		bool depth_test,
		float duration
	);
	extern void debug_draw_box(
		const math::vec3_t& origin,
		const math::vec3_t& mins,
		const math::vec3_t& maxs,
		int r, int g, int b, int a,
		float duration
	);
	extern weapons get_weapon_def(c_view_model* vm, c_weapon* wpn);
	extern weapon_types get_weapon_type(weapons wpn);

	extern cache::object_t::type detect_entity_type(respawn::c_entity* entity);

#pragma pack(push, 1)
	struct net_game_server_t
	{
		__int32 m_hosting_status; //0x0000 
		__int32 m_server_visibility; //0x0004 
		std::string m_host_name; //0x0008 
		std::string m_desc; //0x0028 
		__int64 N00000011; //0x0048 
		std::string m_map; //0x0050 
		std::string m_play_list; //0x0070 
	};
#pragma pack(pop)

	extern int32_t get_main_thread_id();

	enum class SQCONTEXT : long
	{
		SERVER = 0,
		CLIENT,
		UI,
		NONE
	};

	extern bool load_script(__int64 pSQVM, const char* path, __int64 name, int unk0);
	extern void execute_sqc(const char* str, SQCONTEXT mode);

	extern void* create_iface(const char* name, uintptr_t null);
	extern bool world_to_screen(const math::vec3_t& vIn, math::vec3_t& vOut);
	extern bool can_fire(respawn::c_player* me);
	extern bool check_watermark(std::string& data_str);

	extern c_player* local_player();
	extern netchan_t* net_chan();

	extern std::string get_client_user_name();

	extern bool is_third_person();

	extern void cbuf_add_text(const char* cmd);
	extern void net_set_key(const char* key);
	extern void connect(std::string addr, std::string port, std::string key);

	extern void dev_msg(int lvl, const char* fmt, ...);

	extern native_t* nt_find(const char* name);

	template < typename T >
	T get_native_func(const char* name)
	{
		auto nt = respawn::nt_find(name);
		auto disasm = hde::disasm_buffer_ex(nt->m_func, 150);

		if (nt)
		{
			int num_calls = 0;
			u32 offset = 0;

			for (const auto& ins : disasm)
			{
				if (num_calls < 2 && ins.opcode == 0xE8)
					num_calls++;

				if (num_calls >= 2)
					break;

				offset += ins.len;
			}

			auto rs = hde::resolve_call(BASE_OF(nt->m_func) + offset);

			if (rs)
				return (T)rs;
		}

		__debugbreak();

		return T( );
	}

	template < typename T >
	T get_entity_var_addr(void* storage, const char* name, int magic)
	{
		using fn = void* (__fastcall*)(void*, const char*, u8, u8);
		static auto func = reinterpret_cast<fn>(MOD_BASE("r5apex.exe") + 0x301840);

		return (T)func(storage, name, 0, magic);
	}

	enum EHostStatus_t
	{
		NOT_HOSTING,
		HOSTING
	};

	enum EServerVisibility_t
	{
		OFFLINE,
		HIDDEN,
		PUBLIC
	};

#pragma pack(push, 1)
	class c_srv_list_mgr
	{
	public:
		void launch_server();

		auto game_server() { return &m_server; }
	private:
		EHostStatus_t m_hosting_status;
		EServerVisibility_t m_server_vis;

		net_game_server_t m_server;
		std::vector<net_game_server_t> m_server_list;
		mutable std::mutex m_mtx;
	};
#pragma pack(pop)

	inline auto exec_script_cl(const char* text)
	{
		std::string tmp(text);
		tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.cend());

		execute_sqc(tmp.data(), respawn::SQCONTEXT::CLIENT);
	}
}
