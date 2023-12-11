#include "main.h"

MYMOD(net.kubikas3.speedo, Speedo, 1.3.1, kubikas3)

BEGIN_DEPLIST()
ADD_DEPENDENCY_VER(net.rusjj.aml, 1.1)
END_DEPLIST()

NEEDGAME(com.rockstargames.gtasa)

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

void RotateVertices(float *pVertices, int numVertices, float angle, float cx, float cy)
{
    float c = cosf(angle);
    float s = sinf(angle);
    int lenVertices = numVertices * 2;

    for (int i = 0; i < lenVertices; i += 2)
    {
        float dx = pVertices[i] - cx;
        float dy = pVertices[i + 1] - cy;

        pVertices[i] = dx * c - dy * s + cx;
        pVertices[i + 1] = dx * s + dy * c + cy;
    }
}

void DrawTexture(RwTexture *pTexture, CRGBA const &color, float x, float y, float width, float height, float angle = 0)
{
    float x2 = x + width,
          y2 = y + height;

    float vertices[8] = {
        x, y,
        x, y2,
        x2, y2,
        x2, y};

    float tcs[8] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};

    if (angle != 0.0f)
    {
        RotateVertices(vertices, 4, angle, x + width / 2, y + height / 2);
    }

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pTexture->raster);
    SetVertices(4, vertices, tcs, color);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
}

DECL_HOOK(void, RadarLoadTextures, void *self)
{
    logger->Info("Loading sprites");

    const char *pDirectory = "texture/speedo";

    pDialTexture = LoadTextureFromPNG(pDirectory, "sdial");
    pNumbersTexture = LoadTextureFromPNG(pDirectory, "snum");
    pArrowTexture = LoadTextureFromPNG(pDirectory, "sarrow");

    pGlowDialTexture = LoadTextureFromPNG(pDirectory, "gdial");
    pGlowArrowTexture = LoadTextureFromPNG(pDirectory, "garrow");

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
            float angle = speedKmph / SPEEDOMETER_MAX_VALUE * SPEEDOMETER_ARC_ANGLE;

            CRect radarRect;
            GetWidgetScreenRect(ppWidgets[RADAR_WIDGET_ID], &radarRect);

            float radarWidth = radarRect.right - radarRect.left;
            float radarHeight = radarRect.bottom - radarRect.top;
            float speedoWidth = (radarWidth) * 2.0f;
            float speedoHeight = radarHeight * 2.0f;
            float x = radarRect.left + (radarWidth - speedoWidth) / 2;
            float y = radarRect.top + (radarHeight - speedoHeight) / 2;

            unsigned char alpha = 255;
            GetWidgetAlpha(ppWidgets[HORN_WIDGET_ID], &alpha);

            CRGBA black(0, 0, 0, alpha);
            float nitroValue = 0;
            GetVehicleNitroValue(pVehicle, &nitroValue);

            RwTexture *pArrowTex = pArrowTexture;

            if (nitroValue < 1)
            {
                RwTexture *pIndicatorTexture = pDialTexture;
                CRGBA indicatorColor(152, 152, 152, alpha);

                if (nitroValue < 0)
                {
                    nitroValue += 1;
                    indicatorColor = CRGBA(115, 255, 250, alpha);
                    pIndicatorTexture = pGlowDialTexture;
                    pArrowTex = pGlowArrowTexture;
                }

                float tcs[8] = {
                    256.0f / 512.0f, 256.0f / 512.0f,
                    470.6997f / 512.0f, 116.5724f / 512.0f,
                    437.0193f / 512.0f, 437.0193f / 512.0f,
                    116.5724f / 512.0f, 470.6997f / 512.0f};
                float angle = floorf(NITRO_INDICATOR_ANGLE * -nitroValue / SPEEDOMETER_GAP_ANGLE) * SPEEDOMETER_GAP_ANGLE;
                RotateVertices(tcs, 4, angle, 0.5f, 0.5f);

                float vertices[8];
                for (int i = 0; i < 8; i += 2)
                {
                    vertices[i] = x + tcs[i] * speedoWidth;
                    vertices[i + 1] = y + tcs[i + 1] * speedoHeight;
                }

                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pDialTexture->raster);
                SetVertices(4, vertices, tcs, black);
                RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);

                RotateVertices(tcs, 4, NITRO_INDICATOR_ANGLE, 0.5f, 0.5f);
                for (int i = 0; i < 8; i += 2)
                {
                    vertices[i] = x + tcs[i] * speedoWidth;
                    vertices[i + 1] = y + tcs[i + 1] * speedoHeight;
                }

                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pIndicatorTexture->raster);
                SetVertices(4, vertices, tcs, indicatorColor);
                RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 4);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, 0);
            }
            else
            {
                DrawTexture(pDialTexture, black, x, y, speedoWidth, speedoHeight);
            }

            CRGBA white(255, 255, 255, alpha);
            DrawTexture(pNumbersTexture, white, x, y, speedoWidth, speedoHeight);
            CRGBA red(255, 39, 44, alpha);
            DrawTexture(pArrowTex, red, x, y, speedoWidth, speedoHeight, -angle);
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
    logger->SetTag("SpeedoSA");
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

        // CSprite2d
        SET_TO(maVertices, aml->GetSym(hLibGTASA, "_ZN9CSprite2d10maVerticesE"));
        SET_TO(SetVertices, aml->GetSym(hLibGTASA, "_ZN9CSprite2d11SetVerticesEiPfS0_RK5CRGBA"));

        // RenderWare
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

        // Radar Hooks
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
