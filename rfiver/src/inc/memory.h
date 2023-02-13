#pragma once

namespace mem
{
    struct range {
        uintptr_t m_begin{};
        uintptr_t m_end{};
        uintptr_t m_size{};
    };

    inline DWORD_PTR search(PVOID pHayStack, PVOID pNeedle, SIZE_T uHayStackLength, SIZE_T uNeedleLength)
    {
        if (pHayStack == nullptr || pNeedle == nullptr)
            return 0;

        if (uHayStackLength <= NULL || uNeedleLength <= NULL)
            return 0;

        for (auto* pBuffer = static_cast<BYTE*>(pHayStack); uHayStackLength >= uNeedleLength; ++pBuffer, --uHayStackLength)
        {
            if (memcmp(pBuffer, pNeedle, uNeedleLength) == NULL)
                return reinterpret_cast<DWORD_PTR>(pBuffer);
        }

        return 0;
    }

    template < typename T >
    inline DWORD_PTR search_value(PVOID pHayStack, SIZE_T uHayStackLength, const T& sNeedle)
    {
        LPCVOID pNeedle = &sNeedle;
        SIZE_T uNeedleLength = sizeof(sNeedle);

        if (pHayStack == nullptr || pNeedle == nullptr)
            return 0;

        if (uHayStackLength <= NULL || uNeedleLength <= NULL)
            return 0;

        __try
        {
            for (auto* pBuffer = static_cast<BYTE*>(pHayStack); uHayStackLength >= uNeedleLength; ++pBuffer, --uHayStackLength)
            {
                if (memcmp(pBuffer, (PVOID)pNeedle, uNeedleLength) == NULL)
                    return reinterpret_cast<DWORD_PTR>(pBuffer);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return 0;
        }

        return 0;
    }

    class addr
    {
    public:
        addr() = default;
        ~addr() = default;

        bool operator==(const DWORD_PTR& other) const
        {
            return (m_base == other);
        }

        bool operator==(const DWORD_PTR& lhs)
        {
            return (lhs == m_base);
        }

        addr(const DWORD_PTR base)
        {
            m_base = base;
        }

        addr(void* const ptr)
        {
            m_base = reinterpret_cast<DWORD_PTR>(ptr);
        }

        [[nodiscard]] auto Get() const
        {
            return addr(m_base);
        }

        [[nodiscard]] auto Base() const
        {
            return m_base;
        }

        [[nodiscard]] auto Valid() const
        {
            return !(m_base == 0);
        }

        [[nodiscard]] auto Add(const DWORD_PTR t) const
        {
            return addr(m_base + t);
        }

        [[nodiscard]] auto Sub(const DWORD_PTR t) const
        {
            return addr(m_base - t);
        }

        [[nodiscard]] auto Deref() const
        {
            return addr(*reinterpret_cast<DWORD_PTR*>(m_base));
        }

        [[nodiscard]] auto Ptr() const
        {
            return reinterpret_cast<PVOID>(m_base);
        }

        template < class T >
        [[nodiscard]] auto As()
        {
            return reinterpret_cast<T>(m_base);
        }

        template < class T >
        [[nodiscard]] auto Retrieve_as()
        {
            return *reinterpret_cast<T*>(m_base);
        }

        template < class T >
        void Set(const T& value)
        {
            *As<T*>() = value;
        }
    private:
        DWORD_PTR m_base;
    };

    class bare_pointer
    {
    public:
        bare_pointer(void* const ptr)
        {
            m_instance = ptr;
        }

        [[nodiscard]] addr base() const
        {
            return addr(m_instance);
        }

        [[nodiscard]] auto deref() const
        {
            return *static_cast<void**>(m_instance);
        }
    private:
        void* m_instance;
    };

    inline DWORD_PTR ResolveAddressToRelative(DWORD_PTR dwInitial, DWORD_PTR dwInstructionOffset, DWORD_PTR dwPostOffset)
    {
        if (dwInitial == NULL)
            return NULL;

        CONST auto dwRelativeAddyOffset = (dwInitial + dwInstructionOffset);
        CONST auto dwRelativeAddy = *reinterpret_cast<LONG*>(dwRelativeAddyOffset);
        CONST auto dwRelativeResult = dwInitial + dwRelativeAddy;

        if (dwPostOffset != NULL)
            return dwRelativeResult + dwPostOffset;

        return dwRelativeResult;
    }

    extern bool ptr_is_valid(const void* a);

    extern bool compare(const void* a, const void* b, size_t len);
    extern bool copy(void* dst, const void* src, size_t len);
}

#define resolve_addr_to_rel(INITIAL, INSTR_OFFS, OPCODE_LENGTH) mem::ResolveAddressToRelative(INITIAL, INSTR_OFFS, OPCODE_LENGTH)
