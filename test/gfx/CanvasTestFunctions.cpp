/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "CanvasTestHelpers.h"

#include <algorithm>
#include <iostream>

#include <lodepng.h>

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{
namespace test
{
namespace
{
const util::ColorRGB k_colorTransparent{};
const util::ColorRGB k_colorBlack{0x00};
const util::ColorRGB k_colorBlue{0x00, 0x00, 0xff, 0xcc};
const util::ColorRGB k_colorRed{0xff, 0x00, 0x00, 0x7f};
const util::ColorRGB k_colorGreen{0x00, 0xff, 0x00, 0x33};
const util::ColorRGB k_colorWhite{0xff};
const util::ColorRGB k_colorInvert{util::ColorRGB::BlendMode::Invert};
}

//--------------------------------------------------------------------------------------------------

void lines(Canvas* c_)
{
  uint16_t width{c_->width()};
  uint16_t height{c_->height()};

  c_->line(0, height, width, 0, k_colorWhite);
  c_->line(0, 0, width * 2, height * 2, k_colorWhite);
  c_->lineHorizontal(0, height / 2, 2500, k_colorWhite);
  c_->lineVertical(width / 2, height / 2, 2500, k_colorWhite);

  c_->line(0, (height / 4) * 3, width, height, k_colorBlue);
  c_->line(0, ((height / 4) * 3) + 1, width, height + 1, k_colorBlue);

  c_->line(width, (height / 4) * 3, 0, height, k_colorRed);
  c_->line(width, ((height / 4) * 3) + 1, 0, height + 1, k_colorRed);

  c_->lineHorizontal(0, (height / 8) * 7, 2500, k_colorGreen);
  c_->lineHorizontal(0, ((height / 8) * 7) + 1, 2500, k_colorGreen);

  for (uint16_t i = 0; i < height / 2; i += 2)
  {
    c_->lineHorizontal(0, i, 2500, k_colorInvert);
  }

  for (uint16_t i = width / 4; i < (width / 4) * 3; i += 2)
  {
    c_->lineVertical(i, 0, 2500, k_colorInvert);
  }
}

//--------------------------------------------------------------------------------------------------

void circles(Canvas* c_)
{
  uint16_t width{c_->width()};
  uint16_t height{c_->height()};

  c_->circle(width / 2, height / 2, std::min(width / 2, height / 2), k_colorBlue);

  c_->circleFilled(
    (width / 4), (height / 4), std::min(width / 4, height / 4), k_colorBlue, k_colorInvert);
  c_->circleFilled(
    (width / 4) * 3, (height / 4) * 3, std::min(width / 4, height / 4), k_colorGreen, k_colorInvert);
  c_->circleFilled(
    (width / 4) * 3, (height / 4), std::min(width / 4, height / 4), k_colorRed, k_colorInvert);
  c_->circleFilled(
    (width / 4), (height / 4) * 3, std::min(width / 4, height / 4), k_colorWhite, k_colorInvert);

  c_->circleFilled((width / 4),
    (height / 4) * 3,
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::QuarterBottomLeft);
  c_->circleFilled((width / 4) * 3,
    (height / 4) * 3,
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::QuarterBottomRight);
  c_->circleFilled((width / 4),
    (height / 4),
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::QuarterTopLeft);
  c_->circleFilled((width / 4) * 3,
    (height / 4),
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::QuarterTopRight);

  c_->circleFilled((width / 4),
    (height / 2),
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::SemiLeft);
  c_->circleFilled((width / 4) * 3,
    (height / 2),
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::SemiRight);
  c_->circleFilled((width / 2),
    (height / 4),
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::SemiTop);
  c_->circleFilled((width / 2),
    (height / 4) * 3,
    std::min(width / 8, height / 8),
    k_colorBlack,
    k_colorGreen,
    Canvas::CircleType::SemiBottom);

  c_->circleFilled(
    (width / 8), (height / 8), std::min(width / 8, height / 8), k_colorBlue, k_colorInvert);
  c_->circleFilled(
    (width / 8) * 7, (height / 8) * 7, std::min(width / 8, height / 8), k_colorGreen, k_colorInvert);
  c_->circleFilled(
    (width / 8) * 7, (height / 8), std::min(width / 8, height / 8), k_colorRed, k_colorInvert);
  c_->circleFilled(
    (width / 8), (height / 8) * 7, std::min(width / 8, height / 8), k_colorWhite, k_colorInvert);
}

//--------------------------------------------------------------------------------------------------

void triangles(Canvas* c_)
{

}

//--------------------------------------------------------------------------------------------------

void rectangles(Canvas* c_)
{
}

//--------------------------------------------------------------------------------------------------

void text(Canvas* c_)
{
  c_->character(5,10,'*',k_colorGreen,"small");
  c_->character(5,20,'*',k_colorRed,"normal");
  c_->character(5,30,'*',k_colorWhite,"big");
  c_->character(5,44,'*',k_colorBlue,"non-existing-font");

  c_->text(40,10,"Life is short.", k_colorWhite, "small");
  c_->text(40,20,"Smile while", k_colorBlue, "normal");
  c_->text(40,30,"you still", k_colorRed, "big");
  c_->text(40,44,"have teeth.", k_colorGreen, "non-existing-font");

  c_->text(40,44,"Optimism is a lack of information.", k_colorTransparent, "non-existing-font");

}

//--------------------------------------------------------------------------------------------------

void canvas(Canvas* c_)
{
  unsigned x = static_cast<unsigned>(c_->width() * 0.1);
  unsigned y = static_cast<unsigned>(c_->height() * 0.1);
  unsigned w = static_cast<unsigned>(c_->width() * 0.8);
  unsigned h = static_cast<unsigned>(c_->height() * 0.8);

  Canvas internalCanvas(w, h);

  for (unsigned j = 0; j < w; j++)
  {
    for (unsigned i = 0; i < h; i++)
    {
      internalCanvas.setPixel(j, i, {static_cast<uint8_t>(j), static_cast<uint8_t>(255 - j), 0});
    }
  }

  Canvas internaInternallCanvas(w, h);

  for (unsigned j = 0; j < w / 2; j++)
  {
    for (unsigned i = 0; i < h / 2; i++)
    {
      internaInternallCanvas.setPixel(j,
        i,
        {static_cast<uint8_t>(j * 2), static_cast<uint8_t>(j * 2), static_cast<uint8_t>(j * 2)});
    }
  }
  internalCanvas.canvas(internaInternallCanvas, (w / 2), (h / 2));
  c_->canvas(internalCanvas, x, y);
}

//--------------------------------------------------------------------------------------------------

} // namespace test
} // namespace cabl
} // namespace sl
