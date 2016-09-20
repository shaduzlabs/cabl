/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/gfx/FontManager.h"

#include "gfx/fonts/FontBig.h"
#include "gfx/fonts/FontNormal.h"
#include "gfx/fonts/FontSmall.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

FontManager::FontManager() : m_pDefaultFont(new FontNormal)
{
}

//--------------------------------------------------------------------------------------------------

FontManager& FontManager::instance()
{
  static FontManager instance;
  if (!instance.m_initialized)
  {
    instance.m_collFonts.emplace(std::make_pair("normal", std::unique_ptr<Font>(new FontNormal)));
    instance.m_collFonts.emplace(std::make_pair("small", std::unique_ptr<Font>(new FontSmall)));
    instance.m_collFonts.emplace(std::make_pair("big", std::unique_ptr<Font>(new FontBig)));
    instance.m_initialized = true;
  }
  return instance;
}

//--------------------------------------------------------------------------------------------------

const Font* FontManager::getFont(const std::string& name_) const
{
  auto font = m_collFonts.find(name_);
  if (font != m_collFonts.end())
  {
    return font->second.get();
  }

  return m_pDefaultFont.get();
}

//--------------------------------------------------------------------------------------------------

const Font* FontManager::getDefaultFont() const
{
  return m_pDefaultFont.get();
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl

//--------------------------------------------------------------------------------------------------
