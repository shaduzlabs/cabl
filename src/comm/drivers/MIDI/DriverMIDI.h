/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "comm/DeviceHandleImpl.h"
#include "comm/DriverImpl.h"

#include <algorithm>
#include <thread>

#include <RtMidi.h>
#include <unmidify.hpp>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class DriverMIDI : public DriverImpl
{
public:
  DriverMIDI();
  ~DriverMIDI() override;

  Driver::tCollDeviceDescriptor enumerate() override;
  tPtr<DeviceHandleImpl> connect(const DeviceDescriptor&) override;
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
