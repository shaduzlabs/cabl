/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include "Euklid.h"

#include <sstream>

#include <devices/DeviceMaschineMK1.h>
#include <devices/DeviceMaschineMikroMK2.h>

namespace
{
static const uint8_t kEuklidDefaultSteps = 16;
static const uint8_t kEuklidDefaultPulses = 4;
static const uint8_t kEuklidDefaultOffset = 0;
static const uint8_t kEuklidNumTracks = 3;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{

//----------------------------------------------------------------------------------------------------------------------

Euklid::Euklid()
  : Application({DeviceDescriptor(0x17cc, 0x1200)})
  , m_encoderState(EncoderState::Length)
  , m_screenPage(ScreenPage::Sequencer)
  , m_play(false)
  , m_currentTrack(0)
  , m_bpm(120)
  , m_shuffle(60)
  , m_pMidiout(new RtMidiOut)
  , m_delayEven(125)
  , m_delayOdd(125)
{

}

//----------------------------------------------------------------------------------------------------------------------

bool Euklid::initHardware()
{  
  getDevice(0)->getDisplay(0)->black();

  getDevice(0)->setCallbackButtonChanged(
    std::bind(&Euklid::buttonChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  getDevice(0)->setCallbackEncoderChanged(
    std::bind(&Euklid::encoderChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  getDevice(0)->setCallbackPadChanged(
    std::bind(&Euklid::padChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  for (uint8_t i = 0; i < kEuklidNumTracks; i++)
  {
    m_lengths[i] = kEuklidDefaultSteps;
    m_pulses[i] = kEuklidDefaultPulses;
    m_rotates[i] = kEuklidDefaultOffset;
    m_sequences[i].calculate(m_lengths[i], m_pulses[i]);
    m_sequences[i].rotate(m_rotates[i]);
  }

  updateGUI();
  updateGroupLeds();
  updatePads();

  m_pMidiout->openVirtualPort("Euklid");
  return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool Euklid::tick()
{
  return getDevice(0)->tick();
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::buttonChanged(kio::Device::Button button_, bool buttonState_, bool shiftState_)
{
  if (button_ == kio::Device::Button::F1)
  {
    if (getScreenPage() == Euklid::ScreenPage::Configuration)
    {
      setEncoderState(Euklid::EncoderState::Speed);
    }
    else
    {
      setEncoderState(Euklid::EncoderState::Length);
    }
  }
  else if (button_ == kio::Device::Button::F2)
  {
    if (getScreenPage() == Euklid::ScreenPage::Configuration)
    {
      setEncoderState(Euklid::EncoderState::Shuffle);
    }
    else
    {
      setEncoderState(Euklid::EncoderState::Pulses);
    }
  }
  else if (button_ == kio::Device::Button::F3)
  {
    if (getScreenPage() == Euklid::ScreenPage::Sequencer)
    {
      setEncoderState(Euklid::EncoderState::Rotate);
    }
  }
  else if (button_ == kio::Device::Button::Group && buttonState_)
  {
    changeTrack();
  }
  else if (button_ == kio::Device::Button::Play && buttonState_)
  {
    togglePlay();
  }
  else if (button_ == kio::Device::Button::Control && buttonState_)
  {
    setScreenPage(getScreenPage() == Euklid::ScreenPage::Configuration
                                  ? Euklid::ScreenPage::Sequencer
                                  : Euklid::ScreenPage::Configuration);
  }
  updateGUI();
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
	setEncoder(valueIncreased_, shiftPressed_);
  updateGUI();
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::padChanged(kio::Device::Pad pad_, uint16_t value_, bool shiftPressed_)
{
  static auto lastEvent = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  if (now - lastEvent > std::chrono::milliseconds(180))
  {
    lastEvent = now;
    uint8_t padIndex = getPadIndex(pad_);
    if (m_sequences[m_currentTrack].toggleStep(padIndex))
    {
      getDevice(0)->setLed(getPadLed(padIndex), 255, 255, 255);
    }
    else
    {
      getDevice(0)->setLed(getPadLed(padIndex), 0);
    }
    updateGUI();
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::updateClock()
{
  unsigned quarterDuration = 60000/m_bpm;
  float delayQuarterNote = quarterDuration / 4.0f;
  float shuffleDelay = delayQuarterNote * (m_shuffle/300.0f);
  m_delayEven = static_cast<uint16_t>(delayQuarterNote + shuffleDelay);
  m_delayOdd = static_cast<uint16_t>(delayQuarterNote - shuffleDelay);
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::play()
{
  m_quarterNote = 0;
  updateClock();
  
  while (m_play)
  {
    updatePads();
    updateGUI();
    for (uint8_t i = 0; i < kEuklidNumTracks; i++)
    {
      uint8_t channel = 0x99 + i;
      uint8_t note = 0x24;
      if (i == 1)
      {
        note = 0x26;
      }
      else if (i == 2)
      {
        note = 0x2A;
      }
      if (m_sequences[i].next())
      {
        std::vector<uint8_t> msg{channel, note, 0x7f};
        m_pMidiout->sendMessage(&msg);
      }
    }

    uint16_t delay = m_delayEven;
    if (m_quarterNote % 2 > 0)
    {
      delay = m_delayOdd;
    }

    if (++m_quarterNote > 3)
    {
      m_quarterNote = 0;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::updateGUI()
{
  getDevice(0)->getDisplay(0)->black();
  getDevice(0)->getDisplay(0)->printStr(32, 52, "E U K L I D");
  getDevice(0)->getDisplay(0)->drawFilledRect(0, 52, 28, 6, kio::Canvas::tColor::WHITE, kio::Canvas::tColor::WHITE);
  getDevice(0)->getDisplay(0)->drawFilledRect(100, 52, 28, 6, kio::Canvas::tColor::WHITE, kio::Canvas::tColor::WHITE);

  switch (m_screenPage)
  {
    case ScreenPage::Configuration:
    {
      drawConfigurationPage();
      break;
    }
    case ScreenPage::Sequencer:
    default:
    {
      drawSequencerPage();
      break;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::updateGroupLeds()
{
  switch (m_currentTrack)
  {
  case 0:
    getDevice(0)->setLed(kio::Device::Button::Group, 255, 0, 0);
    break;
  case 1:
    getDevice(0)->setLed(kio::Device::Button::Group, 0, 255, 0);
    break;
  case 2:
    getDevice(0)->setLed(kio::Device::Button::Group, 0, 0, 255);
    break;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::updatePads()
{
  for (uint8_t t = 0; t < kEuklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    uint16_t pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      kio::Device::Button pad = getPadLed(k % m_lengths[t]);

      if (m_currentTrack == t)
      {

        if (i >= m_lengths[t])
        {
          getDevice(0)->setLed(getPadLed(i), 0);
        }
        else if (pulses & (1 << i))
        {
          if (pos == (k % m_lengths[t]))
          {
            switch (m_currentTrack)
            {
            case 0:
              getDevice(0)->setLed(pad, 150, 0, 0);
              break;
            case 1:
              getDevice(0)->setLed(pad, 0, 150, 0);
              break;
            case 2:
              getDevice(0)->setLed(pad, 0, 0, 150);
              break;
            }
          }
          else
          {
            switch (m_currentTrack)
            {
            case 0:
              getDevice(0)->setLed(pad, 255, 0, 0);
              break;
            case 1:
              getDevice(0)->setLed(pad, 0, 255, 0);
              break;
            case 2:
              getDevice(0)->setLed(pad, 0, 0, 255);
              break;
            }
          }
        }
        else
        {
          if (pos == (k % m_lengths[t]))
          {
            getDevice(0)->setLed(pad, 0, 0, 0);
          }
          else
          {
            getDevice(0)->setLed(pad, 0, 0, 0);
          }
        }
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::drawConfigurationPage()
{
  if(m_encoderState != EncoderState::Speed && m_encoderState != EncoderState::Shuffle)
  {
    m_encoderState = EncoderState::Speed;
  }
  
  getDevice(0)->getDisplay(0)->printStr(5, 2, " BPM   Shuffle");
  getDevice(0)->getDisplay(0)->printStr(10, 12, std::to_string(m_bpm).c_str());
  getDevice(0)->getDisplay(0)->printStr(59, 12, std::to_string(m_shuffle).c_str());
  getDevice(0)->setLed(kio::Device::Button::F1, 0);
  getDevice(0)->setLed(kio::Device::Button::F2, 0);
  getDevice(0)->setLed(kio::Device::Button::F3, 0);
  getDevice(0)->setLed(kio::Device::Button::Control, 255);
  
  
  
  switch (m_encoderState)
  {
    case EncoderState::Shuffle:
    {
      getDevice(0)->getDisplay(0)->drawFilledRect(41, 0, 52, 20, kio::Canvas::tColor::INVERT,
                                                  kio::Canvas::tColor::INVERT);
      getDevice(0)->setLed(kio::Device::Button::F2, 255);
      break;
    }
    case EncoderState::Speed:
    {
      getDevice(0)->getDisplay(0)->drawFilledRect(0, 0, 40, 20, kio::Canvas::tColor::INVERT,
                                                  kio::Canvas::tColor::INVERT);
      getDevice(0)->setLed(kio::Device::Button::F1, 255);
      break;
    }
    default:
      break;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::drawSequencerPage()
{
  if (m_encoderState != EncoderState::Length && m_encoderState != EncoderState::Pulses
      && m_encoderState != EncoderState::Rotate)
  {
    m_encoderState = EncoderState::Length;
  }

  getDevice(0)->getDisplay(0)->printStr(5, 2, "Length Pulses Rotate");

  for (uint8_t i = 0; i < kEuklidNumTracks; i++)
  {
    for (uint8_t n = 0; n < m_sequences[i].getLength(); n++)
    {
      getDevice(0)->getDisplay(0)->drawRect(n * 8, 15 + (12 * i), 7, 7, kio::Canvas::tColor::WHITE);
    }
  }

  getDevice(0)->setLed(kio::Device::Button::F1, 0);
  getDevice(0)->setLed(kio::Device::Button::F2, 0);
  getDevice(0)->setLed(kio::Device::Button::F3, 0);
  getDevice(0)->setLed(kio::Device::Button::Control, 0);

  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      getDevice(0)->getDisplay(0)->drawFilledRect(43, 0, 42, 10, kio::Canvas::tColor::INVERT,
                                                  kio::Canvas::tColor::INVERT);
      getDevice(0)->setLed(kio::Device::Button::F2, 255);
      break;
    }
    case EncoderState::Rotate:
    {
      getDevice(0)->getDisplay(0)->drawFilledRect(86, 0, 40, 10, kio::Canvas::tColor::INVERT,
                                                  kio::Canvas::tColor::INVERT);
      getDevice(0)->setLed(kio::Device::Button::F3, 255);
      break;
    }
    case EncoderState::Length:
    {
      getDevice(0)->getDisplay(0)->drawFilledRect(0, 0, 42, 10, kio::Canvas::tColor::INVERT,
                                                  kio::Canvas::tColor::INVERT);
      getDevice(0)->setLed(kio::Device::Button::F1, 255);
      break;
    }
    default:
      break;
  }

  for (uint8_t t = 0; t < kEuklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    uint16_t pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      if (pulses & (1 << i))
      {
        getDevice(0)->getDisplay(0)->drawFilledRect((k % m_lengths[t]) * 8, 15 + (12 * t), 7, 7,
                                                    kio::Canvas::tColor::WHITE, kio::Canvas::tColor::WHITE);
      }
    }
    getDevice(0)->getDisplay(0)->drawRect((pos * 8) + 1, 16 + (12 * t), 5, 5, kio::Canvas::tColor::INVERT);
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::setEncoder(bool valueIncreased_, bool shiftPressed_)
{
  uint8_t step = (shiftPressed_ ? 5 : 1);
  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      m_pulses[m_currentTrack]
        = getEncoderValue(valueIncreased_, step, m_pulses[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Rotate:
    {
      m_rotates[m_currentTrack]
        = getEncoderValue(valueIncreased_, step, m_rotates[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Length:
    {
      m_lengths[m_currentTrack] = getEncoderValue(valueIncreased_, step, m_lengths[m_currentTrack], 1, 16);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Shuffle:
    {
      m_shuffle = getEncoderValue(valueIncreased_, step, m_shuffle, 0, 100);
      updateClock();
      break;
    }
    case EncoderState::Speed:
    {
      m_bpm = getEncoderValue(valueIncreased_, step, m_bpm, 60, 255);
      updateClock();
      break;
    }
    default:
      break;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::togglePlay()
{
  m_play = !m_play;
  if (m_play)
  {
    getDevice(0)->setLed(kio::Device::Button::Play, 255);
    m_clockFuture = std::async(std::launch::async, std::bind(&Euklid::play, this));
  }
  else
  {
    getDevice(0)->setLed(kio::Device::Button::Play, 0);
    m_clockFuture.get();
    for (uint8_t t = 0; t < kEuklidNumTracks; t++)
    {
      m_sequences[t].reset();
    }
    m_quarterNote = 0;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Euklid::changeTrack()
{
  m_currentTrack++;
  if (m_currentTrack >= kEuklidNumTracks)
  {
    m_currentTrack = 0;
  }
  updateGroupLeds();
  updatePads();
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t Euklid::getEncoderValue(
  bool valueIncreased_, uint8_t step_, uint8_t currentValue_, uint8_t minValue_, uint8_t maxValue_)
{
  if (valueIncreased_ && ((currentValue_ + step_) <= maxValue_))
  {
    return currentValue_+step_;
  }
  else if (!valueIncreased_ && ((currentValue_ - step_) >= minValue_))
  {
    return currentValue_-step_;
  }
  return currentValue_;
}

//----------------------------------------------------------------------------------------------------------------------

kio::Device::Button Euklid::getPadLed(uint8_t padIndex_)
{
  switch (padIndex_)
  {
    case 0:  return kio::Device::Button::Pad13;
    case 1:  return kio::Device::Button::Pad14;
    case 2:  return kio::Device::Button::Pad15;
    case 3:  return kio::Device::Button::Pad16;
    case 4:  return kio::Device::Button::Pad9;
    case 5:  return kio::Device::Button::Pad10;
    case 6:  return kio::Device::Button::Pad11;
    case 7:  return kio::Device::Button::Pad12;
    case 8:  return kio::Device::Button::Pad5;
    case 9:  return kio::Device::Button::Pad6;
    case 10: return kio::Device::Button::Pad7;
    case 11: return kio::Device::Button::Pad8;
    case 12: return kio::Device::Button::Pad1;
    case 13: return kio::Device::Button::Pad2;
    case 14: return kio::Device::Button::Pad3;
    case 15: return kio::Device::Button::Pad4;
  }
  return kio::Device::Button::Unknown;
}
//----------------------------------------------------------------------------------------------------------------------

uint8_t Euklid::getPadIndex(kio::Device::Pad pad_)
{
  switch (pad_)
  {
    case kio::Device::Pad::Pad13: return 0;
    case kio::Device::Pad::Pad14: return 1;
    case kio::Device::Pad::Pad15: return 2;
    case kio::Device::Pad::Pad16: return 3;
    case kio::Device::Pad::Pad9:  return 4;
    case kio::Device::Pad::Pad10: return 5;
    case kio::Device::Pad::Pad11: return 6;
    case kio::Device::Pad::Pad12: return 7;
    case kio::Device::Pad::Pad5:  return 8;
    case kio::Device::Pad::Pad6:  return 9;
    case kio::Device::Pad::Pad7:  return 10;
    case kio::Device::Pad::Pad8:  return 11;
    case kio::Device::Pad::Pad1:  return 12;
    case kio::Device::Pad::Pad2:  return 13;
    case kio::Device::Pad::Pad3:  return 14;
    case kio::Device::Pad::Pad4:  return 15;
    default: return 0;
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace sl
