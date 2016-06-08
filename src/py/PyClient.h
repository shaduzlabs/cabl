/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include "client/Client.h"
#include "devices/Coordinator.h"

namespace sl
{
namespace cabl
{

using namespace devices;

class PyClient : public Client
{
public:

  void render() override {};

};

} // namespace cabl
} // namespace sl
