/**
 * @file Font.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include <string>
#include <array>
#include <fstream>
#include <memory>
#include <cstdint>

#include "Font.h"
#include "Glyph.h"

Font::Font(std::string font_file_name, bool proportional,
           std::size_t spacing) {

  // open the font file to read its contents
  std::ifstream font_file(font_file_name, std::ios::binary);

  // make sure the font file was opened
  if (font_file.is_open()) {

    // for each character in the font
    for (std::size_t code_point = 0;
         code_point < FONT_CHAR_COUNT; code_point++) {

      // read the data for the current character into an array
      std::array<uint8_t, Glyph::HEIGHT> new_glyph_data;
      font_file.read(
        reinterpret_cast<char*>(new_glyph_data.data()),
        Glyph::HEIGHT
      );

      // create a new glyph object with the data ready from the font file
      font_glyphs.at(code_point) = new Glyph(
        new_glyph_data, proportional, code_point, spacing);

    }
  }
}