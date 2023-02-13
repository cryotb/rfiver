#include <include.h>

config_t g_cfg{ };

bool cfg::save(config_t* p, const char* path)
{
	p->magic = magic_;
	p->version = g::self.version;

	if (!utl::aob_save_to_disk(p, sizeof(config_t), path))
	{
		INGAME_LOGGER_MSG(5.f, im::color::red, "cfg::save(%s) => failed to write it!", path);
		return false;
	}

	INGAME_LOGGER_MSG(5.f, im::color::green, "cfg::save() success!");

	return true;
}

bool cfg::load(config_t* p, const char* path)
{
	auto pdata = std::vector<uint8_t>();

	if (!utl::aob_read_from_memory(path, &pdata))
	{
		INGAME_LOGGER_MSG(5.f, im::color::red, "cfg::load(%s) => failed to read it!", path);
		return false;
	}

	auto data = reinterpret_cast<config_t*>(pdata.data());

	if (data->magic != magic_)
	{
		INGAME_LOGGER_MSG(5.f, im::color::red, "cfg::load(%s) => unmatched magic!", path);
		return false;
	}

	if (!IS_DEBUG_BUILD() && (!data->version || data->version != g::self.version))
	{
		INGAME_LOGGER_MSG(5.f, im::color::yellow, "cfg::load(%s) => config uses older version!", path);
	}

	INGAME_LOGGER_MSG(5.f, im::color::green, "cfg::load() success!");

	*p = *data;

	return true;
}
