#ifndef __USER_SETTINGS_H__
#define __USER_SETTINGS_H__

#define DSP_TYPE_SIGMA100     1    // Sigma100: ADAU1701/ADAU1702/ADAU1401
#define DSP_TYPE_SIGMA200     2    // Sigma200: ADAU176x/ADAU178x/ADAU144x
#define DSP_TYPE_SIGMA300_350 3    // Sigma300/Sigma350: ADAU145x, ADAU146x

// Which type of DSP is connected?
#define DSP_TYPE DSP_TYPE_SIGMA100

/** If true, the DSP is programmed over SPI. If false, the DSP is programmed over I2C.
 * NOTE: On the example schematic I2C and SPI pins from the microcontroller are connected to
 * the same pins (because Sigma300/Sigma350 multiplex the I2C and SPI pins). The setup
 * function in main.cpp tristates the unused interface to avoid contention between I2C
 * and SPI pins.
 * NOTE: Sigma100 and Sigma200 place I2C on different USBi pins than Sigma300/Sigma350, again due
 * to pin multiplexing. This is why SDA and SCL will each appear twice on the USBi header in
 * the example schematic.
 */

/** Set the SPI bus to run at a clock speed of 1MHz.
 * Very high SPI speeds (>15 MHz) may require shorter programmer cabling for sufficient signal
 * integrity. Keep the SPI speed low (~1MHz) the first time programming the DSP, once the code is
 * working try increasing the bus speed. SPI signal integrity should be verified on an oscilloscope. Some
 * SigmaDSPs have higher maximum SPI speeds than others. Check the datasheet of your DSP for the maximum
 * allowed SPI speed.
 */
#define SPI_SPEED 1000000L
/**
 * Set the I2C bus to run at a clock speed of 400kHz (fast mode). I2C signal integrity
 * should be verified on an oscilloscope. Don't forget I2C pullup resistors!
 */
#define I2C_SPEED 400000L

#define USE_SPI false

#if USE_SPI
#include <SPI.h>
const SPISettings settingsA(SPI_SPEED, MSBFIRST, SPI_MODE3);// Store SPI transaction information to settingsA
#endif

// Store mapping to USBi header here
#define DSP_RESET_PIN 9    // Any GPIO will work
#define DSP_SS_PIN    10

/** By default, SPI instantiates to the main SPI port.
 * MOSI -> pin 11
 * MISO -> pin 12
 * SLCK -> pin 13
 * NOTE: Pin 13 is the built in LED pin on Teensy 4. Lift the LED off the board to avoid unwanted
 * loading of the SCLK pin. You may also change the SPI initializer to use alternate pins. Not
 * every pin can be used as an alternate; refer to Teensy documentation.
 *
 * By default, I2C is mapped to hardware I2C instance 0.
 * SCL -> pin 19
 * SDA -> pin 18
 */

/** If SigmaStudio is set to use the DSP in SPI mode (default for Sigma300/Sigma350), the
 * SigmaStudio exported device address is 0x00. This causes issues if the microcontroller needs to
 * program the DSP over I2C. If OVERRIDE_SIGMASTUDIO_DEVICE_ADDRESS is true, SigmaStudioFW.cpp will
 * use the following device address instead of the one passed to it by argument. The Wire library
 * uses 7-bit I2C addressing, so the R/W bit is omitted from the device address.
 * NOTE: Manually overwriting DEVICE_ADDR_IC_1 in the SigmaStudio exported file is not a good idea
 * because the file will be re-created scratch every time a new SigmaStudio export is performed. It
 * is better to store this configuration data here. NOTE: Additional software logic will be needed
 * if more than one SigmaDSP IC is connected simultaneously.
 */
#define OVERRIDE_SIGMASTUDIO_DEVICE_ADDRESS true
#if OVERRIDE_SIGMASTUDIO_DEVICE_ADDRESS
#if DSP_TYPE == DSP_TYPE_SIGMA100
const int DSP_I2C_ADDR = 0b0110100;    // From ADAU1701 datasheet Table 15; assumes ADDR1 and ADDR0
                                       // low (eval board); adjust for your application.
#elif DSP_TYPE == DSP_TYPE_SIGMA200
const int DSP_I2C_ADDR = 0b0111000;    // From ADAU1761 datasheet Table 21; assumes ADDR1 and ADDR0
                                       // low (eval board); adjust for your application.
#elif DSP_TYPE == DSP_TYPE_SIGMA300_350
const int DSP_I2C_ADDR = 0b0111000;    // From ADAU1463 datasheet Table 26; assumes ADDR1 and ADDR0
                                       // low (eval board); adjust for your application.
#endif
#endif

#endif    // End of USER_SETTINGS.h
