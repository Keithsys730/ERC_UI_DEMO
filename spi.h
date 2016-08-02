#ifndef SPI_H
#define SPI_H
#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <unistd.h>			//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <QtGlobal>
#include <MCP2515.h>

#define SPIDev0 "/dev/spidev0.0"
#define SPIDev1 "/dev/spidev0.1"

class SPI
{
    qint8 cs0Fd;				//file descriptor for the SPI device
    qint8 cs1Fd;				//file descriptor for the SPI device
    unsigned char spiMode;
    unsigned char spiBitsPerWord;
    quint16 spiSpeed;

public:
    SPI();
    ~SPI();

    void openPort(qint8 device);
    void closePort(qint8 device);
    void write(qint8 device, unsigned char *data, qint16 length);
    void delay(qint32 counter);
    void writeBufferTest(qint8 value);
    void writeTxBtoCAN(quint8 buffer);
    u_int8_t readByte(unsigned char address);
    void readSPIConfig();
    void writeByte(unsigned char address, unsigned char data);
    void initMCP2515();
    void readRXBuffer();
    void clearReadRXBuffer();

};

#endif // SPI_H
