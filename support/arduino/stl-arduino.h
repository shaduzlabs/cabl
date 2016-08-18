/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#if ARDUINO

#include <chrono>
#include <sstream>
#include <string>

namespace std
{
namespace this_thread
{

//--------------------------------------------------------------------------------------------------

template <class Rep, class Period>
void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
{
}

//--------------------------------------------------------------------------------------------------
}

//--------------------------------------------------------------------------------------------------

template <typename T>
string to_string(const T& n)
{
  ostringstream stm;
  stm << n;
  return stm.str();
}

//--------------------------------------------------------------------------------------------------

template <class InputIt, class Size, class OutputIt>
OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
  if (count > 0)
  {
    *result++ = *first;
    for (Size i = 1; i < count; ++i)
    {
      *result++ = *++first;
    }
  }
  return result;
}

//--------------------------------------------------------------------------------------------------

template <class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op)
{
  while (first1 != last1)
  {
    *d_first++ = unary_op(*first1++);
  }
  return d_first;
}

//--------------------------------------------------------------------------------------------------

template <class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
OutputIt transform(
  InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first, BinaryOperation binary_op)
{
  while (first1 != last1)
  {
    *d_first++ = binary_op(*first1++, *first2++);
  }
  return d_first;
}

//--------------------------------------------------------------------------------------------------
}

//--------------------------------------------------------------------------------------------------

#endif // ARDUINO
