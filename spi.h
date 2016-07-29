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

    qint16 openPort(qint8 device);
    qint16 closePort(qint8 device);
    qint16 write(qint8 spi_device, unsigned char *data, qint16 length);
    void read();
    void delay(qint32 counter);
    void writeBTest(qint8 value);
    void writeTxBtoCAN();
    void readTxB0CTL();
    void readByte(unsigned char address);
    void readCANTF();
    void writeByte(unsigned char address, unsigned char data);
    void initMCP2515();

};

#endif // SPI_H
