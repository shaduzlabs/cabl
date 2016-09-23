/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineJamHelper.h"

#include <limits>
#include <map>

#include "cabl/util/Color.h"

//--------------------------------------------------------------------------------------------------

namespace
{

// clang-format off

const std::vector<sl::cabl::Color> kJam_colors{
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
//| R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {  32,   0,   0 },
  {  64,   0,   0 }, { 255,   0,   0 }, { 255,  32,  16 }, {  32,  16,   0 }, {  64,  16,   0 },
  { 255,  32,   0 }, { 255,  64,  32 }, {  32,  48,   8 }, {  64,  64,   0 }, { 255,  96,   0 },
  { 255, 128,  48 }, {  40,  56,   0 }, {  64,  96,   0 }, { 255, 224,   0 }, { 128, 255,  64 },
  {  24,  40,   0 }, {  96, 176,   0 }, {  96, 255,   0 }, { 112, 255,  64 }, {  16,  40,   0 },
  {  56, 112,   0 }, {  72, 255,   0 }, { 64,  255,  64 }, {   0,  40,   0 }, {   0,  96,   0 },
  {   0, 255,   0 }, { 48,  255,  48 }, {   0,  40,  16 }, {   0, 112,  32 }, {   0, 255,  48 },
  { 56,  255,  40 }, {   0,  40,  40 }, {   0, 112,  96 }, {   0, 255, 226 }, { 64,  255, 255 },
  {   0,  24,  48 }, {   0,  64, 112 }, {   0,  96, 255 }, { 40,  160, 255 }, {   0,   0,  40 },
  {   0,   0, 128 }, {   0,   0, 255 }, { 40,  104, 255 }, {   8,   0,  44 }, {  32,   0, 128 },
  {  40,   0, 255 }, { 48,   88, 255 }, {  24,   0,  40 }, {  72,   0, 128 }, { 104,   0, 255 },
  { 88,   80, 255 }, {  32,   0,  48 }, {  96,   0,  96 }, { 255,   0, 128 }, { 255,  64, 160 },
  {  32,   0,  16 }, {  90,   0,  48 }, { 255,   0,  64 }, { 255,  40, 104 }, {  40,   0,   8 },
  { 112,   0,  16 }, { 255,   0,  16 }, { 255,  48,  56 }, {  40,  40,  40 }, {  96,  96,  96 },
  { 226, 226, 226 }, { 160, 255, 255 },
};
// clang-format on
}

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

uint8_t MaschineJamHelper::toLedColor(const Color& color_)
{
  std::map<Color, uint8_t> s_colorsCache;

  auto it = s_colorsCache.find(color_);
  if (it != s_colorsCache.end())
  {
    return it->second;
  }

  uint8_t colorIndex = 0;
  double minDistance = std::numeric_limits<double>::max();
  for (uint8_t i = 0; i < kJam_colors.size(); i++)
  {
    double currentDistance = color_.distance(kJam_colors[i]);
    if (currentDistance < minDistance)
    {
      colorIndex = i;
      minDistance = currentDistance;
    }
    if (minDistance == 0)
    {
      break;
    }
  }
  s_colorsCache.emplace(std::move(color_), colorIndex);

  return colorIndex;
}

//--------------------------------------------------------------------------------------------------

Color MaschineJamHelper::fromLedColor(uint8_t color_)
{
  if (color_ < 69)
  {
    return kJam_colors[color_];
  }
  return {};
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
