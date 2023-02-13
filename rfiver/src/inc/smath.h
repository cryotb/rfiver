#pragma once

namespace math
{
	template <typename _t>
	struct position_t
	{
		_t x;
		_t y;

		_t length()
		{
			return x + y;
		}

		_t distance_to(const position_t<_t>& other)
		{
			return length() - other.length();
		}

		template <typename _n>
		_n static_type_convert()
		{
			return position_t<_n>{static_cast<_n>(x), static_cast<_n>(y)};
		}
	};

	template <typename _t>
	struct dimension_t
	{
		_t w;
		_t h;
	};
}
