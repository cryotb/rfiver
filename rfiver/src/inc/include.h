#pragma once

//
// Standard Library
//
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <array>
#include <intrin.h>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <functional>

#define _USE_MATH_DEFINES
#define FMT_HEADER_ONLY 
#include <math.h>
#include <curl/curl.h>
#include <fmt/format.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

//
// Windows
//
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>

//
// External Deps
//
#include "minhook/MinHook.h"
#include <d3d11.h>

// imgui here
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

//
// Internal Deps & Tools
//
#include "defs.h"
#include "x64.h"
#include "drf.h"
#include "log.h"
#include "stl.h"
#include "nt.h"
#include "xorstr.h"
#include "vmtcall.h"
#include "util.h"
#include "vmthk.h"
#include "mhpp.h"
#include "smath.h"
#include "input.h"
#include "memory.h"
#include "signature.h"
#include "pe.h"
#include "r5/r5.h"
#include <hde/hde.h>
#include "hde.h"
#include "respawn/respawn.h"
#include "render.h"
#include "windows/windows.h"
#include "config.h"
#include "imgui/imgui_hlp.h"

//
// Project Files
//
#include "global.h"
#include "iface.h"
#include "cache.h"
#include "features.h"
#include "core.h"
#include "hooks.h"
#include "spectator_mgr.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline auto get_pe_length(void* base)
{
	return reinterpret_cast<PIMAGE_NT_HEADERS>(
		BASE_OF(base) + reinterpret_cast<PIMAGE_DOS_HEADER>(base)->e_lfanew
		)->OptionalHeader.SizeOfImage;
}

inline auto terminate_with_notice(const char* reason)
{
	SuspendThread(OpenThread(THREAD_SUSPEND_RESUME, FALSE, respawn::get_main_thread_id()));
	MessageBox(nullptr,
		reason, _XS("ERROR"),
		MB_ICONERROR
	);

	__fastfail(0x133);
}

class c_game_thread_callbacks
{
public:
	c_game_thread_callbacks()
	{
		InitializeCriticalSection(&m_cs);
	}

	~c_game_thread_callbacks()
	{
		DeleteCriticalSection(&m_cs);
	}

	auto add(void(*func)())
	{
		m_list.push_back(func);
	}

	auto process()
	{
		for (auto cb : m_list)
		{
			cb();
			m_list.pop_back();
		}
	}
private:
	CRITICAL_SECTION m_cs;
	std::vector< std::function<void()> > m_list;
};

extern c_game_thread_callbacks* g_gtc;

template < typename T >
inline bool is_within_self(T addr)
{
	u64 my_start = g::self.base;
	u64 my_end = my_start + g::self.length;

	return (BASE_OF(addr) >= my_start && BASE_OF(addr) < my_end);
}

inline auto __build_id()
{
	auto c_date = __DATE__;
	auto c_time = __TIME__;

	auto h_date = utl::string_hash(c_date, strlen(c_date));
	auto h_time = utl::string_hash(c_time, strlen(c_time));

	LARGE_INTEGER li;
	li.LowPart = h_date;
	li.HighPart = h_time;

	return li.QuadPart;
}
