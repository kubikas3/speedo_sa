#include "widget.h"
#include <cstdint>

void GetWidgetScreenRect(uintptr_t pWidget, CRect *pWidgetRect)
{
    *pWidgetRect = *(CRect *)(pWidget + 0x20);
}

void GetWidgetAlpha(uintptr_t pWidget, unsigned char *pAlpha)
{
    *pAlpha = *(unsigned char *)(pWidget + 0x4C);
}
