#pragma once

namespace pe
{
	struct universal_opt_hdr_t
	{
		u32 magic;
		char linker_ver_major;
		char linker_ver_minor;
		u32 size_of_code;
		u32 entry_point;
		u32 base_of_code;
		u32 base_of_data;
		u32 section_align;
		u32 file_align;
		u32 size_of_image;
		u32 size_of_headers;
		u16 dll_characteristics;
		u32 ldr_flags;
		u32 number_of_rva_and_sizes;
		u64 image_base;
		IMAGE_DATA_DIRECTORY data_dir[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};

	struct export_t
	{
		char name[256];
		u64 addr;
		u16 ordinal;
	};

	using export_list = std::vector<export_t>;

	template < typename T >
	inline auto get_headers(void* base, PIMAGE_DOS_HEADER* out_dh, PIMAGE_NT_HEADERS* out_nh)
	{
		__try
		{
			auto temp_dh = reinterpret_cast<PIMAGE_DOS_HEADER>(base);

			if (temp_dh->e_magic == IMAGE_DOS_SIGNATURE)
			{
				auto temp_nh = reinterpret_cast<T>(BASE_OF(base) + temp_dh->e_lfanew);

				if (temp_nh->Signature == IMAGE_NT_SIGNATURE)
				{
					if (out_dh) *out_dh = temp_dh;
					if (out_nh) *out_nh = temp_nh;

					return true;
				}
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}

		return false;
	}

	inline auto get_arch(void* base) -> unsigned short
	{
		PIMAGE_DOS_HEADER temp_dh;
		PIMAGE_NT_HEADERS temp_nh;

		if (get_headers<decltype(temp_nh)>(base, &temp_dh, &temp_nh))
		{
			//
			// File header is identical on all architectures IIRC.
			// Only Optional header differs and we don't need it for determining arch.
			//

			switch (temp_nh->FileHeader.Machine)
			{
			case IMAGE_FILE_MACHINE_I386:
				return 32;
			case IMAGE_FILE_MACHINE_AMD64:
				return 64;
			}
		}

		return 0;
	}

	template < typename _nth >
	__inline bool get_data_directory_info(uintptr_t base, _nth nt_hdr, int index, IMAGE_DATA_DIRECTORY& out)
	{
		out = nt_hdr->OptionalHeader.DataDirectory[index];

		return true;
	}

	template < typename _pt, typename _nth >
	__inline _pt* get_data_directory_ptr(uintptr_t base, _nth nt_hdr, int index)
	{
		const auto offset =
			nt_hdr->OptionalHeader.DataDirectory[index].VirtualAddress;

		return reinterpret_cast<_pt*>((base + offset));
	}

	extern bool get_universal_opt_hdr(void* base, universal_opt_hdr_t& hdr);
	extern export_list get_exports(uintptr_t base, _pimage_dos_header dos_hdr, _pimage_nt_header nt_hdr, bool skip_high_ordinals);

	template < typename T >
	inline auto find_export(u64 base, const char* name)
	{
		T result = (T)nullptr;

		_pimage_dos_header dh;
		_pimage_nt_header nh;

		if (get_headers<_pimage_nt_header>(PTR_OF(base), &dh, &nh))
		{
			auto export_descriptor =
				get_data_directory_ptr<_image_export_directory>(base, nh,
					IMAGE_DIRECTORY_ENTRY_EXPORT);

			IMAGE_DATA_DIRECTORY export_descriptor_info;

			if (export_descriptor == nullptr)
				return result;

			if (!get_data_directory_info(base, nh,
				IMAGE_DIRECTORY_ENTRY_EXPORT, export_descriptor_info))
				return result;

			auto export_descriptor_rva =
				(reinterpret_cast<uintptr_t>(export_descriptor) - base);

			auto names_array = reinterpret_cast<u32*>(base + export_descriptor->AddressOfNames);
			auto addresses_array = reinterpret_cast<u32*>(base + export_descriptor->AddressOfFunctions);
			auto ordinals_array = reinterpret_cast<u16*>(base + export_descriptor->AddressOfNameOrdinals);

			auto export_descriptor_rva_begin = export_descriptor_rva;
			auto export_descriptor_rva_end = (export_descriptor_rva + export_descriptor_info.Size);

			for (size_t i = 0; i < export_descriptor->NumberOfNames; i++)
			{
				auto exp_name_rva = names_array[i];
				auto exp_name = reinterpret_cast<char*>(base + exp_name_rva);

				if (exp_name_rva >= export_descriptor_rva_begin &&
					exp_name_rva <= export_descriptor_rva_end)
				{
					if (!strcmp(exp_name, name))
					{
						result = (T)(base + addresses_array[ordinals_array[i]]);
						break;
					}
				}
			}

			return result;
		}

		return result;
	}
}
