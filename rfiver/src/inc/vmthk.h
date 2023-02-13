#pragma once

class vmthk
{
public:
    vmthk() = delete;
    vmthk(void* inst)
    {
        m_inst = inst;
        m_table_original = *reinterpret_cast<void***>(inst);
        m_count = calc_cnt();

        if (!m_count) return;

       
        m_len = (m_count + 1 + 1) * sizeof uptr;
        m_table = reinterpret_cast<void**>(malloc(m_len));
        if (!m_table) return;

        
        memset(m_table, 0, m_len);

        m_table[0] = m_table_original[-1];

       
        memcpy(&m_table[1], m_table_original, m_len - (sizeof uptr * 2));
    }

    const auto& count() const { return m_count; }

    auto table() { return m_table; }
    auto enable() { *reinterpret_cast<uptr*>(m_inst) = BASE_OF(&m_table[1]); }
    auto disable() { *reinterpret_cast<void***>(m_inst) = m_table_original; }

    auto place(size_t idx2, void* handler) -> void*
    {
        auto idx = idx2 + 1;
        if (idx >= m_count) return nullptr;

        auto orig = m_table[idx];
        m_table[idx] = handler;

        return orig;
    }

    template < typename R >
    auto original(size_t idx) -> R
    {
        return (R)m_table_original[idx];
    }
private:
    size_t calc_cnt()
    {
        size_t index{ };
        
        while (m_table_original[index] != nullptr &&
            utl::is_executable(m_table_original[index]))
            ++index;

        return index;
    }
private:
    void* m_inst;
    void** m_table;
    void** m_table_original;
    size_t m_count;
    size_t m_len;
};