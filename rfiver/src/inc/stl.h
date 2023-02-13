#pragma once

namespace stl
{
	template < typename T >
	inline T mem_alloc(size_t length, ulong protect = PAGE_READWRITE)
	{
		auto result = HeapAlloc(
			GetProcessHeap(), 0, length
		);

		if (result != nullptr)
			return (T)result;

		return T{};
	}

	inline auto mem_free(void* p)
	{
		return HeapFree(GetProcessHeap(), 0, p);
	}

	template < typename T, size_t C >
	class circ_buf_t
	{
	public:
		circ_buf_t()
		{
			m_head = 0;
		}

		auto front() { return m_data.front(); }
		auto back() { return m_data.back(); }
		auto arr() { return m_data; }

		auto insert(const T& t)
		{
			if (m_head < m_data.size())
			{
				m_data[m_head++] = t;
			}
			else
			{
				m_head = 0;
				m_data[m_head] = t;
			}
		}
	private:
		size_t m_head;
		std::array<T, C> m_data;
	};

	template < typename T >
	class circ_bufd_t
	{
	public:
		circ_bufd_t()
		{
			m_head = 0;
		}

		auto init(size_t c) { m_data.resize(c); }
		auto destroy() { m_data.clear(); }

		auto front() { return m_data.front(); }
		auto back() { return m_data.back(); }

		auto insert(const T& t)
		{
			if (m_head < m_data.size())
			{
				m_data[m_head++] = t;
			}
			else
			{
				m_head = 0;
				m_data[m_head] = t;
			}
		}
	private:
		size_t m_head;
		std::vector<T> m_data;
	};

	class c_pool_buffer
	{
	public:
		c_pool_buffer() = delete;
		~c_pool_buffer()
		{
			if (m_data)
				mem_free(m_data);
		}

		c_pool_buffer(size_t length, bool zero = true)
		{
			m_length = length;
			m_data = mem_alloc<void*>(m_length);

			if (m_data && zero)
				RtlSecureZeroMemory(m_data, length);
		}

		auto length() const { return m_length; }
		auto valid() const { return !(m_data == nullptr); }
		auto ptr() const { return m_data; }
	private:
		size_t m_length{};
		void* m_data{};
	};
}
