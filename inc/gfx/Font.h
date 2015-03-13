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

#ifndef SL_FONT_H
#define SL_FONT_H

namespace sl
{

class Font
{

public:


  virtual uint8_t  getWidth()         const noexcept = 0;
  virtual uint8_t  getHeight()        const noexcept = 0;
  virtual uint8_t  getCharSpacing()   const noexcept = 0;
  
  virtual uint8_t  getFirstChar()     const noexcept = 0;
  virtual uint8_t  getLastChar()      const noexcept = 0;
  
  virtual uint8_t  getBytesPerLine()  const noexcept = 0;
  
  virtual bool     getPixel( uint8_t char_, uint8_t x_, uint8_t y_ ) const noexcept = 0;
  
  virtual inline bool getPixelImpl( uint8_t* pFontData_, uint8_t c_, uint8_t x_, uint8_t y_ ) const noexcept
  {
    if( c_ > getLastChar() || x_ >= getWidth() || y_ >= getHeight() )
      return false;
    
    if( getBytesPerLine() == 1 )
    {
      return ( ( pFontData_[ ( c_ * getHeight() ) + y_ ] & ( 0x080 >> x_ ) ) > 0 );
    }
    else
    {
      return (
       ( pFontData_[ ( c_ * getHeight() ) + ( y_ * getBytesPerLine() ) + ( x_ >> 3 ) ] & ( 0x080 >> ( x_ % 8 ) ) ) > 0
      );
    }
  }

};

template<class TFontClass>
class FontBase : public Font
{

public:

  static TFontClass* get()
  {
    static TFontClass m_font;
    return &m_font;
  }
  

};
  
//----------------------------------------------------------------------------------------------------------------------

} // sl

#endif // SL_FONT__H
