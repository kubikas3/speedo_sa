#include "common.h"

uintptr_t FindPlayerVehicle(int playerId, bool includeRemote)
{
  return Invoke::Function<uintptr_t, int, bool>(g_pLibGTASA + 0x40B530 + 1, playerId, includeRemote);
}