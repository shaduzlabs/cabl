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

#if defined(NDEBUG)
  #define M_LOG(msg)
#else

//from: http://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming

#include <iostream>
#define M_LOG(msg) (sl::util::Log(__TIME__  , sl::util::LogData<sl::util::None>() << msg))

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
  #ifdef __ICC
    #define NOINLINE_ATTRIBUTE __attribute__(( noinline ))
  #else
    #define NOINLINE_ATTRIBUTE
  #endif // __ICC
#endif // NOINLINE_ATTRIBUTE

namespace sl
{
namespace util
{

//----------------------------------------------------------------------------------------------------------------------

struct None
{

};

//----------------------------------------------------------------------------------------------------------------------

template<typename List>
struct LogData
{
  List list;
};

//----------------------------------------------------------------------------------------------------------------------

template<typename List>
void Log(const char* time, LogData<List>&& data) NOINLINE_ATTRIBUTE
{
  std::cout << time << ": ";
  output(std::cout, std::move(data.list));
  std::cout << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------

template<typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin, Value&& value) noexcept
{
  return{ { std::forward<Begin>(begin.list), std::forward<Value>(value) } };
}

//----------------------------------------------------------------------------------------------------------------------

template<typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(LogData<Begin>&& begin, const char(&value)[n]) noexcept
{
  return{ { std::forward<Begin>(begin.list), value } };
}

//----------------------------------------------------------------------------------------------------------------------

typedef std::ostream& (*PfnManipulator)(std::ostream&);

//----------------------------------------------------------------------------------------------------------------------

template<typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(LogData<Begin>&& begin, PfnManipulator value) noexcept
{
  return{ { std::forward<Begin>(begin.list), value } };
}

//----------------------------------------------------------------------------------------------------------------------

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data)
{
  output(os, std::move(data.first));
  os << data.second;
}

//----------------------------------------------------------------------------------------------------------------------

inline void output(std::ostream& os, None)
{

}

//----------------------------------------------------------------------------------------------------------------------

} // util
} // sl

#endif
