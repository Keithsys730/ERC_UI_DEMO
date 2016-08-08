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
//#include "CMU-CanBus.h"

#define SPIDev0 "/dev/spidev0.0"
#define SPIDev1 "/dev/spidev0.1"

class SPI
{
    qint8 cs0Fd;				//file descriptor for the SPI device
    qint8 cs1Fd;				//file descriptor for the SPI device
    quint8 spiMode;
    quint8 spiBitsPerWord;
    quint16 spiSpeed;

public:
    SPI();
    ~SPI();

    void openPort(qint8 device);
    void closePort(qint8 device);
    void write(qint8 device, quint8 *data, qint16 length);
    void delay(qint32 counter);
    void writeBufferTest(quint8 value);
    void writeTxBtoCAN(quint8 buffer);
    quint8 readByte(quint8 address);
    void readSPIConfig();
    void writeByte(quint8 address, quint8 data);
    void initMCP2515();
    void readRXBuffer(quint8 command, quint8 length);
    void clearReadRXBuffer();
    void StartCANbusComm();

};

#endif // SPI_H
