/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <algorithm>
#include <unmidify.hpp>

#include "cabl/devices/Device.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

using namespace midi;

//--------------------------------------------------------------------------------------------------

class USBMidi : public midi::Unmidify, public Device
{

public:
  void onNoteOff(NoteOff /*msg_*/) override
  {
  }
  void onNoteOn(NoteOn /*msg_*/) override
  {
  }
  void onPolyPressure(PolyPressure /*msg_*/) override
  {
  }
  void onControlChange(ControlChange /*msg_*/) override
  {
  }
  void onProgramChange(ProgramChange /*msg_*/) override
  {
  }
  void onChannelPressure(ChannelPressure /*msg_*/) override
  {
  }
  void onPitchBend(PitchBend /*msg_*/) override
  {
  }
  void onClock(Clock /*msg_*/) override
  {
  }
  void onSysEx(SysEx /*msg_*/) override
  {
  }
  void onUSysExRT(USysExRT /*msg_*/) override
  {
  }
  void onUSysExNonRT(USysExNonRT /*msg_*/) override
  {
  }

  bool sendSysex(const SysEx&);

private:
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
