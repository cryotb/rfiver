#pragma once

#include <array>

class c_ent_cache
{
public:
	const auto& objects() { return _objects; }
	const auto& objects_prev() { return _objects_prev; }

	void init();
	void update();

	auto local_object() -> const cache::object_t*
	{
		auto idx = ix::engine_client->get_local_player_idx();

		if (idx)
		{
			return &_objects[idx];
		}

		return nullptr;
	}

	auto reset()
	{
		for (auto& obj : _objects)
		{
			if (obj.ds) { delete obj.ds; obj.ds = nullptr; }
			if (obj.ds_prev) { delete obj.ds_prev;  obj.ds_prev = nullptr; }
		}

		memset(_objects.data(), 0, _objects.size() * sizeof(cache::object_t));
	}

	ImColor object_color(const cache::object_t& obj);
private:
	std::vector<cache::object_t> _objects;
	std::vector<cache::object_t> _objects_prev;
};

#define CE_IS_VALID( X ) ( X.ds != nullptr && X.ds_prev != nullptr )
#define CE_IS_VALID_PTR( X ) CE_IS_VALID( (*X) )
#define CE_IS_TYPE_PLAYER( X ) ( X.hdr.m_type == cache::object_t::type::player )

extern c_ent_cache g_ent_cache;
