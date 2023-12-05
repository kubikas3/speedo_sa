#include "widget.h"

void GetWidgetScreenRect(uintptr_t pWidget, CRect *pWidgetRect)
{
    *pWidgetRect = *(CRect *)(pWidget + 0x2C);
}

void GetWidgetAlpha(uintptr_t pWidget, unsigned char *pAlpha)
{
    *pAlpha = *(unsigned char *)(pWidget + 0x58);
}
