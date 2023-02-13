#pragma once

struct netvar_t
{
	char name[128];
	ulong offset;
	respawn::recv_prop* ptr;
};

struct netvar_table_t
{
	char name[128];
	std::vector<netvar_t> props;
};

class c_netvar_manager
{
public:
	bool initialize();
	bool is_prop_garbage(const char* name);

	void process_table(respawn::recv_table* table);
	void process_prop(respawn::recv_table* table, respawn::recv_prop* prop, std::vector<netvar_t>& list);

	ulong get(const char* table_name, const char* prop_name) noexcept
	{
		std::vector<netvar_t>* props_head = nullptr;

		for (auto i = 0ull; i < m_list.size(); i++)
		{
			auto* const table = &m_list[i];

			if (table == nullptr)
				continue;

			if (strcmp(table_name, table->name))
				continue;

			props_head = &table->props;
		}

		if (props_head == nullptr)
			return 0;

		ulong result{};

		for (auto i = 0ull; i < props_head->size(); i++)
		{
			auto* const prop = &(*props_head)[i];

			if (!strcmp(prop_name, prop->name))
			{
				result = prop->offset;
				break;
			}
		}

		return result;
	}

	bool setup_recv_propxy(const char* table_name, const char* prop_name, void* func)
	{
		for (const auto& dt : m_list)
		{
			if (!strcmp(dt.name, table_name))
			{
				for (const auto& p : dt.props)
				{
					if (!strcmp(p.name, prop_name) && p.ptr)
					{
						msg("setup_recv_proxy -> found %s->%s at %p", table_name, prop_name, p.ptr);
						p.ptr->proxy_fn = func;

						return true;
					}
				}
			}
		}

		return false;
	}
private:
	std::vector<netvar_table_t> m_list;
};

#define NETVAR(TABLE, NAME) g::netvar_mgr.get(_XS(TABLE), _XS(NAME))
#define NETVAR_X(TABLE, NAME) static ulong __##NAME = NETVAR(TABLE, NAME)
