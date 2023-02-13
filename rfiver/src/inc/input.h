#pragma once

enum button_states
{
	button_state_invalid = -1,
	button_state_unassigned = 0,

	button_state_released,
	button_state_busy,

	button_state_count,
};

struct button_t
{
	int m_state = -1;
	bool m_pressed = false;
};

class c_synced_input
{
public:
	void sync(std::array<button_t, 255>& buttons);
	bool is_button_down(uintptr_t key_code) noexcept;
	bool was_button_down(uintptr_t key_code) noexcept;
private:
	std::array<button_t, 255> m_buttons;
	std::array<button_t, 255> m_buttons_prev;
};

class c_input_manager
{
public:
	void update();
	void window_proc(UINT msg, WPARAM w_param, LPARAM l_param);
	void sync(c_synced_input* si);
public:
	bool is_button_down(uintptr_t key_code) noexcept;
	math::position_t<int> get_cursor_pos() noexcept;
private:
	void internal_handler_key_down(uintptr_t key_code);
	void internal_handler_key_up(uintptr_t key_code);
private:
	math::position_t<int> m_cursor_pos;

	std::array<button_t, 255> m_buttons;
	std::array<button_t, 255> m_buttons_prev;
};

extern c_input_manager g_input_mgr;