/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "gfx/GDisplay.h"

#include <map>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class GDisplayMaschineMK1 : public GDisplay
{
public:
  //! Constructor
  /*!
     \param width_  Display width in pixels
     \param height_ Display height in pixels
     \param height_ Number of data chunks (Default value is 8)
     */
  GDisplayMaschineMK1();

  void white() override;

  void black() override;

protected:
  //! Set a pixel
  /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \param color_           The pixel color (white, black, invert)
     \param bSetDirtyChunk_  If TRUE, the dirty flag for the pertaining chunk is set
     */
  void setPixelImpl(uint16_t x_, uint16_t y_, Color color_, bool bSetDirtyChunk_ = true) override;

  //! Get the pixel value
  /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \return                 The color of the selected pixel
     */
  Color pixelImpl(uint16_t x_, uint16_t y_) const override;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
