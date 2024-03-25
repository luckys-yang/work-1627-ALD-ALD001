/**************************************************************************
 *
 *        Copyright (c) 2007-2008 by Sunplus mMedia Inc., Ltd.
 *
 *  This software is copyrighted by and is the property of Sunplus
 *  mMedia Inc., Ltd. All rights are reserved by Sunplus mMedia
 *  Inc., Ltd. This software may only be used in accordance with the
 *  corresponding license agreement. Any unauthorized use, duplication,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice MUST not be removed or modified without prior
 *  written consent of Sunplus mMedia Inc., Ltd.
 *
 *  Sunplus mMedia Inc., Ltd. reserves the right to modify this
 *  software without notice.
 *
 *  Sunplus mMedia Inc., Ltd.
 *  19-1, Innovation First Road, Science-Based Industrial Park,
 *  Hsin-Chu, Taiwan, R.O.C.
 *
 **************************************************************************/

/**************************************************************************
 *                            H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"
#include "asicreg.h"
#include "ae_api.h"
#include "ae_def.h"
#include "ae_debug.h"
#include "front_logi.h"
#include "initio.h"
#include "sp1k_cdsp_api.h"
#include "sp1k_calibration_api.h"
#include "hal.h"
#include "hal_cdsp.h"
#include "dbg_def.h"
#include "hal_front.h"
#include "hal_global.h"
#include "reg_def.h"
#include "sp1k_ae_api.h"
#include "sp1k_front_api.h"
#include "gpio_api.h"
#include "hal_fd.h"
#include "awb_api.h"
#include "cdsp_task.h"
#include "icat_stub.h"
#include "sp1k_osd_api.h"
#include "os_api.h"
#include "af_api.h"


#if (AF_FIXTURE_EN == 1)

AFstr afPara = {
	1,
	1,
	0,
	8,
	0,
	10,
	0,
};

UINT8 afEn;
UINT32 AFWinvalue;

/**************************************************************************
 *                           C O N S T A N T S
 **************************************************************************/
#define __FILE	__FILE_ID_LIB_AF_API__

/**************************************************************************
 *                              M A C R O S
 **************************************************************************/

/**************************************************************************
 *                          D A T A    T Y P E S
 **************************************************************************/

/**************************************************************************
 *                         G L O B A L    D A T A
 **************************************************************************/

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
/**
 * @brief	Enable/Disable AF control
 * @param	en	: disable/enable, 0/1
 * @author	LinJieCheng
 * @todo
 * @since	2012-5-10
 */
void 
afWinEnSet(
	UINT8 en
)
{
	HAL_CdspAFWinEn(en,0,0,afPara.afFlt2Type,1);
	XBYTE[REG_Cdsp_evnt1] = 0x10;
	afEn = en;
}

/**
 * @brief	 Get Enable/Disable AF status
 * @param	NULL
 * @author	LinJieCheng
 * @todo
 * @return	AF Enable/Disable
 * @since	2012-5-10
 */
UINT8 
afWinEnGet(
	void
)
{
	return afEn;
}

/**
 * @brief	 Set AF Window Config
 * @param	frontHSize : sensor output h size
 * @param     frontVSize : sensor output v size
 * @param	cropHSize : roi h size
 * @param     cropVSize : roi v size
 * @author	LinJieCheng
 * @todo
 * @return	AF Enable/Disable
 * @since	2012-5-10
 */
void
afWinCfg(
	UINT16 frontHSize,
	UINT16 frontVSize,
	UINT16 cropHSize,
	UINT16 cropVSize
)
{
	
	UINT16 afHShift,afVShift;
	UINT16 afHStart,afHEnd,afVStart,afVEnd;

	/* AF filter 1/2 weight */
	HAL_CdspAFFilterWtSet(afPara.afFlt1Wt,afPara.afFlt2Wt);

	/* AF statistic data mode, AF pg type */
	HAL_CdspAFDataModeSet(afPara.afRgbMode,afPara.afPgType);

	/* AF coring threshold, value */
	HAL_CdspAFCoringSet(afPara.afCorThr,afPara.afCorVal);   // coring threshold and gain  
	
	afHShift = (frontHSize - cropHSize)/2;
	afVShift = (frontVSize - cropVSize)/2;
	
	afHStart = 7 + afHShift;
	afHEnd = 16 + afHShift;

	afVStart = 7 + afVShift;
	afVEnd = 8 + afVShift;
	
	HAL_CdspAFWinSizeSet(afHStart,cropHSize-afHEnd,afVStart,cropVSize-afVEnd);
}

/**
 * @brief	 Get AF Value
 * @param	NULL
 * @author	LinJieCheng
 * @todo
 * @return	AF Value
 * @since	2012-5-10
 */
UINT32 
afWinValueGet(
	void
)
{
    return AFWinvalue;
}



#endif
