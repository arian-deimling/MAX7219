#ifdef _WIN32
    #include <Windows.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
    #define sleep(x) usleep(1000*x)
#endif

#include <vector>
#include "MAX7219Chain.h"
#include "MatrixChainImage.h"
#include "Font.h"


int main() {

    // number of MAX7219 chips/8x8 matrices on the physical display
    const int DEVICE_LENGTH{8};

    // construct an image and device
    MatrixChainImage my_chain{40};

    /*
    MAX7219Chain(std::size_t length, std::size_t matrix_orientation,
                 bool upside_down, char intensity);
    */

    MAX7219Chain device{DEVICE_LENGTH, 0, true, 0};

    std::cout << "constructed" << std::endl;

    // construct a font from the specified font file
    Font cp437("./cp437.scrollerfont", true, 1);
    
    std::cout << "font" << std::endl;

    // draw some text on the image
    std::string text_to_draw{
        "        HELLO, MY NAME IS ARIAN AND I WANT YOU TO ENJOY THIS PROGRAM "
        "THAT I HAVE WRITTEN! IT IS HONESTLY QUITE COOL!"
    };
    
    // create a vector to hold pre-generated frames
    std::vector<std::vector<std::vector<char>*>*> frames;

    do {

        text_to_draw = my_chain.draw_text(text_to_draw, cp437);

        for (std::size_t i{0}; i < 32; i++) {

            // generate a frame from the image and store it
            frames.push_back(device.generate_frame(
                my_chain.get_cropped_image(DEVICE_LENGTH)));
            
            // shift the image in preparation for generating the next frame
            my_chain.left_shift_image();
        }

    // generate frames until all of the text has been written to the display
    } while (text_to_draw != "");

    // generate additional frames to ensure the entire message is displayed
    // and scrolled entirely across the display
    for (std::size_t i{0}; i < my_chain.length * MatrixImage::WIDTH; i++) {

        // generate a frame from the image and store it
        frames.push_back(device.generate_frame(
            my_chain.get_cropped_image(DEVICE_LENGTH)));
        
        // shift the image in preparation for generating the next frame
        my_chain.left_shift_image();
    }

    for (auto frame_data : frames) {
        sleep(100);
        device.send_command_vectors(frame_data);
    }

    std::cout << text_to_draw << std::endl;

    sleep(5000);

}