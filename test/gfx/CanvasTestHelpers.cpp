/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "CanvasTestHelpers.h"

#include <iostream>

#include <lodepng.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{

//--------------------------------------------------------------------------------------------------

bool matchColorForAllPixels(const Canvas* canvas_, const Color& color_)
{
  for (unsigned x = 0U; x < canvas_->width(); x++)
  {
    for (unsigned y = 0U; y < canvas_->height(); y++)
    {
      if (color_ != canvas_->pixel(x, y))
      {
        return false;
      }
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool compare(const Canvas* c1_, const Canvas* c2_)
{
  if (c1_->width() != c2_->width() || c1_->height() != c2_->height())
  {
    return false;
  }
  for (unsigned x = 0U; x < c1_->width(); x++)
  {
    for (unsigned y = 0U; y < c1_->height(); y++)
    {
      if (c1_->pixel(x, y) != c2_->pixel(x, y))
      {
        std::cout << x << ":" << y << " " << c1_->pixel(x, y) << " != " << c2_->pixel(x, y)
                  << std::endl;
        return false;
      }
    }
  }

  return true;
}

//--------------------------------------------------------------------------------------------------

bool pngWrite(const Canvas* c_, const std::string& fileName_)
{
  std::vector<uint8_t> pixels(c_->width() * c_->height() * 4);
  for (unsigned y = 0; y < c_->height(); y++)
  {
    for (unsigned x = 0; x < c_->width(); x++)
    {
      unsigned index = 4 * ((y * c_->width()) + x);
      Color color = c_->pixel(x, y);
      pixels[index++] = color.red();
      pixels[index++] = color.green();
      pixels[index++] = color.blue();
      pixels[index++] = 0xff;
    }
  }

  std::vector<unsigned char> png;
  unsigned error = lodepng::encode(png, pixels, c_->width(), c_->height());
  if (!error)
  {
    lodepng::save_file(png, fileName_.c_str());
  }
  else
  {
    std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

bool pngRead(Canvas* c_, const std::string& fileName_)
{
  std::vector<unsigned char> png;
  std::vector<unsigned char> pixels;
  unsigned width, height;

  // load and decode
  unsigned error = lodepng::load_file(png, fileName_.c_str());
  if (!error)
  {
    error = lodepng::decode(pixels, width, height, png);
  }

  if (error)
  {
    std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return false;
  }

  for (unsigned y = 0; y < height; y++)
  {
    for (unsigned x = 0; x < width; x++)
    {
      unsigned index = 4 * ((y * width) + x);
      c_->setPixel(x, y, {pixels[index], pixels[index + 1], pixels[index + 2]});
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
