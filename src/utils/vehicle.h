#include <cstdint>
#include <mod/amlmod.h>
#include "../game_sa/CVector.h"

enum eVehicleType
{
    VEHICLE_AUTOMOBILE,
    VEHICLE_MTRUCK,
    VEHICLE_QUAD,
    VEHICLE_HELI,
    VEHICLE_PLANE,
    VEHICLE_BOAT,
    VEHICLE_TRAIN,
    VEHICLE_FHELI,
    VEHICLE_FPLANE,
    VEHICLE_BIKE,
    VEHICLE_BMX,
    VEHICLE_TRAILER
};

void GetVehicleSpeedVec(uintptr_t pVehicle, CVector *pSpeedVec);
void GetVehicleSubClass(uintptr_t pVehicle, eVehicleType *pSubClass);
void GetVehicleNitroValue(uintptr_t pVehicle, float *pNitroValue);
