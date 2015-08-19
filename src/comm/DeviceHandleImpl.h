/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <functional>

namespace sl
{
namespace kio
{

//--------------------------------------------------------------------------------------------------

class Transfer;

//--------------------------------------------------------------------------------------------------

class DeviceHandleImpl
{

public:

  using tCbRead = std::function<void(Transfer)>;

  virtual ~DeviceHandleImpl(){}

  virtual void disconnect() = 0;

  virtual bool read( Transfer&, uint8_t ) = 0;
  virtual bool write( const Transfer&, uint8_t ) const = 0;

  virtual void readAsync(uint8_t, tCbRead) {}
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
