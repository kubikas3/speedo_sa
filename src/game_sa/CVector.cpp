#include "CVector.h"

CVector::CVector() {}

float CVector::Magnitude()
{
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}
