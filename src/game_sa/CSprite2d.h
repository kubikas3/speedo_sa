#pragma once
#include "CRect.h"
#include "CRGBA.h"

class CSprite2d
{
public:
    struct RwTexture *m_pRwTexture;

    void Draw(float x, float y, float width, float height, CRGBA const &color);
    void Draw(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, CRGBA const &color);
    void Draw(float x, float y, float width, float height, float angle, float cx, float cy, CRGBA const &color);
    void Delete();

    static void DrawRect(CRect const &rect, CRGBA const &color);

    ~CSprite2d();
};
