/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/
#pragma once

#include <stdint.h>
#include "util/Types.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class Font;

//----------------------------------------------------------------------------------------------------------------------

/**
  \class Canvas
  \brief The canvas base class

*/

class Canvas{
  
public:
  
  /**
   * @defgroup Canvas Canvas
   * @{
   */
  
//----------------------------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Types Public types
   * @ingroup Canvas
   * @{
   */
  
  //! The font size enum
  enum class tFont{
    DEFAULT, //!< No font size specified
    SMALL,   //!< 3x5 pixels font
    NORMAL,  //!< 5x7 pixels font
    BIG,     //!< 8x8 pixels font
  } ;
  
  //! The pixel rendering color
  enum class tColor{
    BLACK,  //!< Color is black
    WHITE,  //!< Color is white
    INVERT, //!< Invert the current color
    RANDOM, //!< Random color
    NONE,   //!< Transparent
  };
  
  //! The circle types
  enum class tCircle{
    FULL,                   //!< Full circle
    SEMI_LEFT,              //!< Semicircle (left half)
    SEMI_TOP,               //!< Semicircle (top half)
    SEMI_RIGHT,             //!< Semicircle (right half)
    SEMI_BOTTOM,            //!< Semicircle (bottom half)
    QUARTER_TOP_LEFT,       //!<  Quarter of circle (top left)
    QUARTER_TOP_RIGHT,      //!< Quarter of circle (top right)
    QUARTER_BOTTOM_RIGHT,   //!< Quarter of circle (bottom right)
    QUARTER_BOTTOM_LEFT,    //!< Quarter of circle (bottom left)
  };
  
  
  //! The allocation type
  enum class tAllocation{
    ROW_1BYTE_8_PIXELS,       //!< 1 bit per pixel, 1 byte packs a row of 8 pixels
    COL_1BYTE_8_PIXELS,       //!< 1 bit per pixel, 1 byte packs a column of 8 pixels
    ROW_2BYTES_3_PIXELS,      //!< 3 pixels each 2 bytes
  };
  
  /** @} */ // End of group Types
  
//----------------------------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup Canvas
   * @{
   */
  
  //! Constructor
  /*!
   \param width_  Canvas width in pixels
   \param height_ Canvas height in pixels
   \param height_ Number of data chunks (Default value is 8)
   */
  Canvas( uint16_t width_, uint16_t height_, tAllocation = tAllocation::ROW_1BYTE_8_PIXELS );
  
  //! Destructor
  virtual ~Canvas();
  
  /** @} */ // End of group Lifetime

//----------------------------------------------------------------------------------------------------------------------
  
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
  virtual void fillPattern( uint8_t value_ );
  
  /** @} */ // End of group Fill
  
//----------------------------------------------------------------------------------------------------------------------
  
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
  virtual void setPixel( uint16_t x_, uint16_t y_, tColor color_ );
  
  //! Get the pixel value
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual tColor getPixel(uint16_t x_, uint16_t y_ ) const;
  
  //! Draw a line
  /*!
   \param x0_              The X coordinate of the first point
   \param y0_              The Y coordinate of the first point
   \param x1_              The X coordinate of the second point
   \param y1_              The Y coordinate of the second point
   \param color_           The line color (white, black, invert, random)
   */
  virtual void drawLine( uint16_t x0_, uint16_t y0_, uint16_t x1_, uint16_t y1_, tColor color_ );
  
  //! Draw a bitmap
  /*!
   \param x_               The X coordinate of the upper-left corner
   \param y_               The Y coordinate of the upper-left corner
   \param w_               The width of the bitmap
   \param h_               The height of the bitmap
   \param pBitmap_         The bitmap to be printed
   \param color_           The color of the pixels
   */
  
  virtual void drawLineVertical( uint16_t x_, uint16_t y_, uint16_t h_, tColor color_ );
  virtual void drawLineHorizontal( uint16_t x_, uint16_t y_, uint16_t w_, tColor color_ );
  
  virtual void drawTriangle(
                            uint16_t x0_,
                            uint16_t y0_,
                            uint16_t x1_,
                            uint16_t y1_,
                            uint16_t x2_,
                            uint16_t y2_,
                            tColor color_
                            );
  virtual void drawFilledTriangle(
                                  uint16_t x0_,
                                  uint16_t y0_,
                                  uint16_t x1_,
                                  uint16_t y1_,
                                  uint16_t x2_,
                                  uint16_t y2_,
                                  tColor color_,
                                  tColor fillColor_
                                  );
  
  virtual void drawRect( uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, tColor color_ );
  virtual void drawFilledRect( uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, tColor color_, tColor fillColor_ );
  virtual void drawRectRounded(uint16_t x_, uint16_t y_, uint16_t w_, uint16_t h_, uint16_t r_, tColor color_);
  virtual void drawFilledRectRounded(
                                     uint16_t x_,
                                     uint16_t y_,
                                     uint16_t w_,
                                     uint16_t h_,
                                     uint16_t r_,
                                     tColor color_,
                                     tColor fillColor_
                                     );
  
  virtual void drawCircle( uint16_t rx, uint16_t ry, uint16_t r, tColor color_, tCircle = tCircle::FULL );
  
  virtual void drawFilledCircle(
                                uint16_t x_,
                                uint16_t y_,
                                uint16_t r_,
                                tColor color_,
                                tColor fillColor_,
                                tCircle = tCircle::FULL
                                );
  
  virtual void drawBitmap(
                          uint16_t x_,
                          uint16_t y_,
                          uint16_t w_,
                          uint16_t h_,
                          const uint8_t* pBitmap_,
                          tColor color_ = tColor::WHITE
                          );
  
  virtual void draw(
    const Canvas& c_,
    uint16_t xDest_,
    uint16_t yDest_,
    uint16_t xSource_ = 0,
    uint16_t ySource_ = 0,
    uint16_t w_ = 0,
    uint16_t h_ = 0
  );
  
  /** @} */ // End of group Primitives
  
//----------------------------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Text Text
   * @ingroup Drawing
   * @{
   */
  
  //! Print a single char
  /*!
   \param x_               The X coordinate
   \param y_               The Y coordinate
   \param c_               The char to be printed
   \param font_            The font (Default is the global font which can be set using setDefaultFont)
   \param color_           The color of the pixels
   */
  virtual void printChar(
                         uint16_t x_,
                         uint16_t y_,
                         char c_,
                         Font* pFont_,
                         tColor color_ = tColor::WHITE
                         );
  
  //! Print a string
  /*!
   \param x_               The X coordinate
   \param y_               The Y coordinate
   \param pStr_            The string to be printed
   \param font_            The font (Default is the global font which can be set using setDefaultFont)
   \param color_           The color of the pixels
   \param spacing_         Additional space between characters in pixels
   */
  virtual void printStr(
                        uint16_t x_,
                        uint16_t y_,
                        const char *pStr_,
                        tFont font_ = tFont::DEFAULT,
                        tColor color_ = tColor::WHITE,
                        uint8_t spacing_ = 0
                        );
  
  //! Print a string
  /*!
   \param x_               The X coordinate
   \param y_               The Y coordinate
   \param pStr_            The string to be printed
   \param pFont_           A pointer to a Font
   \param color_           The color of the pixels
   \param spacing_         Additional space between characters in pixels
   */
  virtual void printStr(
                        uint16_t x_,
                        uint16_t y_,
                        const char *pStr_,
                        Font* pFont_,
                        tColor color_ = tColor::WHITE,
                        uint8_t spacing_ = 0
                        );
  
  /**@}*/ // End of Text group
  
//----------------------------------------------------------------------------------------------------------------------
  
  /** @} */ // End of group Drawing
  
//----------------------------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Configuration Configuration of Canvas properties
   * @ingroup Canvas
   * @{
   */
  
//----------------------------------------------------------------------------------------------------------------------
  
  //! Set the default font size
  /*!
   \param font_  The font size to be set as default
   */
  virtual void setDefaultFont( tFont font_ );
  
  /** @} */ // End of group Configuration
  
//----------------------------------------------------------------------------------------------------------------------
  
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
  virtual const uint8_t* getPtr( uint16_t offset_ ) const;
  
  /** @} */ // End of group Access
  
//----------------------------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup Canvas
   * @{
   */
  
  //! Get the Canvas width in pixels
  /*!
   \return         The Canvas width in pixels
   */
  uint16_t getWidth() const noexcept { return m_width; }
  
  //! Get the Canvas height in pixels
  /*!
   \return         The Canvas height in pixels
   */
  uint16_t getHeight() const noexcept { return m_height; }
      
  //! Print a string representation of the Canvas data (space if value of the pixel is 0, '0' otherwise)
  virtual void toString();
  
  /** @} */ // End of group Utility
  
  /** @} */ // End of group Canvas
  
//----------------------------------------------------------------------------------------------------------------------

protected:

  tRawData& getData() { return m_data; }
  const tRawData& getData() const { return m_data; }
  uint16_t getCanvasWidthInBytes() const noexcept { return m_canvasWidthInBytes; }
  
private:

  tRawData                m_data;                                //!< The raw Canvas data
  uint16_t                m_canvasWidthInBytes;
  uint32_t                m_canvasSizeInBytes;                    //!< Frame size in bytes
  uint16_t                m_width;                                //!< Canvas width in pixels
  uint16_t                m_height;                               //!< Canvas height in pixels
  
  Font*                   m_pFont;                                //!< The current font
};


//----------------------------------------------------------------------------------------------------------------------

inline void Canvas::white()
{
  fillPattern( 0xFF );
}

//----------------------------------------------------------------------------------------------------------------------

inline void Canvas::black()
{
  fillPattern( 0x00 );
}

//----------------------------------------------------------------------------------------------------------------------

inline const uint8_t* Canvas::getPtr( uint16_t offset ) const
{
  return m_data.data() + offset;
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
