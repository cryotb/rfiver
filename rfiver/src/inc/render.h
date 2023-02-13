#pragma once

namespace render
{
	extern void debug_queue();

	extern void dbg_line(float dur, ImVec2 p1, ImVec2 p2, ImU32 col);

	extern void text(ImVec2 pos, ImU32 col, const char* fmt, ...);
	extern void text_centered(ImVec2 pos, ImU32 col, const char* fmt, ...);

	extern void text_centered_bg(ImVec2 pos, ImU32 col, const char* fmt, ...);

	inline void line(ImVec2 p1, ImVec2 p2, ImU32 col)
	{
		ImGui::GetBackgroundDrawList()->AddLine(p1, p2, col);
	}

	inline void rect(ImVec2 vmin, ImVec2 vmax, ImU32 col)
	{
		ImGui::GetBackgroundDrawList()->AddRect(vmin, vmax, col);
	}

	inline void rect_filled(ImVec2 vmin, ImVec2 vmax, ImU32 col)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(vmin, vmax, col);
	}

	inline void circle(ImVec2 center, float radius, int nseg, ImU32 col)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(center, radius, col, nseg);
	}
}
