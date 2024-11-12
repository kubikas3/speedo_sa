#include "main.h"

MYMODCFG(net.kubikas3.speedo, Speedo, 1.5.0, kubikas3)

BEGIN_DEPLIST()
ADD_DEPENDENCY_VER(net.rusjj.aml, 1.0.1)
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
        RotateVertices(vertices, 4, angle, x + width / 2.0f, y + height / 2.0f);
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
            CRect radarRect;
            GetWidgetScreenRect(ppWidgets[RADAR_WIDGET_ID], &radarRect);
            float radarWidth = radarRect.right - radarRect.left;
            float radarHeight = radarRect.bottom - radarRect.top;

            float speedoWidth = ((pCfgSizeX == nullptr) ? radarWidth : *pCfgSizeX) * cfgScaleX;
            float speedoHeight = ((pCfgSizeY == nullptr) ? radarHeight : *pCfgSizeY) * cfgScaleY;

            float x = ((pCfgPositionX == nullptr) ? radarRect.left + (radarWidth - speedoWidth) / 2.0f : *pCfgPositionX) + cfgOffsetX;
            float y = ((pCfgPositionY == nullptr) ? radarRect.top + (radarHeight - speedoHeight) / 2.0f : *pCfgPositionY) + cfgOffsetY;

            unsigned char alpha = 255;
            GetWidgetAlpha(ppWidgets[HORN_WIDGET_ID], &alpha);
            float alphaNormalized = alpha / 255.0f;

            float nitroValue = 1.0f;
            if (vehicleSubClass == VEHICLE_AUTOMOBILE)
            {
                GetVehicleNitroValue(pVehicle, &nitroValue);
            }

            RwTexture *pArrowTex = pArrowTexture;
            CRGBA colorDial(cfgDialColor.r, cfgDialColor.g, cfgDialColor.b, static_cast<unsigned char>(alphaNormalized * cfgDialColor.a));

            if (nitroValue < 1.0f)
            {
                RwTexture *pIndicatorTexture = pDialTexture;
                CRGBA nitroColor(cfgNitroLoadingColor.r, cfgNitroLoadingColor.g, cfgNitroLoadingColor.b, static_cast<unsigned char>(alphaNormalized * cfgNitroLoadingColor.a));

                if (nitroValue < 0.0f)
                {
                    nitroValue += 1.0f;
                    nitroColor = CRGBA(cfgNitroActiveColor.r, cfgNitroActiveColor.g, cfgNitroActiveColor.b, static_cast<unsigned char>(alphaNormalized * cfgNitroActiveColor.a));
                    pIndicatorTexture = pGlowDialTexture;
                    pArrowTex = pGlowArrowTexture;
                }

                float tcs[12] = {0.5f, 0.5f};
                float vertices[12];
                float nitroTickCount = cfgDialTickCount / cfgDialArcAngle * abs(cfgNitroArcMaskEndAngle - cfgNitroArcMaskStartAngle);
                float snappedNitroValue = ceilf(nitroValue * nitroTickCount) / nitroTickCount;

                for (int i = 0; i < 10; i += 2)
                {
                    float a = cfgNitroArcMaskStartAngle + (cfgNitroArcMaskEndAngle - cfgNitroArcMaskStartAngle) * (i / 8.0f) * snappedNitroValue;
                    tcs[2 + i] = tcs[0] + cosf(a);
                    tcs[3 + i] = tcs[1] + sinf(a);
                }

                for (int i = 0; i < 12; i += 2)
                {
                    vertices[i] = x + tcs[i] * speedoWidth;
                    vertices[i + 1] = y + tcs[i + 1] * speedoHeight;
                }

                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pIndicatorTexture->raster);
                SetVertices(6, vertices, tcs, nitroColor);
                RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 6);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);

                for (int i = 0; i < 10; i += 2)
                {
                    float a = cfgNitroArcMaskEndAngle - (cfgNitroArcMaskEndAngle - cfgNitroArcMaskStartAngle) * (i / 8.0f) * (1.0f - snappedNitroValue);
                    tcs[2 + i] = tcs[0] + cosf(a);
                    tcs[3 + i] = tcs[1] + sinf(a);
                }

                for (int i = 0; i < 12; i += 2)
                {
                    vertices[i] = x + tcs[i] * speedoWidth;
                    vertices[i + 1] = y + tcs[i + 1] * speedoHeight;
                }

                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pDialTexture->raster);
                SetVertices(6, vertices, tcs, colorDial);
                RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, maVertices, 6);
                RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
            }
            else
            {
                DrawTexture(pDialTexture, colorDial, x, y, speedoWidth, speedoHeight);
            }

            CRGBA colorNumbers(cfgNumbersColor.r, cfgNumbersColor.g, cfgNumbersColor.b, static_cast<unsigned char>(alphaNormalized * cfgNumbersColor.a));
            DrawTexture(pNumbersTexture, colorNumbers, x, y, speedoWidth, speedoHeight);

            CVector vehicleVelocity;
            GetVehicleSpeedVec(pVehicle, &vehicleVelocity);

            float vehicleSpeed = vehicleVelocity.Magnitude() * ((cfgMeasurementSystem == eMeasurementSystem::METRIC) ? 180.0f : 111.8468f);
            float arrowAngle = vehicleSpeed / cfgDialMaxValue * cfgDialArcAngle;

            CRGBA colorArrow(cfgArrowColor.r, cfgArrowColor.g, cfgArrowColor.b, static_cast<unsigned char>(alphaNormalized * cfgArrowColor.a));
            DrawTexture(pArrowTex, colorArrow, x, y, speedoWidth, speedoHeight, -arrowAngle);
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

extern "C" void OnAllModsLoaded()
{
    logger->SetTag("SpeedoSA");

    const char *szCfgPositionX = cfg->GetString("PositionX", "auto", "Layout");
    if (strcasecmp(szCfgPositionX, "auto"))
    {
        pCfgPositionX = new float;
        *pCfgPositionX = strtof(szCfgPositionX, nullptr);
    }

    const char *szCfgPositionY = cfg->GetString("PositionY", "auto", "Layout");
    if (strcasecmp(szCfgPositionY, "auto"))
    {
        pCfgPositionY = new float;
        *pCfgPositionY = strtof(szCfgPositionY, nullptr);
    }

    const char *szCfgSizeX = cfg->GetString("SizeX", "auto", "Layout");
    if (strcasecmp(szCfgSizeX, "auto"))
    {
        pCfgSizeX = new float;
        *pCfgSizeX = strtof(szCfgSizeX, nullptr);
    }

    const char *szCfgSizeY = cfg->GetString("SizeY", "auto", "Layout");
    if (strcasecmp(szCfgSizeY, "auto"))
    {
        pCfgSizeY = new float;
        *pCfgSizeY = strtof(szCfgSizeY, nullptr);
    }

    cfgOffsetX = cfg->GetFloat("OffsetX", cfgOffsetX, "Layout");
    cfgOffsetY = cfg->GetFloat("OffsetY", cfgOffsetY, "Layout");
    cfgScaleX = cfg->GetFloat("ScaleX", cfgScaleX, "Layout");
    cfgScaleY = cfg->GetFloat("ScaleY", cfgScaleY, "Layout");

    cfgDialMaxValue = cfg->GetFloat("DialMaxValue", cfgDialMaxValue, "Display");
    cfgDialTickCount = cfg->GetInt("DialTickCount", cfgDialTickCount, "Display");
    cfgDialArcAngle = cfg->GetFloat("DialArcAngle", cfgDialArcAngle, "Display");
    cfgDialArcAngle *= DEG_TO_RAD;
    cfgNitroArcMaskStartAngle = cfg->GetFloat("NitroArcMaskStartAngle", cfgNitroArcMaskStartAngle, "Display");
    cfgNitroArcMaskStartAngle *= DEG_TO_RAD;
    cfgNitroArcMaskEndAngle = cfg->GetFloat("NitroArcMaskEndAngle", cfgNitroArcMaskEndAngle, "Display");
    cfgNitroArcMaskEndAngle *= DEG_TO_RAD;

    const char *szMeasurementSystem = cfg->GetString("MeasurementSystem", "metric", "Display");
    if (strcasecmp(szMeasurementSystem, "metric"))
    {
        cfgMeasurementSystem = eMeasurementSystem::IMPERIAL;
    }

    cfgDialColor = cfg->GetColor("DialColor", cfgDialColor, "Palette");
    cfgNumbersColor = cfg->GetColor("NumbersColor", cfgNumbersColor, "Palette");
    cfgNitroLoadingColor = cfg->GetColor("NitroLoadingColor", cfgNitroLoadingColor, "Palette");
    cfgNitroActiveColor = cfg->GetColor("NitroActiveColor", cfgNitroActiveColor, "Palette");
    cfgArrowColor = cfg->GetColor("ArrowColor", cfgArrowColor, "Palette");

    uintptr_t pLibGTASA = aml->GetLib("libGTASA.so");
    void *hLibGTASA = aml->GetLibHandle("libGTASA.so");

    if (hLibGTASA)
    {
#ifdef AML32
        ppWidgets = *(uintptr_t **)(pLibGTASA + 0x67947C);
#else
        ppWidgets = *(uintptr_t **)(pLibGTASA + 0x850910);
#endif
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
}

extern "C" void OnModUnload()
{
    if (pCfgPositionX)
    {
        delete pCfgPositionX;
    }

    if (pCfgPositionY)
    {
        delete pCfgPositionY;
    }

    if (pCfgSizeX)
    {
        delete pCfgSizeX;
    }

    if (pCfgSizeY)
    {
        delete pCfgSizeY;
    }
}