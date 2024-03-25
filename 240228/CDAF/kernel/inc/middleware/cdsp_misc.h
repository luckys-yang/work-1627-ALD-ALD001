/**************************************************************************
 *         Copyright(c) 2007 by Sunplus mMedia  Technology Co., Ltd.       *
 *                                                                         *
 *  This software is copyrighted by and is the property of Sunplus mMedia  *
 *  Technology Co., Ltd. All rights are reserved by Sunplus mMedia Techno- *
 *  logy Co., Ltd. This software may only be used in accordance with the   *
 *  corresponding license agreement. Any unauthorized use, duplication,    *
 *  distribution, or disclosure of this software is expressly forbidden.   *
 *                                                                         *
 *  This Copyright notice "M U S T" not be removed or modified without     *
 *  prior written consent of Sunplus mMedia Technology  Co., Ltd.          *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd. reserves the right to modify this  *
 *  software without notice.                                               *
 *                                                                         *
 *  Sunplus mMedia Technology Co., Ltd.                                    *
 *  19-1, Innovation First Road, Science-Based Industrial Park,            *
 *  Hsin-Chu, Taiwan, R.O.C.                                               *
 *                                                                         *
 **************************************************************************/
#ifndef __CDSP_MISC_H__
#define __CDSP_MISC_H__

/** \file sp1k_cdsp_api.h
 * @brief		Spca1528 cdsp related interface
 */

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

void cdspBinRead( UINT8 *f, UINT32 startAddr );
void hueBinRead( UINT8 *f, UINT8 channel, UINT32 startAddr );
void lscBinRead( UINT8 *f, UINT32 startAddr );
void gammaBinRead( UINT8 *f, UINT32 startAddr );
void wdrBinRead( UINT8 *f, UINT32 startAddr );

void pvCdspBinRead( UINT8 *f, UINT32 startAddr );
void pvCdspDynamicBinRead(UINT8 *f, UINT8 intensity, UINT32 startAddr);
void pvCdspDynamicBinReadtoPool(UINT8 *f,	UINT32 startAddr);
void cdspPvDynamicEnhanSet(UINT8 fileId, UINT8 intensity);
void cdspPvDynamicParaFullLoad(UINT8 fileId);


void stillCdspBinRead( UINT8 *f, UINT32 startAddr );
void stillCdspDynamicBinRead(UINT8 *f, UINT8 intensity, UINT32 startAddr);
void cdspStillDynamicEnhanSet(UINT8 fileId, UINT8 intensity);

void iqDataWrite( UINT8 id, UINT32 startAddr );
void criteriaDataRead( UINT32 startAddr );
void criteriaDataWrite( UINT32 startAddr );

void cdspColorMatrixRestore(void);
void cdspWdrInit(void);

void cdspHueBinSet( UINT8 fileId, UINT8 channel);


#endif //__SP1K_OSD_API_H__

