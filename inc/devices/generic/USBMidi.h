/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "devices/Device.h"
#include "gfx/displays/LCDDisplayGeneric.h"

#include "midi/MidiMessage.hpp"

namespace sl
{
namespace kio
{

using namespace midi;

namespace devices
{

//--------------------------------------------------------------------------------------------------

class USBMidi : public midi::MidiMessageListener, public Device
{
 
public:
  
  USBMidi(tPtr<DeviceHandle>);
  
  void onNoteOff(tPtr<NoteOff> msg);
  void onNoteOn(tPtr<NoteOn> msg);
  void onPolyPressure(tPtr<PolyPressure> msg);
  void onControlChange(tPtr<ControlChange> msg);
  void onProgramChange(tPtr<ProgramChange> msg);
  void onChannelPressure(tPtr<ChannelPressure> msg);
  void onPitchBend(tPtr<PitchBend> msg);
  
  bool sendSysex(const SysEx&);

private:


};
  
//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
