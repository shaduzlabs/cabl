/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <algorithm>
#include <vector>

#include "Canvas.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/**
  \class CanvasBase
  \brief The canvas base class

*/

class DynamicCanvas : public Canvas
{

public:
  DynamicCanvas(unsigned w_, unsigned h_, unsigned size_ = 0, unsigned nChunks_ = 1)
    : m_width(w_)
    , m_height(h_)
    , m_size(size_ == 0 ? w_ * h_ * 3 : size_)
    , m_nChunks(nChunks_)
    , m_data(m_size)
    , m_chunkDirtyFlags(m_nChunks)
  {
    black();
  }
  unsigned width() const noexcept override
  {
    return m_width;
  }

  unsigned height() const noexcept override
  {
    return m_height;
  }

  unsigned canvasWidthInBytes() const noexcept override
  {
    return m_height > 0 ? m_size / m_height : 0;
  }

  unsigned numberOfChunks() const noexcept override
  {
    return m_nChunks;
  }

  const uint8_t* buffer() override
  {
    return m_data.data();
  }

  unsigned bufferSize() const override
  {
    return m_size;
  }

  const uint8_t* data() const override
  {
    return m_data.data();
  }

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup GDisplay
   * @{
   */

  //! Sets all of the dirty flags to true
  void setDirty() override
  {
    std::fill(m_chunkDirtyFlags.begin(), m_chunkDirtyFlags.end(), true);
  }

  //! Is any of the display chunks dirty?
  /*!
     \return  TRUE if any of the display chunks is dirty, FALSE otherwise
  */
  bool dirty() const override
  {
    return std::any_of(
      m_chunkDirtyFlags.begin(), m_chunkDirtyFlags.end(), [](bool b) { return b; });
  }

  //! Is a specific display chunk dirty?
  /*!
     \param chunk_   The display chunk index
     \return         TRUE if the selected display chunk is dirty, FALSE otherwise
     */
  bool dirtyChunk(unsigned chunk_) const override
  {
    return m_chunkDirtyFlags[chunk_];
  }

  void setDirtyChunk(unsigned yStart_) const override
  {
    unsigned chunkHeight = m_height / m_nChunks;
    if (chunkHeight == 0 || m_nChunks == 0)
    {
      return;
    }
    if (yStart_ < m_height)
    {
      unsigned chunk = std::min(static_cast<unsigned>(yStart_ / chunkHeight), m_nChunks - 1);
      m_chunkDirtyFlags[chunk] = true;
    }
  }

  //! Reset the global dirty flag and the chunk-specific dirty flags
  void resetDirtyFlags() const override
  {
    std::fill(m_chunkDirtyFlags.begin(), m_chunkDirtyFlags.end(), false);
  }

  /** @} */ // End of group Access

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup GDisplay
   * @{
   */


  /** @} */ // End of group Utility

protected:
  uint8_t* data() override
  {
    return m_data.data();
  }

private:
  friend class py::CanvasHelper;
  unsigned m_width;
  unsigned m_height;
  unsigned m_size;
  unsigned m_nChunks;

  std::vector<uint8_t> m_data;                 //!< The raw Canvas data
  mutable std::vector<bool> m_chunkDirtyFlags; //!< Chunk-specific dirty flags
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
