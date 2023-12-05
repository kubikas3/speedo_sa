#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include "main.h"
#include "game_sa/rw/rwcore.h"
#include "game_sa/CRGBA.h"
#include "game_sa/CRect.h"
#include "utils/vehicle.h"
#include "utils/widget.h"

MYMOD(net.kubikas3.speedo, Speedo, 1.2, kubikas3)

BEGIN_DEPLIST()
ADD_DEPENDENCY_VER(net.rusjj.aml, 1.1)
END_DEPLIST()

NEEDGAME(com.rockstargames.gtasa)

void Setup2DVertex(RwOpenGLVertex &vertex, float x, float y, float u, float v, RwUInt32 color)
{
    vertex.x = x;
    vertex.y = y;
    vertex.z = *NearScreenZ + 0.0001f;
    vertex.u = u;
    vertex.v = v;
    vertex.rhw = *RecipNearClip;
    vertex.emissiveColor = color;
}

RwTexture *LoadTextureFromPNG(const char *path, const char *name)
{
    char file[512];
    int w, h, d, f;
    RwTexture *pTexture = nullptr;

    sprintf(file, "%s/%s.png", path, name);

    RwImage *pImage = RtPNGImageRead(file);

    if (!pImage)
    {
        logger->Error("Failed to read %s image", name);
    }

    RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &w, &h, &d, &f);
    RwRaster *pRaster = RwRasterCreate(w, h, d, f);

    if (!pRaster)
    {
        logger->Error("Failed to create raster for %s", name);
    }

    RwRasterSetFromImage(pRaster, pImage);
    pTexture = RwTextureCreate(pRaster);

    if (!pTexture)
    {
        logger->Error("Failed to create texture from %s", name);
    }

    RwTextureSetName(pTexture, name);

    if ((pTexture->raster->cFormat & 0x80) == 0)
        RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
    else
        RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);

    RwTextureSetAddressing(pTexture, rwTEXTUREADDRESSWRAP);

    RwImageDestroy(pImage);

    return pTexture;
}

void RotateVertices(RwOpenGLVertex *pVertices, int numVertices, float angle, float cx, float cy)
{
    float c = cosf(angle);
    float s = sinf(angle);

    for (int i = 0; i < numVertices; i++)
    {
        float dx = pVertices[i].x - cx;
        float dy = pVertices[i].y - cy;
        pVertices[i].x = dx * c - dy * s + cx;
        pVertices[i].y = dx * s + dy * c + cy;
    }
}

void DrawTexture(RwTexture *pTexture, RwUInt32 color, float x, float y, float width, float height, float angle = 0)
{
    float x2 = x + width,
          y2 = y + height;

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pTexture->raster);

    RwOpenGLVertex vertices[4];
    Setup2DVertex(vertices[0], x, y, 0.0f, 0.0f, color);
    Setup2DVertex(vertices[1], x, y2, 0.0f, 1.0f, color);
    Setup2DVertex(vertices[2], x2, y2, 1.0f, 1.0f, color);
    Setup2DVertex(vertices[3], x2, y, 1.0f, 0.0f, color);

    if (angle != 0.0f)
    {
        RotateVertices(vertices, 4, angle, x + width / 2, y + height / 2);
    }

    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, vertices, 4);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
}

DECL_HOOK(void, RadarLoadTextures, void *self)
{
    logger->Info("Loading sprites");

    pDialTexture = LoadTextureFromPNG("texture/speedo", "sspeed");

    if (!pDialTexture)
    {
        logger->Error("Failed to load sspeed texture");
    }

    pArrowTexture = LoadTextureFromPNG("texture/speedo", "sarrow");

    if (!pArrowTexture)
    {
        logger->Error("Failed to load sarrow texture");
    }

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

            CRect radarRect;
            GetWidgetRect(ppWidgets[RADAR_WIDGET_ID], &radarRect);

            float radarWidth = radarRect.right - radarRect.left;
            float radarHeight = radarRect.bottom - radarRect.top;
            float speedoWidth = radarWidth * 1.9f;
            float speedoHeight = radarHeight * 1.9f;
            float x = radarRect.left + (radarWidth - speedoWidth) / 2;
            float y = radarRect.top + (radarHeight - speedoHeight) / 2;

            CRGBA color(255, 255, 255, 255);
            GetWidgetAlpha(ppWidgets[RADAR_WIDGET_ID], &color.a);
            RwInt32 colorInt = color.ToIntARGB();

            DrawTexture(pDialTexture, colorInt, x, y, speedoWidth, speedoHeight);
            DrawTexture(pArrowTexture, colorInt, x, y, speedoWidth, speedoHeight, -angle);
        }
    }

    return DrawMap(self);
}

DECL_HOOK(void, RadarShutdown, void *self)
{
    logger->Info("Unloading sprites");

    if (pDialTexture)
    {
        RwTextureDestroy(pDialTexture);
    }

    if (pArrowTexture)
    {
        RwTextureDestroy(pArrowTexture);
    }

    logger->Info("Sprites unloaded successfully");

    return RadarShutdown(self);
}

extern "C" void OnModLoad()
{
    logger->SetTag("Speedo");
    void *hLibGTASA = aml->GetLibHandle("libGTASA.so");
    void *hLibSAUtils = aml->GetLibHandle("libSAUtils.so");

    if (hLibGTASA)
    {
        logger->Info("Speedo mod is loaded!");

        if (hLibSAUtils)
        {
            logger->Info("Using SAUtils widgets pointer");
            SET_TO(ppWidgets, aml->GetSym(hLibSAUtils, "pNewWidgets"));
        }
        else
        {
            logger->Info("Using default widgets pointer");
            SET_TO(ppWidgets, aml->GetSym(hLibGTASA, "_ZN15CTouchInterface10m_pWidgetsE"));
        }

        SET_TO(FindPlayerVehicle, aml->GetSym(hLibGTASA, "_Z17FindPlayerVehicleib"));

        SET_TO(NearScreenZ, aml->GetSym(hLibGTASA, "_ZN9CSprite2d11NearScreenZE"));
        SET_TO(RecipNearClip, aml->GetSym(hLibGTASA, "_ZN9CSprite2d13RecipNearClipE"));

        // SET_TO(maVertices, aml->GetSym(hLibGTASA, "_ZN9CSprite2d10maVerticesE"));
        // SET_TO(SetVertices, aml->GetSym(hLibGTASA, "_ZN9CSprite2d11SetVerticesEP14RwOpenGLVertexRK5CRectRK5CRGBAS7_S7_S7_ffffffff"));

        SET_TO(RwRenderStateSet, aml->GetSym(hLibGTASA, "_Z16RwRenderStateSet13RwRenderStatePv"));
        SET_TO(RwIm2DRenderPrimitive, aml->GetSym(hLibGTASA, "_Z28RwIm2DRenderPrimitive_BUGFIX15RwPrimitiveTypeP14RwOpenGLVertexi"));

        // Load PNG
        SET_TO(RtPNGImageRead, aml->GetSym(hLibGTASA, "_Z14RtPNGImageReadPKc"));
        SET_TO(RwImageFindRasterFormat, aml->GetSym(hLibGTASA, "_Z23RwImageFindRasterFormatP7RwImageiPiS1_S1_S1_"));
        SET_TO(RwRasterCreate, aml->GetSym(hLibGTASA, "_Z14RwRasterCreateiiii"));
        SET_TO(RwRasterSetFromImage, aml->GetSym(hLibGTASA, "_Z20RwRasterSetFromImageP8RwRasterP7RwImage"));
        SET_TO(RwImageDestroy, aml->GetSym(hLibGTASA, "_Z14RwImageDestroyP7RwImage"));
        SET_TO(RwTextureCreate, aml->GetSym(hLibGTASA, "_Z15RwTextureCreateP8RwRaster"));
        SET_TO(RwTextureDestroy, aml->GetSym(hLibGTASA, "_Z16RwTextureDestroyP9RwTexture"));
        SET_TO(RwTextureSetName, aml->GetSym(hLibGTASA, "_Z16RwTextureSetNameP9RwTexturePKc"));

        HOOK(RadarLoadTextures, aml->GetSym(hLibGTASA, "_ZN6CRadar12LoadTexturesEv"));
        HOOK(DrawMap, aml->GetSym(hLibGTASA, "_ZN6CRadar7DrawMapEv"));
        HOOK(RadarShutdown, aml->GetSym(hLibGTASA, "_ZN6CRadar8ShutdownEv"));
    }
    else
    {
        logger->Error("Speedo mod is not loaded :(");
        return;
    }
}
