#pragma once

__forceinline auto msg(const char* fmt, ...)
{
#if defined _DEBUG
	va_list vl;
	char buf[1024];
	ZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	printf("%s\n", buf);
#endif
}

__forceinline auto dbg(const char* fmt, ...)
{
#if defined _DEBUG
	va_list vl;
	char buf[1024];
	ZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	vsprintf_s(buf, fmt, vl);
	va_end(vl);

	printf("[dbg] %s\n", buf);
#endif
}

__forceinline auto __log_internal(const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	RtlZeroMemory(&vl, sizeof(vl));
	RtlZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	wvsprintfA(buf, fmt, vl);
	va_end(vl);

	OutputDebugStringA(buf);
}

__forceinline auto __dbg_msg(const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	RtlZeroMemory(&vl, sizeof(vl));
	RtlZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	wvsprintfA(buf, fmt, vl);
	va_end(vl);

	__log_internal("[msg] %s", buf);
}

__forceinline auto __dbg_warn(const char* fmt, ...)
{
	va_list vl;
	char buf[512];

	RtlZeroMemory(&vl, sizeof(vl));
	RtlZeroMemory(buf, sizeof(buf));

	va_start(vl, fmt);
	wvsprintfA(buf, fmt, vl);
	va_end(vl);

	__log_internal("[wrn] %s", buf);
}

#define _msg(X, ...) msg(_XS(X), __VA_ARGS__)
#define _dbg(X, ...) dbg(_XS(X), __VA_ARGS__)
