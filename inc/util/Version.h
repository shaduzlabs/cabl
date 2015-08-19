/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

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
