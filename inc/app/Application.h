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

#include <map>

#include "comm/Driver.h"
#include "devices/Device.h"
#include "midi/MidiMessage.h"

namespace sl
{
namespace kio
{

//----------------------------------------------------------------------------------------------------------------------

class Application
{
public:
  using tCollDevices = std::vector<tPtr<Device>>;
  using tCollDrivers = std::map<Driver::Type, tPtr<Driver>>;
  Application(const Driver::tCollDeviceDescriptor&);

  virtual bool tick() = 0;
  virtual bool initHardware() = 0;

  void run();
  Driver::tCollDeviceDescriptor enumerateDevices();
  bool connect(Driver::tCollDeviceDescriptor);
  
  void setMaxConsecutiveErrors(unsigned nErrors_){ m_maxConsecutiveErrors = nErrors_;}

protected:
  void setConnected(bool connected_)
  {
    m_connected = connected_;
  };
  Device* getDevice(unsigned index)
  {
    return m_collDevices[index].get();
  }
  Driver* getDriver(Driver::Type);
  
private:
  bool isKnownDevice(const DeviceDescriptor&) const;
  bool isSupportedDevice(const DeviceDescriptor&) const;

  bool m_appStopped;
  bool m_connected;

  unsigned                      m_maxConsecutiveErrors;
  
  Driver::tCollDeviceDescriptor m_collKnownDevices;
  Driver::tCollDeviceDescriptor m_collSupportedDevices;

  tCollDevices m_collDevices;
  tCollDrivers m_collDrivers;
};

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
