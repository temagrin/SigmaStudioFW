#ifndef __SIGMASTUDIOFW_H__
#define __SIGMASTUDIOFW_H__
#include <Arduino.h>


#if USE_SPI
#include <SPI.h>
#else
#include <Wire.h>
#endif

#define ADI_DATA_U16 const PROGMEM uint16_t
#define ADI_REG_TYPE const PROGMEM uint8_t


/*
 * Parameter data format
 */
#define SIGMASTUDIOTYPE_FIXPOINT 0
#define SIGMASTUDIOTYPE_INTEGER  1

 /*
  * Convert a floating-point value to SigmaDSP (5.23 or 8.24) fixed point format
  */
#if DSP_TYPE == DSP_TYPE_SIGMA300_350
int32_t SIGMASTUDIOTYPE_FIXPOINT_CONVERT(double value) { return int32_t(value * (0x01 << 24)); }
#else
int32_t SIGMASTUDIOTYPE_FIXPOINT_CONVERT(double value) { return int32_t(value * (0x01 << 23)) & 0xFFFFFFF; }
#endif

// For compatibility with certain export files, redirect SIGMASTUDIOTYPE_8_24_CONVERT to
// SIGMASTUDIOTYPE_FIXPOINT_CONVERT
#define SIGMASTUDIOTYPE_8_24_CONVERT(x) SIGMASTUDIOTYPE_FIXPOINT_CONVERT(x)

// Separate a 32-bit floating point value into four bytes
void SIGMASTUDIOTYPE_REGISTER_CONVERT(int32_t fixpt_val, byte dest[4]) {
    dest[0] = (fixpt_val >> 24) & 0xFF;
    dest[1] = (fixpt_val >> 16) & 0xFF;
    dest[2] = (fixpt_val >> 8) & 0xFF;
    dest[3] = (fixpt_val) & 0xFF;
}

// The I2C buffer declared by Arduino is 32 bytes long by default. Adjust for your processor.
// Longer buffers use more microcontroller RAM, but allow faster programming
// because I2C overhead is lower.
// The two address bytes shorten the data burst size by 2 bytes.
const int MAX_I2C_DATA_LENGTH = 30;

/** Return the depth (in bytes) of a certain DSP memory location.
 * Currently this function is only implemented for data memory and program memory.
 * Control registers are not included.
 * Function is only required for I2C; it exists because of buffer size limitations in the Teensy I2C library.
 */


#if USE_SPI == false
byte getMemoryDepth(uint32_t address) {
#if DSP_TYPE == DSP_TYPE_SIGMA100
    if (address < 0x0400)
        return 4;    // Parameter RAM is 4 bytes deep
    else {
        return 5;    // Program RAM is 5 bytes deep
    }
#elif DSP_TYPE == DSP_TYPE_SIGMA200
    // Based on ADAU1761
    if (address < 0x0800) {
        return 4;    // Parameter RAM is 4 bytes deep
    }
    else {
        return 5;
    }
#elif (DSP_TYPE == DSP_TYPE_SIGMA300_350)
    if (address < 0xF000) {
        return 4;    // Program Memory, DM0, and DM1 all store 4 bytes (ADAU1463 datasheet
                     // page 90)
    }
    else {
        return 2;    // Control registers all store 2 bytes (ADAU1463 datasheet page 93)
    }
#else
    return 0;    // We should never reach this return
#endif
}
#endif


void SIGMA_WRITE_REGISTER_BLOCK(byte devAddress, int address, int length, byte pData[]) {
#if USE_SPI    // Start of function for SPI write
    // Assert SPI slave select line
    digitalWrite(DSP_SS_PIN, LOW);        // Assert SPI slave select line (active low)
    SPI.beginTransaction(settingsA);      // Initialize SPI
    SPI.transfer(0x0);                    // SPI read address + read/!write bit
    SPI.transfer(address >> 8);           // High byte of address
    SPI.transfer(address & 0xff);         // Low byte of address
    for (int i = 0; i < length; i++) {    // For each data byte in the packet...
        SPI.transfer(pData[i]);           // Write the data byte to the DSP
    }
    SPI.endTransaction();              // Release the SPI bus
    digitalWrite(DSP_SS_PIN, HIGH);    // Pull up the slave select line
// End of function for SPI write
#else     // Start of function for I2C write


     // If data can fit into one I2C buffer, send it in one burst.
    if (length < MAX_I2C_DATA_LENGTH) {
        Wire.beginTransmission(DSP_I2C_ADDR);    // Initialize a new I2C transmission at
                                                 // DSP_I2C_ADDR (adjust in USER_DEFINES.h)
        Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
        Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
        Wire.write(pData, length);               //  Add the whole data packet to the I2C buffer
        Wire.endTransmission();                  // Send the entire I2C transmission to the DSP
    }
    else {
        // Go through the transmission one byte at a time.
        int currentByte = 0;

        while (currentByte < length) {
            // Begin a new I2C transaction.
            Wire.beginTransmission(DSP_I2C_ADDR);
            Wire.write(address >> 8);
            Wire.write(address & 0xff);

            int bytesTransmitted = 0;
            // For each byte, find out if it will fit into the current transaction, and
            // make sure there is still data left to send.
            while ((bytesTransmitted + getMemoryDepth(uint32_t(address)) <= MAX_I2C_DATA_LENGTH) &&
                (currentByte < length)) {
                // If another register will fit inside the current I2C burst write,
                // add each byte to the I2C buffer one at a time.
                for (byte i = 0; i < getMemoryDepth(uint32_t(address)); i++) {
                    Wire.write(pData[currentByte++]);
                    bytesTransmitted++;
                }
                // Increment address once per written register.
                address++;
            }
            Wire.endTransmission();    // Now that the buffer is full, send the I2C burst.
            if (currentByte == length) {
                break;
            }
        }
    }
#endif    // End of function for I2C write
}

// use for progmem type
void SIGMA_WRITE_REGISTER_BLOCK(byte devAddress, int address, int length, const uint8_t pData[]) {
#if USE_SPI    // Start of function for SPI write
    // Assert SPI slave select line
    digitalWrite(DSP_SS_PIN, LOW);        // Assert SPI slave select line (active low)
    SPI.beginTransaction(settingsA);      // Initialize SPI
    SPI.transfer(0x0);                    // SPI read address + read/!write bit
    SPI.transfer(address >> 8);           // High byte of address
    SPI.transfer(address & 0xff);         // Low byte of address
    for (int i = 0; i < length; i++) {    // For each data byte in the packet...
        SPI.transfer(pgm_read_byte_near(pData, i));           // Write the data byte to the DSP
    }
    SPI.endTransaction();              // Release the SPI bus
    digitalWrite(DSP_SS_PIN, HIGH);    // Pull up the slave select line
// End of function for SPI write
#else     // Start of function for I2C write


     // If data can fit into one I2C buffer, send it in one burst.
    if (length < MAX_I2C_DATA_LENGTH) {
        Wire.beginTransmission(DSP_I2C_ADDR);    // Initialize a new I2C transmission at
        Wire.write(address >> 8);                // Add high byte of address to the I2C buffer
        Wire.write(address & 0xff);              // Add low byte of address to the I2C buffer
        for (int i=0;i<length;i++){
            Wire.write(pgm_read_byte_near(pData+i));               //  Add the whole data packet to the I2C buffer
        }
        Wire.endTransmission();                  // Send the entire I2C transmission to the DSP
    }
    else {
        // Go through the transmission one byte at a time.
        int currentByte = 0;

        while (currentByte < length) {
            // Begin a new I2C transaction.
            Wire.beginTransmission(DSP_I2C_ADDR);
            Wire.write(address >> 8);
            Wire.write(address & 0xff);
            
            int bytesTransmitted = 0;
            // For each byte, find out if it will fit into the current transaction, and
            // make sure there is still data left to send.
            while ((bytesTransmitted + getMemoryDepth(uint32_t(address)) <= MAX_I2C_DATA_LENGTH) &&
                (currentByte < length)) {
                // If another register will fit inside the current I2C burst write,
                // add each byte to the I2C buffer one at a time.
                for (byte i = 0; i < getMemoryDepth(uint32_t(address)); i++) {
                    Wire.write(pgm_read_byte_near(pData+currentByte)); 
                    currentByte++;
                    bytesTransmitted++;
                }
                // Increment address once per written register.
                address++;
            }
            Wire.endTransmission();    // Now that the buffer is full, send the I2C burst.
            if (currentByte == length) {
                break;
            }
        }
    }
#endif
}

// Alternative function call without the address (single dsp system)
void SIGMA_WRITE_REGISTER_BLOCK(int address, int length, byte pData[]) {
    SIGMA_WRITE_REGISTER_BLOCK(DSP_I2C_ADDR, address, length, pData);
}

// Write a 32-bit integer to the DSP. NOTE: 5.23 not supported quite yet.
void SIGMA_WRITE_REGISTER_INTEGER(int address, int32_t pData) {
    byte byte_data[4];
    SIGMASTUDIOTYPE_REGISTER_CONVERT(pData, byte_data);
    SIGMA_WRITE_REGISTER_BLOCK(address, 4, byte_data);
}

void SIGMA_WRITE_REGISTER_FLOAT(int address, double pData) {
    SIGMA_WRITE_REGISTER_INTEGER(address, SIGMASTUDIOTYPE_FIXPOINT_CONVERT(pData));
}

void SIGMA_WRITE_DELAY(byte devAddress, int length, byte pData[]) {
    int delay_length = 0;    // Initialize delay length variable
    for (byte i = length; i > 0; i--) {
        // Unpack pData to calculate the delay length as an integer
        delay_length = (delay_length << 8) + pData[i];
    }
    delay(delay_length);    // Delay this processor (not the DSP) by the appropriate time
}

void SIGMA_WRITE_DELAY(byte devAddress, int length, const uint8_t pData[]) {
    int delay_length = 0;    // Initialize delay length variable
    for (byte i = length; i > 0; i--) {
        // Unpack pData to calculate the delay length as an integer
        delay_length = (delay_length << 8) + pgm_read_byte_near(pData+i);
    }
    delay(delay_length);    // Delay this processor (not the DSP) by the appropriate time
}

// Function to read back data from the DSP, not called by SigmaStudio export files
void SIGMA_READ_REGISTER_BYTES(int address, int length, byte* pData) {
#if USE_SPI
    digitalWrite(DSP_SS_PIN, LOW);
    SPI.beginTransaction(settingsA);
    // SPI read address + read/!write bit
    SPI.transfer(0x1);
    SPI.transfer(address >> 8);
    SPI.transfer(address & 0xff);
    for (int i = 0; i < length; i++) {
        pData[i] = SPI.transfer(0);
    }
    SPI.endTransaction();
    digitalWrite(DSP_SS_PIN, HIGH);
#else
    Wire.beginTransmission(DSP_I2C_ADDR);
    Wire.write(address >> 8);
    Wire.write(address & 0xff);
    Wire.endTransmission(false);
    Wire.requestFrom(DSP_I2C_ADDR, length);
    for (int i = 0; i < length; i++) {
        if (Wire.available()) pData[i] = Wire.read();
    }
#endif
}

int32_t SIGMA_READ_REGISTER_INTEGER(int address, int length) {
    int32_t result = 0;
    byte register_value[length];
    SIGMA_READ_REGISTER_BYTES(address, length, register_value);
    for (int i = 0; i < length; i++) {
        result = (result << 8) + register_value[i];
    }
    return result;
}

double SIGMA_READ_REGISTER_FLOAT(int address) {
    int32_t integer_val = SIGMA_READ_REGISTER_INTEGER(address, 4);
#if DSP_TYPE == DSP_TYPE_SIGMA300_350
    return double(integer_val) / (1 << 24);
#else
    return double(integer_val) / (1 << 23);
#endif
}

// Function for reading and printing DSP registers to the serial port, not called by
// SigmaStudio export files
// Since memory must be allocated for register_value bytes, keep dataLength to a low value
void SIGMA_PRINT_REGISTER(int address, int dataLength) {
    Serial.print("VALUE AT 0x");
    Serial.print(address, HEX);
    Serial.print(": 0x");
    byte register_value[dataLength] = {};
    SIGMA_READ_REGISTER_BYTES(address, dataLength, register_value);
    for (int i = 0; i < dataLength; i++) {
        if (register_value[i] < 16) {
            Serial.print('0');
        }
        Serial.print(register_value[i], HEX);
        Serial.print(' ');
    }
    Serial.println();
}

/* SIGMASTUDIOTYPE_INTEGER_CONVERT is included in export files but not generally required.
 * Here it is simply a passthrough macro.
 */
#define SIGMASTUDIOTYPE_INTEGER_CONVERT(_value) (_value)

#endif
