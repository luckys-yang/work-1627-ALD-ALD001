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
#ifndef __ICAT_DEBUG_H__
#define __ICAT_DEBUG_H__

//=============================================================================
//Header file
//=============================================================================
#include "dbg_def.h"
//=============================================================================
//Symbol
//=============================================================================
//------------------------------------------------------------------------------
//Constant
//------------------------------------------------------------------------------

#define icatPrintfLog printf 			/* printf important message */
//#define icatPrintf printf 			/* printf function name */
//#define icatPrintfCmdFlow printf 		/* printf command flow */
//#define icatPrintfDownload printf 	/* printf download flow */


#ifndef icatPrintfLog
#define icatPrintfLog /_printf
#endif

#ifndef icatPrintf
#define icatPrintf /_printf
#endif

#ifndef icatPrintfCmdFlow
#define icatPrintfCmdFlow /_printf
#endif

#ifndef icatPrintfDownload
#define icatPrintfDownload /_printf
#endif

#endif /*__PB_DEBUG_H__*/




