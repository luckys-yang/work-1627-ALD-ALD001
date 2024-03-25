/**************************************************************************
 *         Copyright(c) 2010 by i-Catch Technology Co., Ltd.              *
 *                                                                        *
 *  This software is copyrighted by and is the property of i-Catch Tech-  *
 *  nology Co., Ltd. All rights are reserved by i-Catch Technology Co.,   *
 *  Ltd. This software may only be used in accordance with the            *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice "M U S T" not be removed or modified without    *
 *  prior written consent of i-Catch Technology Co., Ltd.                 *
 *                                                                        *
 *  i-Catch Technology Co., Ltd. reserves the right to modify this        *
 *  software without notice.                                              *
 *                                                                        *
 *  i-Catch Technology Co., Ltd.                                          *
 *  19-1, Innovation First Road, Science-Based Industrial Park,           *
 *  Hsin-Chu, Taiwan, R.O.C.                                              *
 *                                                                        *
 *************************************************************************/
 
/**
 * @file      msdc_dma.h
 * @brief     the head file of msdc_dma
 * @author    Richie li
 * @since     2010-01-01
 * @date      2010-01-01
*/
#ifndef __MSDC_DMA_H__
#define __MSDC_DMA_H__
/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
 
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
 
/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
 
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
 
/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
 
/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/
 
/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/

UINT8 USBMSDC_HAL_DoDMA(UINT8 Src,UINT8 Dst, UINT32 nByte, UINT8 Mode, UINT8 FlashPageDMA);
UINT8 USBMSDC_Dram2Usb(UINT32 DramAddr,UINT16 size);
UINT8 USBMSDC_Usb2Dram(UINT32 DramAddr,UINT16 size);
UINT8 USBMSDC_Sram2Usb(UINT16 size);
UINT8 USBMSDC_Usb2Sram(UINT8* sramAddr, UINT16 size);
UINT8 USBMSDC_HAL_Set4KSRAMMode(UINT8 Mode);
UINT8 USBMSDC_HAL_SRAMHigh2Low(UINT16 size,UINT8* buffer);
UINT8 USBMSDC_HAL_SRAMLow2High(UINT16 size,UINT8* buffer);
UINT8 USBMSDC_HAL_SetSRAMDMA(UINT16 SRAMAddr);
//UINT8 USBMSDC_HAL_SetUSBDMA(UINT8 USBDMASrc, UINT8 USBDMADst);
UINT8 USBMSDC_HAL_SetDRAMDMA(UINT32 DRAMAddr);
void USBMSDC_HAL_ReadDRAMDMAAdr(UINT32* DRAMAddr);


#endif /*__MSDC_DMA_H__*/
