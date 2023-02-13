#pragma once

extern "C"
{
	uptr __rs_jmp(
		void* jc
	);

	extern uptr GADGET_TRAMP_BEGIN;
	extern uptr GADGET_TRAMP_END;
}

namespace rs
{
#pragma pack(push, 1)
	struct reg_t
	{
		union
		{
			uptr val;
			void* ptr;
		};
	};

	struct context_t
	{
		void* target;

		struct
		{
			reg_t rcx;
			reg_t rdx;
			reg_t r8;
			reg_t r9;
		} regs;
	};
#pragma pack(pop)

	template < typename R >
	R invoke(context_t* ctx)
	{
		using fn = R(__fastcall*)(void*);
		auto jumper = reinterpret_cast<fn>(__rs_jmp);

		return jumper(ctx);
	}
}
