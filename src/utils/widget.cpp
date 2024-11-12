#include "widget.h"

#ifdef AML32
const uintptr_t pWidgetRect = 0x20;
const uintptr_t pWidgetAlpha = 0x4C;
#else
const uintptr_t pWidgetRect = 0x2C;
const uintptr_t pWidgetAlpha = 0x58;
#endif

void GetWidgetScreenRect(uintptr_t pWidget, CRect *pRect)
{
    *pRect = *(CRect *)(pWidget + pWidgetRect);
}

void GetWidgetAlpha(uintptr_t pWidget, unsigned char *pAlpha)
{
    *pAlpha = *(unsigned char *)(pWidget + pWidgetAlpha);
}
