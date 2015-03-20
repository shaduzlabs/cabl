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

#include "Evklid.h"

#include <sstream>

#include <devices/DeviceMaschineMK1.h>
#include <devices/DeviceMaschineMikroMK2.h>

namespace
{
static const uint8_t kEvklidDefaultSteps = 16;
static const uint8_t kEvklidDefaultPulses = 4;
static const uint8_t kEvklidDefaultOffset = 0;
static const uint8_t kEvklidNumTracks = 3;
}

//----------------------------------------------------------------------------------------------------------------------

namespace sl
{

//----------------------------------------------------------------------------------------------------------------------

void padsChanged(uint16_t mask, const uint16_t* pPads)
{
  // nothing to do here...
}

//----------------------------------------------------------------------------------------------------------------------

Evklid::Evklid(Device* pDevice_)
  : m_pDevice(pDevice_)
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
  m_pDevice->connect();
  m_pDevice->getDisplay(0)->black();

//  m_pDevice->setCallbackPadsChanged(padsChanged);
  m_pDevice->setCallbackButtonChanged(std::bind(&Evklid::buttonChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_pDevice->setCallbackEncoderChanged(std::bind(&Evklid::encoderChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  for (uint8_t i = 0; i < kEvklidNumTracks; i++)
  {
    m_lengths[i] = kEvklidDefaultSteps;
    m_pulses[i] = kEvklidDefaultPulses;
    m_rotates[i] = kEvklidDefaultOffset;
    m_sequences[i].calculate(m_lengths[i], m_pulses[i]);
    m_sequences[i].rotate(m_rotates[i]);
  }

  m_pMidiout->openVirtualPort("Euklid");
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_)
{
  if (button_ == Device::Button::F1)
  {
    if (getScreenPage() == Evklid::ScreenPage::Configuration)
    {
      setEncoderState(Evklid::EncoderState::Speed);
    }
    else
    {
      setEncoderState(Evklid::EncoderState::Length);
    }
  }
  else if (button_ == Device::Button::F2)
  {
    if (getScreenPage() == Evklid::ScreenPage::Configuration)
    {
      setEncoderState(Evklid::EncoderState::Shuffle);
    }
    else
    {
      setEncoderState(Evklid::EncoderState::Pulses);
    }
  }
  else if (button_ == Device::Button::F3)
  {
    if (getScreenPage() == Evklid::ScreenPage::Sequencer)
    {
      setEncoderState(Evklid::EncoderState::Rotate);
    }
  }
  else if (button_ == Device::Button::Group && buttonState_)
  {
    changeTrack();
  }
  else if (button_ == Device::Button::Play && buttonState_)
  {
    togglePlay();
  }
  else if (button_ == Device::Button::Control && buttonState_)
  {
    setScreenPage(getScreenPage() == Evklid::ScreenPage::Configuration
                                  ? Evklid::ScreenPage::Sequencer
                                  : Evklid::ScreenPage::Configuration);
  }
  //  getDevice()->setLed(button_, ((unsigned)(buttonState_)) * 255);
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::encoderChanged(uint8_t encoderIndex_, bool valueIncreased_, bool shiftPressed_)
{
  setEncoder(valueIncreased_, shiftPressed_);
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::tick()
{
  m_pDevice->tick();
  updateGUI();
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::updateClock()
{
  unsigned quarterDuration = 60000/m_bpm;
  float delayQuarterNote = quarterDuration / 4.0;
  float shuffleDelay = delayQuarterNote * (m_shuffle/300.0);
  m_delayEven = delayQuarterNote + shuffleDelay;
  m_delayOdd = delayQuarterNote - shuffleDelay;
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::play()
{
  m_quarterNote = 0;
  updateClock();
  
  while (m_play)
  {
    for (uint8_t i = 0; i < kEvklidNumTracks; i++)
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

    uint8_t delay = m_delayEven;
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

void Evklid::updateGUI()
{
  m_pDevice->getDisplay(0)->black();
  m_pDevice->getDisplay(0)->printStr(32, 52, "E U K L I D");
  m_pDevice->getDisplay(0)->drawFilledRect(0, 52, 28, 6, Canvas::tColor::WHITE, Canvas::tColor::WHITE);
  m_pDevice->getDisplay(0)->drawFilledRect(100, 52, 28, 6, Canvas::tColor::WHITE, Canvas::tColor::WHITE);

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
  
  // Update group led
  switch (m_currentTrack)
  {
    case 0:
      m_pDevice->setLed(Device::Button::Group, 255, 0, 0);
      break;
    case 1:
      m_pDevice->setLed(Device::Button::Group, 0, 255, 0);
      break;
    case 2:
      m_pDevice->setLed(Device::Button::Group, 0, 0, 255);
      break;
  }

  // Update pads
  for (uint8_t t = 0; t < kEvklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    uint16_t pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      Device::Button pad = getPadLed(k % m_lengths[t]);

      if (m_currentTrack == t)
      {

        if (i >= m_lengths[t])
        {
          m_pDevice->setLed(getPadLed(i), 0, 0, 0);
        }
        else if (pulses & (1 << i))
        {
          if (pos == (k % m_lengths[t]))
          {
            switch (m_currentTrack)
            {
              case 0:
                m_pDevice->setLed(pad, 255, 0, 0);
                break;
              case 1:
                m_pDevice->setLed(pad, 0, 255, 0);
                break;
              case 2:
                m_pDevice->setLed(pad, 0, 0, 255);
                break;
            }
          }
          else
          {
            switch (m_currentTrack)
            {
              case 0:
                m_pDevice->setLed(pad, 150, 0, 0);
                break;
              case 1:
                m_pDevice->setLed(pad, 0, 150, 0);
                break;
              case 2:
                m_pDevice->setLed(pad, 0, 0, 150);
                break;
            }
          }
        }
        else
        {
          if (pos == (k % m_lengths[t]))
          {
            m_pDevice->setLed(pad, 50, 50, 50);
          }
          else
          {
            m_pDevice->setLed(pad, 20, 20, 20);
          }
        }
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------

void Evklid::drawConfigurationPage()
{
  if(m_encoderState != EncoderState::Speed && m_encoderState != EncoderState::Shuffle)
  {
    m_encoderState = EncoderState::Speed;
  }
  
  m_pDevice->getDisplay(0)->printStr(5, 2, " BPM   Shuffle");
  m_pDevice->getDisplay(0)->printStr(10, 12, std::to_string(m_bpm).c_str());
  m_pDevice->getDisplay(0)->printStr(59, 12, std::to_string(m_shuffle).c_str());
  m_pDevice->setLed(Device::Button::F1, 0);
  m_pDevice->setLed(Device::Button::F2, 0);
  m_pDevice->setLed(Device::Button::F3, 0);
  m_pDevice->setLed(Device::Button::Control, 255);
  
  
  
  switch (m_encoderState)
  {
    case EncoderState::Shuffle:
    {
      m_pDevice->getDisplay(0)->drawFilledRect(41, 0, 52, 20, Canvas::tColor::INVERT, Canvas::tColor::INVERT);
      m_pDevice->setLed(Device::Button::F2, 255);
      break;
    }
    case EncoderState::Speed:
    {
      m_pDevice->getDisplay(0)->drawFilledRect(0, 0, 40, 20, Canvas::tColor::INVERT, Canvas::tColor::INVERT);
      m_pDevice->setLed(Device::Button::F1, 255);
      break;
    }
    default:
      break;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::drawSequencerPage()
{
  if(m_encoderState != EncoderState::Length && m_encoderState != EncoderState::Pulses && m_encoderState != EncoderState::Rotate)
  {
    m_encoderState = EncoderState::Length;
  }

  m_pDevice->getDisplay(0)->printStr(5, 2, "Length Pulses Rotate");

  for (uint8_t i = 0; i < kEvklidNumTracks; i++)
  {
    for (uint8_t n = 0; n < m_sequences[i].getLength(); n++)
    {
      m_pDevice->getDisplay(0)->drawRect(n * 8, 15 + (12 * i), 7, 7, Canvas::tColor::WHITE);
    }
  }

  m_pDevice->setLed(Device::Button::F1, 0);
  m_pDevice->setLed(Device::Button::F2, 0);
  m_pDevice->setLed(Device::Button::F3, 0);
  m_pDevice->setLed(Device::Button::Control, 0);

  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      m_pDevice->getDisplay(0)->drawFilledRect(43, 0, 42, 10, Canvas::tColor::INVERT, Canvas::tColor::INVERT);
      m_pDevice->setLed(Device::Button::F2, 255);
      break;
    }
    case EncoderState::Rotate:
    {
      m_pDevice->getDisplay(0)->drawFilledRect(86, 0, 40, 10, Canvas::tColor::INVERT, Canvas::tColor::INVERT);
      m_pDevice->setLed(Device::Button::F3, 255);
      break;
    }
    case EncoderState::Length:
    {
      m_pDevice->getDisplay(0)->drawFilledRect(0, 0, 42, 10, Canvas::tColor::INVERT, Canvas::tColor::INVERT);
      m_pDevice->setLed(Device::Button::F1, 255);
      break;
    }
    default:
      break;
  }

  for (uint8_t t = 0; t < kEvklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    uint16_t pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      if (pulses & (1 << i))
      {
        m_pDevice->getDisplay(0)
          ->drawFilledRect((k % m_lengths[t]) * 8, 15 + (12 * t), 7, 7, Canvas::tColor::WHITE, Canvas::tColor::WHITE);
      }
    }
    m_pDevice->getDisplay(0)->drawRect((pos * 8) + 1, 16 + (12 * t), 5, 5, Canvas::tColor::INVERT);
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::setEncoder(bool valueIncreased_, bool shiftPressed_)
{
  uint8_t step = (shiftPressed_ ? 5 : 1);
  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      m_pulses[m_currentTrack]
        = getEncoderValue(valueIncreased_, step, m_pulses[m_currentTrack], 0, m_lengths[m_currentTrack]);
      break;
    }
    case EncoderState::Rotate:
    {
      m_rotates[m_currentTrack]
        = getEncoderValue(valueIncreased_, step, m_rotates[m_currentTrack], 0, m_lengths[m_currentTrack]);
      break;
    }
    case EncoderState::Length:
    {
      m_lengths[m_currentTrack] = getEncoderValue(valueIncreased_, step, m_lengths[m_currentTrack], 1, 16);
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
  m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
  m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::togglePlay()
{
  m_play = !m_play;
  if (m_play)
  {
    m_pDevice->setLed(Device::Button::Play, 255);
    m_clockFuture = std::async(std::launch::async, std::bind(&Evklid::play, this));
  }
  else
  {
    m_pDevice->setLed(Device::Button::Play, 0);
    m_clockFuture.get();
    for (uint8_t t = 0; t < kEvklidNumTracks; t++)
    {
      m_sequences[t].reset();
    }
    m_quarterNote = 0;
  }
}

//----------------------------------------------------------------------------------------------------------------------

void Evklid::changeTrack()
{
  m_currentTrack++;
  if (m_currentTrack >= kEvklidNumTracks)
  {
    m_currentTrack = 0;
  }
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t Evklid::getEncoderValue(
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

Device::Button Evklid::getPadLed(uint8_t padIndex_)
{
  switch (padIndex_)
  {
    case 0:
      return Device::Button::Pad13;
      break;
    case 1:
      return Device::Button::Pad14;
      break;
    case 2:
      return Device::Button::Pad15;
      break;
    case 3:
      return Device::Button::Pad16;
      break;
    case 4:
      return Device::Button::Pad9;
      break;
    case 5:
      return Device::Button::Pad10;
      break;
    case 6:
      return Device::Button::Pad11;
      break;
    case 7:
      return Device::Button::Pad12;
      break;
    case 8:
      return Device::Button::Pad5;
      break;
    case 9:
      return Device::Button::Pad6;
      break;
    case 10:
      return Device::Button::Pad7;
      break;
    case 11:
      return Device::Button::Pad8;
      break;
    case 12:
      return Device::Button::Pad1;
      break;
    case 13:
      return Device::Button::Pad2;
      break;
    case 14:
      return Device::Button::Pad3;
      break;
    case 15:
      return Device::Button::Pad4;
      break;
  }
  return Device::Button::Unknown;
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace sl
