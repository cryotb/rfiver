#pragma once

namespace respawn
{
	class c_engine_trace
	{
	public:
		void trace_ray(Ray_t& ray, u32 mask, void* filter, void* trace)
		{
			using fn = void(__thiscall*)(void*, void*, u32, void*, void*, void*);
			return vfunc::call<3, void>(this, ray, mask, filter, trace);
		}

		void trace_ray(Ray_t& ray, unsigned int mask, trace_t* trace)
		{
			return vfunc::call<4, void>(this, ray, mask, trace);
		}

		void clip_trace_to_player(Ray_t& ray, unsigned int mask, void* entity, trace_t* trace)
		{
			return vfunc::call<2, void>(this, ray, mask, entity, trace);
		}
	};

	class trace_filter
	{
	public:
		virtual void pad_1() { }
		virtual bool should_hit_entity(void* ent, int cmask) // 1
		{
			if (ent == m_ignore_ent)
				return false;

			return true;
		}

		virtual int get_trace_type()
		{
			return 0; /* TRACE_EVERYTHING */
		}

		virtual bool unk2()
		{
			return false;
		}

		virtual bool unk3()
		{
			return false;
		}

		virtual void set_ignore_ent(void* ent)
		{
			m_ignore_ent = ent;
		}

		virtual void setunk(u64 var)
		{
			
		}
	private:
		void* m_ignore_ent;
	};
}
