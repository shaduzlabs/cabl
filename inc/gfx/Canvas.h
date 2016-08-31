/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>
#include <string>

#include "util/ColorRGB.h"
#include "util/Types.h"

#include "gfx/FontManager.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

namespace test
{
class CanvasTestHelper;
}

class Font;

//--------------------------------------------------------------------------------------------------

/**
  \class Canvas
  \brief The canvas base class

*/

class Canvas
{

public:
  /**
   * @defgroup Canvas Canvas
   * @{
   */

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Types Public types
   * @ingroup Canvas
   * @{
   */

  //! The font size enum
  enum class FontType
  {
    Default, //!< No font size specified
    Small,   //!< 3x5 pixels font
    Normal,  //!< 5x7 pixels font
    Big,     //!< 8x8 pixels font
  };

  //! The circle types
  enum class CircleType
  {
    Full,               //!< Full circle
    SemiLeft,           //!< Semicircle (left half)
    SemiTop,            //!< Semicircle (top half)
    SemiRight,          //!< Semicircle (right half)
    SemiBottom,         //!< Semicircle (bottom half)
    QuarterTopLeft,     //!<  Quarter of circle (top left)
    QuarterTopRight,    //!< Quarter of circle (top right)
    QuarterBottomRight, //!< Quarter of circle (bottom right)
    QuarterBottomLeft,  //!< Quarter of circle (bottom left)
  };

  /** @} */ // End of group Types

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup Canvas
   * @{
   */

  //! Constructor
  /*!
   \param width_  Canvas width in pixels
   \param height_ Canvas height in pixels
   \param alloc_  Number of data chunks (Default value is 8)
   */
  Canvas(uint16_t width_, uint16_t height_);

  //! Destructor
  virtual ~Canvas() = default;

  //! Get the Canvas width in pixels
  /*!
   \return         The Canvas width in pixels
   */
  uint16_t width() const noexcept
  {
    return m_width;
  }

  //! Get the Canvas height in pixels
  /*!
   \return         The Canvas height in pixels
   */
  uint16_t height() const noexcept
  {
    return m_height;
  }

  /** @} */ // End of group Lifetime

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Fill Fill functions
   * @ingroup Drawing
   * @{
   */

  //! Fill the Canvas with white pixels
  virtual void white();

  //! Fill the Canvas with black pixels
  virtual void black();

  //! Invert the content of the Canvas
  virtual void invert();

  //! Fill the Canvas with a specific pattern
  /*!
   \param value_  The value of a pattern of eight pixels (8x1 pixels)
   */
  virtual void fillPattern(uint8_t value_);

  /** @} */ // End of group Fill

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Primitives Drawing primitives
   * @ingroup Drawing
   * @{
   */

  //! Set a pixel
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \param color_           The pixel color (RGB + Monochrome)
   */
  virtual void setPixel(uint16_t x_, uint16_t y_, const util::ColorRGB& color_);

  //! Get the pixel value as an RGB color
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual util::ColorRGB pixel(uint16_t x_, uint16_t y_) const;

  //! Draw a line
  /*!
   \param x0_              The X coordinate of the first point
   \param y0_              The Y coordinate of the first point
   \param x1_              The X coordinate of the second point
   \param y1_              The Y coordinate of the second point
   \param color_           The line color (white, black, invert, random)
   */
  virtual void line(
    uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, const util::ColorRGB& color_);

  //! Draw a bitmap
  /*!
   \param x_               The X coordinate of the upper-left corner
   \param y_               The Y coordinate of the upper-left corner
   \param w_               The width of the bitmap
   \param h_               The height of the bitmap
   \param pBitmap_         The bitmap to be printed
   \param color_           The color of the pixels
   */

  virtual void lineVertical(
    uint16_t x_, uint16_t y_, uint16_t h_, const util::ColorRGB& color_);
  virtual void lineHorizontal(
    uint16_t x_, uint16_t y_, uint16_t w_, const util::ColorRGB& color_);

  virtual void triangle(uint16_t x0_,
    uint16_t y0_,
    uint16_t x1_,
    uint16_t y1_,
    uint16_t x2_,
    uint16_t y2_,
    const util::ColorRGB& color_);
  virtual void triangleFilled(uint16_t x0_,
    uint16_t y0_,
    uint16_t x1_,
    uint16_t y1_,
    uint16_t x2_,
    uint16_t y2_,
    const util::ColorRGB& color_,
    const util::ColorRGB& fillColor_);

  virtual void rectangle(
    uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, const util::ColorRGB& color_);

  virtual void rectangleFilled(uint16_t x_,
    uint16_t y_,
    uint16_t w_,
    uint16_t h_,
    const util::ColorRGB& color_,
    const util::ColorRGB& fillColor_);

  virtual void rectangleRounded(
    uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, uint16_t r_, const util::ColorRGB& color_);

  virtual void rectangleRoundedFilled(uint16_t x_,
    uint16_t y_,
    uint16_t w_,
    uint16_t h_,
    uint16_t r_,
    const util::ColorRGB& color_,
    const util::ColorRGB& fillColor_);

  virtual void circle(uint16_t rx_,
    uint16_t ry_,
    uint16_t r_,
    const util::ColorRGB& color_,
    CircleType = CircleType::Full);

  virtual void circleFilled(uint16_t x_,
    uint16_t y_,
    uint16_t r_,
    const util::ColorRGB& color_,
    const util::ColorRGB& fillColor_,
    CircleType = CircleType::Full);

  virtual void bitmap(uint16_t x_,
    uint16_t y_,
    uint16_t w_,
    uint16_t h_,
    const uint8_t* pBitmap_,
    const util::ColorRGB& color_);

  virtual void canvas(const Canvas& c_,
    uint16_t xDest_,
    uint16_t yDest_,
    uint16_t xSource_ = 0,
    uint16_t ySource_ = 0,
    uint16_t w_ = 0,
    uint16_t h_ = 0);

  /** @} */ // End of group Primitives

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Text Text
   * @ingroup Drawing
   * @{
   */

  //! Print a single char
  /*!
   \param x_             The X coordinate
   \param y_             The Y coordinate
   \param c_             The char to be printed
   \param font_          The font
   \param color_         The color of the pixels
   */
  virtual void character(
    uint16_t x_, uint16_t y_, char c_, const util::ColorRGB& color_, const std::string& font_ = "");

  //! Print a string
  /*!
   \param x_             The X coordinate
   \param y_             The Y coordinate
   \param pStr_          The string to be printed
   \param font_          The font
   \param color_         The color of the pixels
   \param spacing_       Additional space between characters in pixels
   */
  virtual void text(uint16_t x_,
    uint16_t y_,
    const char* pStr_,
    const util::ColorRGB& color_,
    const std::string& font_ = "",
    uint8_t spacing_ = 0);

  /**@}*/ // End of Text group

  //--------------------------------------------------------------------------------------------------

  /** @} */ // End of group Drawing

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Configuration Configuration of Canvas properties
   * @ingroup Canvas
   * @{
   */

  //--------------------------------------------------------------------------------------------------

  /** @} */ // End of group Configuration

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup Canvas
   * @{
   */

  //! Get a pointer to the specified offset of the Canvas data
  /*!
   \param offset_  Offset of the pointer starting from the first byte of Canvas data
   \return         A pointer to Canvas data (at the specified offset)
   */
  virtual const uint8_t* ptr(uint16_t offset_) const;

  /** @} */ // End of group Access

  /** @} */ // End of group Canvas

  //--------------------------------------------------------------------------------------------------

  const tRawData& data() const
  {
    return m_data;
  }

protected:
  virtual void initialize();

  virtual uint16_t canvasWidthInBytes() const;

  tRawData& data()
  {
    return m_data;
  }

private:
  friend class test::CanvasTestHelper;

  tRawData m_data; //!< The raw Canvas data

  uint16_t m_width;  //!< Canvas width in pixels
  uint16_t m_height; //!< Canvas height in pixels

  const Font* m_pFont; //!< The current font
};


//--------------------------------------------------------------------------------------------------

inline void Canvas::white()
{
  fillPattern(0xFF);
}

//--------------------------------------------------------------------------------------------------

inline void Canvas::black()
{
  fillPattern(0x00);
}

//--------------------------------------------------------------------------------------------------

inline const uint8_t* Canvas::ptr(uint16_t offset) const
{
  return m_data.data() + offset;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
