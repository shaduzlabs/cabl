/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "devices/ni/MaschineJamHelper.h"

#include <map>

#include "util/ColorRGB.h"

//--------------------------------------------------------------------------------------------------

namespace
{

// clang-format off

const std::vector<sl::util::ColorRGB> kJam_colors{
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
//| R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |   | R  | G  | B  |
//+----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+   +----+----+----+
  {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, {   0,   0,   0 }, { 255,  36,  36 },
  { 242,  58,  12 }, { 255, 153,   0 }, { 127,   0,   0 }, { 237, 249,  90 }, { 193, 157,   8 },
  { 255, 255,   0 }, {  86, 191,  19 }, {  44, 132,   3 }, {  36, 107,  36 }, {  25, 255,  48 },
  {  21, 149, 115 }, {  23, 107,  80 }, {   0, 255, 255 }, {   0, 116, 252 }, {  39,  79, 204 },
  {   0,  68, 140 }, { 100,  74, 217 }, {  77,  63, 160 }, { 135,   0, 255 }, { 230,  87, 227 },
  { 102,   0, 153 }, { 255,   0, 153 }, { 161,  76,  95 }, { 255,  77, 196 }, { 235, 139, 225 },
  { 166,  52,  33 }, { 153,  86,  40 }, { 135, 103,   0 }, { 144, 130,  31 }, {  74, 135,   0 },
  {   0, 127,  18 }, {  24,  83, 178 }, {  98,  75, 173 }, { 115,  58, 103 }, { 248, 188, 175 },
  { 255, 155, 118 }, { 255, 191,  95 }, { 217, 175, 113 }, { 255, 244, 128 }, { 191, 186, 105 },
  { 188, 204, 136 }, { 174, 255, 153 }, { 124, 221, 159 }, { 137, 180, 125 }, { 128, 243, 255 },
  { 122, 206, 252 }, { 104, 161, 211 }, { 133, 143, 194 }, { 187, 170, 242 }, { 205, 187, 228 },
  { 239, 139, 176 }, { 133, 157, 140 }, { 107, 117, 110 }, { 132, 144, 155 }, { 106, 112, 117 },
  { 136, 133, 157 }, { 108, 106, 117 }, { 157, 133, 156 }, { 116, 106, 116 }, { 156, 157, 133 },
  { 116, 117, 106 }, { 157, 132, 132 }, { 255, 255, 255 }, { 255, 255, 255 },
};
// clang-format on

}

namespace sl
{
namespace cabl
{
namespace devices
{

//--------------------------------------------------------------------------------------------------

uint8_t MaschineJamHelper::toLedColor(const sl::util::ColorRGB& color_)
{
  std::map<util::ColorRGB, uint8_t> s_colorsCache;

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

util::ColorRGB MaschineJamHelper::fromLedColor(uint8_t color_)
{
  if( color_ < 69)
  {
    return kJam_colors[color_];
  }
  return {};
}

//--------------------------------------------------------------------------------------------------

} // namespace devices
} // namespace cabl
} // namespace sl
