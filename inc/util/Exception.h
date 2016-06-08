/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <stdint.h>

namespace sl
{
namespace util
{

//--------------------------------------------------------------------------------------------------

class Exception : public std::exception
{
public:

  Exception(std::string errorMessage_)
    : m_errorMessage(std::move(errorMessage_))
  {}

  const char* what() const noexcept override
  {
    return m_errorMessage.c_str();
  }

  const std::string& message() const noexcept
  {
    return m_errorMessage;
  }

private:

  std::string m_errorMessage;

};

//--------------------------------------------------------------------------------------------------

} // util
} // sl
