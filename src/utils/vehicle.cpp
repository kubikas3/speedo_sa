#include "vehicle.h"
#include "../globals.h"
#include "../utils/invoke.h"

void GetVehicleSpeedVec(uintptr_t pVehicle, CVector *pSpeed)
{
    if (pVehicle)
    {
        pSpeed->x = *(float *)(pVehicle + 72);
        pSpeed->y = *(float *)(pVehicle + 76);
        pSpeed->z = *(float *)(pVehicle + 80);
    }
}

void GetVehicleSubClass(uintptr_t pVehicle, eVehicleType *pSubClass)
{
    if (pVehicle)
    {
        *pSubClass = *(eVehicleType *)(pVehicle + 0x5A4);
    }
}
