/**
 * @file MatrixImage.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include <array>
#include <cstdint>
#include <memory>
#include <climits>
#include <exception>

#include "MatrixImage.h"

constexpr std::uint_fast8_t MatrixImage::reverse_bits_lookup_table[];

void MatrixImage::rotate_image(std::size_t rotation) {

  // nothing needs to be done if the rotation is by 0 degrees
  if (rotation % 4 == 0) { return; }

  // create an image that is a copy of this image
  std::unique_ptr<MatrixImage> old_image(new MatrixImage(*this));

  switch (rotation % 4) {

  case 1:

    // write old image data rotated 90 degrees clockwise into this image
    for (std::size_t row = 0; row < HEIGHT; row++) {
      for (std::size_t col = 0; col < WIDTH; col++) {
        std::size_t old_row{WIDTH - 1 - col};
        std::size_t old_col{row};
        set_pixel(row, col, old_image->get_pixel(old_row, old_col));
      }
    }
    break;

  case 2:

    // to rotate 180 degrees, reverse the order of the rows and rotate the bits
    // in each of the rows
    for (std::size_t row = 0; row < HEIGHT; row++) {
      std::size_t old_row{HEIGHT - 1 - row};
      set_row(row, reverse_bits_lookup_table[old_image->get_row(old_row)]); 
    }

    // write old image data rotated 180 degrees clockwise into this image
    for (std::size_t row = 0; row < 8; row++) {
      for (std::size_t col = 0; col < 8; col++) {
        std::size_t old_row = HEIGHT - 1 - row;
        std::size_t old_col = WIDTH - 1 - col;
        set_pixel(row, col, old_image->get_pixel(old_row, old_col));
      }
    }
    break;

  case 3:

    // write this image rotated 270 degrees clockwise into new image
    for (int r = 0; r < 8; r++) {
      for (int c = 0; c < 8; c++) {
        int old_r = c;
        int old_c = HEIGHT - 1 - r;
        set_pixel(r, c, old_image->get_pixel(old_r, old_c));
      }
    }
    break;
  }
}