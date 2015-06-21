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

#pragma once

#include <future>
#include <stdint.h>

#include <k-io.h>

#include "Sequence.h"

#define __MACOSX_CORE__ 1
#include "RtMidi.h"

namespace sl
{

using namespace kio;

class Euklid : public Application
{
public:

  enum class EncoderState
  {
    Length,
    Pulses,
    Rotate,
    Shuffle,
    Speed,
  };

  enum class ScreenPage
  {
    Sequencer,
    Configuration,
  };

  Euklid();

  bool initHardware() override;
  bool tick() override;
  
  void buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_);
  void encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_);
  void padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed);
  
  
  void updateClock();
  
  void play();

  void updateGUI();
  void updateGroupLeds();
  void updatePads();
  
  void drawConfigurationPage();
  void drawSequencerPage();

  void setEncoderState(EncoderState encoderState_)
  {
    m_encoderState = encoderState_;
  }

  ScreenPage getScreenPage() const{ return m_screenPage; }
  void setScreenPage(ScreenPage screenPage_)
  {
    m_screenPage = screenPage_;
  }

  void setEncoder(bool valueIncreased_, bool shiftPressed_);

  void togglePlay();

  void changeTrack(uint8_t track_ = 0xFF);

private:

  uint8_t getEncoderValue(
    bool valueIncreased_, uint8_t step_, uint8_t currentValue_, uint8_t minValue_, uint8_t maxValue_);
  Device::Button getPadLed(uint8_t padIndex_);
  uint8_t getPadIndex(Device::Pad pad_);

  Sequence<uint16_t> m_sequences[3];

  std::future<void> m_clockFuture;
  EncoderState m_encoderState;
  ScreenPage m_screenPage;

  uint8_t m_lengths[3];
  uint8_t m_pulses[3];
  uint8_t m_rotates[3];

  bool m_play;
  uint8_t m_currentTrack;

  uint8_t m_bpm;
  uint8_t m_shuffle;

  RtMidiOut* m_pMidiout;

  uint8_t   m_quarterNote;
  uint16_t  m_delayEven;
  uint16_t  m_delayOdd;
  
  bool      m_update;
  
};

} // namespace sl
