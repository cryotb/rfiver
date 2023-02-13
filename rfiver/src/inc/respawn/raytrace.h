#pragma once

namespace respawn
{
	struct cplane_t
	{
		math::vec3_t			surfaceNormal;			//0x0000
		float			dist;					//0x000C
												//char			signbits;		// signx + (signy<<1) + (signz<<1)
												//char			pad[2];
	}; //0x0010


	struct csurface_t
	{
		const char* name;					//0x0000
		int				surfaceProp;			//0x0008
		unsigned short	flags;					//0x000A
	}; //0x0010 (aligned)

	struct trace_t
	{
		math::vec3_t			start;						//0x0000
		int				unk000;					//0x000C
		math::vec3_t			endpos;						//0x0010
		char			_unk0x001C[0x4];			//0x001C
		cplane_t		plane;						//0x0020
		float			fraction;					//0x0030
		int				contents;					//0x0034
		bool			allSolid;					//0x0038
		bool			startSolid;					//0x0039
		char			_unk0x003A[0x6];			//0x003A
		csurface_t		surface;					//0x0040
		float			fractionleftsolid;			//0x0050
		int				hitgroup;					//0x0054
		short			physicsBone;				//0x0058
		char			_unk0x005A[0x6];			//0x005A
		void* m_pEnt;					//0x0060
		int				hitbox;						//0x0068
		char			_unk0x006C[0x114];			//0x006C

	}; //Size = 0x180

	class Ray_t
	{
	public:
		math::Vector4 start_pos;			//0x000
		math::Vector4 delta_pos;			//0x010
		math::Vector4 m_StartOffset;		//0x020
		math::VectorAligned3 m_Extents;			//0x030
		char gap2C[0x10]; // probably contains m_Extents

		math::matrix3x4* m_pWorldAxisTransform;
		bool m_IsRay;
		bool m_IsSwept;

		//Ray_t() : m_pWorldAxisTransform(NULL) {}

		void Init(math::vec3_t vecStart, math::vec3_t vecEnd)
		{
			delta_pos = math::ToVector4((vecEnd - vecStart), 0);
			m_IsSwept = (delta_pos.LengthSquare() != 0.f);
			m_Extents.Zero();
			m_pWorldAxisTransform = NULL;
			m_IsRay = true;
			m_StartOffset = { 0, 0, 0, 0 };
			start_pos = math::ToVector4(vecStart, 0);

		}
	}; //Size: 0x0168
}
