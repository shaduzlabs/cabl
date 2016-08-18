/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "util/Types.h"
#include <stdint.h>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

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

  //! The pixel rendering color
  enum class Color
  {
    Black,  //!< Color is black
    White,  //!< Color is white
    Invert, //!< Invert the current color
    Random, //!< Random color
    None,   //!< Transparent
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


  //! The allocation type
  enum class Allocation
  {
    None,

    // B&W
    OneBytePacksOneRowOfEightPixels, //!< 1 bit per pixel, 1 byte packs a row of 8 pixels
    OneBytePacksOneColOfEightPixels, //!< 1 bit per pixel, 1 byte packs a column of 8 pixels
    TwoBytesPackThreePixelsInARow,   //!< 3 pixels each 2 bytes

    // Color
    RGB565, //!< 2 bytes per pixel (RGB565)
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
  Canvas(uint16_t width_, uint16_t height_, Allocation alloc_);

  //! Destructor
  virtual ~Canvas();

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
   \param color_           The pixel color (white, black, invert...)
   */
  virtual void setPixel(uint16_t x_, uint16_t y_, Color color_);

  //! Get the pixel value
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual Color pixel(uint16_t x_, uint16_t y_) const;

  //! Draw a line
  /*!
   \param x0_              The X coordinate of the first point
   \param y0_              The Y coordinate of the first point
   \param x1_              The X coordinate of the second point
   \param y1_              The Y coordinate of the second point
   \param color_           The line color (white, black, invert, random)
   */
  virtual void drawLine(uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, Color color_);

  //! Draw a bitmap
  /*!
   \param x_               The X coordinate of the upper-left corner
   \param y_               The Y coordinate of the upper-left corner
   \param w_               The width of the bitmap
   \param h_               The height of the bitmap
   \param pBitmap_         The bitmap to be printed
   \param color_           The color of the pixels
   */

  virtual void drawLineVertical(uint16_t x_, uint16_t y_, uint16_t h_, Color color_);
  virtual void drawLineHorizontal(uint16_t x_, uint16_t y_, uint16_t w_, Color color_);

  virtual void drawTriangle(uint16_t x0_,
    uint16_t y0_,
    uint16_t x1_,
    uint16_t y1_,
    uint16_t x2_,
    uint16_t y2_,
    Color color_);
  virtual void drawFilledTriangle(uint16_t x0_,
    uint16_t y0_,
    uint16_t x1_,
    uint16_t y1_,
    uint16_t x2_,
    uint16_t y2_,
    Color color_,
    Color fillColor_);

  virtual void drawRect(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, Color color_);

  virtual void drawFilledRect(
    uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, Color color_, Color fillColor_);

  virtual void drawRectRounded(
    uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, uint16_t r_, Color color_);

  virtual void drawFilledRectRounded(
    uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, uint16_t r_, Color color_, Color fillColor_);

  virtual void drawCircle(
    uint16_t rx_, uint16_t ry_, uint16_t r_, Color color_, CircleType = CircleType::Full);

  virtual void drawFilledCircle(uint16_t x_,
    uint16_t y_,
    uint16_t r_,
    Color color_,
    Color fillColor_,
    CircleType = CircleType::Full);

  virtual void drawBitmap(uint16_t x_,
    uint16_t y_,
    uint16_t w_,
    uint16_t h_,
    const uint8_t* pBitmap_,
    Color color_ = Color::White);

  virtual void draw(const Canvas& c_,
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
   \param font_          The font (Default is the global font which can be set using setDefaultFont)
   \param color_         The color of the pixels
   */
  virtual void printChar(
    uint16_t x_, uint16_t y_, char c_, Font* pFont_, Color color_ = Color::White);

  //! Print a string
  /*!
   \param x_             The X coordinate
   \param y_             The Y coordinate
   \param pStr_          The string to be printed
   \param font_          The font (Default is the global font which can be set using setDefaultFont)
   \param color_         The color of the pixels
   \param spacing_       Additional space between characters in pixels
   */
  virtual void printStr(uint16_t x_,
    uint16_t y_,
    const char* pStr_,
    FontType font_ = FontType::Default,
    Color color_ = Color::White,
    uint8_t spacing_ = 0);

  //! Print a string
  /*!
   \param x_               The X coordinate
   \param y_               The Y coordinate
   \param pStr_            The string to be printed
   \param pFont_           A pointer to a Font
   \param color_           The color of the pixels
   \param spacing_         Additional space between characters in pixels
   */
  virtual void printStr(uint16_t x_,
    uint16_t y_,
    const char* pStr_,
    Font* pFont_,
    Color color_ = Color::White,
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

  //! Set the default font size
  /*!
   \param font_  The font size to be set as default
   */
  virtual void setDefaultFont(FontType font_);

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

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup Canvas
   * @{
   */

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

  /** @} */ // End of group Utility

  /** @} */ // End of group Canvas

  //--------------------------------------------------------------------------------------------------

protected:
  tRawData& data()
  {
    return m_data;
  }
  const tRawData& data() const
  {
    return m_data;
  }
  uint16_t canvasWidthInBytes() const noexcept
  {
    return m_canvasWidthInBytes;
  }

private:
  tRawData m_data; //!< The raw Canvas data
  uint16_t m_canvasWidthInBytes;
  uint32_t m_canvasSizeInBytes; //!< Frame size in bytes
  uint16_t m_width;             //!< Canvas width in pixels
  uint16_t m_height;            //!< Canvas height in pixels

  Font* m_pFont; //!< The current font
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

} // cabl
} // sl
