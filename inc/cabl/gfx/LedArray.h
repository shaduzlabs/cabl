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
  virtual void setPixel(unsigned pos_, const Color& color_) = 0;

  //! Get the pixel value as an RGB color
  /*!
   \param x_               The X coordinate of the pixel
   \param y_               The Y coordinate of the pixel
   \return                 The color of the selected pixel
   */
  virtual Color pixel(unsigned pos_) const = 0;

  virtual void setValue(double val_, Color color_, Alignment alignment_ = Alignment::Left) = 0;
  
  virtual void clear() = 0;
  
  /** @} */ // End of group Lifetime

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup LedArray
   * @{
   */

  //! \return the length of the led array
  virtual unsigned length() const = 0;

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

template <unsigned L, unsigned SIZE = L>
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

  void setValue(double val_, Color color_, Alignment alignment_) override
  {
    double val = std::min(val_, 1.0);
    clear();
    switch(alignment_)
    {
      case Alignment::Left:
      {
        val = std::max(val_, 0.0);
        unsigned nLedsOn = static_cast<unsigned>((val * SIZE) + 0.5);
        for(unsigned i = 0; i < nLedsOn; i++)
        {
          setPixel(i,color_);
        }
        break;
      }
      case Alignment::Center:
      {
        val = (std::max(val_, -1.0)) / 2.0;
		int nLedsOn = static_cast<int>((val * SIZE) + 0.5);
        if(val < 0)
        {
          for(int i = nLedsOn; i >= 0; i--)
          {
            setPixel(i,color_);
          }
        }
        else if (val > 0)
        {
          for(int i = nLedsOn; i < SIZE; i++)
          {
            setPixel(i,color_);
          }
        }
        setPixel(SIZE/2, color_);
        break;
      }
      case Alignment::Right:
      {
        val = std::max(val_, 0.0);
		int nLedsOn = static_cast<int>((val * SIZE) + 0.5);
        for(int i = SIZE; i >= nLedsOn; i--)
        {
          setPixel(i,color_);
        }
        break;
      }
    }
    setDirty();
  }
  
  //! turn off all leds
  void clear() override
  {
    std::fill(m_data.begin(), m_data.end(), 0);
  }
  
  //! \return the length of the led array
  unsigned length() const override
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
  const uint8_t* buffer()
  {
    return m_data.data();
  };

protected:
  const uint8_t* data() const
  {
    return m_data.data();
  };
  uint8_t* data()
  {
    return m_data.data();
  };

private:
  mutable bool m_dirty;
  std::array<uint8_t, SIZE> m_data{};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
