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

#include <cstdint>
#include "util/Types.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class Transfer
{
    
public:
  
  Transfer();
  Transfer( uint16_t length_ );
  
  Transfer( tRawData data_ );
  Transfer( tRawData header_, tRawData data_ );
  Transfer( tRawData header_, const uint8_t* pData_, size_t dataLength_ );
  
  virtual ~Transfer();
  
  operator bool() const{ return ( m_data.size() > 0 ); }
  
  inline uint8_t &operator[](int i)
  {
    return m_data[i];
  }
 
  inline const uint8_t &operator[](int i) const
  {
    return m_data[i];
  }
  
  void reset();
  
//  uint8_t* getDataPtr() const { return m_pData.get(); }
//  void setData( const uint8_t*, uint16_t );
  const tRawData& getData() const { return m_data; }
  void setData( const uint8_t*, size_t);
  
  size_t size() const noexcept{ return m_data.size(); }

private:

  tRawData         m_data;
 
};
  
//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
