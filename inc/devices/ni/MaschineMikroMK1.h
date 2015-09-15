/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "Device.h"

namespace sl
{
namespace kio
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

class MaschineMikroMK1 : public Device
{
  
public:
  
  MaschineMikroMK1();
  ~MaschineMikroMK1() override;
  
};

//--------------------------------------------------------------------------------------------------

} // devices
} // kio
} // sl
