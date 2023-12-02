#include <mod/logger.h>
#include "../globals.h"
#include "../utils/invoke.h"

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
