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

#include "comm/Transfer.h"
#include <iostream>

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer()
{

}
  
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( uint16_t length_ )
{
  m_data.resize(length_);
}

//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( tRawData data_ )
{
  m_data.resize(data_.size());
  std::copy(data_.begin(),data_.end(),data_.begin());
}

//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( tRawData header_, tRawData data_ )
{
  m_data.resize(header_.size()+data_.size());
  std::copy(header_.begin(),header_.end(),m_data.begin());
  std::copy(data_.begin(),data_.end(),&m_data[header_.size()]);
}

//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( tRawData header_, const uint8_t* pData_, uint16_t dataLength_ )
{
  m_data.resize(header_.size()+dataLength_);
  std::copy(header_.begin(),header_.end(),m_data.begin());
  std::copy(pData_,pData_+dataLength_,&m_data[header_.size()]);
}

/*

Transfer::Transfer( const std::initializer_list<uint8_t>& data_, uint8_t endpoint_ )
  : Transfer( data_.size(), endpoint_ )
{
  uint16_t i = 0;
  for( const auto& byte : data_ )
  {
    m_pData[i++]= byte;
  }
}
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer(
  const std::initializer_list<uint8_t>& header_,
  const uint8_t* data_,
  uint16_t length,
  uint8_t endpoint_
)
  : Transfer( ( header_.size() + length ), endpoint_ )
{
  uint16_t i = 0;
  for( const auto& byte : header_ )
  {
    m_pData[i++] = byte;
  }

  memcpy( ( m_pData.get() + i ), data_, m_length );

}
//----------------------------------------------------------------------------------------------------------------------
*/
Transfer::~Transfer()
{
  reset();
}
  
//----------------------------------------------------------------------------------------------------------------------

void Transfer::reset()
{  

}

//----------------------------------------------------------------------------------------------------------------------

void Transfer::setData( const uint8_t* data_, uint16_t length_ )
{
  if( length_ == 0 || data_ == nullptr )
    return;
  
  m_data.resize( length_ );
  std::copy(data_,(data_+length_),m_data.begin());
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
