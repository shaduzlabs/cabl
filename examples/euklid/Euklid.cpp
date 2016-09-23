/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "Euklid.h"

#include <algorithm>
#include <cmath>
#include <sstream>

#include <unmidify.hpp>

#include <cabl/gfx/TextDisplay.h>

namespace
{
const uint8_t kEuklidDefaultSteps = 16;
const uint8_t kEuklidDefaultPulses = 4;
const uint8_t kEuklidDefaultOffset = 0;
const uint8_t kEuklidNumTracks = 3;

const sl::Color kEuklidColor_Track[3] = {{60, 0, 0, 80}, {0, 60, 0, 80}, {0, 0, 60, 80}};
const sl::Color kEuklidColor_Track_CurrentStep[3]
  = {{127, 0, 0, 127}, {0, 127, 0, 127}, {0, 0, 127, 127}};

const sl::Color kEuklidColor_Black(0, 0, 0, 0);

const sl::Color kEuklidColor_Step_Empty(35, 35, 35, 20);
const sl::Color kEuklidColor_Step_Empty_Current(127, 127, 127, 50);
}

//--------------------------------------------------------------------------------------------------

namespace sl
{

using namespace midi;
using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

Euklid::Euklid()
  : m_encoderState(EncoderState::Length)
  , m_screenPage(ScreenPage::Sequencer)
  , m_play(false)
  , m_currentTrack(0)
  , m_bpm(120.)
  , m_shuffle(60.)
  , m_pMidiout(new RtMidiOut)
  , m_delayEven(125)
  , m_delayOdd(125)
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

void Euklid::initDevice()
{
  device()->setKeyLed(0, kEuklidColor_Track[0]);
}

//--------------------------------------------------------------------------------------------------

void Euklid::render()
{
  updateGUI();
  updateGroupLeds();
  updatePads();
  updateTouchStrips();
}

//--------------------------------------------------------------------------------------------------

void Euklid::buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_)
{
  if (button_ == Device::Button::F1 || button_ == Device::Button::DisplayButton1)
  {
    if (screenPage() == Euklid::ScreenPage::Configuration)
    {
      setEncoderState(Euklid::EncoderState::Speed);
    }
    else
    {
      setEncoderState(Euklid::EncoderState::Length);
    }
  }
  else if (button_ == Device::Button::F2 || button_ == Device::Button::DisplayButton2)
  {
    if (screenPage() == Euklid::ScreenPage::Configuration)
    {
      setEncoderState(Euklid::EncoderState::Shuffle);
    }
    else
    {
      setEncoderState(Euklid::EncoderState::Pulses);
    }
  }
  else if (button_ == Device::Button::F3 || button_ == Device::Button::DisplayButton3)
  {
    if (screenPage() == Euklid::ScreenPage::Sequencer)
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
    setScreenPage(screenPage() == Euklid::ScreenPage::Configuration
                    ? Euklid::ScreenPage::Sequencer
                    : Euklid::ScreenPage::Configuration);
  }
  else if (button_ >= Device::Button::Pad1 && button_ <= Device::Button::Pad16 && buttonState_)
  {
    uint8_t padIndex = static_cast<uint8_t>(button_) - static_cast<uint8_t>(Device::Button::Pad1);
    m_sequences[m_currentTrack].toggleStep(padIndex);
  }
  else
  {
    return;
  }

  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

void Euklid::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
  uint8_t step = (shiftPressed_ ? 5 : 1);
  switch (encoder_)
  {
    case 0:
    {
      setEncoder(valueIncreased_, shiftPressed_);
      break;
    }
    case 1:
    {
      m_lengths[m_currentTrack]
        = encoderValue(valueIncreased_, step, m_lengths[m_currentTrack], 1, 16);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 2:
    {
      m_pulses[m_currentTrack] = encoderValue(
        valueIncreased_, step, m_pulses[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 3:
    {
      m_rotates[m_currentTrack] = encoderValue(
        valueIncreased_, step, m_rotates[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 4:
    {
      m_bpm = encoderValue(valueIncreased_, step, m_bpm, 60, 255);
      updateClock();
      break;
    }
    case 5:
    {
      m_shuffle = encoderValue(valueIncreased_, step, m_shuffle, 0, 100);
      updateClock();
      break;
    }
    default:
      break;
  }
  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

void Euklid::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
  static auto lastEvent = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  if (now - lastEvent > std::chrono::milliseconds(180))
  {
    lastEvent = now;
    m_sequences[m_currentTrack].toggleStep(index_);
    requestDeviceUpdate();
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::controlChanged(unsigned pot_, double value_, bool shiftPressed_)
{
  switch (pot_)
  {
    case 0:
    {
      m_lengths[m_currentTrack] = std::max<uint8_t>(1, static_cast<uint8_t>((value_ * 16)+0.5));
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 1:
    {
      m_pulses[m_currentTrack] = std::max<uint8_t>(0, static_cast<uint8_t>(( m_lengths[m_currentTrack] * value_ ) + 0.5));
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 2:
    {
    
      m_rotates[m_currentTrack] = std::max<uint8_t>(0, static_cast<uint8_t>((m_lengths[m_currentTrack] * value_)+0.5));
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case 3:
    {
      m_bpm = (value_ * 195) + 60;
      updateClock();
      break;
    }
    case 4:
    {
      m_shuffle = value_ * 100;
      updateClock();
      break;
    }
    default:
      break;
  }
  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

void Euklid::updateClock()
{
	double quarterDuration = 60000.0f / m_bpm;
  double delayQuarterNote = quarterDuration / 4.0f;
  double shuffleDelay = delayQuarterNote * (m_shuffle / 300.0f);
  m_delayEven = static_cast<unsigned>(delayQuarterNote + shuffleDelay);
  m_delayOdd = static_cast<unsigned>(delayQuarterNote - shuffleDelay);
}

//--------------------------------------------------------------------------------------------------

void Euklid::play()
{
  m_quarterNote = 0;
  updateClock();

  while (m_play)
  {
    requestDeviceUpdate();
    unsigned delay = m_delayEven;
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
           //   device()->sendMidiMsg(msg);*/
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::updateGUI()
{
  for (unsigned j = 0; j < device()->graphicDisplay(0)->width(); j++)
  {
    for (unsigned i = 0; i < device()->graphicDisplay(0)->height(); i++)
    {
      device()->graphicDisplay(0)->setPixel(j, i, {static_cast<uint8_t>(j), 0, 0});
    }
  }
  static Color s_colorWhite{0xff};
  static Alignment s_alignCenter = Alignment::Center;

  std::string strTrackName = "TRACK " + std::to_string(m_currentTrack + 1);

  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(32, 52, "E U K L I D", s_colorWhite, "normal");
  device()->graphicDisplay(0)->rectangleFilled(0, 52, 28, 6, s_colorWhite, s_colorWhite);
  device()->graphicDisplay(0)->rectangleFilled(100, 52, 28, 6, s_colorWhite, s_colorWhite);

  device()->textDisplay(0)->putText("AB", 0);


  device()->textDisplay(0)->putText(strTrackName, 1);
  device()->textDisplay(0)->putText("{EUKLID}", 2, s_alignCenter);

  device()->textDisplay(1)->putText("Length", 1, s_alignCenter);
  device()->textDisplay(1)->putValue(
    static_cast<float>(m_lengths[m_currentTrack]) / kEuklidDefaultSteps, 0);
  device()->textDisplay(1)->putText(static_cast<int>(m_lengths[m_currentTrack]), 2, s_alignCenter);

  device()->textDisplay(2)->putText("Density", 1);
  device()->textDisplay(2)->putValue(
    static_cast<float>(m_pulses[m_currentTrack]) / kEuklidDefaultSteps, 0);
  device()->textDisplay(2)->putText(
    static_cast<double>(m_pulses[m_currentTrack]) / kEuklidDefaultSteps, 2, s_alignCenter);

  device()->textDisplay(3)->putText("Rotation", 1);
  device()->textDisplay(3)->putValue(
    static_cast<float>(m_rotates[m_currentTrack]) / kEuklidDefaultSteps, 0);
  device()->textDisplay(3)->putText(static_cast<int>(m_rotates[m_currentTrack]), 2, s_alignCenter);

  device()->textDisplay(4)->putText("BPM", 1, s_alignCenter);
  device()->textDisplay(4)->putValue(static_cast<double>(m_bpm) / 255.0, 0);
  device()->textDisplay(4)->putText(static_cast<int>(m_bpm), 2, s_alignCenter);

  device()->textDisplay(5)->putText("Shuffle", 1, s_alignCenter);
  device()->textDisplay(5)->putValue(static_cast<float>(m_shuffle) / 100, 0);
  device()->textDisplay(5)->putText(static_cast<int>(m_shuffle), 2, s_alignCenter);

  //  device()->textDisplay(3)->putText(m_rotates[m_currentTrack], 2);

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
      device()->setButtonLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[0]);
      device()->setButtonLed(Device::Button::GroupA, kEuklidColor_Track_CurrentStep[0]);
      device()->setButtonLed(Device::Button::GroupB, kEuklidColor_Black);
      device()->setButtonLed(Device::Button::GroupC, kEuklidColor_Black);
      break;
    case 1:
      device()->setButtonLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[1]);
      device()->setButtonLed(Device::Button::GroupA, kEuklidColor_Black);
      device()->setButtonLed(Device::Button::GroupB, kEuklidColor_Track_CurrentStep[1]);
      device()->setButtonLed(Device::Button::GroupC, kEuklidColor_Black);
      break;
    case 2:
      device()->setButtonLed(Device::Button::Group, kEuklidColor_Track_CurrentStep[2]);
      device()->setButtonLed(Device::Button::GroupA, kEuklidColor_Black);
      device()->setButtonLed(Device::Button::GroupB, kEuklidColor_Black);
      device()->setButtonLed(Device::Button::GroupC, kEuklidColor_Track_CurrentStep[2]);
      break;
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::updatePads()
{
  for (uint8_t t = 0; t < kEuklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    unsigned pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, j = m_rotates[t]; i < 16; i++, j++)
    {
      if (m_currentTrack == t)
      {

        if (i >= m_lengths[t])
        {
          device()->setKeyLed(i, kEuklidColor_Black);
        }
        else if (pulses & (1 << i))
        {
          if (pos == (j % m_lengths[t]) && m_play)
          {
            device()->setKeyLed(i, kEuklidColor_Track_CurrentStep[m_currentTrack]);
          }
          else
          {
            device()->setKeyLed(i, kEuklidColor_Track[m_currentTrack]);
          }
        }
        else
        {
          if (pos == (j % m_lengths[t]) && m_play)
          {
            device()->setKeyLed(i, kEuklidColor_Step_Empty_Current);
          }
          else
          {
            device()->setKeyLed(i, kEuklidColor_Step_Empty);
          }
        }
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::updateTouchStrips()
{
  device()->ledArray(0)->setValue(m_lengths[m_currentTrack] / 16.0, kEuklidColor_Track_CurrentStep[m_currentTrack], Alignment::Left);
  device()->ledArray(1)->setValue(m_pulses[m_currentTrack] / static_cast<float>(m_lengths[m_currentTrack]), kEuklidColor_Track_CurrentStep[m_currentTrack], Alignment::Left);
  device()->ledArray(2)->setValue(m_rotates[m_currentTrack] / static_cast<float>(m_lengths[m_currentTrack]), kEuklidColor_Track_CurrentStep[m_currentTrack], Alignment::Left);
  device()->ledArray(3)->setValue((m_bpm - 60) / 195.0, {0xff}, Alignment::Left);
  device()->ledArray(4)->setValue(m_shuffle / 100.0, {0xff}, Alignment::Left);
}

//--------------------------------------------------------------------------------------------------

void Euklid::drawConfigurationPage()
{
  if (m_encoderState != EncoderState::Speed && m_encoderState != EncoderState::Shuffle)
  {
    m_encoderState = EncoderState::Speed;
  }


  device()->graphicDisplay(0)->putText(5, 2, " BPM   Shuffle", {0xff}, "normal");
  device()->graphicDisplay(0)->putText(10, 12, std::to_string(m_bpm).c_str(), {0xff}, "normal");
  device()->graphicDisplay(0)->putText(59, 12, std::to_string(m_shuffle).c_str(), {0xff}, "normal");

  device()->setButtonLed(Device::Button::F1, 0);
  device()->setButtonLed(Device::Button::F2, 0);
  device()->setButtonLed(Device::Button::F3, 0);
  device()->setButtonLed(Device::Button::DisplayButton1, 0);
  device()->setButtonLed(Device::Button::DisplayButton2, 0);
  device()->setButtonLed(Device::Button::DisplayButton3, 0);
  device()->setButtonLed(Device::Button::Control, 255);


  switch (m_encoderState)
  {
    case EncoderState::Shuffle:
    {
      device()->graphicDisplay(0)->rectangleFilled(
        41, 0, 52, 20, {BlendMode::Invert}, {BlendMode::Invert});
      device()->setButtonLed(Device::Button::F2, 255);
      device()->setButtonLed(Device::Button::DisplayButton2, 255);
      break;
    }
    case EncoderState::Speed:
    {
      device()->graphicDisplay(0)->rectangleFilled(
        0, 0, 40, 20, {BlendMode::Invert}, {BlendMode::Invert});
      device()->setButtonLed(Device::Button::F1, 255);
      device()->setButtonLed(Device::Button::DisplayButton1, 255);
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

  device()->graphicDisplay(0)->putText(5, 2, "Length Pulses Rotate", {0xff}, "normal");
  for (uint8_t i = 0; i < kEuklidNumTracks; i++)
  {
    for (uint8_t n = 0; n < m_sequences[i].getLength(); n++)
    {
      device()->graphicDisplay(0)->rectangle(n * 8, 15 + (12 * i), 7, 7, {0xff});
    }
  }

  device()->setButtonLed(Device::Button::F1, 0);
  device()->setButtonLed(Device::Button::F2, 0);
  device()->setButtonLed(Device::Button::F3, 0);
  device()->setButtonLed(Device::Button::DisplayButton1, 0);
  device()->setButtonLed(Device::Button::DisplayButton2, 0);
  device()->setButtonLed(Device::Button::DisplayButton3, 0);
  device()->setButtonLed(Device::Button::Control, 0);

  switch (m_encoderState)
  {
    case EncoderState::Pulses:
    {
      device()->graphicDisplay(0)->rectangleFilled(
        43, 0, 42, 10, {BlendMode::Invert}, {BlendMode::Invert});
      device()->setButtonLed(Device::Button::F2, 255);
      device()->setButtonLed(Device::Button::DisplayButton2, 255);
      break;
    }
    case EncoderState::Rotate:
    {
      device()->graphicDisplay(0)->rectangleFilled(
        86, 0, 40, 10, {BlendMode::Invert}, {BlendMode::Invert});
      device()->setButtonLed(Device::Button::F3, 255);
      device()->setButtonLed(Device::Button::DisplayButton3, 255);
      break;
    }
    case EncoderState::Length:
    {
      device()->graphicDisplay(0)->rectangleFilled(
        0, 0, 42, 10, {BlendMode::Invert}, {BlendMode::Invert});
      device()->setButtonLed(Device::Button::F1, 255);
      device()->setButtonLed(Device::Button::DisplayButton1, 255);
      break;
    }
    default:
      break;
  }

  for (uint8_t t = 0; t < kEuklidNumTracks; t++)
  {
    uint8_t pos = (m_sequences[t].getPos()) % m_lengths[t];

    unsigned pulses = m_sequences[t].getBits();
    for (uint8_t i = 0, k = m_rotates[t]; i < 16; i++, k++)
    {
      if (pulses & (1 << i))
      {
        device()->graphicDisplay(0)->rectangleFilled(
          (k % m_lengths[t]) * 8, 15 + (12 * t), 7, 7, {0xff}, {0xff});
      }
    }
    device()->graphicDisplay(0)->rectangle((pos * 8) + 1, 16 + (12 * t), 5, 5, {BlendMode::Invert});
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
      m_pulses[m_currentTrack] = encoderValue(
        valueIncreased_, step, m_pulses[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Rotate:
    {
      m_rotates[m_currentTrack] = encoderValue(
        valueIncreased_, step, m_rotates[m_currentTrack], 0, m_lengths[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Length:
    {
      m_lengths[m_currentTrack]
        = encoderValue(valueIncreased_, step, m_lengths[m_currentTrack], 1, 16);
      m_sequences[m_currentTrack].calculate(m_lengths[m_currentTrack], m_pulses[m_currentTrack]);
      m_sequences[m_currentTrack].rotate(m_rotates[m_currentTrack]);
      break;
    }
    case EncoderState::Shuffle:
    {
      m_shuffle = encoderValue(valueIncreased_, step, m_shuffle, 0, 100);
      updateClock();
      break;
    }
    case EncoderState::Speed:
    {
      m_bpm = encoderValue(valueIncreased_, step, m_bpm, 60, 255);
      updateClock();
      break;
    }
    default:
      break;
  }

  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

void Euklid::togglePlay()
{
  m_play = !m_play;
  if (m_play)
  {
    device()->setButtonLed(Device::Button::Play, 255);
    m_clockFuture = std::async(std::launch::async, std::bind(&Euklid::play, this));
  }
  else
  {
    device()->setButtonLed(Device::Button::Play, 0);
    m_clockFuture.get();
    for (uint8_t t = 0; t < kEuklidNumTracks; t++)
    {
      m_sequences[t].reset();
    }
    m_quarterNote = 0;
    requestDeviceUpdate();
  }
}

//--------------------------------------------------------------------------------------------------

void Euklid::changeTrack(uint8_t track_)
{
  if (track_ == 0xFF)
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
  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

void Euklid::nextTrack()
{
  if (m_currentTrack >= (kEuklidNumTracks - 1))
  {
    m_currentTrack = 0;
  }
  else
  {
    m_currentTrack++;
  }
  requestDeviceUpdate();
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
  requestDeviceUpdate();
}

//--------------------------------------------------------------------------------------------------

uint8_t Euklid::encoderValue(
  bool valueIncreased_, uint8_t step_, uint8_t currentValue_, uint8_t minValue_, uint8_t maxValue_)
{
  if (valueIncreased_ && ((currentValue_ + step_) <= maxValue_))
  {
    return currentValue_ + step_;
  }
  else if (!valueIncreased_ && ((currentValue_ - step_) >= minValue_))
  {
    return currentValue_ - step_;
  }
  return currentValue_;
}

//--------------------------------------------------------------------------------------------------

} // namespace sl
