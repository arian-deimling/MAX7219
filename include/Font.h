/**
 * @file Font.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef SCROLLER_FONT_H_
#define SCROLLER_FONT_H_

#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <memory>
#include <cstdint>

#include "Glyph.h"

/**
 * @brief A font that holds data on how to draw UTF-8 characters and can be
 *        drawn onto a `MatrixChainImage`.
 */
class Font {

public:

  /**
   * Number of glyphs that can be stored in a font.
   */
  static const std::size_t FONT_CHAR_COUNT{256};

private:

  /**
   * Array of `Glyph`s for this font where the indices of the glyphs are the
   * UTF-8 code points of the respective characters.
   */
  std::array<Glyph*, FONT_CHAR_COUNT> font_glyphs;

public:

  /**
   * Deletion of functions that could potentially be implicitly declared in
   * order to prevent errors from accidental use.
   */
  Font(const Font&) = delete;
  Font(Font&&) = delete;
  Font& operator=(const Font&) = delete;
  Font& operator=(Font&&) = delete;
     
  /**
   * @brief Constructs a font from a `.scrollerfont` font file.
   * 
   * @param font_file_name name of the font file to read font data from
   * @param proportional whether to create `Glyph`s with excess whitespace
   *                     removed from both sides of the glyphs
   * @param spacing number of blank columns of spacing to include at the end
   *                of each glyph
   */
  Font(std::string font_file_name, bool proportional, std::size_t spacing);

  /**
   * @brief `Font` destructor.
   */
  ~Font();

  /**
   * @brief Returns the width of the `Glyph` with the specified UTF-8 code
   *        point.
   * 
   * @param code_point code point of the desired glyph
   * @return the width of the glyph with the specified code point
   */
  std::size_t get_glyph_width(std::uint_fast8_t code_point);

  /**
   * @brief Returns the specified pixel of the glyph with the specified UTF-8
   *        code point.
   * 
   * @param code_point code point of the desired glyph
   * @param r row of the glyph of the desired pixel
   * @param c column of the glyph of the desired pixel
   * @return `1` if the specified pixel is set on or `0` if the pixel is set
   *         off
   */
  std::uint_fast8_t get_pixel(std::uint_fast8_t code_point, std::size_t row,
                              std::size_t col);

};  // class Font

inline Font::~Font() {
  for (std::size_t i = 0; i < font_glyphs.size(); i++) {
    delete font_glyphs.at(i);
  }
}

inline std::uint_fast8_t Font::get_pixel(std::uint_fast8_t code_point,
                                         std::size_t row, std::size_t col) {

  return font_glyphs.at(code_point)->get_pixel(row, col);
}

inline std::size_t Font::get_glyph_width(std::uint_fast8_t code_point) {
  return font_glyphs.at(code_point)->get_width();
}

#endif  // SCROLLER_FONT_H_
