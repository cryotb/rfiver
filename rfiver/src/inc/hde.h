#pragma once

#include "hde/table64.h"
#include "hde/hde64.h"

#pragma once

namespace hde
{
    inline size_t disasm_buffer(void* data, size_t length, hde64s* list, size_t list_length)
    {
        size_t left = length;
        size_t count = 0;
        size_t offset = 0;
        hde64s info{};

        do
        {
            if (mem::addr(data).Add(offset).Base() >
                mem::addr(data).Add(length).Base())
                break;

            if (!hde64_disasm(mem::addr(data).Add(offset).Ptr(), &info))
                break;

            const auto delta = left - info.len;

            if (static_cast<int>(delta) <= 0)
                break;

            memcpy(&list[count], &info, sizeof(hde64s));

            count++;
            left -= info.len;
            offset += info.len;
        } while (left > 0 && (count + 1) < list_length);

        return count;
    }

    inline auto disasm_buffer_ex(void* data, size_t length)
    {
        auto result = std::vector<hde64s>();

        size_t left = length;
        size_t count = 0;
        size_t offset = 0;
        hde64s info{};

        do
        {
            if (mem::addr(data).Add(offset).Base() >
                mem::addr(data).Add(length).Base())
                break;

            if (!hde64_disasm(mem::addr(data).Add(offset).Ptr(), &info))
                break;

            const auto delta = left - info.len;

            if (static_cast<int>(delta) <= 0)
                break;

            result.push_back(info);

            count++;
            left -= info.len;
            offset += info.len;
        } while (left > 0);

        return result;
    }

    inline size_t get_code_size(void* buffer, size_t length, bool function = false)
    {
        auto tmp_buffer_mem = stl::c_pool_buffer(0x1000);
        auto tmp_buffer = mem::addr(tmp_buffer_mem.ptr()).As<hde64s*>();

        if (!tmp_buffer_mem.valid())
            return 0;

        const auto count = disasm_buffer(
            buffer, length,
            tmp_buffer,
            tmp_buffer_mem.length()
        );

        size_t index = 0;
        size_t result = 0;
        hde64s* instr = tmp_buffer;

        do
        {
            result += instr->len;

            if (function && instr->opcode == 0xC3)
                break;

            ++index;
            ++instr;
        } while (index < count && instr != nullptr);

        return result;
    }

    inline size_t get_function_size(void* buffer, size_t length)
    {
        return get_code_size(buffer, length, true);
    }

    inline bool is_function_hookable(UINT8* buffer, SIZE_T length,
        SIZE_T* backup_size, SIZE_T minimum_length, UINT32& status_code)
    {
        auto tmp_buffer_mem = stl::c_pool_buffer(0x1000);
        auto tmp_buffer = mem::addr(tmp_buffer_mem.ptr()).As<hde64s*>();

        if (!tmp_buffer_mem.valid())
            return 0;

        SIZE_T instr_count = disasm_buffer(buffer, length, tmp_buffer, tmp_buffer_mem.length());

        if (instr_count <= 0)
        {
            status_code = 0xE02;
            return false;
        }

        auto result = false;
        SIZE_T instr_total_length = 0;

        for (SIZE_T index = 0; index < instr_count; index++)
        {
            const auto& instr = tmp_buffer[index];

            instr_total_length += instr.len;

            if (instr_total_length >= minimum_length)
            {
                result = true;
                break;
            }
        }

        if (result && backup_size)
            *backup_size = instr_total_length;

        status_code = 0x1;

        return result;
    }

    inline bool resolve_jump(void* ptr, uintptr_t& dst, bool follow = false)
    {
        hde64s list[64];
        uint8_t snippet[128] = { 0 };

        memset(list, 0, sizeof(list));
        memcpy(snippet, ptr, sizeof(snippet));

        size_t count = disasm_buffer(snippet, sizeof(snippet), list, sizeof(list));

        if (count <= 0)
            return false;

        const auto& jump_instr = list[0];

        if (jump_instr.opcode != 0xFF)
            return false;

        uintptr_t jump_rip = ((uintptr_t)ptr) + jump_instr.len;
        uintptr_t jump_offset_addr = ((uintptr_t)&snippet[0]) + jump_instr.len - sizeof(std::uint32_t);
        uint32_t result = 0;

        memcpy(&result, (void*)jump_offset_addr, sizeof(result));

        dst = jump_rip + result;

        if (follow)
            dst = *(uintptr_t*)dst;

        return true;
    }

    inline u64 resolve_call(u64 rip)
    {
        auto disasm = disasm_buffer_ex(reinterpret_cast<u8*>(rip), 15);

        if (!disasm.empty())
        {
            const auto& ins = disasm.front();

            if (ins.opcode == 0xE8)
            {
                auto offset = *reinterpret_cast<int32_t*>(rip + 0x1) + ins.len;
                return rip + offset;
            }
        }

        return 0ull;
    }
}

