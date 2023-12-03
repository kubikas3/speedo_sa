#include "RenderWare.h"
#include "../globals.h"
#include "../utils/invoke.h"

RwImage *RtPNGImageRead(const RwChar *imageName)
{
	return Invoke::Function<RwImage *, const RwChar *>(g_pLibGTASA + 0x20A474 + 0x1, imageName);
}

RwImage *RwImageFindRasterFormat(RwImage *ipImage, RwInt32 nRasterType, RwInt32 *npWidth, RwInt32 *npHeight, RwInt32 *npDepth, RwInt32 *npFormat)
{
	return Invoke::Function<RwImage *, RwImage *, RwInt32, RwInt32 *, RwInt32 *, RwInt32 *, RwInt32 *>(g_pLibGTASA + 0x1DA51C + 0x1, ipImage, nRasterType, npWidth, npHeight, npDepth, npFormat);
}

RwRaster *RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags)
{
	return Invoke::Function<RwRaster *, RwInt32, RwInt32, RwInt32, RwInt32>(g_pLibGTASA + 0x1DAA50 + 0x1, width, height, depth, flags);
}

RwRaster *RwRasterSetFromImage(RwRaster *raster, RwImage *image)
{
	return Invoke::Function<RwRaster *, RwRaster *, RwImage *>(g_pLibGTASA + 0x1DA4F8 + 0x1, raster, image);
}

RwBool RwImageDestroy(RwImage *image)
{
	return Invoke::Function<RwBool, RwImage *>(g_pLibGTASA + 0x1D8EF8 + 0x1, image);
}

RwTexture *RwTextureCreate(RwRaster *raster)
{
	return Invoke::Function<RwTexture *, RwRaster *>(g_pLibGTASA + 0x1DB83C + 0x1, raster);
}

RwTexture *RwTextureSetName(RwTexture *texture, char const *name)
{
	return Invoke::Function<RwTexture *, RwTexture *, const char *>(g_pLibGTASA + 0x1DB8A0 + 0x1, texture, name);
}
