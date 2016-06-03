/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "client/Client.h"

#include "devices/ableton/Push2.h"
#include "devices/ableton/Push2Display.h"
#include "devices/akai/Push.h"
#include "devices/ni/KompleteKontrol.h"
#include "devices/ni/MaschineMK1.h"
#include "devices/ni/MaschineMK2.h"
#include "devices/ni/MaschineMikroMK2.h"
#include "devices/ni/TraktorF1MK2.h"

namespace sl
{
namespace cabl
{

using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

Client::Client()
  : m_coordinator(std::bind(&Client::devicesListChanged,this,std::placeholders::_1))
{
  m_coordinator.run();
}

//--------------------------------------------------------------------------------------------------

void Client::onInitDevice()
{
  M_LOG("[Client] onInitDevice" );
  
  if(!m_pDevice)
  {
    return;
  }
  
  for(size_t i = 0; i < m_pDevice->numOfGraphicDisplays() ; i++)
  {
    m_pDevice->getGraphicDisplay(i)->black();
  }
  
  for(size_t i = 0; i < m_pDevice->numOfLCDDisplays() ; i++)
  {
    m_pDevice->getLCDDisplay(i)->clear();
  }
  
  m_pDevice->setCallbackRender(std::bind(&Client::onRender, this));
  
  m_pDevice->setCallbackButtonChanged(std::bind(&Client::buttonChanged, this, _1, _2, _3));
  m_pDevice->setCallbackEncoderChanged(std::bind(&Client::encoderChanged, this, _1, _2, _3));
  m_pDevice->setCallbackPadChanged(std::bind(&Client::padChanged, this, _1, _2, _3));
  m_pDevice->setCallbackKeyChanged(std::bind(&Client::keyChanged, this, _1, _2, _3));
  
  initDevice();
  
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::onRender()
{
  bool expected = true;
  if(m_update.compare_exchange_weak(expected, false) && m_pDevice)
  {
    render();
  }
  else
  {
    std::this_thread::yield();
  }
}

//--------------------------------------------------------------------------------------------------

void Client::buttonChanged(Device::Button button_, bool buttonState_, bool shiftPressed_)
{
  M_LOG("[Client] encoderChanged " << static_cast<int>(button_) << " (" << ( buttonState_ ? "clicked " : "released" ) << ") " << ( shiftPressed_ ? " SHIFT" : "" ));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::encoderChanged(Device::Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
  M_LOG("[Client] encoderChanged " << static_cast<int>(encoder_) << ( valueIncreased_ ? "++ " : "--" ) << ") " << ( shiftPressed_ ? " SHIFT" : "" ));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::padChanged(Device::Pad pad_, uint16_t value_, bool shiftPressed_)
{
  M_LOG("[Client] padChanged " << static_cast<int>(pad_) << " (" << value_ << ") " << ( shiftPressed_ ? " SHIFT" : "" ));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::keyChanged(Device::Key key_, uint16_t value_, bool shiftPressed_)
{
  M_LOG("[Client] keyChanged " << static_cast<int>(key_) << " (" << value_ << ") " << ( shiftPressed_ ? " SHIFT" : "" ));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::devicesListChanged(Coordinator::tCollDeviceDescriptor devices_)
{
  M_LOG("[Client] devicesListChanged : " << devices_.size() << " devices" );
  
  if(m_pDevice)
  {
    if(!m_pDevice->hasDeviceHandle())
    {
      if(devices_.size() > 0)
      {
        m_pDevice = m_coordinator.connect(devices_[0]);
        onInitDevice();
      }
    }
  }
  else
  {
    if(devices_.size() > 0)
    {
      m_pDevice = m_coordinator.connect(devices_[0]);
      onInitDevice();
    }
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
