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
#include "devices/generic/USBMidi.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class Push2 : public USBMidi
{

public:
  Push2();

  void setButtonLed(Device::Button, const Color&) override;
  void setKeyLed(unsigned, const Color&) override;

  size_t numOfGraphicDisplays() const override
  {
    return 0;
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

  void sendMidiMsg(tRawData) override;

  bool tick() override;

private:
  enum class Led : uint8_t;
  enum class Button : uint8_t;
  enum class Encoder : uint8_t;

  static constexpr uint8_t kPush_nDisplays = 4;
  static constexpr uint8_t kPush_nButtons = 45;
  static constexpr uint8_t kPush_ledsDataSize = 78;
  static constexpr uint8_t kPush_buttonsDataSize = 138;
  static constexpr uint8_t kPush_padDataSize = 64;
  static constexpr uint8_t kPush_nPads = 64;
  static constexpr uint8_t kPush_padsBufferSize = 16;

  void init() override;

  bool sendLeds();

  void setLedImpl(Led, const Color&);
  bool isRGBLed(Led) const noexcept;
  Led led(Device::Button) const noexcept;
  Led led(unsigned) const noexcept;

  Device::Button deviceButton(Button) const noexcept;

  uint8_t getColorIndex(const Color&);

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

  void processNote(uint8_t note_, uint8_t velocity_);

  static void midiInCallback(double timeStamp, std::vector<unsigned char>* message, void* userData);

  std::array<uint8_t, kPush_ledsDataSize> m_leds;
  bool m_shiftPressed;

  bool m_isDirtyLeds;

  std::map<Color, uint8_t> m_colorsCache;
  tPtr<RtMidiOut> m_pMidiOut;
  tPtr<RtMidiIn> m_pMidiIn;
};

//--------------------------------------------------------------------------------------------------

M_REGISTER_DEVICE_CLASS(
  Push2, "Ableton Push Live Port", DeviceDescriptor::Type::MIDI, 0x211D, 0x6732);

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
