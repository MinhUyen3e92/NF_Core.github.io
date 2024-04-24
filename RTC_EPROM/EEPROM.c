/******************************************************************************/
/*                                                                            */
/*                                      */
/*                                                                            */


/***************************** Include Files *******************************/

#include "EEPROM.h"
#include "xil_cache.h"
/************************** Function Definitions ***************************/

XIic_Config EEPROM_Config =
{
   0,
   0,
   0,
   2
};

/* ------------------------------------------------------------ */
/*** void RTCC_begin(PmodRTCC *InstancePtr, u32 IIC_Address)
**
**   Parameters:
**      InstancePtr:  A PmodRTCC device to start
**      IIC_Address:  The Base address of the PmodRTCC IIC
**      Chip_Address: 7 bit Pmod device address
**
**   Return Value:
**      none
**
**   Errors:
**      none
**
**   Description:
**      Initialize the PmodRTCC.
*/
void EEPROM_begin(EEPROM24C *InstancePtr, u32 IIC_Address, u8 Chip_Address) {
	EEPROM_Config.BaseAddress = IIC_Address;
   InstancePtr->chipAddr = Chip_Address;

   EEPROM_IICInit(&InstancePtr->EEPROMIic);

   XIic_SetAddress(&InstancePtr->EEPROMIic, XII_ADDR_TO_SEND_TYPE,
         InstancePtr->chipAddr);
}

/* ------------------------------------------------------------ */
/*** RTCC_IICInit
**
**   Parameters:
**      none
**
**   Return Value:
**      none
**
**   Errors:
**      none
**
**   Description:
**      Initializes the PmodRTCC IIC.
**
*/
u32 EEPROM_IICInit(XIic *IicInstancePtr) {
   XStatus Status;

   Status = XIic_CfgInitialize(IicInstancePtr, &EEPROM_Config,
		   EEPROM_Config.BaseAddress);
   if (Status != XST_SUCCESS) {
      return XST_FAILURE;
   }

   /*
    * Start the IIC driver so that the device is enabled.
    */
   XIic_Start(IicInstancePtr);

   /*
    * Disable Global interrupt to use polled mode operation
    */
   XIic_IntrGlobalDisable(IicInstancePtr);

   return XST_SUCCESS;
}

/* ------------------------------------------------------------ */
/*** void RTCC_ReadIIC(PmodRTCC *InstancePtr, u8 reg, u8 *Data, int nData)
**
**   Parameters:
**      InstancePtr - PmodRTCC device to initialize
**      reg         - Register to read from
**      Data        - Pointer to recieve buffer
**      nData       - Number of data values to read
**
**   Return Value:
**      none
**
**   Errors:
**      none
**
**   Description:
**      Reads nData data bytes from register reg
**
*/
void EEPROM_ReadIIC(EEPROM24C *InstancePtr, u8 reg, u8 *Data, int nData) {
   XStatus Status;
   Status = XIic_Start(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }

   XIic_Send(InstancePtr->EEPROMIic.BaseAddress, InstancePtr->chipAddr, &reg, 1,
         XII_REPEATED_START_OPTION);
   InstancePtr->currentRegister = reg;

   Status = XIic_Recv(InstancePtr->EEPROMIic.BaseAddress, InstancePtr->chipAddr,
         Data, nData, XIIC_STOP);

   Status = XIic_Stop(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }
}

/* ------------------------------------------------------------ */
/*** void RTCC_WriteIIC(PmodRTCC *InstancePtr, u8 reg, u8 *Data, int nData)
**
**   Parameters:
**      InstancePtr - PmodRTCC device to initialize
**      reg         - Register to send to
**      Data        - Pointer to data buffer to send
**      nData       - Number of data values to send
**
**   Return Value:
**      none
**
**   Errors:
**      none
**
**   Description:
**      Writes nData data bytes to chosen register
*/
void EEPROM_WriteIIC(EEPROM24C *InstancePtr, u8 reg, u8 *Data, int nData) {
   u8 out[10];
   out[0] = reg;
   out[1] = *Data;
   XStatus Status;

   if (InstancePtr->currentRegister != reg) {
      InstancePtr->currentRegister = reg;
   }

   Status = XIic_Start(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }

   XIic_Send(InstancePtr->EEPROMIic.BaseAddress, InstancePtr->chipAddr, out,
         nData + 1, XIIC_STOP);

   Status = XIic_Stop(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }
}
/*
void EEPROM_ReadIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData)
{
	u8 cell_num = (reg%256)%64;
	u8 page_num = (reg/256)%4;
	u16 new_reg = page_num*64 + cell_num;
	EEPROM_ReadIIC(	InstancePtr, 	new_reg, 	Data,	 nData);
}


void EEPROM_WriteIIC_paging(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData)
{
	u8 cell_num = (reg%256)%64;
	u8 page_num = (reg/256)%4;
	u16 new_reg = page_num*64 + cell_num;
	EEPROM_WriteIIC(	InstancePtr, 	new_reg, 	Data,	 nData);
}*/

void EEPROM_WriteIIC_2(EEPROM24C *InstancePtr, u16 reg, u8 *Data, int nData) {
   u8 out[10];
   out[0] = reg>>8;
   out[1] = reg;
   out[2] = *Data;
   XStatus Status;

   if (InstancePtr->currentRegister != reg) {
      InstancePtr->currentRegister = reg;
   }

   Status = XIic_Start(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }

   XIic_Send(InstancePtr->EEPROMIic.BaseAddress, InstancePtr->chipAddr, out,
         nData + 2, XIIC_STOP);

   Status = XIic_Stop(&InstancePtr->EEPROMIic);
   if (Status != XST_SUCCESS) {
      return;
   }
}

/*** u8 RTCC_getSec(PmodRTCC *InstancePtr, RTCC_Target src)
**
**   Parameters:
**      InstancePtr - PmodRTCC device to use
**      src         - RTCC_TARGET_RTCC - real-time clock
**                    RTCC_TARGET_ALM0 - Alarm 0
**                    RTCC_TARGET_ALM1 - Alarm 1
**         ** RTCC_TARGET_PWRU and RTCC_TARGET_PWRD does not have the second
**            parameter
**   Return Value:
**      uint8_t - second in hexadecimal
**
**   Description:
**      This function returns the second of the source
*/

u8 EEPROM_getSec(EEPROM24C *InstancePtr) {
   u8 bSecBuffer = 0;
   u8 bSec;

   // Set address of the second register depending on the source
     bSec = 0x10;
     EEPROM_ReadIIC(InstancePtr, bSec, &bSecBuffer, 1);
   // Return valid bits
  // return (bSecBuffer & 0x7F);
   return (bSecBuffer);
}

/*** void RTCC_setSec(PmodRTCC *InstancePtr, RTCC_Target dest, u8 value)
**
**   Parameters:
**      InstancePtr - PmodRTCC device to use
**      dest        - RTCC_TARGET_RTCC - real-time clock
**                    RTCC_TARGET_ALM0 - Alarm 0
**                    RTCC_TARGET_ALM1 - Alarm 1
**      value       - desired value for the second in HEX 0x00-0x59
**
**   Return Value:
**      None
**
**   Description:
**      This function sets the second register of the target with the value.
*/
void EEPROM_setSec(EEPROM24C *InstancePtr, u8 value) {
   u8 rgbSec[2];
   rgbSec[1] = value;

   // Set address of the second register depending on the destination
	rgbSec[0] = 0x10;
	/*
	EEPROM_ReadIIC(InstancePtr, rgbSec[0], &data, 1);
	if ((data & 0x80) == 0x80) {
	 // Preserve configuration bits
	 rgbSec[1] = (rgbSec[1] | 0x80);
	}
	*/
	EEPROM_WriteIIC(InstancePtr, rgbSec[0], &rgbSec[1], 1);
}
