/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <map>
#include <memory>
#include <string>

#include "cabl/gfx/Font.h"

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

/**
  \class FontManager
  \brief The font manager class

*/
class FontManager
{
public:
  static FontManager& instance();

  const Font* getFont(const std::string& /*name_*/) const;
  const Font* getDefaultFont() const;

private:
  FontManager();

  std::unique_ptr<Font> m_pDefaultFont;
  std::map<std::string, std::unique_ptr<Font>> m_collFonts;
  bool m_initialized{false};
};

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
