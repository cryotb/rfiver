#pragma once

typedef unsigned long ulong;
typedef SOCKET sock_t;

#define AUTO auto

#define BASE_OF(X) (DWORD_PTR)X
#define PTR_OF(X) (PVOID)X

#define DECLARE_OFFSET(NAME, OFFS) constexpr u_long NAME = OFFS
#define MOD_BASE(X) BASE_OF(GetModuleHandleA(X))

#define GET_PTYPE( X ) reinterpret_cast<decltype(&X)>
#define HK_GET_ORIG( X ) GET_PTYPE(X)(o_##X)

#define __inline_block __declspec(noinline) inline

#if defined(_DEBUG)
#define IS_DEBUG_BUILD() TRUE
#else
#define IS_DEBUG_BUILD() FALSE
#endif

#if defined(_DEBUG)
#define DBG_BREAK() __debugbreak()
#else
#define DBG_BREAK() __noop()
#endif

#define TICK_COUNT() GetTickCount64()

#define MAKE_PAD(OFF) char pad_##OFF[OFF]

#define LOG_VAR(X) msg("%s => %p", #X, X)
#define HK_VMT_GET_ORIG( MGR, NAME, IDX ) MGR->original<decltype(&NAME)>(IDX)

#define DATA_FIELD(NAME, INST, TYPE, OFFSET) TYPE& NAME() { return *(TYPE*)( (DWORD_PTR)INST + OFFSET); }
#define DATA_FIELD_NR(NAME, INST, TYPE, OFFSET) TYPE* NAME() { return (TYPE*)( (DWORD_PTR)INST + OFFSET); }

#define NETVAR_FIELD(DEF_NAME, INST, TYPE, TABLE, NAME) TYPE& DEF_NAME() { \
                                                              static ulong __offset = NETVAR(TABLE, NAME); \
                                                              return *(TYPE*)( (DWORD_PTR)INST + __offset); \
                                                              }

#define FORWARD_DECLARE(X) X

#define RETURNS_WITHIN_SELF() (BASE_OF(_ReturnAddress()) >= g::self.base && BASE_OF(_ReturnAddress()) < (g::self.base + g::self.length)) 

#define NETVAR_FIELD_NR(DEF_NAME, INST, TYPE, TABLE, NAME) TYPE DEF_NAME() { \
                                                              static ulong __offset = NETVAR(TABLE, NAME); \
                                                              return (TYPE)( (DWORD_PTR)INST + __offset); \
                                                              }

#define TICK_INTERVAL			(ix::globals->IntervalPerTick)


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace d3d
{
    typedef struct _D3DMATRIX {
        union {
            struct {
                float        _11, _12, _13, _14;
                float        _21, _22, _23, _24;
                float        _31, _32, _33, _34;
                float        _41, _42, _43, _44;

            };
            float m[4][4];
        };
    } D3DMATRIX;
}

namespace im
{
    namespace color
    {
        inline ImColor red = ImColor(255, 65, 65);
        inline ImColor green = ImColor(65, 255, 65);
        inline ImColor blue = ImColor(65, 65, 255);
        inline ImColor yellow = ImColor(252, 186, 3);
        inline ImColor white = ImColor(255, 255, 255);
    }
}

#include "itypes.h"

typedef IMAGE_DOS_HEADER _image_dos_header;
typedef IMAGE_NT_HEADERS _image_nt_header;
typedef PIMAGE_DOS_HEADER _pimage_dos_header;
typedef PIMAGE_NT_HEADERS _pimage_nt_header;
typedef IMAGE_NT_HEADERS32 _image_nt_header_x86;
typedef IMAGE_NT_HEADERS64 _image_nt_header_x64;
typedef PIMAGE_NT_HEADERS32 _pimage_nt_header_x86;
typedef PIMAGE_NT_HEADERS64 _pimage_nt_header_x64;
typedef IMAGE_SECTION_HEADER _image_section_header;
typedef PIMAGE_SECTION_HEADER _pimage_section_header;
typedef IMAGE_EXPORT_DIRECTORY _image_export_directory;
typedef PIMAGE_EXPORT_DIRECTORY _pimage_export_directory;
typedef IMAGE_IMPORT_DESCRIPTOR _image_import_descriptor;
typedef PIMAGE_IMPORT_DESCRIPTOR _pimage_import_descriptor;
typedef IMAGE_THUNK_DATA _image_thunk_data;
typedef PIMAGE_THUNK_DATA _pimage_thunk_data;
typedef IMAGE_THUNK_DATA32 _image_thunk_data_x86;
typedef IMAGE_THUNK_DATA64 _image_thunk_data_x64;
typedef PIMAGE_THUNK_DATA _pimage_thunk_data;
typedef PIMAGE_THUNK_DATA32 _pimage_thunk_data_x86;
typedef PIMAGE_THUNK_DATA64 _pimage_thunk_data_x64;
