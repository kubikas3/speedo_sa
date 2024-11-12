#include "vehicle.h"

#ifdef AML32
const uintptr_t pVehicleSpeedVec = 0x48;
const uintptr_t pVehicleSubClass = 0x5A4;
const uintptr_t pVehicleNitro = 0x8B8;
#else
const uintptr_t pVehicleSpeedVec = 0x68;
const uintptr_t pVehicleSubClass = 0x738;
const uintptr_t pVehicleNitro = 0xAC0;
#endif

void GetVehicleSpeedVec(uintptr_t pVehicle, CVector *pSpeedVec)
{
    if (pVehicle)
    {
        *pSpeedVec = *(CVector *)(pVehicle + pVehicleSpeedVec);
    }
}

void GetVehicleSubClass(uintptr_t pVehicle, eVehicleType *pSubClass)
{
    if (pVehicle)
    {
        *pSubClass = *(eVehicleType *)(pVehicle + pVehicleSubClass);
    }
}

void GetVehicleNitroValue(uintptr_t pVehicle, float *pNitroValue)
{
    if (pVehicle)
    {
        *pNitroValue = *(float *)(pVehicle + pVehicleNitro);
    }
}
