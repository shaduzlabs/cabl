/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "devices/Device.h"
#include "gfx/displays/GDisplayDummy.h"
#include "gfx/displays/LCDDisplayKompleteKontrol.h"

namespace sl
{
namespace kio
{
namespace devices
{
  
//--------------------------------------------------------------------------------------------------
    
class KompleteKontrol : public Device
{
 
public:
  
  KompleteKontrol(tPtr<DeviceHandle>, uint8_t numKeys_);
  ~KompleteKontrol() override;
  
  void setLed(Device::Button, const util::LedColor&) override;
  void setLed(Device::Key, const util::LedColor&) override;

  void sendMidiMsg(tRawData) override;
  
  GDisplay* getGraphicDisplay(uint8_t displayIndex_) override;
  LCDDisplay* getLCDDisplay(uint8_t displayIndex_) override;

  bool tick() override;

private:

  enum class Led    : uint16_t;
  enum class Button : uint8_t;

  static constexpr uint8_t kKK_nButtons = 37;
  static constexpr uint8_t kKK_buttonsDataSize = 6;
  static constexpr uint8_t kKK_nEncoders = 9;
  static constexpr uint8_t kKK_nDisplays = 9;

  void init() override;
  bool sendDisplayData();
  bool sendLeds();
  bool read();
  
  void processButtons( const Transfer& );
  
  void setLedImpl(Led, const util::LedColor&);
  bool isRGBLed(Led) const noexcept;
  Led getLed(Device::Key) const noexcept;
  Led getLed(Device::Button) const noexcept;

  Device::Button getDeviceButton( Button btn_ ) const noexcept;
  bool isButtonPressed( Button button ) const noexcept;
  bool isButtonPressed( const Transfer&, Button button_) const noexcept;
  
  static void midiInCallback(double timeStamp, std::vector<unsigned char> *message, void *userData);

  GDisplayDummy               m_displayDummy;
  tRawData                    m_leds;
  tRawData                    m_ledsKeys;
  tRawData                    m_buttons;
  bool                        m_buttonStates[kKK_nButtons];
  uint16_t                    m_encoderValues[kKK_nEncoders];

  uint8_t                     m_numKeys;
  uint16_t                    m_ledKeysDataSize;
    
  bool                        m_isDirtyLeds;
  bool                        m_isDirtyKeyLeds;

  uint8_t                     m_firstOctave;

  LCDDisplayKompleteKontrol   m_displays[kKK_nDisplays];
    
#if defined(_WIN32) || defined(__APPLE__) || defined(__linux)
  tPtr<RtMidiOut>     m_pMidiOut;
  tPtr<RtMidiIn>      m_pMidiIn;
#endif
};
  
//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
