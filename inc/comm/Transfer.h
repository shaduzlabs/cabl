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

#ifndef SL_TRANSFER_H
#define SL_TRANSFER_H

#include <cstdint>
#include "Types.h"

namespace sl
{

class Transfer
{
    
public:
  
  Transfer();
  Transfer( uint16_t length_ );
  Transfer( const uint8_t* data_, uint16_t length_ );
  Transfer( const uint8_t* pHeader_, uint16_t headerLength_, const uint8_t* pData_, uint16_t dataLength_ );
  
  //Transfer( const std::initializer_list<uint8_t>& data_, uint8_t endpoint_ );
 // Transfer( const std::initializer_list<uint8_t>& header_, const uint8_t* data_, uint16_t length_, uint8_t endpoint_ );
  
  virtual ~Transfer();
  
  operator bool() const{ return ( m_pData && ( m_length > 0 ) ); }
  
  inline uint8_t &operator[](int i)
  {
    return m_pData[i];
  }
 
  inline const uint8_t &operator[](int i) const
  {
    return m_pData[i];
  }
  
  void reset();
  
  uint8_t* getDataPtr() const { return m_pData.get(); }
  void setData( const uint8_t*, uint16_t );
  
  uint16_t getSize() const noexcept{ return m_length; }

private:

  tPtr<uint8_t[]>  m_pData;
  uint16_t         m_length;
 
};
  
//----------------------------------------------------------------------------------------------------------------------

} // sl

#endif // SL_TRANSFER_H
