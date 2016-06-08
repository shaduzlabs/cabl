/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <stdint.h>

#include "util/Types.h"

namespace sl
{
namespace cabl
{
class LCDDisplay
{
  
public:
  
  /**
   * @defgroup LCDDisplay LCD display
   * @{
   */
  
//--------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Types Public types
   * @ingroup LCDDisplay
   * @{
   */
  
  //! The alignment type
  enum class Align
  {
    Left,     //!< Align text to the left
    Center,   //!< Center text
    Right,    //!< Align text to the right
  };
  
  /** @} */ // End of group Types
  
//--------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup LCDDisplay
   * @{
   */
  
  //! Constructor
  /*!
   \param charsPerRow_     Number of chars per row
   \param nRows_           Number of rows
  */
  LCDDisplay(uint8_t numCharsPerRow_, uint8_t numRows_)
  : m_isDirty(false)
  , m_numCharsPerRow(numCharsPerRow_)
  , m_numRows(numRows_)
  {}
  
  //! Destructor
  virtual ~LCDDisplay() = default;
  
  virtual void clear() {}
  
  virtual const tRawData& displayData() const { return m_data; }
  
  /** @} */ // End of group Lifetime
 
//--------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Text Text
   * @ingroup LCDDisplay
   * @{
   */
  
  //! Print a single char
  /*!
   \param col_             The column at which the char must be printed
   \param row_             The row at which the char must be printed
   \param c_               The char to be printed
  */
  virtual void setCharacter(uint8_t col_, uint8_t row_, char c_){}
   
  //! Print a string
  /*!
   \param string_          The string to be printed
   \param row_             The row at which the string must be printed
   */
  virtual void setText(const std::string& string_, uint8_t row_, Align = Align::Left){}
  
  //! Print an integer
  /*!
   \param value_           The number to be printed
   \param row_             The row at which the number must be printed
   */
  virtual void setText(int value_, uint8_t row_, Align = Align::Left){}
    
  //! Print a floating point number
  /*!
   \param value_           The number to be printed
   \param row_             The row at which the number must be printed
   */
  virtual void setText(double value_, uint8_t row_, Align = Align::Left){}
  
  //! Fill characters in a row according to the value
  /*!
   \param value_           The value to be shown (0...1) by filling the available chars in a row
   \param row_             The row at which the value must be shown
   */
  virtual void setValue(float value_, uint8_t row_, Align = Align::Left){}
  
  /**@}*/ // End of Text group
  
//--------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Access Access and state queries functions
   * @ingroup LCDDisplay
   * @{
   */

  
  //! Is any of the display rows dirty?
  /*!
   \return        true if the display must be redrawn, false otherwise
   */
  virtual bool isDirty() const { return m_isDirty; }

  //! Is the specified row dirty?
  /*!
   \param row_    The display row to check
   \return true if the display row must be redrawn, false otherwise
   */
  virtual bool isDirtyRow(uint8_t row_) const { return m_isDirty; }
  
  //! Reset the global dirty flag
  virtual void resetDirtyFlags(){ m_isDirty = false; }
  
  /** @} */ // End of group Access
  
//--------------------------------------------------------------------------------------------------
  
  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup LCDDisplay
   * @{
   */
  
  virtual uint8_t numberOfCharsPerRow() const noexcept { return m_numCharsPerRow; }
  virtual uint8_t numberOfRows() const noexcept { return m_numRows; }

  void setDirty(bool isDirty_){ m_isDirty = isDirty_; }
  
  /** @} */ // End of group Utility
  
  /** @} */ // End of group LCDDisplay
  
//--------------------------------------------------------------------------------------------------

protected:

  tRawData& data(){ return m_data; }
  
private:

  bool                    m_isDirty;
  uint8_t                 m_numCharsPerRow;
  uint8_t                 m_numRows;

  tRawData                m_data;
  
};
  
//--------------------------------------------------------------------------------------------------
  
} // cabl
} // sl
