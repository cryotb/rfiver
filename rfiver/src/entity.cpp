#include <include.h>

using namespace respawn;

c_weapon* c_player::get_active_weapon() noexcept
{
	int wpn_index{ };
	ulong wpn_handle = latest_primary_weapon();

	if (!wpn_handle)
		return nullptr;

	wpn_index = ENTITY_INDEX_BY_HANDLE(wpn_handle);

	if (!wpn_index)
		return nullptr;

	return (c_weapon*)ix::ent_list->GetClientEntity(wpn_index);
}

c_view_model* c_player::get_view_model() noexcept
{
	int vm_index{ };
	ulong vm_handle = view_model();

	if (!vm_handle)
		return nullptr;

	vm_index = ENTITY_INDEX_BY_HANDLE(vm_handle);

	if (!vm_index)
		return nullptr;

	return (c_view_model*)ix::ent_list->GetClientEntity(vm_index);

}

respawn::mstudiobbox_t c_player::get_hitbox(int i) noexcept
{
	auto shdr = get_studio_hdr();

	if (shdr)
	{
		auto hb = shdr->get_hitbox(0, i);
		auto bm = bone_matrix();

		if (hb && bm)
			return *hb;
	}

	return { };
}

math::vec3_t c_player::get_hitbox_pos(int i) noexcept
{
	auto shdr = get_studio_hdr();

	if (shdr)
	{
		auto hb = shdr->get_hitbox(0, i);
		auto bm = bone_matrix();

		if (hb && bm)
		{
			auto center = (hb->min + hb->max) * 0.5f;
			auto result = math::vec3_t{ };

			if (hb->bone < shdr->num_bones)
			{
				math::vector_transform(center, bm[hb->bone], result);

				return get_abs_origin() + result;
			}
		}
	}

	return { };
}

void c_player::start_observer_mode(int mode) noexcept
{
	auto addr = MOD_BASE("r5apex.exe") + 0xDD5F20;

	msg("start observer(%p) => %i", addr, mode);

	return reinterpret_cast<void(__fastcall*)(void*, int)>(addr)(this, mode);
}

void c_player::set_observer_target(void* entity) noexcept
{
	auto addr = MOD_BASE("r5apex.exe") + 0xDD60F0;

	msg("set observer(%p) => %p", addr, entity);

	return reinterpret_cast<void(__fastcall*)(void*, void*)>(addr)(this, entity);
}

int c_player::get_observer_target_idx() noexcept
{
	auto handle = observer_target();

	if (!handle)
		return -1;

	return ENTITY_INDEX_BY_HANDLE(handle);
}

bool c_entity::set_mdl_idx(int idx)
{
	auto addr = MOD_BASE("r5apex.exe") + 0x7DBE00;
	return reinterpret_cast<bool(__fastcall*)(void*, int)>(addr)(this, idx);
}

bool c_entity::set_legendary_model_idx(int idx)
{
	auto addr = MOD_BASE("r5apex.exe") + 0xA3CF80;
	return reinterpret_cast<bool(__fastcall*)(void*, int)>(addr)(this, idx);
}
