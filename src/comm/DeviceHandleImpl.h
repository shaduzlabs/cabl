/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandle.h"

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

  virtual ~DeviceHandleImpl(){}

  virtual void disconnect() = 0;

  virtual bool read( Transfer&, uint8_t ) = 0;
  virtual bool write( const Transfer&, uint8_t ) = 0;

  virtual void readAsync(uint8_t, DeviceHandle::tCbRead) {}
};
  
//--------------------------------------------------------------------------------------------------

} // kio
} // sl
