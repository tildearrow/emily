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

#ifndef SFML_FLOATCOLOR_HPP
#define SFML_FLOATCOLOR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Color.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating floating point RGBA colors
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API FloatColor
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an opaque black color. It is equivalent to
    /// sf::FloatColor(0, 0, 0, 1).
    ///
    ////////////////////////////////////////////////////////////
    FloatColor();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from its 4 RGBA components
    ///
    /// \param red   Red component (in the range [0, 1])
    /// \param green Green component (in the range [0, 1])
    /// \param blue  Blue component (in the range [0, 1])
    /// \param alpha Alpha (opacity) component (in the range [0, 1])
    ///
    ////////////////////////////////////////////////////////////
    FloatColor(float red, float green, float blue, float alpha = 1);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from 128-bit floating point array
    ///
    /// \param color Number containing the RGBA components (in that order)
    ///
    ////////////////////////////////////////////////////////////
    FloatColor(float* color);
    
////////////////////////////////////////////////////////////
    /// \brief Construct the color from a non-floating-point color
    ///
    /// \param color Integer color
    ///
    ////////////////////////////////////////////////////////////
    FloatColor(Color color);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the color as a 128-bit float array
    ///
    /// \return Color represented as a 128-bit float array
    ///
    ////////////////////////////////////////////////////////////
    float* toFloat() const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const FloatColor Black;       ///< Black predefined color
    static const FloatColor White;       ///< White predefined color
    static const FloatColor Red;         ///< Red predefined color
    static const FloatColor Green;       ///< Green predefined color
    static const FloatColor Blue;        ///< Blue predefined color
    static const FloatColor Yellow;      ///< Yellow predefined color
    static const FloatColor Magenta;     ///< Magenta predefined color
    static const FloatColor Cyan;        ///< Cyan predefined color
    static const FloatColor Transparent; ///< Transparent (black) predefined color

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    mutable float r; ///< Red component
    float g; ///< Green component
    float b; ///< Blue component
    float a; ///< Alpha (opacity) component
};

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the == operator
///
/// This operator compares two colors and check if they are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if colors are equal, false if they are different
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator ==(const FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the != operator
///
/// This operator compares two colors and check if they are different.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if colors are different, false if they are equal
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator !=(const FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary + operator
///
/// This operator returns the component-wise sum of two colors.
/// Components that exceed 255 are clamped to 255.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left + \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor operator +(const FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary - operator
///
/// This operator returns the component-wise subtraction of two colors.
/// Components below 0 are clamped to 0.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left - \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor operator -(const FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary * operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left * \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor operator *(const FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary += operator
///
/// This operator computes the component-wise sum of two colors,
/// and assigns the result to the left operand.
/// Components that exceed 255 are clamped to 255.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor& operator +=(FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary -= operator
///
/// This operator computes the component-wise subtraction of two colors,
/// and assigns the result to the left operand.
/// Components below 0 are clamped to 0.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor& operator -=(FloatColor& left, const FloatColor& right);

////////////////////////////////////////////////////////////
/// \relates FloatColor
/// \brief Overload of the binary *= operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors, and assigns
/// the result to the left operand.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API FloatColor& operator *=(FloatColor& left, const FloatColor& right);

} // namespace sf


#endif // SFML_FLOATCOLOR_HPP


////////////////////////////////////////////////////////////
/// \class sf::FloatColor
/// \ingroup graphics
///
/// sf::FloatColor is a floating point version of sf::Color.
///
/// Each component is a public member, a floating point number in
/// the range [0, 1]. Thus, colors can be constructed and
/// manipulated very easily:
///
/// \code
/// sf::FloatColor color(1, 0, 0); // red
/// color.r = 0;                // make it black
/// color.b = 0.5;              // make it dark blue
/// \endcode
///
/// The fourth component of colors, named "alpha", represents
/// the opacity of the color. A color with an alpha value of
/// 1 will be fully opaque, while an alpha value of 0 will
/// make a color fully transparent, whatever the value of the
/// other components is.
///
/// The most common colors are already defined as static variables:
/// \code
/// sf::FloatColor black       = sf::FloatColor::Black;
/// sf::FloatColor white       = sf::FloatColor::White;
/// sf::FloatColor red         = sf::FloatColor::Red;
/// sf::FloatColor green       = sf::FloatColor::Green;
/// sf::FloatColor blue        = sf::FloatColor::Blue;
/// sf::FloatColor yellow      = sf::FloatColor::Yellow;
/// sf::FloatColor magenta     = sf::FloatColor::Magenta;
/// sf::FloatColor cyan        = sf::FloatColor::Cyan;
/// sf::FloatColor transparent = sf::FloatColor::Transparent;
/// \endcode
///
/// Colors can also be added and modulated (multiplied) using the
/// overloaded operators + and *.
///
/// FloatColors can also be converted to normal colors using the
/// overloaded operator =.
///
////////////////////////////////////////////////////////////
