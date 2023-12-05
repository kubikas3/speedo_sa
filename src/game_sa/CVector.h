#pragma once

class CVector
{
public:
    float x, y, z;

    CVector();
    CVector(float X, float Y, float Z);

    float Magnitude();

    void operator=(const CVector &right);
};
