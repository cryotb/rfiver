#include <include.h>

struct recv_proxy_data_t
{
	const respawn::recv_prop* m_recv_prop;		// The property it's receiving.
	respawn::d_variant_t		m_value;		// The value given to you to store.
	int				m_element;					// Which array element you're getting.
	int				m_object_id;				// The object being referred to.
};

void rp_local_player_eye_angles(const recv_proxy_data_t* data, void* stru, void* out)
{
	msg("yikes");
}

bool c_netvar_manager::initialize()
{
	auto* const head = ix::base_client->get_all_classes();

	if (head == nullptr)
		return false;

	msg("initializing the netvar table.");

	for (auto* client_class = head; client_class != nullptr; client_class = client_class->next)
	{
		auto* const table = client_class->table;

		if (table == nullptr)
			continue;

		process_table(table);
	}

	msg("netvar table was set up, there is a total of %i tables.", m_list.size());

	return true;
}

bool c_netvar_manager::is_prop_garbage(const char* name)
{
	if (strstr(name, "baseclass") ||
		strstr(name, "0") ||
		strstr(name, "1") ||
		strstr(name, "2"))
		return true;

	return false;
}

void c_netvar_manager::process_table(respawn::recv_table* table)
{
	auto** const head = table->props;

	if (head == nullptr)
		return;

	netvar_table_t table_entry{};

	strcpy(table_entry.name, table->name);

	for (auto i = 0ull; i < table->num_props; i++)
	{
		auto* const prop = head[i];

		//
		// validate the prop pointer, at first.
		//
		if (prop == nullptr)
			continue;

		//
		// validate that the prop is not garbage data.
		//
		if (is_prop_garbage(prop->name))
			continue;

		//
		// process the actual prop here.
		//
		process_prop(table, prop, table_entry.props);

		//
		// if this prop has a table, then recursively process it.
		//
		if (prop->data_table != nullptr)
			process_table(prop->data_table);
	}

	m_list.push_back(table_entry);
}

void c_netvar_manager::process_prop(respawn::recv_table* table, respawn::recv_prop* prop, std::vector<netvar_t>& list)
{
	netvar_t entry{};

	strcpy(entry.name, prop->name);
	entry.offset = prop->offset;
	entry.ptr = prop;

	list.push_back(entry);
}
