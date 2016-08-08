//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// CMU hardware driver, for LCD, RTC, EEPROM, ADC
// data format convertion
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//#include "PeripheralHeaderIncludes.h"
#include <QtGlobal>
#include "DSP280x_ECan.h"
#include "CMU-GlobalVar.h"
//#include "CMU-comm.h"

volatile qint16 faultUnit=0;
void readData_Poll(void);
void setData_Poll (quint16 setDataAddr,quint16 txDataM,quint16 txDataH,quint16 txDataL );
int checkLink (quint16 ii);

//============================================================================
//============================================================================
//
// FILE:	CMU-DevInit_F280x.c
//
// TITLE:	Device initialization for F280x series
//
// Version:
//============================================================================
//============================================================================

//#include "PeripheralHeaderIncludes.h"

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.
#pragma CODE_SECTION(InitFlash, "ramfuncs");
volatile unsigned int IFR;
volatile unsigned int IER;
/*
void DeviceInit(void);
void PieCntlInit(void);
void PieVectTableInit(void);
void WDogDisable(void);
void WDogEnable(void);
void PLLset(quint16);
void ISR_ILLEGAL(void);
void PeripheralInit(void);
void InitECana(void);
//--------------------------------------------------------------------
//  Configure Device for target Application Here
//--------------------------------------------------------------------
void DeviceInit(void)
{
//	WDogDisable(); 	// Disable the watchdog initially
    WDogEnable();
    DINT;			// Global Disable all Interrupts
    IER = 0x0000;	// Disable CPU interrupts
    IFR = 0x0000;	// Clear all CPU interrupt flags

// SYSTEM CLOCK speed based on Crystal = 20 MHz
// 0xA =  100	MHz		(10)
// 0x9 =  90	MHz		(9)
// 0x8 =  80	MHz		(8)
// 0x7 =  70	MHz		(7)
// 0x6 =  60	MHz		(6)
// 0x5 =  50	MHz		(5)
// 0x4 =  40	MHz		(4)
// 0x3 =  30	MHz		(3)
// 0x2 =  20	MHz		(2)

    PLLset(0xA);	// choose from options above

// Initialise interrupt controller and Vector Table
// to defaults for now. Application ISR mapping done later.
    PieCntlInit();
    PieVectTableInit();

   EALLOW; // below registers are "protected", allow access.

// HIGH / LOW SPEED CLOCKS prescale register settings
   SysCtrlRegs.HISPCP.all = 0x0002;		// Sysclk / 4 (25 MHz)
   SysCtrlRegs.LOSPCP.all = 0x0002;		// Sysclk / 4 (25 MHz)
//   SysCtrlRegs.XCLK.bit.XCLKOUTDIV=2;	// Sysclk (100 MHz)
   SysCtrlRegs.XCLK.bit.XCLKOUTDIV=3;	// turn off XCLKOUT

// PERIPHERAL CLOCK ENABLES
//---------------------------------------------------
// If you are not using a peripheral you may want to switch
// the clock off to save power, i.e. set to =0
//
// Note: not all peripherals are available on all 280x derivates.
// Refer to the datasheet for your particular device.

   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // ADC
   //------------------------------------------------
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   // I2C
   //------------------------------------------------
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK=0;     // SPI-A
   SysCtrlRegs.PCLKCR0.bit.SPIBENCLK=0;     // SPI-B
   SysCtrlRegs.PCLKCR0.bit.SPICENCLK=0;     // SPI-C
   SysCtrlRegs.PCLKCR0.bit.SPIDENCLK=0;     // SPI-D
   //------------------------------------------------
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK=1;     // SCI-A
   SysCtrlRegs.PCLKCR0.bit.SCIBENCLK=0;     // SCI-B
   //------------------------------------------------
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;    // eCAN-A
   SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=0;    // eCAN-B
   //------------------------------------------------
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;  // eCAP1
   SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;  // eCAP2
   SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
   SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
   //------------------------------------------------
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;   // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;   // ePWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 0;   // ePWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;   // ePWM4
   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;   // ePWM5
   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;   // ePWM6
   //------------------------------------------------
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;   // eQEP1
   SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;   // eQEP2
   //------------------------------------------------
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Enable TBCLK
   //------------------------------------------------


// GPIO (GENERAL PURPOSE I/O) CONFIG
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// QUICK NOTES on USAGE:
//-----------------------
// If GpioCtrlRegs.GP?MUX?bit.GPIO?= 1, 2 or 3 (i.e. Non GPIO func), then leave
//	rest of lines commented
// If GpioCtrlRegs.GP?MUX?bit.GPIO?= 0 (i.e. GPIO func), then:
//	1) uncomment GpioCtrlRegs.GP?DIR.bit.GPIO? = ? and choose pin to be IN or OUT
//	2) If IN, can leave next to lines commented
//	3) If OUT, uncomment line with ..GPACLEAR.. to force pin LOW or
//			   uncomment line with ..GPASET.. to force pin HIGH or
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//  GPIO-00 - PIN FUNCTION = LCD_BL_DIM
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;		// 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
//	GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO0 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-01 - PIN FUNCTION = LCD_CST_DIM
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;		// 0=GPIO,  1=EPWM1B,  2=SPISIMO-D,  3=Resv
//	GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO1 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-02 - PIN FUNCTION = BUZZER
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;		// 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
//	GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO2 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-03 - PIN FUNCTION = DIN1
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;		// 0=GPIO,  1=EPWM2B,  2=SPISOMI-D,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO3 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-04 - PIN FUNCTION = DIN2
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;		// 0=GPIO,  1=EPWM3A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO4 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-05 - PIN FUNCTION = DIN4
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;		// 0=GPIO,  1=EPWM3B,  2=SPICLK-D,  3=ECAP1
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO5 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-06 - PIN FUNCTION = I2C_SDA
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;		// 0=GPIO,  1=EPWM4A,  2=SYNCI,  3=SYNCO
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPASET.bit.GPIO6 = 1;		// uncomment if --> Set High initially
    GpioCtrlRegs.GPAPUD.bit.GPIO6=1;		//disable pull-up
//--------------------------------------------------------------------------------------
//  GPIO-07 - PIN FUNCTION = I2C_SCL
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;		// 0=GPIO,  1=EPWM4B,  2=SPISTE-D,  3=ECAP2
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPASET.bit.GPIO7 = 1;		// uncomment if --> Set High initially
    GpioCtrlRegs.GPAPUD.bit.GPIO7=1;		//disable pull-up
//--------------------------------------------------------------------------------------
//  GPIO-08 - PIN FUNCTION = RELAY1
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;		// 0=GPIO,  1=EPWM5A,  2=CANTX-B,  3=ADCSOC-A
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO8 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-09 - PIN FUNCTION = RELAY2
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;		// 0=GPIO,  1=EPWM5B,  2=SCITX-B,  3=ECAP3
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO9 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-10 - PIN FUNCTION = RELAY3
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;	// 0=GPIO,  1=EPWM6A,  2=CANRX-B,  3=ADCSOC-B
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO10 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-11 - PIN FUNCTION = RELAY4
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;	// 0=GPIO,  1=EPWM6B,  2=SCIRX-B,  3=ECAP4
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO11 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-12 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;	// 0=GPIO,  1=TZ1,  2=CANTX-B,  3=SPISIMO-B
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO12 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-13 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;	// 0=GPIO,  1=TZ2,  2=CANRX-B,  3=SPISOMI-B
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO13 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-14 - PIN FUNCTION = TX_ETHERNET
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;	// 0=GPIO,  1=TZ3,  2=SCITX-B,  3=SPICLK-B
//	GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO14 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-15 - PIN FUNCTION = RX_ETHERNET
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;	// 0=GPIO,  1=TZ4,  2=SCIRX-B,  3=SPISTE-B
//	GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO15 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//  GPIO-16 - PIN FUNCTION = LCD_4094_DATA
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;	// 0=GPIO,  1=SPISIMO-A,  2=CANTX-B,  3=TZ5
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO16 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-17 - PIN FUNCTION = DIN3
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;	// 0=GPIO,  1=SPISOMI-A,  2=CANRX-B,  3=TZ6
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO17 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-18 - PIN FUNCTION = LCD_4094_CLK
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;	// 0=GPIO,  1=SPICLK-A,  2=SCITX-B,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO18 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-19 - PIN FUNCTION = LCD_4094_STB
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;	// 0=GPIO,  1=SPISTE-A,  2=SCIRX-B,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;		// 1=OUTput,  0=INput
    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO19 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-20 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;	// 0=GPIO,  1=EQEPA-1,  2=SPISIMO-C,  3=CANTX-B
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;	// uncomment if --> Set Low initially
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;    // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;  // Sync to SYSCLKOUT GPIO25 (EQEP2B)
//	GpioDataRegs.GPASET.bit.GPIO20 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-21 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;	// 0=GPIO,  1=EQEPB-1,  2=SPISOMI-C,  3=CANRX-B
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;	// uncomment if --> Set Low initially
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;    // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;  // Sync to SYSCLKOUT GPIO25 (EQEP2B)
//	GpioDataRegs.GPASET.bit.GPIO21 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-22 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;	// 0=GPIO,  1=EQEPS-1,  2=SPICLK-C,  3=SCITX-B
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO22 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-23 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;	// 0=GPIO,  1=EQEPI-1,  2=SPISTE-C,  3=SCIRX-B
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO23 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-24 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;	// 0=GPIO,  1=ECAP1,  2=EQEPA-2,  3=SPISIMO-B
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;	// uncomment if --> Set Low initially
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;    // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;  // Sync to SYSCLKOUT GPIO25 (EQEP2B)
//	GpioDataRegs.GPASET.bit.GPIO24 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-25 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;	// 0=GPIO,  1=ECAP2,  2=EQEPB-2,  3=SPISOMI-B
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;	// uncomment if --> Set Low initially
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;    // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;  // Sync to SYSCLKOUT GPIO25 (EQEP2B)
//	GpioDataRegs.GPASET.bit.GPIO25 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-26 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;	// 0=GPIO,  1=ECAP3,  2=EQEPI-2,  3=SPICLK-B
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO26 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-27 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;	// 0=GPIO,  1=ECAP4,  2=EQEPS-2,  3=SPISTE-B
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO27 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-28 - PIN FUNCTION = USB
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;	// 0=GPIO,  1=SCIRX-A,  2=Resv,  3=TZ5
//	GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO28 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-29 - PIN FUNCTION = USB
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;	// 0=GPIO,  1=SCITXD-A,  2=Resv,  3=TZ6
//	GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO29 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-30 - PIN FUNCTION = CAN
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// 0=GPIO,  1=CANRX-A,  2=Resv,  3=Resv
//	GpioCtrlRegs.GPADIR.bit.GPIO30 = 0;		// 1=OUTput,  0=INput
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
//	GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO30 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-31 - PIN FUNCTION = CAN_CONTROL
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// 0=GPIO,  1=CANTX-A,  2=Resv,  3=Resv
//	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPASET.bit.GPIO31 = 1;		// uncomment if --> Set High initially
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//  GPIO-32 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;	// 0=GPIO,  1=I2C-SDA,  2=SYNCI,  3=ADCSOCA
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPBSET.bit.GPIO32 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-33 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;	// 0=GPIO,  1=I2C-SCL,  2=SYNCO,  3=ADCSOCB
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPBSET.bit.GPIO33 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------
//  GPIO-34 - PIN FUNCTION = LED3 on controlCARD
//	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;	// 0=GPIO,  1=Resv,  2=Resv,  3=Resv
//	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 0;		// 1=OUTput,  0=INput
//	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;	// uncomment if --> Set Low initially
//	GpioDataRegs.GPBSET.bit.GPIO34 = 1;		// uncomment if --> Set High initially
//--------------------------------------------------------------------------------------

    EDIS;	// Disable register access

}



//============================================================================
// NOTE:
// IN MOST APPLICATIONS THE FUNCTIONS AFTER THIS POINT CAN BE LEFT UNCHANGED
// THE USER NEED NOT REALLY UNDERSTAND THE BELOW CODE TO SUCCESSFULLY RUN THIS
// APPLICATION.
//============================================================================

void WDogDisable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}
void WDogEnable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x002D;
    EDIS;
}
// This function initializes the PLLCR register.
//void InitPll(quint16 val, quint16 clkindiv)
void PLLset(quint16 val)
{
   volatile quint16 iVol;

   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
      // Missing external clock has been detected
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function.
      asm("        ESTOP0");
   }

   // CLKINDIV MUST be 0 before PLLCR can be changed from
   // 0x0000. It is set to 0 by an external reset XRSn
   if (SysCtrlRegs.PLLSTS.bit.CLKINDIV != 0)
   {
       SysCtrlRegs.PLLSTS.bit.CLKINDIV = 0;
   }

   // Change the PLLCR
   if (SysCtrlRegs.PLLCR.bit.DIV != val)
   {

      EALLOW;
      // Before setting PLLCR turn off missing clock detect logic
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
      SysCtrlRegs.PLLCR.bit.DIV = val;
      EDIS;

      // Optional: Wait for PLL to lock.
      // During this time the CPU will switch to OSCCLK/2 until
      // the PLL is stable.  Once the PLL is stable the CPU will
      // switch to the new PLL value.
      //
      // This time-to-lock is monitored by a PLL lock counter.
      //
      // Code is not required to sit and wait for the PLL to lock.
      // However, if the code does anything that is timing critical,
      // and requires the correct clock be locked, then it is best to
      // wait until this switching has completed.

      // Wait for the PLL lock bit to be set.
      // The watchdog should be disabled before this loop, or fed within
      // the loop via ServiceDog().

      // Uncomment to disable the watchdog
      WDogDisable();
      //WDogEnable();
      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
//      SysCtrlRegs.PLLSTS.bit.CLKINDIV != clkindiv;
      SysCtrlRegs.PLLSTS.bit.CLKINDIV = 0;

      EDIS;
    }
}


// This function initializes the PIE control registers to a known state.
//
void PieCntlInit(void)
{
    // Disable Interrupts at the CPU level:
    DINT;

    // Disable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    // Clear all PIEIER registers:
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    // Clear all PIEIFR registers:
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;
}


void PieVectTableInit(void)
{
    qint16	i;
    quint32 *Source = (void *) &ISR_ILLEGAL;
    quint32 *Dest = (void *) &PieVectTable;

    EALLOW;
    for(i=0; i < 128; i++)
        *Dest++ = *Source;
    EDIS;

    // Enable the PIE Vector Table
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

interrupt void ISR_ILLEGAL(void)   // Illegal operation TRAP
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm("          ESTOP0");
  for(;;);

}

// This function initializes the Flash Control registers

//                   CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results

void InitFlash(void)
{
   EALLOW;
   //Enable Flash Pipeline mode to improve performance
   //of code executed from Flash.
   FlashRegs.FOPT.bit.ENPIPE = 1;

   //                CAUTION
   //Minimum waitstates required for the flash operating
   //at a given CPU rate must be characterized by TI.
   //Refer to the datasheet for the latest information.

   //Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;

   //Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;

   //Set the Waitstate for the OTP
   FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;

   //                CAUTION
   //ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
   FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
   FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
   EDIS;

   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.

   asm(" RPT #7 || NOP");
}


// This function will copy the specified memory contents from
// one location to another.
//
//	quint16 *SourceAddr        Pointer to the first word to be moved
//                          SourceAddr < SourceEndAddr
//	quint16* SourceEndAddr     Pointer to the last word to be moved
//	quint16* DestAddr          Pointer to the first destination word
//
// No checks are made for invalid memory locations or that the
// end address is > then the first start address.

void MemCopy(quint16 *SourceAddr, quint16* SourceEndAddr, quint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    {
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


void PeripheralInit(void)
{
//--------------------------------------------------------------------------------------
// ADC CONFIG
//--------------------------------------------------------------------------------------
    //Power up ADC
    AdcRegs.ADCTRL3.all = 0x00E0;  			// Power up bandgap/reference/ADC circuits, 5mSec delay
    // Configure ADC
    AdcRegs.ADCMAXCONV.all = 0x0010;       	// Setup 1 conv's on SEQ1 & SEQ2
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x7; 	// Setup ADCINA7 as 1st SEQ1 conv.
//	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x0; 	// Setup ADCINA0 as 1st SEQ1 conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0xf; 	// Setup ADCINB7 as 1st SEQ2 conv.
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0xe; 	// Setup ADCINB6 as 1st SEQ2 conv.
//--------------------------------------------------------------------------------------
// EPWM CONFIG
//--------------------------------------------------------------------------------------
    //EPWM1 setting
    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = 0x00;		// Count up
    EPwm1Regs.TBPRD = 20000;				// Set timer period
    EPwm1Regs.TBCTL.bit.PHSEN = 0x0;		// Disable phase loading
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;	// Phase is 0
    EPwm1Regs.TBCTR = 0x0000;				// Clear counter
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x1;	// Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x1;
    // Setup shadow register load on ZERO
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0x0;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = 0x0;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0x0;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = 0x0;
    // Set actions
    EPwm1Regs.AQCTLA.bit.ZRO = 0x2;			// Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAU = 0x1;			// Clear PWM1A on event A, up count
    EPwm1Regs.AQCTLB.bit.ZRO = 0x2;			// Set PWM1B on Zero
    EPwm1Regs.AQCTLB.bit.CBU = 0x1;			// Clear PWM1B on event B, up count
    // Set Compare values
    EPwm1Regs.CMPA.half.CMPA = 0;			// Set compare A value, PWM=0
    EPwm1Regs.CMPB = 65535;					// Set Compare B value, PWM=1
    //EPWM2 setting
    // Setup TBCLK
    EPwm2Regs.TBCTL.bit.CTRMODE = 0x00;		// Count up
    EPwm2Regs.TBPRD = 6000;					// Set timer period
    EPwm2Regs.TBCTL.bit.PHSEN = 0x0;		// Disable phase loading
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;	// Phase is 0
    EPwm2Regs.TBCTR = 0x0000;				// Clear counter
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x1;	// Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = 0x1;
    // Setup shadow register load on ZERO
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = 0x0;
//	EPwm2Regs.CMPCTL.bit.SHDWBMODE = 0x0;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = 0x01;
//	EPwm2Regs.CMPCTL.bit.LOADBMODE = 0x0;

    // Set actions
    EPwm2Regs.AQCTLA.bit.ZRO = 0x2;			// Set PWM1A on Zero
    EPwm2Regs.AQCTLA.bit.CAU = 0x1;			// Clear PWM1A on event A, up count
//	EPwm2Regs.AQCTLB.bit.ZRO = 0x2;			// Set PWM1B on Zero
//	EPwm2Regs.AQCTLB.bit.CBU = 0x1;			// Clear PWM1B on event B, up count
    // Set Compare values
    EPwm2Regs.CMPA.half.CMPA = 000;			// Set compare A value
//	EPwm2Regs.CMPB = 0;						// Set Compare B value

//--------------------------------------------------------------------------------------
// EQEP CONFIG
//--------------------------------------------------------------------------------------

//  EQep2Regs.QUPRD=1000000;			// Unit Timer for 100Hz
//	EQep2Regs.QDECCTL.bit.QSRC=11;		// QEP clock Inputs
//	EQep2Regs.QDECCTL.bit.XCR=0;
//	EQep2Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep2Regs.QEPCTL.bit.PCRM=01;		// PCRM=00 mode

//	EQep2Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
//	EQep2Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
    EQep2Regs.QEINT.bit.PCO=1;
    EQep2Regs.QEINT.bit.PCU=1;

    EQep2Regs.QPOSMAX=3999;
    EQep2Regs.QEPCTL.bit.QPEN = 1; 		// QEP enable

//	EQep2Regs.QCAPCTL.bit.UPPS=2;   	// 1/4 for unit position
//	EQep2Regs.QCAPCTL.bit.CCPS=0;		// 000	//1/128 for CAP clock
//	EQep2Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable


//  EQep1Regs.QUPRD=1000000;			// Unit Timer for 100Hz
//	EQep1Regs.QDECCTL.bit.QSRC=11;		// QEP clock Inputs
//	EQep1Regs.QDECCTL.bit.XCR=0;
//	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep1Regs.QEPCTL.bit.PCRM=01;		// PCRM=00 mode

//	EQep1Regs.QEPCTL.bit.UTE=1; 		// Unit Timeout Enable
//	EQep1Regs.QEPCTL.bit.QCLM=1; 		// Latch on unit time out
    EQep1Regs.QEINT.bit.PCO=1;
    EQep1Regs.QEINT.bit.PCU=1;

    EQep1Regs.QPOSMAX=3999;
    EQep1Regs.QEPCTL.bit.QPEN = 1; 		// QEP enable

//	EQep1Regs.QCAPCTL.bit.UPPS=2;   	// 1/4 for unit position
//	EQep1Regs.QCAPCTL.bit.CCPS=0;		// 000	//1/128 for CAP clock
//	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable



//--------------------------------------------------------------------------------------
// SCI CONFIG
//--------------------------------------------------------------------------------------
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

//SCI-A
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;

    SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;
//	SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 25MHz.
//	SciaRegs.SCILBAUD    =0x0044;
//	SciaRegs.SCIHBAUD    =0x0000;  // 19200 baud @LSPCLK = 25MHz.
//	SciaRegs.SCILBAUD    =0x00A2;
    SciaRegs.SCIHBAUD    =0x0000;  // 115200 baud @LSPCLK = 25MHz.
    SciaRegs.SCILBAUD    =0x001B;
    SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset

//SCI-B
    ScibRegs.SCIFFTX.all=0xE040;
    ScibRegs.SCIFFRX.all=0x204f;
    ScibRegs.SCIFFCT.all=0x0;

    ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all =0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA =1;
    ScibRegs.SCICTL2.bit.RXBKINTENA =1;
//	ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 25MHz.
//	ScibRegs.SCILBAUD    =0x0044;
    ScibRegs.SCIHBAUD    =0x0000;  // 19200 baud @LSPCLK = 25MHz.
    ScibRegs.SCILBAUD    =0x00A2;
//	ScibRegs.SCIHBAUD    =0x0000;  // 115200 baud @LSPCLK = 25MHz.
//	ScibRegs.SCILBAUD    =0x001B;
    ScibRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
//--------------------------------------------------------------------------------------
// Initialize eCAN-A module
//--------------------------------------------------------------------------------------

    InitECana();

//--------------------------------------------------------------------------------------
// I2C CONFIG -- for PMBus
//--------------------------------------------------------------------------------------
   // Initialize I2C
   I2caRegs.I2CPSC.all = 9;			// Prescaler - need 7-12 Mhz on module clk
   I2caRegs.I2CCLKL = 45;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 45;			// NOTE: must be non zero
   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

}
*/
//--------------------------------------------------------------------------------------
// Initialize eCAN-A module
//--------------------------------------------------------------------------------------
/*
void InitECana(void)		// Initialize eCAN-A module
{
// Create a shadow register structure for the CAN control registers. This is
// needed, since, only 32-bit access is allowed to these registers. 16-bit access
// to these registers could potentially corrupt the register contents. This is
// especially true while writing to a bit (or group of bits) among bits 16 - 31

struct ECAN_REGS ECanaShadow;

//	EALLOW;		// EALLOW enables access to protected bits

// Configure eCAN RX and TX pins for eCAN transmissions using eCAN reg
    EALLOW;
    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;
    EDIS;
// Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31)
                                    // HECC mode also enables time-stamping feature

//	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
//	ECanaShadow.CANMC.bit.SCB = 1;
//	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    // Disable all Mailboxes
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANME.all = 0;
// Initialize all bits of 'Master Control Field' to zero
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero
     // Mailboxs can be written to 16-bits or 32-bits at a time
    // Write to the MSGID field of TRANSMIT mailboxes MBOX0 - 15
     ECanaMboxes.MBOX0.MSGID.all = 0x80000300;
     ECanaMboxes.MBOX1.MSGID.all = 0x800003FF;
     ECanaMboxes.MBOX2.MSGID.all = 0x80000302;
     ECanaMboxes.MBOX3.MSGID.all = 0x80000303;
     ECanaMboxes.MBOX4.MSGID.all = 0x80000304;
     ECanaMboxes.MBOX5.MSGID.all = 0x80000305;
     ECanaMboxes.MBOX6.MSGID.all = 0x80000306;
     ECanaMboxes.MBOX7.MSGID.all = 0x80000307;
     ECanaMboxes.MBOX8.MSGID.all = 0x80000308;
     ECanaMboxes.MBOX9.MSGID.all = 0x80000309;
     ECanaMboxes.MBOX10.MSGID.all = 0x8000030A;
     ECanaMboxes.MBOX11.MSGID.all = 0x8000030B;
     ECanaMboxes.MBOX12.MSGID.all = 0x8000030C;
     ECanaMboxes.MBOX13.MSGID.all = 0x8000030D;
     ECanaMboxes.MBOX14.MSGID.all = 0x8000030E;
     ECanaMboxes.MBOX15.MSGID.all = 0x8000030F;

    // Write to the MSGID field of RECEIVE mailboxes MBOX16 - 31
     ECanaMboxes.MBOX16.MSGID.all = 0x80000400;
     ECanaMboxes.MBOX17.MSGID.all = 0x80000301;
     ECanaMboxes.MBOX18.MSGID.all = 0x80000302;
     ECanaMboxes.MBOX19.MSGID.all = 0x80000303;
     ECanaMboxes.MBOX20.MSGID.all = 0x80000304;
     ECanaMboxes.MBOX21.MSGID.all = 0x80000305;
     ECanaMboxes.MBOX22.MSGID.all = 0x80000306;
     ECanaMboxes.MBOX23.MSGID.all = 0x80000307;
     ECanaMboxes.MBOX24.MSGID.all = 0x80000308;
     ECanaMboxes.MBOX25.MSGID.all = 0x80000309;
     ECanaMboxes.MBOX26.MSGID.all = 0x8000030a;
     ECanaMboxes.MBOX27.MSGID.all = 0x8000030b;
     ECanaMboxes.MBOX28.MSGID.all = 0x8000030c;
     ECanaMboxes.MBOX29.MSGID.all = 0x8000030d;
     ECanaMboxes.MBOX30.MSGID.all = 0x8000030e;
     ECanaMboxes.MBOX31.MSGID.all = 0x8000030f;

    // Configure Mailboxes 0-15 as Tx, 16-31 as Rx
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANMD.all = 0xFFFF0000;  // 1:The mailbox is configured as a receive mailbox.
                                       // 0:The mailbox is configured as a transmit mailbox.

    // Enable all Mailboxes
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANME.all = 0xFFFFFFFF;

    // Specify that 8 bits will be sent/received
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX7.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX8.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX9.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX10.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX11.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX12.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX13.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX14.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX15.MSGCTRL.bit.DLC = 8;

    // No remote frame is requested
    // Since RTR bit is undefined upon reset,
    // it must be initialized to the proper value
    ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX1.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX2.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX3.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX4.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX5.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX6.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX7.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX8.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX9.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX10.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX11.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX12.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX13.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX14.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX15.MSGCTRL.bit.RTR = 0;

    // Write to the mailbox RAM field of MBOX0 - 15
    ECanaMboxes.MBOX0.MDL.all = 0xffffffff;
    ECanaMboxes.MBOX0.MDH.all = 0x00000000;
    ECanaMboxes.MBOX1.MDL.all = 0x00000001;
    ECanaMboxes.MBOX1.MDH.all = 0x00000002;
    ECanaMboxes.MBOX2.MDL.all = 0x00000003;
    ECanaMboxes.MBOX2.MDH.all = 0x00000004;
    ECanaMboxes.MBOX3.MDL.all = 0x00000005;
    ECanaMboxes.MBOX3.MDH.all = 0x00000006;
    ECanaMboxes.MBOX4.MDL.all = 0x00000007;
    ECanaMboxes.MBOX4.MDH.all = 0x00000008;
    ECanaMboxes.MBOX5.MDL.all = 0x00000009;
    ECanaMboxes.MBOX5.MDH.all = 0x0000000a;
    ECanaMboxes.MBOX6.MDL.all = 0x0000000b;
    ECanaMboxes.MBOX6.MDH.all = 0x0000000c;
    ECanaMboxes.MBOX7.MDL.all = 0x0000000d;
    ECanaMboxes.MBOX7.MDH.all = 0x0000000e;
    ECanaMboxes.MBOX8.MDL.all = 0x0000000f;
    ECanaMboxes.MBOX8.MDH.all = 0x00000010;
    ECanaMboxes.MBOX9.MDL.all = 0x00000011;
    ECanaMboxes.MBOX9.MDH.all = 0x00000012;
    ECanaMboxes.MBOX10.MDL.all = 0x00000013;
    ECanaMboxes.MBOX10.MDH.all = 0x00000014;
    ECanaMboxes.MBOX11.MDL.all = 0x00000015;
    ECanaMboxes.MBOX11.MDH.all = 0x00000016;
    ECanaMboxes.MBOX12.MDL.all = 0x00000017;
    ECanaMboxes.MBOX12.MDH.all = 0x00000018;
    ECanaMboxes.MBOX13.MDL.all = 0x00000019;
    ECanaMboxes.MBOX13.MDH.all = 0x0000001a;
    ECanaMboxes.MBOX14.MDL.all = 0x0000001b;
    ECanaMboxes.MBOX14.MDH.all = 0x0000001c;
    ECanaMboxes.MBOX15.MDL.all = 0x0000001d;
    ECanaMboxes.MBOX15.MDH.all = 0x0000001e;

        EALLOW;
        ECanaRegs.CANMIM.all = 0xFFFFFFFF;

    // Configure bit timing parameters for eCANA
        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
        ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
        EDIS;
    //     do
    //    {
          ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    //    }while(ECanaRegs.CANES.bit.CCE != 1 );   // Wait for CCE bit to be set..

        // Configure the eCAN timing
    // Baud rate =(SYSCLK/(BRP*Bit time))=(100MHz/(20*(14+1+3+1+1)))=250kbps
        EALLOW;
        ECanaShadow.CANBTC.all = ECanaRegs.CANBTC.all;
    //    ECanaShadow.CANBTC.all = 0;
        ECanaShadow.CANBTC.bit.BRPREG = 19;		//
        ECanaShadow.CANBTC.bit.TSEG2REG = 4;	//
        ECanaShadow.CANBTC.bit.TSEG1REG = 13; 	//
        ECanaShadow.CANBTC.bit.SAM = 1;			//1= The CAN module samples three times and make a majority decision. The triple sample mode
                                                //   shall be selected only for bit rate prescale values greater than 4 (BRP > 4).
                                                //0= The CAN module samples only once at the sampling point.
        ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;	//

        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 0;
        ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
        EDIS;
        // Wait until the CPU no longer has permission to change the
        // configuration registers
    //    do
    //    {
          ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    //    } while(ECanaShadow.CANES.bit.CCE != 0 );

    // Configure the eCAN for self test mode
    // Enable the enhanced features of the eCAN.
    EALLOW;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.STM = 0;    // Configure CAN for self-test mode
    ECanaShadow.CANMC.bit.SCB = 1;    // eCAN mode (reqd to access 32 mailboxes)
    ECanaShadow.CANMC.bit.DBO = 0;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.
    EALLOW;
    ECanaRegs.CANTA.all	= 0xFFFFFFFF;	// Clear all TAn bits
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;	// Clear all RMPn bits

    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;	// Clear all interrupt flag bits
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANGIM.all = 0x00000002;	//
    ECanaRegs.CANMIM.all = 0x00010001;	// enable eCAN Mailbox Interrupt Mask register (CANMIM)
    ECanaRegs.CANMIL.all = 0x00010001;	//  0 -> Int 9.5(high level)   1 -> Int 9.6(low level)
    EDIS;

// Disable all Mailboxes
// 	ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs

  //  EDIS;
}
*/




//===========================================================================
// End of file.
//===========================================================================


/*** CANBus Tx Process ***************************/
/*
void canbusPolling()
{
CanBus_Polling:
    if(CAN_Pollcnt[0]<(MAX_CANB_UNIT) )
    {
        if(table_CANBPoll[CAN_Pollcnt[1]]!=4)	// table_CANBPoll[CAN_Pollcnt[1]]= 4 polling all rack
        {
            if(checkLink(CAN_Pollcnt[0])==0)
            {
                CAN_Pollcnt[0]++;
                goto	CanBus_Polling;
            }
        }
    }
    else
    {	CAN_Pollcnt[0]=0;
        if(CANB_STS1==2)			// for setting command
            canTxStep=1;			// 設定參數輪詢完成旗標建立
        else
        {	if(CAN_Pollcnt[1]>5)
            {
                CAN_Pollcnt[1]=0;
                polling_around_flag=1; // polling reset
            }
            else
                CAN_Pollcnt[1]++;	// next polling data
        }
        goto	CanBus_Polling;
    }

    if(CANB_STS1!=2)							// for read command
        Data_Polling= table_CANBPoll[CAN_Pollcnt[1]];

    readData_Poll();
}

void setRackPowerModuleCurrentAtCCMode()
{
    qint16 recycle=0;
    switch (setStep[4])
    {
        case 0:
            setData_Poll(0x04,0x00,0x00,rackCCI_m1)	;
            setStep[4]=1;
            break;
        case 1:
            setData_Poll(0x05,0x00,0x00,rackCCI_m2)	;
            setStep[4]=2;
            break;
        case 2:
            setData_Poll(0x06,0x00,0x00,rackCCI_m3)	;
            setStep[4]=3;
            break;
        case 3:
            setData_Poll(0x07,0x00,0x00,rackCCI_m4)	;
            setStep[4]=4;
            break;
        case 4:
            setData_Poll(0x08,0x00,0x00,rackCCI_m5)	;
            setStep[4]=5;
            break;
        case 5:
            setData_Poll(0x09,0x00,0x00,rackCCI_m6)	;
            setStep[4]=6;
            break;
        case 6:
            setData_Poll(0x0A,0x00,0x00,rackCCI_m7)	;
            setStep[4]=7;
            break;
        case 7:
            setData_Poll(0x0B,0x00,0x00,rackCCI_m8)	;
            setStep[4]=8;
            break;
        case 8:
            setData_Poll(0x0C,0x00,0x00,rackCCI_m9)	;
            setStep[4]=9;
            break;
        case 9:
            setData_Poll(0x0D,0x00,0x00,rackCCI_m10)	;
            setStep[4]=10;
            break;
        case 10:
            setData_Poll(0x0E,0x00,0x00,rackCCI_m11)	;
            setStep[4]=11;
            break;
        case 11:
            setData_Poll(0x0F,0x00,0x00,rackCCI_m12)	;
            setStep[4]=12;
            break;
        case 12:
            setData_Poll(0x10,0x00,0x00,rackCCI_m13)	;
            setStep[4]=13;
            break;
        case 13:
            setData_Poll(0x11,0x00,0x00,rackCCI_m14)	;
            setStep[4]=14;
            break;
        case 14:
            setData_Poll(0x12,0x00,0x00,rackCCI_m15)	;
            setStep[4]=15;
            break;
        case 15:
            setData_Poll(0x13,0x00,0x00,rackCCI_m16)	;
            CAN_Pollcnt[0] = 0	;
            canTxStep=0;
            setStep[4]=16;
            break;
        case 16:	 // 讀回設定值
            Data_Polling = 24	;
            if(canTxStep)
                setStep[4]=17;
            else
            {
                canbusPolling();//goto CanBus_Polling	;
            }
        case 17:	 // 確認是否設定寫入完成
            recycle=0;
            for(qint16 ii=0;ii<MAX_CANB_UNIT;ii++)
            {
                quint8 x1;

                if(checkLink(ii))
                {
                    x1 = CANB_U[ii].a04_setCCM_M1;
                    if(rackCCI_m1!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a05_setCCM_M2;
                    if(rackCCI_m2!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a06_setCCM_M3;
                    if(rackCCI_m3!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a07_setCCM_M4;
                    if(rackCCI_m4!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a08_setCCM_M5;
                    if(rackCCI_m5!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a09_setCCM_M6;
                    if(rackCCI_m6!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0A_setCCM_M7;
                    if(rackCCI_m7!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0B_setCCM_M8;
                    if(rackCCI_m8!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0C_setCCM_M9;
                    if(rackCCI_m9!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0D_setCCM_M10;
                    if(rackCCI_m10!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0E_setCCM_M11;
                    if(rackCCI_m11!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a0F_setCCM_M12;
                    if(rackCCI_m12!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a10_setCCM_M13;
                    if(rackCCI_m13!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a11_setCCM_M14;
                    if(rackCCI_m14!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a12_setCCM_M15;
                    if(rackCCI_m15!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a13_setCCM_M16;
                    if(rackCCI_m16!= x1)
                        recycle++;	// write error
                }
            }

            if(recycle==0)
            {
                setCANB_04 = 0;				// setting done and clear flag
                Alarm_CMU.bit.WriC04Er=0;
            }
            else 							// 當re-cycle超過3次視為寫入異常
            {
                if( RetrySetcnt >=4)
                {
                    setCANB_04 = 0;
                    RetrySetcnt=0;
                    Alarm_CMU.bit.WriC04Er=1;
                }
                else
                    RetrySetcnt++;
            }
            setStep[4] = 0;
        break;
    }
}

void setRackPowerModulePowerAtCPMode()
{
    qint16 recycle=0;

    switch (setStep[5])
    {
        case 0:
            setData_Poll(0x14,0x00,0x00,rackCPP_m1)	;
            setStep[5]=1;
            break;
        case 1:
            setData_Poll(0x15,0x00,0x00,rackCPP_m2)	;
            setStep[5]=2;
            break;
        case 2:
            setData_Poll(0x16,0x00,0x00,rackCPP_m3)	;
            setStep[5]=3;
            break;
        case 3:
            setData_Poll(0x17,0x00,0x00,rackCPP_m4)	;
            setStep[5]=4;
            break;
        case 4:
            setData_Poll(0x18,0x00,0x00,rackCPP_m5)	;
            setStep[5]=5;
            break;
        case 5:
            setData_Poll(0x19,0x00,0x00,rackCPP_m6)	;
            setStep[5]=6;
            break;
        case 6:
            setData_Poll(0x1A,0x00,0x00,rackCPP_m7)	;
            setStep[5]=7;
            break;
        case 7:
            setData_Poll(0x1B,0x00,0x00,rackCPP_m8)	;
            setStep[5]=8;
            break;
        case 8:
            setData_Poll(0x1C,0x00,0x00,rackCPP_m9)	;
            setStep[5]=9;
            break;
        case 9:
            setData_Poll(0x1D,0x00,0x00,rackCPP_m10)	;
            setStep[5]=10;
            break;
        case 10:
            setData_Poll(0x1E,0x00,0x00,rackCPP_m11)	;
            setStep[5]=11;
            break;
        case 11:
            setData_Poll(0x1F,0x00,0x00,rackCPP_m12)	;
            setStep[5]=12;
            break;
        case 12:
            setData_Poll(0x20,0x00,0x00,rackCPP_m13)	;
            setStep[5]=13;
            break;
        case 13:
            setData_Poll(0x21,0x00,0x00,rackCPP_m14)	;
            setStep[5]=14;
            break;
        case 14:
            setData_Poll(0x22,0x00,0x00,rackCPP_m15)	;
            setStep[5]=15;
            break;
        case 15:
            setData_Poll(0x23,0x00,0x00,rackCPP_m16)	;
            CAN_Pollcnt[0] = 0	;
            canTxStep=0;
            setStep[5]=16;
            break;
        case 16:	 // 讀回設定值
            Data_Polling = 28	;
            if(canTxStep)
            {	setStep[5]=17;				}
            else
            {
                canbusPolling();//goto CanBus_Polling	;
            }
        case 17:	 // 確認是否設定寫入完成
            recycle=0;
            for(qint16 ii=0;ii<MAX_CANB_UNIT;ii++)
            {
                quint8 x1;

                if(checkLink(ii))
                {
                    x1 = CANB_U[ii].a14_setCPM_M1;
                    if(rackCPP_m1!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a15_setCPM_M2;
                    if(rackCPP_m2!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a16_setCPM_M3;
                    if(rackCPP_m3!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a17_setCPM_M4;
                    if(rackCPP_m4!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a18_setCPM_M5;
                    if(rackCPP_m5!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a19_setCPM_M6;
                    if(rackCPP_m6!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1A_setCPM_M7;
                    if(rackCPP_m7!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1B_setCPM_M8;
                    if(rackCPP_m8!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1C_setCPM_M9;
                    if(rackCPP_m9!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1D_setCPM_M10;
                    if(rackCPP_m10!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1E_setCPM_M11;
                    if(rackCPP_m11!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a1F_setCPM_M12;
                    if(rackCPP_m12!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a20_setCPM_M13;
                    if(rackCPP_m13!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a21_setCPM_M14;
                    if(rackCPP_m14!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a22_setCPM_M15;
                    if(rackCPP_m15!= x1)
                        recycle++;	// write error
                    x1 = CANB_U[ii].a23_setCPM_M16;
                    if(rackCPP_m16!= x1)
                        recycle++;	// write error
                }
            }
            if(recycle==0)
            {
                setCANB_08 = 0;				// setting done and clear flag
                Alarm_CMU.bit.WriC08Er=0;
            }
            else 							// 當re-cycle超過3次視為寫入異常
            {
                if( RetrySetcnt >=4)
                {
                    setCANB_08 = 0;
                    RetrySetcnt=0;
                    Alarm_CMU.bit.WriC08Er=1;
                }
                else
                    RetrySetcnt++;
            }
            setStep[5] = 0;
            break;
    }
}

void clearFlag()
{
    switch (setStep[1])
    {
        case 0:
            setData_Poll(0x01,0x0000,0x0000,0xFFFF);
            setStep[1]=1;
            break;
        case 1:
            setCANB_01 = 0;
            setStep[1] = 0;
            break;
    }
}

void setRackCCModeCurrent()
{
    qint16 recycle=0;

    switch (setStep[2])
    {
        case 0:
            setData_Poll(0x02,0x0000,0x0000,rackCCI);
    //		if(setCANB_00){setCANB_02 = 0;setStep[2] = 0;break;}
            CAN_Pollcnt[0] = 0	;
            canTxStep=0;
            setStep[2]=1;
        break;
        case 1: // 讀回設定值
            Data_Polling = 22	;

            if(canTxStep)
            {	setStep[2]=2;				}
            else
            {
                canbusPolling();//goto CanBus_Polling	;
            }
        case 2: // 確認是否設定寫入完成
            recycle=0;
            for(qint16 ii=0;ii<MAX_CANB_UNIT;ii++)
            {
                quint8 x1;

                if(checkLink(ii))
                {
                    x1 = CANB_U[ii].a02_setCCM_R;

                    if(rackCCI!= x1)
                        recycle++;	// write error
                }
            }
            if(recycle==0)
            {
                setCANB_02 = 0;				// setting done and clear flag
                Alarm_CMU.bit.WriC02Er=0;
            }
            else 							// 當re-cycle 超過3次視為寫入異常
            {
                if( RetrySetcnt >=4)
                {
                    setCANB_02 = 0;
                    RetrySetcnt=0;
                    Alarm_CMU.bit.WriC02Er=1;
                }
                else
                    RetrySetcnt++;
            }
            setStep[2] = 0;
        break;
    }
}

void setRackCPModePower()
{
    qint16 recycle=0;

    switch (setStep[3])
    {
        case 0:
            setData_Poll(0x03,0x00,0x00,rackCPP)	;
    //		if(setCANB_00){setCANB_03 = 0;setStep[3] = 0;break;}
            CAN_Pollcnt[0] = 0	;
            canTxStep=0;
            setStep[3]=1;
        break;
        case 1:	 // 讀回設定值
            Data_Polling = 23	;
            if(canTxStep)
            {	setStep[3]=2;				}
            else
            {
                canbusPolling();//goto CanBus_Polling	;
            }
        case 2:	 // 確認是否設定寫入完成
            recycle=0;
            for(qint16 ii=0;ii<MAX_CANB_UNIT;ii++)
            {
                quint8 x1;

                if(checkLink(ii))
                {
                    x1 = CANB_U[ii].a03_setCPM_R;

                    if(rackCPP!= x1)
                        recycle++;	// write errorr
                }
            }
            if(recycle==0)
            {
                setCANB_03 = 0;				// setting done and clear flag
                Alarm_CMU.bit.WriC03Er=0;
            }
            else // 當re-cycle超過3次視為寫入異常
            {
                if( RetrySetcnt >=4)
                {
                    setCANB_03 = 0;
                    RetrySetcnt=0;
                    Alarm_CMU.bit.WriC03Er=1;
                }
                else
                    RetrySetcnt++;
            }
            setStep[3] = 0;
        break;
    }
}

void setRackOnOff()
{
    quint32 ak47;
    qint16 recycle=0;

    switch (setStep[0])					//set Parameter data step
    {
        case 0:
            CAN_Pollcnt[0] = 0	;
            canTxStep = 0	;
            setStep[0] = 1	;
        break;
        case 1:	//此項目為各別單機傳送不廣播
            recycle=rackON_OFF[CAN_Pollcnt[0]];
            ak47 = recycle >> 8;
            ECanaMboxes.MBOX0.MDL.all = (0x01000000|ak47);
            ak47 = recycle & 0xff;
            ak47 = ak47 << 24;
            ECanaMboxes.MBOX0.MDH.all = ak47;
            if(load_start_flag !=0) Relay1On;
            if(load_start_flag ==0) Relay1Off;
            Data_Polling = 0xFF	;		//設定無效位置
            readData_Poll();
            if(CAN_Pollcnt[0]>=MAX_CANB_UNIT)
            {
                setStep[0] = 2	;
                CAN_Pollcnt[0]=0;
            }
        break;
        case 2:	//	讀回設定值
            Data_Polling = 20	;
            if(canTxStep)
                setStep[0] = 3	;
            else
            {
                canbusPolling();//goto CanBus_Polling	;
            }
        case 3:	// 確認是否設定寫入完成
            recycle=0;
            for(qint16 ii=0;ii<MAX_CANB_UNIT;ii++)
            {
                if(checkLink(ii))
                {	if(rackON_OFF[ii]!= CANB_U[ii].a00_operation)
                    {	recycle++	;		}// write error
                }
            }
            if(recycle==0)
            {
                on_off_count =0;
                setCANB_00 = 0;			// setting done and clear flag
                Alarm_CMU.bit.WriC00Er=0;
            }
            else 						// 當re-cycle超過3次視為寫入異常
            {	if( RetrySetcnt >=4)
                {
                    on_off_count =0;
                    setCANB_00 = 0;
                    RetrySetcnt=0;
                    Alarm_CMU.bit.WriC00Er=1;
                }
                else
                {	RetrySetcnt++ ;		}
            }
            setStep[0] = 0;
        break;
    }
}

void CANBusTxProcess(void)
{  
	if(SetCANBus.all==0)
	{	CANB_STS1=1;	}	// setting read flag
	else
	{	CANB_STS1=2;	}	// setting write flag

	if(CANB_STS1==2)							//set Parameter
    {
        if(setCANB_01)	// clear flag
            clearFlag();
		else if(setCANB_02) // setting rack CC Mode current 
		{
            setRackCCModeCurrent();
		}
		else if(setCANB_03)  // setting rack CP Mode power 
        {
            setRackCPModePower();
		}
		else if(setCANB_04)	 // setting rack power module CC Mode current 
        {
            setRackPowerModuleCurrentAtCCMode();
		}
		else if(setCANB_08)		// setting rack power module CP Mode power
        {
            setRackPowerModulePowerAtCPMode();
		}
		else if(setCANB_00 ) //RACK On/Off setting
		{
            setRackOnOff();
		}
    }
	else if(CANB_STS1==1)	//read data (polling connect rack)
	{
//CanBus_Polling:
        canbusPolling();
                                // call read data polling
	}
}
//
// setting command data
void setData_Poll (quint16 setDataAddr,quint16 txDataM,quint16 txDataH,quint16 txDataL )
{
    quint32 aa,bb,cc,dd;
	aa = (setDataAddr*256)|(0x01000000);
	bb = txDataL >>8;
    ECanaMboxes.MBOX1.MDL.all = aa | bb;
	bb = txDataL & 0xFF;
	bb = bb << 24;
	cc = txDataM << 8;
	dd = txDataH & 0xFF;
	ECanaMboxes.MBOX1.MDH.all = bb + cc + dd;
	ECanaRegs.CANTRS.all = 0x00000002;  			// setting mailbox 1 transmit flag
	canTxStart=1;
	canBusTxToutCnt=0;
}
// read command data polling
void readData_Poll(void)
{
    ECanaRegs.CANME.all = 0x00000000;				// disable all mailbox
	ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_L=0x300+CAN_Pollcnt[0];	// setting rack mailbox ID
	ECanaRegs.CANME.all = 0xFFFFFFFF;				// enable all mailbox
	CAN_Pollcnt[0]++;								// setting next rack polling address 
//CAN bus polling data  
	switch	(Data_Polling)
	{	case 1:										// 1: read address 0x30 ~ 0x3B (Statu ,PWM ,Fault)
			ECanaMboxes.MBOX0.MDL.all = 0x02060200;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;
		break;
		case 2:										// 2: read address 0x3C ~ 0x47 (Vin ,Iin ,Power)
			ECanaMboxes.MBOX0.MDL.all = 0x02090206;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;			
		break;
		case 3:										// 3: read address 0x48 ~ 0x53 (Temperature.1 ,Temperature.2 ,Temperature.3)
			ECanaMboxes.MBOX0.MDL.all = 0x02040220;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;			
		break;
		case 4:										// 4: read address 0x58 ~ 0x5B (Revision)
			ECanaMboxes.MBOX0.MDL.all = 0x02030224;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;
		break;	
		case 20:									// 20: read address 0x00 (Operation)
			ECanaMboxes.MBOX0.MDL.all = 0x02010000;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;	
		break;		
		case 22:									// 22: read address 0x02 (rack I_CC)
			ECanaMboxes.MBOX0.MDL.all = 0x02010002;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;	
		break;	
		case 23:									// 23: read address 0x03 (rack P_CP)
			ECanaMboxes.MBOX0.MDL.all = 0x02010003;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;	
		break;
		case 24:									// 24: read address 0x04 ~ 0x07 (power module I_CC)		
			ECanaMboxes.MBOX0.MDL.all = 0x02100004;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;	
		break;
		case 28:									// 28: read address 0x08 ~ 0x0B (power module P_CP)
			ECanaMboxes.MBOX0.MDL.all = 0x02100014;
    		ECanaMboxes.MBOX0.MDH.all = 0x00000000;	
		break;
		default:
		break;
	}
	ECanaRegs.CANTRS.all = 0x00000001;  			// setting mailbox 0 transmit flag
	canTxStart=1;
	canBusTxToutCnt=0;
}
*/
/*** CANBus Rx Process ***************************/

void CANBusRxProcess(void)
{
    quint32 rackAddr,packetCnt,tolPackCnt,startAddr,endAddr ;
    quint16 *address,totalCount,mask,packetcount;//,commandID;
	rackAddr = (canBus_RxBuf[0]&0xFF000000)>>24 ;	
//	commandID = (canBus_RxBuf[0]&0x00FF0000)>>16 ;
	packetCnt = (canBus_RxBuf[0]&0xFF00)>>8 ;
	tolPackCnt = (canBus_RxBuf[0]&0x000000FF);
	startAddr = (canBus_RxBuf[1]&0xFFFF0000)>>16;
	endAddr = (canBus_RxBuf[1]&0x0000FFFF);

//封包數目&rack adder 確認
    packetcount=packetCnt;
	canRxAddrEr=0;
	canRxPackEr=0;
    mask=0;

    for(totalCount=1;totalCount<(tolPackCnt);totalCount++)
	{
        mask = (canBus_RxBuf[totalCount*2]&0xFF000000)>>24;
        if(mask!=rackAddr)							// 接收到的封包rack位址是否同一個
            canRxAddrEr=1;

        packetcount += ((canBus_RxBuf[totalCount*2]&0x00FF0000)>>16);
    }

    if((packetcount*2)!=((tolPackCnt+1)*tolPackCnt))			// 接收到的封包數量是否與總封包數一樣
        canRxPackEr=1;
	else if(tolPackCnt!=canRxCnt)
        canRxPackEr=1;

    if((packetcount*2)==((tolPackCnt+1)*tolPackCnt))			// 接收到的封包數量是否與總封包數一樣
	{
        if(remove_address_error_count>0)
            remove_address_error_count--;

		address_right_count++;
	}

	if(address_right_count>=64)
    {
        address_right_count=0;

        if(address_repeat_error_count>0)
            address_repeat_error_count--;
	}

	if(remove_address_error_count==0)
	{
	//	ers_address_repeat_error=0;
	}

    if((packetcount*2)>((tolPackCnt+1)*tolPackCnt))			// 接收到的封包數量是否與總封包數一樣
    {
        if(load_start_flag ==0)
            address_repeat_error_count++;
	}

	if(address_repeat_error_count>4)
	{
	//	ers_address_repeat_error=1;
		remove_address_error_count=64;
	}

	if(canRxPackEr || canRxAddrEr)
	{
        for(totalCount=0;totalCount<33;totalCount++)
            canBus_RxBuf[totalCount]=0;
	}
	else if(tolPackCnt==canRxCnt)
	{	
		CANB_TO_cnt[rackAddr]=0	;
		
		if(endAddr<=0xFF)							//資料區域位於0x00~0xFF之間
        {
            address = &CANB_U[rackAddr].a00_operation;
            address +=(startAddr-0x00);
		}
		else if(startAddr>=0x0200 && endAddr<=0x020E)	//資料區域位於0x200~0x20E之間
        {
            address = &CANB_U[rackAddr].a200_OTP_1;
            address +=(startAddr-0x200);
		}
		else if(startAddr>=0x0220 && endAddr<=0x02FF)	//資料區域位於0x220~0x2FF之間
        {
            address = &CANB_U[rackAddr].a220_Revision;
            address +=(startAddr-0x220);
		}

        mask=0;
        for(totalCount=1;totalCount<(tolPackCnt);totalCount++)				//解封包資料
		{						
            *(address+mask) = (canBus_RxBuf[totalCount*2]&0x0000ffff)	;
            if(mask==(endAddr-startAddr))
            {
                break;
                //goto PackDone	;
            }
            mask++;
            *(address+mask) = (canBus_RxBuf[(totalCount*2)+1]&0xffff0000)>>16	;
            if(mask==(endAddr-startAddr))
            {
                break;
                //goto PackDone	;
            }
            mask++;
            *(address+mask) = (canBus_RxBuf[(totalCount*2)+1]&0x0000ffff) ;
            if(mask==(endAddr-startAddr))
            {
                break;
                //goto PackDone ;
            }
            mask++;
		}
//PackDone:
        for(totalCount=0;totalCount<33;totalCount++)						//clear all RX buffer //收資料上限等於45 BYTE
            canBus_RxBuf[totalCount]=0;
	}
	canRxDone=0;
	canTxDone=0;
	canTxStart=0;
	canRxCnt=0;
}

// /*** CAN Bus time out counter ***************************/
/*
void CANRackTOutCheck(qint16 unit)
{
    quint16	*address;

    for (unit;unit<unit+16;++unit)
    {
        if (CANB_TO_cnt[unit]<=1000)				// 2sec
            CANB_TO_cnt[unit]++;

        if (CANB_TO_cnt[unit]>=1000)				// time out?
        {
            if(canBusErrorCnt<=4000)	// 4sec
                canBusErrorCnt++;
            else
            {
                InitECana();			//  initialization ECAN register
                can_time_out_flag_a=1;
                canBusErrorCnt=0;
            }

            for (address=&CANB_U[unit].a00_operation;address<((&CANB_U[unit].a00_operation)+CMD_size);++address)
            {
                *address=0x0000;
            }

            //rack 0~15
            if(((racklinkFlag[0].all>>unit)&0x0001)==1)
            {
                racklinkFlag[0].all &= (~(0x0001<<unit));		// 1-->0
            }

        }
        else if (CANB_TO_cnt[unit]<1000)
        {
            //rack 0~15
            if(((racklinkFlag[0].all>>unit)&0x0001)==0)
            {
                racklinkFlag[0].all |= (0x0001<<unit);		// 1-->0
            }
        }
    }
}
*/
//**check rack now connect status *************************************************
int checkLink (quint16 rack)
{	
    if(rack>=48)										// rack  48~63
	{
        if(((racklinkFlag[3].all>>(rack-48))&0x0001)==0)
		{	return 0;	}							// rack 未連接
		else
		{	return 1; 	}							// rack 連接
	}
    else if(rack>=32 && rack<48)						// rack   32~47
	{
        if(((racklinkFlag[2].all>>(rack-32))&0x0001)==0)
		{	return 0;	}							// rack 未連接
		else
		{	return 1; 	}							// rack 連接
	}
    else if(rack>=16 && rack<32)						// rack   16~31
	{		
        if(((racklinkFlag[1].all>>(rack-16))&0x0001)==0)
		{	return 0;	}							// rack 未連接
		else
		{	return 1; 	}							// rack 連接
	}			
	else											// rack   0~15
    {	if(((racklinkFlag[0].all>>(rack))&0x0001)==0)
		{	return 0;	}							// rack 未連接
		else
		{	return 1; 	}							// rack 連接
	}
}

void erase_erm_type_flag(void)
{
    qint16 unit=0;

    if(clear_erm_type_flag==1)return;
    //清除ERM TYPE與ERROR狀態的紀錄  避免矩陣的初始值不為零 造成誤判
    while(unit<MAX_CANB_UNIT)
    {
        (CANB_U[unit].a20B_R_ERMCom_0)=0;
        (CANB_U[unit].a20C_R_ERMCom_1)=0;
        (CANB_U[unit].a20D_R_ERMCom_2)=0;
        (CANB_U[unit].a20E_R_ERMCom_3)=0;
        (CANB_U[unit].a207_Rfault)=0;
        (CANB_U[unit].a20A_R_ERMfault)=0;
        unit++;
    }
    clear_erm_type_flag=1;
}

void erm_type_check(void)
{
    volatile qint16 unit=0,temp=0;

    if(polling_around_flag==0)return;

    while(temp==0 && unit<MAX_CANB_UNIT)
    {
        if(((CANB_U[unit].a20B_R_ERMCom_0) & 0x2222) !=0){temp=1;}
        if(((CANB_U[unit].a20C_R_ERMCom_1) & 0x2222) !=0){temp=1;}
        if(((CANB_U[unit].a20D_R_ERMCom_2) & 0x2222) !=0){temp=1;}
        if(((CANB_U[unit].a20E_R_ERMCom_3) & 0x2222) !=0){temp=1;}
        unit++;
    }
    if(temp==0)erm_model_type_flag=1; //用來決定調整功率上下限的旗標  0:erm350 1:erm100
    if(temp==1)erm_model_type_flag=0; //用來決定調整功率上下限的旗標  0:erm350 1:erm100
    polling_around_flag=0;
}
//
qint16 CANRackCheck(volatile qint16 unit)
{
    volatile qint16 temp=0;

    while(temp==0 && unit<(unit+16))
    {
        if((CANB_U[unit].a207_Rfault) || (CANB_U[unit].a20A_R_ERMfault))
        {
            temp=1;
            faultUnit=unit;
        }
        unit++;
    }
    return temp;
}

void CANrackFaultCheck0()
{
    if(CANRackCheck(0))
    {
        error_code_r0=0;
        error_code_r0|=CANB_U[faultUnit].a207_Rfault;
        error_code_r0=error_code_r0<<8;
        error_code_r0|=CANB_U[faultUnit].a20A_R_ERMfault;
    }
    else
        error_code_r0=0;

}

void CANrackFaultCheck1()
{
    if(CANRackCheck(16))
    {
        error_code_r1=0;
        error_code_r1|=CANB_U[faultUnit].a207_Rfault;
        error_code_r1=error_code_r1<<8;
        error_code_r1|=CANB_U[faultUnit].a20A_R_ERMfault;
    }
    else
        error_code_r1=0;

}

void CANrackFaultCheck2()
{
    if(CANRackCheck(32))
    {
        error_code_r2=0;
        error_code_r2|=CANB_U[faultUnit].a207_Rfault;
        error_code_r2=error_code_r2<<8;
        error_code_r2|=CANB_U[faultUnit].a20A_R_ERMfault;
    }
    else
        error_code_r2=0;

}

void CANrackFaultCheck3()
{
    if(CANRackCheck(48))
    {
        error_code_r3=0;
        error_code_r3|=CANB_U[faultUnit].a207_Rfault;
        error_code_r3=error_code_r3<<8;
        error_code_r3|=CANB_U[faultUnit].a20A_R_ERMfault;
    }
    else
        error_code_r3=0;

}

void CANrackFaultcodesum(void)
{
    error_code = error_code_r0 | error_code_r1 | error_code_r2 | error_code_r3;

    if(can_time_out_flag_a && can_time_out_flag_b && can_time_out_flag_c && can_time_out_flag_d)
    {
        ers_disconnect_error=1;
        ers_address_repeat_error=0;
        address_repeat_error_count=0;
        Alarm_CMU.all=0;
    }

    if(((error_code&0x0004)!=0) && ERM_OVP_ERROR_COUNT<10)
        ERM_OVP_ERROR_COUNT++;

    if(((error_code&0x0004)==0) && ERM_OVP_ERROR_COUNT>0)
        ERM_OVP_ERROR_COUNT--;

    if(ERM_OVP_ERROR_COUNT>=4)
        ERM_OVP_FLAG=1;

    if(ERM_OVP_ERROR_COUNT==0)
        ERM_OVP_FLAG=0;

    if(((error_code&0x4000)!=0) && ERM_ADD_ERROR_COUNT<10)
        ERM_ADD_ERROR_COUNT++;

    if(((error_code&0x4000)==0) && ERM_ADD_ERROR_COUNT>0)
        ERM_ADD_ERROR_COUNT--;

    // if(ERM_ADD_ERROR_COUNT>=4)ERM_ADD_FLAG=1;
    // if(ERM_ADD_ERROR_COUNT==0)ERM_ADD_FLAG=0;

    if(((error_code&0x8000)!=0) && ERS_BUK_ERROR_COUNT<10)
        ERS_BUK_ERROR_COUNT++;

    if(((error_code&0x8000)==0) && ERS_BUK_ERROR_COUNT>0)
        ERS_BUK_ERROR_COUNT--;

    if(ERS_BUK_ERROR_COUNT>=4)
        ERS_BUK_FLAG=1;

    if(ERS_BUK_ERROR_COUNT==0)
        ERS_BUK_FLAG=0;
}
//
/*** CANBus control Process ***************************/
//
void checkTXTimeout()
{
    if(canTxStart==1)									//check TX time out
    {
        if(canBusTxToutCnt<8)						//6msec time out
        {	canBusTxToutCnt++;	}
        else
        {	canTxStart=0;
            ECanaRegs.CANTA.bit.TA0=1;				// Clear TA0 bit
            ECanaRegs.CANTA.bit.TA1=1;				// Clear TA1 bit
            ECanaRegs.CANGIF0.all = 0xFFFFFFFF;		// Clear all interrupt flag bits
            ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
        }
        if(ECanaRegs.CANES.bit.ACKE)
        {	canTxStart=0;
            ECanaRegs.CANAA.bit.AA0=1;
            ECanaRegs.CANTRS.all = 0x00000000;
            ECanaRegs.CANTA.bit.TA0=1;				// Clear TA0 bit
            ECanaRegs.CANTA.bit.TA1=1;				// Clear TA1 bit
            ECanaRegs.CANGIF0.all = 0xFFFFFFFF;		// Clear all interrupt flag bits
            ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
        }
    }
}
//
void checkData()
{
    if(canRxDone)													//check data
    {
        ers_disconnect_error=0;  //清除ERS沒有連線的錯誤
        can_time_out_flag_a=0;
        can_time_out_flag_b=0;
        can_time_out_flag_c=0;
        can_time_out_flag_d=0;
        CANBusRxProcess();

    }
}
//
void setCurrentPowerPerMinute()
{
    //當開始燒機時，每1分鐘寫入設定電流功率值。
    if(((rackON_OFF_ALL&0x00FF)==0x00AA)&&(SetCurrentCnt>=60))
    {
        SetCurrentCnt=0;
        setCANB_02 = 1;
        setCANB_03 = 1;
    }
}
/*
void pollingPer10msec()
{
    if((!canRxDone)&&(!canTxStart))
    {
        if((VTimer2[0]%10)==4)										// 10msec polling
            delay_tx_count=0;

        CANBusTxProcess();
    }
}

void canbusCommTest()
{
    //test
    if((VTimer2[1]%100)==5)
    {
        setCANB_02=1;
        setCANB_03=1;
        setCANB_00=1;
        setCANB_01=1;
        setCANB_04=1;
        setCANB_08=1;
    }
}
//
void checkRackCANbusTimeout(qint8 timer)
{
    //check rack CAN BUS time out  //本程序由於在中斷程序內執行  太長的話會影響到主程式迴圈的執行  故分成四次執行
    switch (timer)
    {
        case 1:
            CANRackTOutCheck(0);
            CANrackFaultCheck0();
            break;
        case 2:
            CANRackTOutCheck(16);
            CANrackFaultCheck1();
            break;
        case 3:
            CANRackTOutCheck(32);
            CANrackFaultCheck2();
            break;
        case 0:
            CANRackTOutCheck(48);
            CANrackFaultCheck3();
            break;
    }
}
*/
void canbusComm(void)
{
    checkTXTimeout();
    checkData();
    setCurrentPowerPerMinute();
    //pollingPer10msec();
	
    //canbusCommTest();

    //checkRackCANbusTimeout(VTimer2[0]%4);

    //check rack ERM TYPE          //本程序由於在中斷程序內執行
    erase_erm_type_flag();
    erm_type_check();
    CANrackFaultcodesum();
}
