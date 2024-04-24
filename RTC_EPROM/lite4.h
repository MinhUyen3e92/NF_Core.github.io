/*****************************************************************************
* Filename:          E:\Cty\Netflow\TEST/drivers/lite4_v1_00_a/src/lite4.h
* Version:           1.00.a
* Description:       lite4 Driver Header File
* Date:              Thu Apr 16 17:25:05 2020 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef LITE4_H
#define LITE4_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 * -- SLV_REG4 : user logic slave module register 4
 * -- SLV_REG5 : user logic slave module register 5
 * -- SLV_REG6 : user logic slave module register 6
 * -- SLV_REG7 : user logic slave module register 7
 */
#define LITE4_USER_SLV_SPACE_OFFSET (0x00000000)
#define LITE4_SLV_REG0_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000000)
#define LITE4_SLV_REG1_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000004)
#define LITE4_SLV_REG2_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000008)
#define LITE4_SLV_REG3_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x0000000C)
#define LITE4_SLV_REG4_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000010)
#define LITE4_SLV_REG5_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000014)
#define LITE4_SLV_REG6_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x00000018)
#define LITE4_SLV_REG7_OFFSET (LITE4_USER_SLV_SPACE_OFFSET + 0x0000001C)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a LITE4 register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the LITE4 device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void LITE4_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define LITE4_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a LITE4 register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the LITE4 device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 LITE4_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define LITE4_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from LITE4 user logic slave registers.
 *
 * @param   BaseAddress is the base address of the LITE4 device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void LITE4_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 LITE4_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define LITE4_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg4(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG4_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg5(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG5_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg6(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG6_OFFSET) + (RegOffset), (Xuint32)(Value))
#define LITE4_mWriteSlaveReg7(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (LITE4_SLV_REG7_OFFSET) + (RegOffset), (Xuint32)(Value))

#define LITE4_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG0_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG1_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg2(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG2_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg3(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG3_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg4(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG4_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg5(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG5_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg6(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG6_OFFSET) + (RegOffset))
#define LITE4_mReadSlaveReg7(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (LITE4_SLV_REG7_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the LITE4 instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus LITE4_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 8


#endif /** LITE4_H */
