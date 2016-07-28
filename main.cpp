#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QThread>
#include "settingdialog.h"
#include "statusdialog.h"
#include "logdialog.h"
#include "informationdialog.h"
#include "keyboard.h"
#include <QTranslator>

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

#include "MCP2515.h"

//***********************************
//***********************************
//********** SPI OPEN PORT **********
//***********************************
//***********************************
//spi_device	0=CS0, 1=CS1
int SpiOpenPort (int spi_device)
{
    int status_value = -1;
    int *spi_cs_fd;


    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
    //SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
    spi_mode = SPI_MODE_0;

    //----- SET BITS PER WORD -----
    spi_bitsPerWord = 8;

    //----- SET SPI BUS SPEED -----
    spi_speed = 400000;		//1000000 = 1MHz (1uS per bit)


    if (spi_device)
        spi_cs_fd = &spi_cs1_fd;
    else
        spi_cs_fd = &spi_cs0_fd;


    if (spi_device)
        *spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
        *spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return(status_value);
}



//************************************
//************************************
//********** SPI CLOSE PORT **********
//************************************
//************************************
int SpiClosePort (int spi_device)
{
    int status_value = -1;
    int *spi_cs_fd;

    if (spi_device)
        spi_cs_fd = &spi_cs1_fd;
    else
        spi_cs_fd = &spi_cs0_fd;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
        perror("Error - Could not close SPI device");
        exit(1);
    }
    return(status_value);
}



//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int SpiWrite (int spi_device, unsigned char *data, int length)
{
    struct spi_ioc_transfer spi[length];
    int i = 0;
    int retVal = -1;
    int *spi_cs_fd;

    if (spi_device)
        spi_cs_fd = &spi_cs1_fd;
    else
        spi_cs_fd = &spi_cs0_fd;

    //one spi transfer for each byte

    for (i = 0 ; i < length ; i++)
    {
        memset(&spi[i], 0, sizeof (spi[i]));
        spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
        spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
        spi[i].len           = sizeof(*(data + i)) ;
        spi[i].delay_usecs   = 5 ;
        spi[i].speed_hz      = spi_speed ;
        spi[i].bits_per_word = spi_bitsPerWord ;
        spi[i].cs_change = 0;
    }

    retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;
    //retVal = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi);
    if(retVal < 0)
    {
        perror("Error - Problem transmitting spi data..ioctl");
        exit(1);
    }

    return retVal;
}

void SPIReadStatus()
{
    int retVal = -1;
    u_int8_t tx[] = {MCP_READ_STATUS};
    //u_int8_t rx[] = {0x00, 0x00};
    SpiWrite(0, tx, sizeof(tx));


    //
    for(retVal =0; retVal < sizeof(tx); ++retVal)
    {
        //if(!(retVal % 6))
            //puts("");
        printf("%.2X ", *(tx+retVal));
    }
    //
    //return retVal;
}

void SPIReadTest()
{
    int retVal = -1;
    u_int8_t tx[] = {MCP_READ, 0x31, 0x00};
    SpiWrite(0, tx, sizeof(tx));

    for(retVal =0; retVal < sizeof(tx); ++retVal)
    {
        printf("%.2X ", *(tx+retVal));
    }

    u_int8_t rx[] = {MCP_READ, 0x3C, 0x00};
    SpiWrite(0, rx, sizeof(rx));

    for(retVal =0; retVal < sizeof(rx); ++retVal)
    {
        printf("%.2X ", *(rx+retVal));
    }
}

void SPIWriteBTest()
{
    int retVal = -1;
    u_int8_t rx[] = {MCP_LOAD_TX0, 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C};
    SpiWrite(0, rx, sizeof(rx));

}

/*
unsigned char ReadSPI( void )
{
    SSPBUF = 0x00;                  //   Initiate a bus cycle by writing
    while ( ! BF );             	// to SSPBUF.  Wait until a received
    return ( SSPBUF );              // byte is ready, and return it.
}

unsigned char WriteSPI( unsigned char data_out )
{
    SSPBUF = data_out;              //   Write the data byte to SSPBUF
    if ( WCOL )                     // register.  If a write collision
        return ( -1 );              // occurred, return an error condition.
    else                            // Otherwise, wait until the SSPBUF
    {                               // is empty and return success.
        while( ! BF );
    }
    return ( 0 );
}

unsigned char MCP_Read_Byte( unsigned char Address)
{
    unsigned char temp_data;
    SELECT_MCP;
    WriteSPI( MCP_READ );
    WriteSPI( Address );
    temp_data=ReadSPI();
    UNSELECT_MCP;
    return(temp_data);
}

void MCP_Write_Cmd( unsigned char Cmd )
{
    SELECT_MCP;
    WriteSPI( Cmd );
    UNSELECT_MCP;
}
*/

void MCP_Write_Byte( unsigned char address, unsigned char data )
{
    u_int8_t writeData[] =
    {
        MCP_WRITE, address, data,
    };

    SpiWrite(0, writeData, sizeof(writeData));
}

void init_MCP2515()
{
    //initial SPI
    SpiClosePort(0);
    SpiOpenPort(0);

    u_int8_t resetCommand[] = {MCP_RESET};

    //initial MCP2515
    SpiWrite(0, resetCommand, sizeof(resetCommand));	//Reset MCP2515
//    DelayUs(2000);									//wait for MCP2515 start up
    MCP_Write_Byte(MCP_CNF1,def_CNF1);				//setup CNF1~3
    MCP_Write_Byte(MCP_CNF2,def_CNF2);
    MCP_Write_Byte(MCP_CNF3,def_CNF3);
    MCP_Write_Byte(MCP_RXM0SIDH,def_RXM0SIDH);		//setup RXM0
    MCP_Write_Byte(MCP_RXM0SIDL,def_RXM0SIDL);
    MCP_Write_Byte(MCP_RXM0EID8,def_RXM0EID8);
    MCP_Write_Byte(MCP_RXM0EID0,def_RXM0EID0);
    MCP_Write_Byte(MCP_RXF0SIDH,def_RXF0SIDH);		//setup RXF0
    MCP_Write_Byte(MCP_RXF0SIDL,def_RXF0SIDL);
    MCP_Write_Byte(MCP_RXF0EID8,def_RXF0EID8);
    MCP_Write_Byte(MCP_RXF0EID0,def_RXF0EID0);

    MCP_Write_Byte(MCP_RXM1SIDH,def_RXM1SIDH);		//setup RXM1
    MCP_Write_Byte(MCP_RXM1SIDL,def_RXM1SIDL);
    MCP_Write_Byte(MCP_RXM1EID8,def_RXM1EID8);
    MCP_Write_Byte(MCP_RXM1EID0,def_RXM1EID0);
    MCP_Write_Byte(MCP_RXF2SIDH,def_RXF2SIDH);		//setup RXF2
    MCP_Write_Byte(MCP_RXF2SIDL,def_RXF2SIDL);
    MCP_Write_Byte(MCP_RXF2EID8,def_RXF2EID8);
    MCP_Write_Byte(MCP_RXF2EID0,def_RXF2EID0);

    MCP_Write_Byte(MCP_CANCTRL,def_CANCTRL);		//change to normal mode

    MCP_Write_Byte(MCP_TXB0CTRL+1,def_TXB0SIDH);		//setup TXB0
    MCP_Write_Byte(MCP_TXB0CTRL+2,def_TXB0SIDL);
    MCP_Write_Byte(MCP_TXB0CTRL+3,def_TXB0EID8);
    MCP_Write_Byte(MCP_TXB0CTRL+4,def_TXB0EID0);
    MCP_Write_Byte(MCP_TXB0CTRL+5,4);					//DLC
    MCP_Write_Byte(MCP_TXB0CTRL+6,0);
    MCP_Write_Byte(MCP_TXB0CTRL+7,0);
    MCP_Write_Byte(MCP_TXB0CTRL+8,0);
    MCP_Write_Byte(MCP_TXB0CTRL+9,0);

    MCP_Write_Byte(MCP_TXB1CTRL+1,def_TXB1SIDH);		//setup TXB1
    MCP_Write_Byte(MCP_TXB1CTRL+2,def_TXB1SIDL);
    MCP_Write_Byte(MCP_TXB1CTRL+3,def_TXB1EID8);
    MCP_Write_Byte(MCP_TXB1CTRL+4,def_TXB1EID0);
    MCP_Write_Byte(MCP_TXB1CTRL+5,4);					//DLC
    MCP_Write_Byte(MCP_TXB1CTRL+6,0);
    MCP_Write_Byte(MCP_TXB1CTRL+7,0);
    MCP_Write_Byte(MCP_TXB1CTRL+8,0);
    MCP_Write_Byte(MCP_TXB1CTRL+9,0);
}


int main(int argc, char *argv[])
{
    //QApplication::setSetuidAllowed(true);
    QApplication app(argc, argv);
    QTranslator translator;

    translator.load(":/erc_demo_cn_zh.qm");
    app.installTranslator(&translator);

    MainWindow mainPage;
    SettingDialog settingDialog;
    StatusDialog statusDialog;
    LogDialog logDialog;
    InformationDialog informationDialog;
    KeyBoard keyBoard;

    /*
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/buttonphotos/Meanwell_2.png"));
    splash->show();

    Qt::Alignment bottomCenter = Qt::AlignBottom | Qt::AlignCenter;
    splash->showMessage(QObject::tr("Starting the ERC program..."),bottomCenter, Qt::white);
    //QThread::msleep(2000);
    //splash->hide();
    */

    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    //mainPage.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //settingDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //statusDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //logDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //informationDialog.setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    //mainPage.showFullScreen();

    QObject::connect(&mainPage, SIGNAL(showSettingDialog()),&settingDialog,SLOT(receiveShow()));
    QObject::connect(&settingDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showStatusDialog()),&statusDialog,SLOT(receiveShow()));
    QObject::connect(&statusDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showLogDialog()),&logDialog,SLOT(receiveShow()));
    QObject::connect(&logDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showInformationDialog()),&informationDialog,SLOT(receiveShow()));
    QObject::connect(&informationDialog, SIGNAL(showMainPage()),&mainPage,SLOT(receiveShow()));
    QObject::connect(&mainPage, SIGNAL(showKeyBoard()),&keyBoard,SLOT(receiveShow()));
    QObject::connect(&keyBoard, SIGNAL(sendInputValue(QString)),&mainPage,SLOT(receiveInputValue(QString)));

    SpiOpenPort(0);
    //u_int8_t tx[] = {0xC0};
    //SpiWriteAndRead(0, "0xC0", 1);
    //u_int8_t tx[] = {0xA0,0x00,0x00};
    //for(int temp=0; temp<10000; ++temp)
        //SpiWrite(0, tx, sizeof(tx));
    //init_MCP2515();
    //SpiWrite(0, tx, sizeof(tx));

    //u_int8_t rx[] = {0x00};
    //SPIReadStatus();

    //SPIReadTest();
    SPIWriteBTest();
    SPIReadTest();

    SpiClosePort(0);

    mainPage.show();

    return app.exec();
}
