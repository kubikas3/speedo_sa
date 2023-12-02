#include <cmath>
#include "../globals.h"
#include "../utils/invoke.h"
#include "CSprite2d.h"

void CSprite2d::Draw(float x, float y, float width, float height, CRGBA const &color)
{
    Invoke::Method<void, CSprite2d *, float, float, float, float, CRGBA const &>(g_pLibGTASA + 0x5C8ED0 + 1, this, x, y, width, height, color);
}

void CSprite2d::Draw(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, CRGBA const &color)
{
    Invoke::Method<void, CSprite2d *, float, float, float, float, float, float, float, float, CRGBA const &>(g_pLibGTASA + 0x5C9BC8 + 1, this, x1, y1, x2, y2, x3, y3, x4, y4, color);
}

void CSprite2d::Draw(float x, float y, float width, float height, float angle, float cx, float cy, CRGBA const &color)
{
    float _cx = x + width * cx,
          _cy = y + height * cx;

    struct CPoint
    {
        float x;
        float y;
    };

    CPoint quad[4] = {
        {x + width, y + height},
        {x, y + height},
        {x, y},
        {x + width, y}};

    if (angle != 0.0f)
    {
        float c = cosf(angle);
        float s = sinf(angle);

        for (int i = 0; i < 4; i++)
        {
            float dx = quad[i].x - _cx;
            float dy = quad[i].y - _cy;
            quad[i] = {dx * c - dy * s + _cx, dx * s + dy * c + _cy};
        }
    }

    this->Draw(quad[1].x, quad[1].y, quad[0].x, quad[0].y, quad[2].x, quad[2].y, quad[3].x, quad[3].y, color);
}

void CSprite2d::Delete()
{
    Invoke::Method<CSprite2d *>(g_pLibGTASA + 0x5C881C + 1, this);
}

void CSprite2d::DrawRect(CRect const &rect, CRGBA const &color)
{
    Invoke::Function<void, CRect const &, CRGBA const &>(g_pLibGTASA + 0x5CA1E4 + 1, rect, color);
}

CSprite2d::~CSprite2d()
{
    Invoke::Method<void, CSprite2d *>(g_pLibGTASA + 0x5C8806 + 1, this);
}
