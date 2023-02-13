#include <include.h>

c_input_manager g_input_mgr;

void c_synced_input::sync(std::array<button_t, 255>& buttons)
{
	m_buttons_prev = m_buttons;
	m_buttons = buttons;
}

bool c_synced_input::is_button_down(uintptr_t key_code) noexcept
{
	return (m_buttons[key_code].m_state == button_state_busy);
}

bool c_synced_input::was_button_down(uintptr_t key_code) noexcept
{
	return (m_buttons_prev[key_code].m_state != button_state_busy && m_buttons[key_code].m_state ==
		button_state_busy);
}

void c_input_manager::update()
{
	ix::input_system->get_cursor_pos(&m_cursor_pos.x, &m_cursor_pos.y);
}

void c_input_manager::window_proc(UINT msg, WPARAM w_param, LPARAM l_param)
{
	m_buttons_prev = m_buttons;

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		if (!(HIWORD(l_param) & KF_REPEAT))
			internal_handler_key_down(w_param);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (!(HIWORD(l_param) & KF_REPEAT))
			internal_handler_key_down(VK_LBUTTON);

		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (!(HIWORD(l_param) & KF_REPEAT))
			internal_handler_key_down(VK_RBUTTON);

		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (!(HIWORD(l_param) & KF_REPEAT))
			internal_handler_key_down(VK_MBUTTON);

		break;
	}
	case WM_XBUTTONDOWN:
	{
		if (!(HIWORD(l_param) & KF_REPEAT))
		{
			auto xb_index = GET_XBUTTON_WPARAM(w_param);

			internal_handler_key_down(((xb_index == 2) ? VK_XBUTTON2 : VK_XBUTTON1));
		}

		break;
	}
	case WM_KEYUP:
	{
		internal_handler_key_up(w_param);

		break;
	}
	case WM_LBUTTONUP:
	{
		internal_handler_key_up(VK_LBUTTON);

		break;
	}
	case WM_RBUTTONUP:
	{
		internal_handler_key_up(VK_RBUTTON);

		break;
	}
	case WM_MBUTTONUP:
	{
		internal_handler_key_up(VK_MBUTTON);

		break;
	}
	case WM_XBUTTONUP:
	{
		auto xb_index = GET_XBUTTON_WPARAM(w_param);

		internal_handler_key_up(((xb_index == 2) ? VK_XBUTTON2 : VK_XBUTTON1));

		break;
	}
	default:
		break;
	}
}

void c_input_manager::sync(c_synced_input* si)
{
	si->sync(m_buttons);
}

void c_input_manager::internal_handler_key_down(uintptr_t key_code)
{
	m_buttons[key_code].m_state = button_state_busy;
	m_buttons[key_code].m_pressed = true;
}

void c_input_manager::internal_handler_key_up(uintptr_t key_code)
{
	m_buttons[key_code].m_state = button_state_released;
	m_buttons[key_code].m_pressed = false;
}

bool c_input_manager::is_button_down(uintptr_t key_code) noexcept
{
	return (m_buttons[key_code].m_state == button_state_busy);
}

math::position_t<int> c_input_manager::get_cursor_pos() noexcept
{
	return m_cursor_pos;
}

