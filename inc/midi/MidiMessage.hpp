/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <functional>
#include <vector>

namespace sl
{

namespace midi
{

using tRawData = std::vector<uint8_t>;

//--------------------------------------------------------------------------------------------------

#define M_MIDI_BYTE(data) static_cast<uint8_t>(0x7F&data)
#define M_UINT8(data) static_cast<uint8_t>(data)

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
    return M_UINT8((m_octave+1)*12 + M_UINT8(m_note));
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

protected:

  virtual tRawData& _data() = 0;

private:
   
  friend class MidiMessageListener;
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
    , m_data{M_UINT8((M_UINT8(channel_)|M_UINT8(getType())))}
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

protected:

  virtual tRawData& _data() override { return m_data; }
  
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

  NoteOff(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}

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

  NoteOn(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  PolyPressure(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  ControlChange(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  ProgramChange(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  ChannelPressure(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  PitchBend(tRawData data_)
  : MidiMessageBase(std::move(data_))
  {}
  
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

  SysEx(tRawData data_)
    : MidiMessageBase(std::move(data_))
  {
    checkAndSetStartAndStopBytes();
  }
  
  SysEx(const tRawData& header_, const tRawData& data_)
    : MidiMessageBase(header_,data_)
  {
    checkAndSetStartAndStopBytes();
  }
  
  SysEx(uint8_t manufacturerId, const tRawData& data_)
    : SysEx({0xF0, manufacturerId},data_)
  {
  }

  SysEx(uint8_t manufacturerIdHi, uint8_t manufacturerIdLo, const tRawData& data_)
    : SysEx({0xF0, 0, manufacturerIdHi, manufacturerIdLo},data_)
  {
  }

  
  tRawData getHeader() const
  {
    return tRawData(data().begin()+1,data().begin()+1+getHeaderLength());
  }
  
  
  tRawData getPayload() const
  {
    size_t headerLength = getManufacturerIdLength() + 1;
    size_t payloadLength = data().size() - headerLength - ((data().back() == 0xF7) ? 1 : 0);
    
    return tRawData(data().begin()+headerLength,data().begin()+headerLength+payloadLength);
  }

  virtual size_t getHeaderLength() const
  {
    return getManufacturerIdLength();
  }
  
private:

  size_t getManufacturerIdLength() const
  {
    return (data()[1] == 0) ? 3 : 1;
  }
  
  void checkAndSetStartAndStopBytes()
  {
    if(_data().front() != 0xF0)
    {
      _data().insert(data().begin(),0xF0);
    }
    if(_data().back() != 0xF7)
    {
      _data().push_back(0xF7);
    }
  }
  
};

//--------------------------------------------------------------------------------------------------

/**
  \class USysExNonRT
  \brief An Universal SysEx Non-RealTime MIDI message
*/

class USysExNonRT : public SysEx
{
public:

  enum class MessageId{
    SampleDumpHeader          = 0x01, //!<
    SampleDumpData            = 0x02, //!<
    SampleDumpRequest         = 0x03, //!<
    MIDITimeCode              = 0x04, //!<
    SampleDumpExtensions      = 0x05, //!<
    GeneralInformation        = 0x06, //!<
    FileDump                  = 0x07, //!<
    MIDITuning                = 0x08, //!<
    GeneralMIDI               = 0x09, //!<
    DownloadableSounds        = 0x0A, //!<
    FileReferenceMessage      = 0x0B, //!<
    MIDIVisualControl         = 0x0C, //!<

    GenericHandshakingEOF     = 0x7B, //!<
    GenericHandshakingWait    = 0x7C, //!<
    GenericHandshakingCancel  = 0x7D, //!<
    GenericHandshakingNAK     = 0x7E, //!<
    GenericHandshakingACK     = 0x7F, //!<

    Unknown,                          //!< Unknown
  };
  
  USysExNonRT(tRawData data_)
    : SysEx(std::move(data_))
  {
  }
  
  USysExNonRT(const tRawData& header_, const tRawData& data_)
    : SysEx(header_,data_)
  {
  }
  
  USysExNonRT(MessageId msgId_,const tRawData& data_)
    : SysEx({0xF0, 0x7E, 0x00, M_UINT8(msgId_)}, data_)
  {

  }
  
  uint8_t getTargetDevice() const
  {
    return data()[2];
  }
  
  MessageId getMessageId() const
  {
    uint8_t m = data()[3];
    if((m > M_UINT8(MessageId::MIDIVisualControl) && m < M_UINT8(MessageId::GenericHandshakingEOF))
       || m > M_UINT8(MessageId::GenericHandshakingACK))
    {
      return MessageId::Unknown;
    }
    return static_cast<MessageId>(m);
  }
    
  uint8_t getSubId2() const
  {
    return getHeaderLength() == 4 ? data()[4] : 0;
  }
  
private:

  size_t getHeaderLength() const override
  {
    uint8_t m = data()[3];
    if(m<= M_UINT8(MessageId::SampleDumpRequest) ||
       (m >= M_UINT8(MessageId::GenericHandshakingEOF) && m <= M_UINT8(MessageId::GenericHandshakingACK))
    )
    {
      return 3;
    }
    return 4;
  }
  
};

//--------------------------------------------------------------------------------------------------

/**
  \class USysExRT
  \brief An Universal SysEx RealTime MIDI message
*/

class USysExRT : public SysEx
{
public:

  enum class MessageId : uint8_t{
    MIDITimeCode                 = 0x01, //!<
    MIDIShowControl              = 0x02, //!<
    NotationInformation          = 0x03, //!<
    DeviceControl                = 0x04, //!<
    RealTimeMTCCueing            = 0x05, //!<
    MIDIMachineControlCommands   = 0x06, //!<
    MIDIMachineControlResponses  = 0x07, //!<
    MIDITuning                   = 0x08, //!<
    ControllerDestinationSetting = 0x09, //!<
    KeyBasedInstrumentControl    = 0x0A, //!<
    ScalablePoliphonyMIP         = 0x0B, //!<
    MobilePhoneControlMessage    = 0x0C, //!<
    Unknown                              //!<
  };
  
  USysExRT(tRawData data_)
    : SysEx(std::move(data_))
  {
    
  }
  
  USysExRT(const tRawData& header_, const tRawData& data_)
    : SysEx(header_,data_)
  {
    
  }
    
  USysExRT(MessageId msgId_,const tRawData& data_)
    : SysEx({0xF0, 0x7F, 0x00, M_UINT8(msgId_)}, data_)
  {

  }
  
  uint8_t getTargetDevice() const
  {
    return data()[2];
  }
  
  MessageId getMessageId() const
  {
    uint8_t m = data()[3];
    if(m > M_UINT8(MessageId::MobilePhoneControlMessage))
    {
      return MessageId::Unknown;
    }
    return static_cast<MessageId>(m);
  }
  
  uint8_t getSubId2() const
  {
    return data()[4];
  }
  
private:

  size_t getHeaderLength() const override
  {
    return 4;
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
    std::unique_ptr<MidiMessage> message = parseMidiMessage(data_);
    if (message)
    {
    
#define M_MESSAGE_CB(idMsg)                                 \
        case MidiMessage::Type::idMsg:                      \
        {                                                   \
          on##idMsg(std::move(message->_data()));           \
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
        case MidiMessage::Type::SysEx:
        {
          if(message->data()[1] == 0x7E)
          {
            onUSysExNonRT(std::move(message->_data()));
          }
          else if(message->data()[1] == 0x7F)
          {
            onUSysExRT(std::move(message->_data()));
          }
          else
          {
            onSysEx(std::move(message->_data()));
          }
          break;
        }
      default:
        break;
      }
#undef M_MESSAGE_CB
    }
  }

  virtual ~MidiMessageListener() {}

  virtual void onNoteOff(NoteOff msg_) const { }

  virtual void onNoteOn(NoteOn msg_) const { }

  virtual void onPolyPressure(PolyPressure msg_) const { }

  virtual void onControlChange(ControlChange msg_) const { }

  virtual void onProgramChange(ProgramChange msg_) const { }

  virtual void onChannelPressure(ChannelPressure msg_) const { }

  virtual void onPitchBend(PitchBend msg_) const { }
  
  virtual void onSysEx(SysEx msg_) const { }
  virtual void onUSysExNonRT(USysExNonRT msg_) const { }
  virtual void onUSysExRT(USysExRT msg_) const { }

private:

  std::unique_ptr<MidiMessage> parseMidiMessage(const tRawData& data_)
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
    return length > 1 ? std::unique_ptr<idMsg>(new idMsg(channel, data_[1])) : nullptr;
#define M_CHANNEL_MSG_3(idMsg)     \
  case MidiMessage::Type::idMsg:   \
    return length > 2 ? std::unique_ptr<idMsg>(new idMsg(channel, data_[1], data_[2])) : nullptr;

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
        if((length > 2 && data_[1] <  0x7E) ||
           (length > 4 && data_[1] == 0x7E) ||
           (length > 5 && data_[1] == 0x7F)  )
        {
          return std::unique_ptr<SysEx>(new SysEx(data_));
        }
      }
      default:
        return nullptr;
      }
    }

  }
};
/** @} */ // End of group Utilities
/** @} */ // End of group Midi
  
//--------------------------------------------------------------------------------------------------

#undef M_MIDI_BYTE
#undef M_UINT8

} // midi
} // sl
