/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <stdio.h>
#include <thread>

#include <cabl.h>
#include "Euklid.h"


using namespace sl;
using namespace sl::cabl;

//--------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  Euklid euklid;

  std::cout << "Type 'q' and hit ENTER to quit." << std::endl;

  while (true)//std::cin.get() != 'q')
  {
    euklid.tick();
  }

  return 0;
}

//--------------------------------------------------------------------------------------------------
