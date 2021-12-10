/**
 * @file MAX7219.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef MAX7219_H_
#define MAX7219_H_

#include <vector>
#include "SPI.h"
#include "MatrixChainImage.h"

class MAX7219 {

public:

    /**
     * Destructor is declared as virtual function to prevent instantiation
     * of this class which is intended to be used as an abstract class.
     */
    virtual ~MAX7219() = 0;

protected:

    /**
     * Register addresses for the MAX7219 chip.
     * 
     * The rows of the LED matrix connected to the MAX7219 chip are controlled
     * using registers 0x01 - 0x08 for rows 1 through 8 respectively.
     */
    enum class MAX7219Register {
        NO_OP       = 0x00,
        DECODE_MODE = 0x09,
        INTENSITY   = 0x0A,
        SCAN_LIMIT  = 0x0B,
        SHUTDOWN    = 0x0C,
        TEST        = 0x0F,
    };

    /**
     * Decode mode for the MAX7219 used for 7-segment LED displays.
     */
    enum class DecodeMode {
        NO_DECODE           = 0x00,
        DECODE_ROW_1_ONLY   = 0x01,
        DECODE_ROW_1_2_3_4  = 0x0F,
        DECODE_ALL          = 0xFF,
    };

    /**
     * Minimum and Maximum values for the intensity of the LEDs in the display.
     */
    enum class Intensity {
        MIN = 0x00,
        MAX = 0x0F,
    };

    /**
     * Limit MAX7219 output to display only on certain digits in a 7-segment 
     * display; digits on a 7-segment are equivalent to rows on an LED matrix.
     */
    enum class ScanLimit {
        SHOW_DIGIT_1_ONLY           = 0x00,
        SHOW_DIGIT_1_2              = 0x01,
        SHOW_DIGIT_1_2_3            = 0x02,
        SHOW_DIGIT_1_2_3_4          = 0x03,
        SHOW_DIGIT_1_2_3_4_5        = 0x04,
        SHOW_DIGIT_1_2_3_4_5_6      = 0x05,
        SHOW_DIGIT_1_2_3_4_5_6_7    = 0x06,
        SHOW_ALL_DIGITS             = 0x07,
    };

    /**
     * Low power mode options; used to turn low power mode on or off.
     */
    enum class ShutdownMode {
        DEVICE_OFF  = 0x00,
        DEVICE_ON   = 0x01,
    };

    /**
     * Test mode options; used to turn test mode (all LEDs on) on or off.
     */
    enum class TestMode {
        TEST_OFF    = 0x00,
        TEST_ON     = 0x01,
    };

};

inline MAX7219::~MAX7219() {}

#endif  // MAX7219_H_