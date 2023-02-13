#pragma once

namespace respawn
{
	FORWARD_DECLARE_CLASS(c_weapon);
	FORWARD_DECLARE_CLASS(c_view_model);

	struct srv_player_t
	{
		char pad_1[0x68];
		char* m_mdl_name; // 0x68
		char pad_2[0x8];
		char* m_identifier; // 0x78
	};

	class mstudiobone_t
	{
	public:
		__int32 name_index; //0x0000 
		__int32 parent; //0x0004 
		__int32 m_bc; //0x0008 
		__int32 m_bc2; //0x000C 
		__int32 m_bc3; //0x0010 
		__int32 m_bc4; //0x0014 
		__int32 m_bc5; //0x0018 
		__int32 m_bc6; //0x001C 
		math::vec3_t m_pos; //0x0020 
		math::vec3_t m_quat; //0x002C 
		math::vec3_t m_rot; //0x0038 
		math::vec3_t m_pos_scale; //0x0044 
		math::vec3_t m_rot_scale; //0x0050 
		math::matrix3x4 m_pose_to_bone; //0x005C 
		math::vec3_t m_align; //0x008C 
		__int32 m_flags; //0x0098 
		__int32 N000001D5; //0x009C 
		__int32 N00000276; //0x00A0 
		__int32 N000001D6; //0x00A4 
		__int32 N00000279; //0x00A8 
		__int32 N000001D7; //0x00AC 
		__int32 N0000027C; //0x00B0 
		__int32 N000001D8; //0x00B4 

		char* const name() { return reinterpret_cast<char*>(BASE_OF(this) + name_index); }
	}; //Size=0x00B8

	class mstudiobbox_t {
	public:
		int32_t bone;             // 0x0000
		int32_t group;            // 0x0004
		math::vec3_t min;          // 0x0008
		math::vec3_t max;          // 0x0014
		int32_t hitbox_name_id;   // 0x0020
		char pad_0024[4];         // 0x0024
		int32_t hitbox_name_id_;  // 0x0028
	};

	class mstudiohitboxset_t {
	public:
		int32_t hitbox_set_name_index;  // 0x0000
		int32_t num_hitboxes;           // 0x0004
		int32_t hitbox_index;           // 0x0008

		auto hitbox(int i)
		{
			return reinterpret_cast<mstudiobbox_t*>(BASE_OF(this) + hitbox_index + (i * sizeof(mstudiobbox_t)));
		}
	};

	class studiohdr_t
	{
	public:
		__int32 id; //0x0000 
		__int32 version; //0x0004 
		char pad_8[8]; //0x946AE9C0 
		char model_name[64]; //0x946AE9C0 
		__int32 length; //0x0050 
		math::vec3_t eye_pos; //0x0054 
		math::vec3_t illum_pos; //0x0060 
		math::vec3_t hull_min; //0x006C 
		math::vec3_t hull_max; //0x0078 
		math::vec3_t view_bbmin; //0x0084 
		math::vec3_t view_bbmax; //0x0090 
		__int32 flags; //0x009C 
		__int32 num_bones; //0x00A0 
		__int32 bone_index; //0x00A4 
		__int32 num_bone_controllers; //0x00A8 
		__int32 bone_controller_index; //0x00AC 
		__int32 num_hitbox_sets; //0x00B0 
		__int32 hitbox_set_index; //0x00B4 

		auto bone(int i)
		{
			return reinterpret_cast<mstudiobone_t*>(BASE_OF(this) + bone_index + (i * sizeof(mstudiobone_t)));
		}

		auto hitbox_set(int i)
		{
			return reinterpret_cast<mstudiohitboxset_t*>(BASE_OF(this) + hitbox_set_index + (i * sizeof(mstudiohitboxset_t)));
		}

		auto get_hitbox(int set, int i) -> mstudiobbox_t*
		{
			auto hbs = hitbox_set(set);

			if (!hbs) return nullptr;

			if (i > hbs->num_hitboxes-1) return nullptr;

			return hbs->hitbox(i);
		}
	}; //Size=0x00B8

	struct player_state_t
	{
		NETVAR_FIELD_NR(dead_flag, this, void*, "DT_PlayerState", "deadflag");
	};

	class c_entity
	{
	public:
		VIRTUAL_FUNC(get_abs_origin, math::vec3_t&, 8, (), this);

		DATA_FIELD_NR(renderable, this, void*, 0x10);
		DATA_FIELD_NR(networkable, this, void*, 0x18);

		DATA_FIELD(model_name, this, char*, 0x30);
		DATA_FIELD(model_ptr, this, studiohdr_t*, 0x10E0);
		DATA_FIELD(view_offset, this, math::vec3_t, 0x1B68);
		DATA_FIELD(bone_matrix, this, math::matrix3x4a*, 0x0EE0);

		DATA_FIELD(eye_angles, this, math::ang3_t, 0x2188);
		DATA_FIELD(dynamic_angles, this, math::ang3_t, 0x2188 - 0x10);
		DATA_FIELD(simulation_tick, this, int, 0x784);

		NETVAR_FIELD(eye_angles2, this, math::ang3_t, "DT_CurrentData_Player", "m_angEyeAngles.x");

		DATA_FIELD(view_recoil, this, math::ang3_t, 0x20BC);
		DATA_FIELD(server_time, this, float, 0x1D18); // "m_currentFramePlayer.timeBase"
		DATA_FIELD(anim_sequence, this, int, 0xF2C); // "m_currentFrameBaseAnimating.animSequence"

		NETVAR_FIELD(identifier, this, PCHAR, "DT_BaseEntity", "m_iSignifierName");
		NETVAR_FIELD(script_name_index, this, int, "DT_BaseEntity", "m_scriptNameIndex");
		NETVAR_FIELD(model_index, this, int, "DT_BaseEntity", "m_nModelIndex");
		NETVAR_FIELD(thirdp_shoulder_view, this, bool, "DT_LocalPlayerExclusive", "m_thirdPersonShoulderView");

		NETVAR_FIELD_NR(pl, this, player_state_t*, "DT_Player", "pl");

		bool set_mdl_idx(int idx);
		bool set_legendary_model_idx(int idx);

		auto get_nucleus_id()
		{
			return *reinterpret_cast<uint64_t*>(BASE_OF(this) + 0x2220);
		}

		auto get_studio_hdr() -> studiohdr_t*
		{
			auto p = model_ptr();

			if (!p) return nullptr;

			return *reinterpret_cast<studiohdr_t**>( BASE_OF(p) + 0x8  );
		}

		auto get_identifier() -> char*
		{
			static auto offs_signifier_name = NETVAR("DT_BaseEntity", "m_iSignifierName");

			if (!offs_signifier_name) __debugbreak();

			auto identifier_addr = *reinterpret_cast<uintptr_t*>(BASE_OF(this) + offs_signifier_name);

			if (identifier_addr && !IsBadReadPtr(PTR_OF(identifier_addr), sizeof(void*)))
				return reinterpret_cast<char*>(identifier_addr);

			return nullptr;
		}

		math::vec3_t get_bone_pos(int id)
		{
			auto bm = bone_matrix();

			if (bm == nullptr) return { };

			return math::vec3_t{
				bm[id][0][3],
				bm[id][1][3],
				bm[id][2][3]
			} + get_abs_origin();
		}

		math::vec3_t& get_abs_velocity()
		{
			using fn = math::vec3_t& (__fastcall*)(void*);
			return reinterpret_cast<fn>(MOD_BASE("r5apex.exe") + 0x5a9910)(this);
		}

		auto set_valid_health_bar_target(bool b)
		{
			using fn = void(__fastcall*)(void*, bool);
			auto func = reinterpret_cast<fn>(MOD_BASE("r5apex.exe") + 0xE8E2C0);

			return func(this, b);
		}
	};

	class c_player : public c_entity
	{
	public:
		NETVAR_FIELD(life_state, this, uint8_t, "DT_Player", "m_lifeState");
		NETVAR_FIELD(health, this, int, "DT_Player", "m_iHealth");
		NETVAR_FIELD(health_max, this, int, "DT_Player", "m_iMaxHealth");
		NETVAR_FIELD(armor, this, int, "DT_BaseEntity", "m_shieldHealth");
		NETVAR_FIELD(armor_max, this, int, "DT_BaseEntity", "m_shieldHealthMax");
		NETVAR_FIELD(observer_mode, this, int, "DT_Player", "m_iObserverMode");
		NETVAR_FIELD(observer_target, this, int, "DT_Player", "m_hObserverTarget");
		NETVAR_FIELD(view_model, this, int, "DT_Player", "m_hViewModels");
		NETVAR_FIELD(latest_primary_weapon, this, ulong, "DT_BaseCombatCharacter", "m_latestPrimaryWeapons");
		NETVAR_FIELD(is_zooming, this, bool, "DT_Player", "m_bZooming");
		NETVAR_FIELD(flags, this, int, "DT_Player", "m_fFlags");
		NETVAR_FIELD(last_fired_time, this, float, "DT_BaseCombatCharacter", "m_lastFiredTime");

		auto is_alive() { return life_state() == 0; }

		c_view_model* get_view_model() noexcept;
		c_weapon* get_active_weapon() noexcept;
		int get_observer_target_idx() noexcept;
		respawn::mstudiobbox_t get_hitbox(int i) noexcept;
		math::vec3_t get_hitbox_pos(int i) noexcept;
		void start_observer_mode(int mode) noexcept;
		void set_observer_target(void* entity) noexcept;
	};
}
