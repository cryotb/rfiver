#pragma once

namespace wnd
{
	class c_logger : public c_base
	{
	public:
		struct field_t
		{
			int m_id;
			char m_text[256];
			int m_duration;
			uint64_t m_time_registered;
			ImColor m_color;
		};

		void draw_messages();
		void paint() override;

		void add_message(int duration, ImColor color, const char* fmt, ...);
	private:
		std::mutex m_mtx;
		std::vector<field_t> m_fields;
	};
}

#define INGAME_LOGGER_MSG(DURATION, COLOR, TEXT, ...) wnds::logger.add_message(DURATION, COLOR, _XS(TEXT), __VA_ARGS__)
