#include <stdio.h>
#include "platform.h"
#include "PmodRTCC.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xuartlite_l.h"
#include "EEPROM.h"
#include "define_Epprom.h"
//#include "xiicps.h"
#include "xstatus.h"



#define BaseAddress1 0x40600000				// BaseAddress UART
#define RegOffsetRX XUL_RX_FIFO_OFFSET
#define RegOffsetTX XUL_TX_FIFO_OFFSET

#define netflow_baseaddr 0x78E00000
#define Reg1 0x01
#define Reg2 0x02
#define Reg3 0x03
#define Reg4 0x04
#define Reg5 0x05
#define Reg6 0x06
#define Reg7 0x07
#define Reg8 0x08
#define Reg_empty 0x09
#define timestamps_add 	0x0A
#define active 			0x0B
#define in_active 		0x0C
#define read_fifo		0x0D
#define IIC_DEVICE_ID XPAR_IIC_EEPROM_DEVICE_ID
#define IIC_SCLK_RATE	100000
#define PAGE_SIZE	1
#define IIC_MUX_ADDRESS	0x74
typedef u8 AddressType;
int IicPsEeprom_RD(u8 EEPROM_START_ADDRESS, u8 iic_slaver_address);
int EepromReadData(u8 *BufferPtr, u16 ByteCount, u8 EEPROM_START_ADDRESS, u8 iic_slaver_address);
int EepromWriteData(u16 ByteCount, u8 iic_slaver_address);


u8 WriteBuffer[sizeof(AddressType) + PAGE_SIZE];
u8 ReadBuffer[PAGE_SIZE];
u32 loctime, loctimeOld;

typedef struct timezone
{
	int second;
	int minute;
	int hour;
	int ampm;
	int day;
	int date;
	int month;
	int year;
	int	tm_isdst;
}timezone ;
PmodRTCC myDevice;
//EEPROM24C myDevice_EEPROM;
typedef struct RTCC_Time {
   u8 second;
   u8 minute;
   u8 hour;
   u8 ampm;
   u8 day;
   u8 date;
   u8 month;
   u8 year;
} RTCC_Time;
EEPROM24C EEPROM_IIC;
typedef struct OUT{
	u8 Active;
	u8 Active1;
	u8 inActive;
	u8 inActive1;
}OUT;
void DemoInitialize(u8 mode);
void PrintTime(RTCC_Target src);
RTCC_Time GetTime(PmodRTCC *InstancePtr, RTCC_Target src);
void SetTime(PmodRTCC *InstancePtr, RTCC_Target dest, RTCC_Time val);
RTCC_Time IncrementTime(RTCC_Time time, int delta_seconds);
void delay_1ms();
void EEPROM_ReadIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData);
void EEPROM_WriteIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData);
void reset_my_eeprom();
void IicPsEeprom_WR(u8 EEPROM_START_ADDRESS, u8 Data, u8 iic_slaver_address);
void usleep(unsigned long useconds);

u8 bcd2int(u8 data);
u8 int2bcd(u8 data) ;
void print(char *str);
void DemoCleanup();
void EnableCaches();
void DisableCaches();
void Readtime();
void Mode_Start();
void SetOnly();
void Yes_No();
void Set_Active();
void PrintTimeLite();
void Read_data_flow();
void Time_Lite();


int main()
{
    init_platform();
    EnableCaches();
    RTCC_begin(&myDevice, XPAR_IIC_0_BASEADDR, 0x68);
    EEPROM_begin(&EEPROM_IIC, XPAR_IIC_0_BASEADDR, 0x54);
    xil_printf("\r\n*********************************************** \r\n");
    xil_printf("**              MAIN BOARD SP605             ** \r\n");
    xil_printf("***********************************************	\r\n ");
    //Time_Lite();
    Mode_Start();
    while(1){}
    return 0;
}
//======================================================================
//	SetOnly: Chi thuc hien set theo gia tri defaut cho san( yeu cau 2030)
//======================================================================
void SetOnly() {
		Yes_No();
	    RTCC_Time time;
	    time.second = 0x30;
	    time.minute = 0x30;
	    time.hour   = 0x01;
	    time.ampm   = ~RTCC_PM;
	    time.day    = 0x01;
	    time.date   = 0x01;
	    time.month  = 0x01;
	    time.year   = 0x30;
	    SetTime(&myDevice, RTCC_TARGET_RTCC, time);
}
void Mode_Start(){
    xil_printf("\r\n CHON CAC MODE CO SAN:\r\n");
    xil_printf("\t 1: DIEU CHINH THOI GIAN. \r\n");
    xil_printf("\t 2: CAI DAT THOI GIAN MAC DINH. \r\n");
    xil_printf("\t 3: HIEN THI TIMESTAMP VA REAL TIME. \r\n");
    xil_printf("\t 4: SET ACTIVE VA INACTIVE_TIMEOUT. \r\n");
    xil_printf("\t 5: XUAT RA TIME & FLOW ENTRY. \r\n");
    u8 Mode;
    while(1){
    xil_printf("MOI NHAP MODE: \r\n");
    	Mode =XUartLite_RecvByte(BaseAddress1);
    	    if (Mode == '1'){
    	    	Readtime();
    	    	xil_printf("SETTING TIME: \t");
    	    	PrintTime(RTCC_TARGET_RTCC);
    	    	Mode_Start();}
    	    	else if(Mode == '2'){
    	    		SetOnly();
    	    		xil_printf("TIME DA SET: \t");
    	    		PrintTime(RTCC_TARGET_RTCC);
    	    		xil_printf("\r\n\r\n");
    	    		Mode_Start();
    	    	}
    	    	else if (Mode == '3'){
    	    		while(1)
    	    		{
    	    	    PrintTime(RTCC_TARGET_RTCC);
    	    	    PrintTimeLite();
    	    	    delay_1ms();
    	    		}
    	    		}
    	    	else if (Mode =='4'){
    	    		Set_Active();
    	    		xil_printf("\r\n\r\n");
    	    		Mode_Start();
    	    		}
    	    	else if(Mode == '5'){
    	    		while(1){
    	    			PrintTimeLite();
    	    		}}
    	    	else
    	    	{
    	    	xil_printf("\r\nMode khong ton tai. moi nhap lai mode \r\n");
    	    	}
    }
}
//====================================================
//	Ham hoi yes, no truoc khi thuc hien write, read
//====================================================

void Yes_No(){
	u8 YN;
	xil_printf("\r\nNhap Y de xac nhan. N de huy va quay lai mode \r\n");
	YN=XUartLite_RecvByte(BaseAddress1);
	while(1){
		YN=XUartLite_RecvByte(BaseAddress1);
		if(YN=='N'||YN=='n'){
				Mode_Start();
			}
		if (YN=='Y'||YN=='y'){
			break;
		}
	}


}
//=====================================================================
//	Ham nay de set 2 gia tri Active va inActive. tuy nhien su dung 2
//	thanh ghi de gia tri co the dat duoc toi da 65500 giay.Su dung chia
//	lay nguyen de lay 8bit cao, chia lay du de lay 8bit thap. Khai
//	bao bien su dung 32bit de kiem tra. neu gia tri ngoai 16bit(> 65535)
//	hoac < 0 thi quay lai va yeu cau nhap lai
//=====================================================================
void Set_Active(){
	u32 dataLiteout1,dataLiteout2;
	OUT time;
	u32 time_bf,time_next;
	u32 time_bf1,time_next1;
	u8 Second_Active,Second_Active1,Second_inActive,Second_inActive1;
	xil_printf("\r\nNhap gia tri Active time(second)  : ");
	time_bf  =(XUartLite_RecvByte(BaseAddress1) - '0')*10000;
	time_bf +=(XUartLite_RecvByte(BaseAddress1) - '0')*1000;
	time_bf +=(XUartLite_RecvByte(BaseAddress1) - '0')*100;
	time_bf +=(XUartLite_RecvByte(BaseAddress1) - '0')*10;
	time_bf +=(XUartLite_RecvByte(BaseAddress1) - '0');
	if((time_bf<0) || (time_bf >65535)){
		xil_printf("\r\nGia tri %d khong hop le  ",time_bf);
		Set_Active ();
	}
	xil_printf("%d", time_bf);
	time.Active = (time_bf % 256);
	time.Active1 =(time_bf /256);
	xil_printf("\r\nNhap gia tri inActive time(second): ");
	time_bf1 =(XUartLite_RecvByte(BaseAddress1) - '0')*10000;
	time_bf1 +=(XUartLite_RecvByte(BaseAddress1) - '0')*1000;
	time_bf1 +=(XUartLite_RecvByte(BaseAddress1) - '0')*100;
	time_bf1 +=(XUartLite_RecvByte(BaseAddress1) - '0')*10;
	time_bf1 +=(XUartLite_RecvByte(BaseAddress1) - '0');
	if((time_bf1<0) || (time_bf1 >65535)){
			xil_printf("\r\nGia tri %d khong hop le  ",time_bf1);
			Set_Active ();
		}
	xil_printf("%d", time_bf1);
	time.inActive = (time_bf1 % 256);
	time.inActive1 =(time_bf1 / 256);

	Yes_No();	// xac thuc yeu cau
	//	Su dung ham write va read. nap 2 gia tri vao 2 thanh ghi
	//	Sau do read 4 thanh ghi tu dia chi vua luu vao EEPROM de kiem tra
	//	da luu va doc tu EEPROM thanh cong.
	EEPROM_WriteIIC(&EEPROM_IIC, 0x00, &time.Active,1);
	delay_1ms();
	EEPROM_WriteIIC(&EEPROM_IIC, 0x01, &time.Active1,1);
	delay_1ms();
	EEPROM_WriteIIC(&EEPROM_IIC, 0x20, &time.inActive,1);
	delay_1ms();
	EEPROM_WriteIIC(&EEPROM_IIC, 0x21, &time.inActive1,1);
	delay_1ms();

	EEPROM_ReadIIC(&EEPROM_IIC, 0x00, &Second_Active, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x01, &Second_Active1, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x20, &Second_inActive, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x21, &Second_inActive1, 1);
	delay_1ms();
	time_next = (Second_Active1*256 + Second_Active);
	time_next1 = (Second_inActive1*256 + Second_inActive);
	xil_printf("\r\nActive time   : %5d",time_next);
	xil_printf("\r\ninActive time : %5d",time_next1);
}
//=====================================================================
//	Time_Lite. Ham nay doc ra cac gia tri Active va inActive tu EEPROM va
//	chuyen ve he decemal 16 bit va thuc hien ghi vao IPcore NF. va thuc hien
//	read ra de kiem tra viec thuc hien write, read da thanh cong.
//=====================================================================
void Time_Lite(){
	u16 dataLite1,dataLite2;
	u32 dataLiteout1,dataLiteout2;
	u8 Second_Active,Second_Active1,Second_inActive,Second_inActive1;
	EEPROM_ReadIIC(&EEPROM_IIC, 0x00, &Second_Active, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x01, &Second_Active1, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x20, &Second_inActive, 1);
	delay_1ms();
	EEPROM_ReadIIC(&EEPROM_IIC, 0x21, &Second_inActive1, 1);
	delay_1ms();
	dataLite1 = (Second_Active1*256 + Second_Active);
	dataLite2 = (Second_inActive1*256 + Second_inActive);

/*	LITE4_mWriteReg(netflow_baseaddr, active, dataLite1);
	dataLiteout1=LITE4_mReadReg(netflow_baseaddr, active);
	LITE4_mWriteReg(netflow_baseaddr, in_active, dataLite2);
	dataLiteout2=LITE4_mReadReg(netflow_baseaddr,in_active);*/

	xil_printf("\r\n Active: %d  In_Active: %d \t",dataLiteout1, dataLiteout2);
}
//=====================================================================
// Readtime: Ham nay cho phep nguoi dung nhap gia tri thoi gian tu
// ban phim(UART)
//=====================================================================
void Readtime(){
	RTCC_Time time;
	int temp=0;
	u8 Dtime;
	{
		temp=1;
		XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
		while(temp>0){
		switch(temp) {
			case 1:	xil_printf("\r\n Nhap nam theo dinh dang 'YY':");
					temp=2;
			case 2:
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						//xil_printf ("%d,Dtime");
						if( bcd2int(Dtime)>0){
							time.year= Dtime;
							temp =3;
							xil_printf(" %x",Dtime);
							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n Nam %x ko hop le",Dtime);
						}
			}
			case 3:	xil_printf("\r\n Nhap thang theo dinh dang 'MM':");
					temp=4;
			case 4: //////////////////// MONTH////////////////////
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						if( bcd2int(Dtime)>0 && bcd2int(Dtime)<=12){
							time.month= Dtime;
							temp =5;
							xil_printf(" %x",Dtime);

							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n Thang %x ko hop le",Dtime);
						}
				}
			case 5:	xil_printf("\r\n Nhap ngay theo dinh dang 'DD':");
					temp=6;
			case 6://////////////////// DAY////////////////////
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						if( bcd2int(Dtime)>0 && bcd2int(Dtime)<=31){
							time.date= Dtime;
							temp =7;
							xil_printf(" %x",Dtime);
							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n Ngay %x ko hop le ",Dtime);
						}
				}
			case 7:	xil_printf("\r\n Nhap gio theo dinh dang 'HH':");
					temp=8;
			case 8://////////////////// HOUR////////////////////
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						if( bcd2int(Dtime)>0 && bcd2int(Dtime)<=24){
							time.hour= Dtime;
							temp =9;
							xil_printf("\r %x",Dtime);
							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n Gio %x ko hop le",Dtime);
						}
				}
			case 9:	xil_printf("\r\n Nhap phut theo dinh dang 'MM':");
					temp=10;
			case 10://////////////////// MINUTE////////////////////
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						if(bcd2int(Dtime)>0 && bcd2int(Dtime)<=60){
							time.minute= Dtime;
							temp =11;
							xil_printf(" %x",Dtime);
							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n phut %x ko hop le",Dtime);
						}
				}
			case 11:	xil_printf("\r\n Nhap giay theo dinh dang 'SS':");
						temp=12;
			case 12://////////////////// SECOND////////////////////
				while(1){
					Dtime=0;
						Dtime = (XUartLite_RecvByte(BaseAddress1)-'0')*16;
						Dtime +=XUartLite_RecvByte(BaseAddress1)-'0';
						if( bcd2int(Dtime)>=0 && bcd2int(Dtime)<=60){
							time.second= Dtime;
							temp =0;
							xil_printf(" %x",Dtime);
							XUartLite_ReadReg(BaseAddress1, RegOffsetRX);
							break;
						}
						else {
							xil_printf("\r\n giay %x ko hop le",Dtime);
						}
				}
		 break;
		}
	xil_printf("\r\n Data write %x/%x/%x - %x:%x:%x \r\n",time.year,time.month,time.date,time.hour,time.minute,time.second);
	break;
	}
		Yes_No();
		SetTime(&myDevice,RTCC_TARGET_RTCC,time);
	}
}
//=================================================================================
//	Read_data_flow: ham nay thuc hien kiem tra tin hieu empty cua fifo. sau do read
//	ra cac thanh ghi chua flow entry. trich xuat cac thong tin va xuat ra man hinh
//=================================================================================
void Read_data_flow(){
	u32 dataLite1,dataLite2,dataLite3,dataLite4,dataLite5;
	dataLite1=LITE4_mReadReg(netflow_baseaddr, Reg_empty);		//read empty fifo
			if(dataLite1==1){
					// Enable read fifo
					LITE4_mWriteReg(netflow_baseaddr, 0x10, 1);
					//Counter
					dataLite1=LITE4_mReadReg(netflow_baseaddr, Reg1);
					xil_printf("\r\n- Byte counter: %d\r\n", dataLite1);
					dataLite2=LITE4_mReadReg(netflow_baseaddr, Reg2);
					xil_printf("- Packet counter: %d\r\n", dataLite2);
					//Last timestamp
					dataLite1=LITE4_mReadReg(netflow_baseaddr, Reg3);
					xil_printf("- Last timestamp: %d\r\n",dataLite1);
					//intini timestamp
					dataLite2=LITE4_mReadReg(netflow_baseaddr, Reg4);
					xil_printf("- Initial timestamp: %d\r\n",dataLite2);
					dataLite1=LITE4_mReadReg(netflow_baseaddr, Reg5);
					dataLite2=LITE4_mReadReg(netflow_baseaddr, Reg6);
					xil_printf("- TCP flags: 0x%x\r\n",(u8)dataLite1);
					xil_printf("- protocol: 0x%x\r\n",((u16)dataLite1)>>8);
					xil_printf("- dest-Port: 0x%x\r\n",dataLite1>>16);
					xil_printf("- source-Port: 0x%x\r\n",(u16)dataLite2);
					dataLite1=LITE4_mReadReg(netflow_baseaddr, Reg7);
					xil_printf("- dest-IP: %d.%d.%d.%d \r\n",(u16)dataLite1>>8,(u8)dataLite1,(u8)(dataLite2>>24),(u8)(dataLite2>>16));
					dataLite2=LITE4_mReadReg(netflow_baseaddr, Reg8);
					xil_printf("- source-IP: %d.%d.%d.%d \r\n",(u16)dataLite2>>8,(u8)dataLite2,(u8)(dataLite1>>24),(u8)(dataLite1>>16));

				}
}
//=================================================================================
//	Ham xuat ra thoi gian dinh dang %d-%m-%Y %H:%M:%S
//=================================================================================
void PrintTime(RTCC_Target src) {
   RTCC_Time time;
   time = GetTime(&myDevice, 0);
	xil_printf("%x/%x", time.date, time.month);
	xil_printf("/%02x", time.year);
	xil_printf(" %x:%02x", time.hour, time.minute);
	xil_printf(":%02x", time.second);
	if (time.ampm) {
	      xil_printf(" PM");
	   } else {
	      xil_printf(" AM");
	   }
	xil_printf("\r\n\r\n");
}
// in ra gia tri timestamp(time ma hoa);
//=================================================================================
//	PrintTimeLite() Ham chuyen doi time thuc te sang timestamp de dua vao ipcore NF
//
//=================================================================================
u32 loctimeOld;
void PrintTimeLite() {
	RTCC_Time time;
	timezone timezone1;
	char Timeout[20];
	time = GetTime(&myDevice,RTCC_TARGET_RTCC);
	Timeout[0] = (char)((time.date/16)+48);
	Timeout[1] = (char)((time.date%16)+48);
	Timeout[2] = '-';
	Timeout[3] = (char)((time.month/16)+48);
	Timeout[4] = (char)((time.month%16)+48);
	Timeout[5] = '-';
	Timeout[6] = '2';
	Timeout[7] = '0';
	Timeout[8] = (char)((time.year/16)+48);
	Timeout[9] = (char)((time.year%16)+48);
	Timeout[10] = ' ';
	Timeout[11] = (char)((time.hour/16)+48);
	Timeout[12] = (char)((time.hour%16)+48);
	Timeout[13] = ':';
	Timeout[14] = (char)((time.minute/16)+48);
	Timeout[15] = (char)((time.minute%16)+48);
	Timeout[16] = ':';
	Timeout[17] = (char)((time.second/16)+48);
	Timeout[18] = (char)((time.second%16)+48);
	Timeout[19] = 0;
	strptime(Timeout,"%d-%m-%Y %H:%M:%S",&timezone1);
		loctime = mktime(&timezone1);
		strftime(Timeout, 100,"%d-%m-%Y %H:%M:%S",&timezone1);
		if( loctime != loctimeOld ) {
			//LITE4_mWriteReg(netflow_baseaddr, timestamps_add, loctime);
			xil_printf("\r\n Current time: ");
			puts(Timeout);
			loctimeOld =	loctime ;
			//Read_data_flow();
		}
}
//	Su dung cac ham read cac gia tri day, month, year, hour, minute, second
RTCC_Time GetTime(PmodRTCC *InstancePtr, RTCC_Target src) {
   RTCC_Time val;

   if (src != RTCC_TARGET_PWRD && src != RTCC_TARGET_PWRU) {
      val.second = RTCC_getSec(&myDevice, src);
   }
   val.minute = RTCC_getMin(&myDevice, src);
   val.hour   = RTCC_getHour(&myDevice, src);
   val.ampm   = RTCC_getAmPm(&myDevice, src);
   val.day    = RTCC_getDay(&myDevice, src);
   val.date   = RTCC_getDate(&myDevice, src);
   val.month  = RTCC_getMonth(&myDevice, src);

   if (src == RTCC_TARGET_RTCC) {
      val.year = RTCC_getYear(&myDevice);
   } else {
      val.year = 0;
   }

   return val;
}
// Su dung cac ham write cac gia tri day, month, year, hour, minute, second
void SetTime(PmodRTCC *InstancePtr, RTCC_Target dest, RTCC_Time val) {
   if (dest != RTCC_TARGET_PWRD && dest != RTCC_TARGET_PWRU) {
      RTCC_setSec(&myDevice, dest, val.second);
   }
   RTCC_setMin(&myDevice, dest, val.minute);
   RTCC_setHour12(&myDevice, dest, val.hour, val.ampm);
   RTCC_setDay(&myDevice, dest, val.day);
   RTCC_setDate(&myDevice, dest, val.date);
   RTCC_setMonth(&myDevice, dest, val.month);

   if (dest == RTCC_TARGET_RTCC) {
      RTCC_setYear(&myDevice, val.year);
   }
}

u8 bcd2int(u8 data) {
   return ((data >> 4) * 10) + (data & 0xF);
}
u8 int2bcd(u8 data) {
   return (((data / 10) & 0xF) << 4) + ((data % 10) & 0xF);
}
void DemoCleanup() {
   DisableCaches();
}
void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#endif
}
void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#endif
}
void delay_1ms(){
	u32 _time;
	_time = 1000000;
	while(_time){
		_time=_time-1;
	}
}






