/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "devices/Device.h"
#include "gfx/displays/LCDDisplayGeneric.h"

#include <unMIDIfy.hpp>

namespace sl
{
namespace cabl
{

using namespace midi;

namespace devices
{

//--------------------------------------------------------------------------------------------------

class USBMidi : public midi::Unmidifier, public Device
{

public:

  void onNoteOff(NoteOff msg) override {}
  void onNoteOn(NoteOn msg) override {}
  void onPolyPressure(PolyPressure msg) override {}
  void onControlChange(ControlChange msg) override {}
  void onProgramChange(ProgramChange msg) override {}
  void onChannelPressure(ChannelPressure msg) override {}
  void onPitchBend(PitchBend msg) override {}
  void onClock(Clock msg_) override {}
  void onSysEx(SysEx msg_) override {}
  void onUSysExRT(USysExRT msg_) override {}
  void onUSysExNonRT(USysExNonRT msg_) override {}

  bool sendSysex(const SysEx&);

private:


};

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
