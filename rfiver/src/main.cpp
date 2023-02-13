#include <include.h>

auto get_backed_module(uintptr_t addr) -> utl::module_t
{
	auto list = utl::get_process_modules(GetCurrentProcess());

	for (const auto& m : list)
	{
		if (addr >= m.m_base && addr < (m.m_base + m.m_size))
			return m;
	}

	if (addr >= g::self.base && addr < (g::self.base + g::self.length))
	{
		/* when we're mapped unconventionally, create a fake module which
		* represents the manually loaded image in memory as our selves. */

		utl::module_t mod;
		mod.m_base = g::self.base;
		mod.m_size = g::self.length;
		mod.m_name = "rfiver_MM.dll";
		return mod;
	}

	return { };
}

extern "C"
{
	char buf[512];

	LONG VeHandler(PEXCEPTION_POINTERS ep)
	{
		DRFAR_FORCE_STACK_FUCK();

		auto ctx = ep->ContextRecord;
		auto exc = ep->ExceptionRecord;

		auto my_base = BASE_OF(&__ImageBase);
		auto within_self = (ctx->Rip >= g::self.base && ctx->Rip < (g::self.base + g::self.length));

		if (
			exc->ExceptionCode != EXCEPTION_BREAKPOINT &&
			exc->ExceptionCode != EXCEPTION_SINGLE_STEP &&
			exc->ExceptionCode != EXCEPTION_GUARD_PAGE)
		{
			msg(_XS("exception at RIP %p with code %x"), exc->ExceptionAddress, exc->ExceptionCode);

			memset(buf, 0, sizeof(buf));

			sprintf_s(buf, _XS("CODE=%x\nADDR=%p\nRIP=%p\n\nRSP=%p\nRBP=%p\n\nRAX=%p\nRBX=%p\nRCX=%p\nRDX=%p\n"
				"R8=%p\nR9=%p"), 
				exc->ExceptionCode, exc->ExceptionAddress,
				PTR_OF(ctx->Rip), PTR_OF(ctx->Rsp), PTR_OF(ctx->Rbp),
				PTR_OF(ctx->Rax), PTR_OF(ctx->Rbx), PTR_OF(ctx->Rcx), 
				PTR_OF(ctx->Rdx), PTR_OF(ctx->R8), PTR_OF(ctx->R9)
			);

			std::vector<uintptr_t> stack_trace;

			sprintf(buf + strlen(buf), "\n\n");

			if (utl::construct_thread_stack(ctx, stack_trace))
			{
				int i = 0;

				for (uintptr_t rip : stack_trace)
				{
					auto mod = get_backed_module(rip);

					if (!mod.m_base)
					{
						sprintf(buf + strlen(buf), "0x%llx\n", rip);
					}
					else
					{
						sprintf(buf + strlen(buf), "(%i) [%s + 0x%llx]\n", i, mod.m_name.c_str(), (rip - mod.m_base));
					}

					i++;
				}
			}

			sprintf(buf + strlen(buf), "\n\n");

			MessageBoxA(0, buf, _XS("program failure"), MB_ICONERROR);
			ExitProcess(0);

			/* */
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

	DWORD WINAPI ThrMain(LPVOID lpParam)
	{
		DRFAR_FORCE_STACK_FUCK();

		_CRT_INIT( (HINSTANCE)lpParam, DLL_PROCESS_ATTACH, nullptr );

		core::start();

		return NULL;
	}

	BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
	{
		DRFAR_FORCE_FAKE_RIP_ACCESS();

		if (fdwReason == DLL_PROCESS_ATTACH)
		{
			g::self.base = BASE_OF(&__ImageBase);

			auto nh = reinterpret_cast<PIMAGE_NT_HEADERS>(
				g::self.base +
				reinterpret_cast<PIMAGE_DOS_HEADER>(g::self.base)->e_lfanew
				);

			g::self.length = nh->OptionalHeader.SizeOfImage;
			g::self.version = nh->OptionalHeader.CheckSum;

			SetUnhandledExceptionFilter(VeHandler);
			CreateThread(0, 0, ThrMain, hInst, 0, 0);
		}

		DRFAR_FORCE_FAKE_RIP_ACCESS();

		return TRUE;
	}
}
