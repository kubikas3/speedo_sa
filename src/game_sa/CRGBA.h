#pragma once
#include "rw/rwcore.h"

class CRGBA : public RwRGBA
{
public:
    CRGBA(unsigned char red, unsigned char green, unsigned char blue);
    CRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    CRGBA(CRGBA const &rhs);
    CRGBA(unsigned int intValue);
    CRGBA();

    void Set(unsigned char red, unsigned char green, unsigned char blue);
    void Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    void Set(unsigned int intValue);
    void Set(CRGBA const &rhs);
    void Set(CRGBA const &rhs, unsigned char alpha);

    CRGBA ToRGB() const;
    unsigned int ToInt() const;
    unsigned int ToIntARGB() const;

    void FromARGB(unsigned int intValue);

    void Invert();
    CRGBA Inverted() const;

    bool operator==(CRGBA const &rhs) const;
    CRGBA &operator=(CRGBA const &rhs);
};
