#pragma once
#include "rw/rwcore.h"

RwImage *RtPNGImageRead(const RwChar *imageName);
RwImage *RwImageFindRasterFormat(RwImage *ipImage, RwInt32 nRasterType, RwInt32 *npWidth, RwInt32 *npHeight, RwInt32 *npDepth, RwInt32 *npFormat);
RwRaster *RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
RwRaster *RwRasterSetFromImage(RwRaster *raster, RwImage *image);
RwBool RwImageDestroy(RwImage *image);
RwTexture *RwTextureCreate(RwRaster *raster);
RwTexture *RwTextureSetName(RwTexture *texture, char const *name);
