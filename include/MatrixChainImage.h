/**
 * @file MatrixChainImage.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef SCROLLER_MATRIX_CHAIN_IMAGE_H_
#define SCROLLER_MATRIX_CHAIN_IMAGE_H_

#include <vector>
#include <cstdint>

#include "MatrixImage.h"
#include "Font.h"

/**
 * @brief A monochrome image that can be displayed on a chain of 8x8 LED 
 *        Matrices.
 * 
 * The class has processing capabilities in order to assist with display and 
 * modification of the image. In addition, in conjunction with the font class,
 * text can be written onto the image.
 * 
 * Visual Representation of the layout of pixels within the `MatrixChainImage`
 * with the characters 'H' and 'E' written onto the image with 1-px spacing.
 * ```
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * |    C |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |  13 |  14 |  15 |
 * | R    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 0    |  █  |  █  |     |     |  █  |  █  |     |  █  |  █  |  █  |  █  |  █  |  █  |  █  |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 1    |  █  |  █  |     |     |  █  |  █  |     |     |  █  |  █  |     |     |     |  █  |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 2    |  █  |  █  |     |     |  █  |  █  |     |     |  █  |  █  |     |  █  |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 3    |  █  |  █  |  █  |  █  |  █  |  █  |     |     |  █  |  █  |  █  |  █  |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 4    |  █  |  █  |     |     |  █  |  █  |     |     |  █  |  █  |     |  █  |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 5    |  █  |  █  |     |     |  █  |  █  |     |     |  █  |  █  |     |     |     |  █  |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 6    |  █  |  █  |     |     |  █  |  █  |     |  █  |  █  |  █  |  █  |  █  |  █  |  █  |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * | 7    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
 * +------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
 * ```
 */
class MatrixChainImage {

public:  // public data members

  /**
   * The number of `MatrixImage`s in the chain; generally this is longer
   * than the physical display to allow for creation of a frame buffer.
   */
  const std::size_t length;

private:  // private data members

  /**
   * Image data is stored in individual `MatrixImage`s.
   */
  std::vector<MatrixImage*> matrices;

  /**
   * Position (column) at which subsequent characters will be added when
   * drawing a string onto the image.
   */
  std::size_t cursor_position;

public:  // public methods

  /**
   * Deletion of functions that could potentially be implicitly declared in
   * order to prevent errors from accidental use.
   */
  MatrixChainImage() = delete;
  MatrixChainImage(const MatrixChainImage&) = delete;
  MatrixChainImage(MatrixChainImage&&) = delete;
  MatrixChainImage& operator=(const MatrixChainImage&) = delete;
  MatrixChainImage& operator=(MatrixChainImage&&) = delete;

  /**
   * @brief Constructs a blank image of the specified length in matrices.
   * 
   * @param length number of 8x8 `MatrixImage`s that make up this image
   */
  MatrixChainImage(std::size_t length);

  /**
   * @brief `MatrixChainImage` destructor.
   */
  ~MatrixChainImage();

  /**
   * @brief Sets the pixel at the specified position, in the context of the
   *        entire image, to the specified value.
   * 
   * @param row row of the pixel to be set
   * @param col column of the pixel to be set
   * @param value `1` to turn the specified pixel on or `0` to turn the
   *              specified pixel off
   */
  void set_pixel(std::size_t row, std::size_t col, std::uint_fast8_t value);

  /**
   * @brief Returns the value of the pixel at the specified position in the
   *        context of the entire image.
   * 
   * @param row row of the pixel to be retrieved
   * @param col column of the pixel to be retrieved
   * @return `1` if the specified pixel is set on or `0` if the pixel is set
   *         off
   */
  std::uint_fast8_t get_pixel(std::size_t row, std::size_t col) const;

  /**
   * @brief Returns the data for an entire row of a specified matrix within
   *        the image.
   * 
   * @param matrix index of the matrix within this image to retrieve data from
   * @param row row for which to return image data
   * @return image data for a row where each digit in the binary
   *         representation of the value represents a column within the row
   */
  std::uint_fast8_t get_row_of_matrix(std::size_t matrix,
                                      std::size_t row) const;

  /**
   * @brief Returns the width of the image in pixels.
   * 
   * @return width of the image in pixels
   */
  std::size_t get_pixel_width() const;

  /**
   * @brief Draw the specified text on the display using the specified font.
   * 
   * @param text string to draw onto the display
   * @param font font to use to draw characters from the text onto the display
   * @return a substring of the orginal string representing the characters that
   *         could not fit on this image
   */
  std::string draw_text(const std::string& text, Font& font);

  /**
   * @brief Move each pixel one pixel to the left - creates scrolling visual
   *        effect if called successively at constant intervals.
   */
  void left_shift_image();

  /**
   * @brief Transform this image into one in which each 8x8 section of the
   *        image has been rotated by the specified number of degrees.
   * 
   * See @ref MatrixImage `.getImageRotated()` method for more information.
   * 
   * @param rotation number of 90 degree clockwise rotations to do on the image
   */
  void rotate_matrices(std::size_t rotation);

  /**
   * @brief Transform this image by rotating it 180 (or 0) degrees.
   * 
   * @param rotation number of 180 degree rotations to apply to the image
   */
  void rotate_image(std::size_t rotation);

  /**
   * Create a cropped version of this image that is made up of `length` 8x8
   * `MatrixImage`s (deep copy).
   * 
   * @param length length of the cropped image in `MatrixImage`s (8x8 images)
   * @return pointer to a new chain image
   */
  MatrixChainImage* get_cropped_image(std::size_t length);

private:  // private methods

  /**
   * @brief Sets the pixel at the specified position, in the context of the
   *        specified matrix, to the specified value.
   * 
   * @param matrix matrix on which the specified pixel exists
   * @param row row on the matrix of the pixel to be set
   * @param col column on the matrix of the pixel to be set
   * @param value `1` to turn the specified pixel on or `0` to turn the
   *              specified pixel off
   */
  void set_pixel(std::size_t matrix, std::size_t row, std::size_t col, 
                 std::uint_fast8_t value);

  /**
   * @brief Returns the value of the pixel at the specified position in the
   *        context of a specified matrix.
   * 
   * @param matrix matrix on which the specified pixel exists
   * @param row row on the matrix of the pixel to be retrieved
   * @param col column on the matrix of the pixel to be retrieved
   * @return `1` if the specified pixel is set on or `0` if the pixel is set
   *         off
   */
  std::uint_fast8_t get_pixel(std::size_t matrix, std::size_t row, 
                              std::size_t col) const;

  /**
   * @brief Sets the data for an entire row of a specified matrix within the
   * image.
   * 
   * @param matrix index of the matrix within this image to set data for
   * @param row row for which to set image data
   * @param value value to store as the data for the specified row and matrix
   */
  void set_row_of_matrix(std::size_t matrix, std::size_t row,
                         std::uint_fast8_t value);

  /**
   * @brief Draws the character with the specified UTF-8 code point onto
   *        this image at the current cursor position.
   * 
   * This method also updates the cursor position based on the width of the
   * `Glyph` associated with the specified code point.
   * 
   * @param code_point UTF-8 code point of the character to be drawn
   * @param font font to use to draw the character
   */
  void draw_character(std::uint_fast8_t code_point, Font& font);

  /**
   * @brief Draws the character with the specified UTF-8 code point onto
   *        this image at the specified position.
   * 
   * @param code_point UTF-8 code point of the character to be drawn
   * @param position column of the image at which to begin drawing
   * @param font font to use to draw the character
   */
  void draw_character(std::uint_fast8_t code_point, std::size_t position,
                      Font& font);

};  // class MatrixChainImage

inline MatrixChainImage::MatrixChainImage(std::size_t length)
  : length{length}
  , matrices(length, nullptr)
  , cursor_position{0} {

  // fill the array of `MatrixImage`s with blank images
  for (std::size_t i = 0; i < this->length; i++) {
    matrices.at(i) = new MatrixImage();
  }

}

inline MatrixChainImage::~MatrixChainImage() {
  for (std::size_t i = 0; i < matrices.size(); i++) {
    // free memory allocated for image components
    delete matrices.at(i);
  }
}

inline void MatrixChainImage::set_pixel(std::size_t row, std::size_t col,
                                        std::uint_fast8_t value) {

  set_pixel(col / MatrixImage::WIDTH, row, col % MatrixImage::WIDTH, value);
}

inline void MatrixChainImage::set_pixel(std::size_t matrix, std::size_t row,
                                        std::size_t col, 
                                        std::uint_fast8_t value) {

  matrices.at(matrix)->set_pixel(row, col, value);
}

inline std::uint_fast8_t MatrixChainImage::get_pixel(std::size_t row,
                                                     std::size_t col) const {

  return get_pixel(col / MatrixImage::WIDTH, row, col % MatrixImage::WIDTH);
}

inline std::uint_fast8_t MatrixChainImage::get_row_of_matrix(
      std::size_t matrix, std::size_t row) const {

  return matrices.at(matrix)->get_row(row);
}

inline std::size_t MatrixChainImage::get_pixel_width() const {
  return length * MatrixImage::WIDTH;
}

inline std::uint_fast8_t MatrixChainImage::get_pixel(std::size_t matrix, 
                                                     std::size_t row,
                                                     std::size_t col) const {
  return matrices.at(matrix)->get_pixel(row, col);
}

inline void MatrixChainImage::set_row_of_matrix(std::size_t matrix, 
                                                std::size_t row,
                                                std::uint_fast8_t value) {
  
  matrices.at(matrix)->set_row(row, value);
}

#endif  // SCROLLER_MATRIX_CHAIN_IMAGE_H_
