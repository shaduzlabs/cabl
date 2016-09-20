/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "DriverMIDI.h"

#include <future>

#include "DeviceHandleMIDI.h"

using namespace std::placeholders;

namespace sl
{
namespace cabl
{

using namespace midi;

//--------------------------------------------------------------------------------------------------

DriverMIDI::DriverMIDI()
{
  M_LOG("[DriverMIDI] initialization");
}

//--------------------------------------------------------------------------------------------------

DriverMIDI::~DriverMIDI()
{
  M_LOG("[DriverMIDI] exit");
}

//--------------------------------------------------------------------------------------------------

Driver::tCollDeviceDescriptor DriverMIDI::enumerate()
{
  M_LOG("[DriverMIDI] enumerate");
  Driver::tCollDeviceDescriptor collDevices;
  std::string portNameIn, portNameOut;
  RtMidiIn midiIn;
  RtMidiOut midiOut;
  std::mutex mtxDevices;

  std::vector<std::future<void>> pendingFutures;

  unsigned nPortsOut = midiOut.getPortCount();
  unsigned nPortsIn = midiIn.getPortCount();

  for (unsigned int iOut = 0; iOut < nPortsOut; iOut++)
  {
    try
    {
      portNameOut = midiOut.getPortName(iOut);
      if (portNameOut != "")
      {
        for (unsigned int iIn = 0; iIn < nPortsIn; iIn++)
        {
          try
          {
            portNameIn = midiIn.getPortName(iIn);
            if (portNameIn == portNameOut)
            {
              M_LOG("[DriverMIDI] out: " << portNameOut << " ->" << iOut);
              M_LOG("[DriverMIDI] in: " << portNameIn << " ->" << iIn);
              auto f = std::async(
                std::launch::async, [this, &mtxDevices, &collDevices, iIn, iOut]() {
                  bool received = false;
                  bool timeout = false;
                  RtMidiIn _midiIn;
                  RtMidiOut _midiOut;
                  std::vector<unsigned char> recv;
                  std::vector<unsigned char> sysExIdentity = {0xF0, 0x7E, 0x00, 0x06, 0x01, 0xF7};

                  _midiIn.openPort(iIn);
                  _midiIn.ignoreTypes(false);

                  _midiOut.openPort(iOut);
                  _midiOut.sendMessage(&sysExIdentity);

                  auto start = std::chrono::system_clock::now();
                  while (!received && !timeout)
                  {
                    _midiIn.getMessage(&recv);
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now() - start);
                    if (recv.size() > 0)
                    {
                      if (recv[0] == 0xF0 && recv[1] == 0x7E && recv[3] == 0x06 && recv[4] == 0x02)
                      {
                        received = true;
                        unsigned vendorId = recv[5];
                        unsigned productId = (recv[6] << 8) | recv[7];
                        std::lock_guard<std::mutex> lock(mtxDevices);
                        collDevices.emplace_back(_midiIn.getPortName(iIn),
                          DeviceDescriptor::Type::MIDI,
                          vendorId,
                          productId,
                          "",
                          iIn,
                          iOut);
                        M_LOG("[DriverMIDI] found device: " << vendorId << ":" << productId);
                      }
                    }
                    else if (duration > std::chrono::milliseconds(500))
                    {
                      timeout = true;
                      M_LOG("[DriverMIDI] identity reply timeout on port #" << iOut);
                    }
                  }
                  _midiIn.closePort();
                  _midiOut.closePort();
                });
              pendingFutures.push_back(std::move(f));
            }
          }
          catch (RtMidiError& error)
          {
			std::string strError(error.getMessage());
            M_LOG("[DriverMIDI] RtMidiError: " << strError);
          }
        }
      }
    }
    catch (RtMidiError& error)
    {
	  std::string strError(error.getMessage());
      M_LOG("[DriverMIDI] RtMidiError: " << strError);
    }
  }
  return collDevices;
}

//--------------------------------------------------------------------------------------------------

tPtr<DeviceHandleImpl> DriverMIDI::connect(const DeviceDescriptor& device_)
{
  M_LOG("[DriverMIDI] connecting to " << device_.name() << ":" << device_.vendorId() << ":"
                                      << device_.productId());
  try
  {
    return tPtr<DeviceHandleImpl>(new DeviceHandleMIDI(device_));
  }
  catch (RtMidiError& error)
  {
	std::string strError(error.getMessage());
    M_LOG("[DriverMIDI] RtMidiError: " << strError);
    return nullptr;
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
