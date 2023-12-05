#include "CRect.h"

CRect::CRect()
{
}

CRect::CRect(float left, float top, float right, float bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}
