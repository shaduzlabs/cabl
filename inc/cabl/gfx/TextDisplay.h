/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <array>
#include <bitset>
#include <cstdint>

#include "cabl/util/Types.h"

//--------------------------------------------------------------------------------------------------

namespace sl
{
namespace cabl
{

//--------------------------------------------------------------------------------------------------

class TextDisplay
{

public:
  /**
   * @defgroup TextDisplay LCD display
   * @{
   */

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Lifetime Constructor and destructor
   * @ingroup TextDisplay
   * @{
   */

  //! Destructor
  virtual ~TextDisplay() = default;

  virtual void clear() = 0;

  virtual void fill(uint8_t value_) = 0;

  virtual const uint8_t* displayData() const = 0;

  /** @} */ // End of group Lifetime

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Text Text
   * @ingroup TextDisplay
   * @{
   */

  //! Print a single char
  /*!
   \param col_             The column at which the char must be printed
   \param row_             The row at which the char must be printed
   \param c_               The char to be printed
  */
  virtual void putCharacter(unsigned col_, unsigned row_, char c_)
  {
  }

  //! Print a string
  /*!
   \param string_          The string to be printed
   \param row_             The row at which the string must be printed
   */
  virtual void putText(const std::string& string_, unsigned row_, Alignment = Alignment::Left)
  {
  }

  //! Print an integer
  /*!
   \param value_           The number to be printed
   \param row_             The row at which the number must be printed
   */
  virtual void putText(int value_, unsigned row_, Alignment = Alignment::Left)
  {
  }

  //! Print a floating point number
  /*!
   \param value_           The number to be printed
   \param row_             The row at which the number must be printed
   */
  virtual void putText(double value_, unsigned row_, Alignment = Alignment::Left)
  {
  }

  //! Fill characters in a row according to the value
  /*!
   \param value_           The value to be shown (0...1) by filling the available chars in a row
   \param row_             The row at which the value must be shown
   */
  virtual void putValue(float value_, unsigned row_, Alignment = Alignment::Left)
  {
  }

  /**@}*/ // End of Text group

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Access Access and state queries functions
   * @ingroup TextDisplay
   * @{
   */


  //! Is any of the display rows dirty?
  /*!
   \return        true if the display must be redrawn, false otherwise
   */
  virtual bool dirty() const = 0;

  //! Is the specified row dirty?
  /*!
   \param row_    The display row to check
   \return true if the display row must be redrawn, false otherwise
   */
  virtual bool dirtyRow(unsigned row_) const = 0;

  //! Reset the global dirty flag
  virtual void resetDirtyFlags() const = 0;

  /** @} */ // End of group Access

  //--------------------------------------------------------------------------------------------------

  /**
   * @defgroup Utility Utility/debug functions
   * @ingroup TextDisplay
   * @{
   */

  virtual unsigned width() const = 0;

  virtual unsigned height() const = 0;

  virtual unsigned dataSize() const = 0;

  /** @} */ // End of group Utility

  /** @} */ // End of group TextDisplay

  //--------------------------------------------------------------------------------------------------

protected:
  virtual uint8_t* data() = 0;
  virtual void setDirty(unsigned row_) const = 0;
};

//--------------------------------------------------------------------------------------------------

template <unsigned COLUMNS, unsigned ROWS, unsigned DATA_SIZE = COLUMNS* ROWS>
class TextDisplayBase : public TextDisplay
{

public:
  TextDisplayBase()
  {
    clear();
  }

  void clear() override
  {
    m_data.fill(0);
    m_dirtyFlags.set();
  }

  void fill(uint8_t value_) override
  {
    m_data.fill(value_);
    m_dirtyFlags.set();
  }

  const uint8_t* displayData() const override
  {
    return m_data.data();
  }

  bool dirty() const override
  {
    return m_dirtyFlags.any();
  }

  bool dirtyRow(unsigned row_) const override
  {
    return m_dirtyFlags.test(row_);
  }

  virtual void resetDirtyFlags() const override
  {
    m_dirtyFlags.reset();
  }

  unsigned width() const noexcept override
  {
    return COLUMNS;
  }

  unsigned height() const noexcept override
  {
    return ROWS;
  }

  unsigned dataSize() const noexcept override
  {
    return DATA_SIZE;
  }

  /** @} */ // End of group Utility

  /** @} */ // End of group TextDisplay

  //--------------------------------------------------------------------------------------------------

protected:
  uint8_t* data() override
  {
    return m_data.data();
  }

  void setDirty(unsigned row_) const override
  {
    m_dirtyFlags[row_] = true;
  }

private:
  mutable std::bitset<ROWS> m_dirtyFlags;

  std::array<uint8_t, DATA_SIZE> m_data;
};

using TextDisplayDummy = TextDisplayBase<0, 0>;

//--------------------------------------------------------------------------------------------------

} // namespace cabl
} // namespace sl
