/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>

#include "Canvas.h"

namespace sl
{
namespace cabl
{
class Font;

class GDisplay : public Canvas
{

public:
  /**
   * @defgroup GDisplay Graphic display
   * @{
   */

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup GDisplay
   * @{
   */

  //! Constructor
  /*!
     \param width_  Display width in pixels
     \param height_ Display height in pixels
     \param numDisplayChunks_ Number of data chunks (Default value is 8)
     */
  GDisplay(uint16_t width_, uint16_t height_, uint8_t numDisplayChunks_);

  //! Destructor
  virtual ~GDisplay() = default;


  /** @} */ // End of group Lifetime

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
  void setPixel(uint16_t x_, uint16_t y_, const util::ColorRGB& color_) override;

  //! Get the pixel value as an RGB color
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  util::ColorRGB pixel(uint16_t x_, uint16_t y_) const override;

  /** @} */ // End of group Primitives

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup GDisplay
   * @{
   */


  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if any of the display chunks is dirty, FALSE otherwise
     */
  virtual bool isDirty() const
  {
    return m_isDirty;
  }

  //! Is a specific display chunk dirty?
  /*!
     \param chunk_   The display chunk index
     \return         TRUE if the selected display chunk is dirty, FALSE otherwise
     */
  virtual bool isChunkDirty(uint8_t chunk_) const;

  //! Reset the global dirty flag and the chunk-specific dirty flags
  virtual void resetDirtyFlags() const;

  /** @} */ // End of group Access

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup GDisplay
   * @{
   */

  virtual uint8_t numberOfChunks() const noexcept
  {
    return m_numDisplayChunks;
  }

  /** @} */ // End of group Utility

  /** @} */ // End of group GDisplay

  //--------------------------------------------------------------------------------------------------

protected:
  void initialize() override;
  virtual void initializeImpl() = 0;

  uint16_t canvasWidthInBytes() const override;
  virtual uint16_t canvasWidthInBytesImpl() const = 0;

  //! Set a pixel (implementation)
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \param color_           The pixel color (RGB + Monochrome)
   */
  virtual void setPixelImpl(
    uint16_t x_, uint16_t y_, const util::ColorRGB& color_, bool bSetDirtyChunk_)
    = 0;

  //! Get the pixel value (implementation)
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual util::ColorRGB pixelImpl(uint16_t x_, uint16_t y_) const = 0;

  virtual void setDirtyChunks(uint16_t yStart_, uint16_t yEnd_ = 0xFFFF);

  mutable volatile bool m_isDirty; //!< Global 'dirty' flag

private:
  mutable tCollFlags m_pChunksDirtyFlags; //!< Chunk-specific dirty flags
  uint8_t m_numDisplayChunks;             //!< Number of display chunks
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
