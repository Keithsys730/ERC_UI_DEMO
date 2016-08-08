//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// CMU Global Variable define
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//#include "PeripheralHeaderIncludes.h"
#include <QtGlobal>
//#include "DSP280x_Gpio.h"

#ifndef CMU_GlobalVar_H
#define CMU_GlobalVar_H

//---------------------------------------------------------------------------
//Struct & Union variable 
//---------------------------------------------------------------------------
//struct define//
struct bit16_def {	        // bits		description
   quint16 bit_00:1;			// 0		bit0
   quint16 bit_01:1;			// 1		bit1
   quint16 bit_02:1;			// 2		bit2
   quint16 bit_03:1;			// 3		bit3
   quint16 bit_04:1;			// 4		bit4
   quint16 bit_05:1;			// 5		bit5
   quint16 bit_06:1;			// 6		bit6
   quint16 bit_07:1;			// 7		bit7
   quint16 bit_08:1;			// 8		bit8
   quint16 bit_09:1;			// 9		bit9
   quint16 bit_10:1;			// 10		bit10
   quint16 bit_11:1;			// 11		bit11
   quint16 bit_12:1;			// 12		bit12
   quint16 bit_13:1;			// 13		bit13
   quint16 bit_14:1;			// 14		bit14
   quint16 bit_15:1;			// 15		bit15
};

//union define//
union bit16_var {
    quint16					all;
	struct bit16_def		bit;
};

//variable//
#define     CC_CP_SELECT GpioDataRegs.GPADAT.bit.GPIO23
volatile int can_time_out_flag_a=0;
volatile int can_time_out_flag_b=0;
volatile int can_time_out_flag_c=0;
volatile int can_time_out_flag_d=0;
volatile int remove_address_error_count=0;
volatile int ers_address_repeat_error=0;
volatile int ers_disconnect_error=1;
union bit16_var data_4094;
#define		LED_R		data_4094.bit.bit_06	//flag Red-LED
#define		LED_G		data_4094.bit.bit_07	//flag Green-LED
union bit16_var flag1;
#define		ENT_key		flag1.bit.bit_00	//flag button ENT key	
#define		DN_key		flag1.bit.bit_01	//flag button DN key
#define		UP_key		flag1.bit.bit_02	//flag button UP key
#define		Load_key	flag1.bit.bit_03	//flag button Load key
#define		ENT_old		flag1.bit.bit_04	//flag ENT key, old data
#define		DN_old		flag1.bit.bit_05	//flag DN key, old data
#define		UP_old		flag1.bit.bit_06	//flag UP key, old data
#define		ESC_old		flag1.bit.bit_07	//flag ESC key, old data
#define		sw_cnt_lv1	flag1.bit.bit_08	//flag switch_cnt level 1
#define		sw_cnt_lv2	flag1.bit.bit_09	//flag switch_cnt level 2
//#define	flag1.bit.bit_10~11
#define		flg_1s		flag1.bit.bit_11	//flag 1sec
#define		E_buf_empty	flag1.bit.bit_12	//flag EEPROM buffer empty
#define		init_state	flag1.bit.bit_13	//flag initial state
#define		flg_50ms	flag1.bit.bit_14	//flag 50msec, reset by C-TASK
#define		flg_05s		flag1.bit.bit_15	//flag 0.5sec

union bit16_var flag2;
#define		EEP_error	flag2.bit.bit_00	//flag EEPROM error
#define		RTC_error	flag2.bit.bit_01	//flag EEPROM error
#define		PMB_W_busy	flag2.bit.bit_02	//flag PMBus busy (write)
#define		PT_NConnect flag2.bit.bit_03	// No link any RCP2000
#define		PT_Error	flag2.bit.bit_04	// RCP2000 Type error ,have two type
#define		StarC01F	flag2.bit.bit_05	// start psu flag
#define		VarModifyF  flag2.bit.bit_06	// variable modify eeprom flag    
#define		EventLogF	flag2.bit.bit_07	// Event log flag (1: set Event log )    
#define		F_ReLodEE	flag2.bit.bit_08	// reLoder Default EEPROM
#define		CMUStsrtPSU	flag2.bit.bit_09	// all RCP2000 Off/On flag
#define		PSUAll_Off	flag2.bit.bit_10	// to control all RCP2000 off flag
#define		F_PMB9ADone flag2.bit.bit_11	// PMBUS command 9Ah done	1:Done	0: runing		
#define		F_PassUlock flag2.bit.bit_12	// PASSWORD UnLOCK	1:Unlock ,0:lock
#define		F_LoDefault	flag2.bit.bit_13	// Load Default flag
#define		F_C_EvenLog	flag2.bit.bit_14	// clear Event Log flag
#define		F_C_DataLog	flag2.bit.bit_15	// clear Data Log flag

union bit16_var flag3;		//for EEPROM
#define		F_USB		flag3.bit.bit_00	//flag USB enable
#define		F_RS232		flag3.bit.bit_01	//flag RS232 enable
#define		F_RS23_GSM 	flag3.bit.bit_02	//flag RS232 to GSM enable
#define		F_C_Sound 	flag3.bit.bit_03	//Buzzer Alarm sound Control
#define		F_O_Sound 	flag3.bit.bit_05	//Buzzer Alarm OFF
union bit16_var flag4;
#define		pollingall 	flag4.bit.bit_00	// polling all pmBUS flag
#define		testHWF 	flag4.bit.bit_01	// for start debug
#define		pollingDone	flag4.bit.bit_02	//polling allpmbus flag done
#define		wriRangEr	flag4.bit.bit_03	//write rang error flag
#define		wriEEerror	flag4.bit.bit_04	//write EEPROM error flag

union bit16_var RI_flag;					// Relay flag / Input flag
#define		F_Relay1	RI_flag.bit.bit_00	//flag for Relay1
#define		F_Relay2	RI_flag.bit.bit_01	//flag for Relay2
#define		F_Relay3	RI_flag.bit.bit_02	//flag for Relay3
#define		F_Relay4	RI_flag.bit.bit_03	//flag for Relay4

//#define		F_Input1	RI_flag.bit.bit_08	//flag for Input controll 1
//#define		F_Input2	RI_flag.bit.bit_09	//flag for Input controll 2
//#define		F_Input3	RI_flag.bit.bit_10	//flag for Input controll 3
//#define		F_Input4	RI_flag.bit.bit_11	//flag for Input controll 4

volatile union bit16_var SetCANBus;
#define		setCANB_00	SetCANBus.bit.bit_00	//Set CAN BUS address 0x00h
#define		setCANB_01	SetCANBus.bit.bit_01	//Set CAN BUS address 0x01h
#define		setCANB_02	SetCANBus.bit.bit_02	//Set CAN BUS address 0x02h
#define		setCANB_03	SetCANBus.bit.bit_03	//Set CAN BUS address 0x03h
#define		setCANB_04	SetCANBus.bit.bit_04	//Set CAN BUS address 0x04h
#define		setCANB_08	SetCANBus.bit.bit_05	//Set CAN BUS address 0x08h

//union bit16_var oSetPMBusF;
//#define		oSF_PMBC01	oSetPMBusF.bit.bit_00	//Old set flag PMbus 0x01h
//#define		oSF_PMBC22	oSetPMBusF.bit.bit_01	//Old set flag PMbus 0x22h
//#define		oSF_PMBC46	oSetPMBusF.bit.bit_02	//Old set flag PMbus 0x46h
//#define		oSF_PMBC9C	oSetPMBusF.bit.bit_03	//Old set flag PMbus 0x9ch
//#define		oSF_PMBC9D	oSetPMBusF.bit.bit_04	//Old set flag PMbus 0x9dh
//#define		oSF_PMBC9E	oSetPMBusF.bit.bit_05	//Old set flag PMbus 0x9eh

union bit16_var SCI_Flag;
#define		SCIA_Rx		SCI_Flag.bit.bit_00		//	if 1: RXing
#define		SCIB_Rx		SCI_Flag.bit.bit_01		//
#define		SCIA_Tx		SCI_Flag.bit.bit_02		//
#define		SCIB_Tx		SCI_Flag.bit.bit_03		//
#define		SCIRxAProc	SCI_Flag.bit.bit_04		//	SCI RxA Process
#define		SCIRxBProc	SCI_Flag.bit.bit_05		//	SCI RxB Process

union bit16_var canBus_Flag;
#define		canRxDone 	canBus_Flag.bit.bit_00
#define		canRxStart 	canBus_Flag.bit.bit_01
#define		canTxDone 	canBus_Flag.bit.bit_02
#define		canTxStart 	canBus_Flag.bit.bit_03
#define		canRxAddrEr	canBus_Flag.bit.bit_04	//canbus RX data divice address error
#define		canRxPackEr	canBus_Flag.bit.bit_05	//canbus RX data packet error
#define		canTxStep	canBus_Flag.bit.bit_06	// 傳送參數設定時判斷是否已完成ALL RACK 資料的回讀
volatile quint16      polling_around_flag=0;              //用來判斷已經完成一輪的資料讀取
volatile quint16      clear_erm_type_flag=0;
volatile quint16      erm_model_type_flag=0;              //用來決定調整功率上下限的旗標  0:erm350 1:erm100
volatile quint16      change_page_timer_count=0;          //用來減慢換頁後的速度
volatile quint16      change_conn_page_timer_count=0;          //用來減慢換頁後的速度
union bit16_var burnInMode;
#define		burnInCCM 	burnInMode.bit.bit_00
#define		burnInCPM 	burnInMode.bit.bit_01

volatile unsigned int error_code=0;      //用來紀錄發生的錯誤
volatile unsigned int error_code_r0=0;   //用來紀錄發生的錯誤ers0~15
volatile unsigned int error_code_r1=0;   //用來紀錄發生的錯誤ers16~31
volatile unsigned int error_code_r2=0;   //用來紀錄發生的錯誤ers32~47
volatile unsigned int error_code_r3=0;   //用來紀錄發生的錯誤ers48~63
volatile unsigned int erro_matrix[11]={0,0,0,0,0,0,0,0,0,0};
volatile unsigned int delay_tx_count=0;
volatile int error_show_nb=0,error_time_flag=0,error_stroe_nb=0;
#define		canRxDone 	canBus_Flag.bit.bit_00
#define		canRxStart 	canBus_Flag.bit.bit_01
union bit16_var eePROM1;					// for debug test check
union bit16_var eePROM2;					// for debug test check
union bit16_var eePROM3;					// for debug test check	
union bit16_var test1;						// for debug test check

union bit16_var racklinkFlag[4]={0,0,0,0} ;			//current run quantity flag
union bit16_var old_racklinkFlag[4]={0,0,0,0} ;		//Old quantity flag

//---------------------------------------------------------------------------
//General variable
//---------------------------------------------------------------------------
//LCD & menu
quint16	LCD_state=64800;	//65500;	// LCD state-machine
quint16	LCD_index=0xFF;		// index for LCD RAM, 0xFF: update finished
char	LCD_buff[80];		// LCD buffer, 20char*4line
quint16	menu[5]={0,99,0,0,0};	// LCD menu index
quint16	LCD_InitDelay=0;

//on_off_delay
volatile int on_off_count=0;

//ADC
int		V_switch[2]={0,0};	// ADC value for panel switch
int		V_li_batt=0;		// ADC value for Li-battery
//cnt
quint16	switch_cnt=0;		// counter for panel switch
quint16	switch_cnt2=0;		// counter for panel switch
quint16	switch_hold_cnt=0;	// hold counter for panel switch
quint16  PSUOn_Cnt=0;		//
quint16	Sound_Cnt=0;		// [OFF : Sound]
quint16	switch_holdF=0;		//
quint16	ReChkSetcnt=0;		// recheck set command cnt
quint16	RetrySetcnt=0;		// re-try set command cnt
quint16	CMUAlrDelycnt=0;	// cmu alram delay counter
int  	swTimout=0;			//
quint16	hwTestcnt=0;		// into H.W. test mode cnt
quint16	eeWriteCnt=0;		// Alarm check time
quint16	burnInCnt=0;
quint16	qepSound_Cnt=0;
	
//RTC
quint16	RTC_SEC=0;			//seconds of RTC
quint16	RTC_MIN=0;			//minutes of RTC
quint16	RTC_HOUR=0;			//hours of RTC
quint16	RTC_DAY=0;			//day of RTC
quint16	RTC_DATE=0;			//date of RTC
quint16	RTC_MONTH=0;		//month of RTC
quint16	RTC_YEAR=0;			//year of RTC
quint16	RTC_CONTROL=0;		//control of RTC
//EEPROM
quint16	evLog_buff[8]={0,0,0,0,0,0,0,0};
quint16	EEP_buff[8]={0,0,0,0,0,0,0,0};		//EEPROM buffer, 16-bit data [addr_H,addr_L]
quint16	EEindex[6]={0,0,0,0,0,0};			//EEPROM index
//common
quint16	ASC[5];				//ASCII output for data convert
//temp. variable
quint16	main_temp[5];			//temp. for main routine
//quint16	T1_temp[5];			//temp. for T1 routine
//quint16	T2_temp[5];			//temp. for T2 routine

#define	SCI_buf_size	32			//for using RAM
quint16	SCI_A_buf[SCI_buf_size];	//receive buffer for SCI-A
quint16	SCI_A_head=0;				//head index for SCI-A
quint16	SCI_A_tail=0;				//tail index for SCI-A
quint16	cmd_A_index=0;				//index for command, SCI-A
quint16	cmd_A[SCI_buf_size];		//command buffer for SCI-A
quint16	SCI_B_buf[SCI_buf_size];	//receive buffer for SCI-B
quint16	SCI_B_head=0;				//head index for SCI-B
quint16	SCI_B_tail=0;				//tail index for SCI-B
quint16	cmd_B_index=0;				//index for command, SCI-B
quint16	cmd_B[SCI_buf_size];		//command buffer for SCI-B
quint16	*cmd_ptr=0;					//command pointer for SCI-A & B
quint16	cmd_step=0xFFFF;			//command step for SCI-A & B
volatile quint16	*TXBUF_ptr;			//Txbuffer index
volatile quint16	*FFTX_ptr;
quint16	*Rxptr=0;
quint16	*Txptr=0;
quint16	TxAbyteCnt[2]={0,0};
quint16	TxBbyteCnt[2]={0,0};
quint16	psuStatuTemp=0;
quint16	nWriteaddr=0;				//
quint16  address_repeat_error_count=0;
quint16  address_right_count=0;
quint16  ERM_OVP_ERROR_COUNT=0;
quint16  ERM_ADD_ERROR_COUNT=0;
quint16  ERS_BUK_ERROR_COUNT=0;
quint16  ERM_OVP_FLAG=0;
quint16  ERM_ADD_FLAG=0;
quint16  ERS_BUK_FLAG=0;
quint16	SCI_A_Rbuf[255];
quint16	SCI_B_Rbuf[255];
quint16	SCI_A_Tbuf[255];
quint16	SCI_B_Tbuf[255];

quint16	SCI_A_Rtout=0;
quint16	SCI_B_Rtout=0;

//by yifeng 
qint16	DispMData_Temp=0;				// Display modify data temp.
//volatile quint8	DispMData_Q8Temp=quint8(00.50);//0;		// Display modify data temp. (Q8 type )
//volatile quint8	DispMData_Q6Temp=quint8(05.00);//0;
//quint16	Data_LogCnt=0;				// Data log counter
//quint16	DLogIntervalT=0;			// Data log interval time
//quint16	DLogIndex[2]={0,0};			// Data log EEPROM address index
//quint16	Password=0;					// password key for MAINTENANCE
//quint16  	All_C22_VOUT_TRIM=0;		// set All PSU VOUT trim
//quint16	All_C46_IOUT_OC_FL=0;		// set all psu Iout limit
//quint16	IOUT_OC_FL[32];				//
//quint16	C01_OPER[32];				//
//quint16	oC01_OPER[32];				// old pmbus c01 buffer;
quint16	BusVoltage=0;				// bus voltage
quint16	TotalIout=0;				//

quint16  Buzzer_Cnt=0;				// Buzzer Sound cnt hi byte:Sound 0FF time ,lo byte: Sound 0N time
quint16	ProRelay[4]={0,0,0,0};		// [alarmF:15~12,diginF:11~8,PSUOffF:7~6,PSUOnF:5~4,Type:3~0 ]
quint16	ReDelayTim[4]={0,0,0,0};	// relay delay time set
quint16	ReDelayCnt[4]={0,0,0,0};	// relay delay time conter
quint16	All_IOUT_SUM=0;				// All C8C_RD_IOUT SUM
quint16 	Vin_C88Min=0;				// PMBUS Min C88h_RD_VIN
quint16 	Vin_C88Max=0;				// PMBUS Max C88h_RD_VIN
quint16	Vout_C8B=0;					// PMBUS C8Bh_RD_Vout
quint16	Alarmflash=0;				// alarm flash flag
quint16	PSU_Units=0;				// current rcp2000 run quantity
//quint16	EventBuf[33];
//CAN BUS
quint32	canBus_RxBuf[32];			//Save RX data buffer
quint16	canRxCnt=0;
quint16	canTxCnt=0;
quint16	Data_Polling =0;			//傳送各RACK的資料輪詢位置
volatile quint16	CAN_Pollcnt[3]={0,0,0};		// for CANbus polling 0:polling rack addr 1:data polling
quint16	CANB_STS1=0;				// CanBus status1 1=read data, 2=set data
quint16	canBusTxToutCnt=0;			// Tx start time out conter
quint16	CMUaddr=1;					// CMU device number (1:master 2:)
quint16	rxBuffCnt=0;
quint16	setStep[7]={0,0,0,0,0,0,0};	// 傳送參數設定時,各設定所需的SWITCH STEP
quint16	canBusErrorCnt=0;				// CAN BUS 連線異常計時器
quint16  canstartCnt=0;
quint16  SetCurrentCnt=0;

quint16	rackON_OFF[32];
quint16	rackON_OFF_ALL=0xFF00;
//
quint8	rackCCI=quint8(00.50);//0;	I_minvalue=0.1A	0;		// RACK all module	CC mode current
quint8	rackCPP=quint8(05.00);//0;	P_minvalue=2.0W	0;		// RACK all module	CP mode power
quint8	rackCCI_m1=0;				// RACK  module	1 CC mode current
quint8	rackCCI_m2=0;				// RACK  module	2 CC mode current
quint8	rackCCI_m3=0;				// RACK  module	3 CC mode current
quint8	rackCCI_m4=0;				// RACK  module	4 CC mode current
quint8	rackCCI_m5=0;				// RACK  module	5 CC mode current
quint8	rackCCI_m6=0;				// RACK  module	6 CC mode current
quint8	rackCCI_m7=0;				// RACK  module	7 CC mode current
quint8	rackCCI_m8=0;				// RACK  module	8 CC mode current
quint8	rackCCI_m9=0;				// RACK  module	9 CC mode current
quint8	rackCCI_m10=0;				// RACK  module	10 CC mode current
quint8	rackCCI_m11=0;				// RACK  module	11 CC mode current
quint8	rackCCI_m12=0;				// RACK  module	12 CC mode current
quint8	rackCCI_m13=0;				// RACK  module	13 CC mode current
quint8	rackCCI_m14=0;				// RACK  module	14 CC mode current
quint8	rackCCI_m15=0;				// RACK  module	15 CC mode current
quint8	rackCCI_m16=0;				// RACK  module	16 CC mode current
quint8	rackCPP_m1=0;				// RACK  module	1 CP mode power
quint8	rackCPP_m2=0;				// RACK  module	2 CP mode power
quint8	rackCPP_m3=0;				// RACK  module	3 CP mode power
quint8	rackCPP_m4=0;				// RACK  module	4 CP mode power
quint8	rackCPP_m5=0;				// RACK  module	5 CP mode power
quint8	rackCPP_m6=0;				// RACK  module	6 CP mode power
quint8	rackCPP_m7=0;				// RACK  module	7 CP mode power
quint8	rackCPP_m8=0;				// RACK  module	8 CP mode power
quint8	rackCPP_m9=0;				// RACK  module	9 CP mode power
quint8	rackCPP_m10=0;				// RACK  module	10 CP mode power
quint8	rackCPP_m11=0;				// RACK  module	11 CP mode power
quint8	rackCPP_m12=0;				// RACK  module	12 CP mode power
quint8	rackCPP_m13=0;				// RACK  module	13 CP mode power
quint8	rackCPP_m14=0;				// RACK  module	14 CP mode power
quint8	rackCPP_m15=0;				// RACK  module	15 CP mode power
quint8	rackCPP_m16=0;				// RACK  module	16 CP mode power
//
//QEP
quint16	qepCntNew=0;
quint16  qepCntOld=0;
volatile quint16	qepCntDiff=0;
quint16  qepCnt=0;
quint16  Buzzer_OnCnt=0;
quint16  Buzzer_OffCnt=0;
volatile qint16	qepCntSpeed=0;



quint16	CMUFirwVer=01;									// 10=CMU Firware ver1.0
//quint16	MACaddress[3]={0,0,0};						// CMU MAC address
//quint16	IPaddress[2]={0,0};							// CMU IP address
//quint16	SubnetMask[2]={0,0};						// CMU Subnet mask
//quint16	Gateway[2]={0,0};							// CMU Gateway
//quint16	CMU_Date[3]={0,0,0};						// CMU MFR Date
//quint16	CMU_Serial[6]={0,0,0,0,0,0};				// CMU MFR serial number
//quint16	CMU_Loca[3]={0,0,0};						// CMU MFR Location
//quint16	gsmPrcessStatus=0;							// GSM send prcess status
//quint16	gsmMessagBuf[255];							// GSM messages buffer
//quint16	gsmErrorCnt=0;								// GSM error
//quint16	mobiePhone[10]={0,0,0,0,0,0,0,0,0,0};
//quint16	sendSMScnt=0;
//quint16	sendSMStime=0;
//quint16	eePSUtypeTemp=0;							//EEPROM  PSUtype
quint16	wriEEerrorCnt=0;							//EEPROM RETrycnt

//---------------------------------------------------------------------------
//CANBus data for ERL-2000 RACK (read back data)
//---------------------------------------------------------------------------
//#define	MAX_PMB_UNIT	3		//for using RAM
#define	MAX_CANB_UNIT	64   		//for using FLASH
#define	CMD_size	58			//command size in 16-bit
#define	CANB_RAMSIZE		MAX_CANB_UNIT*CMD_size		//total 94 words per unit
struct CANB_data {				//addr	description
quint16	a00_operation	;			//	OPERATION                                   */
quint16	a01_clear_Faults;			//	clear power module fault flag
quint16	a02_setCCM_R;				//	setting  CCmode rack current
quint16	a03_setCPM_R;				//  setting  CPmode rack power
quint16	a04_setCCM_M1;				//  setting  CCmode power module 1 current
quint16	a05_setCCM_M2;				//  setting  CCmode power module 2 current
quint16	a06_setCCM_M3;				//  setting  CCmode power module 3 current
quint16	a07_setCCM_M4;				//  setting  CCmode power module 4 current
quint16	a08_setCCM_M5;				//  setting  CCmode power module 5 current
quint16	a09_setCCM_M6;				//  setting  CCmode power module 6 current
quint16	a0A_setCCM_M7;				//  setting  CCmode power module 7 current
quint16	a0B_setCCM_M8;				//  setting  CCmode power module 8 current
quint16	a0C_setCCM_M9;				//  setting  CCmode power module 9 current
quint16	a0D_setCCM_M10;				//  setting  CCmode power module 10 current
quint16	a0E_setCCM_M11;				//  setting  CCmode power module 11 current
quint16	a0F_setCCM_M12;				//  setting  CCmode power module 12 current
quint16	a10_setCCM_M13;				//  setting  CCmode power module 13 current
quint16	a11_setCCM_M14;				//  setting  CCmode power module 14 current
quint16	a12_setCCM_M15;				//  setting  CCmode power module 15 current
quint16	a13_setCCM_M16;				//  setting  CCmode power module 16 current
quint16	a14_setCPM_M1;				//  setting  CCmode power module 1 power
quint16	a15_setCPM_M2;				//  setting  CCmode power module 2 power
quint16	a16_setCPM_M3;				//  setting  CCmode power module 3 power
quint16	a17_setCPM_M4;				//  setting  CCmode power module 4 power
quint16	a18_setCPM_M5;				//  setting  CCmode power module 5 power
quint16	a19_setCPM_M6;				//  setting  CCmode power module 6 power
quint16	a1A_setCPM_M7;				//  setting  CCmode power module 7 power
quint16	a1B_setCPM_M8;				//  setting  CCmode power module 8 power
quint16	a1C_setCPM_M9;				//  setting  CCmode power module 9 power
quint16	a1D_setCPM_M10;				//  setting  CCmode power module 10 power
quint16	a1E_setCPM_M11;				//  setting  CCmode power module 11 power
quint16	a1F_setCPM_M12;				//  setting  CCmode power module 12 power
quint16	a20_setCPM_M13;				//  setting  CCmode power module 13 power
quint16	a21_setCPM_M14;				//  setting  CCmode power module 14 power
quint16	a22_setCPM_M15;				//  setting  CCmode power module 15 power
quint16	a23_setCPM_M16;				//  setting  CCmode power module 16 power
quint16	a200_OTP_1;			        //  RACK溫度偵測點1的溫度值
quint16	a201_OTP_2;		 	        //  RACK溫度偵測點2的溫度值
quint16	a202_Vbuck;			        //  RACK輸出端電壓
quint16	a203_Vboost;			    //  RACK_BOOST電壓
quint16	a204_Isense;			    //  RACK_BOOST級電流
quint16	a205_BoostOutP;			    //  RACK_BOOST提供功率
quint16	a206_Rstatus;			    //  RACK本身的運轉狀態旗標
quint16	a207_Rfault;			    //  RACK本身的告警旗標
quint16	a208_R_ERMstatus_0;			//  RACK的ERM運作狀態旗標
quint16	a209_R_ERMstatus_1;			//  RACK的ERM運作狀態旗標
quint16	a20A_R_ERMfault;			//  RACK的ERM告警旗標
quint16	a20B_R_ERMCom_0;			//  RACK的ERM連線旗標
quint16	a20C_R_ERMCom_1;			//  RACK的ERM連線旗標
quint16	a20D_R_ERMCom_2;			//  RACK的ERM連線旗標
quint16	a20E_R_ERMCom_3;			//  RACK的ERM連線旗標
quint16	a220_Revision;			    //  ERS韌體版本
quint16	a221_Model_0;				//  Rack型號
quint16	a222_Model_1;				//  Rack型號
quint16	a223_Model_2;				//  Rack型號
quint16	a224_Serial_YM;				//  Rack出廠日(年月)
quint16	a225_Serial_0D;				//  Rack出廠日(日)
quint16	a226_Serial_NN;				//  Rack出場日(流水號)
};
struct CANB_data CANB_U[MAX_CANB_UNIT];
quint16	CANB_TO_cnt[MAX_CANB_UNIT];	//CANBus rack time out counter
//---------------------------------------------------------------------------
//ERL2K Alarm_data----------------------------------------------------------------
//---------------------------------------------------------------------------
struct Alarm_data {				//addr	description
    quint16	Vout_OV:1;			// 0	Vout overvoltage fault
    quint16	Iout_OC:1;			// 1	Iout overcurrent fault
    quint16	Iout_Short:1;		// 2	Iout current short
    quint16	AC_Fault:1;			// 3	AC input voltage
    quint16	Fan1_Fault:1;		// 4	Fan1_Fault
    quint16	Fan2_Fault:1;		// 5	Fan2_Fault
    quint16	Temp_Over:1;		// 6	OverTemperture
    quint16	Temp_High:1;		// 7	HighTemperture
    quint16	NC8:1;				// 8
    quint16	NC9:1;				// 9
    quint16	NC10:1;				// a
    quint16	NC11:1;				// b
    quint16	NC12:1;				// c
    quint16	NC13:1;				// d
    quint16	SAlarm:1;			// e    sleve  alarm (High temp)
    quint16	MAlarm:1;			// f	master alarm (OVP,OLP,OTP,Fanlock,OCshort )
};
//struct Alarm_data Alarm1_PSU[MAX_PMB_UNIT];
//union define//
union bit16_Alarmvar {
    quint16					all;
	struct Alarm_data		bit;
};
union bit16_Alarmvar Alarm1_PSU[MAX_CANB_UNIT] ;
quint16	OldAlarm1_PSU[MAX_CANB_UNIT];
//---------------------------------------------------------------------------
//CMU Alarm_data----------------------------------------------------------------
//---------------------------------------------------------------------------
struct Alarm_CMUdata {			//addr	description
    quint16	EthernetF:1;		// 0	Ethernet fault
    quint16	RS232F:1;			// 1	USB/RS232  fault
    quint16	EEP_errorF:1;		// 2	flag EEPROM error
    quint16	RTC_errorF:1;		// 3	flag EEPROM error
    quint16	PT_NConnectF:1;		// 4	No connect any RCP2000
    quint16	PT_ErrorF:1;		// 5	RCP2000 Type error ,have are two type
    quint16	WriC00Er:1;			// 6	設定RACK開關機時資料異常
    quint16	WriC01Er:1;			// 7
    quint16	WriC02Er:1;			// 8   設定RACK CC mode 電流參數資料異常
    quint16	WriC03Er:1;			// 9   設定RACK CP mode 功率參數資料異常
    quint16	WriC04Er:1;			// a   設定RACK module CC mode 電流參數資料異常
    quint16	WriC08Er:1;			// b   設定RACK module CP mode 功率參數資料異常
    quint16	canBusError:1;				// c
    quint16	NC13:1;				// d
    quint16	NC14:1;				// e
    quint16	NC15:1;				// f
};
//union define//
union bit16_Alarmvar1 {
    quint16					all;
	struct Alarm_CMUdata	bit;
};
union bit16_Alarmvar1 Alarm_CMU ;
quint16	OldAlarm_CMU=0;

//---------------------------------------------------------------------------
//variable, store in EEPROM
//---------------------------------------------------------------------------
quint16	LCD_BL_SET;			//LCD backlight setting
quint16	LCD_CST_SET;		//LCD contrast setting

//---------------------------------------------------------------------------
// CPU Timer Definitions:
// uSec
// Timer definitions based on 100MHz System Clock
#define		uSec10		1000		// 10 uS
#define		uSec20		2000		// 20 uS
#define		uSec50		5000		// 50 uS
#define		uSec100		10000		// 100 uS
#define		uSec200		20000		// 200 uS
#define		uSec500		50000		// 500 uS


//---------------------------------------------------------------------------
// GPIO SET/CLEAR define
#define	SDA_OUT		GpioCtrlRegs.GPADIR.bit.GPIO6 = 1
#define	SDA_IN		GpioCtrlRegs.GPADIR.bit.GPIO6 = 0
#define	SDA_0		GpioDataRegs.GPACLEAR.bit.GPIO6 = 1
#define	SDA_1		GpioDataRegs.GPASET.bit.GPIO6 = 1
#define	SCL_0		GpioDataRegs.GPACLEAR.bit.GPIO7 = 1
#define	SCL_1		GpioDataRegs.GPASET.bit.GPIO7 = 1

//---------------------------------------------------------------------------
// I2C device address define
#define addr_M41T0		0x00D0			//RTC: M41T0
//#define addr_AT24512_0	0x00A0			//EEPROM: AT24C512B, device0
#define addr_AT24512_1	0x00A0			//EEPROM: AT24C512B, device1
volatile int pass_test_mode=0,test_into_count=0,test_into_flag=0,load_key_count=0;
//---------------------------------------------------------------------------
// PMBus device address define
#define addr_RCP2K_0	0x0080			//RCP2000 address 0, unit1, 1000000xb

//---------------------------------------------------------------------------
// tables
const char	table_BL[]		=	{5000,7000,9000,11000,13000,15000,17000,19000};
const char	table_CST[]		=	{18000,16500,15000,13500,12000,10500,9000,7500};

//CANBus poll table
const char table_CANBPoll[]	=	{1,2,3,1,2,4};

//pmbus poll table
//const char	table_PMBPoll[]	=	{ 0x79,0x7A,0x7B,0x81,0x88,0x8B,0x8C,0x8D,0x91,0x9A,0x88,0x8B,0x8C
//								 ,0x79,0x7B,0x7C,0x7D,0x81,0x88,0x8B,0x8C,0x90,0x9A,0x88,0x8B,0x8C
//								 ,0x79,0x7A,0x7C,0x7D,0x8B,0x8C,0x99,0x9B,0x88,0x8B,0x8C
//								 ,0x9D,0x9E,0x22,0x9C
//								};
							//	{ 0x79,0x7B,0x7D,0x88,0x8B,0x8C,0x8D,0x90,0x91,0x9A,0x88,0x8B,0x8C
							//	 ,0x79,0x7B,0x7D,0x88,0x8B,0x8C,0x02,0x19,0x20,0x21,0x47,0x88,0x8B,0x8C
							//	 ,0x79,0x7B,0x7D,0x88,0x8B,0x8C,0x8D,0x90,0x91,0x9A,0x88,0x8B,0x8C
							//	 ,0x79,0x7B,0x7D,0x88,0x8B,0x8C,0x98,0x99,0x9B,0x88,0x8B,0x8C 
							//	};

quint16	*table_MODaddr[255];
quint16	SCI_Tbuf[128];
quint16	SCI_Rbuf[128];
//---------------------------------------------------------------------------
// ASCII constant
#define key_CR	0x000D			//
#define key_LF	0x000A			//

//---------------------------------------------------------------------------
//RS232/USB
//#define USB_Enable  GpioDataRegs.GPASET.bit.GPIO24 =1			//
//#define RS232_Enable  GpioDataRegs.GPACLEAR.bit.GPIO24 =1			//

//---------------------------------------------------------------------------
//Relay
//#define Relay1On  GpioDataRegs.GPASET.bit.GPIO8 =1			//
//#define Relay1Off GpioDataRegs.GPACLEAR.bit.GPIO8 =1			//
//#define Relay2On  GpioDataRegs.GPASET.bit.GPIO9 =1			//
//#define Relay2Off GpioDataRegs.GPACLEAR.bit.GPIO9 =1			//
//#define Relay3On  GpioDataRegs.GPASET.bit.GPIO10 =1			//
//#define Relay3Off GpioDataRegs.GPACLEAR.bit.GPIO10 =1			//
//#define Relay4On  GpioDataRegs.GPASET.bit.GPIO11 =1			//
//#define Relay4Off GpioDataRegs.GPACLEAR.bit.GPIO11 =1			//
//
//---------------------------------------------------------------------------
//---EEPROM Default
#define	IlimitDef_12V	0xEB80		//0x0380	//112A	//0x0320	//100A
#define	IlimitDef_24V	0xEACD		//0x02cd	//89.6A	//0x0280	//80A
#define	IlimitDef_48V	0xE978		//0x0178	//47A	//0x0150	//42A
#define	VoutTrimDef		0			// 0v
#define	flag3Def		0x0009		// flag3 (Bit5~3:buzzer controll) (Bit5~3:RS232/USB switch)
#define	BuzzerCntDef	0x0101		// Buzzer Sound cnt hi byte:Sound 0FF time ,lo byte: Sound 0N time 
#define	DataLogTDef		5
#define	ProRelay1Def	0x1010
#define	ProRelay2Def	0x1010
#define	ProRelay3Def	0x1010
#define	ProRelay4Def	0x1010
#define	PasswordDef		0
#define	LCD_BL_Def		5
#define	LCD_CST_Def		5
#define	ELogIndexDef	0
#define	ELogMaxNDef		0
#define	DLogMaxNDef		0
#define	ReDelayTimDef	300
#define mobiePhoneDef	0

//---------------------------------------------------------------------------
// modbus start address
//---------------------------------------------------------------------------
#define	CMURSTATUADDR	0x8000		//CMU read ststus address
#define	CMURSTATUEND	0x800D		//CMU read ststus address end
#define	PSURSTATUADDR	0x8100		//PSU read ststus address
#define	PSURSTATUEND	0x82C0		//PSU read ststus address end
#define	CMURWDATAADDR	0x8600		//CMU read/write data address
#define	CMURWDATAEND	0x8631 		//CMU read/write data address end
#define	PSURWDATAADDR	0x8700		//PSU read/write data address
#define	PSURWDATAEND	0x8E40		//PSU read/write data address end
#define	CMUCONTRADR		0x9B00		//CMU control address
#define	CMUCONTREND		0x9B04		//CMU control address end
#define	CMUREVENTADDR	0x9C00		//CMU read Event log address
#define	CMUREVENTEND	0x9E59		//CMU read Event log address end
#define	CMURDATAADDR	0x9F00		//CMU read Data log address
#define	CMURDATAEND		0xA2E8		//CMU read Data log address end

//---------------------------------------------------------------------------

volatile int load_start_flag=0;
qint16 VTimer2[4];
#define Relay1On GpioDataRegs.GPASET.bit.GPIO8 = 1
#define Relay1Off GpioDataRegs.GPACLEAR.bit.GPIO8 = 1
#define Relay2On GpioDataRegs.GPASET.bit.GPIO9 = 1
#define Relay2Off GpioDataRegs.GPACLEAR.bit.GPIO9 = 1
#define Relay3On GpioDataRegs.GPASET.bit.GPIO10 = 1
#define Relay3Off GpioDataRegs.GPACLEAR.bit.GPIO10 = 1
#define Relay4On GpioDataRegs.GPASET.bit.GPIO11 = 1
#define Relay4Off GpioDataRegs.GPACLEAR.bit.GPIO11 = 1

//volatile struct GPIO_DATA_REGS GpioDataRegs;
//volatile struct GPIO_CTRL_REGS GpioCtrlRegs;
//volatile struct GPIO_INT_REGS GpioIntRegs;
#endif



