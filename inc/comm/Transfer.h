/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "util/Types.h"
#include <cstdint>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Transfer
{

public:
  Transfer();
  Transfer(uint16_t length_);

  Transfer(tRawData data_);
  Transfer(const tRawData& header_, const tRawData& data_);
  Transfer(const tRawData& header_, const uint8_t* pData_, size_t dataLength_);

  virtual ~Transfer();

  operator bool() const
  {
    return (m_data.size() > 0);
  }

  inline uint8_t& operator[](int i)
  {
    return m_data[i];
  }

  inline const uint8_t& operator[](int i) const
  {
    return m_data[i];
  }

  void reset();

  //  uint8_t* dataPtr() const { return m_pData.get(); }
  //  void setData( const uint8_t*, uint16_t );
  const tRawData& data() const
  {
    return m_data;
  }
  void setData(const uint8_t*, size_t);

  size_t size() const noexcept
  {
    return m_data.size();
  }

private:
  tRawData m_data;
};

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
