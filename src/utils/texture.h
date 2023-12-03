#pragma once
#include "../game_sa/RenderWare.h"

uintptr_t GetTexture(const char *texture);
uintptr_t LoadTextureFromDB(const char *dbname, const char *texture);
RwTexture *LoadTextureFromPNG(const char *path, const char *name);
