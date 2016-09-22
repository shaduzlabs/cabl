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

//--------------------------------------------------------------------------------------------------

namespace
{
const sl::Color kColor_Black{0};
const sl::Color kColor_Red{0xff, 0, 0, 0xff};
const sl::Color kColor_Yellow{0xff, 0xff, 0, 0x55};
const sl::Color kColor_Blue{0xff, 0, 0, 0x77};
}

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace midi;
using namespace cabl;
using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

void DeviceTest::initDevice()
{
  for(unsigned i  = 0; i< device()->numOfLedArrays(); i++)
  {
    device()->ledArray(i)->setValue(0.5, kColor_Blue);
  }

  for(unsigned i  = 0; i< device()->numOfGraphicDisplays(); i++)
  {
    unsigned w = device()->graphicDisplay(i)->width();
    unsigned h = device()->graphicDisplay(i)->height();
    device()->graphicDisplay(i)->black();
    device()->graphicDisplay(i)->line(0, 0, w, h, {0xff});
    device()->graphicDisplay(i)->line(0, h, w, 0, {0xff});
    device()->graphicDisplay(i)->line(w/2, h, w/2, 0, {0xff});
    device()->graphicDisplay(i)->line(0, h/2, w, h/2, {0xff});
    device()->graphicDisplay(i)->rectangle(0, 0, w, h, {0xff});
    device()->graphicDisplay(i)->circle(w/2, h/2, w/2, {0xff});
    device()->graphicDisplay(i)->circle(w/2, h/2, h/2, {0xff});
  }

  
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

void DeviceTest::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
  std::string value = "Enc#" + std::to_string(static_cast<int>(encoder_)) + ( valueIncreased_ ? " increased" : " decreased" );

  device()->textDisplay(0)->putText(value.c_str(), 0);

  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, value.c_str(), {0xff});

}

//--------------------------------------------------------------------------------------------------

void DeviceTest::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
  device()->setKeyLed(index_, {static_cast<uint8_t>(value_ * 0xff)});
}

//--------------------------------------------------------------------------------------------------

void DeviceTest::controlChanged(unsigned pot_, double value_, bool shiftPressed_)
{
  std::string value = "Pot#" + std::to_string(static_cast<int>(pot_)) + " " + std::to_string(static_cast<int>(value_ * 100));

  device()->textDisplay(0)->putText(value.c_str(), 0);

  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, value.c_str(), {0xff});
  
  device()->ledArray(pot_)->setValue(value_, kColor_Red);
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
