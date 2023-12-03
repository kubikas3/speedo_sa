#include <mod/logger.h>
#include "texture.h"
#include "invoke.h"
#include "../globals.h"

uintptr_t GetTexture(const char *textureName)
{
	logger->Info("GetTexture: %s", textureName);

	// GetTexture
	uintptr_t pRwTexture = Invoke::Function<uintptr_t, const char *>(g_pLibGTASA + 0x1E9CE5, textureName);

	if (pRwTexture)
	{
		int iTextureCount = *(int *)(pRwTexture + 0x54);
		iTextureCount++;
		*(int *)(pRwTexture + 0x54) = iTextureCount;
	}

	return pRwTexture;
}

uintptr_t LoadTextureFromDB(const char *dbName, const char *textureName)
{
	// TextureDatabaseRuntime::GetDatabase
	uintptr_t dbHandle = Invoke::Function<uintptr_t, const char *>(g_pLibGTASA + 0x1EAC8C + 1, dbName);

	if (!dbHandle)
	{
		logger->Error("Error: Database not found! (%s)", dbName);
		return 0;
	}

	// TextureDatabaseRuntime::Register
	Invoke::Function<void, uintptr_t>(g_pLibGTASA + 0x1E9BC8 + 1, dbHandle);
	uintptr_t pRwTexture = GetTexture(textureName);

	if (!pRwTexture)
		logger->Error("Error: Texture (%s) not found in database (%s)", dbName, textureName);

	// TextureDatabaseRuntime::Unregister
	Invoke::Function<void, uintptr_t>(g_pLibGTASA + 0x1E9C80 + 1, dbHandle);

	return pRwTexture;
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
