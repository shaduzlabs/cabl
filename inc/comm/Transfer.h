/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cstdint>
#include "util/Types.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

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
  
//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
