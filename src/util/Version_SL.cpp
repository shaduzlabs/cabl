/*----------------------------------------------------------------------------------------------------------------------

                 %%%%%%%%%%%%%%%%%                
                 %%%%%%%%%%%%%%%%%
                 %%%           %%%
                 %%%           %%%
                 %%%           %%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% www.shaduzlabs.com %%%%%

------------------------------------------------------------------------------------------------------------------------

  Copyright (C) 2014 Vincenzo Pacella

  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
  License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with this program.  
  If not, see <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------------------------------------------------------*/

#include "util/Version_SL.h"

#include <sstream>

namespace sl
{
namespace util
{

//----------------------------------------------------------------------------------------------------------------------

void Version::reset()
{
  m_vMajor = 0;
  m_vMinor = 0;
  m_vMicro = 0;
}

//--------------------------------------------------------------------------------------------------------------------

std::string Version::toString() const
{
  std::ostringstream stringStream;
  stringStream << m_vMajor << "." << m_vMinor << "." << m_vMicro;
  return stringStream.str();
}

//--------------------------------------------------------------------------------------------------------------------

Version::operator std::string() const
{
  return toString();
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator == ( const Version& other_ ) const
{
  return (m_vMajor == other_.m_vMajor) && (m_vMinor == other_.m_vMinor) && (m_vMicro == other_.m_vMicro);
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator != ( const Version& other_ ) const
{
  return !( operator == ( other_ ) );
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator >= ( const Version& other_ ) const
{
  if( m_vMajor != other_.m_vMajor )
    return m_vMajor > other_.m_vMajor;

  if( m_vMinor != other_.m_vMinor )
    return m_vMinor > other_.m_vMinor;

  return m_vMicro >= other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator > ( const Version& other_ ) const
{
  if( m_vMajor != other_.m_vMajor )
    return m_vMajor > other_.m_vMajor;

  if( m_vMinor != other_.m_vMinor )
    return m_vMinor > other_.m_vMinor;

  return m_vMicro > other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator <= ( const Version& other_ ) const
{
  if( m_vMajor != other_.m_vMajor )
    return m_vMajor < other_.m_vMajor;

  if( m_vMinor != other_.m_vMinor )
    return m_vMinor < other_.m_vMinor;

  return m_vMicro <= other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------------------------

bool Version::operator < ( const Version& other_ ) const
{
  if( m_vMajor != other_.m_vMajor )
    return m_vMajor < other_.m_vMajor;

  if( m_vMinor != other_.m_vMinor )
    return m_vMinor < other_.m_vMinor;

  return m_vMicro < other_.m_vMicro;
}

//--------------------------------------------------------------------------------------------------------------------

} // namespace util
} // namespace sl
