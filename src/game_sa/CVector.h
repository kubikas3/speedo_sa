#pragma once
#include "rw/rwcore.h"

class CVector : public RwV3d
{
public:
    CVector();

    float Magnitude();

    void operator=(const CVector &right);
};
