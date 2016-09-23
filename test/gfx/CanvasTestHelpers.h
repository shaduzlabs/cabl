/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <cabl/gfx/Canvas.h>

//--------------------------------------------------------------------------------------------------

//#define DO_WRITE_PICTURES 1 // Enable this to generate the pngs, must be disabled on build servers

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

bool matchColorForAllPixels(const Canvas* /*canvas_*/, const Color& /*color_*/);

bool compare(const Canvas* /*c1_*/, const Canvas* /*c2_*/);

bool pngWrite(const Canvas* /*c_*/, const std::string& /*fileName_*/);

bool pngRead(Canvas* /*c_*/, const std::string& /*fileName_*/);

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
