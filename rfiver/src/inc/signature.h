#pragma once

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

namespace sig
{
	inline
		bool isMatch(const PBYTE addr, const PBYTE pat, const PBYTE msk)
	{
		size_t n = 0;
		while (addr[n] == pat[n] || msk[n] == (BYTE)'?') {
			if (!msk[++n]) {
				return true;
			}
		}
		return false;
	}

	inline PBYTE find_pattern(const PBYTE rangeStart, DWORD len, const char* pattern)
	{
		size_t l = strlen(pattern);
		PBYTE patt_base = static_cast<PBYTE>(_alloca(l >> 1));
		PBYTE msk_base = static_cast<PBYTE>(_alloca(l >> 1));
		PBYTE pat = patt_base;
		PBYTE msk = msk_base;
		l = 0;
		while (*pattern) {
			if (*pattern == ' ')
				pattern++;
			if (!*pattern)
				break;
			if (*(PBYTE)pattern == (BYTE)'\?') {
				*pat++ = 0;
				*msk++ = '?';
				pattern += ((*(PWORD)pattern == (WORD)'\?\?') ? 2 : 1);
			}
			else {
				*pat++ = getByte(pattern);
				*msk++ = 'x';
				pattern += 2;
			}
			l++;
		}
		*msk = 0;
		pat = patt_base;
		msk = msk_base;
		for (DWORD n = 0; n < (len - l); ++n)
		{
			if (isMatch(rangeStart + n, patt_base, msk_base)) {
				return rangeStart + n;
			}
		}
		return NULL;
	}
}

#define find_pattern(base, length, pat) (DWORD_PTR)sig::find_pattern((PBYTE)base, length, _XS(pat))
#define find_ptr(base, pat, o1, o2) resolve_addr_to_rel( \
					(DWORD_PTR)find_pattern(base, get_pe_length( PTR_OF(base) ), pat), o1, o2 )
