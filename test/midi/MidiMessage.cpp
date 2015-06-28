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

#include "catch.hpp"

#include <midi/MidiMessage.h>
#include <util/Log.h>

namespace sl
{
namespace midi
{
namespace test
{

class TestMidiListener : public MidiMessageListener
{
public:
  TestMidiListener()
  {
    setCallbackNoteOff(std::bind(&TestMidiListener::noteOff, this, std::placeholders::_1));
    setCallbackNoteOn(std::bind(&TestMidiListener::noteOn, this, std::placeholders::_1));
    setCallbackPolyPressure(std::bind(&TestMidiListener::polyPressure, this, std::placeholders::_1));
    setCallbackControlChangee(std::bind(&TestMidiListener::controlChange, this, std::placeholders::_1));
    setCallbackProgramChange(std::bind(&TestMidiListener::programChange, this, std::placeholders::_1));
    setCallbackChannelPressure(std::bind(&TestMidiListener::channelPressure, this, std::placeholders::_1));
    setCallbackPitchBend(std::bind(&TestMidiListener::pitchBend, this, std::placeholders::_1));
  }

  void noteOff(tPtr<NoteOff> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::NoteOff);
    CHECK(3 == msg->data().size());
  }

  void noteOn(tPtr<NoteOn> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::NoteOn);
    CHECK(3 == msg->data().size());
  }

  void polyPressure(tPtr<PolyPressure> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::PolyPressure);
    CHECK(3 == msg->data().size());
  }

  void controlChange(tPtr<ControlChange> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::ControlChange);
    CHECK(3 == msg->data().size());
  }

  void programChange(tPtr<ProgramChange> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::ProgramChange);
    CHECK(2 == msg->data().size());
  }

  void channelPressure(tPtr<ChannelPressure> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::ChannelPressure);
    CHECK(2 == msg->data().size());
  }
  void pitchBend(tPtr<PitchBend> msg)
  {
    CHECK(msg->getType() == MidiMessage::Type::PitchBend);
    CHECK(3 == msg->data().size());
  }
};

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Test MidiMessage", "[midi]") {

  NoteOff msgNoteOff(MidiMessage::Channel::Ch1, 121,61);
  CHECK(msgNoteOff.getChannel() == MidiMessage::Channel::Ch1);
  CHECK(msgNoteOff.getNote() == 121);
  CHECK(msgNoteOff.getVelocity() == 61);
  TestMidiListener midiListener;

  processMidi(&midiListener, msgNoteOff.data());

  NoteOn msgNoteOn(MidiMessage::Channel::Ch2, 122,62);
  processMidi(&midiListener, msgNoteOn.data());
  
  PolyPressure msgPolyPressure(MidiMessage::Channel::Ch3, 123,61);
  processMidi(&midiListener, msgPolyPressure.data());

  ControlChange msgControlChange(MidiMessage::Channel::Ch4, 124,62);
  processMidi(&midiListener, msgControlChange.data());

  ProgramChange msgProgramChange(MidiMessage::Channel::Ch5, 125 );
  processMidi(&midiListener, msgProgramChange.data());

  ChannelPressure msgChannelPressure(MidiMessage::Channel::Ch6, 126 );
  processMidi(&midiListener, msgChannelPressure.data());

  PitchBend msgPitchBend(MidiMessage::Channel::Ch7, 127, 67 );
  processMidi(&midiListener, msgPitchBend.data());

}
 

//----------------------------------------------------------------------------------------------------------------------

} // test
} // midi
} // sl