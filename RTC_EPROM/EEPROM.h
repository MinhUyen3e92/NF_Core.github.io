

#ifndef EEPROM_H
#define EEPROM_H

/* ------------------------------------------------------------ */
/*                  Include Files                               */
/* ------------------------------------------------------------ */

#include "xiic.h"
#include "xiic_l.h"
#include "xil_types.h"
#include "xstatus.h"
/* ------------------------------------------------------------ */
/*                  Definitions                                 */
/* ------------------------------------------------------------ */
//user
//user
//******************************** define eeprom *********************//
/*
u8 KeySecurity			=0x000;
u8 userRoot_addr		=0x010;
u8 passRoot_addr 		=0x020;
u8 user1_addr	 		=0x030;
u8 pass1_addr	 		=0x040;
u8 user2_addr	 		=0x050;
u8 pass2_addr	 		=0x060;
u8 user3_addr	 		=0x070;
u8 pass3_addr	 		=0x080;
u8 user4_addr	 		=0x090;
u8 pass4_addr	 		=0x0a0;
u8 user5_addr	 		=0x0b0;
u8 pass5_addr	 		=0x0c0;
u8 user6_addr	 		=0x0d0;
u8 pass6_addr	 		=0x0e0;
u8 user7_addr	 		=0x0f0;

u8 port_number_addr 		=0x100;
u8 entry_number_addr 		=0x108;

u8 active_time_hi_addr 			=0x110;
u8 active_time_lo_addr			=0x111;
u8 inactive_time_hi_addr 		=0x114;
u8 inactive_time_lo_addr		=0x115;
u8 timestamp_1_addr				=0x118;
u8 timestamp_2_addr				=0x119;
u8 timestamp_3_addr				=0x11a;
u8 timestamp_4_addr				=0x11b;

u8  port_mirror_1_hi_addr		=0x120;
u8	port_mirror_1_lo_addr		=0x121;
u8 	port_mirror_2_hi_addr 	    =0x122;
u8	port_mirror_2_lo_addr		=0x123;
u8 	port_mirror_3_hi_addr		=0x124;
u8 	port_mirror_3_lo_addr		=0x125;
u8	port_mirror_4_hi_addr		=0x126;
u8	port_mirror_4_lo_addr		=0x127;
u8	port_mirror_5_hi_addr		=0x128;
u8	port_mirror_5_lo_addr		=0x129;
u8	port_mirror_6_hi_addr		=0x130;
u8	port_mirror_6_lo_addr		=0x131;
u8	port_mirror_7_hi_addr		=0x132;
u8	port_mirror_7_lo_addr		=0x133;
u8	port_mirror_8_hi_addr		=0x134;
u8	port_mirror_8_lo_addr		=0x135;
u8 	mirror_status_addr			=0x13a;

u8 	vlan_id_1_hi_addr			=0x200;
u8 	vlan_id_1_lo_addr			=0x201;
u8 	vlan_id_2_hi_addr			=0x202;
u8 	vlan_id_2_lo_addr			=0x203;
u8 	vlan_id_3_hi_addr			=0x204;
u8 	vlan_id_3_lo_addr			=0x205;
u8 	vlan_id_4_hi_addr			=0x206;
u8 	vlan_id_4_lo_addr			=0x207;
u8 	vlan_id_5_hi_addr			=0x208;
u8 	vlan_id_5_lo_addr			=0x209;
u8 	vlan_id_6_hi_addr			=0x20a;
u8 	vlan_id_6_lo_addr			=0x20b;
u8 	vlan_id_7_hi_addr			=0x20c;
u8 	vlan_id_7_lo_addr			=0x20d;
u8 	vlan_id_8_hi_addr			=0x20e;
u8 	vlan_id_8_lo_addr			=0x20f;
u8 	vlan_id_9_hi_addr			=0x210;
u8 	vlan_id_9_lo_addr			=0x211;
u8 	vlan_id_10_hi_addr			=0x212;
u8 	vlan_id_10_lo_addr			=0x213;
u8 	vlan_id_11_hi_addr			=0x214;
u8 	vlan_id_11_lo_addr			=0x215;
u8 	vlan_id_12_hi_addr			=0x216;
u8 	vlan_id_12_lo_addr			=0x217;
u8 	vlan_id_13_hi_addr			=0x218;
u8 	vlan_id_13_lo_addr			=0x210;
u8 	vlan_id_14_hi_addr			=0x21a;
u8 	vlan_id_14_lo_addr			=0x21b;
u8 	vlan_id_15_hi_addr			=0x21c;
u8 	vlan_id_15_lo_addr			=0x21d;
u8 	vlan_id_16_hi_addr			=0x21e;
u8 	vlan_id_16_lo_addr			=0x21f;
u8 	vlan_id_17_hi_addr			=0x220;
u8 	vlan_id_17_lo_addr			=0x221;
u8 	vlan_id_18_hi_addr			=0x222;
u8 	vlan_id_18_lo_addr			=0x223;
u8 	vlan_id_19_hi_addr			=0x224;
u8 	vlan_id_19_lo_addr			=0x225;
u8 	vlan_id_20_hi_addr			=0x226;
u8 	vlan_id_20_lo_addr			=0x227;
u8 	vlan_id_21_hi_addr			=0x228;
u8 	vlan_id_21_lo_addr			=0x229;
u8 	vlan_id_22_hi_addr			=0x22a;
u8 	vlan_id_22_lo_addr			=0x22b;
u8 	vlan_id_23_hi_addr			=0x22c;
u8 	vlan_id_23_lo_addr			=0x22d;
u8 	vlan_id_24_hi_addr			=0x22e;
u8 	vlan_id_24_lo_addr			=0x22f;
u8 	vlan_id_25_hi_addr			=0x230;
u8 	vlan_id_25_lo_addr			=0x231;
u8 	vlan_id_26_hi_addr			=0x232;
u8 	vlan_id_26_lo_addr			=0x233;
u8 	vlan_id_27_hi_addr			=0x234;
u8 	vlan_id_27_lo_addr			=0x235;
u8 	vlan_id_28_hi_addr			=0x236;
u8 	vlan_id_28_lo_addr			=0x237;
u8 	vlan_id_29_hi_addr			=0x238;
u8 	vlan_id_29_lo_addr			=0x239;
u8 	vlan_id_30_hi_addr			=0x23a;
u8 	vlan_id_30_lo_addr			=0x23b;
u8 	vlan_id_31_hi_addr			=0x23c;
u8 	vlan_id_31_lo_addr			=0x23d;
u8 	vlan_id_32_hi_addr			=0x23e;
u8 	vlan_id_32_lo_addr			=0x23f;

u8	dest_port_vlan_1_hi_addr		=0x300;
u8	dest_port_vlan_1_lo_addr		=0x301;
u8	dest_port_vlan_2_hi_addr		=0x302;
u8	dest_port_vlan_2_lo_addr		=0x303;
u8	dest_port_vlan_3_hi_addr		=0x304;
u8	dest_port_vlan_3_lo_addr		=0x305;
u8	dest_port_vlan_4_hi_addr		=0x306;
u8	dest_port_vlan_4_lo_addr		=0x307;
u8	dest_port_vlan_5_hi_addr		=0x308;
u8	dest_port_vlan_5_lo_addr		=0x309;
u8	dest_port_vlan_6_hi_addr		=0x30a;
u8	dest_port_vlan_6_lo_addr		=0x30b;
u8	dest_port_vlan_7_hi_addr		=0x30c;
u8	dest_port_vlan_7_lo_addr		=0x30d;
u8	dest_port_vlan_8_hi_addr		=0x30e;
u8	dest_port_vlan_8_lo_addr		=0x30f;
u8	dest_port_vlan_9_hi_addr		=0x310;
u8	dest_port_vlan_9_lo_addr		=0x311;
u8	dest_port_vlan_10_hi_addr		=0x312;
u8	dest_port_vlan_10_lo_addr		=0x313;
u8	dest_port_vlan_11_hi_addr		=0x314;
u8	dest_port_vlan_11_lo_addr		=0x315;
u8	dest_port_vlan_12_hi_addr		=0x316;
u8	dest_port_vlan_12_lo_addr		=0x317;
u8	dest_port_vlan_13_hi_addr		=0x318;
u8	dest_port_vlan_13_lo_addr		=0x319;
u8	dest_port_vlan_14_hi_addr		=0x31a;
u8	dest_port_vlan_14_lo_addr		=0x31b;
u8	dest_port_vlan_15_hi_addr		=0x31c;
u8	dest_port_vlan_15_lo_addr		=0x31d;
u8	dest_port_vlan_16_hi_addr		=0x31e;
u8	dest_port_vlan_16_lo_addr		=0x31f;
u8	dest_port_vlan_17_hi_addr		=0x320;
u8	dest_port_vlan_17_lo_addr		=0x321;
u8	dest_port_vlan_18_hi_addr		=0x322;
u8	dest_port_vlan_18_lo_addr		=0x323;
u8	dest_port_vlan_19_hi_addr		=0x324;
u8	dest_port_vlan_19_lo_addr		=0x325;
u8	dest_port_vlan_20_hi_addr		=0x326;
u8	dest_port_vlan_20_lo_addr		=0x327;
u8	dest_port_vlan_21_hi_addr		=0x328;
u8	dest_port_vlan_21_lo_addr		=0x329;
u8	dest_port_vlan_22_hi_addr		=0x32a;
u8	dest_port_vlan_22_lo_addr		=0x32b;
u8	dest_port_vlan_23_hi_addr		=0x32c;
u8	dest_port_vlan_23_lo_addr		=0x32d;
u8	dest_port_vlan_24_hi_addr		=0x32e;
u8	dest_port_vlan_24_lo_addr		=0x32f;
u8	dest_port_vlan_25_hi_addr		=0x330;
u8	dest_port_vlan_25_lo_addr		=0x331;
u8	dest_port_vlan_26_hi_addr		=0x332;
u8	dest_port_vlan_26_lo_addr		=0x333;
u8	dest_port_vlan_27_hi_addr		=0x334;
u8	dest_port_vlan_27_lo_addr		=0x335;
u8	dest_port_vlan_28_hi_addr		=0x336;
u8	dest_port_vlan_28_lo_addr		=0x337;
u8	dest_port_vlan_29_hi_addr		=0x338;
u8	dest_port_vlan_29_lo_addr		=0x339;
u8	dest_port_vlan_30_hi_addr		=0x33a;
u8	dest_port_vlan_30_lo_addr		=0x33b;
u8	dest_port_vlan_31_hi_addr		=0x33c;
u8	dest_port_vlan_31_lo_addr		=0x33d;
u8	dest_port_vlan_32_hi_addr		=0x33e;
u8	dest_port_vlan_32_lo_addr		=0x33f;
*/
typedef struct EEPROM24C {
   XIic EEPROMIic;
   u8 chipAddr;
   u8 currentRegister; //u8
} EEPROM24C;

/* ------------------------------------------------------------ */
/*                  Procedure Declarations                      */
/* ------------------------------------------------------------ */

void EEPROM_begin(EEPROM24C *InstancePtr, u32 IIC_Address, u8 Chip_Address);
u32  EEPROM_IICInit(XIic *IicInstancePtr);
void EEPROM_ReadIIC(EEPROM24C *InstancePtr, u8 reg, u8 *Data, int nData);
void EEPROM_WriteIIC(EEPROM24C *InstancePtr, u8 reg, u8 *Data, int nData);
void EEPROM_WriteIIC_2(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData);
//void EEPROM_WriteIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData);
//void EEPROM_ReadIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData);
u8 EEPROM_getSec(EEPROM24C *InstancePtr);

void EEPROM_setSec(EEPROM24C *InstancePtr, u8 value);



#endif // PMODRTCC_H
