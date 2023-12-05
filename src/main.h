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
const float SPEEDOMETER_MULTIPLIER = 150.0f / 250.0f; // arc angle / max value

RwTexture *pDialTexture = nullptr;
RwTexture *pArrowTexture = nullptr;
