/**
 * @file MatrixChainImage.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include <vector>
#include <exception>

#include "MatrixChainImage.h"

void MatrixChainImage::draw_character(std::uint_fast8_t code_point,
                                      Font& font) {

  draw_character(code_point, cursor_position, font);
  cursor_position += font.get_glyph_width(code_point);
}

void MatrixChainImage::draw_character(std::uint_fast8_t code_point,
                                      std::size_t position, Font& font) {

  // for each row index in the glyph
  for (std::size_t i = 0; i < Glyph::HEIGHT; i++) {

    // for each column in the glyph
    for (std::size_t j = 0; j < font.get_glyph_width(code_point); j++) {
      set_pixel(i, position + j, font.get_pixel(code_point, i, j));
    }
  }
}

std::string MatrixChainImage::draw_text(const std::string& text, Font& font) {
  
  // for each character in the string
  std::size_t i;
  for (i = 0; i < text.length(); i++) {

    // get the codepoint for the letter/glyph that is going to be drawn
    std::uint_fast8_t codepoint{static_cast<std::uint_fast8_t>(text.at(i))};

    // get the amount of space between the cursor and end of the image
    std::size_t remaining_space{get_pixel_width() - cursor_position};

    // if the glyph will not fit on this image
    if (font.get_glyph_width(codepoint) > remaining_space) {
      break;
    }

    // draw the character glyph on the image at the current cursor postion;
    // cast from signed value to unsigned value because there are 256 possible
    // glyphs which are accessed in the Font class via codepoint indices
    draw_character(codepoint, font);

  }

  // return any characters that could not be displayed 
  return text.substr(i, std::string::npos);

}

void MatrixChainImage::left_shift_image() {

  // for each row in the matrices
  for (std::size_t row = 0; row < MatrixImage::HEIGHT; row++) {

    // add a new column of 0's on the rightmost edge
    uint8_t fill_bit = 0;

    // iterate over the matrices in the chain in reverse order
    for (auto matrix_reverse_iter = matrices.rbegin();
         matrix_reverse_iter != matrices.rend(); 
         matrix_reverse_iter++) {

      fill_bit = (*matrix_reverse_iter)->left_shift_row(row, fill_bit);
    }
  }

  // move the cursor to adjust for the movement of the image unless the cursor
  // is already at position 0 in which case it cannot move further to the left
  if (cursor_position != 0) {
    cursor_position--;
  }

}

// NEED TO LOOK AT THIS AGAIN - THIS IMPL MAY NOT BE BEST
void MatrixChainImage::rotate_matrices(std::size_t rotation) {

  // rotate each individual matrix within the chain in place
  for (auto matrix_iter = matrices.begin();
       matrix_iter != matrices.end(); 
       matrix_iter++) {

    (*matrix_iter)->rotate_image(rotation);
  }

}

MatrixChainImage* MatrixChainImage::get_cropped_image(std::size_t length) {

  // create a new, blank chain image
  MatrixChainImage* cropped_image{new MatrixChainImage(length)};

  // for each matrix index in the cropped image
  for (std::size_t matrix = 0; matrix < length; matrix++) {

    // for each row in each matrix
    for (std::size_t row = 0; row < MatrixImage::HEIGHT; row++) {

      // retrieve the row value for this image and store it into the cropped
      // image
      std::uint_fast8_t image_row_value = get_row_of_matrix(matrix, row);
      cropped_image->set_row_of_matrix(matrix, row, image_row_value);
    }
  }
  
  return cropped_image;
}

void MatrixChainImage::rotate_image(std::size_t rotation) {

  // rotating by 180 degrees * rotation is equivalent to rotating by
  // 180 degrees * (rotation % 2)
  switch (rotation % 2) {

  case 0:

    // if we are rotating 180 degrees * 0, there is nothing to do
    break;

  case 1:

    // for each 8x8 matrix in the image
    for (std::size_t matrix{0}; matrix < length; matrix++) {

      // for each of the rows in the top half of the matrix
      for (std::size_t row{0}; row < MatrixImage::HEIGHT / 2; row++) {
        
        // to rotate 180 degrees, we can swap entire rows of a matrix at a 
        // time; we specify two matrix rows in the image that need to be
        // swapped, a and b, and we swap their locations and then reverse the
        // order of the bits in the row

        // location a is an arbitrary location that is bound to the top half of
        // image
        std::size_t swap_matrix_a{matrix};
        std::size_t swap_row_a{row};

        // location b is the location of a if the image were rotated 180
        // degrees; by nature, it is bound to the bottom half of the image
        std::size_t swap_matrix_b{length - 1 - matrix};
        std::size_t swap_row_b{MatrixImage::HEIGHT - 1 - row};

        // get the row values at position a and b
        std::uint_fast8_t row_value_a{
          get_row_of_matrix(swap_matrix_a, swap_row_a)};
        std::uint_fast8_t row_value_b{
          get_row_of_matrix(swap_matrix_b, swap_row_b)};

        // set the value at position b as the reversed value from position a
        set_row_of_matrix(
          swap_matrix_b,
          swap_row_b,
          MatrixImage::reverse_bits_lookup_table[row_value_a]
        );

        // set the value at position a as the reversed value from position b
        set_row_of_matrix(
          swap_matrix_a,
          swap_row_a,
          MatrixImage::reverse_bits_lookup_table[row_value_b]
        );

      }

    }

    break;

  }
}
