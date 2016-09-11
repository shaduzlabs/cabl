/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <algorithm>
#include <cstdint>

#include "Canvas.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

class Font;

//--------------------------------------------------------------------------------------------------

class LedArray
{

public:

  /**
   * @defgroup LedArray an array of L leds
   * @{
   */

//--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup LedArray
   * @{
   */


  //! Destructor
  virtual ~LedArray() = default;

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
  virtual void setPixel(uint16_t pos_, const util::ColorRGB& color_) = 0;

  //! Get the pixel value as an RGB color
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual util::ColorRGB pixel(uint16_t pos_) const = 0;

  /** @} */ // End of group Lifetime

//--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup LedArray
   * @{
   */

  //! \return the length of the led array
  virtual uint16_t length() const = 0;

  //! Set the dirty flag to true
  virtual void setDirty() = 0;

  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if the matrix is dirty, FALSE otherwise
  */
  virtual bool dirty() const = 0;

  //! Reset the dirty flag
  virtual void resetDirty() const = 0;

  /** @} */ // End of group Access

//--------------------------------------------------------------------------------------------------

  /** @} */ // End of group LedArray

};

//--------------------------------------------------------------------------------------------------

template<unsigned L, unsigned SIZE = L>
class LedArrayBase : public LedArray
{

public:

  /**
   * @defgroup LedArray an array of L leds
   * @{
   */

//--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup LedArray
   * @{
   */


  //! Destructor
  virtual ~LedArrayBase() = default;

  /**
   * @defgroup Primitives Drawing primitives
   * @ingroup Drawing
   * @{
   */


  /** @} */ // End of group Lifetime

//--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup LedArray
   * @{
   */

  //! \return the length of the led array
  uint16_t length() const override
  {
    return L;
  }

  //! Set the dirty flag to true
  void setDirty() override
  {
    m_dirty = true;
  }

  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if the matrix is dirty, FALSE otherwise
  */
  bool dirty() const override
  {
    return m_dirty;
  }

  //! Reset the dirty flag
  void resetDirty() const override
  {
    m_dirty = false;
  }

  /** @} */ // End of group Access

//--------------------------------------------------------------------------------------------------

  /** @} */ // End of group LedArray

//--------------------------------------------------------------------------------------------------
  const uint8_t* buffer() { return m_data.data(); };

protected:
  const uint8_t* data() const { return m_data.data(); };
  uint8_t* data() { return m_data.data(); };

private:

  mutable bool m_dirty;
  std::array<uint8_t, SIZE> m_data;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
