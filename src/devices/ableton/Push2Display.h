/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <map>

#include "devices/Device.h"
#include "gfx/displays/GDisplayPush2.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class Push2Display : public Device
{

public:
  Push2Display();
  ~Push2Display() override;

  void setLed(Device::Button, const util::ColorRGB&) override
  {
  }
  void setLed(Device::Pad, const util::ColorRGB&) override
  {
  }
  void sendMidiMsg(tRawData) override
  {
  }

  GDisplay* displayGraphic(size_t displayIndex_) override;
  LCDDisplay* displayLCD(size_t displayIndex_) override;

  bool tick() override;

private:
  bool sendDisplayData() const;

  void processNote(uint8_t note_, uint8_t velocity_);
  static void midiInCallback(double timeStamp, std::vector<unsigned char>* message, void* userData);

  tPtr<RtMidiOut> m_pMidiOut;
  tPtr<RtMidiIn> m_pMidiIn;

  static constexpr uint16_t kPush2_nRows = 160;
  static constexpr uint16_t kPush2_nColumns = 1024;
  static constexpr uint8_t kPush2_nBytesPerPixel = 2;

  void init() override;

  bool m_shiftPressed;

  GDisplayPush2 m_display;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  Push2Display, "Ableton Push 2", DeviceDescriptor::Type::USB, 0x2982, 0x1967);

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
