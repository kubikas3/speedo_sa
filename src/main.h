#include <cstdint>
#include <cstdlib>
#include <string.h>
#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include "game_sa/rw/rwcore.h"
#include "game_sa/CRect.h"
#include "game_sa/CRGBA.h"
#include "utils/vehicle.h"
#include "utils/widget.h"

enum eMeasurementSystem
{
    METRIC,
    IMPERIAL,
};

uintptr_t *ppWidgets = nullptr;
uintptr_t (*FindPlayerVehicle)(int playerId, bool remoteVehicle);

RwOpenGLVertex *maVertices;
void (*SetVertices)(int number, float *pVertices, float *pTcs, CRGBA const &color);
void (*RwRenderStateSet)(RwRenderState, void *);
void (*RwIm2DRenderPrimitive)(RwPrimitiveType primType, RwOpenGLVertex *vertices, int numVertices);

RwImage *(*RtPNGImageRead)(const RwChar *imageName);
RwImage *(*RwImageFindRasterFormat)(RwImage *ipImage, RwInt32 nRasterType, RwInt32 *npWidth, RwInt32 *npHeight, RwInt32 *npDepth, RwInt32 *npFormat);
RwRaster *(*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
RwRaster *(*RwRasterSetFromImage)(RwRaster *raster, RwImage *image);
RwBool *(*RwImageDestroy)(RwImage *image);
RwTexture *(*RwTextureCreate)(RwRaster *raster);
int (*RwTextureDestroy)(RwTexture *texture);
RwTexture *(*RwTextureSetName)(RwTexture *texture, char const *name);

const unsigned int HORN_WIDGET_ID = 7;
const unsigned int RADAR_WIDGET_ID = 161;
const float DEG_TO_RAD = 3.141592653589793f / 180.0f;

float *pCfgPositionX = nullptr;
float *pCfgPositionY = nullptr;
float *pCfgSizeX = nullptr;
float *pCfgSizeY = nullptr;
float cfgOffsetX = 0.0f;
float cfgOffsetY = 0.0f;
float cfgScaleX = 2.0f;
float cfgScaleY = 2.0f;

unsigned int cfgDialTickCount = 25;
float cfgDialMaxValue = 250.0f;
float cfgDialArcAngle = 150.0f;
float cfgNitroArcMaskStartAngle = 123.0f;
float cfgNitroArcMaskEndAngle = -33.0f;
eMeasurementSystem cfgMeasurementSystem = eMeasurementSystem::METRIC;

rgba_t cfgDialColor(0, 0, 0);
rgba_t cfgArrowColor(255, 39, 44);
rgba_t cfgNumbersColor(255, 255, 255);
rgba_t cfgNitroLoadingColor(152, 152, 152);
rgba_t cfgNitroActiveColor(115, 255, 250);

RwTexture *pDialTexture = nullptr;
RwTexture *pNumbersTexture = nullptr;
RwTexture *pArrowTexture = nullptr;
RwTexture *pGlowDialTexture = nullptr;
RwTexture *pGlowArrowTexture = nullptr;
