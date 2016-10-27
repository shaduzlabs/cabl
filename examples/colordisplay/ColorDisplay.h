/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <future>
#include <stdint.h>

#include "DisplayClient.h"

namespace sl
{

using namespace cabl;

class ColorDisplay : public Client
{
public:
  ColorDisplay();
  void initDevice() override;
  void render() override;
  /*
    void buttonChanged(Device::Button button_, bool buttonState_, bool shiftState_) override;
    void encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_) override;
    void keyChanged(unsigned index_, double value_, bool shiftPressed) override;
    void controlChanged(unsigned pot_, double value_, bool shiftPressed) override;
    */
  
private:
  DisplayClient m_displayClient;
};

} // namespace sl
