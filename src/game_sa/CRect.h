#pragma once

struct CRect
{
public:
    CRect();
    CRect(float left, float top, float right, float bottom);

    float left;   // x1
    float bottom; // y1
    float right;  // x2
    float top;    // y2
};
