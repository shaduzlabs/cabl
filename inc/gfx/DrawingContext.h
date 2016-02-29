/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <atomic>
#include <stdint.h>
#include "util/Types.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------
/**
  \class Canvas
  \brief The canvas base class

*/
class DrawingContext{

public:

  /**
   * @defgroup DrawingContext DrawingContext
   * @{
   */

//--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup DrawingContext
   * @{
   */

  //! Constructor
  /*!
   \param width_          Context width in pixels
   \param height_         Context height in pixels
   \param bytesPerPixel_  Number of bytes per pixel
   */

  DrawingContext(unsigned width_, unsigned height_, unsigned bytesPerPixel_)
    : m_width(width_)
    , m_height(height_)
    , m_bytesPerPixel(bytesPerPixel_)
    , m_data(m_width * m_height * m_bytesPerPixel, 0x00)
  {
  }

  //! Destructor
  virtual ~DrawingContext(){}

  /** @} */ // End of group Lifetime

//--------------------------------------------------------------------------------------------------

  unsigned getWidth() const
  {
    return m_width;
  }

  unsigned getHeight() const
  {
    return m_height;
  }

  unsigned getBytesPerPixel() const
  {
    return m_bytesPerPixel;
  }

  unsigned getSize() const
  {
    return m_width * m_height * m_bytesPerPixel;
  }

  bool isDirty() const
  {
    return m_dirty;
  }

  void setDirty(bool dirty_)
  {
    m_dirty = dirty_;
  }

  void clear()
  {
    std::fill( m_data.begin(), m_data.end(), 0x00 );
    m_dirty = true;
  }

  tRawData& getData() { return m_data; }

private:

  unsigned            m_width{0};
  unsigned            m_height{0};
  unsigned            m_bytesPerPixel{1};
  std::atomic<bool>   m_dirty{true};
  tRawData            m_data; //!< The raw data
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
