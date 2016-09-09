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

class Push : public USBMidi
{

public:
  Push();

  void setLed(Device::Button, const util::ColorRGB&) override;
  void setLed(Device::Pad, const util::ColorRGB&) override;

  void sendMidiMsg(tRawData) override;

  LCDDisplay* displayLCD(size_t displayIndex_) override;

  size_t numOfGraphicDisplays() const override { return 0; }

  size_t numOfLCDDisplays() const override { return 4; }

  size_t numOfLedMatrices() const override { return 0; }

  size_t numOfLedArrays() const override { return 0; }

  bool tick() override;

private:
  enum class Led : uint8_t;
  enum class Button : uint8_t;
  enum class Encoder : uint8_t;

  static constexpr uint8_t kPush_nDisplays = 4;
  static constexpr uint8_t kPush_nButtons = 75;
  static constexpr uint8_t kPush_ledsDataSize = 184;
  static constexpr uint8_t kPush_buttonsDataSize = 138;
  static constexpr uint8_t kPush_padDataSize = 64;
  static constexpr uint8_t kPush_nPads = 64;
  static constexpr uint8_t kPush_padsBufferSize = 16;

  void init() override;

  void initDisplay() const;
  bool sendDisplayData();
  bool sendLeds();

  void setLedImpl(Led, const util::ColorRGB&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(Device::Pad) const noexcept;

  Device::Button deviceButton(Button) const noexcept;
  Device::Encoder deviceEncoder(Encoder) const noexcept;

  uint8_t getColorIndex(const util::ColorRGB&);

  void onNoteOff(NoteOff msg) override;
  void onNoteOn(NoteOn msg) override;
  void onPolyPressure(PolyPressure msg) override;
  void onControlChange(ControlChange msg) override;
  void onProgramChange(ProgramChange msg) override;
  void onChannelPressure(ChannelPressure msg) override;
  void onPitchBend(PitchBend msg) override;
  void onClock(Clock msg_) override
  {
  }
  void onSysEx(SysEx msg_) override;
  void onUSysExRT(USysExRT msg_) override;
  void onUSysExNonRT(USysExNonRT msg_) override;

  void processNote(uint8_t, uint8_t);

  LCDDisplayGeneric<17, 4> m_displays[kPush_nDisplays];

  std::array<uint8_t, kPush_ledsDataSize> m_leds;
  std::array<uint8_t, kPush_ledsDataSize> m_ledsPrev;

  bool m_shiftPressed;

  bool m_isDirtyLeds;

  std::map<util::ColorRGB, uint8_t> m_colorsCache;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  Push, "Ableton Push User Port", DeviceDescriptor::Type::MIDI, 0x0047, 0x1500);

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
