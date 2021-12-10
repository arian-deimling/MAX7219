/**
 * @file Glyph.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include <array>
#include <cstdint>
#include <algorithm>

#include "Glyph.h"

Glyph::Glyph(std::array<uint8_t, HEIGHT>& data, bool proportional,
             std::uint_fast8_t code_point, std::size_t spacing)
  : glyph_data(data) {
    
  // set glyph to the default/max width
  glyph_width = WIDTH_MAX;

  if (proportional) {

    // assume the glyph has no exceess spacing on the left-most side
    int left_spacing = 0;

    // for each column in the glyph
    for (std::size_t col = 0; col < WIDTH_MAX; col++) {

      // assume that the column is empty
      bool empty_column = true;

      // check whether the column is empty
      for (std::size_t row = 0; row < HEIGHT; row++) {
        empty_column = empty_column && !get_pixel(row, col);
      }

      // if the column was empty increment and continue
      if (empty_column) {
        left_spacing++;
      } else {
        break;
      }
    }

    // left-shift each row by the amount needed to make it proportional 
    for (std::size_t row = 0; row < HEIGHT; row++) {
      glyph_data.at(row) <<= left_spacing;
    }

    // for each column (from right-most to left-most)
    for (std::size_t col = WIDTH_MAX - 1; col--;) {

      // assume that the column is empty
      bool empty_column = true;

      // check whether the column is empty
      for (std::size_t row = 0; row < HEIGHT; row++) {
        empty_column = empty_column && !get_pixel(row, col);
      }

      // if there is an empty column, decrement the width of the glyph
      // until a column that is not empty is encountered
      if (empty_column) {
        glyph_width--;
      } else {
        break;
      }
    }

    // code point 0x20 (' ' character) is special (width 4 plus spacing)
    if (code_point == 0x20) {
      glyph_width = 4;
    }

    // add the specified amount of spacing
    glyph_width += spacing;

    // code point 0xFF is also special (width 1 regardless of spacing)
    if (code_point == 0xFF) {
      glyph_width = 1;
    }

    // cast is required because it creates a copy of WIDTH_MAX; without cast,
    // since std::min() takes parameters by reference, we cannot supply it
    // with a static const value
    glyph_width = std::min(glyph_width, static_cast<std::size_t>(WIDTH_MAX));

  }
}