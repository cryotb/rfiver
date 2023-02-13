#pragma once

namespace respawn
{
	class c_model : public c_entity
	{
	public:
	private:
	};

	class c_model_info
	{
	public:
		c_model* get_model(int idx)
		{
			return vfunc::call<1, c_model*>(this, idx);
		}

		int get_model_index(const char* nm)
		{
			return vfunc::call<2, int>(this, nm);
		}

		const char* get_model_name(c_model* mdl)
		{
			return vfunc::call<3, const char*>(this, mdl);
		}
	private:
	};

}
