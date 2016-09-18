/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DeviceTest.h"

#include <algorithm>
#include <cmath>
#include <sstream>

#include <unmidify.hpp>

#include <cabl/gfx/TextDisplay.h>
#include <devices/ni/MaschineMK1.h>
#include <devices/ni/MaschineMK2.h>
#include <devices/ni/MaschineMikroMK2.h>

//--------------------------------------------------------------------------------------------------

namespace
{
const sl::util::ColorRGB kColor_Black{0};
const sl::util::ColorRGB kColor_Red{0xff, 0, 0, 0xff};
const sl::util::ColorRGB kColor_Yellow{0xff, 0xff, 0, 0x55};
}

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace midi;
using namespace util;
using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

void DeviceTest::initDevice()
{
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::render()
{
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_)
{
  device()->setButtonLed(
    button_, buttonState_ ? (shiftState_ ? kColor_Red : kColor_Yellow) : kColor_Black);
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
  device()->setKeyLed(index_, {static_cast<uint8_t>(value_ * 0xff)});
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::controlChanged(Device::Potentiometer pot_, double value_, bool shiftPressed_)
{
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
