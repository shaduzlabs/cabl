/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "Euklid.h"

#include <sstream>

#include <gfx/LCDDisplay.h>
#include <devices/DeviceMaschineMK1.h>
#include <devices/DeviceMaschineMikroMK2.h>

namespace
{
static const uint8_t kEuklidDefaultSteps = 16;
static const uint8_t kEuklidDefaultPulses = 4;
static const uint8_t kEuklidDefaultOffset = 0;
static const uint8_t kEuklidNumTracks = 3;

static const sl::util::LedColor kEuklidColor_Track[3] = {
  { 60,0,0,80 },{ 0,60,0,80 },{ 0,0,60,80 }
};
static const sl::util::LedColor kEuklidColor_Track_CurrentStep[3] = {
  { 127,0,0,127 },{ 0,127,0,127 },{ 0,0,127,127 }
};

static const sl::util::LedColor kEuklidColor_Black         (  0,   0,   0,   0);

static const sl::util::LedColor kEuklidColor_Step_Empty    (35, 35, 35,  20);
static const sl::util::LedColor kEuklidColor_Step_Empty_Current    (127, 127, 127,  50);

}

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace midi;
using namespace util;
using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

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
  , m_update(true)
{

  for (uint8_t i = 0; i < kEuklidNumTracks; i++)
  {
    m_lengths[i] = kEuklidDefaultSteps;
    m_pulses[i] = kEuklidDefaultPulses;
    m_rotates[i] = kEuklidDefaultOffset;
    m_sequences[i].calculate(m_lengths[i], m_pulses[i]);
    m_sequences[i].rotate(m_rotates[i]);
  }
  
  m_pMidiout->openVirtualPort("Euklid");
}

//--------------------------------------------------------------------------------------------------

bool Euklid::initHardware()
{
  getDevice(0)->getGraphicDisplay(0)->black();
  getDevice(0)->getGraphicDisplay(1)->black();

  getDevice(0)->setLed(Device::Key::Key1, kEuklidColor_Track[0]);
  
  getDevice(0)->setCallbackButtonChanged(std::bind(&Euklid::buttonChanged, this, _1, _2, _3));
  getDevice(0)->setCallbackEncoderChanged(std::bind(&Euklid::encoderChanged, this, _1, _2, _3));
  getDevice(0)->setCallbackPadChanged(std::bind(&Euklid::padChanged, this, _1, _2, _3));
  getDevice(0)->setCallbackKeyChanged(std::bind(&Euklid::keyChanged, this, _1, _2, _3));
  
  m_update = true;

  return true;
}

//--------------------------------------------------------------------------------------------------

bool Euklid::tick()
{
  if(m_update)
  {
    updateGUI();
    updateGroupLeds();
    updatePads();

    m_update = false;
  }

  return getDevice(0)->tick();
}

//--------------------------------------------------------------------------------------------------

void Euklid::buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_)
{
  if (button_ == Device::Button::F1)
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
  else if (button_ == Device::Button::F2)
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
  else if (button_ == Device::Button::F3)
  {
    if (getScreenPage() == Euklid::ScreenPage::Sequencer)
    {
      setEncoderState(Euklid::EncoderState::Rotate);
    }
  }
  else if (buttonState_ && (button_ == Device::Button::Group || button_ == Device::Button::Browse))
  {
    changeTrack();
  }
  else if (buttonState_ && button_ == Device::Button::PageLeft)
  {
    prevTrack();
  }
  else if (buttonState_ && button_ == Device::Button::PageRight)
  {
    nextTrack();
  }
  else if (buttonState_ && button_ == Device::Button::GroupA)
  {
    changeTrack(0);
  }
  else if (buttonState_ && button_ == Device::Button::GroupB)
  {
    changeTrack(1);
  }
  else if (buttonState_ && button_ == Device::Button::GroupC)
  {
    changeTrack(2);
  }
  else if ((button_ == Device::Button::Play || button_ == Device::Button::Sync) && buttonState_)
  {
    togglePlay();
  }
  else if (button_ == Device::Button::Control && buttonState_)
  {
    setScreenPage(getScreenPage() == Euklid::ScreenPage::Configuration
                                  ? Euklid::ScreenPage::Sequencer
                                  : Euklid::ScreenPage::Configuration);
  }
  else if(button_ >= Device::Button::Pad1 && button_ <= Device::Button::Pad16 && buttonState_)
  {
    uint8_t padIndex = static_cast<uint8_t>(button_) - static_cast<uint8_t>(Device::Button::Pad1 );
    m_sequences[m_currentTrack].toggleStep(padIndex);
    m_update = true;
  }
  updateGUI();
}

//--------------------------------------------------------------------------------------------------

void Euklid::encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
  uint8_t step = (shiftPressed_ ? 5 : 1);
  switch(encoder_)
  {
    case Device::Encoder::Encoder1:
    {
      m_lengths[m_currentTrack] = getEncoderValue(
        valueIncreased_,
        step,
        m_lengths[m_currentTrack],
        1,
        16
      );
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case Device::Encoder::Main:
    {
      setEncoder(valueIncreased_, shiftPressed_);
      break;
    }
    case Device::Encoder::Encoder2:
    {
      m_pulses[m_currentTrack] = getEncoderValue(
        valueIncreased_,
        step,
        m_pulses[m_currentTrack],
        0, m_lengths[m_currentTrack]
      );
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case Device::Encoder::Encoder3:
    {
      m_rotates[m_currentTrack] = getEncoderValue(
        valueIncreased_,
        step,
        m_rotates[m_currentTrack],
        0,
        m_lengths[m_currentTrack]
      );
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case Device::Encoder::Encoder4:
    {
      m_bpm = getEncoderValue(valueIncreased_, step, m_bpm, 60, 255);
      updateClock();
      break;
    }
    case Device::Encoder::Encoder5:
    {
      m_shuffle = getEncoderValue(valueIncreased_, step, m_shuffle, 0, 100);
      updateClock();
      break;
    }
    default:
      break;
  }
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Euklid::padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed_)
{
  static auto lastEvent = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  if (now - lastEvent > std::chrono::milliseconds(180))
  {
    lastEvent = now;
    uint8_t padIndex = getPadIndex(pad_);
    if (m_sequences[m_currentTrack].toggleStep(padIndex))
    {/*
      switch (m_currentTrack)
      {
      case 0:
        getDevice(0)->setLed(getPadLed(padIndex), 127, 0, 0);
        break;
      case 1:
        getDevice(0)->setLed(getPadLed(padIndex), 0, 127, 0);
        break;
      case 2:
        getDevice(0)->setLed(getPadLed(padIndex), 0, 0, 127);
        break;
      }*/
    }
    else
    {
   //   getDevice(0)->setLed(getPadLed(padIndex), 0);
    }
    m_update = true;
  }

}

//--------------------------------------------------------------------------------------------------

void Euklid::keyChanged(Device::Key key_, uint16_t value_, bool shiftPressed_)
{
  static auto lastEvent = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  if (now - lastEvent > std::chrono::milliseconds(180))
  {
    lastEvent = now;
    uint8_t padIndex = static_cast<uint8_t>(key_);
    if (m_sequences[m_currentTrack].toggleStep(padIndex))
    {/*
      switch (m_currentTrack)
      {
      case 0:
        getDevice(0)->setLed(getPadLed(padIndex), 127, 0, 0);
        break;
      case 1:
        getDevice(0)->setLed(getPadLed(padIndex), 0, 127, 0);
        break;
      case 2:
        getDevice(0)->setLed(getPadLed(padIndex), 0, 0, 127);
        break;
      }*/
    }
    else
    {
   //   getDevice(0)->setLed(getPadLed(padIndex), 0);
    }
    m_update = true;
  }

}

//--------------------------------------------------------------------------------------------------

void Euklid::updateClock()
{
  float quarterDuration = 60000.0f/m_bpm;
  float delayQuarterNote = quarterDuration / 4.0f;
  float shuffleDelay = delayQuarterNote * (m_shuffle/300.0f);
  m_delayEven = static_cast<uint16_t>(delayQuarterNote + shuffleDelay);
  m_delayOdd = static_cast<uint16_t>(delayQuarterNote - shuffleDelay);
}

//--------------------------------------------------------------------------------------------------

void Euklid::play()
{
  m_quarterNote = 0;
  updateClock();

  while (m_play)
  {
    m_update = true;
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
    for (uint8_t i = 0; i < kEuklidNumTracks; i++)
    {
      MidiNote note(MidiNote::Name::C, 2);
      if (i == 1)
      {
        note.setNote(MidiNote::Name::D);
      }
      else if (i == 2)
      {
        note.setNote(MidiNote::Name::FSharp);
      }
      if (m_sequences[i].next())
      {
  /*      MidiMessage* m = new NoteOn(0, note.value(), 127);
        NoteOn noteObj(0, note.value(), 127);
        std::vector<uint8_t> msg(noteObj.data());
        m_pMidiout->sendMessage(&msg);
     //   getDevice(0)->sendMidiMsg(msg);*/
      }
    }
    
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::updateGUI()
{
  static Canvas::Color s_colorWhite = Canvas::Color::White;
  static LCDDisplay::Align s_alignCenter = LCDDisplay::Align::Center;

  std::string strTrackName = "TRACK " + std::to_string(m_currentTrack+1);
  
  getDevice(0)->getGraphicDisplay(0)->black();
  getDevice(0)->getGraphicDisplay(0)->printStr(32, 52, "E U K L I D");
  getDevice(0)->getGraphicDisplay(0)->drawFilledRect(0, 52, 28, 6, s_colorWhite, s_colorWhite);
  getDevice(0)->getGraphicDisplay(0)->drawFilledRect(100, 52, 28, 6, s_colorWhite, s_colorWhite);
  
   getDevice(0)->getLCDDisplay(0)->setText("AB", 0);
  /*

  getDevice(0)->getLCDDisplay(0)->setText(strTrackName, 1);
  getDevice(0)->getLCDDisplay(0)->setText("{EUKLID}", 2,s_alignCenter);

  getDevice(0)->getLCDDisplay(1)->setText("Length", 1, s_alignCenter);
  getDevice(0)->getLCDDisplay(1)->setValue(
    static_cast<float>(m_lengths[m_currentTrack]) / kEuklidDefaultSteps,
    0
  );
  getDevice(0)->getLCDDisplay(1)->setText(
    static_cast<int>(m_lengths[m_currentTrack]),
    2,
    s_alignCenter
  );

  getDevice(0)->getLCDDisplay(2)->setText("Density", 1);
  getDevice(0)->getLCDDisplay(2)->setValue(
    static_cast<float>(m_pulses[m_currentTrack]) / kEuklidDefaultSteps,
    0
  );
  getDevice(0)->getLCDDisplay(2)->setText(
    static_cast<double>(m_pulses[m_currentTrack]) / kEuklidDefaultSteps,
    2,
    s_alignCenter
  );
  
  getDevice(0)->getLCDDisplay(3)->setText("Rotation", 1);
  getDevice(0)->getLCDDisplay(3)->setValue(
    static_cast<float>(m_rotates[m_currentTrack]) / kEuklidDefaultSteps,
    0
  );
  getDevice(0)->getLCDDisplay(3)->setText(
    static_cast<int>(m_rotates[m_currentTrack]),
    2,s_alignCenter
  );

  getDevice(0)->getLCDDisplay(4)->setText("BPM", 1, s_alignCenter);
  getDevice(0)->getLCDDisplay(4)->setValue(static_cast<float>(m_bpm) / 255.0, 0);
  getDevice(0)->getLCDDisplay(4)->setText(static_cast<int>(m_bpm), 2, s_alignCenter);

  getDevice(0)->getLCDDisplay(5)->setText("Shuffle", 1, s_alignCenter);
  getDevice(0)->getLCDDisplay(5)->setValue(static_cast<float>(m_shuffle) / 100, 0);
  getDevice(0)->getLCDDisplay(5)->setText(static_cast<int>(m_shuffle), 2, s_alignCenter);
  */
//  getDevice(0)->getLCDDisplay(3)->setText(m_rotates[m_currentTrack], 2);
  
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

//--------------------------------------------------------------------------------------------------

void Euklid::updateGroupLeds()
{
  switch (m_currentTrack)
  {
  case 0:
    getDevice(0)->setLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[0]);
    getDevice(0)->setLed(Device::Button::GroupA, kEuklidColor_Track_CurrentStep[0]);
    getDevice(0)->setLed(Device::Button::GroupB, kEuklidColor_Black);
    getDevice(0)->setLed(Device::Button::GroupC, kEuklidColor_Black);
    break;
  case 1:
    getDevice(0)->setLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[1]);
    getDevice(0)->setLed(Device::Button::GroupA, kEuklidColor_Black);
    getDevice(0)->setLed(Device::Button::GroupB, kEuklidColor_Track_CurrentStep[1]);
    getDevice(0)->setLed(Device::Button::GroupC, kEuklidColor_Black);
    break;
  case 2:
    getDevice(0)->setLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[2]);
    getDevice(0)->setLed(Device::Button::GroupA, kEuklidColor_Black);
    getDevice(0)->setLed(Device::Button::GroupB, kEuklidColor_Black);
    getDevice(0)->setLed(Device::Button::GroupC, kEuklidColor_Track_CurrentStep[2]);
    break;
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::updatePads()
{  
  for (uint8_t t = 0; t < kEuklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    uint16_t pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      Device::Pad pad = getPad(i);
      Device::Key key = static_cast<Device::Key>(i);

      if (m_currentTrack == t)
      {

        if (i >= m_lengths[t])
        {
          getDevice(0)->setLed(pad, kEuklidColor_Black);
          getDevice(0)->setLed(key, kEuklidColor_Black);
        }
        else if (pulses & (1 << i))
        {
          if (pos == (k % m_lengths[t]) && m_play)
          {
            getDevice(0)->setLed(pad, kEuklidColor_Track_CurrentStep[m_currentTrack]);
            getDevice(0)->setLed(key, kEuklidColor_Track_CurrentStep[m_currentTrack]);
          }
          else
          {
            getDevice(0)->setLed(pad, kEuklidColor_Track[m_currentTrack]);
            getDevice(0)->setLed(key, kEuklidColor_Track[m_currentTrack]);
          }
        }
        else
        {
          if (pos == (k % m_lengths[t]) && m_play)
          {
            getDevice(0)->setLed(pad, kEuklidColor_Step_Empty_Current);
            getDevice(0)->setLed(key, kEuklidColor_Step_Empty_Current);
          }
          else
          {
            getDevice(0)->setLed(pad, kEuklidColor_Step_Empty);
            getDevice(0)->setLed(key, kEuklidColor_Step_Empty);
          }
        }
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::drawConfigurationPage()
{
  if(m_encoderState != EncoderState::Speed && m_encoderState != EncoderState::Shuffle)
  {
    m_encoderState = EncoderState::Speed;
  }
  

  getDevice(0)->getGraphicDisplay(0)->printStr(5, 2, " BPM   Shuffle");
  getDevice(0)->getGraphicDisplay(0)->printStr(10, 12, std::to_string(m_bpm).c_str());
  getDevice(0)->getGraphicDisplay(0)->printStr(59, 12, std::to_string(m_shuffle).c_str());

  getDevice(0)->setLed(Device::Button::F1, 0);
  getDevice(0)->setLed(Device::Button::F2, 0);
  getDevice(0)->setLed(Device::Button::F3, 0);
  getDevice(0)->setLed(Device::Button::Control, 255);
  
  
  
  switch (m_encoderState)
  {
    case EncoderState::Shuffle:
    {
      getDevice(0)->getGraphicDisplay(0)->drawFilledRect(41, 0, 52, 20, Canvas::Color::Invert,
                                                  Canvas::Color::Invert);
      getDevice(0)->setLed(Device::Button::F2, 255);
      break;
    }
    case EncoderState::Speed:
    {
      getDevice(0)->getGraphicDisplay(0)->drawFilledRect(0, 0, 40, 20, Canvas::Color::Invert,
                                                  Canvas::Color::Invert);
      getDevice(0)->setLed(Device::Button::F1, 255);
      break;
    }
    default:
      break;
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::drawSequencerPage()
{
  if (m_encoderState != EncoderState::Length && m_encoderState != EncoderState::Pulses
      && m_encoderState != EncoderState::Rotate)
  {
    m_encoderState = EncoderState::Length;
  }

  getDevice(0)->getGraphicDisplay(0)->printStr(5, 2, "Length Pulses Rotate");
  for (uint8_t i = 0; i < kEuklidNumTracks; i++)
  {
    for (uint8_t n = 0; n < m_sequences[i].getLength(); n++)
    {
      getDevice(0)->getGraphicDisplay(0)->drawRect(
        n * 8,
        15 + (12 * i),
        7,
        7,
        Canvas::Color::White
      );
    }
  }

  getDevice(0)->setLed(Device::Button::F1, 0);
  getDevice(0)->setLed(Device::Button::F2, 0);
  getDevice(0)->setLed(Device::Button::F3, 0);
  getDevice(0)->setLed(Device::Button::Control, 0);

  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      getDevice(0)->getGraphicDisplay(0)->drawFilledRect(43, 0, 42, 10, Canvas::Color::Invert,
                                                  Canvas::Color::Invert);
        getDevice(0)->setLed(Device::Button::F2, 255);
      break;
    }
    case EncoderState::Rotate:
    {
      getDevice(0)->getGraphicDisplay(0)->drawFilledRect(86, 0, 40, 10, Canvas::Color::Invert,
                                                  Canvas::Color::Invert);
      getDevice(0)->setLed(Device::Button::F3, 255);
      break;
    }
    case EncoderState::Length:
    {
      getDevice(0)->getGraphicDisplay(0)->drawFilledRect(0, 0, 42, 10, Canvas::Color::Invert,
                                                  Canvas::Color::Invert);
      getDevice(0)->setLed(Device::Button::F1, 255);
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
        getDevice(0)->getGraphicDisplay(0)->drawFilledRect((k % m_lengths[t]) * 8, 15 + (12 * t), 7,
                                                           7, Canvas::Color::White,
                                                           Canvas::Color::White);
      }
    }
    getDevice(0)->getGraphicDisplay(0)->drawRect((pos * 8) + 1, 16 + (12 * t), 5, 5,
                                                 Canvas::Color::Invert);
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::setEncoder(bool valueIncreased_, bool shiftPressed_)
{
  uint8_t step = (shiftPressed_ ? 5 : 1);
  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      m_pulses[m_currentTrack] = getEncoderValue(valueIncreased_, step, m_pulses[m_currentTrack], 0,
                                                 m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Rotate:
    {
      m_rotates[m_currentTrack] = getEncoderValue(valueIncreased_, step, m_rotates[m_currentTrack],
                                                  0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Length:
    {
      m_lengths[m_currentTrack]
        = getEncoderValue(valueIncreased_, step, m_lengths[m_currentTrack], 1, 16);
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
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Euklid::togglePlay()
{
  m_play = !m_play;
  if (m_play)
  {
    getDevice(0)->setLed(Device::Button::Play, 255);
    m_clockFuture = std::async(std::launch::async, std::bind(&Euklid::play, this));
  }
  else
  {
    getDevice(0)->setLed(Device::Button::Play, 0);
    m_clockFuture.get();
    for (uint8_t t = 0; t < kEuklidNumTracks; t++)
    {
      m_sequences[t].reset();
    }
    m_quarterNote = 0;
    m_update = true;
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::changeTrack(uint8_t track_)
{
  if(track_==0xFF)
  {
    m_currentTrack++;
    if (m_currentTrack >= kEuklidNumTracks)
    {
      m_currentTrack = 0;
    }
  }
  else
  {
    m_currentTrack = track_;
  }
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Euklid::nextTrack()
{
  if (m_currentTrack >= (kEuklidNumTracks -1))
  {
    m_currentTrack = 0;
  }
  else
  {
    m_currentTrack++;
  }
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Euklid::prevTrack()
{
  if (m_currentTrack > 0)
  {
    m_currentTrack--;
  }
  else
  {
    m_currentTrack = (kEuklidNumTracks - 1);
  }
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------

Device::Pad Euklid::getPad(uint8_t padIndex_)
{
  switch (padIndex_)
  {
    case 0:  return Device::Pad::Pad13;
    case 1:  return Device::Pad::Pad14;
    case 2:  return Device::Pad::Pad15;
    case 3:  return Device::Pad::Pad16;
    case 4:  return Device::Pad::Pad9;
    case 5:  return Device::Pad::Pad10;
    case 6:  return Device::Pad::Pad11;
    case 7:  return Device::Pad::Pad12;
    case 8:  return Device::Pad::Pad5;
    case 9:  return Device::Pad::Pad6;
    case 10: return Device::Pad::Pad7;
    case 11: return Device::Pad::Pad8;
    case 12: return Device::Pad::Pad1;
    case 13: return Device::Pad::Pad2;
    case 14: return Device::Pad::Pad3;
    case 15: return Device::Pad::Pad4;
  }
  return Device::Pad::Unknown;
}
//--------------------------------------------------------------------------------------------------

uint8_t Euklid::getPadIndex(Device::Pad pad_)
{
  switch (pad_)
  {
    case Device::Pad::Pad13: return 0;
    case Device::Pad::Pad14: return 1;
    case Device::Pad::Pad15: return 2;
    case Device::Pad::Pad16: return 3;
    case Device::Pad::Pad9:  return 4;
    case Device::Pad::Pad10: return 5;
    case Device::Pad::Pad11: return 6;
    case Device::Pad::Pad12: return 7;
    case Device::Pad::Pad5:  return 8;
    case Device::Pad::Pad6:  return 9;
    case Device::Pad::Pad7:  return 10;
    case Device::Pad::Pad8:  return 11;
    case Device::Pad::Pad1:  return 12;
    case Device::Pad::Pad2:  return 13;
    case Device::Pad::Pad3:  return 14;
    case Device::Pad::Pad4:  return 15;
    default: return 0;
  }
  return 0;
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
