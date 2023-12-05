#include "CRGBA.h"

CRGBA::CRGBA(unsigned char red, unsigned char green, unsigned char blue)
{
    Set(red, green, blue, 255);
}

CRGBA::CRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    Set(red, green, blue, alpha);
}

CRGBA::CRGBA(CRGBA const &rhs)
{
    Set(rhs);
}

CRGBA::CRGBA(unsigned int intValue)
{
    Set(intValue);
}

CRGBA::CRGBA() {}

void CRGBA::Set(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void CRGBA::Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    Set(red, green, blue);
    this->alpha = alpha;
}

void CRGBA::Set(unsigned int intValue)
{
    red = (intValue >> 24) & 0xFF;
    green = (intValue >> 16) & 0xFF;
    blue = (intValue >> 8) & 0xFF;
    alpha = intValue & 0xFF;
}

void CRGBA::Set(CRGBA const &rhs)
{
    Set(rhs.red, rhs.green, rhs.blue, rhs.alpha);
}

void CRGBA::Set(CRGBA const &rhs, unsigned char alpha)
{
    Set(rhs.red, rhs.green, rhs.blue, alpha);
}

unsigned int CRGBA::ToInt() const
{
    return alpha | (blue << 8) | (green << 16) | (red << 24);
}

unsigned int CRGBA::ToIntARGB() const
{
    return blue | (green << 8) | (red << 16) | (alpha << 24);
}

void CRGBA::FromARGB(unsigned int intValue)
{
    alpha = (intValue >> 24) & 0xFF;
    red = (intValue >> 16) & 0xFF;
    green = (intValue >> 8) & 0xFF;
    blue = intValue & 0xFF;
}

void CRGBA::Invert()
{
    Set(255 - red, 255 - green, 255 - blue);
}

CRGBA CRGBA::Inverted() const
{
    CRGBA invertedColor = *this;
    invertedColor.Invert();
    return invertedColor;
}

bool CRGBA::operator==(CRGBA const &rhs) const
{
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
}

CRGBA &CRGBA::operator=(CRGBA const &rhs)
{
    Set(rhs);
    return *this;
}

CRGBA CRGBA::ToRGB() const
{
    return CRGBA(red, green, blue, 255);
}
