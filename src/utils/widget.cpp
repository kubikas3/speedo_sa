#include "widget.h"
#include "../globals.h"

void GetWidgetRect(unsigned int widgetId, CRect *pWidgetRect)
{
    if (g_ppWidgets)
    {
        *pWidgetRect = *(CRect *)(g_ppWidgets[widgetId] + 0x20);
    }
}
