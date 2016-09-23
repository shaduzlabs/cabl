/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

class Font;

//--------------------------------------------------------------------------------------------------

class LedArrayDummy : public LedArrayBase<0>
{

public:
  void setPixel(unsigned pos_, const Color& color_) override
  {
  }

  Color pixel(unsigned pos_) const override
  {
    return {};
  }
  
   void setValue(double val_, Color color_, Alignment alignment_) override
   {
   }
  
   void clear() override
   {
   
   }
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
