/**
 * @file Glyph.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef SCROLLER_GLYPH_H_
#define SCROLLER_GLYPH_H_

#include <array>
#include <cstdint>

/**
 * @brief The content of a `Font` which contains the data for how to draw a
 *        specified character for that `Font`.
 *
 * Visual representation of the binary (integer) representation of a glyph
 * as well as how the character could look when printed to terminal.
 * 
 * ```
 * {                   +--------+
 * 0b11111110,         |███████ |
 * 0b01100010,         | ██   █ |
 * 0b01101000,     \   | ██ █   |
 * 0b01111000, =====\  | ████   |
 * 0b01101000, =====/  | ██ █   |
 * 0b01100010,     /   | ██   █ |
 * 0b11111110,         |███████ |
 * 0b00000000,         |        |
 * }                   +--------+
 * ```
 */
class Glyph {

public:

  /**
   * Height of each `Glyph`; constant because Glyph placement does not change
   * vertically on the image.
   */
  static const std::size_t HEIGHT{8};

  /**
   * Maximum width of each `Glyph` and the default width of `Glyph`s that are
   * not proportional nor have a spacing.
   */
  static const std::size_t WIDTH_MAX{8};

private:

  /**
   * `Glyph` data is stored as an array of 8-bit unsigned integers where
   * each pixel is represented by a single bit.
   */
  std::array<std::uint_fast8_t, HEIGHT> glyph_data;

  /**
   * Width of the `Glyph` when drawn including any spacing; since the 8-bit
   * integers used to represent rows of a glyph must be `8` wide, the width
   * that we want to use when printing them must be stored separately.
   */
  std::size_t glyph_width;

public:

  /**
   * Deletion of functions that could potentially be implicitly declared in
   * order to prevent errors from accidental use.
   */
  Glyph(const Glyph&) = delete;
  Glyph(Glyph&&) = delete;
  Glyph& operator=(const Glyph&) = delete;
  Glyph& operator=(Glyph&&) = delete;

  /**
   * @brief Construct a `Glyph` by supplying data, codepoint,
   *        proportionality, and and spacing information.
   * 
   * A `Glyph` in its default form would have `proportional` set to `false`
   * and spacing of zero which would make each Glyph have an 8x8 size.
   * Setting proportional to `true` trims all excess width from a character
   * so that its width is equal to the number of columns between the left-
   * most and right-most columns with any pixel set to on. After this
   * adjustment, spacing adds a specified number of empty columns.
   * 
   * ASCII codepoints 0x20 and 0xFF are special when proportional is used in
   * that 0x20 and 0xFF both have no pixels set to on but have widths of `4`
   * and `1` repsectively prior to addition of spacing. In addition,
   * codepoint 0x20 is affected by spacing whereas 0xFF is not.
   * 
   * @param data information describing how to draw the glyph
   * @param proportional whether to draw the glyph with excess whitespace
   *                     removed from both sides of the glyph
   * @param code_point integer position of the glyph in UTF-8
   * @param spacing number of blank columns of spacing to include at the end
   *                of the glyph
   * 
   * @warning Maximum glyph width of 8 cannot be exceeded; glyphs for which
   *          spacing increases width beyond 8, will be truncated at width 8.
   */
  Glyph(std::array<uint8_t, HEIGHT>& data, bool proportional,
        std::uint_fast8_t code_point, std::size_t spacing);

  /**
   * @brief Returns the value of the pixel at the specified position.
   * 
   * @param row row of the pixel to be retrieved
   * @param col column of the pixel to be retrieved
   * @return `1` if the specified pixel is set on or `0` if the pixel is set
   *         off
   */
  std::uint_fast8_t get_pixel(std::size_t row, std::size_t col);

  /**
   * @brief Returns the width of this glyph in pixels.
   * 
   * @return pixel width of this glyph
   */
  std::size_t get_width();

};

inline std::uint_fast8_t Glyph::get_pixel(std::size_t row, std::size_t col) {
  return !!(glyph_data.at(row) & (1 << (WIDTH_MAX - 1 - col)));
}

inline std::size_t Glyph::get_width() {
  return glyph_width;
}

#endif  // SCROLLER_GLYPH_H_
