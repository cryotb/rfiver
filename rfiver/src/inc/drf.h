#pragma once

namespace __drf
{
    __forceinline auto opaque_predicate_fuck_stack()
    {

    }

    __forceinline auto opaque_predicate_rip_access()
    {
    }
}

#define DRFAR_FORCE_FAKE_RIP_ACCESS() __drf::opaque_predicate_rip_access()
#define DRFAR_FORCE_STACK_FUCK() __drf::opaque_predicate_fuck_stack()
#define DRFAR_INVOKE_AS_FAKE_DATA(X) DRFAR_FORCE_FAKE_RIP_ACCESS(); X
