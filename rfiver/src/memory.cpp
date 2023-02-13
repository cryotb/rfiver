#include <include.h>

bool mem::ptr_is_valid(const void* a)
{
	if (a == nullptr)
		return false;

	return true;
}

bool mem::compare(const void* a, const void* b, size_t len)
{
	if (!ptr_is_valid(a) || !ptr_is_valid(b) || !len)
		return false;

	bool result{ };

	__try
	{
		result = ( memcmp( a, b, len ) == 0 );
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		result = false;
	}

	return result;
}

bool mem::copy(void* dst, const void* src, size_t len)
{
	if (!ptr_is_valid(dst) || !ptr_is_valid(src) || !len)
		return false;

	bool result{ };

	__try
	{
		result = ( memcpy( dst, src, len ) == dst );
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		result = false;
	}

	return result;
}
