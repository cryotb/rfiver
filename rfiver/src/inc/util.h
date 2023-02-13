#pragma once

#pragma warning(disable : 4311 4302 4018)

namespace utl
{
	struct module_t
	{
		std::string m_name{};

		std::uintptr_t m_base{};
		std::uintptr_t m_size{};
	};

	inline auto query_virt_mem(void* addr) -> MEMORY_BASIC_INFORMATION
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(addr, &mbi, sizeof mbi))
			return { };

		return mbi;
	}

	inline auto is_executable(void* addr)
	{
		auto inf = query_virt_mem(addr);

		return (inf.Protect == PAGE_EXECUTE_READ ||
			inf.Protect == PAGE_EXECUTE_READWRITE);
	}

	FORCEINLINE bool make_string_unsafe(char* data, std::string& out)
	{
		try
		{
			out = std::string(data);
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FORCEINLINE bool patch_prot(uintptr_t dst, std::initializer_list<uint8_t> src)
	{
		ulong old_prot;

		if (VirtualProtect(PTR_OF(dst), src.size(), PAGE_EXECUTE_READWRITE, &old_prot))
		{
			memcpy(PTR_OF(dst), src.begin(), src.size());

			VirtualProtect(PTR_OF(dst), src.size(), old_prot, &old_prot);
			return true;
		}

		return false;
	}

	FORCEINLINE bool construct_thread_stack(PCONTEXT pctx, std::vector<uintptr_t>& buffer)
	{
		auto ctx = *pctx;

		DWORD_PTR module_base, est_frame;
		PVOID handler_data;
		KNONVOLATILE_CONTEXT_POINTERS nvctx;

		__try
		{
			for (int i = 0; i < 9; i++)
			{
				auto func = RtlLookupFunctionEntry(ctx.Rip, &module_base, nullptr);
				ZeroMemory(&nvctx, sizeof(nvctx));

				if (func)
				{
					RtlVirtualUnwind(
						UNW_FLAG_NHANDLER, module_base,
						ctx.Rip, func, &ctx, &handler_data,
						&est_frame, &nvctx
					);
				}
				else
				{
					ctx.Rip = *reinterpret_cast<ULONG_PTR*>(ctx.Rsp);
					ctx.Rsp += 8;
				}

				if (!ctx.Rip) break;

				buffer.push_back(ctx.Rip);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}

		return (buffer.size() > 0);
	}

	FORCEINLINE std::vector<std::string> split_string(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();

		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}

	FORCEINLINE PVOID query_system_info(SYSTEM_INFORMATION_CLASS sysInfoClass)
	{
		ULONG initial_buffer_size = 0;
		ULONG actual_bytes_written = 0;

		auto result = NtQuerySystemInformation(
			sysInfoClass,
			nullptr,
			initial_buffer_size,
			&actual_bytes_written
		);

		if (result != 0xC0000004)
			return FALSE;

		if (actual_bytes_written <= 0)
			return FALSE;

		if (sysInfoClass == 16)
			actual_bytes_written = 0x1000000u;

		void* data_buffer = malloc(actual_bytes_written);

		if (data_buffer == nullptr)
			return FALSE;

		initial_buffer_size = actual_bytes_written;

		do
		{
			result = NtQuerySystemInformation(
				sysInfoClass,
				data_buffer,
				initial_buffer_size,
				&actual_bytes_written
			);

			if (result == 0xC0000004)
			{
				msg("buf change from %i to %i", initial_buffer_size, actual_bytes_written);
				data_buffer = realloc(data_buffer, actual_bytes_written);

				initial_buffer_size = actual_bytes_written;
			}

			msg("result => %x", result);
		} while (result == 0xC0000004);

		if (result >= 0)
			return data_buffer;

		return FALSE;
	}

	FORCEINLINE u64 get_thread_start(HANDLE id)
	{
		u64 rs = 0;

		auto handle = OpenThread(THREAD_ALL_ACCESS, FALSE, (DWORD)id);

		if (handle)
		{
			if (nt::QueryInfoThread(handle, (THREADINFOCLASS)9, &rs, sizeof(rs), 0) >= 0)
			{
				CloseHandle(handle);
				return rs;
			}

			CloseHandle(handle);
		}

		return 0;
	}

	FORCEINLINE auto get_process_threads(HANDLE id)
	{
		std::vector<nt::SYSTEM_THREAD_INFORMATION> tl;
		nt::PROCESS_BASIC_INFORMATION pbi{ };

		if (auto info = reinterpret_cast<nt::PSYSTEM_PROCESS_INFORMATION>(utl::query_system_info(SystemProcessInformation)))
		{
			auto curr = reinterpret_cast<decltype(info)>(BASE_OF(info) + info->NextEntryOffset);

			while (curr->NextEntryOffset)
			{
				if (curr->UniqueProcessId == id)
				{
					for (int i = 0; i < curr->NumberOfThreads; i++)
					{
						auto entry = curr->Threads[i];
						entry.StartAddress = PTR_OF(get_thread_start(entry.ClientId.UniqueThread));
						tl.push_back(entry);
					}
				}

				curr = reinterpret_cast<decltype(curr)>(BASE_OF(curr) + curr->NextEntryOffset);
			}
		}

		return tl;
	}

	__forceinline auto get_current_peb()
	{
		auto peb = reinterpret_cast<nt::PPEB>(__readgsqword(0x60));

		return peb;
	}

	__forceinline auto get_modules()
	{
		auto result = std::vector<module_t>();
		auto peb = get_current_peb();

		if (peb == nullptr)
			return result;

		if (peb->Ldr)
		{
			PLIST_ENTRY head = &peb->Ldr->InLoadOrderModuleList;

			for (auto p = head->Flink; p != head; p = p->Flink)
			{
				auto data = CONTAINING_RECORD(p, nt::LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

				if (!data->DllBase) continue;

				auto entry = module_t{ };

				entry.m_base = reinterpret_cast<uintptr_t>(data->DllBase);
				entry.m_size = data->SizeOfImage;

				if (entry.m_base && entry.m_size > 0)
				{
					char name_buf[64];
					if (data->BaseDllName.Length <= sizeof(name_buf))
					{
						wcstombs(
							name_buf,
							data->BaseDllName.Buffer,
							data->BaseDllName.Length
						);

						for (size_t i = 0; i < strlen(name_buf); i++)
						{
							name_buf[i] = tolower(name_buf[i]);
						}

						entry.m_name = name_buf;
					}

					result.push_back(entry);
				}
			}
		}

		return result;
	}

	FORCEINLINE bool is_backed_by_module(u64 rip)
	{
		auto mods = get_modules();

		for (const auto& mod : mods)
		{
			if (rip >= mod.m_base && rip < (mod.m_base + mod.m_size))
				return true;
		}
		
		return false;
	}

	FORCEINLINE bool is_module_present(const char* mod)
	{
		auto mods = get_modules();

		for (const auto& m : mods)
		{
			if (!strcmp(m.m_name.c_str(), mod))
				return true;
		}

		return false;
	}

	FORCEINLINE HANDLE get_process_main_thread(HANDLE id)
	{
		auto tl = get_process_threads(id);

		auto main_module = get_modules().front();

		for (const auto& thr : tl)
		{
			auto start_addr = BASE_OF(thr.StartAddress);

			if (start_addr >= main_module.m_base && start_addr < (main_module.m_base + main_module.m_size))
				return thr.ClientId.UniqueThread;
		}

		return 0;
	}

	FORCEINLINE uint64_t convert_string_to_i64(std::string s)
	{
		uint64_t result{};

		std::istringstream iss(s);
		iss >> result;

		return result;
	}

	inline bool aob_read_from_memory(const std::string& file_path, std::vector<uint8_t>* out_buffer)
	{
		std::ifstream file_ifstream(file_path, std::ios::binary);

		if (!file_ifstream)
			return false;

		out_buffer->assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
		file_ifstream.close();

		return true;
	}

	inline bool aob_save_to_disk(PVOID pData, SIZE_T nLength, const std::string& szOutputFileName)
	{
		if (pData == nullptr || nLength == NULL || szOutputFileName.empty())
			return false;

		auto zFileStream = std::ofstream(szOutputFileName, std::ios::beg | std::ios::binary);

		if (!zFileStream.is_open())
			return false;

		zFileStream.write(static_cast<LPCSTR>(pData), nLength);
		zFileStream.close();

		return true;
	}

	inline unsigned int mmh_32(const void* key, int len, unsigned int seed)
	{
		const unsigned int m = 0x5bd1e995;
		const int r = 24;

		unsigned int h = seed ^ len;

		const unsigned char* data = (const unsigned char*)key;

		while (len >= 4)
		{
			unsigned int k = *(unsigned int*)data;

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		switch (len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}

	inline int string_hash(const char* text, size_t length)
	{
		return mmh_32((void*)text, static_cast<int>(length), 6);
	}

	inline auto calc_percent(int curr, int total)
	{
		auto a = static_cast<double>(curr);
		auto b = static_cast<double>(total);

		// F =  ( CURRENT ÷ MAX ) x 100.f
		return static_cast<int>(((a / b) * 100.f));
	}

	inline double calc_percent_reverse(int curv, int maxv) {
		double percentageReturn = (((double)(maxv - curv) / (double)maxv) *
			100);

		return percentageReturn;
	}

	inline auto mem_read_s(void* dst, uintptr_t src, size_t len)
	{
		void* psrc = reinterpret_cast<void*>(src);
		void* rs = nullptr;

		if (!IsBadReadPtr(psrc, len))
		{
			__try
			{
				rs = memcpy(dst, psrc, len);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				rs = nullptr;
			}
		}
		
		return rs;
	}

	inline bool create_console()
	{
		auto result = AllocConsole();

		// it was already present, however we've cleared it so it should be working as supposed.
		if (result == false && !(GetConsoleWindow() == nullptr))
			result = true;

		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

		return result;
	}

	inline DWORD get_process_id_by_name(const std::string& name)
	{
		PROCESSENTRY32 entry{};
		entry.dwSize = sizeof(PROCESSENTRY32);

		DWORD result = NULL;
		auto* const snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot_handle, &entry) == FALSE)
		{
			CloseHandle(snapshot_handle);
			return result;
		}

		while (Process32Next(snapshot_handle, &entry))
		{
			std::string entry_name(entry.szExeFile);

			std::transform(entry_name.begin(), entry_name.end(), entry_name.begin(), [](wchar_t character)
				{
					return std::tolower(character);
				});

			if (entry_name.empty() || entry_name[0] == 0)
			{
				continue;
			}

			//printf("%ws.\n", entry_name.c_str());

			if (entry_name == name)
			{
				result = entry.th32ProcessID;
				break;
			}
		}

		CloseHandle(snapshot_handle);
		return result;
	}

	__forceinline auto text_to_lower(const std::string& input)
	{
		auto output = std::string(input);

		std::transform(output.begin(), output.end(), output.begin(),
			[](const unsigned char c) { return std::tolower(c); });

		return output;
	}

	using module_list = std::vector<module_t>;

	inline module_list get_process_modules(HANDLE process)
	{
		auto snapshot = HANDLE{};

		auto result = module_list();
		auto entry = MODULEENTRY32{};

		auto process_id = GetProcessId(process);

		entry.dwSize = sizeof(entry);

		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

		if (!Module32First(snapshot, &entry))
		{
			CloseHandle(snapshot);
			return result;
		}

		do
		{
			const auto module_name = std::filesystem::path(entry.szExePath).filename().string();

			auto& info = result.emplace_back();

			info.m_name = text_to_lower(module_name);
			info.m_base = (DWORD_PTR)entry.modBaseAddr;
			info.m_size = entry.modBaseSize;

		} while (Module32Next(snapshot, &entry));

		CloseHandle(snapshot);

		return result;
	}

	inline int get_num_procs_with_name(const std::string& name)
	{
		int result = 0;

		PROCESSENTRY32 entry{};
		entry.dwSize = sizeof(PROCESSENTRY32);

		auto* const snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot_handle, &entry) == FALSE)
		{
			CloseHandle(snapshot_handle);
			return result;
		}

		while (Process32Next(snapshot_handle, &entry))
		{
			std::string entry_name(entry.szExeFile);

			std::transform(entry_name.begin(), entry_name.end(), entry_name.begin(), [](wchar_t character)
				{
					return std::tolower(character);
				});

			if (entry_name.empty() || entry_name[0] == 0)
			{
				continue;
			}

			//printf("%ws.\n", entry_name.c_str());

			if (entry_name == name)
			{
				result++;
			}
		}

		CloseHandle(snapshot_handle);
		return result;
	}

	inline void* get_vtable_func(void* class_base, size_t index)
	{
		if (!class_base || index < 0)
			return nullptr;

		auto** const class_vtable = *(void***)class_base;

		if (class_vtable == nullptr)
			return nullptr;

		return class_vtable[index];
	}
}

#define MMH32_STR(str, len) utl::string_hash(str, len)
