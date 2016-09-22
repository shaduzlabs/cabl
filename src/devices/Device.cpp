/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/devices/Device.h"
#include "cabl/comm/DeviceHandle.h"


#include "cabl/gfx/Canvas.h"
#include "cabl/gfx/TextDisplay.h"
#include "cabl/gfx/LedArray.h"
#include "cabl/gfx/LedMatrix.h"

#include "gfx/displays/NullCanvas.h"
#include "gfx/LedArrayDummy.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

void Device::setDeviceHandle(tPtr<DeviceHandle> pDeviceHandle_)
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
  m_pDeviceHandle = std::move(pDeviceHandle_);
}

//--------------------------------------------------------------------------------------------------

void Device::resetDeviceHandle()
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
  m_pDeviceHandle = nullptr;
}

//--------------------------------------------------------------------------------------------------

Canvas* Device::graphicDisplay(size_t displayIndex_)
{
  static NullCanvas s_dummyDisplay;
  return &s_dummyDisplay;
}

//--------------------------------------------------------------------------------------------------

TextDisplay* Device::textDisplay(size_t displayIndex_)
{
  static TextDisplayDummy s_dummyTextDisplay;
  return &s_dummyTextDisplay;
}

//--------------------------------------------------------------------------------------------------

Canvas* Device::ledMatrix(size_t ledMatrixInde_)
{
  static NullCanvas s_dummyLedMatrix;
  return &s_dummyLedMatrix;
}

//--------------------------------------------------------------------------------------------------

LedArray* Device::ledArray(size_t ledArrayIndex_)
{
  static LedArrayDummy s_dummyLedArray;
  return &s_dummyLedArray;
}

//--------------------------------------------------------------------------------------------------

void Device::setButtonLed(Button, const Color&)
{
}

//--------------------------------------------------------------------------------------------------

void Device::setKeyLed(unsigned, const Color&)
{
}

//--------------------------------------------------------------------------------------------------

void Device::sendMidiMsg(tRawData)
{
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackDisconnect(tCbDisconnect cbDisconnect_)
{
  m_cbDisconnect = cbDisconnect_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackRender(tCbRender cbRender_)
{
  m_cbRender = cbRender_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackButtonChanged(tCbButtonChanged cbButtonChanged_)
{
  m_cbButtonChanged = cbButtonChanged_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackEncoderChanged(tCbEncoderChanged cbEncoderChanged_)
{
  m_cbEncoderChanged = cbEncoderChanged_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackKeyChanged(tCbKeyChanged cbKeyChanged_)
{
  m_cbKeyChanged = cbKeyChanged_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackControlChanged(tCbControlChanged cbControlChanged_)
{
  m_cbControlChanged = cbControlChanged_;
}

//--------------------------------------------------------------------------------------------------

bool Device::hasDeviceHandle()
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
  return static_cast<bool>(m_pDeviceHandle);
}

//--------------------------------------------------------------------------------------------------

bool Device::writeToDeviceHandle(const Transfer& transfer_, uint8_t endpoint_) const
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);

  if (m_pDeviceHandle)
  {
    return m_pDeviceHandle->write(transfer_, endpoint_);
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

bool Device::readFromDeviceHandle(Transfer& transfer_, uint8_t endpoint_) const
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
  if (m_pDeviceHandle)
  {
    return m_pDeviceHandle->read(transfer_, endpoint_);
  }

  return false;
}

//--------------------------------------------------------------------------------------------------

void Device::readFromDeviceHandleAsync(uint8_t endpoint_, DeviceHandle::tCbRead cbRead_) const
{
  std::lock_guard<std::mutex> lock(m_mtxDeviceHandle);
  if (m_pDeviceHandle)
  {
    return m_pDeviceHandle->readAsync(endpoint_, cbRead_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::buttonChanged(Button button_, bool buttonState_, bool shiftPressed_)
{
  if (m_cbButtonChanged)
  {
    m_cbButtonChanged(button_, buttonState_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
  if (m_cbEncoderChanged)
  {
    m_cbEncoderChanged(encoder_, valueIncreased_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
  if (m_cbKeyChanged)
  {
    m_cbKeyChanged(index_, value_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::controlChanged(unsigned potentiometer_, double value_, bool shiftPressed_)
{
  if (m_cbControlChanged)
  {
    m_cbControlChanged(potentiometer_, value_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

bool Device::onTick()
{
  if (!hasDeviceHandle())
  {
    return false;
  }

  Device::render();
  if (!m_connected)
  {
    return true;
  }
  return tick();
}

//--------------------------------------------------------------------------------------------------

void Device::onConnect()
{
  init();
  m_connected = true;
}

//--------------------------------------------------------------------------------------------------

void Device::onDisconnect()
{
  m_connected = false;
  resetDeviceHandle();
  if (m_cbDisconnect)
  {
    m_cbDisconnect();
  }
}

//--------------------------------------------------------------------------------------------------

void Device::render()
{
  if (m_cbRender)
  {
    m_cbRender();
  }
}

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
