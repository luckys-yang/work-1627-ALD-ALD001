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
 * @file      gui_dbg.h
 * @brief     GUI debug
 * @author    Phil Lin
 * @since     2011-09-05
 * @date      2011-09-05
*/

#ifndef __GUI_DBG_H__
#define __GUI_DBG_H__

/**************************************************************************
 *                        H E A D E R   F I L E S
 **************************************************************************/
#include "gui_core.h" 
/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/
 
#define GUI_DBG_LV	2

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
void guiDbgEnSet(UINT8 en);
UINT8 guiObjDraw(guiObj_t *pObj, UINT8 color);
UINT8 guiObjClr(guiObj_t *pObj);
void guiObjDump(void);

#endif  /* __GUI_DBG_H__ */
