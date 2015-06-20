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

#include <algorithm>
#include <vector>

class MidiNote
{
public:  
  enum class Name : uint8_t {
    C,
    CSharp, DFlat = CSharp,
    D,
    DSharp, EFlat = DSharp,
    E,
    F,
    FSharp, GFlat = FSharp,
    G,
    GSharp, AFlat = GSharp,
    A,
    ASharp, BFlat = ASharp,
    B,
  };
  
  MidiNote(Name note_, int8_t octave_)
  : m_note(note_)
  , m_octave(std::min<int8_t>(9,std::max<int8_t>(-1,octave_)))
  {
  
  }
  
  void setNote(Name note_){ m_note = note_; }
  
  uint8_t value()
  {
    return (m_octave+1)*12 + static_cast<uint8_t>(m_note);
  }
  
private:
  Name      m_note;
  int8_t    m_octave;
};
  
class MidiMessage
{
public:

  enum class Type : uint8_t {
    NoteOff = 0x80,
    NoteOn = 0x90,
    PolyPressure = 0xa0,
    ControlChange = 0xb0,
    ProgramChange = 0xc0,
    ChannelPressure = 0xd0,
    PitchBend = 0xe0,
    SysexStart = 0xf0,
    MTC = 0xf1,
    SongPosition = 0xf2,
    SongSelect = 0xf3,
    TuneRequest = 0xf6,
    SysexEnd = 0xf7,
    Reset = 0xff,
  };

  static std::vector<uint8_t> noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
  {
    return {channel&0x0Fu|static_cast<uint8_t>(Type::NoteOn), note&0x7Fu, velocity&0x7Fu};
  }

};
