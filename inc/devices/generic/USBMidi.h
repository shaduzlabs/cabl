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
namespace devices
{

//--------------------------------------------------------------------------------------------------

class USBMidi : public midi::MidiMessageListener
{
 
public:
  
  bool sendSysex(const midi::SysEx&) const;

private:


};
  
//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
