/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "cabl/gfx/CanvasBase.h"

#include <map>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class GDisplayMaschineMK2 : public CanvasBase<256, 64, 2048, 8>
{
public:
  //! Set a pixel
  /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \param color_           The pixel color (RGB + monochrome)
     \param bSetDirtyChunk_  If TRUE, the dirty flag for the pertaining chunk is set
     */
  void setPixel(
    unsigned x_, unsigned y_, const Color& color_, bool bSetDirtyChunk_ = true) override;

  //! Get the pixel value as an RGB color
  /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \return                 The color of the selected pixel
     */
  Color pixel(unsigned x_, unsigned y_) const override;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
