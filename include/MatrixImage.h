/**
 * @file MatrixImage.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef SCROLLER_MATRIX_IMAGE_H_
#define SCROLLER_MATRIX_IMAGE_H_

#include <array>
#include <cstdint>
#include <memory>
#include <climits>
#include <exception>
#include <string>

/**
 * @brief An monochrome image that can be displayed on an 8x8 LED Matrix.
 * 
 * Visual Representation of the layout of pixels within the `MatrixImage`:
 * ```
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * |    C |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
 * | R    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 0    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 1    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 2    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 3    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 4    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 5    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 6    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 7    |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+
 * ```
 */
class MatrixImage {

public:  // (constant) public data members

  /**
   * Height of a LED Matrix in LEDs/pixels.
   */
  static const std::size_t HEIGHT{8};

  /**
   * Width of a LED Matrix in LEDs/pixels.
   */
  static const std::size_t WIDTH{8};

  static constexpr std::uint_fast8_t reverse_bits_lookup_table[]{
    0b00000000, 0b10000000, 0b01000000, 0b11000000,
    0b00100000, 0b10100000, 0b01100000, 0b11100000,
    0b00010000, 0b10010000, 0b01010000, 0b11010000,
    0b00110000, 0b10110000, 0b01110000, 0b11110000,
    0b00001000, 0b10001000, 0b01001000, 0b11001000,
    0b00101000, 0b10101000, 0b01101000, 0b11101000,
    0b00011000, 0b10011000, 0b01011000, 0b11011000,
    0b00111000, 0b10111000, 0b01111000, 0b11111000,
    0b00000100, 0b10000100, 0b01000100, 0b11000100,
    0b00100100, 0b10100100, 0b01100100, 0b11100100,
    0b00010100, 0b10010100, 0b01010100, 0b11010100,
    0b00110100, 0b10110100, 0b01110100, 0b11110100,
    0b00001100, 0b10001100, 0b01001100, 0b11001100,
    0b00101100, 0b10101100, 0b01101100, 0b11101100,
    0b00011100, 0b10011100, 0b01011100, 0b11011100,
    0b00111100, 0b10111100, 0b01111100, 0b11111100,
    0b00000010, 0b10000010, 0b01000010, 0b11000010,
    0b00100010, 0b10100010, 0b01100010, 0b11100010,
    0b00010010, 0b10010010, 0b01010010, 0b11010010,
    0b00110010, 0b10110010, 0b01110010, 0b11110010,
    0b00001010, 0b10001010, 0b01001010, 0b11001010,
    0b00101010, 0b10101010, 0b01101010, 0b11101010,
    0b00011010, 0b10011010, 0b01011010, 0b11011010,
    0b00111010, 0b10111010, 0b01111010, 0b11111010,
    0b00000110, 0b10000110, 0b01000110, 0b11000110,
    0b00100110, 0b10100110, 0b01100110, 0b11100110,
    0b00010110, 0b10010110, 0b01010110, 0b11010110,
    0b00110110, 0b10110110, 0b01110110, 0b11110110,
    0b00001110, 0b10001110, 0b01001110, 0b11001110,
    0b00101110, 0b10101110, 0b01101110, 0b11101110,
    0b00011110, 0b10011110, 0b01011110, 0b11011110,
    0b00111110, 0b10111110, 0b01111110, 0b11111110,
    0b00000001, 0b10000001, 0b01000001, 0b11000001,
    0b00100001, 0b10100001, 0b01100001, 0b11100001,
    0b00010001, 0b10010001, 0b01010001, 0b11010001,
    0b00110001, 0b10110001, 0b01110001, 0b11110001,
    0b00001001, 0b10001001, 0b01001001, 0b11001001,
    0b00101001, 0b10101001, 0b01101001, 0b11101001,
    0b00011001, 0b10011001, 0b01011001, 0b11011001,
    0b00111001, 0b10111001, 0b01111001, 0b11111001,
    0b00000101, 0b10000101, 0b01000101, 0b11000101,
    0b00100101, 0b10100101, 0b01100101, 0b11100101,
    0b00010101, 0b10010101, 0b01010101, 0b11010101,
    0b00110101, 0b10110101, 0b01110101, 0b11110101,
    0b00001101, 0b10001101, 0b01001101, 0b11001101,
    0b00101101, 0b10101101, 0b01101101, 0b11101101,
    0b00011101, 0b10011101, 0b01011101, 0b11011101,
    0b00111101, 0b10111101, 0b01111101, 0b11111101,
    0b00000011, 0b10000011, 0b01000011, 0b11000011,
    0b00100011, 0b10100011, 0b01100011, 0b11100011,
    0b00010011, 0b10010011, 0b01010011, 0b11010011,
    0b00110011, 0b10110011, 0b01110011, 0b11110011,
    0b00001011, 0b10001011, 0b01001011, 0b11001011,
    0b00101011, 0b10101011, 0b01101011, 0b11101011,
    0b00011011, 0b10011011, 0b01011011, 0b11011011,
    0b00111011, 0b10111011, 0b01111011, 0b11111011,
    0b00000111, 0b10000111, 0b01000111, 0b11000111,
    0b00100111, 0b10100111, 0b01100111, 0b11100111,
    0b00010111, 0b10010111, 0b01010111, 0b11010111,
    0b00110111, 0b10110111, 0b01110111, 0b11110111,
    0b00001111, 0b10001111, 0b01001111, 0b11001111,
    0b00101111, 0b10101111, 0b01101111, 0b11101111,
    0b00011111, 0b10011111, 0b01011111, 0b11011111,
    0b00111111, 0b10111111, 0b01111111, 0b11111111,
  };

private:  // private data members

  /**
   * Image data is stored as an array of 8-bit unsigned integers. The integer
   * at index `i` represents row `i` and each bit in the integer represents
   * a column where the most-significant bit represents column index `0`.
   */
  std::array<std::uint_fast8_t, HEIGHT> image_data;

public:  // public methods

  /**
   * Deletion of functions that could potentially be implicitly declared in
   * order to prevent errors from accidental use.
   */
  MatrixImage(MatrixImage&&) = delete;
  MatrixImage& operator=(const MatrixImage&) = delete;
  MatrixImage& operator=(MatrixImage&&) = delete;

  /**
   * @brief Default constructor that constructs a blank image.
   */
  MatrixImage();

  /**
   * @brief Construct an image with the same data as another image.
   * 
   * @param image_to_copy A `MatrixImage` object to copy
   */
  MatrixImage(const MatrixImage& image_to_copy);

  /**
   * @brief Move bits of a row to the left by one place; returns the left-
   *        most bit and sets the right-most bit to a specified value.
   * 
   * This is used to create the visual effect of scrolling an image across
   * the display from right to left. It is designed so that if multiple 
   * images are chained together, one can easily left shift all of the images
   * by started from the right-most image and then using the return value
   * from the function as the `fill_bit` parameter for the next image.
   * 
   * @param row row on which to perform the left-shift and fill operation
   * @param fill_bit bit to set as the new least-significant bit in the row
   * @return most-significant bit in the row prior to left shift operation
   * 
   * @warning Providing a `fill_bit` value that is not 0 or 1 will result in
   *          undefined behavior
   */
  std::uint_fast8_t left_shift_row(std::size_t row, 
                                   std::uint_fast8_t fill_bit);

  /**
   * @brief Sets the pixel at the specified position to the specified value.
   * 
   * @param row row of the pixel to be set
   * @param col column of the pixel to be set
   * @param value `1` to turn the specified pixel on or `0` to turn the
   *              specified pixel off
   */
  void set_pixel(std::size_t row, std::size_t col, std::uint_fast8_t value);

  /**
   * @brief Returns the value of the pixel at the specified position.
   * 
   * @param row row of the pixel to be retrieved
   * @param col column of the pixel to be retrieved
   * @return `1` if the specified pixel is set on or `0` if the pixel is set
   *         off
   */
  std::uint_fast8_t get_pixel(std::size_t row, std::size_t col) const;

  /**
   * @brief Returns the data for an entire row of the this image.
   * 
   * @param row row for which to return image data
   * @return image data for a row where each digit in the binary
   *         representation of the value represents a column within the row
   */
  std::uint_fast8_t get_row(std::size_t row) const;

  /**
   * TODO - this should not be a public method - need to make ChainImage a friend.
   * @brief Sets the data for an entire row of this image.
   * 
   * @param row row for which to set image data
   * @param value value to which to set the data for the specified row
   */
  void set_row(std::size_t row, std::uint_fast8_t value);

  /**
   * @brief Rotates this image in place in increments of 90 degrees.
   * 
   * Creates a new image in place of this image that is made by rotating this
   * image by 0, 90, 180, or 270 degrees. These rotation amounts are achieved
   * by using values `0`, `1`, `2`, and `3` respectively for the value of the
   * parameter `rotation`.
   * 
   * @param rotation number of 90 degree clockwise rotations to perform
   *                 on this `MatrixImage`
   */
  void rotate_image(std::size_t rotation);

};  // class MatrixImage

inline MatrixImage::MatrixImage() : image_data{} { /* no body */ }

inline MatrixImage::MatrixImage(const MatrixImage& image_to_copy) {
  // call the std::array copy assignment operator to copy array values 
  image_data = image_to_copy.image_data;
}

inline std::uint_fast8_t MatrixImage::left_shift_row(
    std::size_t row,
    std::uint_fast8_t fill_bit) {
  
  // store the msb which will be the return value
  std::uint8_t msb{
    static_cast<std::uint8_t>(image_data.at(row) >> (WIDTH - 1))};

  // shift left one bit and fill in the rightmost bit with the fill bit
  image_data.at(row) <<= 1;
  image_data.at(row) |= fill_bit;

  // ensure only the least-significant 8 bits can be set for each row of
  // data because the LED matrix only has 8 columns (pixels) per row
  image_data.at(row) &= 0xFF;

  return msb;

}

inline void MatrixImage::set_pixel(std::size_t row, std::size_t col,
                                   std::uint_fast8_t value) {

  // throw an exception if the column index provided is invalid
  if (col >= WIDTH) {
    throw std::out_of_range{
      std::string{__FILE__} + ":" + std::to_string(__LINE__) + "\t"
      + "col index must be less than matrix width of " 
      + std::to_string(WIDTH) + "; provided value was " 
      + std::to_string(col)
    };
  }

  // set or unset the pixel depending on the boolean value of v; this can be
  // accomplished without the if-else block, but I am assuming the compiler 
  // will optimize that out
  if (value) {
      image_data.at(row) |= 1 << (WIDTH - 1 - col);     
  } else {
      image_data.at(row) &= ~(1 << (WIDTH - 1 - col));
  }

}

inline std::uint_fast8_t MatrixImage::get_pixel(std::size_t row,
                                                std::size_t col) const {

  // throw an exception if the column index provided is invalid
  if (col >= WIDTH) {
    throw std::out_of_range{
      std::string{__FILE__} + ":" + std::to_string(__LINE__) + "\t"
      + "col index must be less than matrix width of " 
      + std::to_string(WIDTH) + "; provided value was " 
      + std::to_string(col)
    };
  }

  // return boolean value (1 or 0) of the pixel in the specified location
  return !!(image_data.at(row) & (1 << (WIDTH - 1 - col)));

}

inline std::uint_fast8_t MatrixImage::get_row(std::size_t row) const {
    return image_data.at(row);
}

inline void MatrixImage::set_row(std::size_t row, std::uint_fast8_t value) {
  image_data.at(row) = value;
}

#endif  // SCROLLER_MATRIX_IMAGE_H_
