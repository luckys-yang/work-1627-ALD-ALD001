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
 * @file      File name
 * @brief     Brief
 * @author    Author's name
 * @since     2010-01-01
 * @date      2010-01-01
*/

#ifndef __USB_UITOOL_H__
#define __USB_UITOOL_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"
#include "dbg_def.h"
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/
#define UITOOL_REQUEST	0xb1 //for UI TOOL
//#define  uiToolSet sp1kUIToolSet

//#define uiToolPrintf printf
#ifndef uiToolPrintf
#define uiToolPrintf /_printf
#endif
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
UINT8
sp1kUIToolSet(
	UINT8 order,
	UINT8 id,
	UINT32 addr,
	UINT32 size,
	UINT16 resid,
	UINT16 msg
);
UINT8
sp1kUIToolGet(
	UINT8 id,
	UINT32 xdata* addr,
	UINT32 xdata* size
);
void uiToolPrepareData();
UINT8 uiToolProcessData(UINT8 id);
#endif  /* __TEMPLATE_HDR_H__ */
