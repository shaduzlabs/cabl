/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/client/Client.h"

#include "devices/ableton/Push2.h"
#include "devices/ableton/Push2Display.h"
#include "devices/akai/Push.h"
#include "devices/ni/KompleteKontrol.h"
#include "devices/ni/MaschineJam.h"
#include "devices/ni/MaschineMK1.h"
#include "devices/ni/MaschineMK2.h"
#include "devices/ni/MaschineMikroMK1.h"
#include "devices/ni/MaschineMikroMK2.h"
#include "devices/ni/TraktorF1MK2.h"

namespace sl
{
namespace cabl
{

using namespace std::placeholders;

//--------------------------------------------------------------------------------------------------

Client::Client(DiscoveryPolicy discoveryPolicy_)
  : m_clientId(Coordinator::instance().registerClient(
      std::bind(&Client::devicesListChanged, this, std::placeholders::_1)))
  , m_discoveryPolicy(std::move(discoveryPolicy_))
{
  M_LOG("[Client] Client");
}

//--------------------------------------------------------------------------------------------------

Client::~Client()
{
  Coordinator::instance().unregisterClient(m_clientId);
}

//--------------------------------------------------------------------------------------------------

void Client::init()
{
  devicesListChanged(Coordinator::instance().enumerate());
}

//--------------------------------------------------------------------------------------------------

void Client::onInitDevice()
{
  M_LOG("[Client] onInitDevice");

  if (!m_pDevice)
  {
    return;
  }

  for (size_t i = 0; i < m_pDevice->numOfGraphicDisplays(); i++)
  {
    m_pDevice->graphicDisplay(i)->black();
  }

  for (size_t i = 0; i < m_pDevice->numOfTextDisplays(); i++)
  {
    m_pDevice->textDisplay(i)->clear();
  }

  m_pDevice->setCallbackDisconnect(std::bind(&Client::disconnected, this));
  m_pDevice->setCallbackRender(std::bind(&Client::onRender, this));

  m_pDevice->setCallbackButtonChanged(std::bind(&Client::buttonChanged, this, _1, _2, _3));
  m_pDevice->setCallbackEncoderChanged(std::bind(&Client::encoderChanged, this, _1, _2, _3));
  m_pDevice->setCallbackKeyChanged(std::bind(&Client::keyChanged, this, _1, _2, _3));
  m_pDevice->setCallbackControlChanged(std::bind(&Client::controlChanged, this, _1, _2, _3));

  initDevice();

  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::disconnected()
{
  M_LOG("[Client] device disconnected");
}

//--------------------------------------------------------------------------------------------------

void Client::onRender()
{
  bool expected = true;
  if (m_update.compare_exchange_weak(expected, false) && m_pDevice && m_pDevice->hasDeviceHandle())
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
  M_LOG("[Client] encoderChanged " << static_cast<int>(button_) << " ("
                                   << (buttonState_ ? "clicked " : "released")
                                   << ") "
                                   << (shiftPressed_ ? " SHIFT" : ""));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
  M_LOG("[Client] encoderChanged " << static_cast<int>(encoder_) << (valueIncreased_ ? "++ " : "--")
                                   << ") "
                                   << (shiftPressed_ ? " SHIFT" : ""));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
  M_LOG("[Client] keyChanged " << static_cast<int>(index_) << " (" << value_ << ") "
                               << (shiftPressed_ ? " SHIFT" : ""));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::controlChanged(unsigned pot_, double value_, bool shiftPressed_)
{
  M_LOG("[Client] controlChanged " << static_cast<int>(pot_) << " (" << value_ << ") "
                                   << (shiftPressed_ ? " SHIFT" : ""));
  m_update = true;
}

//--------------------------------------------------------------------------------------------------

void Client::devicesListChanged(Coordinator::tCollDeviceDescriptor deviceDescriptors_)
{
  M_LOG("[Client] devicesListChanged : " << deviceDescriptors_.size() << " devices");

  for (const auto& deviceDescriptor : deviceDescriptors_)
  {
    if (!m_discoveryPolicy.matches(deviceDescriptor))
    {
      continue;
    }
    if ((m_pDevice && !m_pDevice->hasDeviceHandle()) || !m_pDevice)
    {
      m_pDevice = Coordinator::instance().connect(deviceDescriptor);
      onInitDevice();
      break;
    }
  }
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
