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

//--------------------------------------------------------------------------------------------------

namespace py
{
class CanvasHelper;
}

class Font;

//--------------------------------------------------------------------------------------------------

class LedMatrix : public Canvas
{

public:
  /**
   * @defgroup LedMatrix A N x M led matrix
   * @{
   */

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup LedMatrix
   * @{
   */

  //! Constructor
  /*!
     \param width_  Display width in pixels
     \param height_ Display height in pixels
     \param numDisplayChunks_ Number of data chunks (Default value is 8)
     */
  LedMatrix(uint16_t width_, uint16_t height_)
    : Canvas(width_, height_)
  {}

  //! Destructor
  virtual ~LedMatrix() = default;


  /** @} */ // End of group Lifetime

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup LedMatrix
   * @{
   */

  //! Set the dirty flag to true
  virtual void setDirty()
  {
    m_dirty = true;
  }

  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if the matrix is dirty, FALSE otherwise
  */
  virtual bool dirty() const
  {
    return m_dirty;
  }

  //! Reset the dirty flag
  virtual void resetDirty() const { m_dirty = false; }

  /** @} */ // End of group Access

  //--------------------------------------------------------------------------------------------------

  /** @} */ // End of group LedMatrix

  //--------------------------------------------------------------------------------------------------

private:
  friend class py::CanvasHelper;
  mutable bool m_dirty;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
