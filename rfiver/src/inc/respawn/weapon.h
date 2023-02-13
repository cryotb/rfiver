#pragma once

namespace respawn
{
	enum class weapon_name_indexes : int
	{
		hands = 1,
		heirloom = 51,
	};

	class c_weapon : public c_entity
	{
	public:
		NETVAR_FIELD(is_reloading, this, bool, "DT_WeaponX_LocalWeaponData", "m_bInReload");
		NETVAR_FIELD(next_primary_attack, this, float, "DT_WeaponX_LocalWeaponData", "m_nextPrimaryAttackTime");
		NETVAR_FIELD(next_ready_time, this, float, "DT_WeaponX_LocalWeaponData", "m_nextReadyTime");
		NETVAR_FIELD(ammo_in_clip, this, int, "DT_WeaponX_LocalWeaponData", "m_ammoInClip");
		NETVAR_FIELD(name_index, this, weapon_name_indexes, "DT_WeaponX", "m_weaponNameIndex");

		DATA_FIELD(projectile_speed, this, float, 0x1B3C);
		DATA_FIELD(projectile_gravity, this, float, 0x1CB8);
		DATA_FIELD(shots_fired, this, int, 0x2A4C);

		auto is_hands() { return name_index() == weapon_name_indexes::hands; }

		bool can_fire(c_player* owner) noexcept;

		auto is_melee() noexcept
		{
			auto idx = name_index();

			return (idx == weapon_name_indexes::hands ||
					idx == weapon_name_indexes::heirloom);
		}
	private:
	};
}
