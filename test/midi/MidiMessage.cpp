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

namespace sl
{
namespace midi
{
namespace test
{

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Test MidiMessage", "[midi]") {

  NoteOff msgNoteOff(MidiMessage::Channel::Ch13, 120,64);
  CHECK(msgNoteOff.getChannel() == MidiMessage::Channel::Ch13);
  CHECK(msgNoteOff.getNote() == 120);
  CHECK(msgNoteOff.getVelocity() == 64);

  tPtr<MidiMessage> msgNoteOffParsed = parseMidiMessage(msgNoteOff.data());
  CHECK(msgNoteOff.getType() == msgNoteOffParsed->getType());

}
 

//----------------------------------------------------------------------------------------------------------------------

} // test
} // midi
} // sl