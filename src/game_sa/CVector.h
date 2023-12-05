#pragma once
#include "rw/rwcore.h"

class CVector : public RwV3d
{
public:
    CVector();
    CVector(float X, float Y, float Z);

    float Magnitude();

    void operator=(const CVector &right);
};
