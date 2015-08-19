/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once
#include <stdint.h>
#include <memory>

namespace sl
{
namespace util
{

//--------------------------------------------------------------------------------------------------

template < typename T, uint16_t bufferSize >
class CircularBuffer
{
public:

  explicit CircularBuffer()
    : m_size( bufferSize )
  {
    clear();
  }
 
  ~CircularBuffer()
  {}
  
  CircularBuffer( CircularBuffer& ) = delete;
  CircularBuffer& operator=( const CircularBuffer& ) = delete;
  CircularBuffer( CircularBuffer&& ) = delete;
  CircularBuffer& operator=( CircularBuffer&& ) = delete;

  void clear()
  {
    m_available = 0;
    m_indexRead = 0;
    m_indexWrite = 0;
    m_pData.reset( new T[m_size] );
  }

  void write( T value_ )
  {
    m_pData[ m_indexWrite ] = value_;
    m_indexWrite = ( m_indexWrite + 1 ) % m_size;
    if(m_available < m_size)
    {
      m_available++;
    }
  }

  bool read( T& value ) const
  {
    if( m_available <= 0 )
    {
      return false;
    }
    
    value = m_pData[ m_indexRead ];
    m_available--;
    m_indexRead = ( m_indexRead + 1 ) % m_size;
    
    return true;
  }
 
  constexpr size_t size() const
  {
    return bufferSize;
  }
  

private:  

  uint16_t              m_size;
  uint16_t              m_indexWrite;
  mutable uint16_t      m_indexRead;
  mutable uint16_t      m_available;
  
  std::unique_ptr<T[]> m_pData;

};

//--------------------------------------------------------------------------------------------------
  
} // namespace util
} // namespace sl
