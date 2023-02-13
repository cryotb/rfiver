#include <include.h>

struct dbg_op_t
{
	bool operator==(const dbg_op_t& other) const
	{
		return other.uid == this->uid;
	}

	enum class types
	{
		unset = 0,

		text,
		line,
		rect,
		rect_filled,
		circle,
		circle_filled,
	};

	int uid;
	types type;
	ImVec2 pos1;
	ImVec2 pos2;
	ImU32 col;
	std::string fmt;
	float duration;
	float register_time;
};

std::list<dbg_op_t> dbg_queue;
CRITICAL_SECTION cs;

void render::debug_queue()
{
	static bool init = false;

	if (!init)
	{
		InitializeCriticalSection(&cs);
		init = true;
		return;
	}

	EnterCriticalSection(&cs);

	for (const auto& obj : dbg_queue)
	{
		auto time_delta = (*g_gv.time - obj.register_time);

		if (obj.register_time != 0.f && time_delta > obj.duration)
		{
			dbg_queue.remove(obj);
			break;
		}

		switch (obj.type)
		{
		case dbg_op_t::types::line:
			render::line(obj.pos1, obj.pos2, obj.col);
			break;
		}
	}

	LeaveCriticalSection(&cs);
}

void render::dbg_line(float dur, ImVec2 p1, ImVec2 p2, ImU32 col)
{
	EnterCriticalSection(&cs);

	dbg_op_t op;

	op.uid = (int)dbg_queue.size();
	op.pos1 = p1;
	op.pos2 = p2;
	op.col = col;
	op.register_time = *g_gv.time;
	op.duration = dur;
	op.type = dbg_op_t::types::line;

	dbg_queue.push_back(op);

	LeaveCriticalSection(&cs);
}

void render::text(ImVec2 pos, ImU32 col, const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	ZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	ImGui::GetBackgroundDrawList()->AddText(pos, col, buf);
}

void render::text_centered(ImVec2 pos, ImU32 col, const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	ZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	float text_width = ImGui::CalcTextSize(buf).x;

	pos.x -= text_width * 0.5f;

	ImGui::GetBackgroundDrawList()->AddText(pos, col, buf);
}

void render::text_centered_bg(ImVec2 pos, ImU32 col, const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	ZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	auto text_dim = ImGui::CalcTextSize(buf);

	const auto& text_height = text_dim.y;
	const auto& text_width = text_dim.x;

	pos.x -= text_width * 0.5f;

	float xscale = (text_width);
	float yscale = (text_height);

	rect_filled(
		{ pos.x - 2.f, pos.y - 2.f }, 
		{ pos.x + xscale, pos.y + yscale + 2.f }, 
		ImColor(5, 5, 5, 200)
	);

	ImGui::GetBackgroundDrawList()->AddText(pos, col, buf);
}
