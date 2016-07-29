#include "spi.h"

SPI::SPI()
{

}

SPI::~SPI()
{

}

qint16 SPI::openPort(qint8 device)
{
    qint16 status = -1;
    qint8 *csFd;


    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
    //SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
    spiMode = SPI_MODE_0;

    //----- SET BITS PER WORD -----
    spiBitsPerWord = 8;

    //----- SET SPI BUS SPEED -----
    spiSpeed = 400000;		//400kHz (running on 250kHz actually)


    if (device)
        csFd = &cs1Fd;
    else
        csFd = &cs0Fd;


    if (device)
        *csFd = open(SPIDev1, O_RDWR);
    else
        *csFd = open(SPIDev0, O_RDWR);

    if (*csFd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_WR_MODE, &spiMode);
    if(status < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_RD_MODE, &spiMode);
    if(status < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_WR_BITS_PER_WORD, &spiBitsPerWord);
    if(status < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_RD_BITS_PER_WORD, &spiBitsPerWord);
    if(status < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
    if(status < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status = ioctl(*csFd, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
    if(status < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }

    return(status);
}

qint16 SPI::closePort(qint8 device)
{
    int status_value = -1;
    qint8 *cs_fd;

    if (device)
        cs_fd = &cs1Fd;
    else
        cs_fd = &cs0Fd;


    status_value = close(*cs_fd);
    if(status_value < 0)
    {
        perror("Error - Could not close SPI device");
        exit(1);
    }
    return(status_value);
}

qint16 SPI::write(qint8 spi_device, unsigned char *data, qint16 length)
{
    struct spi_ioc_transfer spi[length];
    int i = 0;
    int retVal = -1;
    qint8 *cs_fd;

    if (spi_device)
        cs_fd = &cs1Fd;
    else
        cs_fd = &cs0Fd;

    //one spi transfer for each byte

    for (i = 0 ; i < length ; i++)
    {
        memset(&spi[i], 0, sizeof (spi[i]));
        spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
        spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
        spi[i].len           = sizeof(*(data + i)) ;
        spi[i].delay_usecs   = 100 ;
        spi[i].speed_hz      = spiSpeed ;
        spi[i].bits_per_word = spiBitsPerWord ;
        spi[i].cs_change = 0;
    }

    retVal = ioctl(*cs_fd, SPI_IOC_MESSAGE(length), &spi) ;
    //retVal = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi);
    if(retVal < 0)
    {
        perror("Error - Problem transmitting spi data..ioctl");
        exit(1);
    }

    return retVal;
}

void SPI::read()
{
    int retVal = -1;
    u_int8_t tx[] = {MCP_READ, 0x31, 0x00};
    write(0, tx, sizeof(tx));

    //for(retVal =0; retVal < sizeof(tx); ++retVal)
    //{
    //    printf("Read 0x31: %.2X \n", *(tx+2));
    //}

    u_int8_t rx[] = {MCP_READ, 0x34, 0x00};
    write(0, rx, sizeof(rx));

    for(retVal =0; retVal < sizeof(rx); ++retVal)
    {
        printf("Read 0x31-3D: %.2X \n", *(rx+retVal));
    }
}

void SPI::delay(qint32 counter)
{
    while(counter)
    {
        counter--;
    }
}

void SPI::writeBTest(qint8 value)
{
    u_int8_t rx[] = {MCP_LOAD_TX0+1, value,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D};
    write(0, rx, sizeof(rx));

    delay(50000);

    u_int8_t tx[] = {MCP_WRITE, MCP_TXB0CTRL, 0x0F};
    write(0, tx, sizeof(tx));
}

void SPI::writeTxBtoCAN()
{
    u_int8_t ax[] = {MCP_WRITE,0x3F,0x00};
    write(0, ax, sizeof(ax));

    u_int8_t bx[] = {MCP_WRITE,MCP_CANINTF,0x00};
    write(0, bx, sizeof(bx));

    u_int8_t rx[] = {MCP_WRITE,MCP_TXB0CTRL,0x00};
    write(0, rx, sizeof(rx));

    u_int8_t tx[] = {MCP_WRITE, MCP_TXB0CTRL, 0x0F};
    write(0, tx, sizeof(tx));
}

void SPI::readTxB0CTL()
{
    u_int8_t rx[] = {MCP_READ, MCP_TXB0CTRL,0x00};
    write(0, rx, sizeof(rx));

    printf("Read TxB0CTL: %.2X \n", *(rx+2));
}

void SPI::readByte(unsigned char address)
{
    u_int8_t tx[] = {MCP_READ, address,0x00};
    write(0, tx, sizeof(tx));
    delay(50000);

    printf("Read : %.2X \n",  *(tx+2));
}

void SPI::readCANTF()
{
    readByte(0x3F);

    readByte(MCP_CANINTF);

    readByte(0x0C);

    readByte(0x0D);

    readByte(MCP_CANSTAT);

    readByte(MCP_TEC);

    readByte(MCP_REC);

    readByte(MCP_CNF3);

    readByte(MCP_CNF2);

    readByte(MCP_CNF1);

    readByte(MCP_CANINTE);

    readByte(MCP_EFLG);

    readByte(MCP_TXB1CTRL);

    readByte(MCP_TXB2CTRL);

    readByte(MCP_RXB0CTRL);

    readByte(MCP_RXB1CTRL);
}

void SPI::writeByte(unsigned char address, unsigned char data)
{
    u_int8_t writeData[] =
    {
        MCP_WRITE, address, data,
    };

    write(0, writeData, sizeof(writeData));

    delay(50000);
}

void SPI::initMCP2515()
{
    //initial SPI
    closePort(0);
    openPort(0);

    u_int8_t resetCommand[] = {MCP_RESET};

    //initial MCP2515
    write(0, resetCommand, sizeof(resetCommand));	//Reset MCP2515
//    DelayUs(2000);									//wait for MCP2515 start up
    int counter = 5000;
    while(counter)
    {
        counter--;
    }

    writeByte(MCP_CNF1,def_CNF1);				//setup CNF1~3
    writeByte(MCP_CNF2,def_CNF2);
    writeByte(MCP_CNF3,def_CNF3);

    writeByte(MCP_RXM0SIDH,def_RXM0SIDH);		//setup RXM0
    writeByte(MCP_RXM0SIDL,def_RXM0SIDL);
    writeByte(MCP_RXM0EID8,def_RXM0EID8);
    writeByte(MCP_RXM0EID0,def_RXM0EID0);
    writeByte(MCP_RXF0SIDH,def_RXF0SIDH);		//setup RXF0
    writeByte(MCP_RXF0SIDL,def_RXF0SIDL);
    writeByte(MCP_RXF0EID8,def_RXF0EID8);
    writeByte(MCP_RXF0EID0,def_RXF0EID0);

    writeByte(MCP_RXM1SIDH,def_RXM1SIDH);		//setup RXM1
    writeByte(MCP_RXM1SIDL,def_RXM1SIDL);
    writeByte(MCP_RXM1EID8,def_RXM1EID8);
    writeByte(MCP_RXM1EID0,def_RXM1EID0);
    writeByte(MCP_RXF2SIDH,def_RXF2SIDH);		//setup RXF2
    writeByte(MCP_RXF2SIDL,def_RXF2SIDL);
    writeByte(MCP_RXF2EID8,def_RXF2EID8);
    writeByte(MCP_RXF2EID0,def_RXF2EID0);

    writeByte(MCP_CANCTRL,def_CANCTRL);		//change to normal mode

    writeByte(MCP_TXB0CTRL+1,def_TXB0SIDH);		//setup TXB0
    writeByte(MCP_TXB0CTRL+2,def_TXB0SIDL);
    writeByte(MCP_TXB0CTRL+3,def_TXB0EID8);
    writeByte(MCP_TXB0CTRL+4,def_TXB0EID0);
    writeByte(MCP_TXB0CTRL+5,4);					//DLC
    writeByte(MCP_TXB0CTRL+6,0);
    writeByte(MCP_TXB0CTRL+7,0);
    writeByte(MCP_TXB0CTRL+8,0);
    writeByte(MCP_TXB0CTRL+9,0);

    writeByte(MCP_TXB1CTRL+1,def_TXB1SIDH);		//setup TXB1
    writeByte(MCP_TXB1CTRL+2,def_TXB1SIDL);
    writeByte(MCP_TXB1CTRL+3,def_TXB1EID8);
    writeByte(MCP_TXB1CTRL+4,def_TXB1EID0);
    writeByte(MCP_TXB1CTRL+5,4);					//DLC
    writeByte(MCP_TXB1CTRL+6,0);
    writeByte(MCP_TXB1CTRL+7,0);
    writeByte(MCP_TXB1CTRL+8,0);
    writeByte(MCP_TXB1CTRL+9,0);
}
