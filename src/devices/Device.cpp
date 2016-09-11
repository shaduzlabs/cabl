/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/Device.h"
#include "comm/DeviceHandle.h"


#include "gfx/Canvas.h"
#include "gfx/LCDDisplay.h"
#include "gfx/LedArray.h"
#include "gfx/LedMatrix.h"

#include "gfx/displays/GDisplayDummy.h"
#include "gfx/displays/LedArrayDummy.h"

namespace sl
{
namespace cabl
{
namespace devices
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

Canvas* Device::displayGraphic(size_t displayIndex_)
{
  static GDisplayDummy s_dummyDisplay;
  return &s_dummyDisplay;
}

//--------------------------------------------------------------------------------------------------

LCDDisplay* Device::displayLCD(size_t displayIndex_)
{
  static LCDDisplayDummy s_dummyLCDDisplay;
  return &s_dummyLCDDisplay;
}

//--------------------------------------------------------------------------------------------------

Canvas* Device::ledMatrix(size_t ledMatrixInde_)
{
  static GDisplayDummy s_dummyLedMatrix;
  return &s_dummyLedMatrix;
}

//--------------------------------------------------------------------------------------------------

LedArray* Device::ledArray(size_t ledArrayIndex_)
{
  static LedArrayDummy s_dummyLedArray;
  return &s_dummyLedArray;
}

//--------------------------------------------------------------------------------------------------

void Device::setLed(Pad, const util::ColorRGB&)
{
}

//--------------------------------------------------------------------------------------------------

void Device::setLed(Key, const util::ColorRGB&)
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

void Device::setCallbackPadChanged(tCbPadChanged cbPadChanged_)
{
  m_cbPadChanged = cbPadChanged_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackKeyChanged(tCbKeyChanged cbKeyChanged_)
{
  m_cbKeyChanged = cbKeyChanged_;
}

//--------------------------------------------------------------------------------------------------

void Device::setCallbackPotentiometerChanged(tCbPotentiometerChanged cbPotentiometerChanged_)
{
  m_cbPotentiometerChanged = cbPotentiometerChanged_;
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

void Device::encoderChanged(Encoder encoder_, bool valueIncreased_, bool shiftPressed_)
{
  if (m_cbEncoderChanged)
  {
    m_cbEncoderChanged(encoder_, valueIncreased_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::padChanged(Pad pad_, uint16_t value_, bool shiftPressed_)
{
  if (m_cbPadChanged)
  {
    m_cbPadChanged(pad_, value_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::keyChanged(Key key_, uint16_t value_, bool shiftPressed_)
{
  if (m_cbKeyChanged)
  {
    m_cbKeyChanged(key_, value_, shiftPressed_);
  }
}

//--------------------------------------------------------------------------------------------------

void Device::potentiometerChanged(Potentiometer potentiometer_, uint16_t value_, bool shiftPressed_)
{
  if (m_cbPotentiometerChanged)
  {
    m_cbPotentiometerChanged(potentiometer_, value_, shiftPressed_);
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

} // devices
} // cabl
} // sl
