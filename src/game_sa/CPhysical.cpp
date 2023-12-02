#include "CPhysical.h"
#include "../globals.h"
#include "../utils/invoke.h"

void CPhysical::GetSpeed(CVector *pSpeed, uintptr_t pPhysical)
{
    Invoke::Method<void, CVector *, uintptr_t>(g_pLibGTASA + 0x3FD724 + 1, pSpeed, pPhysical);
    // ((void (*)(CVector *, void *))(g_pLibGTASA + 0x3FD724 + 1))(pSpeed, pPhysical);
}
