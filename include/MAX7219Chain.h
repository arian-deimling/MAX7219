/**
 * @file MAX7219Chain.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef MAX7219_CHAIN_H_
#define MAX7219_CHAIN_H_

#include <vector>
#include "SPI.h"
#include "MAX7219.h"
#include "MatrixChainImage.h"

class MAX7219Chain : public MAX7219 {

private:

  /**
   * The number of individual 8x8 LED matrices in the device.
   */
  const std::size_t length;

  /**
   * Number of 90 degree clockwise rotations to make to each individual 8x8
   * matrix in order for images to be displayed properly on the device.
   */
  const std::size_t matrix_orientation;

  /**
   * Whether the physical matrix chain is oriented upside down.
   */
  const bool upside_down;

  /**
   * Intensity (brightness) of the device LEDs.
   */
  char intensity;

public:

  /**
   * Deletion of functions that could potentially be implicitly declared in
   * order to prevent errors from accidental use.
   */
  MAX7219Chain(const MAX7219Chain&) = delete;
  MAX7219Chain(MAX7219Chain&&) = delete;
  MAX7219Chain& operator=(const MAX7219Chain&) = delete;
  MAX7219Chain& operator=(MAX7219Chain&&) = delete;

  MAX7219Chain(std::size_t length, std::size_t matrix_orientation,
               bool upside_down, char intensity);

  ~MAX7219Chain();

  void set_intensity(char intensity); //
  void display_raw(MatrixChainImage& image); //
  void display(MatrixChainImage& image);
  void clear(); //
  void show(); //
  void hide(); //
  void preprocess(MatrixChainImage& image); //
  static std::vector<std::vector<char>*>* image_to_command_vectors(
      MatrixChainImage* image);
  std::vector<std::vector<char>*>* generate_frame(MatrixChainImage* image);
  void send_command_vectors(std::vector<std::vector<char>*>* command_vectors);

private:

  void send_command_all(MAX7219Register device_register, char data); //
  void send_command_all(char register_value, char data);
  void send_command_string(std::vector<char>* command_string); //
  static std::size_t row_register_to_row_index(char device_register); //
  static char row_index_to_row_register(std::size_t index); //

};

#endif  // MAX7219_CHAIN_H_