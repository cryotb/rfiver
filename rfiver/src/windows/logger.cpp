#include <include.h>

using namespace wnd;

void c_logger::paint()
{
	draw_messages();
}

void c_logger::draw_messages()
{
	std::lock_guard _(m_mtx);

	float push_y = 0.0f;
	float start_x = 2.0f, start_y = 2.0f;

	for (auto it = m_fields.begin(); it != m_fields.end();)
	{
		const auto& entry = *it;

		bool requires_removal = (entry.m_time_registered && llabs(entry.m_time_registered - TICK_COUNT()) > entry.m_duration);
		auto text_size = ImGui::CalcTextSize(entry.m_text);

		render::rect_filled({ start_x, start_y + push_y },
			{ start_x + 1 + text_size.x,
			start_y + push_y + text_size.y + 1.0f },
			ImColor(5, 5, 5)
		);

		render::text({ start_x + 1, start_y + push_y }, entry.m_color, entry.m_text);

		if (requires_removal)
		{
			auto rm = std::remove_if(m_fields.begin(), m_fields.end(), [&](const field_t& other)
				{
					return other.m_id == entry.m_id;
				});

			it = m_fields.erase(rm);

			continue;
		}
		else
		{
			++it;
		}

		push_y += text_size.y;
	}
}

void c_logger::add_message(int duration, ImColor color, const char* fmt, ...)
{
	std::lock_guard _(m_mtx);

	if (!duration || !fmt)
		return;

	va_list list{ };
	field_t entry{ };

	memset(&entry, 0, sizeof(entry));

	if (strlen(fmt) >= (sizeof(entry.m_text) - 1))
		return;

	entry.m_id = (int)m_fields.size();
	entry.m_duration = duration * 1000;
	entry.m_color = color;
	entry.m_time_registered = TICK_COUNT();

	va_start(list, fmt);
	vsnprintf(entry.m_text, sizeof(entry.m_text), fmt, list);
	va_end(list);

	m_fields.push_back(entry);
}
