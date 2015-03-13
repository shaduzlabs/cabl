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

#include <iostream>

namespace sl
{
namespace util
{

template < typename T >
class ScopedPointer
{
public:

  inline ScopedPointer ( T* const ptr_  = nullptr )
    : m_ptr( ptr_ )
  {
  }
  
  ScopedPointer& operator= ( ScopedPointer& scopedPtr_ )
  {
    if ( this != scopedPtr_.getAddress() )
    {
      T* const oldObj = m_ptr;
      m_ptr = scopedPtr_.m_ptr;
      scopedPtr_.object = nullptr;
      delete oldObj;
    }

    return *this;
  }
   
  virtual ~ScopedPointer()
  {
    delete m_ptr;
  }

  inline operator T*() const                                    { return m_ptr; }

  inline T* get() const                                         { return m_ptr; }

  inline T& operator*() const                                   { return *m_ptr; }

  inline T* operator->() const                                  { return m_ptr; }
  
  
  inline ScopedPointer& reset( T* const ptr_ )
  {
    if ( m_ptr != ptr_ )
    {
      T* const oldObj = m_ptr;
      m_ptr = ptr_;
      delete oldObj;
    }

    return *this;
  }
  
private:

  ScopedPointer(const ScopedPointer<T>&);
	ScopedPointer<T>& operator=(const ScopedPointer<T>&);
  
  T*    m_ptr;

};
  
template < typename T >
class ScopedPointer <T[]>
{

public:

  inline ScopedPointer() : m_ptr(nullptr)
  {
  }
  
  inline ScopedPointer ( T* const ptr_ )
    : m_ptr( ptr_ )
  {
  }
  
  ScopedPointer ( ScopedPointer& scopedPtr_ )
    : m_ptr ( scopedPtr_.m_ptr )
  {
    scopedPtr_.m_ptr = nullptr;
  }
  
  ScopedPointer& operator= ( ScopedPointer& scopedPtr_ )
  {
    if ( this != scopedPtr_.getAddress() )
    {
      T* const oldObj = m_ptr;
      m_ptr = scopedPtr_.m_ptr;
      scopedPtr_.object = nullptr;
      delete oldObj;
    }

    return *this;
  }
   
  ~ScopedPointer()
  {
    delete[] m_ptr;
  }

  inline T &operator[](int i)
  {
    return m_ptr[i];
  }
 
  inline const T &operator[](int i) const
  {
    return m_ptr[i];
  }
  
  inline operator T*() const                                    { return m_ptr; }

  inline T* get() const                                         { return m_ptr; }

  inline T& operator*() const                                   { return *m_ptr; }

  inline T* operator->() const                                  { return m_ptr; }
  
  
  inline ScopedPointer& reset( T* const ptr_ )
  {
    if ( m_ptr != ptr_ )
    {
      T* const oldObj = m_ptr;
      m_ptr = ptr_;
      delete oldObj;
    }

    return *this;
  }
  
private:

  ScopedPointer(const ScopedPointer<T>&);
	ScopedPointer<T>& operator=(const ScopedPointer<T>&);

  T*    m_ptr;

  
};
  
template <class T>
bool operator== (const ScopedPointer<T>& ptr1_, T* const ptr2_)
{
  return static_cast<T*> ( ptr1_ ) == ptr2_;
}

template <class T>
bool operator!= (const ScopedPointer<T>& ptr1_, T* const ptr2_ )
{
  return static_cast<T*> ( ptr1_ ) != ptr2_;
}

} // UTIL
} // SL
