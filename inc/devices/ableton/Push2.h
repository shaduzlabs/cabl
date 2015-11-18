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
#include "devices/generic/USBMidi.h"
#include "gfx/displays/LCDDisplayGeneric.h"

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class Push2 : public USBMidi
{

public:

  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Pad, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;

  GDisplay* getGraphicDisplay(uint8_t displayIndex_) override;
  LCDDisplay* getLCDDisplay(uint8_t displayIndex_) override;

  bool tick() override;

private:

  enum class Led    : uint8_t;
  enum class Button : uint8_t;
  enum class Encoder: uint8_t;

  static constexpr uint8_t kPush_nDisplays         = 4;
  static constexpr uint8_t kPush_nButtons          = 45;
  static constexpr uint8_t kPush_ledsDataSize      = 78;
  static constexpr uint8_t kPush_buttonsDataSize   = 138;
  static constexpr uint8_t kPush_padDataSize       = 64;
  static constexpr uint8_t kPush_nPads             = 64;
  static constexpr uint8_t kPush_padsBufferSize    = 16;

  void init() override;

  void initDisplay() const;
  bool sendDisplayData();
  bool sendLeds();

  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led getLed(Device::Button) const noexcept;
  Led getLed(Device::Pad) const noexcept;

  Device::Button getDeviceButton(Button) const noexcept;
  Device::Encoder getDeviceEncoder(Encoder) const noexcept;

  uint8_t getColorIndex(const util::LedColor&);

  void onNoteOff(NoteOff msg) override;
  void onNoteOn(NoteOn msg) override;
  void onPolyPressure(PolyPressure msg) override;
  void onControlChange(ControlChange msg) override;
  void onProgramChange(ProgramChange msg) override;
  void onChannelPressure(ChannelPressure msg) override;
  void onPitchBend(PitchBend msg) override;
  void onSysEx(SysEx msg_) override;
  void onUSysExRT(USysExRT msg_) override;
  void onUSysExNonRT(USysExNonRT msg_) override;

  LCDDisplayGeneric     m_displays[kPush_nDisplays];

  std::array<uint8_t,kPush_ledsDataSize>  m_leds;
  bool                  m_shiftPressed;

  bool                  m_isDirtyLeds;

  std::map<util::RGBColor,uint8_t>  m_colorsCache;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(Push2,"Ableton Push Live Port",DeviceDescriptor::Type::MIDI,0x211D,0x6732);

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
