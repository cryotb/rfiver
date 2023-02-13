#pragma once

namespace im
{
	class table
	{
	public:
		static bool begin(const char* name, int col, ImGuiTableFlags flags = 0)
		{
			auto result = ImGui::BeginTable(name, col, flags);

			if (result)
				down();

			return result;
		}

		static bool begin_ex(const char* name, int col, ImGuiTableFlags flags = 0, ImVec2 size = { 150, 150 })
		{
			auto result = ImGui::BeginTable(name, col, flags, size);

			if (result)
				down();

			return result;
		}

		static void end()
		{
			ImGui::EndTable();
		}

		static void right()
		{
			ImGui::TableNextColumn();
		}

		static void down()
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
		}
	private:
	};
}
