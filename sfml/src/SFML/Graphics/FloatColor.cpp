////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/FloatColor.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
const FloatColor FloatColor::Black(0, 0, 0);
const FloatColor FloatColor::White(1, 1, 1);
const FloatColor FloatColor::Red(1, 0, 0);
const FloatColor FloatColor::Green(0, 1, 0);
const FloatColor FloatColor::Blue(0, 0, 1);
const FloatColor FloatColor::Yellow(1, 1, 0);
const FloatColor FloatColor::Magenta(1, 0, 1);
const FloatColor FloatColor::Cyan(0, 1, 1);
const FloatColor FloatColor::Transparent(0, 0, 0, 0);


////////////////////////////////////////////////////////////
FloatColor::FloatColor() :
r(0),
g(0),
b(0),
a(1)
{

}


////////////////////////////////////////////////////////////
FloatColor::FloatColor(float red, float green, float blue, float alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{

}


////////////////////////////////////////////////////////////
FloatColor::FloatColor(float* color) :
r(color[0]),
g(color[1]),
b(color[2]),
a(color[3])
{

}


////////////////////////////////////////////////////////////
FloatColor::FloatColor(Color color) :
r(float(color.r)/255.0f),
g(float(color.g)/255.0f),
b(float(color.b)/255.0f),
a(float(color.a)/255.0f)
{

}


////////////////////////////////////////////////////////////
float* FloatColor::toFloat() const
{
    return &r;
}


////////////////////////////////////////////////////////////
bool operator ==(const FloatColor& left, const FloatColor& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}


////////////////////////////////////////////////////////////
bool operator !=(const FloatColor& left, const FloatColor& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
FloatColor operator +(const FloatColor& left, const FloatColor& right)
{
    return FloatColor(left.r + right.r,
                 left.g + right.g,
                 left.b + right.b,
                 left.a + right.a);
}


////////////////////////////////////////////////////////////
FloatColor operator -(const FloatColor& left, const FloatColor& right)
{
    return FloatColor(left.r - right.r,
                 left.g - right.g,
                 left.b - right.b,
                 left.a - right.a);
}


////////////////////////////////////////////////////////////
FloatColor operator *(const FloatColor& left, const FloatColor& right)
{
    return FloatColor(left.r * right.r,
                 left.g * right.g,
                 left.b * right.b,
                 left.a * right.a);
}


////////////////////////////////////////////////////////////
FloatColor& operator +=(FloatColor& left, const FloatColor& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
FloatColor& operator -=(FloatColor& left, const FloatColor& right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
FloatColor& operator *=(FloatColor& left, const FloatColor& right)
{
    return left = left * right;
}

} // namespace sf
