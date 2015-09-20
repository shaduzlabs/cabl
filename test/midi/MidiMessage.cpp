/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <midi/MidiMessage.hpp>
#include <util/Log.h>

using namespace std::placeholders;

namespace sl
{
namespace midi
{
namespace test
{

class TestMidiListenerCallbacks : public MidiMessageListener
{
public:
  TestMidiListenerCallbacks()
  {
    setCallbackNoteOff(std::bind(&TestMidiListenerCallbacks::noteOff, this, _1));
    setCallbackNoteOn(std::bind(&TestMidiListenerCallbacks::noteOn, this, _1));
    setCallbackPolyPressure(std::bind(&TestMidiListenerCallbacks::polyPressure, this, _1));
    setCallbackControlChangee(std::bind(&TestMidiListenerCallbacks::controlChange, this, _1));
    setCallbackProgramChange(std::bind(&TestMidiListenerCallbacks::programChange, this, _1));
    setCallbackChannelPressure(std::bind(&TestMidiListenerCallbacks::channelPressure, this, _1));
    setCallbackPitchBend(std::bind(&TestMidiListenerCallbacks::pitchBend, this, _1));
  }

  void noteOff(tPtr<NoteOff> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::NoteOff);
    MidiNote note(MidiNote::Name::CSharp, 6);
    CHECK(note == msg_->getNote());
    CHECK(0x41 == msg_->getVelocity());
    CHECK(3    == msg_->data().size());
  }

  void noteOn(tPtr<NoteOn> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::NoteOn);
    MidiNote note(MidiNote::Name::D, 6);
    CHECK(note == msg_->getNote());
    CHECK(0x42 == msg_->getVelocity());
    CHECK(3    == msg_->data().size());
  }

  void polyPressure(tPtr<PolyPressure> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::PolyPressure);
    CHECK(0x63 == msg_->getNote());
    CHECK(0x43 == msg_->getPressure());
    CHECK(3   == msg_->data().size());
  }

  void controlChange(tPtr<ControlChange> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::ControlChange);
    CHECK(0x64 == msg_->getControl());
    CHECK(0x44 == msg_->getValue());
    CHECK(3   == msg_->data().size());
  }

  void programChange(tPtr<ProgramChange> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::ProgramChange);
    CHECK(0x65 == msg_->getProgram());
    CHECK(2    == msg_->data().size());
  }

  void channelPressure(tPtr<ChannelPressure> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::ChannelPressure);
    CHECK(0x66 == msg_->getPressure());
    CHECK(2    == msg_->data().size());
  }
  
  void pitchBend(tPtr<PitchBend> msg_)
  {
    CHECK(msg_->getType() == MidiMessage::Type::PitchBend);
    CHECK(0x8FF == msg_->getPitch());
    CHECK(3     == msg_->data().size());
  }
};

//--------------------------------------------------------------------------------------------------

TEST_CASE("Test MidiMessageListener callbacks", "[midi]")
{

  TestMidiListenerCallbacks midiListener;

  NoteOff msgNoteOff(MidiMessage::Channel::Ch1, 0x61,0x41);
  midiListener.process(msgNoteOff.data());

  NoteOn msgNoteOn(MidiMessage::Channel::Ch2, 0x62,0x42);
  midiListener.process(msgNoteOn.data());
  
  PolyPressure msgPolyPressure(MidiMessage::Channel::Ch3, 0x63,0x43);
  midiListener.process(msgPolyPressure.data());

  ControlChange msgControlChange(MidiMessage::Channel::Ch4, 0x64,0x44);
  midiListener.process(msgControlChange.data());

  ProgramChange msgProgramChange(MidiMessage::Channel::Ch5, 0x65 );
  midiListener.process(msgProgramChange.data());

  ChannelPressure msgChannelPressure(MidiMessage::Channel::Ch6, 0x66 );
  midiListener.process(msgChannelPressure.data());

  PitchBend msgPitchBend(MidiMessage::Channel::Ch7, 0x7F, 0x11 );
  midiListener.process(msgPitchBend.data());
}
 

//--------------------------------------------------------------------------------------------------

TEST_CASE("Test SysEx messages", "[midi]")
{
  // 0x00 0x21 0x02 - Mutable Instruments
  SysEx msgSysEx_1({0xF0,0x00,0x21,0x02,0x00,0x02,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xf7});
  SysEx msgSysEx_2({0xF0,0x00,0x21,0x02,0x00,0x02,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09});
  
  CHECK(3  == msgSysEx_1.getHeader().size());
  CHECK(11 == msgSysEx_1.getPayload().size());
  
  CHECK(3  == msgSysEx_2.getHeader().size());
  CHECK(11 == msgSysEx_2.getPayload().size());

  CHECK(msgSysEx_1.getPayload().size() == msgSysEx_2.getPayload().size());

  // 0x41 - Roland Corporation
  SysEx msgSysEx_3({0xF0,0x41,0x00,0x02,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xf7});
  CHECK(1  == msgSysEx_3.getHeader().size());
  CHECK(11 == msgSysEx_3.getPayload().size());

}

//--------------------------------------------------------------------------------------------------

} // test
} // midi
} // sl