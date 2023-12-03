#include "widget.h"
#include "../globals.h"

void GetWidgetRect(unsigned int widgetId, CRect *pWidgetRect)
{
    if (g_ppWidgets)
    {
        *pWidgetRect = *(CRect *)(g_ppWidgets[widgetId] + 0x20);
    }
}

void GetWidgetAlpha(unsigned int widgetId, unsigned char *pAlpha)
{
    if (g_ppWidgets)
    {
        *pAlpha = *(unsigned char *)(g_ppWidgets[widgetId] + 0x4C);
    }
}
