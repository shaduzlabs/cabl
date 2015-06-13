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

#include "gfx/GDisplay.h"

#include <map>

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class GDisplayMaschineMikro : public GDisplay
{
public:
 
    //! Constructor
    /*!
     \param width_  Display width in pixels
     \param height_ Display height in pixels
     \param height_ Number of data chunks (Default value is 8)
     */
    GDisplayMaschineMikro();

protected:

    //! Set a pixel
    /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \param color_           The pixel color (white, black, invert)
     \param bSetDirtyChunk_  If TRUE, the dirty flag for the pertaining chunk is set
     */
    void setPixelImpl( uint16_t x_, uint16_t y_, tColor color_, bool bSetDirtyChunk_ = true ) override;
    
    //! Get the pixel value
    /*!
     \param x_               The X coordinate of the pixel
     \param y_               The Y coordinate of the pixel
     \return                 The color of the selected pixel
     */
    tColor getPixelImpl( uint16_t x_, uint16_t y_ ) const override;

};
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
