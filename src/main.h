#include <cstdint>
#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include "game_sa/rw/rwcore.h"
#include "game_sa/CRect.h"
#include "game_sa/CRGBA.h"
#include "utils/vehicle.h"
#include "utils/widget.h"

uintptr_t *ppWidgets = 0;
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
const float PI = 3.141592653589793f;
const float SPEEDOMETER_MAX_VALUE = 250.0f;
const float SPEEDOMETER_ARC_ANGLE = 150.0f / 180.0f * PI;
const float SPEEDOMETER_TICK_COUNT = 25;
const float SPEEDOMETER_GAP_ANGLE = SPEEDOMETER_ARC_ANGLE / SPEEDOMETER_TICK_COUNT;
const float NITRO_INDICATOR_ANGLE = SPEEDOMETER_ARC_ANGLE + SPEEDOMETER_GAP_ANGLE;

RwTexture *pDialTexture = nullptr;
RwTexture *pNumbersTexture = nullptr;
RwTexture *pArrowTexture = nullptr;
RwTexture *pGlowDialTexture = nullptr;
RwTexture *pGlowArrowTexture = nullptr;
