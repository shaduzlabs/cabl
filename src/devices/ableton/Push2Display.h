/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <map>

#include "cabl/devices/Device.h"
#include "gfx/displays/GDisplayPush2.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Push2Display : public Device
{

public:

  void setButtonLed(Device::Button, const Color&) override
  {
  }
  void setKeyLed(unsigned, const Color&) override
  {
  }

  Canvas* graphicDisplay(size_t displayIndex_) override;

  size_t numOfGraphicDisplays() const override
  {
    return 1;
  }

  size_t numOfTextDisplays() const override
  {
    return 0;
  }

  size_t numOfLedMatrices() const override
  {
    return 0;
  }

  size_t numOfLedArrays() const override
  {
    return 0;
  }

  bool tick() override;

private:
  bool sendDisplayData() const;

  void init() override;

  GDisplayPush2 m_display;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  Push2Display, "Ableton Push 2", DeviceDescriptor::Type::USB, 0x2982, 0x1967);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
