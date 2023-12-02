#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include "globals.h"
#include "game_sa/rw/RenderWare.h"
#include "game_sa/common.h"
#include "game_sa/CRGBA.h"
#include "game_sa/CRect.h"
#include "game_sa/CPhysical.h"
#include "game_sa/CSprite2d.h"
#include "utils/texture.h"
#include "utils/vehicle.h"

MYMOD(net.kubikas3.speedo, Speedo, 1.0, kubikas3)

BEGIN_DEPLIST()
ADD_DEPENDENCY_VER(net.rusjj.aml, 1.1)
END_DEPLIST()

NEEDGAME(com.rockstargames.gtasa)

const float PI = 3.141592653589793f;
const float SPEEDOMETER_MULTIPLIER = 150.0f / 250.0f; // arc angle / max value
const CRGBA WHITE(255, 255, 255);

CSprite2d *pDialSprite;
CSprite2d *pArrowSprite;

DECL_HOOK(void, RadarLoadTextures, void *self)
{
    logger->Info("Loading sprites");

    pDialSprite = new CSprite2d;
    pDialSprite->m_pRwTexture = (RwTexture *)LoadTextureFromDB("txd", "sspeed");

    pArrowSprite = new CSprite2d;
    pArrowSprite->m_pRwTexture = (RwTexture *)LoadTextureFromDB("txd", "sarrow");

    logger->Info("Sprites loaded successfully");

    return RadarLoadTextures(self);
}

DECL_HOOK(void, DrawMap, void *self)
{
    uintptr_t pVehicle = FindPlayerVehicle(-1, 0);

    if (pVehicle)
    {
        eVehicleType vehicleSubClass;
        GetVehicleSubClass(pVehicle, &vehicleSubClass);

        if (vehicleSubClass == VEHICLE_AUTOMOBILE || vehicleSubClass == VEHICLE_MTRUCK ||
            vehicleSubClass == VEHICLE_BIKE || vehicleSubClass == VEHICLE_QUAD)
        {
            CVector speedVec;
            GetVehicleSpeedVec(pVehicle, &speedVec);

            float speedKmph = speedVec.Magnitude() * 180;
            float angle = SPEEDOMETER_MULTIPLIER * speedKmph / 180 * PI;

            uintptr_t pRadarWidget = *(uintptr_t *)(g_pLibGTASA + 0x6F3A18);
            CRect *pRadarRect = (CRect *)(pRadarWidget + 0x20);
            float radarWidth = pRadarRect->right - pRadarRect->left;
            float radarHeight = pRadarRect->bottom - pRadarRect->top;
            float speedoWidth = radarWidth * 1.9f;
            float speedoHeight = radarHeight * 1.9f;
            float x = pRadarRect->left + (radarWidth - speedoWidth) / 2;
            float y = pRadarRect->top + (radarHeight - speedoHeight) / 2;

            pDialSprite->Draw(x, y, speedoWidth, speedoHeight, WHITE);
            pArrowSprite->Draw(x, y, speedoWidth, speedoHeight, -angle, 0.5f, 0.5f, WHITE);
        }
    }

    return DrawMap(self);
}

DECL_HOOK(void, RadarShutdown, void *self)
{
    logger->Info("Unloading sprites");

    if (pDialSprite)
    {
        pDialSprite->Delete();
    }

    if (pArrowSprite)
    {
        pArrowSprite->Delete();
    }

    logger->Info("Sprites unloaded successfully");

    return RadarShutdown(self);
}

extern "C" void OnModLoad()
{
    logger->SetTag("Speedo");
    g_pLibGTASA = aml->GetLib("libGTASA.so");
    g_hLibGTASA = aml->GetLibHandle("libGTASA.so");

    if (g_pLibGTASA)
    {
        logger->Info("Speedo mod is loaded!");

        HOOK(RadarLoadTextures, aml->GetSym(g_hLibGTASA, "_ZN6CRadar12LoadTexturesEv"));
        HOOK(DrawMap, aml->GetSym(g_hLibGTASA, "_ZN6CRadar7DrawMapEv"));
        HOOK(RadarShutdown, aml->GetSym(g_hLibGTASA, "_ZN6CRadar8ShutdownEv"));
    }
    else
    {
        logger->Error("Speedo mod is not loaded :(");
        return;
    }
}
