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

#include "comm/DeviceHandle.h"
#include "comm/DeviceHandleImpl.h"

namespace sl
{
namespace kio
{
  
//----------------------------------------------------------------------------------------------------------------------
 
DeviceHandle::DeviceHandle( tPtr<DeviceHandleImpl> pDeviceHandle_ )
  :m_pImpl(std::move(pDeviceHandle_))
{

}

//----------------------------------------------------------------------------------------------------------------------

void DeviceHandle::disconnect()
{
  m_pImpl->disconnect();
}

//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandle::read( Transfer& transfer_, uint8_t endpoint_ )
{
  return m_pImpl->read( transfer_, endpoint_ );
}
  
//----------------------------------------------------------------------------------------------------------------------

bool DeviceHandle::write(const Transfer& transfer_, uint8_t endpoint_ ) const
{
  return m_pImpl->write( transfer_, endpoint_ );
}

//----------------------------------------------------------------------------------------------------------------------

} // kio
} // sl
