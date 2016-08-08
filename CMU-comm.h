//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// CMU modbus data Process ,
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//#include "PeripheralHeaderIncludes.h"
#include <QtGlobal>
//#include "CMU-GlobalVar.h"
///*** GSM Message ***************************/                          
/* //									 01234567890123456789012345678901 */
/* const char	message_at[]		=	"AT";                             */
/* const char	message_SMS1[]		=	"AT+CNMI=0,1,1,1,0";              */
/* const char	message_SMS2[]		=	"AT+CSMP=17,169,0,0";             */
/* const char	message_SMS3[]		=	"AT+CMGF=1";                      */
/* const char	message_SMS4[]		=	"AT+CMGS=";                       */
/* const char	message_SMS5[]		=	"> ";                             */
/* const char	message_SMS6[]		=	"RKP-CMU EventLog message :";     */
/* const char	message_ok[]		=	"OK";                             */
/* const char	message_error[]		=	"ERROR";                          */
/*                                                                        */
/* const char	message_SMS_0[]		=	" OVP  Fault ,";                  */
/* const char	message_SMS_1[]		=	" OLP  Fault ,";                  */
/* const char	message_SMS_2[]		=	" Short ckt  ,";                  */
/* const char	message_SMS_3[]		=	" AC   Fault ,";                  */
/* const char	message_SMS_4[]		=	" Fan1 Lock  ,";                  */
/* const char	message_SMS_5[]		=	" Fan2 Lock  ,";                  */
/* const char	message_SMS_6[]		=	" OTP  Fault ,";                  */
/* const char	message_SMS_7[]		=	" High Temp. ,";                  */
/*                                                                        */
/* const char	message_SMS_10[]	=	"Ethnet error,";                  */
/* const char	message_SMS_11[]	=	" RS232 error,";                  */
/* const char	message_SMS_12[]	=	"EEPROM error,";                  */
/* const char	message_SMS_13[]	=	" RTC   error,";                  */
/* const char	message_SMS_14[]	=	" PMBus error,";                  */
/* const char	message_SMS_15[]	=	" Model error,";                  */
/* const char	message_SMS_16[]	=	"Alarm remove,";                  */
/* const char	message_SMS_17[]	=	"W PM01 Error,";                  */
/* const char	message_SMS_18[]	=	"W PM22 Error,";                  */
/* const char	message_SMS_19[]	=	"W PM46 Error,";                  */
/* const char	message_SMS_20[]	=	"W PM9C Error,";                  */
/* const char	message_SMS_21[]	=	"W PM9D Error,";                  */
/* const char	message_SMS_22[]	=	"W PM9E Error,"                   */
/*** Modbus Tx Process ***************************/
/*
void ModbusTxAProcess(void)
{	quint16 temp,tempCnt[2];
	
	if(Txptr==&SCI_A_Tbuf[0])
	{	tempCnt[0]=TxAbyteCnt[0];
		tempCnt[1]=TxAbyteCnt[1];
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	tempCnt[0]=TxBbyteCnt[0];
		tempCnt[1]=TxBbyteCnt[1];
	}		
	temp=*FFTX_ptr;
	if ((((temp)>>8)& 0x1F) ==0)			//if buffer not enough
	{			
			temp=*(Txptr+tempCnt[0]);
		
			*TXBUF_ptr=temp;
			tempCnt[0]++;
			if(tempCnt[1]==(tempCnt[0]-1))			//TX finish?
			{
				if(Txptr==&SCI_A_Tbuf[0])
				{	SCIA_Tx=0;}
				else if(Txptr==&SCI_B_Tbuf[0])
				{	SCIB_Tx=0;}
			}						
		if(Txptr==&SCI_A_Tbuf[0])
		{	TxAbyteCnt[0]=tempCnt[0];}
		else if(Txptr==&SCI_B_Tbuf[0])
		{	TxBbyteCnt[0]=tempCnt[0];}
	}	
}
*/
/*** Modbus CRC16 Rx/Tx ***************************/
int CRC16 (quint16 length,quint16 *addr)
{	quint16 calcCRC16,aa,ii,dd;

 	calcCRC16= 0xFFFF;	
	for(aa=0;aa<=length;aa++)	
	{
		dd=*(addr+aa)&0x00FF;	
		calcCRC16 ^=dd ;
		for(ii = 0; ii<8; ii++)
		{				
			if(calcCRC16 & 0x0001 )	 
			{	calcCRC16 = ((calcCRC16 >>1)^0xA001);	}  	 // Carry bit =1 ?? 
			else
			{	calcCRC16 >>= 1;	}  	    
 		}		
	}		
	return calcCRC16;
}
/*** Modbus Error  ***************************/
/*
void ModbusEr(quint16 addr,quint16 function ,quint16 erCode)
{	quint16 mi=0,aa;
	
	if(addr!=0)			
	{	*Txptr= addr;		//CMU device address
		mi++;				//Mi= SCI Tx buffre index
	}
	else
	{	return;	}		
	*(Txptr+mi)=(function+0x80);
	mi++;	
	*(Txptr+mi)= 0x0001;	//byte conter
	mi++;
	*(Txptr+mi)= erCode;	//Error code
	
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;
	mi++;
	*(Txptr+mi)= aa>>8;
	
	*(Txptr+256)=mi;				//save byte Quantity to SCI_A/B_RBuffer[256]
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	

}
*/
/*** Modbus CMUStatuRead  ***************************/
/*
void CMUStatuRead(quint16 addr,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,indexAddr=0,ii,aa;
	if(addr!=0)			
	{	*Txptr= addr;		//CMU device address
		mi++;				//Mi= SCI Tx buffre index
	}
	else
	{	return;	}								//Mi= SCI Tx buffre index
	*(Txptr+mi)= function;	
	mi++;
	*(Txptr+mi)= rwQuantity*2;

	indexAddr=(rwStartAddr - CMURSTATUADDR);

	table_MODaddr[0]  = &BusVoltage		;
	table_MODaddr[1]  = &TotalIout		;
	table_MODaddr[2]  = &RI_flag.all	;
	table_MODaddr[3]  = &CMUFirwVer		;
	table_MODaddr[4]  =	&PSU_Units		;
	table_MODaddr[5]  =	&PSULinkF[0].all;
	table_MODaddr[6]  =	&PSULinkF[1].all;
	table_MODaddr[7]  =	&ELogMaxN		;
	table_MODaddr[8]  = &DLogMaxN		;
	table_MODaddr[9]  = &ELogIndex		;
	table_MODaddr[10] = &DLogIndex[0]	;
	table_MODaddr[11] =	&DLogIndex[1]	;
	table_MODaddr[12] =	&PType.all		;

	aa=rwQuantity;
	for(ii=indexAddr;ii<13;ii++)
	{
		mi++;
		*(Txptr+mi) =*table_MODaddr[ii]>>8;
		mi++;
		*(Txptr+mi) =(*table_MODaddr[ii]);
		aa--;
		if(aa==0)
		{	ii= 14 ; 		}
	}	
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;
	mi++;
	*(Txptr+mi)= aa>>8;
	
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	
}
*/
/*** Modbus PSUStatuRead  ***************************/
/*
void PSUStatuRead(quint16 addr,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,indexAddr=0,mn=0,ii,aa=0;
	
	if(addr!=0)			
	{	*Txptr= addr;		//CMU device address
		mi++;				//Mi= SCI Tx buffre index
	}
	else
	{	return;	}	

	*(Txptr+mi)= function;	
	mi++;
	*(Txptr+mi)= rwQuantity*2;

	indexAddr= rwStartAddr - PSURSTATUADDR;			
GetPSUStatAddr:	
	if(indexAddr>=14)
	{
		indexAddr -=14		;
		mn++;					// Mn:PSU number
		goto GetPSUStatAddr	;
	}
RedPSUTabStatu:	
	table_MODaddr[0]  = &PMB_U[mn].C19_CAPABILITY	;
	table_MODaddr[1]  = &PMB_U[mn].C79_STS_WORD		;
	table_MODaddr[2]  = &PMB_U[mn].C7A_STS_VOUT		;
	table_MODaddr[3]  = &PMB_U[mn].C7B_STS_IOUT		;
	table_MODaddr[4]  = &PMB_U[mn].C7C_STS_INPUT	;
	table_MODaddr[5]  = &PMB_U[mn].C7D_STS_TEMP		;
	table_MODaddr[6]  = &PMB_U[mn].C81_STS_FAN12	;
	table_MODaddr[7]  = &PMB_U[mn].C88_RD_VIN		;
	table_MODaddr[8]  = &PMB_U[mn].C8B_RD_VOUT		;
	table_MODaddr[9]  = &PMB_U[mn].C8C_RD_IOUT		;
	table_MODaddr[10] = &PMB_U[mn].C8D_RD_TEMP1		;
	table_MODaddr[11] = &PMB_U[mn].C90_RD_FAN_1		;
	table_MODaddr[12] = &PMB_U[mn].C91_RD_FAN_2		;
	table_MODaddr[13] = &PMB_U[mn].C98_PMB_REV		;

	for(ii=indexAddr;ii<14;ii++)
	{	
		mi++;
		*(Txptr+mi) =*table_MODaddr[ii]>>8;		// high byte
		mi++;
		*(Txptr+mi) =(*table_MODaddr[ii]);		// low byte
		aa++;		
		if(aa==rwQuantity)
		{	ii= 15	;	}
	}
	if(aa<rwQuantity)
	{	mn++			;
		indexAddr= 0	;
		goto RedPSUTabStatu	;
	}
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte
	
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	
}
*/
/*** Modbus CMU Data read / write ***************************/
/*
void CMUDataRW(quint16 addr ,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,indexAddr=0,aa,ii,temp;
	
	*Txptr= addr;						//CMU dives address
	mi++;										
	*(Txptr+mi)=function;
	mi++;
	if(function==0x003)
	{	
		*(Txptr+mi)= rwQuantity*2;		
	}
	else if((function==0x006)||(function==0x010))
	{	
		*(Txptr+mi)= rwStartAddr>>8;
		mi++;
		*(Txptr+mi)= (rwStartAddr&0x00FF);
		mi++;
		*(Txptr+mi)= rwQuantity>>8;
		mi++;
		*(Txptr+mi)= (rwQuantity&0x00FF);	
	}
	indexAddr= rwStartAddr - CMURWDATAADDR;
	
	table_MODaddr[0]  = &ProRelay[0]	;
	table_MODaddr[1]  = &ProRelay[1]	;
	table_MODaddr[2]  = &ProRelay[2]	;
	table_MODaddr[3]  = &ProRelay[3]	;
	table_MODaddr[4]  = &DLogIntervalT	;
	table_MODaddr[5]  = &MACaddress[2]	;
	table_MODaddr[6]  = &MACaddress[1]	;
	table_MODaddr[7]  = &MACaddress[0]	;
	table_MODaddr[8]  = &IPaddress[1]	;
	table_MODaddr[9]  = &IPaddress[0]	;		
	table_MODaddr[10] = &SubnetMask[1]	;	
	table_MODaddr[11] = &SubnetMask[0]	;	
	table_MODaddr[12] = &Gateway[1]		;
	table_MODaddr[13] = &Gateway[0]		;
	table_MODaddr[14] = &RTC_YEAR		;
	table_MODaddr[15] = &RTC_MONTH		;
	table_MODaddr[16] = &RTC_DATE		;
	table_MODaddr[17] = &RTC_HOUR		;
	table_MODaddr[18] = &RTC_MIN		;
	table_MODaddr[19] = &RTC_SEC		;
	table_MODaddr[20] = &Password		;
	table_MODaddr[21] = &CMU_Serial[5]	;
	table_MODaddr[22] = &CMU_Serial[4]	;
	table_MODaddr[23] = &CMU_Serial[3]	;
	table_MODaddr[24] = &CMU_Serial[2]	;
	table_MODaddr[25] = &CMU_Serial[1]	;
	table_MODaddr[26] = &CMU_Serial[0]	;
	table_MODaddr[27] = &CMU_Date[2]	;
	table_MODaddr[28] = &CMU_Date[1]	;
	table_MODaddr[29] = &CMU_Date[0]	;
	table_MODaddr[30] = &CMU_Loca[2]	;
	table_MODaddr[31] = &CMU_Loca[1]	;
	table_MODaddr[32] = &CMU_Loca[0]	;
	table_MODaddr[33] = &All_C22_VOUT_TRIM	;
	table_MODaddr[34] = &All_C46_IOUT_OC_FL	;
	table_MODaddr[35] = &ReDelayTim[0]	;
	table_MODaddr[36] = &ReDelayTim[1]	;
	table_MODaddr[37] = &ReDelayTim[2]	;
	table_MODaddr[38] = &ReDelayTim[3]	;
	table_MODaddr[39] = &mobiePhone[0]	;
	table_MODaddr[40] = &mobiePhone[1]	;
	table_MODaddr[41] = &mobiePhone[2]	;
	table_MODaddr[42] = &mobiePhone[3]	;
	table_MODaddr[43] = &mobiePhone[4]	;
	table_MODaddr[44] = &mobiePhone[5]	;
	table_MODaddr[45] = &mobiePhone[6]	;
	table_MODaddr[46] = &mobiePhone[7]	;
	table_MODaddr[47] = &mobiePhone[8]	;
	table_MODaddr[48] = &mobiePhone[9]	;
	if(function==0x003)
	{
		if(addr==0)
		{	return;}

		aa=rwQuantity;
		for(ii=indexAddr;ii<49;ii++)
		{
			mi++;
			*(Txptr+mi) =*table_MODaddr[ii]>>8;
			mi++;
			*(Txptr+mi) =(*table_MODaddr[ii]);
			aa--;
			if(aa==0)
			{	ii= 100 ; 		}
		}		
	}
	else if(function==0x006)
	{
         wriRangEr=0;
	//check rang
		if(indexAddr==33)
		{
			if((PType.all&0x0001)==1)		//12V
			{
				if((rwQuantity>Max_VTrim12v)&&(rwQuantity&0x8000)==0)
				{	  wriRangEr=1;		}					//12V Max rang
				else if(rwQuantity<Min_VTrim12v&&(rwQuantity&0x8000)!=0)
				{	  wriRangEr=1;		}					//12V Min rang
			}
			else if((PType.all&0x0002)==2)	//24V
			{
				if((rwQuantity>Max_VTrim24v)&&(rwQuantity&0x8000)==0)
				{	wriRangEr=1;		}					//24V Max rang
				else if(rwQuantity<Min_VTrim24v&&(rwQuantity&0x8000)!=0)
				{	wriRangEr=1;		}					//24V Min rang				
			}
			else if((PType.all&0x0004)==4)	//48V
			{
				if((rwQuantity>Max_VTrim48v)&&(rwQuantity&0x8000)==0)
				{	wriRangEr=1;		}					//48V Max rang
				else if(rwQuantity<Min_VTrim48v&&(rwQuantity&0x8000)!=0)
				{	wriRangEr=1;		}					//48V Min rang
			}
			else
			{	rwQuantity=0;		}			
		}		
		else if(indexAddr==34)
		{
			if((PType.all&0x0001)==1)			//12V
				{					
					if((rwQuantity&0x07ff)>Max_Iout12v)			//12V Max rang
					{	wriRangEr=1;		}
					else if((rwQuantity&0x07ff)<Min_Iout12v)		// 12V Min rang
					{	wriRangEr=1;		}
				}
				else if((PType.all&0x0002)==2)		//24V
				{
					if((rwQuantity&0x07ff)>Max_Iout24v)			//24V Max rang
					{	wriRangEr=1;		}
					else if((rwQuantity&0x07ff)<Min_Iout24v)		// 24V Min rang
					{	wriRangEr=1;		}				
				}
				else if((PType.all&0x0004)==4)		//48V
				{
					if((rwQuantity&0x07ff)>Max_Iout48v)			//48V Max rang
					{	wriRangEr=1;		}	
					else if((rwQuantity&0x07ff)<Min_Iout48v)		// 48V Min rang
					{	wriRangEr=1;		}					
				}
				else
				{	rwQuantity=0;		}		
		}
		else if(indexAddr==4)
		{
			if(rwQuantity>DLOGINTERTMAX)
			{	wriRangEr=1;	}
			else if(rwQuantity<DLOGINTERTMIN)
			{	wriRangEr=1;	}
		}
		else if(indexAddr==35||indexAddr==36||indexAddr==37||indexAddr==38)
		{
			if(rwQuantity>REDELAYTIMMAX)
			{	wriRangEr=1;	}
			else if(rwQuantity<REDELAYTIMMIN)
			{	wriRangEr=1;	}
		}
	//save value
		if(!wriRangEr)
    	{  *table_MODaddr[indexAddr]=rwQuantity ;	//rwQuantity =value 
    		if(indexAddr==34)
    		{	for(ii=0;ii<=(MAX_PMB_UNIT-1);ii++)
    			{	IOUT_OC_FL[ii]=All_C46_IOUT_OC_FL;		}	
    		}
    		VarModifyF=1;
    		EEindex[0]=0;
    		if(addr==0)
    		{	return;}
        }
		else
		{	return;}
	}
	else if(function==0x010)
	{	aa=0;	
		temp=0;	
		for(ii=indexAddr;ii<49;ii++)
		{	wriRangEr=0;
			temp=(*(Rxptr+aa*2+7))<<8 ;		//rwQuantity =value high byte
			temp|=(*(Rxptr+aa*2+8)) ;		//rwQuantity =value low	byte
		//check rang
			if(ii==33)
			{
				if((PType.all&0x0001)==1)	//12V
				{
					if((temp>Max_VTrim12v)&&(temp&0x8000)==0)
					{	wriRangEr=1;	}					//12V Max rang
					else if(temp<Min_VTrim12v&&(temp&0x8000)!=0)
					{	wriRangEr=1;		}					//12V Min rang
				
                }
				else if((PType.all&0x0002)==2)	//24V
				{
					if((temp>Max_VTrim24v)&&(temp&0x8000)==0)
					{	wriRangEr=1;		}					//24V Max rang
					else if(temp<Min_VTrim24v&&(temp&0x8000)!=0)
					{	wriRangEr=1;		}					//24V Min rang				
				}
				else if((PType.all&0x0004)==4)	//48V
				{
					if((temp>Max_VTrim48v)&&(temp&0x8000)==0)
					{	wriRangEr=1;		}					//48V Max rang
					else if(temp<Min_VTrim48v&&(temp&0x8000)!=0)
					{	wriRangEr=1;		}					//48V Min rang
				}
				else
				{	temp=0;		}				
			}
			else if(ii==34)
			{
				if((PType.all&0x0001)==1)			//12V
				{					
					if((temp&0x07ff)>Max_Iout12v)			//12V Max rang
					{	wriRangEr=1;		}
					else if((temp&0x07ff)<Min_Iout12v)		// 12V Min rang
					{	wriRangEr=1;		}
				}
				else if((PType.all&0x0002)==2)		//24V
				{
					if((temp&0x07ff)>Max_Iout24v)			//24V Max rang
					{	wriRangEr=1;		}
					else if((temp&0x07ff)<Min_Iout24v)		// 24V Min rang
					{	wriRangEr=1;		}				
				}
				else if((PType.all&0x0004)==4)		//48V
				{
					if((temp&0x07ff)>Max_Iout48v)			//48V Max rang
					{	wriRangEr=1;		}	
					else if((temp&0x07ff)<Min_Iout48v)		// 48V Min rang
					{	wriRangEr=1;		}					
				}
				else
				{	temp=0;		}		
			}
			else if(ii==4)
			{
				if(temp>DLOGINTERTMAX)
				{	wriRangEr=1;	}
				else if(temp<DLOGINTERTMIN)
				{	wriRangEr=1;	}
			}
			else if(ii==35||ii==36||ii==37||ii==38)
			{
				if(temp>REDELAYTIMMAX)
				{	wriRangEr=1;	}
				else if(temp<REDELAYTIMMIN)
				{	wriRangEr=1;	}
			}
			//save value
			if(!wriRangEr)
			{    *table_MODaddr[ii]=temp;    }				
			aa++;
			if(aa==rwQuantity)
			{	ii= 100	;	
				VarModifyF=1;
				EEindex[0]=0;
			}	
            	
		}	
		for(ii=0;ii<=(MAX_PMB_UNIT-1);ii++)
		{	IOUT_OC_FL[ii]=All_C46_IOUT_OC_FL;			}		
		VarModifyF=1;
		EEindex[0]=0;
		if(addr==0)
		{	return;}		
	}		
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte	

	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	
}
*/
/*** Modbus PSU Data read / write ***************************/
/*
void PSUDataRW(quint16 addr ,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,mn=0,indexAddr=0,ii,aa,temp;
	
	*Txptr= addr;			//CMU dives address
	mi++;										
	*(Txptr+mi)= function;
	mi++;
	if(function==0x003)
	{	
		*(Txptr+mi)= rwQuantity*2;		
	}
	else if((function==0x006)||(function==0x010))
	{	
		*(Txptr+mi)= rwStartAddr>>8;
		mi++;
		*(Txptr+mi)= (rwStartAddr&0x00FF);
		mi++;
		*(Txptr+mi)= rwQuantity>>8;
		mi++;
		*(Txptr+mi)= (rwQuantity&0x00FF);
	
	}
	indexAddr= rwStartAddr - PSURWDATAADDR;	
				
GetPSUDataAddr:	
	if(indexAddr>=58)
	{
		indexAddr -=58	;
		mn++;									// Mn:PSU number
		goto GetPSUDataAddr	;
	}		
	aa=0;
	if(function==0x003)
	{	
		if(addr==0)
		{	return;}
RWPSUTabData:	
		table_MODaddr[0]  = &PMB_U[mn].C01_OPER			;
		table_MODaddr[1]  = &PMB_U[mn].C02_ONOFF_CFG	;
		table_MODaddr[2]  = &PMB_U[mn].C20_VOUT_MODE	;
		table_MODaddr[3]  = &PMB_U[mn].C21_VOUT_CMD		;
		table_MODaddr[4]  = &PMB_U[mn].C22_VOUT_TRIM	;
		table_MODaddr[5]  = &PMB_U[mn].C46_IOUT_OC_FL	;
		table_MODaddr[6]  = &PMB_U[mn].C47_IOUT_OC_FR	;

		table_MODaddr[7]  = &PMB_U[mn].C99_MFR_ID[12]	;
		table_MODaddr[8]  = &PMB_U[mn].C99_MFR_ID[11]	;
		table_MODaddr[9]  = &PMB_U[mn].C99_MFR_ID[10]	;
		table_MODaddr[10] = &PMB_U[mn].C99_MFR_ID[9]	;
		table_MODaddr[11] = &PMB_U[mn].C99_MFR_ID[8]	;
		table_MODaddr[12] = &PMB_U[mn].C99_MFR_ID[7]	;
		table_MODaddr[13] = &PMB_U[mn].C99_MFR_ID[6]	;
		table_MODaddr[14] = &PMB_U[mn].C99_MFR_ID[5]	;
		table_MODaddr[15] = &PMB_U[mn].C99_MFR_ID[4]	;
		table_MODaddr[16] = &PMB_U[mn].C99_MFR_ID[3]	;
		table_MODaddr[17] = &PMB_U[mn].C99_MFR_ID[2]	;
		table_MODaddr[18] = &PMB_U[mn].C99_MFR_ID[1]	;
	
		table_MODaddr[19] = &PMB_U[mn].C9A_MFR_MDL[12]	;
		table_MODaddr[20] = &PMB_U[mn].C9A_MFR_MDL[11]	;
		table_MODaddr[21] = &PMB_U[mn].C9A_MFR_MDL[10]	;
		table_MODaddr[22] = &PMB_U[mn].C9A_MFR_MDL[9]	;
		table_MODaddr[23] = &PMB_U[mn].C9A_MFR_MDL[8]	;
		table_MODaddr[24] = &PMB_U[mn].C9A_MFR_MDL[7]	;
		table_MODaddr[25] = &PMB_U[mn].C9A_MFR_MDL[6]	;
		table_MODaddr[26] = &PMB_U[mn].C9A_MFR_MDL[5]	;
		table_MODaddr[27] = &PMB_U[mn].C9A_MFR_MDL[4]	;
		table_MODaddr[28] = &PMB_U[mn].C9A_MFR_MDL[3]	;
		table_MODaddr[29] = &PMB_U[mn].C9A_MFR_MDL[2]	;
		table_MODaddr[30] = &PMB_U[mn].C9A_MFR_MDL[1]	;

		table_MODaddr[31] = &PMB_U[mn].C9B_MFR_REV[6]	;
		table_MODaddr[32] = &PMB_U[mn].C9B_MFR_REV[5]	;
		table_MODaddr[33] = &PMB_U[mn].C9B_MFR_REV[4]	;
		table_MODaddr[34] = &PMB_U[mn].C9B_MFR_REV[3]	;
		table_MODaddr[35] = &PMB_U[mn].C9B_MFR_REV[2]	;
		table_MODaddr[36] = &PMB_U[mn].C9B_MFR_REV[1]	;

		table_MODaddr[37] = &PMB_U[mn].C9C_MFR_LOC[3]	;
		table_MODaddr[38] = &PMB_U[mn].C9C_MFR_LOC[2]	;
		table_MODaddr[39] = &PMB_U[mn].C9C_MFR_LOC[1]	;

		table_MODaddr[40] = &PMB_U[mn].C9D_MFR_DATE[6]	;	
		table_MODaddr[41] = &PMB_U[mn].C9D_MFR_DATE[5]	;
		table_MODaddr[42] = &PMB_U[mn].C9D_MFR_DATE[4]	;
		table_MODaddr[43] = &PMB_U[mn].C9D_MFR_DATE[3]	;
		table_MODaddr[44] = &PMB_U[mn].C9D_MFR_DATE[2]	;
		table_MODaddr[45] = &PMB_U[mn].C9D_MFR_DATE[1]	;

		table_MODaddr[46] = &PMB_U[mn].C9E_MFR_SN[12]	;
		table_MODaddr[47] = &PMB_U[mn].C9E_MFR_SN[11]	;	
		table_MODaddr[48] = &PMB_U[mn].C9E_MFR_SN[10]	;
		table_MODaddr[49] = &PMB_U[mn].C9E_MFR_SN[9]	;
		table_MODaddr[50] = &PMB_U[mn].C9E_MFR_SN[8]	;
		table_MODaddr[51] = &PMB_U[mn].C9E_MFR_SN[7]	;
		table_MODaddr[52] = &PMB_U[mn].C9E_MFR_SN[6]	;
		table_MODaddr[53] = &PMB_U[mn].C9E_MFR_SN[5]	;
		table_MODaddr[54] = &PMB_U[mn].C9E_MFR_SN[4]	;
		table_MODaddr[55] = &PMB_U[mn].C9E_MFR_SN[3]	;
		table_MODaddr[56] = &PMB_U[mn].C9E_MFR_SN[2]	;
		table_MODaddr[57] = &PMB_U[mn].C9E_MFR_SN[1]	;		
		for(ii=indexAddr;ii<58;ii++)
		{	mi++;
			*(Txptr+mi) =*table_MODaddr[ii]>>8;
			mi++;
			*(Txptr+mi) =(*table_MODaddr[ii]);			
			aa++;
			if(aa==rwQuantity)
			{	ii= 59	;	}			
		}
		if(aa<rwQuantity)
		{	mn++			;
			indexAddr= 0	;
			goto RWPSUTabData	;
		}			
	}
	else if((function==0x006)||(function==0x010))
	{	
RWPSUTabData1:
		table_MODaddr[0]  = &C01_OPER[mn];
		table_MODaddr[1]  = &nWriteaddr	;		//PMB_U[Mn].C02_ONOFF_CFG	;
		table_MODaddr[2]  = &nWriteaddr	;		//&PMB_U[Mn].C20_VOUT_MODE	;
		table_MODaddr[3]  = &nWriteaddr	;		//&PMB_U[Mn].C21_VOUT_CMD	;
		table_MODaddr[4]  = &All_C22_VOUT_TRIM	;
		table_MODaddr[5]  = &IOUT_OC_FL[mn]	;
		table_MODaddr[6]  = &nWriteaddr	;		//&PMB_U[Mn].C47_IOUT_OC_FR	;

		table_MODaddr[7]  = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[00]	;
		table_MODaddr[8]  = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[01]	;
		table_MODaddr[9]  = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[02]	;
		table_MODaddr[10] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[03]	;
		table_MODaddr[11] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[04]	;
		table_MODaddr[12] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[05]	;
		table_MODaddr[13] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[06]	;
		table_MODaddr[14] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[07]	;
		table_MODaddr[15] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[08]	;
		table_MODaddr[16] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[09]	;
		table_MODaddr[17] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[10]	;
		table_MODaddr[18] = &nWriteaddr	;		//&PMB_U[Mn].C99_MFR_ID[11]	;
	
		table_MODaddr[19] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[00]	;
		table_MODaddr[20] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[01]	;
		table_MODaddr[21] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[02]	;
		table_MODaddr[22] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[03]	;
		table_MODaddr[23] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[04]	;
		table_MODaddr[24] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[05]	;
		table_MODaddr[25] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[06]	;
		table_MODaddr[26] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[07]	;
		table_MODaddr[27] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[08]	;
		table_MODaddr[28] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[09]	;
		table_MODaddr[29] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[10]	;
		table_MODaddr[30] = &nWriteaddr	;		//&PMB_U[Mn].C9A_MFR_MDL[11]	;

		table_MODaddr[31] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[00]	;
		table_MODaddr[32] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[01]	;
		table_MODaddr[33] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[02]	;
		table_MODaddr[34] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[03]	;
		table_MODaddr[35] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[04]	;
		table_MODaddr[36] = &nWriteaddr	;		//&PMB_U[Mn].C9B_MFR_REV[05]	;

		table_MODaddr[37] = &PM9C_Location[2]	;
		table_MODaddr[38] = &PM9C_Location[1]	;
		table_MODaddr[39] = &PM9C_Location[0]	;

		table_MODaddr[40] = &PM9D_Date[5]		;	
		table_MODaddr[41] = &PM9D_Date[4]		;
		table_MODaddr[42] = &PM9D_Date[3]		;
		table_MODaddr[43] = &PM9D_Date[2]		;
		table_MODaddr[44] = &PM9D_Date[1]		;
		table_MODaddr[45] = &PM9D_Date[0]		;

		table_MODaddr[46] = &PM9E_Serial[11]	;
		table_MODaddr[47] = &PM9E_Serial[10]	;	
		table_MODaddr[48] = &PM9E_Serial[9]		;
		table_MODaddr[49] = &PM9E_Serial[8]		;
		table_MODaddr[50] = &PM9E_Serial[7]		;
		table_MODaddr[51] = &PM9E_Serial[6]		;
		table_MODaddr[52] = &PM9E_Serial[5]		;
		table_MODaddr[53] = &PM9E_Serial[4]		;
		table_MODaddr[54] = &PM9E_Serial[3]		;
		table_MODaddr[55] = &PM9E_Serial[2]		;
		table_MODaddr[56] = &PM9E_Serial[1]		;
		table_MODaddr[57] = &PM9E_Serial[0]		;
	
		if(function==0x006)
		{		
		//check rang
			if(indexAddr==4)
			{
				if((PType.all&0x0001)==1)	//12V
				{
					if((rwQuantity>Max_VTrim12v)&&(rwQuantity&0x8000)==0)
					{	wriRangEr=1;		}					//12V Max rang
					else if(rwQuantity<Min_VTrim12v&&(rwQuantity&0x8000)!=0)
					{	wriRangEr=1;		}					//12V Min rang

				}
				else if((PType.all&0x0002)==2)	//24V
				{
					if((rwQuantity>Max_VTrim24v)&&(rwQuantity&0x8000)==0)
					{	wriRangEr=1;		}					//24V Max rang
					else if(rwQuantity<Min_VTrim24v&&(rwQuantity&0x8000)!=0)
					{	wriRangEr=1;		}					//24V Min rang				
				}
				else if((PType.all&0x0004)==4)	//48V
				{
					if((rwQuantity>Max_VTrim48v)&&(rwQuantity&0x8000)==0)
					{	wriRangEr=1;		}					//48V Max rang
					else if(rwQuantity<Min_VTrim48v&&(rwQuantity&0x8000)!=0)
					{	wriRangEr=1;		}					//48V Min rang
				}
				else
				{	rwQuantity=0;		}			
			}	
			else if(indexAddr==5)
			{
				if((PType.all&0x0001)==1)			//12V
				{					
					if((rwQuantity&0x07ff)>Max_Iout12v)			//12V Max rang
					{	wriRangEr=1;		}
					else if((rwQuantity&0x07ff)<Min_Iout12v)		// 12V Min rang
					{	wriRangEr=1;		}
				}
				else if((PType.all&0x0002)==2)		//24V
				{
					if((rwQuantity&0x07ff)>Max_Iout24v)			//24V Max rang
					{	wriRangEr=1;		}
					else if((rwQuantity&0x07ff)<Min_Iout24v)		// 24V Min rang
					{	wriRangEr=1;		}				
				}
				else if((PType.all&0x0004)==4)		//48V
				{
					if((rwQuantity&0x07ff)>Max_Iout48v)			//48V Max rang
					{	wriRangEr=1;		}	
					else if((rwQuantity&0x07ff)<Min_Iout48v)		// 48V Min rang
					{	wriRangEr=1;		}					
				}
				else
				{	rwQuantity=0;		}
			}
			//save value
			if(!wriRangEr)
			{    *table_MODaddr[indexAddr]=rwQuantity ;	//rwQuantity = value 

    			if(indexAddr==00)
    			{	SF_PMBC01=1;
    				PM_Polladdr[1]=0;
    				PMB_STS1=1;		
    				PSUAll_Off=0;			
    			}			
    			else if((indexAddr>=37)&&(indexAddr<=39))
    			{	SF_PMBC9C=1;
    				PM_Polladdr[1]=0;
    				PMB_STS1=1;		
    			}
    			else if((indexAddr>=40)&&(indexAddr<=45))
    			{	SF_PMBC9D=1;
    				PM_Polladdr[1]=0;
    				PMB_STS1=1;	
    			}
    			else if((indexAddr>=46)&&(indexAddr<=57))
    			{	SF_PMBC9E=1;
    				PM_Polladdr[1]=0;
    				PMB_STS1=1;
    			}
    			else
    			{	VarModifyF=1;
    				EEindex[0]=0;
    			}			
    			if(addr==0)
    			{	return;}
            }
			else
			{	return;}
		}
		else if(function==0x010)
		{	aa=0;		
			for(ii=indexAddr;ii<58;ii++)
			{	
				temp=(*(Rxptr+aa*2+7))<<8 ;		//rwQuantity =value high byte
				temp|=(*(Rxptr+aa*2+8)) ;		//rwQuantity =value low	byte
			//check rang
				if(ii==4)
				{
					if((PType.all&0x0001)==1)	//12V
					{
						if((temp>Max_VTrim12v)&&(temp&0x8000)==0)
						{	wriRangEr=1;		}					//12V Max rang
						else if(temp<Min_VTrim12v&&(temp&0x8000)!=0)
						{	wriRangEr=1;		}					//12V Min rang

					}
					else if((PType.all&0x0002)==2)	//24V
					{
						if((temp>Max_VTrim24v)&&(temp&0x8000)==0)
						{	wriRangEr=1;		}					//24V Max rang
						else if(temp<Min_VTrim24v&&(temp&0x8000)!=0)
						{	wriRangEr=1;		}					//24V Min rang				
					}
					else if((PType.all&0x0004)==4)	//48V
					{
						if((temp>Max_VTrim48v)&&(temp&0x8000)==0)
						{	wriRangEr=1;		}					//48V Max rang
						else if(temp<Min_VTrim48v&&(temp&0x8000)!=0)
						{	wriRangEr=1;		}					//48V Min rang
					}
					else
					{	temp=0;		}				
				}
				if(ii==5)
				{
					if((PType.all&0x0001)==1)			//12V
					{					
						if((temp&0x07ff)>Max_Iout12v)			//12V Max rang
						{	wriRangEr=1;		}
						else if((temp&0x07ff)<Min_Iout12v)		// 12V Min rang
						{	wriRangEr=1;		}
					}
					else if((PType.all&0x0002)==2)		//24V
					{
						if((temp&0x07ff)>Max_Iout24v)			//24V Max rang
						{	wriRangEr=1;		}
						else if((temp&0x07ff)<Min_Iout24v)		// 24V Min rang
						{	wriRangEr=1;		}				
					}
					else if((PType.all&0x0004)==4)		//48V
					{
						if((temp&0x07ff)>Max_Iout48v)			//48V Max rang
						{	wriRangEr=1;		}	
						else if((temp&0x07ff)<Min_Iout48v)		// 48V Min rang
						{	wriRangEr=1;		}					
					}
					else
					{	temp=0;		}		
				}				
			//save value
				if(!wriRangEr)
				{   *table_MODaddr[ii]=temp;
				
    				if(ii==00)
    				{	SF_PMBC01=1;
    					PM_Polladdr[1]=0;
    					PMB_STS1=1;	
    					PSUAll_Off=0;
    				}				
    				if((ii>=37)&&(ii<=39))
    				{	SF_PMBC9C=1;
    					PM_Polladdr[1]=0;
    					PMB_STS1=1;		
    				}
    				if((ii>=40)&&(ii<=45))
    				{	SF_PMBC9D=1;
    					PM_Polladdr[1]=0;
    					PMB_STS1=1;	
    				}
    				if((ii>=46)&&(ii<=57))
    				{	SF_PMBC9E=1;
    					PM_Polladdr[1]=0;
    					PMB_STS1=1;
    				}			
                }  								
  				aa++;
  				if(aa==rwQuantity)
  				{	ii= 59	;	
  					VarModifyF=1;
  					EEindex[0]=0;
  				}
  				mi++;
  			}
  			if(aa<rwQuantity)
  			{	mn++			;
  				indexAddr= 0	;
  				goto RWPSUTabData1	;
  			}
  			if(addr==0)
  			{	return;}
		}	
	}
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte
						
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	
}
*/
/*** Modbus CMU control  ***************************/
/*
void CMUControl(quint16 addr ,quint16 function ,quint16 rwStartAddr ,quint16 value)
{	quint16 mi=0,indexAddr=0,ii,aa;
	
	*Txptr= addr;			//CMU dives address
	mi++;										
	*(Txptr+mi)= function;
	mi++;
	*(Txptr+mi)= rwStartAddr>>8;
	mi++;
	*(Txptr+mi)= (rwStartAddr&0x00FF);
	mi++;
	*(Txptr+mi)= value>>8;
	mi++;
	*(Txptr+mi)= (value&0x00FF);	

	indexAddr= rwStartAddr - CMUCONTRADR;	
	switch(indexAddr)
	{	case 0:
			if(value==0xFF00)	// off all PSU
			{			
				for(ii=0;ii<MAX_PMB_UNIT;ii++)
				{	C01_OPER[ii]= 0x00;		}				
				SF_PMBC01=1;;
				PM_Polladdr[1]=0;
				PMB_STS1=1;	
				PSUAll_Off=1;
			}
			else if(value==0x0000)
			{	
				for(ii=0;ii<MAX_PMB_UNIT;ii++)
				{	C01_OPER[ii]= 0x80;		}				
				SF_PMBC01=1;;
				PM_Polladdr[1]=0;
				PMB_STS1=1;	
				PSUAll_Off=0;	
			}
			break;
		case 1:
			if(value==0xFF00)		//clear event log
			{	F_C_EvenLog=1;	}	//clear Event log eeprom flag setting
			
			else if(value==0x0000)
			{	F_C_EvenLog=0;		}
			break;
		case 2:
			if(value==0xFF00)			//clear data log
			{	F_C_DataLog=1;		//clear data log eeprom flag setting
				EEindex[2]=0;
				main_temp[1]=0;		
			}
			else if(value==0x0000)
			{	F_C_DataLog=0;		}
			break;
		case 3:
			if(value==0xFF00)		//load defaule
			{	
				F_LoDefault=1;		
			}
			else if(value==0x0000)
			{	F_LoDefault=0;		}
			break;	
		default:
			ModbusEr(addr,function,2);		 //ExceptionCode = 02
			break;		
	}		
	if(addr==0)
	{	return;}
	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte
						
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
	}	
}
*/
/***  Modbus CMU Event Log read *****************************/
/*
void CMUEventLog(quint16 addr,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,indexAddr=0,aa,ii,temp=0;
    if(addr!=0)
	{	*Txptr= addr;				//CMU device address
		mi++;						//Mi= SCI Tx buffre index
	}
	else
	{	return;}
	
	*(Txptr+mi)= function;	
	mi++;
	*(Txptr+mi)= rwQuantity*ELOGBYTEQ;
	indexAddr= rwStartAddr - CMUREVENTADDR;		
	aa=0;
redCMUEvent:		
	for(ii=0;ii<ELOGBYTEQ;ii+=2)
	{	
		cc=	(EE_EvenLog+((indexAddr+aa)*16)+ii);
		temp= EEP_readW(addr_AT24512_0,(EE_EvenLog+((indexAddr+aa)*ELOGBYTEQ)+ii));
		mi++;
		*(Txptr+mi)=temp>>8;
		mi++;
		*(Txptr+mi)=(temp&0x00ff);	
		temp=0;
	}	
	aa++;		
	if(aa<rwQuantity)
	{	goto redCMUEvent;	}

	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte
	
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
    }
}
*/
/***  Modbus CMU Data Log read *****************************/
/*
void CMUDataLog(quint16 addr ,quint16 function ,quint16 rwStartAddr ,quint16 rwQuantity)
{	quint16 mi=0,indexAddr=0,aa,ii,temp=0;

	if(addr!=0)
	{	*Txptr= addr;				//CMU device address
		mi++;						//Mi= SCI Tx buffre index
	}
	else
	{	return;}
	*(Txptr+mi)= function;	
	mi++;
	*(Txptr+mi)= rwQuantity*DLOGBYTEQ;
	if((rwStartAddr<(CMURDATAADDR+576))&&((rwStartAddr+rwQuantity)<(CMURDATAADDR+576)))
	{	indexAddr= rwStartAddr - CMURDATAADDR;}	
	else
	{	indexAddr= (rwStartAddr - CMURDATAADDR)-(EE_DLog1H/DLOGBYTEQ);}	
	aa=0;
redCMUData:		
	for(ii=0;ii<DLOGBYTEQ;ii+=2)
	{	
		if((rwStartAddr<(CMURDATAADDR+576))&&((rwStartAddr+rwQuantity)<(CMURDATAADDR+576)))
		{	temp= EEP_readW(addr_AT24512_0,(EE_DataLog+((indexAddr+aa)*DLOGBYTEQ)+ii));}
		else
		{	temp= EEP_readW(addr_AT24512_1,(EE_DataLog2+((indexAddr+aa)*DLOGBYTEQ)+ii));}
		mi++;
		*(Txptr+mi)=temp>>8;
		mi++;
		*(Txptr+mi)=(temp&0x00ff);	
		temp=0;
	}	
	aa++;		
	if(aa<rwQuantity)
	{	goto redCMUData;	}

	aa=	CRC16(mi,Txptr);
	mi++;
	*(Txptr+mi)= (aa&0x00FF) ;		//crc low byte
	mi++;
	*(Txptr+mi)= aa>>8;				//crc high byte
	
	if(Txptr==&SCI_A_Tbuf[0])
	{	SCIA_Tx=1;
	 	TxAbyteCnt[0]=0;
		TxAbyteCnt[1]=mi;		 	//save byte Quantity 
	}
	else if(Txptr==&SCI_B_Tbuf[0])
	{	SCIB_Tx=1;
		TxBbyteCnt[0]=0;
		TxBbyteCnt[1]=mi;		 	//save byte Quantity 
    }
}
*/
/*** Modbus  RX data Processing *****************************/
/*
void ModbusRxProcess(void)
{	quint16 function=0,starAddr=0,quantity=0,value,addr;
	if(((*Rxptr&0x00ff)==CMUaddr)||((*Rxptr&0x00ff)==0)||((*Rxptr&0x00ff)==247))
	{	addr=*Rxptr;
		function=*(Rxptr+1);	
		
		if((function==3)||(function==5)||(function==6)||(function==16)||(function==100))
		{	starAddr = *(Rxptr+2)<<8;
			starAddr |= *(Rxptr+3);

			if((starAddr>=CMURSTATUADDR)&&(starAddr<CMURSTATUEND))	//CMU status read rang  (only function 3)
			{
				if(function==3)//function 3
				{	quantity = *(Rxptr+4)<<8;
					quantity |= *(Rxptr+5);				
					if((quantity>125)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((starAddr+quantity)>CMURSTATUEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}	
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}	
					CMUStatuRead(addr,function,starAddr,quantity);								
				}
				else //error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);		 
					goto ModbusProcessDone;
				}	
			}
			else if((starAddr>=PSURSTATUADDR)&&(starAddr<PSURSTATUEND))//PSU status read rang (only function 3)
			{
				if(function==3)		//function 3
				{	quantity = *(Rxptr+4)<<8;
					quantity |= *(Rxptr+5);				
					if((quantity>125)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((starAddr+quantity)>PSURSTATUEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}		
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}	
					PSUStatuRead(addr,function,starAddr,quantity);								
				}
				else //error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);		
					goto ModbusProcessDone;
				}			
			}
			else if((starAddr>=CMURWDATAADDR)&&(starAddr<CMURWDATAEND))//CMU data read/write rang 
			{
					
				if(function==3)//function 3
				{	
					quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);		
					if((quantity>125)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((starAddr+quantity)>CMURWDATAEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}
					CMUDataRW(addr,function,starAddr,quantity);								
				}
				else if(function==6)
				{	
					value = *(Rxptr+4)<<8;
					value |=((*(Rxptr+5))&0x00FF);								
					CMUDataRW(addr,function,starAddr,value);	
				}
				else if(function==16)
				{	
					quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);
					if((quantity>123)||(quantity==0))	//quantity!=(*(Rxptr+6))*2)||
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}				
					if((starAddr+quantity)>CMURWDATAEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}
					CMUDataRW(addr,function,starAddr,quantity);
				}	
				else //error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);		
					goto ModbusProcessDone;
				}			
			}
			else if((starAddr>=PSURWDATAADDR)&&(starAddr<PSURWDATAEND))//PSU data read/write rang
			{
				if(function==3)//function 3
				{	
					quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);			
					if((quantity>125)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((quantity+starAddr)>PSURWDATAEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}	
					PSUDataRW(addr,function,starAddr,quantity);								
				}
				else if(function==6)
				{						
					value = *(Rxptr+4)<<8;
					value |= ((*(Rxptr+5))&0x00FF);		
				  	PSUDataRW(addr,function,starAddr,value);	
				}
				else if(function==16)
				{		
					quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);		
					if((quantity>123)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}				
					if((starAddr+quantity)>PSURWDATAEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}
					PSUDataRW(addr,function,starAddr,quantity);
				}	
				else 	//error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);	
					goto ModbusProcessDone;
				}		
			}	
			else if((starAddr>=CMUCONTRADR)&&(starAddr<CMUCONTREND))//PSU data read/write rang
			{
				if(function==5)
				{
					value = *(Rxptr+4)<<8;
					value |= ((*(Rxptr+5))&0x00FF);		
				  	CMUControl(addr,function,starAddr,value);				
				}
				else 	//error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);	
					goto ModbusProcessDone;
				}
			}	
			else if((starAddr>=CMUREVENTADDR)&&(starAddr<CMUREVENTEND))
			{	
				if(function==100)
				{	quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);			
					if((quantity>15)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((quantity+starAddr)>CMUREVENTEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}	
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}	
			//		if(quantity>ELogMaxN)
			//		{	ModbusEr(function,2);		 //ExceptionCode = 02
			//			goto ModbusProcessDone;
			//		}	
			//		if(starAddr>(CMUREVENTADDR+(ELogMaxN)))
			//		{	ModbusEr(function,2);		 //ExceptionCode = 02
			//			goto ModbusProcessDone;
			//		}	
			//		if((quantity+starAddr)>(CMUREVENTADDR+(ELogMaxN)))
			//		{	ModbusEr(function,2);		 //ExceptionCode = 02
			//			goto ModbusProcessDone;
			//		}					
				  	CMUEventLog(addr,function,starAddr,quantity);				
				}
				else 	//error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);	
					goto ModbusProcessDone;
				}
			}
			else if((starAddr>=CMURDATAADDR)&&(starAddr<CMURDATAEND))
			{
				if(function==100)
				{	quantity = *(Rxptr+4)<<8;
					quantity |= ((*(Rxptr+5))&0x00FF);			
					if((quantity>2)||(quantity==0))
					{	ModbusEr(addr,function,3);		 //ExceptionCode = 03
						goto ModbusProcessDone;
					}
					if((quantity+starAddr)>CMURDATAEND)
					{	ModbusEr(addr,function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}
					if((*Rxptr&0x00ff)==0)				
					{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
						goto ModbusProcessDone;
					}		
		/*			if(quantity>ELogMaxN)
					{	ModbusEr(function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}	
					if(starAddr>(CMUREVENTADDR+(ELogMaxN)))
					{	ModbusEr(function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
					}	
					if((quantity+starAddr)>(CMUREVENTADDR+(ELogMaxN)))
					{	ModbusEr(function,2);		 //ExceptionCode = 02
						goto ModbusProcessDone;
                    }
					if(F_C_DataLog==0)		
				  	CMUDataLog(addr,function,starAddr,quantity);				
				}
				else 	//error function code ExceptionCode = 01
				{	ModbusEr(addr,function,1);	
					goto ModbusProcessDone;
				}
			}
			else	////error address ExceptionCode = 02
			{
				ModbusEr(addr,function,2);		 //ExceptionCode = 02
				goto ModbusProcessDone;
			}
		}
		else //error function code ExceptionCode = 01
		{	ModbusEr(addr,function,1);		 //ExceptionCode = 01
			goto ModbusProcessDone;
		}
	}
ModbusProcessDone:
	return;
}
*/
/*** SCI_comm  ***************************/
/*
void SCI_comm(void)
{
	if(SCIA_Rx)	// || SCIA_Tx 
    {	quint16 CRC=0,aa;									//switch to SCI-A

		Rxptr=&SCI_A_Rbuf[0]	;
		Txptr=&SCI_A_Tbuf[0]	;
		if(F_USB||F_RS232)
		{	if(SCI_A_tail>7)
			{	CRC= *(Rxptr+SCI_A_tail-1)<<8;
				CRC|=(*(Rxptr+SCI_A_tail-2)&0x00FF);
				aa=CRC16((SCI_A_tail-3),Rxptr);
			}
			else
			{	aa=00;}
			if(aa!=CRC)
			{	aa=*(Rxptr+1);
				ModbusEr(*Rxptr,aa,2);		 //ExceptionCode = 02
			} 						
			else 
			{	ModbusRxProcess();		}	
			SCIA_Rx=0;	
			SCI_A_tail=0;		
		}
		else if(F_RS23_GSM)
        {	quint16	ii;
			Rxptr=&SCI_A_Rbuf[0];
			
			for(ii=0;ii<SCI_A_tail;ii++)
			{	gsmMessagBuf[ii]=*(Rxptr+ii);}		
			for(ii=0;ii<50;ii++)
			{	*(Rxptr+ii)=0;		}	
			readGsmMessag=1;
			SCIA_Rx=0;	
			SCI_A_tail=0;	
			readGsmMessag=1;	
		}
	}
	if(SCIB_Rx)	//||SCIB_Tx
	{										//switch to SCI-B
	//	TXBUF_ptr=&ScibRegs.SCITXBUF;
	//	FFTX_ptr=&ScibRegs.SCIFFTX.all;
		Rxptr=&SCI_B_Rbuf[0]	;
		Txptr=&SCI_B_Tbuf[0]	;

		if(SCIB_Rx)
        {	quint16 CRC=0,aa;
			if(SCI_B_tail>7)
			{	CRC= *(Rxptr+SCI_B_tail-1)<<8;
				CRC|=(*(Rxptr+SCI_B_tail-2)&0x00FF);
				aa=CRC16((SCI_B_tail-3),Rxptr);
			}
			else
			{	aa=00;}
			if(aa!=CRC)
			{	aa=*(Rxptr+1);
				ModbusEr(*Rxptr,aa,2);	
			
			}	 //ExceptionCode = 02	
			else
			{	ModbusRxProcess();	}
			SCIB_Rx=0;
			SCI_B_tail=0;	
		}
	//	if(SCIB_Tx)
	//	{	ModbusTxAProcess();		
	//	}
	}	
}	
*/
/*** MODBUS communication ***************************/
/*
void modbusComm(void)
{
	//	PT_48V=0;PT_24V=0;PT_12V=1;	//test
		if (SciaRegs.SCIRXST.bit.RXERROR)			//reset SCI
		{	
            quint16	ii;

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
			
			SCI_A_tail=0;	
			TxAbyteCnt[0]=0;
			TxAbyteCnt[1]=0;	
			for(ii=0;ii<255;ii++)
			{	SCI_A_Rbuf[ii]=0;	
				SCI_A_Tbuf[ii]=0;
			}			
		//	SCI_B_tail=0;
		//	TxBbyteCnt[0]=0;
		//	TxBbyteCnt[1]=0;		
		//	for(ii=0;ii<255;ii++)
		//	{	SCI_B_Rbuf[ii]=0;	
		//		SCI_B_Tbuf[ii]=0;
		//	}
		//	*Rxptr=0;
		//	*Txptr=0;
		}
		else if((SCI_A_Rtout==10)&&(SCIA_Rx&&SCIA_Tx==0))		//Rx is finish if SCI_A Rx time out  
		{	SCI_comm();		}
		
	
		if((SCI_B_Rtout==2)&&(SCIB_Rx&&SCIB_Tx==0))	//Rx is finish if SCI_A Rx time out 
		{	SCI_comm();		}	
}
*/











