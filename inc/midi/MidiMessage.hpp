/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <functional>
#include <vector>

#include "util/Types.h"

namespace sl
{
using namespace kio;
namespace midi
{

//--------------------------------------------------------------------------------------------------

#define M_MIDI_BYTE(data) static_cast<uint8_t>(0x7F&data)

//--------------------------------------------------------------------------------------------------

/**
* @defgroup Midi MIDI
* @{
*/

/**
* @defgroup Basics Basics
* @ingroup Midi
* @{
*/

/**
  \class MidiNote
  \brief Class representing a single midi note
*/

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
  
  MidiNote(uint8_t note_)
  {
    m_octave = static_cast<int8_t>(M_MIDI_BYTE(note_) / 12) - 2;
    m_note = static_cast<Name>(note_%12);
  }
  
  bool operator==(const MidiNote& other_) const
  {
    return (m_note == other_.m_note) && (m_octave == other_.m_octave);
  }

  bool operator!=(const MidiNote& other_) const { return !(operator==(other_)); }
  
  void setNote(Name note_){ m_note = note_; }
  
  uint8_t value()
  {
    return static_cast<uint8_t>((m_octave+1)*12 + static_cast<uint8_t>(m_note));
  }
  
private:
  Name      m_note;
  int8_t    m_octave;
};


/**
  \class MidiMessage
  \brief 

  Full MIDI messages specification: http://www.midi.org/techspecs/midimessages.php
*/
class MidiMessage
{
public:

  enum class Type : uint8_t 
  {
    NoteOff         = 0x80, //!<  Sent when a note is released (ended)
    NoteOn          = 0x90, //!<  Sent when a note is depressed (start)
    PolyPressure    = 0xA0, //!<  Most often sent by pressing down on the key after it "bottoms out"
    ControlChange   = 0xB0, //!<  Sent when a controller value changes
    ProgramChange   = 0xC0, //!<  Sent when the patch number changes
    ChannelPressure = 0xD0, //!<  The highest pressure value of all the current depressed keys
    PitchBend       = 0xE0, //!<  Indicates a change in the pitch bender
    SysEx           = 0xF0, //!<  System Exclusive
    MTC             = 0xF1, //!<  MIDI Time Code Quarter Frame
    SongPosition    = 0xF2, //!<  Song Position Pointer
    SongSelect      = 0xF3, //!<  Song Select
    Reserved_0      = 0xF4, //!<  Undefined(Reserved)
    Reserved_1      = 0xF5, //!<  Undefined(Reserved)
    TuneRequest     = 0xF6, //!<  Tune Request
    SysExEnd        = 0xF7, //!<  Terminates a System Exclusive dump
    TimingClock     = 0xF8, //!<  Timing Clock
    Reserved_2      = 0xF9, //!<  Undefined(Reserved)
    Start           = 0xFA, //!<  Start the current sequence playing
    Continue        = 0xFB, //!<  Continue at the point the sequence was Stopped
    Stop            = 0xFC, //!<  Stop the current sequence
    Reserved_3      = 0xFD, //!<  Undefined(Reserved)
    ActiveSensing   = 0xFE, //!<  Active Sensing
    Reset           = 0xFF, //!<  Reset all receivers in the system to power-up status
  };

  enum class Channel : uint8_t
  {
    Ch1,
    Ch2,
    Ch3,
    Ch4,
    Ch5,
    Ch6,
    Ch7,
    Ch8,
    Ch9,
    Ch10,
    Ch11,
    Ch12,
    Ch13,
    Ch14,
    Ch15,
    Ch16,
    Undefined,
  };

  //! Constructor
  /*!
   \param type_  The MidiMessage type
   */
  MidiMessage(Type type_)
    : m_type(type_)
  {}

  
  virtual const tRawData& data() const = 0;
  Type getType() const { return m_type; }

private:

  Type  m_type;

};

//--------------------------------------------------------------------------------------------------

/**
  \class MidiMessage
  \brief Class representing a MIDI message

*/

template<midi::MidiMessage::Type MsgType>
class MidiMessageBase : public midi::MidiMessage
{
public:
  
  //! Constructor
  /*!
   \param data_ The raw message data
  */
  MidiMessageBase(tRawData data_)
    : midi::MidiMessage(MsgType)
    , m_data(std::move(data_))
  {

  }
  
  //! Constructor
  /*!
   \param header_ The raw message header
   \param data_   The raw message data
  */
  MidiMessageBase(const tRawData& header_, const tRawData& data_)
    : midi::MidiMessage(MsgType)
    , m_data(header_)
  {
    m_data.insert(m_data.end(), data_.begin(), data_.end());
  }
  
  //! Constructor
  /*!
   \param channel_  The MIDI channel
   \param data_     The raw message data
  */
  MidiMessageBase(MidiMessage::Channel channel_, const tRawData& data_)
    : midi::MidiMessage(MsgType)
    , m_data{static_cast<uint8_t>((static_cast<uint8_t>(channel_)|static_cast<uint8_t>(getType())))}
  {
    m_data.insert(m_data.end(), data_.begin(), data_.end());
  }

  virtual ~MidiMessageBase() {}

  const tRawData& data() const override { return m_data; }

  MidiMessage::Channel getChannel() const 
  { 
    return (m_data.size() == 0 || getType()<MidiMessage::Type::SysEx)
      ? MidiMessage::Channel::Undefined
      : static_cast<MidiMessage::Channel>(m_data[0] & 0x0F);
  }

  bool operator == (const MidiMessageBase& other_) const
  {
    return (m_data.size() == other_.m_data.size()) && 
           (std::equal(m_data.begin(),m_data.end(),other_.m_data.begin()));
  }

  bool operator != (const MidiMessageBase& other_) const
  {
    return !(operator == (other_));
  }
private:

  tRawData  m_data;
};

/** @} */ // End of group Basics

//--------------------------------------------------------------------------------------------------

/**
* @defgroup ChannelMessages Channel messages
* @ingroup Midi
* @{
*/

/**
  \class NoteOff
  \brief A NoteOff MIDI message
*/

class NoteOff : public midi::MidiMessageBase<midi::MidiMessage::Type::NoteOff>
{
public:

  NoteOff(MidiMessage::Channel channel_, uint8_t note_, uint8_t velocity_)
  : MidiMessageBase(channel_, { M_MIDI_BYTE(note_), M_MIDI_BYTE(velocity_ )})
  {}

  MidiNote getNote() const { return MidiNote(data()[1]); }

  uint8_t getVelocity() const { return data()[2]; }

};

//--------------------------------------------------------------------------------------------------

/**
  \class NoteOn
  \brief A NoteOn MIDI message
*/

class NoteOn : public midi::MidiMessageBase<midi::MidiMessage::Type::NoteOn>
{
public:

  NoteOn(MidiMessage::Channel channel_, uint8_t note_, uint8_t velocity_)
    : MidiMessageBase(channel_, { M_MIDI_BYTE(note_), M_MIDI_BYTE(velocity_) })
  {}

  MidiNote getNote() const { return MidiNote(data()[1]); }

  uint8_t getVelocity() const { return data()[2]; }

};

//--------------------------------------------------------------------------------------------------

/**
  \class PolyPressure
  \brief A PolyPressure MIDI message
*/

class PolyPressure : public midi::MidiMessageBase<midi::MidiMessage::Type::PolyPressure>
{
public:

  PolyPressure(MidiMessage::Channel channel_, uint8_t note_, uint8_t pressure_)
    : MidiMessageBase(channel_, { M_MIDI_BYTE(note_), M_MIDI_BYTE(pressure_) })
  {}

  uint8_t getNote() const { return data()[1]; }

  uint8_t getPressure() const { return data()[2]; }
};

//--------------------------------------------------------------------------------------------------

/**
  \class ControlChange
  \brief A ControlChange MIDI message
*/

class ControlChange : public midi::MidiMessageBase<midi::MidiMessage::Type::ControlChange>
{
public:

  ControlChange(MidiMessage::Channel channel_, uint8_t control_, uint8_t value_)
    : MidiMessageBase(channel_, { M_MIDI_BYTE(control_), M_MIDI_BYTE(value_) })
  {}

  uint8_t getControl() const { return data()[1]; }

  uint8_t getValue() const { return data()[2]; }

};

//--------------------------------------------------------------------------------------------------

/**
  \class ProgramChange
  \brief A ProgramChange MIDI message
*/

class ProgramChange : public midi::MidiMessageBase<midi::MidiMessage::Type::ProgramChange>
{
public:

  ProgramChange(MidiMessage::Channel channel_, uint8_t program_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(program_)})
  {}

  uint8_t getProgram() const { return data()[1]; }

};

//--------------------------------------------------------------------------------------------------

/**
  \class ChannelPressure
  \brief A ChannelPressure MIDI message
*/

class ChannelPressure : public midi::MidiMessageBase<midi::MidiMessage::Type::ChannelPressure>
{
public:

  ChannelPressure(MidiMessage::Channel channel_, uint8_t pressure_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(pressure_)})
  {}

  uint8_t getPressure() const { return data()[1]; }

};

//--------------------------------------------------------------------------------------------------

/**
  \class PitchBend
  \brief A PitchBend MIDI message
*/

class PitchBend : public midi::MidiMessageBase<midi::MidiMessage::Type::PitchBend>
{
public:

  PitchBend(MidiMessage::Channel channel_, uint8_t pitchL_, uint8_t pitchH_)
    : MidiMessageBase(channel_, { M_MIDI_BYTE(pitchL_), M_MIDI_BYTE(pitchH_) })
  {}

  PitchBend(MidiMessage::Channel channel_, uint16_t pitch_)
    : MidiMessageBase(channel_, { M_MIDI_BYTE(pitch_), M_MIDI_BYTE(pitch_ >> 7) })
  {}

  uint16_t getPitch() const
  {
    return static_cast<uint16_t>(data()[1] | (data()[2] << 7));
  }
};

/** @} */ // End of group ChannelMessages

//--------------------------------------------------------------------------------------------------

/**
* @defgroup SystemMessages System messages
* @ingroup Midi
* @{
*/

/**
  \class SysEx
  \brief A SysEx MIDI message
*/

class SysEx : public midi::MidiMessageBase<midi::MidiMessage::Type::SysEx>
{
public:

  SysEx(const tRawData& data_)
    : MidiMessageBase(data_)
  {
    
  }

  SysEx(uint8_t manufacturerId, const tRawData& data_)
    : MidiMessageBase({manufacturerId},data_)
  {
    
  }

  SysEx(uint8_t manufacturerIdHi, uint8_t manufacturerIdLo, const tRawData& data_)
    : MidiMessageBase({0, manufacturerIdHi, manufacturerIdLo},data_)
  {
    
  }
  
  SysEx(const tRawData& header_, const tRawData& data_)
    : MidiMessageBase(header_,data_)
  {
    
  }
  
  tRawData getHeader() const
  {
    return tRawData(data().begin()+1,data().begin()+1+getManufacturerIdLength());
  }
  
  
  tRawData getPayload() const
  {
    size_t headerLength = getManufacturerIdLength() + 1;
    size_t payloadLength = data().size() - headerLength - ((data().back() == 0xF7) ? 1 : 0);
    
    return tRawData(data().begin()+headerLength,data().begin()+headerLength+payloadLength);
  }
  
  bool isUniversal() const
  {
    return (data()[1] == 0x7E || data()[1] == 0x7F);
  }


private:

  size_t getManufacturerIdLength() const
  {
    return (data()[1] == 0) ? 3 : 1;
  }
  
};

/** @} */ // End of group SystemMessages

//--------------------------------------------------------------------------------------------------

/**
* @defgroup Utilities Utilities
* @ingroup Midi
* @{
*/

/**
  \class MidiMessageListener
  \brief A general purpose MIDI message listener class
*/

class MidiMessageListener
{
public:

  using tCbNoteOff          = std::function<void(tPtr<NoteOff>)>;
  using tCbNoteOn           = std::function<void(tPtr<NoteOn>)>;
  using tCbPolyPressure     = std::function<void(tPtr<PolyPressure>)>;
  using tCbControlChange    = std::function<void(tPtr<ControlChange>)>;
  using tCbProgramChange    = std::function<void(tPtr<ProgramChange>)>;
  using tCbChannelPressure  = std::function<void(tPtr<ChannelPressure>)>;
  using tCbPitchBend        = std::function<void(tPtr<PitchBend>)>;
  using tCbSysEx            = std::function<void(tPtr<SysEx>)>;

  static void process(double timeStamp_, std::vector<unsigned char> *pMessage_, void *pUserData_)
  {
  	if(nullptr == pMessage_ || nullptr == pUserData_)
  	{
      return;
  	}
  	
    MidiMessageListener* pSelf = static_cast<MidiMessageListener*>(pUserData_);
    pSelf->process({pMessage_->begin(),pMessage_->end()});
  }

  void process(const tRawData& data_)
  {
    tPtr<MidiMessage> message = parseMidiMessage(data_);
    if (message)
    {
    
#define M_MESSAGE_CB(idMsg)       \
        case MidiMessage::Type::idMsg:                      \
        {                                                   \
          idMsg* tmp = dynamic_cast<idMsg*>(message.get()); \
          tPtr<idMsg> derivedPointer;                       \
          if(tmp != nullptr)                                \
          {                                                 \
              message.release();                            \
              derivedPointer.reset(tmp);                    \
              cb##idMsg(std::move(derivedPointer));         \
          }                                                 \
          break;                                            \
        }
      
      switch (message->getType())
      {
        M_MESSAGE_CB(NoteOff);
        M_MESSAGE_CB(NoteOn);
        M_MESSAGE_CB(PolyPressure);
        M_MESSAGE_CB(ControlChange);
        M_MESSAGE_CB(ProgramChange);
        M_MESSAGE_CB(ChannelPressure);
        M_MESSAGE_CB(PitchBend);
        M_MESSAGE_CB(SysEx);
      default:
        break;
      }
#undef M_MESSAGE_CB
    }
  }

  virtual ~MidiMessageListener() {}

  void setCallbackNoteOff(tCbNoteOff cbNoteOff_) { m_cbNoteOff = cbNoteOff_; }

  void setCallbackNoteOn(tCbNoteOn cbNoteOn_) { m_cbNoteOn = cbNoteOn_; }

  void setCallbackPolyPressure(tCbPolyPressure cbPolyPressure_)
  { 
    m_cbPolyPressure = cbPolyPressure_;
  }

  void setCallbackControlChangee(tCbControlChange cbControlChange_)
  {
    m_cbControlChange = cbControlChange_;
  }

  void setCallbackProgramChange(tCbProgramChange cbProgramChange_)
  {
    m_cbProgramChange = cbProgramChange_;
  }

  void setCallbackChannelPressure(tCbChannelPressure cbChannelPressure_)
  {
    m_cbChannelPressure = cbChannelPressure_;
  }

  void setCallbackPitchBend(tCbPitchBend cbPitchBend_) { m_cbPitchBend = cbPitchBend_; }

  void setCallbackSysEx(tCbSysEx cbSysEx_) { m_cbSysEx = cbSysEx_; }

  void cbNoteOff(tPtr<NoteOff> msg_)
  { 
    if(m_cbNoteOff)
    { 
      m_cbNoteOff(std::move(msg_)); 
    } 
  }

  void cbNoteOn(tPtr<NoteOn> msg_)
  {
    if(m_cbNoteOn)
    {
      m_cbNoteOn(std::move(msg_));
    }
  }

  void cbPolyPressure(tPtr<PolyPressure> msg_)
  {
    if(m_cbPolyPressure)
    {
      m_cbPolyPressure(std::move(msg_));
    }
  }

  void cbControlChange(tPtr<ControlChange> msg_)
  {
    if(m_cbControlChange)
    {
      m_cbControlChange(std::move(msg_));
    }
  }

  void cbProgramChange(tPtr<ProgramChange> msg_)
  {
    if(m_cbProgramChange)
    {
      m_cbProgramChange(std::move(msg_));
    }
  }

  void cbChannelPressure(tPtr<ChannelPressure> msg_)
  {
    if(m_cbChannelPressure)
    {
      m_cbChannelPressure(std::move(msg_));
    }
  }

  void cbPitchBend(tPtr<PitchBend> msg_)
  {
    if(m_cbPitchBend)
    {
      m_cbPitchBend(std::move(msg_));
    }
  }
  
  void cbSysEx(tPtr<SysEx> msg_)
  {
    if(m_cbSysEx)
    {
      m_cbSysEx(std::move(msg_));
    }
  }

private:

  tPtr<MidiMessage> parseMidiMessage(const tRawData& data_)
  {
    size_t length = data_.size();
    if (length < 1)
    {
      return nullptr;
    }
    uint8_t status = data_[0];
    if ((status  < 0x80) ||
      (status == 0xF4) ||
      (status == 0xF5) ||
      (status == 0xF9) ||
      (status == 0xFD))
    {
      return nullptr;
    }
    else if (status < 0xF0)
    {
      MidiMessage::Type type = static_cast<MidiMessage::Type>(status & 0xF0);
      MidiMessage::Channel channel = static_cast<MidiMessage::Channel>(status & 0x0F);

#define M_CHANNEL_MSG_2(idMsg)     \
  case MidiMessage::Type::idMsg:   \
    return length > 1 ? tPtr<idMsg>(new idMsg(channel, data_[1])) : nullptr;
#define M_CHANNEL_MSG_3(idMsg)     \
  case MidiMessage::Type::idMsg:   \
    return length > 2 ? tPtr<idMsg>(new idMsg(channel, data_[1], data_[2])) : nullptr;

      switch (type)
      {
        M_CHANNEL_MSG_3(NoteOff);
        M_CHANNEL_MSG_3(NoteOn);
        M_CHANNEL_MSG_3(PolyPressure);
        M_CHANNEL_MSG_3(ControlChange);
        M_CHANNEL_MSG_2(ProgramChange);
        M_CHANNEL_MSG_2(ChannelPressure);
        M_CHANNEL_MSG_3(PitchBend);
      default:
        return nullptr;
      }
#undef M_CHANNEL_MSG_2
#undef M_CHANNEL_MSG_3
    }
    else
    {
      MidiMessage::Type type = static_cast<MidiMessage::Type>(status);
      if(type == MidiMessage::Type::Reserved_0 ||
        type == MidiMessage::Type::Reserved_1 ||
        type == MidiMessage::Type::Reserved_2 ||
        type == MidiMessage::Type::Reserved_3
        )
      {
        return nullptr;
      }
      switch (type)
      {
      case MidiMessage::Type::SysEx:
      {
        return length > 2 ? tPtr<SysEx>(new SysEx(data_)) : nullptr;
      }
      default:
        return nullptr;
      }
    }

  }

  tCbNoteOff          m_cbNoteOff;
  tCbNoteOn           m_cbNoteOn;
  tCbPolyPressure     m_cbPolyPressure;
  tCbControlChange    m_cbControlChange;
  tCbProgramChange    m_cbProgramChange;
  tCbChannelPressure  m_cbChannelPressure;
  tCbPitchBend        m_cbPitchBend;
  tCbSysEx            m_cbSysEx;
};
/** @} */ // End of group Utilities
/** @} */ // End of group Midi
  
//--------------------------------------------------------------------------------------------------

#undef M_MIDI_BYTE

} // midi
} // sl
