/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include "cabl/util/Version.h"

#include <sstream>

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

Version::operator bool() const
{
  return (m_vMajor + m_vMinor + m_vMicro) > 0;
}

//--------------------------------------------------------------------------------------------------

void Version::reset()
{
  m_vMajor = 0;
  m_vMinor = 0;
  m_vMicro = 0;
}

//--------------------------------------------------------------------------------------------------

std::string Version::toString() const
{
  std::ostringstream stringStream;
  stringStream << m_vMajor << "." << m_vMinor << "." << m_vMicro;
  return stringStream.str();
}

//--------------------------------------------------------------------------------------------------

Version::operator std::string() const
{
  return toString();
}

//--------------------------------------------------------------------------------------------------

bool Version::operator==(const Version& other_) const
{
  return (m_vMajor == other_.m_vMajor) && (m_vMinor == other_.m_vMinor)
         && (m_vMicro == other_.m_vMicro);
}

//--------------------------------------------------------------------------------------------------

bool Version::operator!=(const Version& other_) const
{
  return !(operator==(other_));
}

//--------------------------------------------------------------------------------------------------

bool Version::operator>=(const Version& other_) const
{
  if (m_vMajor != other_.m_vMajor)
  {
    return m_vMajor > other_.m_vMajor;
  }

  if (m_vMinor != other_.m_vMinor)
  {
    return m_vMinor > other_.m_vMinor;
  }

  return m_vMicro >= other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------

bool Version::operator>(const Version& other_) const
{
  if (m_vMajor != other_.m_vMajor)
  {
    return m_vMajor > other_.m_vMajor;
  }

  if (m_vMinor != other_.m_vMinor)
  {
    return m_vMinor > other_.m_vMinor;
  }

  return m_vMicro > other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------

bool Version::operator<=(const Version& other_) const
{
  if (m_vMajor != other_.m_vMajor)
  {
    return m_vMajor < other_.m_vMajor;
  }

  if (m_vMinor != other_.m_vMinor)
  {
    return m_vMinor < other_.m_vMinor;
  }

  return m_vMicro <= other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------

bool Version::operator<(const Version& other_) const
{
  if (m_vMajor != other_.m_vMajor)
  {
    return m_vMajor < other_.m_vMajor;
  }

  if (m_vMinor != other_.m_vMinor)
  {
    return m_vMinor < other_.m_vMinor;
  }

  return m_vMicro < other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
