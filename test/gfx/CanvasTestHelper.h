/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "catch.hpp"

#include <gfx/Canvas.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

class CanvasTestHelper
{
public:
  //--------------------------------------------------------------------------------------------------

  static std::string displayContent(const Canvas& c_)
  {
    std::string displayContent = "\n";
    for (int row = 0; row < c_.m_height; row++)
    {
      for (int col = 0; col < c_.m_width; col++)
      {
        if (c_.pixel(col, row) == util::ColorRGB(0))
        {
          displayContent += "░";
        }
        else
        {
          displayContent += "█";
        }
      }
      displayContent += "\n";
    }
    displayContent.pop_back();
    return displayContent;
  }
};

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
