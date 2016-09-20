/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "ColorDisplay.h"
#include <cabl.h>


using namespace sl;
using namespace sl::cabl;

//--------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  std::string pngFilesPath(".");
  if (argc == 2)
  {
    pngFilesPath = std::string(argv[1]);
  }
  else if (argc > 2)
  {
    std::cout << "Usage: " << argv[0] << " "
              << "[png files path]" << std::endl;
    return -1;
  }

  ColorDisplay cd(pngFilesPath);

  cd.run();

  return 0;
}

//--------------------------------------------------------------------------------------------------
