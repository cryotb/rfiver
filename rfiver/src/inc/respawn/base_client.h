#pragma once

namespace respawn
{
	class c_base_client
	{
	public:
		client_class* get_all_classes()
		{
			return vfunc::call<12, client_class*>(this);
		}
	};
}
