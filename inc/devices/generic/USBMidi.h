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
  
  void onNoteOff(NoteOff msg) const override;
  void onNoteOn(NoteOn msg) const override;
  void onPolyPressure(PolyPressure msg) const override;
  void onControlChange(ControlChange msg) const override;
  void onProgramChange(ProgramChange msg) const override;
  void onChannelPressure(ChannelPressure msg) const override;
  void onPitchBend(PitchBend msg) const override;
  
  bool sendSysex(const SysEx&);

private:


};
  
//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
