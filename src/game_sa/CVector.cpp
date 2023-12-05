#include "CVector.h"
#include <cmath>

CVector::CVector() {}

float CVector::Magnitude()
{
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

void CVector::operator=(const CVector &right)
{
    this->x = right.x;
    this->y = right.y;
    this->z = right.z;
}
