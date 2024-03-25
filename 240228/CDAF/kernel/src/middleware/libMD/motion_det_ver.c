/**************************************************************************
 *
 *        Copyright (c) 2002-2008 by Sunplus mMedia Inc., Ltd.
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
 *	md01, 20101015 dongdong.wang
 *		a. modify number of version
 *	md02, 20120222 linjie.cheng
 *		a. add md adjust td weight ctrl flow
 *	md03, 20120511 linjie.cheng
 *		a. add md bar function get
 *	md04, 20130318 linjie.cheng
 *           a. add panorama function 
 *           b. adjust motion detect flow
 *	md05, 20130326 linjie.cheng
 *           a. fix panorama flow
 *           b. add ctrl edge yuv src define
 *           c. add ctrl md yuv src define
 *           d. add ctrl 2nd path yuv size define
 *
 *
 **************************************************************************/

/**************************************************************************
 *                            H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"

/**************************************************************************
 *                            G L O B A L   D A T A
 **************************************************************************/
static UINT8 code verStr[] = "$md05";

/**************************************************************************
 *
 *  Function Name: audLibVerGet
 *
 *  Purposes: Get current libaray's version.
 *
 *  Descriptions:
 *
 *  Arguments:
 *
 *  Returns:
 *
 *  See also:
 *
 **************************************************************************/ 
UINT8 *
libMDVerGet(
	void
)
{
	return (verStr + 3);
}

