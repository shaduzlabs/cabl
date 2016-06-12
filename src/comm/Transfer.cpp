/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "comm/Transfer.h"
#include <iostream>

namespace sl
{
namespace cabl
{
  
//--------------------------------------------------------------------------------------------------

Transfer::Transfer( uint16_t length_ )
{
  m_data.resize(length_);
}

//--------------------------------------------------------------------------------------------------

Transfer::Transfer( tRawData data_ )
  : m_data(std::move(data_))
{
}

//--------------------------------------------------------------------------------------------------

Transfer::Transfer( const tRawData& header_, const tRawData& data_ )
{
  m_data.resize(header_.size()+data_.size());
  std::copy(header_.begin(),header_.end(),m_data.begin());
  std::copy(data_.begin(),data_.end(),&m_data[header_.size()]);
}

//--------------------------------------------------------------------------------------------------

Transfer::Transfer( const tRawData& header_, const uint8_t* pData_, size_t dataLength_ )
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
//--------------------------------------------------------------------------------------------------

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
//--------------------------------------------------------------------------------------------------
*/
Transfer::~Transfer()
{
  reset();
}
  
//--------------------------------------------------------------------------------------------------

bool Transfer::operator == ( const Transfer& other_ ) const
{
  return (m_data == other_.m_data);
}

//--------------------------------------------------------------------------------------------------

bool Transfer::operator != ( const Transfer& other_ ) const
{
  return !( operator == ( other_ ) );
}

//--------------------------------------------------------------------------------------------------

void Transfer::reset()
{  
  m_data.clear();
}

//--------------------------------------------------------------------------------------------------

void Transfer::setData( const uint8_t* data_, size_t length_ )
{
  if( length_ == 0 || data_ == nullptr )
    return;
  
  m_data.resize( length_ );
  std::copy(data_,(data_+length_),m_data.begin());
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
