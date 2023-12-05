#include <cstdint>
#include "game_sa/rw/rwcore.h"

uintptr_t *ppWidgets = 0;
int (*FindPlayerVehicle)(int playerId, bool unknown);

// RenderWare
float *NearScreenZ;
float *RecipNearClip;

// RwOpenGLVertex *maVertices;
// void (*SetVertices)(CRect const &posn, CRGBA const &color1, CRGBA const &color2, CRGBA const &color3, CRGBA const &color4, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4);

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

const unsigned int RADAR_WIDGET_ID = 161;
const float PI = 3.141592653589793f;
const float SPEEDOMETER_MULTIPLIER = 150.0f / 250.0f; // arc angle / max value

RwTexture *pDialTexture;
RwTexture *pArrowTexture;
