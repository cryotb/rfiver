#include <include.h>

using e_type = cache::object_t::type;

void respawn::debug_draw_line(
	const math::vec3_t& start,
	const math::vec3_t& end,
	int r,
	int g,
	int b,
	bool thru_world,
	float duration
)
{
	static auto addr = MOD_BASE("r5apex.exe") + 0x8CED60;
	static auto func = reinterpret_cast<decltype(&debug_draw_line)>(addr);

	return func(start, end, r, g, b, thru_world, duration);
}

void respawn::debug_draw_text(
	const math::vec3_t& pos,
	const char* text,
	bool depth_test,
	float duration
)
{
	static auto addr = MOD_BASE("r5apex.exe") + 0xB769C0;
	static auto func = reinterpret_cast<decltype(&debug_draw_text)>(addr);

	return func(pos, text, depth_test, duration);
}

// #STR: "Client_ScriptDebugDrawBox"
void respawn::debug_draw_box(
	const math::vec3_t& origin,
	const math::vec3_t& mins,
	const math::vec3_t& maxs,
	int r, int g, int b, int a,
	float duration
)
{
	static auto addr = MOD_BASE("r5apex.exe") + 0x8CED10;
	static auto func = reinterpret_cast<decltype(&debug_draw_box)>(addr);

	return func(origin, mins, maxs, r, g, b, a, duration);
}

respawn::weapons respawn::get_weapon_def(c_view_model* vm, c_weapon* wpn)
{
	if (wpn)
	{
		if (auto mdl_path = wpn->model_name())
		{
			switch (utl::string_hash(mdl_path, strlen(mdl_path)))
			{
			case 0x977677cf: return weapons::arc_star;
			}
		}

		return (weapons)wpn->name_index();
	}

	return weapons::invalid;
}

respawn::weapon_types respawn::get_weapon_type(weapons wpn)
{
	switch (wpn)
	{
	case weapons::sg_eva8:
	case weapons::sg_mastiff:
	case weapons::sg_mozam:
	case weapons::sg_peace_keeper:
		return weapon_types::shotgun;
	case weapons::sn_charge:
	case weapons::sn_kraber:
	case weapons::sn_longbow:
	case weapons::sn_take_triple:
	case weapons::sn_sentinel:
	case weapons::sn_wingman:
		return weapon_types::sniper;
	default:
		return weapon_types::invalid;
	}
}

cache::object_t::type respawn::detect_entity_type(respawn::c_entity* entity)
{
	auto identifier = entity->get_identifier();

	if (identifier)
	{
		auto hash = *reinterpret_cast<uint64_t*>(identifier);

		switch (hash)
		{
		case 0x726579616c70: return e_type::player;
		case 0x6d6d75645f63706e: return e_type::dummie;
		}
	}

	return e_type::none;
}

int32_t respawn::get_main_thread_id()
{
	static int32_t val = 0;

	if (!val)
		val = (int32_t)utl::get_process_main_thread((HANDLE)GetCurrentProcessId());

	return val;
}

bool respawn::load_script(__int64 pSQVM, const char* path, __int64 name, int unk0)
{
	static auto addr = MOD_BASE("r5apex.exe") + 0x1055630;

	return GET_PTYPE(load_script)(addr)(pSQVM, path, name, unk0);
}

void respawn::execute_sqc(const char* code, SQCONTEXT mode)
{
	static auto addr = find_ptr(MOD_BASE("gamesdk.dll"), "E9 ? ? ? ? 48 8D 0D ? ? ? ? BA ? ? ? ? E9 ? ? ? ? C3", 1, 5);

	return GET_PTYPE(execute_sqc)(addr)(code, mode);
}

void* respawn::create_iface(const char* name, uintptr_t null)
{
	auto addr = reinterpret_cast<decltype(&create_iface)>(MOD_BASE("r5apex.exe") + 0x472710);

	return addr(name, null);
}

bool respawn::world_to_screen(const math::vec3_t& vIn, math::vec3_t& vOut)
{
	auto pvMatrix = ix::view_render->get_view_matrix();

	if (pvMatrix == nullptr)
		return false;

	auto vMatrix = *pvMatrix;

	vOut.x = vMatrix.m[0][0] * vIn.x + vMatrix.m[0][1] * vIn.y + vMatrix.m[0][2] * vIn.z + vMatrix.m[0][3];
	vOut.y = vMatrix.m[1][0] * vIn.x + vMatrix.m[1][1] * vIn.y + vMatrix.m[1][2] * vIn.z + vMatrix.m[1][3];

	float w = vMatrix.m[3][0] * vIn.x + vMatrix.m[3][1] * vIn.y + vMatrix.m[3][2] * vIn.z + vMatrix.m[3][3];

	if (w < 0.01)
		return false;

	float invw = 1.0f / w;

	vOut.x *= invw;
	vOut.y *= invw;

	const auto Cl_width = g::screen.w;
	const auto Cl_height = g::screen.h;

	auto x = static_cast<float>(Cl_width) / 2.f;
	auto y = static_cast<float>(Cl_height) / 2.f;

	x += 0.5f * vOut.x * Cl_width + 0.5f;
	y -= 0.5f * vOut.y * Cl_height + 0.5f;

	vOut.x = x;
	vOut.y = y;

	return true;
}

respawn::c_player* respawn::local_player()
{
	auto idx = ix::engine_client->get_local_player_idx();

	if (idx >= 0)
	{
		auto ptr = (respawn::c_player*)ix::ent_list->GetClientEntity(idx);

		return ptr;
	}

	return nullptr;
}

respawn::netchan_t* respawn::net_chan()
{
	return *g_gv.net_chan;
}

std::string respawn::get_client_user_name()
{
	auto addr = MOD_BASE("r5apex.exe") + 0xc18f028;

	char buf[32];

	memset(buf, 0, sizeof(buf));
	memcpy(buf, (void*)addr, sizeof(buf));
	buf[sizeof(buf) - 1] = '\0';

	return std::string(buf);
}

bool respawn::can_fire(respawn::c_player* me)
{
	auto wpn = me->get_active_weapon();

	if (!wpn) return false;
	if (wpn->is_reloading()) return false;

	return true;
}

bool respawn::check_watermark(std::string& data_str)
{
	try
	{
		auto pos_wm = data_str.find("beta_watermark");

		if (pos_wm != std::string::npos)
		{
			auto curr = pos_wm;
			auto tries = 120;

			do
			{
				if (data_str.at(curr) == '1')
				{
					data_str.at(curr) = '0';

					INGAME_LOGGER_MSG(5.f, im::color::yellow, "server has forced watermark, disabled it!");
					return true;
				}

				curr++;
				tries--;
			} while (tries > 0);
		}
	}
	catch (...)
	{
		DBG_BREAK();
	}

	return false;
}

bool respawn::is_third_person()
{
	return (cvar::third_person->m_value_numeric == 1);
}

void respawn::cbuf_add_text(const char* cmd)
{
	using fn = void(__fastcall*)(int, const void*, int);
	static auto addr = find_ptr(MOD_BASE("r5apex.exe"), "E8 ? ? ? ? FF 05 ? ? ? ? 48 81 C4 ? ? ? ? C3", 1, 5);

	return reinterpret_cast<fn>(addr)(0, cmd, 0);
}

void respawn::net_set_key(const char* key)
{
	static auto ptr = *reinterpret_cast<void**>(resolve_addr_to_rel(
		find_pattern(g::sdk_base, g::sdk_length, "48 8D 0D ? ? ? ? 66 0F 7F 45"),
		3, 7
	));

	using fn = void(__fastcall*)(void* ptr, const char*);
	static auto addr = MOD_BASE("r5apex.exe") + 0x263E70;

	return reinterpret_cast<fn>(addr)(ptr, key);
}

void respawn::connect(std::string addr, std::string port, std::string key)
{
	net_set_key(key.c_str());

	respawn::cbuf_add_text(fmt::format("{:s} \"[{:s}]:{:s}\"", "connect", addr, port).c_str());
}

void respawn::dev_msg(int lvl, const char* fmt, ...)
{
	return reinterpret_cast<decltype(&dev_msg)>(GetProcAddress(GetModuleHandle("gamesdk.dll"), "DevMsg"))(lvl, fmt);
}

respawn::native_t* respawn::detail::nt_find(u64 list_base, const char* name)
{
	auto curr = reinterpret_cast<native_t*>(list_base);

	while ( curr != nullptr && curr->m_name != nullptr )
	{
		if (strcmp(curr->m_name, name) == 0)
			return curr;

		curr++;
	}

	return nullptr;
}

respawn::native_t* respawn::nt_find(const char* name)
{
	auto list1 = *reinterpret_cast<u64*>(MOD_BASE("r5apex.exe") + 0x1f30d10);
	auto list2 = *reinterpret_cast<u64*>(MOD_BASE("r5apex.exe") + 0x1f2f8e0);

	if (list1 && list2)
	{
		if (auto rs = detail::nt_find(list1, name))
			return rs;

		if (auto rs = detail::nt_find(list2, name))
			return rs;
	}

	return nullptr;
}

void respawn::c_srv_list_mgr::launch_server()
{
	using fn = void(__fastcall*)(void*);
	static auto addr = find_ptr(g::sdk_base, "E8 ? ? ? ? EB 24 41 B8", 1, 5);

	return reinterpret_cast<fn>(addr)(this);
}
