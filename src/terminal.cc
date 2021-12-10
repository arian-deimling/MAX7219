#ifdef _WIN32
    #include <Windows.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
    #define sleep(x) usleep(1000*x)
#endif

#include "MatrixChainImage.h"
#include "Font.h"

void print_matrix_chain_image(const MatrixChainImage& img);

int main() {

    MatrixChainImage my_chain{16};
    Font cp437("./cp437.scrollerfont", true, 1);

    my_chain.draw_text("Hello World ", cp437);

    print_matrix_chain_image(my_chain);
    // my_chain.left_shift_image();
    // print_matrix_chain_image(my_chain);
    // my_chain.left_shift_image();
    // print_matrix_chain_image(my_chain);


    for (int i = 0; i < 10; i++) {
        sleep(100);
        my_chain.left_shift_image();
        print_matrix_chain_image(my_chain);
    }

    my_chain.draw_text("Bye!", cp437);

    for (int i = 0; i < 102; i++) {
        sleep(100);
        my_chain.left_shift_image();
        print_matrix_chain_image(my_chain);
    }

    // my_chain.get_rotated_matrices_image(1).print();

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         std::cout << (int) cp437.get_pixel('H', i, j);
    //     }
    //     std::cout << std::endl;
    // }

//     my_chain.left_shift_image();
//     my_chain.left_shift_image();
//     my_chain.left_shift_image();
//     my_chain.left_shift_image();

//     my_chain.print();

//     MatrixChainImage my_processed_chain = my_chain.get_rotated_matrices_image(1);

//     my_processed_chain.print();
}

void print_matrix_chain_image(const MatrixChainImage& img) {
    for (int r = 0; r < MatrixImage::HEIGHT; r++) {
        for (int c = 0; c < img.get_pixel_width(); c++) {
            if (img.get_pixel(r, c)) {
                std::cout << static_cast<char>(42);
            } else {
                std::cout << static_cast<char>(32);
            }
        }
        std::cout << std::endl;
    }
}