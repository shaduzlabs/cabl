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
  
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer()
{

}
  
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( uint16_t length_ )
  : m_pData( new uint8_t[length_] )
  , m_length( length_ )
{

}
  
//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer( const uint8_t* pData_, uint16_t length_ )
  : Transfer( length_ )
{
  setData( pData_, length_ );
}

//----------------------------------------------------------------------------------------------------------------------

Transfer::Transfer(
  const uint8_t* pHeader_,
  uint16_t headerLength_,
  const uint8_t* pData_,
  uint16_t dataLength_
)
  : Transfer( headerLength_ + dataLength_ )
{
  memcpy( m_pData.get(), pHeader_, headerLength_ );
  memcpy( ( m_pData.get() + headerLength_ ), pData_, dataLength_ );
}
//----------------------------------------------------------------------------------------------------------------------
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
  m_length = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void Transfer::setData( const uint8_t* data_, uint16_t length_ )
{
  if( length_ == 0 || data_ == nullptr )
    return;
  
  m_length = length_;
  m_pData.reset( new uint8_t[m_length] );
  memcpy( m_pData.get(), data_, m_length );
}

//----------------------------------------------------------------------------------------------------------------------

} // sl
