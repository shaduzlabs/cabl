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

#pragma once
#include <string>

namespace sl
{
namespace util
{

class Version
{
public:

  Version(unsigned vMajor_ = 0, unsigned vMinor_ = 0, unsigned vMicro_ = 0)
    : m_vMajor( vMajor_ )
    , m_vMinor( vMinor_ )
    , m_vMicro( vMicro_ )
  {}
 
  operator bool() const{ return ( m_vMajor + m_vMinor + m_vMicro ) > 0; }

  unsigned getMajor() const { return m_vMajor; }
  unsigned getMinor() const { return m_vMinor; }
  unsigned getMicro() const { return m_vMicro; }

  void setMajor(unsigned vMajor_){ m_vMajor = vMajor_; }
  void setMinor(unsigned vMinor_){ m_vMinor = vMinor_; }
  void setMicro(unsigned vMicro_){ m_vMicro = vMicro_; }

  void reset();

  std::string toString() const;
  operator std::string() const;

  bool operator == (const Version& other_) const;
  bool operator != (const Version& other_) const;
  bool operator >= (const Version& other_) const;
  bool operator >  (const Version& other_) const;
  bool operator <= (const Version& other_) const;
  bool operator <  (const Version& other_) const;

private:

  unsigned m_vMajor;
  unsigned m_vMinor;
  unsigned m_vMicro;
};

} // namespace util
} // namespace sl
