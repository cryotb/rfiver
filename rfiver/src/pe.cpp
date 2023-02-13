#include <include.h>

bool pe::get_universal_opt_hdr(void* base, universal_opt_hdr_t& hdr)
{
	PIMAGE_DOS_HEADER temp_dh;
	PIMAGE_NT_HEADERS temp_nh;

	if (get_headers<decltype(temp_nh)>(base, &temp_dh, &temp_nh))
	{
		switch (get_arch(base))
		{
		case 32:
		{
			auto nh = reinterpret_cast<PIMAGE_NT_HEADERS32>(temp_nh);
			auto opt = nh->OptionalHeader;

			hdr.magic = opt.Magic;
			hdr.linker_ver_major = opt.MajorLinkerVersion;
			hdr.linker_ver_minor = opt.MinorLinkerVersion;
			hdr.size_of_code = opt.SizeOfCode;
			hdr.entry_point = opt.AddressOfEntryPoint;
			hdr.base_of_code = opt.BaseOfCode;
			hdr.base_of_data = opt.BaseOfData;
			hdr.section_align = opt.SectionAlignment;
			hdr.file_align = opt.FileAlignment;
			hdr.size_of_image = opt.SizeOfImage;
			hdr.size_of_headers = opt.SizeOfHeaders;
			hdr.dll_characteristics = opt.DllCharacteristics;
			hdr.ldr_flags = opt.LoaderFlags;
			hdr.number_of_rva_and_sizes = opt.NumberOfRvaAndSizes;
			hdr.image_base = opt.ImageBase;

			return true;
		}
		case 64:
		{
			auto nh = reinterpret_cast<PIMAGE_NT_HEADERS64>(temp_nh);
			auto opt = nh->OptionalHeader;

			hdr.magic = opt.Magic;
			hdr.linker_ver_major = opt.MajorLinkerVersion;
			hdr.linker_ver_minor = opt.MinorLinkerVersion;
			hdr.size_of_code = opt.SizeOfCode;
			hdr.entry_point = opt.AddressOfEntryPoint;
			hdr.base_of_code = opt.BaseOfCode;
			hdr.base_of_data = 0;
			hdr.section_align = opt.SectionAlignment;
			hdr.file_align = opt.FileAlignment;
			hdr.size_of_image = opt.SizeOfImage;
			hdr.size_of_headers = opt.SizeOfHeaders;
			hdr.dll_characteristics = opt.DllCharacteristics;
			hdr.ldr_flags = opt.LoaderFlags;
			hdr.number_of_rva_and_sizes = opt.NumberOfRvaAndSizes;
			hdr.image_base = opt.ImageBase;

			return true;
		}
		default:
			__debugbreak();
			break;
		}
	}

	return false;
}

pe::export_list pe::get_exports(uintptr_t base, _pimage_dos_header dos_hdr, _pimage_nt_header nt_hdr, bool skip_high_ordinals)
{
	auto result = export_list();

	auto* const export_descriptor =
		get_data_directory_ptr<_image_export_directory>(base, nt_hdr,
			IMAGE_DIRECTORY_ENTRY_EXPORT);

	IMAGE_DATA_DIRECTORY export_descriptor_info;

	if (export_descriptor == nullptr)
		return result;

	if (!get_data_directory_info(base, nt_hdr,
		IMAGE_DIRECTORY_ENTRY_EXPORT, export_descriptor_info))
		return result;

	const auto export_descriptor_rva =
		(reinterpret_cast<uintptr_t>(export_descriptor) - base);

	auto names_rva = export_descriptor->AddressOfNames;
	auto addresses_rva = export_descriptor->AddressOfFunctions;
	auto ordinals_rva = export_descriptor->AddressOfNameOrdinals;

	auto names_address = static_cast<u64>(base + names_rva);
	auto addresses_address = static_cast<u64>(base + addresses_rva);
	auto ordinals_address = static_cast<u64>(base + ordinals_rva);

	auto names_array = reinterpret_cast<u32*>(names_address);
	auto addresses_array = reinterpret_cast<u32*>(addresses_address);
	auto ordinals_array = reinterpret_cast<u16*>(ordinals_address);

	auto export_descriptor_rva_begin = export_descriptor_rva;
	auto export_descriptor_rva_end = (export_descriptor_rva + export_descriptor_info.Size);

	for (size_t i = 0; i < export_descriptor->NumberOfFunctions; i++)
	{
		auto ordinal_rva = ordinals_array[i];

		if (!ordinal_rva)
			continue;

		auto ordinal = ordinals_array[i];

		auto name_rva = names_array[i];
		auto address_rva = addresses_array[(skip_high_ordinals ? (ordinal > 1000 ? i : ordinal) : ordinal)];

		auto name = reinterpret_cast<char*>(base + name_rva);

		if (name_rva >= export_descriptor_rva_begin &&
			name_rva <= export_descriptor_rva_end)
		{
			export_t entry;

			memset(&entry, 0, sizeof(entry));
			strncpy(entry.name, name, strlen(name));
			entry.addr = base + address_rva;
			entry.ordinal = ordinal;

			result.push_back(entry);
		}
	}

	return result;
}

