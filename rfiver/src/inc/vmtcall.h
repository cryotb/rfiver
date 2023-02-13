#pragma once

namespace vfunc
{
    template <size_t Index, typename ReturnType, typename... Args>
    __forceinline ReturnType call(void* instance, Args... args)
    {
        using Fn = ReturnType(__thiscall*)(void*, Args...);

        auto function = (*reinterpret_cast<Fn**>(instance))[Index];

        return function(instance, args...);
    }
}

template < size_t SPACE >
struct pseudo_class_t
{
    struct
    {
        union
        {
            uptr addr;
            u64* table;
        };
    } m_vtable;

    char m_space[SPACE];

    pseudo_class_t(uptr vft_base)
    {
        m_vtable.addr = vft_base;

        memset(m_space, 0, SPACE);
    }

    template < typename ret_type, typename... Args >
    auto call(u32 index, Args... args)
    {
        using fn = ret_type(__thiscall*)(void*, Args...);
        auto fun = reinterpret_cast<fn>(m_vtable.table[index]);

        return fun(this, args...);
    }
};

#define VIRTUAL_FUNC(NAME, RETURN_VALUE, INDEX, ARGS, ...) RETURN_VALUE NAME ARGS { return vfunc::call<INDEX, RETURN_VALUE>(__VA_ARGS__); }
