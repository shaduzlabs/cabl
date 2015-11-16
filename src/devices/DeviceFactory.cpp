/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/DeviceFactory.h"

#include "comm/DeviceDescriptor.h"

#include "devices/ni/KompleteKontrol.h"
#include "devices/ni/MaschineMK1.h"
#include "devices/ni/MaschineMK2.h"
#include "devices/ni/MaschineMikroMK2.h"
#include "devices/ni/TraktorF1MK2.h"

#include "devices/akai/Push.h"
#include "devices/ableton/Push2.h"

//--------------------------------------------------------------------------------------------------

namespace
{

//--------------------------------------------------------------------------------------------------
 
static const unsigned kVendor_Ableton               = 0x0047;
static const unsigned kProduct_Push2                = 0x1500;

//--------------------------------------------------------------------------------------------------
 
static const unsigned kVendor_Akai                  = 0x0047;
static const unsigned kProduct_Push                 = 0x1500;

//--------------------------------------------------------------------------------------------------

static const unsigned kVendor_NativeInstruments     = 0x17CC;

static const unsigned kProduct_KompleteKontrolS25   = 0x1340;
static const unsigned kProduct_KompleteKontrolS49   = 0x1350;
static const unsigned kProduct_KompleteKontrolS61   = 0x1360;

static const unsigned kProduct_MaschineMK1          = 0x0808;
static const unsigned kProduct_MaschineMK2          = 0x1140;
static const unsigned kProduct_MaschineMikroMK1     = 0x1110;
static const unsigned kProduct_MaschineMikroMK2     = 0x1200;

static const unsigned kProduct_TraktorKontrolF1MK2  = 0x1120;

//--------------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

DeviceBase* DeviceFactory::getDevice(
  const DeviceDescriptor& deviceDescriptor_, 
  tPtr<DeviceHandle> pDeviceHandle_
)
{
  if(
     (deviceDescriptor_.getVendorId()!=kVendor_Akai) &&
     (deviceDescriptor_.getVendorId()!=kVendor_Ableton) &&
     (deviceDescriptor_.getVendorId()!=kVendor_NativeInstruments)
  )
  {
    return nullptr;
  }
  switch(deviceDescriptor_.getProductId())
  {
    case kProduct_Push:
    {
      return new Push(std::move(pDeviceHandle_));
    }
    case kProduct_KompleteKontrolS25:
    {
      return new KompleteKontrol(std::move(pDeviceHandle_),25);
    }
    case kProduct_KompleteKontrolS49:
    {
      return new KompleteKontrol(std::move(pDeviceHandle_),49);
    }
    case kProduct_KompleteKontrolS61:
    {
      return new KompleteKontrol(std::move(pDeviceHandle_),61);
    }
    case kProduct_MaschineMK1:
    {
      return new MaschineMK1(std::move(pDeviceHandle_));
    }
    case kProduct_MaschineMK2:
    {
      return new MaschineMK2(std::move(pDeviceHandle_));
    }
    case kProduct_MaschineMikroMK1:
    {
     //!\todo Add support for Maschine Mikro MK1
     // return new MaschineMikroMK1(std::move(pDeviceHandle_));
    }
    case kProduct_MaschineMikroMK2:
    {
      return new MaschineMikroMK2(std::move(pDeviceHandle_));
    }
    case kProduct_TraktorKontrolF1MK2:
    {
      return new TraktorF1MK2(std::move(pDeviceHandle_));
    }
    default:
    {
      return nullptr;
    }
  }
  return nullptr;
}

//--------------------------------------------------------------------------------------------------

} // devices
} // cabl
} // sl
