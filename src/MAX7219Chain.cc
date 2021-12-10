/**
 * @file MAX7219Chain.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include "MAX7219Chain.h"
#include "MatrixChainImage.h"
#include "SPI.h"

MAX7219Chain::MAX7219Chain(std::size_t length, std::size_t matrix_orientation,
                           bool upside_down, char intensity)
  : length{length}
  , matrix_orientation{matrix_orientation}
  , upside_down{upside_down}
  , intensity{intensity} {

  // TODO - add return code checking
  // initalize the SPI library functionality
  spi_init();

  // set SPI options
  spi_set_options("", "", "", "");

  // shutdown all of the LED matrices
  hide();

  // set test mode to 'not testing' mode
  send_command_all(
    MAX7219Register::TEST,
    static_cast<char>(TestMode::TEST_OFF)
  );

  // set scan limit to 'display all digits (rows)'
  send_command_all(
    MAX7219Register::SCAN_LIMIT,
    static_cast<char>(ScanLimit::SHOW_ALL_DIGITS)
  );

  // set decode mode to 'no decoding'
  send_command_all(
    MAX7219Register::DECODE_MODE,
    static_cast<char>(DecodeMode::NO_DECODE)
  );

  // set intensity to the specified value
  set_intensity(intensity);

  // send a blank image to all of the LED matrices
  clear();

  // turn low-power mode off which enables the device display
  show();

}

MAX7219Chain::~MAX7219Chain() {
  clear();
  hide();
  spi_close();
}

void MAX7219Chain::send_command_all(MAX7219Register device_register, char data) {
  send_command_all(static_cast<char>(device_register), data);
}

void MAX7219Chain::send_command_all(char register_value, char data) {

  // create a new vector large enough to hold an element for the register and
  // and element for the data for each MAX7219 chip in the chain
  std::vector<char>* command_string = new std::vector<char>(length * 2);

  for (size_t i = 0; i < length; i++) {

    // fill the command string vector with the same command repeated
    command_string->at(2 * i) = register_value;
    command_string->at(2 * i + 1) = data;

  }

  // send command to device and free memory allocated for the command vector
  send_command_string(command_string);

}

void MAX7219Chain::send_command_string(std::vector<char>* command_string) {
  spi_send_data(command_string->data(), command_string->size());
  delete command_string;
}

void MAX7219Chain::clear() {

  // iterate over the indices of each row in the matrix
  for (size_t r = 0; r < MatrixImage::HEIGHT; r++) {

    // send blank row as data to a single row of the device
    send_command_all(row_index_to_row_register(r), 0x00);

  }
}

void MAX7219Chain::show() {
  send_command_all(
    MAX7219Register::SHUTDOWN,
    static_cast<char>(ShutdownMode::DEVICE_ON)
  );
}

void MAX7219Chain::hide() {
  send_command_all(
    MAX7219Register::SHUTDOWN,
    static_cast<char>(ShutdownMode::DEVICE_OFF)
  );
}

void MAX7219Chain::set_intensity(char intensity) {
  send_command_all(MAX7219Register::INTENSITY, intensity);
  this->intensity = intensity;
}

std::size_t MAX7219Chain::row_register_to_row_index(char register_value) {
  // MAX7219 chip uses register values 1-8 for the rows of each LED matrix
  // while the images use row index values 0-7; therefore, subtract 1
  return static_cast<int>(register_value) - 1;
}

char MAX7219Chain::row_index_to_row_register(std::size_t index) {
  // MAX7219 chip uses register values 1-8 for the rows of each LED matrix
  // while the images use row index values 0-7; therefore, add 1 
  return static_cast<char>(index + 1);
}

void MAX7219Chain::display_raw(MatrixChainImage& image) {

  std::vector<char>* command_string{nullptr};

  // image must be displayed one row at a time
  for (std::size_t r = 0; r < MatrixImage::HEIGHT; r++) {

    // create a vector of appropriate length to hold the command string
    command_string = new std::vector<char>(length * 2);

    // iterate over the matricies that make up the image
    for (std::size_t m = 0; m < length; m++) {

      // fill the command string with row register followed by image data
      // for each matrix in the image
      command_string->at(2 * m) = row_index_to_row_register(r);
      command_string->at(2 * m + 1) = image.get_row_of_matrix(m, r);

    }

    // sends command to the device and frees command string memory
    send_command_string(command_string);

  }
}

void MAX7219Chain::preprocess(MatrixChainImage& image) {

  // rotate entire blocks of the supplied image where blocks correspond with
  // physical circuit boards containing multiple 8x8 matrices
  image.rotate_image(static_cast<std::size_t>(upside_down));

  // rotate the 8x8 `MatrixImage`s that make up the `MatrixChainImage` by
  // the amount needed for the image to display properly on this device
  image.rotate_matrices(matrix_orientation);

}

std::vector<std::vector<char>*>* MAX7219Chain::image_to_command_vectors(
    MatrixChainImage* image) {

  // create a 2D vector container to hold a series of commands for each row in
  // the matrix
  std::vector<std::vector<char>*>* command_vectors = 
    new std::vector<std::vector<char>*>(MatrixImage::HEIGHT);

  // for each row of the matrices
  for (std::size_t row = 0; row < MatrixImage::HEIGHT; row++) {

    // create a vector to hold the commands for this row
    command_vectors->at(row) = new std::vector<char>(2 * image->length);

    // for each matrix in the chain
    for (std::size_t matrix = 0; matrix < image->length; matrix++) {

      // add the row register to the command vector
      command_vectors->at(row)->at(2 * matrix) = 
        row_index_to_row_register(row);

      // add the row data to the command vector
      command_vectors->at(row)->at(2 * matrix + 1) = 
        image->get_row_of_matrix(matrix, row);

    }
  }

  delete image;

  return command_vectors;
}

void MAX7219Chain::send_command_vectors(
    std::vector<std::vector<char>*>* command_vectors) {

  // for each row of the matrix
  for (std::size_t row = 0; row < MatrixImage::HEIGHT; row++) {

    // send the command for the row via SPI and deallocate the memory 
    // associated with the row's command vector
    send_command_string(command_vectors->at(row));
  }

  delete command_vectors;
}

void MAX7219Chain::display(MatrixChainImage& image) {

  // get a cropped version of this image
  MatrixChainImage* cropped_image = image.get_cropped_image(length);

  // apply necessary preprocessing to the image (in place)
  preprocess(*cropped_image);

  // create command vectors from the image
  auto image_commands = image_to_command_vectors(cropped_image);
  cropped_image = nullptr;

  // display the image on the matrix
  send_command_vectors(image_commands);
  image_commands = nullptr;

}

std::vector<std::vector<char>*>* MAX7219Chain::generate_frame(
    MatrixChainImage* image) {
  
  // perform necessary transformations to the image
  preprocess(*image);

  // generate command vectors for the image and return a pointer
  return image_to_command_vectors(image);
}