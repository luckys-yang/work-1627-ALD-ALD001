/**************************************************************************
 *
 *        Copyright (c) 2008-2008 by Sunplus mMedia Inc., Ltd.
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
/**
 * @file		front_ov_ov9655_yuv.c
 * @brief		front sensor omni vision ov9655 driver with yuv format data.
 * @author		CS Lin
 * @since		2008-11-24
 * @date		2008-12-08
 */

/*
 * History
 *
 * 1000, 20081117 Matt Wang
 *    a. Created
 */
 
/**************************************************************************
 *                            H E A D E R   F I L E S
 **************************************************************************/
#include "general.h"
#include "front_logi.h"
#include "gpio_api.h"
#include "i2c.h"
#include "sp1k_util_api.h"
#include "hal.h"
#include "hal_cdsp.h"
#include "hal_cdsp_iq.h"
#include "cdsp_misc.h"
#include "hal_front.h"
#include "hal_front_para.h"
#include "ae_api.h"
#include "sp1k_ae_api.h"
#include "sp1k_hal_api.h"
#include "sp1k_rsvblk_api.h"
#include "initio.h"
#include "dbg_file_list.h"
#include "dbg_def.h"

#include "sp1k_awb_api.h"
/**************************************************************************
 *                              C O N S T A N T S
 **************************************************************************/
#define __FILE	__FILE_ID_SENSOR_DRIVER__
 
#define REGISTER_EXTERN_OP_FUNC		1//0	/* used register extern operating function */
 
/* mode. */
#define PREV_MODE_TOT  2
#define SNAP_MODE_TOT  1

/* dimensiion. */
#define WIDTH_30FPS_PREV      640
#define HEIGHT_30FPS_PREV     480
#define XOFF_30FPS_PREV       94
#define YOFF_30FPS_PREV       11
#define LINE_TOT_30FPS_PREV   780
#define FRAME_TOT_30FPS_PREV  525

#define WIDTH_60FPS_PREV      0
#define HEIGHT_60FPS_PREV     0
#define XOFF_60FPS_PREV       0
#define YOFF_60FPS_PREV       0
#define LINE_TOT_60FPS_PREV   0
#define FRAME_TOT_60FPS_PREV  0

#define WIDTH_SNAP      1280
#define HEIGHT_SNAP     1024
#define XOFF_SNAP       228
#define YOFF_SNAP       12
#define LINE_TOT_SNAP   780
#define FRAME_TOT_SNAP  525

/* clk. */
#define MCLK_SRC  FRONT_MCLK_SRC_EXT
#define PCLK_SRC  FRONT_PCLK_SRC_INT

#define MCLK_DIV_30FPS_PREV    12//16// 2//change by ml 20110316
#define MCLK_PHASE_30FPS_PREV  0
#define PCLK_DIV_30FPS_PREV    2// 1//need to be set 2
#define PCLK_PHASE_30FPS_PREV  0

#define MCLK_DIV_60FPS_PREV    4
#define MCLK_PHASE_60FPS_PREV  0
#define PCLK_DIV_60FPS_PREV    1
#define PCLK_PHASE_60FPS_PREV  0

#define MCLK_DIV_SNAP    		12// 4
#define MCLK_DIV_SNAP_BURST     12
#define MCLK_DIV_SNAP_BURST_UP  12
#define MCLK_PHASE_SNAP  0
#define PCLK_DIV_SNAP    2// 1//need to be set 2
#define PCLK_PHASE_SNAP  0

#if PCLK_SRC == FRONT_PCLK_SRC_EXT
#define PCLK_FREQ_30FPS_PREV  27000000L
#define PCLK_FREQ_60FPS_PREV  0
#define PCLK_FREQ_SNAP        27000000L
#else
#define PCLK_FREQ_30FPS_PREV  261000000L / MCLK_DIV_30FPS_PREV / PCLK_DIV_30FPS_PREV
#define PCLK_FREQ_60FPS_PREV  261000000L / MCLK_DIV_60FPS_PREV / PCLK_DIV_60FPS_PREV
#define PCLK_FREQ_SNAP        261000000L / MCLK_DIV_SNAP / PCLK_DIV_SNAP
#define PCLK_FREQ_SNAP_BURST  	261000000L / MCLK_DIV_SNAP_BURST / PCLK_DIV_SNAP   
#define PCLK_FREQ_SNAP_BURST_UP 261000000L / MCLK_DIV_SNAP_BURST_UP / PCLK_DIV_SNAP   

#endif

/* bypass */
#define BYPASS_HREF_EN			0
#define BYPASS_VREF_EN			0


/* reshape. */
#if BYPASS_HREF_EN == 0
#define RESHAPE_HEN_30FPS_PREV    1
#define RESHAPE_HEN_60FPS_PREV    0
#else
#define RESHAPE_HEN_30FPS_PREV    0	//bypass h don't enable
#define RESHAPE_HEN_60FPS_PREV    0	//bypass h don't enable
#endif
#define RESHAPE_HFALL_30FPS_PREV  39
#define RESHAPE_HRISE_30FPS_PREV  40
#define RESHAPE_HFALL_60FPS_PREV  0
#define RESHAPE_HRISE_60FPS_PREV  0

#if BYPASS_VREF_EN == 0
#define RESHAPE_VEN_30FPS_PREV    1
#define RESHAPE_VEN_60FPS_PREV    0
#else
#define RESHAPE_VEN_30FPS_PREV    0	//bypass v don't enable
#define RESHAPE_VEN_60FPS_PREV    0	//bypass v don't enable
#endif

#define RESHAPE_VFALL_30FPS_PREV  2
#define RESHAPE_VRISE_30FPS_PREV  3
#define RESHAPE_VFALL_60FPS_PREV  0
#define RESHAPE_VRISE_60FPS_PREV  0

#define HSYNC_INV_PREV  1
#define VSYNC_INV_PREV  0

#define RESHAPE_HEN_SNAP    1
#define RESHAPE_HFALL_SNAP  39
#define RESHAPE_HRISE_SNAP  40
#define RESHAPE_VEN_SNAP    1
#define RESHAPE_VFALL_SNAP  2
#define RESHAPE_VRISE_SNAP  3

#define HSYNC_INV_SNAP  0
#define VSYNC_INV_SNAP  0

/* bayer pattern. */
#define BAYER_PTN_PREV  0x00
#define BAYER_PTN_SNAP  0x00

/* yuv interface. */
#define YUV_SEQ    FRONT_YUV_OUTPUT_VYUY//FRONT_YUV_OUTPUT_YVYU//FRONT_YUV_OUTPUT_UYVY
#define CCIR_MODE  FRONT_YUV_CCIR601
#define Y_SUB_128  0
#define U_SUB_128  1
#define V_SUB_128  1

/* i2c interface. */
#define I2C_DEV_ADDR  0x60
#define I2C_CLK_DIV   1// 3

/* gpio interface. */
#define GPIO_FUNC  0x00
#define GPIO_DIR   0x04
#define GPIO_GATE  0x00
#define GPIO_OUT   0x00

/* ae. */
#define AE_30FPS_30_MAX_IDX  1
#define AE_30FPS_30_MIN_IDX  1
#define AE_30FPS_25_MAX_IDX  1
#define AE_30FPS_25_MIN_IDX  1
#define EV10_30FPS_EXP_IDX   100

#define AE_60FPS_60_MAX_IDX  0
#define AE_60FPS_60_MIN_IDX  0
#define AE_60FPS_50_MAX_IDX  0
#define AE_60FPS_50_MIN_IDX  0
#define EV10_60FPS_EXP_IDX   0

/* gain. */
#define GAIN_30FPS_MAX_IDX  46
#define GAIN_30FPS_MIN_IDX  5

#define GAIN_60FPS_MAX_IDX  0
#define GAIN_60FPS_MIN_IDX  0

#define GAIN_LARGE   0
#define GAIN_MIDDLE  0
#define GAIN_SMALL   0

/* Preview SRAM Mode */
#define PREVIEW_SRAM_MODE	FRONT_SRAM_MODE_DISABLE				//FRONT_SRAM_MODE_ENABLE : image width <= 1280
																//FRONT_SRAM_MODE_DISABLE : image width > 1280 or yuv sensor


#define EXP_GAIN_SET_POS  AE_EXP_GAIN_NOT_ACTION_FOR_YUV		// AE_EXP_GAIN_SET_POS_VD_RISING: VD rising, 
																//AE_EXP_GAIN_SET_POS_VVALID_FALLING: VVALID falling,
																//AE_EXP_GAIN_SET_POS_VVALID_RISING: VVALID rising.
#define GAIN_AFTER_EXP    AE_GAIN_AFTER_EXP_OFF					//AE_GAIN_AFTER_EXP_OFF,
																//AE_GAIN_AFTER_EXP_ON

/**************************************************************************
 *                                  M A C R O S
 **************************************************************************/
/* function type definition. */
#define __STATIC  //static

/* operating function. */
#define frontOpen                     frontSensorOpen
#define frontOpDevNameGet             frontSensorOpDevNameGet
#define frontOpFrameRateSet           frontSensorOpFrameRateSet
#define frontOpCapabilityGet          frontSensorOpCapabilityGet
#define frontOpIntrCapabilityGet      frontSensorOpIntrCapabilityGet
#define frontOpPowerOn                frontSensorOpPowerOn
#define frontOpPowerOff               frontSensorOpPowerOff
#define frontOpPreviewModeSet         frontSensorOpPreviewModeSet
#define frontOpSnapModeSet            frontSensorOpSnapModeSet
#define frontOpSnapShot               frontSensorOpSnapShot
#define frontOpGainSet                frontSensorOpGainSet
#define frontOpExposureTimeSet        frontSensorOpExposureTimeSet
#define frontOpIntrGainSet            frontSensorOpIntrGainSet     
#define frontOpIntrExposureTimeSet    frontSensorOpIntrExposureTimeSet
#define frontOpOpticalBlackStatusGet  frontSensorOpOpticalBlackStatusGet
#define frontOpOpticalBlackCalibrate  frontSensorOpOpticalBlackCalibrate
#define frontOpShutterOpen            frontSensorOpShutterOpen
#define frontOpShutterClose           frontSensorOpShutterClose

/* extern operating function. */
#define frontOpAeTargetLuminanceSet       frontSensorOpAeTargetLuminanceSet
#define frontOpAeExposureCompensationSet  frontSensorOpAeExposureCompensationSet
#define frontOpAeFrameRateSet             frontSensorOpAeFrameRateSet
#define frontOpAwbModeSet                 frontSensorOpAwbModeSet
#define frontOpAfterSnapShot              frontSensorOpAfterSnapShot


/**************************************************************************
 *                  F U N C T I O N   D E C L A R A T I O N S
 **************************************************************************/
/* operating function. */
__STATIC UINT8 *frontOpDevNameGet(void);
__STATIC void frontOpFrameRateSet(UINT8 fps);
__STATIC void frontOpCapabilityGet(frontCapabilityArg_t *parg);
__STATIC void frontOpIntrCapabilityGet(frontCapabilityArg_t *parg);
__STATIC void frontOpPowerOn(void);
__STATIC void frontOpPowerOff(void);
__STATIC void frontOpPreviewModeSet(void);
__STATIC void frontOpSnapModeSet(void);
__STATIC void frontOpSnapShot(UINT8 first,UINT8 mode,UINT8 scaleUp);
__STATIC void frontOpGainSet(UINT8 val, UINT8 isr, UINT8 upd);
__STATIC void frontOpExposureTimeSet(frontExposureTimeArg_t *parg);
__STATIC void frontOpIntrGainSet(UINT8 val, UINT8 isr, UINT8 upd);
__STATIC void frontOpIntrExposureTimeSet(frontExposureTimeArg_t *parg);
__STATIC UINT8 frontOpOpticalBlackStatusGet(void);
__STATIC UINT8 frontOpOpticalBlackCalibrate(void);
__STATIC void frontOpShutterOpen(void);
__STATIC void frontOpShutterClose(void);

/* extern operating function. */
__STATIC void frontOpAeTargetLuminanceSet(void *parg);
__STATIC void frontOpAeExposureCompensationSet(void *parg);
__STATIC void frontOpAeFrameRateSet(void *parg);
__STATIC void frontOpAwbModeSet(void *parg);
__STATIC void frontOpAfterSnapShot(void *parg);

/* local function. */
static void frontBeforePowerOn(void);
static void frontAfterPowerOn(void);
static void frontBeforePowerOff(void);
static void frontAfterPowerOff(void);
static void frontSnapExposureTimeSet(void);
static void frontBeforeSnapExposureTimeSet(void);
static void frontResTblBinRead(UINT16 fileId,	UINT16 address);
static void frontResCmdBinSend(UINT16 fileId);

//static void frontColorNoiseReduce(UINT8 gain);

/**************************************************************************
 *                            G L O B A L   D A T A
 **************************************************************************/
/* device name. */
static code UINT8 frontDevName[] = "SENSOR - OV OV9655 YUV";

/**************************************************************************
 *		This segment is used to define the variables which should be convert to resource files
 *					CvtTypeToX: convert to "XDATA" table	
 *					CvtTypeToC: convert to "CODE" table		
 *					CvtTypeRmv: remove the specified table
 *					CvtTypeLnkAddr: convert to 
 **************************************************************************/
#define CVT_DEF  extern

/* Tag for convert start */
CVT_DEF UINT8 TAG_CVT_S;

/* ae table resource file id  */
typedef enum CvtTypeToX_u_0 {
	//_ae15fps50hzTbl = 0x00C0,
	//_ae15fps60hzTbl = 0x00C1,
	_ae20fps50hzTbl = 0x00C2,
	_ae20fps60hzTbl = 0x00C3,
	//_ae24fps50hzTbl = 0x00C4,
	//_ae24fps60hzTbl = 0x00C5,
	_ae30fps50hzTbl = 0x00C6,
	_ae30fps60hzTbl = 0x00C7,
	_ae60fps50hzTbl = 0x00C8,
	_ae60fps60hzTbl = 0x00C9,
} _aeTbl;

/* gain table resource file id */
typedef enum CvtTypeToX_e {
	_gainTbl = 0x00CA,
};

/* i2c initial cmd table resource file id */
typedef enum CvtTypeRmv_e {
	_frontInit0CmdTbl		= 0x00CB,
	_frontInit1CmdTbl		= 0x00CC,
	//_frontInit2CmdTbl		= 0x00CD,
	//_frontInit3CmdTbl		= 0x00CE,
	//_frontInit4CmdTbl		= 0x00CF,
	//_front15fpsPrevCmdTbl	= 0x00D0,
	//_front20fpsPrevCmdTbl	= 0x00D1,
	//_front24fpsPrevCmdTbl	= 0x00D2,
	_front30fpsPrevCmdTbl	= 0x00D3,
	//_front60fpsPrevCmdTbl	= 0x00D4,
	_frontSnapCmdTbl		= 0x00D5,
};

/* ae table & gain table link address */
typedef enum CvtTypeLnkAddr_e {
	__aeTbl = 0xa000,
	__gainTbl = 0xb000,
};

/* Tag for convert end */
CVT_DEF UINT8 TAG_CVT_E;


/* ae table. */
static code frontAe_t ae20fps50hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};
static code frontAe_t ae20fps60hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};

static code frontAe_t ae30fps50hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};

static code frontAe_t ae30fps60hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};                               

static code frontAe_t ae60fps60hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};
static code frontAe_t ae60fps50hzTbl[1] = {
	{    0,     0,  0, 0,  0 },
};


/* gain table. */
static code UINT16 gainTbl[1] = {
	0,
};

/* exposure time index table. */
static code UINT8 ae30fpsMaxIdxTbl[2] = {
	AE_30FPS_30_MAX_IDX,
	AE_30FPS_25_MAX_IDX,
};

static code UINT8 ae30fpsMinIdxTbl[2] = {
	AE_30FPS_30_MIN_IDX,
	AE_30FPS_25_MIN_IDX,
};

static code UINT8 ae60fpsMaxIdxTbl[2] = {
	AE_60FPS_60_MAX_IDX,
	AE_60FPS_50_MAX_IDX,
};

static code UINT8 ae60fpsMinIdxTbl[2] = {
	AE_60FPS_60_MIN_IDX,
	AE_60FPS_50_MIN_IDX,
};

/* device capability. */
static code frontPrevCapabDesc_t frontPrevCapabDesc[PREV_MODE_TOT] = {
	{  /* 30 fps. */
		PREVIEW_SRAM_MODE,
		BAYER_PTN_PREV,
		WIDTH_30FPS_PREV,
		HEIGHT_30FPS_PREV,
		XOFF_30FPS_PREV,
		YOFF_30FPS_PREV,
		LINE_TOT_30FPS_PREV,
		FRAME_TOT_30FPS_PREV,
		MCLK_DIV_30FPS_PREV,
		MCLK_PHASE_30FPS_PREV,
		PCLK_DIV_30FPS_PREV,
		PCLK_PHASE_30FPS_PREV,
		PCLK_FREQ_30FPS_PREV,
		BYPASS_HREF_EN,
		BYPASS_VREF_EN,
		RESHAPE_HEN_30FPS_PREV,
		RESHAPE_HFALL_30FPS_PREV,
		RESHAPE_HRISE_30FPS_PREV,
		RESHAPE_VEN_30FPS_PREV,
		RESHAPE_VFALL_30FPS_PREV,
		RESHAPE_VRISE_30FPS_PREV,
		ae30fps60hzTbl,
		ae30fps50hzTbl,
		ae30fpsMaxIdxTbl,
		ae30fpsMinIdxTbl,
		EV10_30FPS_EXP_IDX,
		gainTbl,
		GAIN_30FPS_MAX_IDX,
		GAIN_30FPS_MIN_IDX,
	},
	{  /* 60 fps. */
		PREVIEW_SRAM_MODE,
		BAYER_PTN_PREV,
		WIDTH_60FPS_PREV,
		HEIGHT_60FPS_PREV,
		XOFF_60FPS_PREV,
		YOFF_60FPS_PREV,
		LINE_TOT_60FPS_PREV,
		FRAME_TOT_60FPS_PREV,
		MCLK_DIV_60FPS_PREV,
		MCLK_PHASE_60FPS_PREV,
		PCLK_DIV_60FPS_PREV,
		PCLK_PHASE_60FPS_PREV,
		PCLK_FREQ_60FPS_PREV,
		BYPASS_HREF_EN,
		BYPASS_VREF_EN,
		RESHAPE_HEN_60FPS_PREV,
		RESHAPE_HFALL_60FPS_PREV,
		RESHAPE_HRISE_60FPS_PREV,
		RESHAPE_VEN_60FPS_PREV,
		RESHAPE_VFALL_60FPS_PREV,
		RESHAPE_VRISE_60FPS_PREV,
		ae60fps60hzTbl,
		ae60fps50hzTbl,
		ae60fpsMaxIdxTbl,
		ae60fpsMinIdxTbl,
		EV10_60FPS_EXP_IDX,
		gainTbl,
		GAIN_60FPS_MAX_IDX,
		GAIN_60FPS_MIN_IDX,
	},
};

static code frontSnapCapabDesc_t frontSnapCapabDesc[SNAP_MODE_TOT] = {
	{  /* 30/60 fps. */
		BAYER_PTN_SNAP,
		WIDTH_SNAP,
		HEIGHT_SNAP,
		XOFF_SNAP,
		YOFF_SNAP,
		LINE_TOT_SNAP,
		FRAME_TOT_SNAP,
		MCLK_DIV_SNAP,
		MCLK_DIV_SNAP_BURST,
		MCLK_DIV_SNAP_BURST_UP,
		MCLK_PHASE_SNAP,
		PCLK_DIV_SNAP,
		PCLK_PHASE_SNAP,
		PCLK_FREQ_SNAP,
		PCLK_FREQ_SNAP_BURST,
		PCLK_FREQ_SNAP_BURST_UP,
		BYPASS_HREF_EN,
		BYPASS_VREF_EN,
		RESHAPE_HEN_SNAP,
		RESHAPE_HFALL_SNAP,
		RESHAPE_HRISE_SNAP,
		RESHAPE_VEN_SNAP,
		RESHAPE_VFALL_SNAP,
		RESHAPE_VRISE_SNAP,
	},
};

/* i2c command table. */
static code UINT8 frontInit0CmdTbl[] = {
	0x12, 0x80,
};

static code UINT8 frontInit1CmdTbl[] = {
	0x00, 0x00,
	0x01, 0x80,
	0x02, 0x80,
	0x03, 0x00, //0x12,
	0x04, 0x03,
	0x0b, 0x57,
	0x0e, 0x61,
	0x0f, 0x43, //0x42,

	0x11, 0x00, //0x80,
	0x12, 0x62,
	0x13, 0xe7,
	0x14, 0x3e, //0x4e,
	0x15, 0x44, //0x04,
	0x16, 0x14, //0x24, //0x04,
	0x17, 0x16,
	0x18, 0x02,
	0x19, 0x01,
	0x1a, 0x3d,
	0x1e, 0x34,

	0x24, 0x3c,
	0x25, 0x36,
	0x26, 0x92, //0x82, //0xf2, //0x72,
	0x27, 0x08,
	0x28, 0x08,
	0x29, 0x00,
	0x2a, 0x00,
	0x2b, 0x04, //0x00,
	0x2c, 0x08,

	0x32, 0x80, //0xff,
	0x33, 0x00,
	0x34, 0x3F,
	0x35, 0x00,
	0x36, 0xfa,
	0x37, 0x08,
	0x38, 0x72,
	0x39, 0x57,
	0x3a, 0x80, //0x82, //0x80,
	0x3b, 0x44,
	0x3d, 0x99,
	0x3e, 0x0c,
	0x3f, 0xc1,

	0x40, 0xc0,
	0x41, 0x00,
	0x42, 0xd1, //0xc0,
	0x43, 0x0a,
	0x44, 0xf0,
	0x45, 0x46,
	0x46, 0x62,
	0x47, 0x2a,
	0x48, 0x3c,
	0x4a, 0xfc,
	0x4b, 0xfc,
	0x4c, 0x7f,
	0x4d, 0x7f,
	0x4e, 0x7f,
	#if 0 // OV default
	0x4f, 0x98,

	0x50, 0x98,
	0x51, 0x00,
	0x52, 0x28,
	0x53, 0x88,
	0x54, 0xb0,
	0x58, 0x1a,
	#endif
	#if 1 // 0.7x saturation
	0x4f, 0x6a,

	0x50, 0x6a,
	0x51, 0x00,
	0x52, 0x1c,
	0x53, 0x5f,
	0x54, 0x7b,
	0x58, 0x1a,
	#endif
	0x59, 0x85,
	0x5a, 0xa9,
	0x5b, 0x64,
	0x5c, 0x84,
	0x5d, 0x53,
	0x5e, 0x0e,
	0x5f, 0xf0,

	0x60, 0xf0,
	0x61, 0xf0,
	0x62, 0x00,
	0x63, 0x00,
	0x64, 0x04,
	0x65, 0x20,
	0x66, 0x00,
	0x69, 0x0a,
	0x6a, 0x02, //0x01,
	0x6b, 0x0a,//0x1a,//change by ml 20110316
	0x6c, 0x04,
	0x6d, 0x55,
	0x6e, 0x00,
	0x6f, 0x9d,

	0x70, 0x21,
	0x71, 0x78,
	0x72, 0xcc, //0x00,
	0x73, 0x00,
	0x74, 0x3a,
	0x75, 0x35,
	0x76, 0x01,
	0x77, 0x02,
	#if 0 //OV default
	0x7a, 0x12,
	0x7b, 0x08,
	0x7c, 0x15,
	0x7d, 0x24,
	0x7e, 0x45,
	0x7f, 0x55,

	0x80, 0x6a,
	0x81, 0x78,
	0x82, 0x87,
	0x83, 0x96,
	0x84, 0xa3,
	0x85, 0xb4,
	0x86, 0xc3,
	0x87, 0xd6,
	0x88, 0xe6,
	0x89, 0xf2,
	#endif
	#if 1 //Lancer 2
	0x7a, 0x0c,
	0x7b, 0x10,
	0x7c, 0x1e,
	0x7d, 0x34,
	0x7e, 0x5d,
	0x7f, 0x6e,

	0x80, 0x7f,
	0x81, 0x8e,
	0x82, 0x9b,
	0x83, 0xa5,
	0x84, 0xaf,
	0x85, 0xc0,
	0x86, 0xcc,
	0x87, 0xde,
	0x88, 0xee,
	0x89, 0xf7,
	#endif
	#if 0 //Lancer 3
	0x7a, 0x0c,
	0x7b, 0x08,
	0x7c, 0x15,
	0x7d, 0x30,
	0x7e, 0x64,
	0x7f, 0x78,

	0x80, 0x89,
	0x81, 0x98,
	0x82, 0xa5,
	0x83, 0xb0,
	0x84, 0xba,
	0x85, 0xca,
	0x86, 0xd6,
	0x87, 0xe6,
	0x88, 0xf0,
	0x89, 0xf7,
	#endif
	#if 0 //OV 2
	0x7a, 0x20,
	0x7b, 0x09,
	0x7c, 0x18,
	0x7d, 0x30,
	0x7e, 0x58,
	0x7f, 0x66,

	0x80, 0x72,
	0x81, 0x7d,
	0x82, 0x86,
	0x83, 0x8f,
	0x84, 0x97,
	0x85, 0xa5,
	0x86, 0xb2,
	0x87, 0xc7,
	0x88, 0xd8,
	0x89, 0xe8,
	#endif

	0x8a, 0x03,
	0x8c, 0x8d,

	0x90, 0x7d,
	0x91, 0x7b,
	0x9d, 0x02,
	0x9e, 0x02,
	0x9f, 0x7a,

	0xa0, 0x79,
	0xa1, 0x40,
	0xa2, 0x96, //50Hz Fliker for 30FPS
	0xa3, 0x7d, //60Hz Fliker for 30FPS
	0xa4, 0x50,
	0xa5, 0x68,
	0xa6, 0x4a,
	0xa8, 0xc1,
	0xa9, 0xef,
	0xaa, 0x92,
	0xab, 0x04,
	0xac, 0x80,
	0xad, 0x80,
	0xae, 0x80,
	0xaf, 0x80,

	0xb2, 0xf2,
	0xb3, 0x20,
	0xb4, 0x03, //0x00,
	0xb5, 0x00,
	0xb6, 0xaf,
	0xbb, 0xae,
	0xbc, 0x7f,
	0xbd, 0x7f,
	0xbe, 0x7f,
	0xbf, 0x7f,

	0xc0, 0xaa,
	0xc1, 0xc0,
	0xc2, 0x01,
	0xc3, 0x4e,
	0xc5, 0x02, //0x01,
	0xc6, 0x05,
	0xc7, 0x80,
	0xc9, 0xe0,
	0xca, 0xe8,
	0xcb, 0xf0,
	0xcc, 0xd8,
	0xcd, 0x93
};

static code UINT8 front30fpsPrevCmdTbl[] = {
#if 0
	0xc1, 0x00,
	0x49, 0x48,
//	0x8e, 0xe0,
	0x39, 0x57,
	0x09, 0x03,
	0xc1, 0xc0,
#endif

	0x03, 0x00, //0x12,
//	0x11, 0x01, //0x83,
	0x12, 0x62,
	0x17, 0x17,
	0x18, 0x03,
	0x1a, 0x3d,
	0x2a, 0x00,
	0x2b, 0x0A, //0x00,
	0x32, 0x80, //0x92, //0xa4,
	0x34, 0x3F,
	0x36, 0xfa,
	0x65, 0x00, //0x20,
	0x66, 0x01, //0x00,
	0x69, 0x0a,
	0x73, 0x00,
	0x8c, 0x89,
	0x9d, 0x03, //0x02,
	0x9e, 0x04, //0x02,
	0xc0, 0xaa,
	0x14, 0x3e, //0x4e,
	0x13, 0xe7,
	/* color bar test pattern*/
	//0x8d, 0x10,  //gray level
};

static code UINT8 frontSnapCmdTbl[] = {
	0x13, 0xe0,
	0x14, 0x3f, //0x4f,
	0x03, 0x12,
//	0x11, 0x00, //0x81,
	0x12, 0x02,
	0x17, 0x1e,
	0x18, 0xbe,
	0x1a, 0x81,
	0x2a, 0x00,
	0x2b, 0x64, //0x00,
	0x32, 0x80,
	0x34, 0x3d,
	0x36, 0xf8,
	0x65, 0x00, //0x16,
	0x66, 0x01, //0x01,
	0x69, 0x02,
	0x73, 0x01,
	0x8c, 0x09,
	0x9d, 0x03, //0x03,
	0x9e, 0x04, //0x04,
	0xc0, 0xe2,
};

static code UINT8 frontSuspendCmdTbl[] = {
	0x13, 0xe0,
	0x6b, 0x4a,
	0x39, 0x5f,
	0x38, 0x72,
	0x10, 0x00,
	0xa1, 0x00,
	0x04, 0x00,
	0x00, 0x00,
	0xc1, 0xc0,
	0x49, 0x08,
//	0x8e, 0xe0,
	0x09, 0x11,
	0x09, 0x01,
	0x09, 0x11
};

static code UINT8 frontResumeCmdTbl[] = {
	0xc1, 0x00,
	0x49, 0x48,
//	0x8e, 0xe0,
	0x39, 0x57,
	0x09, 0x03,
	0xc1, 0xc0,
};

static UINT8 frontPrevMode = 0;
static UINT8 frontSnapMode = 0;

static xdata UINT16 G_ExpLine = 250;
static xdata UINT16 G_ExpGain = 1;

static UINT16 frontLastGain;
static UINT16 frontLastGainIdx;
static UINT32 frontLastShutter;
static UINT16 frontSnapLastGain;
static UINT32 frontSnapLastShutter;

static UINT8 LoadBackfrontLastGainIdx;
static UINT8 LoadBackfrontLastSetting = 0;

static UINT8 frontPrevModeSave = 0;	/* resolve ov5653 sensor different resolution & fps switch issue used */

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S
 **************************************************************************/
  
/**
 * @brief	open device (register operating function).
 * @param	None.
 * @retval	return = NULL or device id.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
UINT8
frontOpen(
	void
)
{
	#if (REGISTER_EXTERN_OP_FUNC == 1)
	UINT8 id;
	#endif
	frontOperatingFunc_t opFunc;


	opFunc.expGainSetPos             = EXP_GAIN_SET_POS;
	opFunc.gainAfterExp              = GAIN_AFTER_EXP;
	opFunc.pfOpDevNameGet            = frontOpDevNameGet;
	opFunc.pfOpFrameRateSet          = frontOpFrameRateSet;
	opFunc.pfOpCapabilityGet         = frontOpCapabilityGet;
	opFunc.pfOpIntrCapabilityGet     = frontOpIntrCapabilityGet;
	opFunc.pfOpPowerOn               = frontOpPowerOn;
	opFunc.pfOpPowerOff              = frontOpPowerOff;
	opFunc.pfOpPreviewModeSet        = frontOpPreviewModeSet;
	opFunc.pfOpSnapModeSet           = frontOpSnapModeSet;
	opFunc.pfOpSnapShot              = frontOpSnapShot;
	opFunc.pfOpGainSet               = frontOpGainSet;
	opFunc.pfOpExposureTimeSet       = frontOpExposureTimeSet;
	opFunc.pfOpIntrGainSet           = frontOpIntrGainSet;
	opFunc.pfOpIntrExposureTimeSet   = frontOpIntrExposureTimeSet;
	opFunc.pfOpOpticalBlackStatusGet = frontOpOpticalBlackStatusGet;
	opFunc.pfOpOpticalBlackCalibrate = frontOpOpticalBlackCalibrate;
	opFunc.pfOpShutterOpen           = frontOpShutterOpen;
	opFunc.pfOpShutterClose          = frontOpShutterClose;

	/* load gain tbl to static xdata UINT16 gainTbl[48] _at_ 0xb000; */
	frontResTblBinRead(_gainTbl, __gainTbl);

	#if (REGISTER_EXTERN_OP_FUNC == 1)
	/* register extern operating function */
	id = frontDevOpen(&opFunc, sizeof(frontOperatingFunc_t));

	if ( id != NULL ) {
		frontExtendOpFuncSet(id, FRONT_EXT_OP_AE_TARGET_LUMINANCE_SET, frontOpAeTargetLuminanceSet);
		frontExtendOpFuncSet(id, FRONT_EXT_OP_AE_EXPOSURE_COMPENSATION_SET, frontOpAeExposureCompensationSet);
		frontExtendOpFuncSet(id, FRONT_EXT_OP_AE_FRAME_RATE_SET, frontOpAeFrameRateSet);
		frontExtendOpFuncSet(id, FRONT_EXT_OP_AWB_MODE_SET, frontOpAwbModeSet);
		frontExtendOpFuncSet(id, FRONT_EXT_OP_AFTER_SNAP_SHOT, frontOpAfterSnapShot);
	}

	return id;
	#else
	return frontDevOpen(&opFunc, sizeof(frontOperatingFunc_t));
	#endif
}

/**
 * @brief	operating function to get device name (ascii string).
 * @param	None.
 * @retval	return = device name string.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC UINT8 *
frontOpDevNameGet(
	void
)
{
	return frontDevName;
}

/**
 * @brief	operating function to set frame rate, called before further operating.
 * @param   fps = [in] frame rate.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-10-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpFrameRateSet(
	UINT8 fps
)
{
	switch ( fps ) {
	case 30:
		//frontPrevMode = 0;
		//frontSnapMode = 0;
		break;

	case 60:
		//frontPrevMode = 0;
		//frontSnapMode = 1;
		break;
	}
}

/**
 * @brief	operating function to get device capability.
 * @param   *(parg->ppprevCap) = [out] pointer to device preview capability.
 * @param   *(parg->ppsnapCap) = [out] pointer to device snap capability.
 * @param   descSize = [in] describer structure size.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpCapabilityGet(
	frontCapabilityArg_t *parg
)
{
	if ( parg->ppprevCap ) {
		*(parg->ppprevCap) = &frontPrevCapabDesc[frontPrevMode];
	}

	if ( parg->ppsnapCap ) {
		*(parg->ppsnapCap) = &frontSnapCapabDesc[frontSnapMode];
	}
}

/**
 * @brief	operating function to get device capability.
 * @param   *(parg->ppprevCap) = [out] pointer to device preview capability.
 * @param   *(parg->ppsnapCap) = [out] pointer to device snap capability.
 * @param   descSize = [in] describer structure size.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-09-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpIntrCapabilityGet(
	frontCapabilityArg_t *parg
)
{
	if ( parg->ppprevCap ) {
		*(parg->ppprevCap) = &frontPrevCapabDesc[frontPrevMode];
	}

	if ( parg->ppsnapCap ) {
		*(parg->ppsnapCap) = &frontSnapCapabDesc[frontSnapMode];
	}
}


/**
 * @brief	operating function to power on device.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpPowerOn(
	void
)
{      

	frontPrevCapabDesc_t *pcap;
	//UINT8 addr[1];
	//UINT8 datatbl[1];
	/* before power on. */
	frontBeforePowerOn();

	pcap = &(frontPrevCapabDesc[frontPrevMode]);

	/* set gpio. */
	gpioByteFuncSet(GPIO_BYTE_TG0, 0xFF, GPIO_FUNC);
	gpioByteDirSet(GPIO_BYTE_TG0, 0xFF, GPIO_DIR);
	gpioByteInGateSet(GPIO_BYTE_TG0, 0xFF, GPIO_GATE);
	gpioByteOutSet(GPIO_BYTE_TG0, 0xFF, GPIO_OUT);

	/* set clk. */
	frontParaSet(FRONT_PARA_MCLK_CFG, MCLK_SRC, pcap->mclkDiv, pcap->mclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_PCLK_CFG, PCLK_SRC, pcap->pclkDiv, pcap->pclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_SYNC_INVERT_ENABLE, HSYNC_INV_PREV, VSYNC_INV_PREV, 0, 0, 0);

	/* set i2c. */
	i2cDeviceAddrSet(I2C_DEV_ADDR);
	i2cClkCfgSet(I2C_CLK_DIV);

	/* set yuv interface. */
	frontParaSet(FRONT_PARA_YUV_OUTPUT_SEQUENCE_CFG, YUV_SEQ, 0, 0, 0, 0);
	frontParaSet(FRONT_PARA_YUV_CCIR_MODE, CCIR_MODE, 0, 0, 0, 0);
	frontParaSet(FRONT_PARA_YUV_SUB128_ENABLE, Y_SUB_128, U_SUB_128, V_SUB_128, 0, 0);
	frontParaSet(FRONT_PARA_YUV_INPUT_ENABLE, 1, 0, 0, 0, 0);

	/* sensor reset complete */
	globalTimerWait(1);
	//sp1kExpstbSet(1);

	/* send i2c command. */
	
	//i2cCmdTableSend(frontInit0CmdTbl, 1, sizeof(frontInit0CmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
	frontResCmdBinSend(_frontInit0CmdTbl);
	
	globalTimerWait(10);
	
	//i2cCmdTableSend(frontInit1CmdTbl, 1, sizeof(frontInit1CmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
	frontResCmdBinSend(_frontInit1CmdTbl);
	//addr[0] = 0x59;
	//i2cRegRead(addr,datatbl,1,1,1,0,0);
	//printf("addr[0]=0x%02bx,datatbl[0]=0x%02bx\n",addr[0],datatbl[0]);
	/* after power on. */
	frontAfterPowerOn();

}

/**
 * @brief	operating function to power off device.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpPowerOff(
	void
)
{
	/* before power off. */
	frontBeforePowerOff();

	/* after power off. */
	frontAfterPowerOff();
}

/**
 * @brief	operating function to set preview mode.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpPreviewModeSet(
	void
)
{
	frontPrevCapabDesc_t *pcap;


	pcap = &(frontPrevCapabDesc[frontPrevMode]);

	frontSignalWait(FRONT_WAIT_VSYNC, FRONT_WAIT_FALLING, 1);
	frontParaSet(FRONT_PARA_MCLK_CFG, MCLK_SRC, pcap->mclkDiv, pcap->mclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_PCLK_CFG, PCLK_SRC, pcap->pclkDiv, pcap->pclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_SYNC_INVERT_ENABLE, HSYNC_INV_PREV, VSYNC_INV_PREV, 0, 0, 0);
	frontParaSet(FRONT_PARA_BYPASS_ENABLE, pcap->bypassHref, pcap->bypassVref, 0, 0, 0);
	frontParaSet(FRONT_PARA_RESHAPE_ENABLE, pcap->reshapeHen, pcap->reshapeVen, 0, 0, 0);
	frontParaSet(FRONT_PARA_RESHAPE_REGION, pcap->reshapeHfall, pcap->reshapeHrise, pcap->reshapeVfall, pcap->reshapeVrise, 0);
	frontParaSet(FRONT_PARA_CROP_REGION, pcap->width, pcap->height, pcap->xoff, pcap->yoff, 0);

	//i2cCmdTableSend(front30fpsPrevCmdTbl, 1, sizeof(front30fpsPrevCmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
	frontResCmdBinSend(_front30fpsPrevCmdTbl);
	
	cdspBayerPatternSet(BAYER_PTN_PREV);
}

/**
 * @brief	operating function to set snap mode.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void frontSnapExposureTimeGet(void);

__STATIC void
frontOpSnapModeSet(
	void
)
{
	frontSnapCapabDesc_t *pcap;

	pcap = &(frontSnapCapabDesc[frontSnapMode]);

	frontSnapExposureTimeGet();

	frontParaSet(FRONT_PARA_MCLK_CFG, MCLK_SRC, pcap->mclkDiv, pcap->mclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_PCLK_CFG, PCLK_SRC, pcap->pclkDiv, pcap->pclkPhase, 0, 0);
	frontParaSet(FRONT_PARA_SYNC_INVERT_ENABLE, HSYNC_INV_SNAP, VSYNC_INV_SNAP, 0, 0, 0);
	frontParaSet(FRONT_PARA_BYPASS_ENABLE, pcap->bypassHref, pcap->bypassVref, 0, 0, 0);
	frontParaSet(FRONT_PARA_RESHAPE_ENABLE, pcap->reshapeHen, pcap->reshapeVen, 0, 0, 0);
	frontParaSet(FRONT_PARA_RESHAPE_REGION, pcap->reshapeHfall, pcap->reshapeHrise, pcap->reshapeVfall, pcap->reshapeVrise, 0);
	frontParaSet(FRONT_PARA_CROP_REGION, pcap->width, pcap->height, pcap->xoff, pcap->yoff, 0);

	//i2cCmdTableSend(frontSnapCmdTbl, 1, sizeof(frontSnapCmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
	frontResCmdBinSend(_frontSnapCmdTbl);

	frontSnapExposureTimeSet();

	cdspBayerPatternSet(BAYER_PTN_SNAP);

	frontSignalWait(FRONT_WAIT_VSYNC, FRONT_WAIT_FALLING, 2);

	//while(1);
}

/**
 * @brief	operating function to snap shot.
 * @param	first = [in] 0: not first, 1: first shot for burst shot.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpSnapShot(
	UINT8 first,
	UINT8 mode,
	UINT8 scaleUp
)
{
	first = first;
	mode = mode;
	scaleUp = scaleUp;
	
	frontSignalWait(FRONT_WAIT_VSYNC, FRONT_WAIT_FALLING, 1); // 20081024 mantis 32544 wenguo
	cdspSkipWrSet(CDSP_SKIP_AWBAEEDGE,0xff);
	cdspSkipWrSyncSet(1);
	frontSignalWait(FRONT_WAIT_VSYNC, FRONT_WAIT_FALLING, 1);
	frontSignalWait(FRONT_WAIT_VSYNC, FRONT_WAIT_RISING, 1);
	frontParaSet(FRONT_PARA_FRONT_TO_CDSP_GATING_ENABLE, 1, 0, 0, 0, 1);

	HAL_CdspEofWait(1);
}

/**
 * @brief	operating function to set gain value.
 * @param	parg->val = [in] gain value.
 * @param	parg->isr = [in] called by 0: regular function, 1: isr.
 * @param	parg->upd = [in] 0: immediately update, 1: sync to vsync falling.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpGainSet(
	UINT8 val,
	UINT8 isr,
	UINT8 upd
)
{
	val = val;
	isr = isr;
	upd = upd;
}

/**
 * @brief	operating function to set exposure time value.
 * @param	parg->val = [in] exposure time value.
 * @param	parg->isr = [in] called by 0: regular function, 1: isr.
 * @param	parg->upd = [in] 0: immediately update, 1: sync to vsync falling.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpExposureTimeSet(
	frontExposureTimeArg_t *parg
)
{
	parg = parg;
}

/**
 * @brief	operating function to set gain value for intr.
 * @param	parg->val = [in] gain value.
 * @param	parg->isr = [in] called by 0: regular function, 1: isr.
 * @param	parg->upd = [in] 0: immediately update, 1: sync to vsync falling.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-09-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpIntrGainSet(
	UINT8 val,
	UINT8 isr,
	UINT8 upd
)
{
	val = val;
	isr = isr;
	upd = upd;

}

/**
 * @brief	operating function to set exposure time value for intr.
 * @param	parg->val = [in] exposure time value.
 * @param	parg->isr = [in] called by 0: regular function, 1: isr.
 * @param	parg->upd = [in] 0: immediately update, 1: sync to vsync falling.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-09-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpIntrExposureTimeSet(
	frontExposureTimeArg_t *parg
)
{
	parg = parg;
}

/**
 * @brief	operating function to get optical black status.
 * @param	None.
 * @retval	return = SUCCESS or FAIL.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC UINT8
frontOpOpticalBlackStatusGet(
	void
)
{

	return SUCCESS;
}

/**
 * @brief	operating function to calibrate optical black.
 * @param	None.
 * @retval	return = SUCCESS or FAIL.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC UINT8
frontOpOpticalBlackCalibrate(
	void
)
{

	return SUCCESS;
}

/**
 * @brief	operating function to open shutter
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpShutterOpen(
	void
)
{

}

/**
 * @brief	operating function to close shutter
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpShutterClose(
	void
)
{

}

/**
 * @brief	operating function to set ae target luminance.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-27
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpAeTargetLuminanceSet(
	void *parg
)
{
	//printf("frontSensorOpAeTargetLuminanceSet=%bu\n",(UINT8)parg);
	// NOTE: use variable "val" for your purpose.
}

/**
 * @brief	operating function to set ae exposure compensation.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-26
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpAeExposureCompensationSet(
	void *parg
)
{	
#if 0
	UINT8 val;
#if 1
	UINT8 high_level, thr_level, low_level;
	UINT8 cmdTbl[2*3];
	static code SINT8 ae_comp[41] = {
#if 1
		  0,
		  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
#else
		-90 ,	-88 ,	-84 ,	-81 ,	-78 ,
		-74 ,	-69 ,	-66 ,	-63 ,	-58 ,
		-53 ,	-48 ,	-43 ,	-39 ,	-33 ,
		-27 ,	-21 ,	-15 ,	-9	,	-4	,
		0	,
		3	,	8	,	17	,	30	,	36	,
		41	,	47	,	53	,	59	,	64	,
		70	,	76	,	82	,	87	,	93	,
		99	,	105 ,	110 ,	115 ,	120 ,
#endif
	};

	UINT8 addrTbl[3], dataTbl[3];

	addrTbl[0] = 0x24; dataTbl[0] = 0;
	addrTbl[1] = 0x25; dataTbl[1] = 0;
	addrTbl[2] = 0x26; dataTbl[2] = 0;
	i2cRegRead(addrTbl, dataTbl, 1, 3, 1, 0, 0);
	printf("high = 0x%bx, low =0x%bx, thr = 0x%bx \n", dataTbl[0],dataTbl[1],dataTbl[2]);

#endif

	val = (UINT8)parg;

#if 1
	if (val > 40) val = 20;

	high_level = 0x70 + ae_comp[val]*2 + 8 - 40*2;
	low_level = 0x70 + ae_comp[val]*2 - 8 - 40*2;
	//thr_level = ((high_level / 10 + 1) & 0x0F) * 16 + ((low_level / 10 - 1) & 0x0F) ;
	thr_level = ((5 + ae_comp[val]* 2 / 10) & 0x0F) * 16 + ((ae_comp[val] / 10 ) & 0x0F) ;

	cmdTbl[0] = 0x24;
	cmdTbl[1] = high_level;
	cmdTbl[2] = 0x25;
	cmdTbl[3] = low_level;
	cmdTbl[4] = 0x26;
	cmdTbl[5] = thr_level;

	i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
#endif
#endif
	//printf("frontOpAeExposureCompensationSet=%bu\n", (UINT8)parg);
	// NOTE: use variable "val" for your purpose.
}

/**
 * @brief	operating function to set ae frame rate.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-27
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpAeFrameRateSet(
	void *parg
)
{
#if 1//0
	UINT16 val;
	UINT8 oldmode, mode;
	UINT8 addrTbl[1], dataTbl[1], cmdTbl[1*2];

	val = (UINT16)parg;

	if((val == SP1K_FRAME_RATE_30) || (val == SP1K_FRAME_RATE_15)) mode = 1;
	if((val == SP1K_FRAME_RATE_25) || (val == SP1K_FRAME_RATE_12P5)) mode = 0;

	addrTbl[0] = 0x42; dataTbl[0] = 0;
	i2cRegRead(addrTbl, dataTbl, 1, 1, 1, 0, 0);

	oldmode = (dataTbl[0] & 0x01);

	if ((mode == 0) && (oldmode == 0)){/*50HZ*/
		dataTbl[0] |= 0x01;
		cmdTbl[0] = 0x42; cmdTbl[1] = dataTbl[0];
		i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
		//printf("50HZ\n");
	}
	else if ((mode == 1) && (oldmode == 1)){/*60HZ*/
		dataTbl[0] &= 0xfe;
		cmdTbl[0] = 0x42; cmdTbl[1] = dataTbl[0];
		i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
		//printf("60HZ\n");
	}
#endif
	//printf("frontOpAeFrameRateSet=%d\n", (UINT16)parg);
	// NOTE: use variable "val" for your purpose.
}

/**
 * @brief	operating function to set awb mode.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-27
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpAwbModeSet(
	void *parg
)
{
#if 0
	UINT8 mode;
	UINT8 rgain, bgain, reg13, oldmode, modeChEn;
	UINT8 addrTbl[2], dataTbl[2], cmdTbl[2*3];

	mode = (UINT8)parg;

	addrTbl[0] = 0x13; dataTbl[0] = 0;
	i2cRegRead(addrTbl, dataTbl, 1, 1, 1, 0, 0);
	reg13 = dataTbl[0];

	addrTbl[0] = 0x01; dataTbl[0] = 0;
	addrTbl[1] = 0x02; dataTbl[1] = 0;
	i2cRegRead(addrTbl, dataTbl, 1, 2, 1, 0, 0);
	//printf("AWB Enable = %bx, bgain = %bu, rgain = %bu\n", reg13&0x02, dataTbl[0], dataTbl[1]);

	oldmode = (reg13 & 0x02);
	modeChEn = 1;

#if 0 // for AWB calibration
	mode = SP1K_AWB_MODE_AUTO;
	oldmode = 0;
	printf("AWB Enable = %bx, bgain = %bu, rgain = %bu\n", reg13&0x02, dataTbl[0], dataTbl[1]);
#endif

	switch(mode){
		default:
		case SP1K_AWB_MODE_AUTO:
			if (oldmode != 0)
				modeChEn = 0;
			reg13 |= 0x02;
			bgain = 0x80;
			rgain = 0x80;
			break;

		case SP1K_AWB_MODE_DAYLIGHT:
			reg13 &= 0xfd;
			bgain = 0x80;
			rgain = 0x80;
			break;

		case SP1K_AWB_MODE_CLOUDY:
			reg13 &= 0xfd;
			bgain = 0x80;
			rgain = 0x90;
			break;

		case SP1K_AWB_MODE_TUNGSTENG:
			reg13 &= 0xfd;
			bgain = 0xc8;
			rgain = 0x80;
			break;

		case SP1K_AWB_MODE_FLUORESCENT:
			reg13 &= 0xfd;
			bgain = 0x98;
			rgain = 0xb3;
			break;
	}

	if (modeChEn){
		if(mode == SP1K_AWB_MODE_AUTO) {
			cmdTbl[0] = 0x13;cmdTbl[1] = reg13;
			i2cCmdTableSend(cmdTbl, 1, 1, I2C_TRANS_MODE_NORMAL, 0, 0);
		}
		else {
			cmdTbl[0] = 0x01;cmdTbl[1] = bgain;
			cmdTbl[2] = 0x02;cmdTbl[3] = rgain;
			cmdTbl[4] = 0x13;cmdTbl[5] = reg13;
			i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
		}
	}
#endif
	//printf("frontOpAwbModeSet=%bu\n", (UINT8)parg);
	// NOTE: use variable "mode" for your purpose.
}

/**
 * @brief	after snap shot.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Wen-Guo Gan
 * @since	2009-01-09
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontOpAfterSnapShot(
	void *parg
)
{
	//printf("frontOpAfterSnapShot=%bu\n", (UINT8)parg);
	// NOTE: use variable "mode" for your purpose.
}

/**
 * @brief	before power on.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-11
 * @todo	N/A
 * @bug		N/A
*/
static void
frontBeforePowerOn(
	void
)
{
#if (_HW_SOLUTION_ == _HW_RDK_A_) 
	/* sensor reset (fmgpio 10) */
	gpioByteFuncSet(GPIO_BYTE_FM1, 0x04, 0xFF);
	gpioByteDirSet(GPIO_BYTE_FM1, 0x04, 0xFF);
	gpioByteOutSet(GPIO_BYTE_FM1, 0x04, 0x00);
	sp1kHalWait(5);
	gpioByteOutSet(GPIO_BYTE_FM1, 0x04, 0xFF);
#endif	
}

/**
 * @brief	after power on.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-11
 * @todo	N/A
 * @bug		N/A
*/
static void
frontAfterPowerOn(
	void
)
{
}

/**
 * @brief	before power off.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-11
 * @todo	N/A
 * @bug		N/A
*/
static void
frontBeforePowerOff(
	void
)
{

}

/**
 * @brief	after power off.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-11
 * @todo	N/A
 * @bug		N/A
*/
static void
frontAfterPowerOff(
	void
)
{

}

/**
 * @brief	set snap exposure time.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontSnapExposureTimeGet(
	void
)
{
	UINT8 addrTbl[4], dataTbl[4];

	addrTbl[0] = 0xA1; dataTbl[0] = 0;
	addrTbl[1] = 0x10; dataTbl[1] = 0;
	addrTbl[2] = 0x04; dataTbl[2] = 0;
	addrTbl[3] = 0x00; dataTbl[3] = 0;
	i2cRegRead(addrTbl, dataTbl, 1, 4, 1, 0, 0);
	G_ExpLine = (UINT16) (dataTbl[0] & 0x3F) * 1024 + (UINT16)dataTbl[1] * 4 + (UINT16)(dataTbl[2] & 0x03);
	G_ExpLine = G_ExpLine / 2;//G_ExpLine / 4;//change by ml 20110316
	G_ExpGain = dataTbl[3];
	if((G_ExpGain & 0x80) != 0) G_ExpLine *= 2;
	if((G_ExpGain & 0x40) != 0) G_ExpLine *= 2;
	if((G_ExpGain & 0x20) != 0) G_ExpLine *= 2;
	G_ExpGain &= 0x1F;

	if(G_ExpLine == 0) G_ExpLine = 1;
}

__STATIC void
frontSnapExposureTimeSet(
	void
)
{
	UINT8 cmdTbl[2*4], addrTbl[4], dataTbl[4];

	addrTbl[0] = 0xA1;
	addrTbl[1] = 0x10;//0xA1;//change by ml 20110316
	addrTbl[2] = 0x04;//0x10;//change by ml 20110316
	addrTbl[3] = 0x00;//0x04;//change by ml 20110316
	i2cRegRead(addrTbl, dataTbl, 1, 4, 1, 0, 0);

	cmdTbl[0] = 0xA1;
	cmdTbl[1] = ((G_ExpLine>>10)&0x3f)|(dataTbl[0]&0xc0);//(G_ExpLine>>10)|(dataTbl[1]&0xc0);//change by ml 20110316
	cmdTbl[2] = 0x10;
	cmdTbl[3] = (G_ExpLine>>2)&0xff;
	cmdTbl[4] = 0x04;
	cmdTbl[5] = (G_ExpLine&0x03)|(dataTbl[2]&0xfc);
	cmdTbl[6] = 0x00;
	cmdTbl[7] = G_ExpGain;

	i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
}

/**
 * @fn        void frontResCmdBinSend(UINT16 fileId)
 * @brief	front Resource File Table Cmd Send.
 * @param	fileId = [in] resource file id...
 * @retval	None.
 * @see
 * @author	LinJieCheng
 * @since	2010-12-07
 * @todo		N/A
 * @bug		N/A
*/
static void
frontResCmdBinSend(
	UINT16 fileId
)
{
	UINT32 size;
	UINT8 sts;
	//UINT32 i;
	
	//printf("fileId = 0x%bx\n",fileId);
	sp1kNandRsvSizeGet(fileId, NULL, &size); // Res_0x2.res = calLen.bin
	if (size > FRONT_RES_CMD_MAX_SIZE) {
		ASSERT(size > FRONT_RES_CMD_MAX_SIZE, 0); 
		return;
	}

	//printf("size = %lu\n",size);

	do {
		sts = sp1kNandRsvRead(fileId, ((K_SDRAM_CodeSize+(((UINT16)G_FPGAMemPool - 0x4000)>>1))<<1));
	}while(!sts);

	//for (i = 0; i < size; i++) {
	//	printf("0x%02bx,",G_FPGAMemPool[i]);
	//}

	i2cCmdTableSend(G_FPGAMemPool, 1, size/2, I2C_TRANS_MODE_NORMAL, 0, 0);
	
}

/**
 * @fn        void frontResTblBinRead(UINT16 fileId)
 * @brief	front Resource File Table Bin Read.
 * @param	fileId = [in] resource file id...
 * @retval	None.
 * @see
 * @author	LinJieCheng
 * @since	2010-12-07
 * @todo		N/A
 * @bug		N/A
*/
static void
frontResTblBinRead(
	UINT16 fileId,
	UINT16 address
)
{
	UINT32 size;
	UINT8 sts;
	
	//printf("fileId = 0x%04x, address = 0x%04x\n",fileId,address);
	sp1kNandRsvSizeGet(fileId, NULL, &size); // Res_0x2.res = calLen.bin
	if (size > FRONT_RES_TBL_MAX_SIZE) {
		ASSERT(size > FRONT_RES_TBL_MAX_SIZE, 0); 
		return;
	}

	//printf("size = %lu\n",size);

	do {
		sts = sp1kNandRsvRead(fileId, ((K_SDRAM_CodeSize + ((address - 0x4000) >> 1)) << 1));
	}while(!sts);

	
	
}


#if 0
/**
 * @brief	reduce saturation.
 * @param	None.
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-17
 * @todo	N/A
 * @bug		N/A
*/
static void
frontColorNoiseReduce(
	UINT8 gain
)
{
	gain = gain;
}
#endif
#if 0
/**
 * @brief	frontMirrorFlipSet.
 * @param	MFmode = [in] Mirror or flip mode, has value as SENSOR_NORMAL, SENSOR_ROTATE_180, SENSOR_MIRROR, SENSOR_FLIP
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontMirrorFlipSet(
	UINT8 MFmode
)
{
	static UINT8 reg1e = 0x34;
	UINT8 cmdTbl[2*1];
	frontPrevCapabDesc_t *pcap;

	switch(MFmode){
		default:
		case SENSOR_NORMAL:
			reg1e &= 0xcf;
			break;

		case SENSOR_ROTATE_180:
			reg1e |= 0x30;
			break;

		case SENSOR_MIRROR:
			reg1e &= 0xcf;
			reg1e |= 0x20;
			break;

		case SENSOR_FLIP:
			reg1e &= 0xcf;
			reg1e |= 0x10;
			break;
	};

	pcap = &(frontPrevCapabDesc[frontPrevMode]);

	cmdTbl[0] = 0x1e;
	cmdTbl[1] = reg1e;
	i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
}

/**
 * @brief	frontFrameRateSet
 * @param	fps = [in] frame rate setting, for 30, 25, 20, 15 fps only
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontFrameRateSet(
	UINT8 fps
)
{
	UINT16 dummyLine = 0;
	UINT8 cmdTbl[2*2];
	frontPrevCapabDesc_t *pcap;

	if (fps >= 30)			//set 30fps
			dummyLine = 0;
	else if(fps >= 25)		//set 25fps
			dummyLine = 100;
	else if(fps >= 20)		//set 20fps
			dummyLine = 250;
	else					//set 15fps
			dummyLine = 500;

	pcap = &(frontPrevCapabDesc[frontPrevMode]);

	cmdTbl[0] = 0x92;
	cmdTbl[1] = (UINT8)(dummyLine&0x00ff);
	cmdTbl[2] = 0x93;
	cmdTbl[3] = (UINT8)(dummyLine>>8);

	i2cCmdTableSend(cmdTbl, 1, sizeof(cmdTbl)/2, I2C_TRANS_MODE_NORMAL, 0, 0);
}

/**
 * @brief	frontEffectModeSet
 * @param	mode = [in] SEPIA, BW, ...
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontEffectModeSet(
	UINT8 mode
)
{
}

/**
 * @brief	frontFrameRateSet
 * @param	mode = [in] AUTO_MODE, SUNSHINE, CLOUDY, TUNGSTEN_LAMP, FLUORESCENT_LAMP
 * @retval	None.
 * @see
 * @author	Matt Wang
 * @since	2008-11-24
 * @todo	N/A
 * @bug		N/A
*/
__STATIC void
frontWhiteBalanceModeSet(
	UINT8 mode
)
{
	UINT16 rgain, bgain, reg13,
	UINT8 modeChEn;
	static UINT8 oldmode = 0;

	cmos_rd(sensorSscSlaveAddr, 0x13, &reg13);

	oldmode = (reg13 & 0x02);
	modeChEn = 1;

	switch(mode){
		default:
		case AUTO_MODE:
			if (oldmode != 0)
				modeChEn = 0;
			reg13 |= 0x02;
			bgain = 0x80;
			rgain = 0x80;
			break;

		case SUNSHINE:
			reg13 &= 0xfd;
			bgain = 0x80;
			rgain = 0x80;
			break;

		case CLOUDY:
			reg13 &= 0xfd;
			bgain = 0x80;
			rgain = 0x90;
			break;

		case TUNGSTEN_LAMP:
			reg13 &= 0xfd;
			bgain = 0xc8;
			rgain = 0x80;
			break;

		case FLUORESCENT_LAMP:
			reg13 &= 0xfd;
			bgain = 0x98;
			rgain = 0xb3;
			break;
	}
	if (modeChEn){
		cmos_wr(sensorSscSlaveAddr, 0x01, bgain);
		cmos_wr(sensorSscSlaveAddr, 0x02, rgain);
		cmos_wr(sensorSscSlaveAddr, 0x13, reg13);
	}
}

/**************************************************************************************
*  Function Name : sensor_NightModeSet
*  Desprictions : Night mode enable/disable
*  Arguments :
*   mode: 0 :disable, 1:enable
*  Returns :
*  See also :
***************************************************************************************/
void sensor_NightModeSet(UINT32 mode)
{
	UINT32 reg3b, oldmode;

	cmos_rd(sensorSscSlaveAddr, 0x3b, &reg3b);
	oldmode = (reg3b & 0x80);

	if ((mode == NIGHT_MODE_ON) && (oldmode == 0)){
		reg3b |= 0xc0;
		cmos_wr(sensorSscSlaveAddr, 0x24, 0x3f);
		cmos_wr(sensorSscSlaveAddr, 0x25, 0x33);
		cmos_wr(sensorSscSlaveAddr, 0x3b, reg3b);
	}
	else if ((mode == NIGHT_MODE_OFF) && (oldmode != 0)){
		reg3b &= 0x1f;
		cmos_wr(sensorSscSlaveAddr, 0x3b, reg3b);
		cmos_wr(sensorSscSlaveAddr, 0x2e, 0);
		cmos_wr(sensorSscSlaveAddr, 0x2d, 0);
		cmos_wr(sensorSscSlaveAddr, 0x24, 0x3c);
		cmos_wr(sensorSscSlaveAddr, 0x25, 0x36);
	}
}


/**************************************************************************************
*  Function Name : sensor_PwrFreqModeSet
*  Desprictions : Set power freq. mode, de-flicker.
*  Arguments :
*   mode: 0 :50Hz, 1:60Hz
*  Returns :
*  See also :
***************************************************************************************/
void sensor_PwrFreqModeSet(UINT32 mode)
{
	UINT32 reg42, oldmode;

	cmos_rd(sensorSscSlaveAddr, 0x42, &reg42);
	oldmode = (reg42 & 0x01);

	if ((mode == POWER_FREQ_50HZ) && (oldmode == 0)){
		reg42 |= 0x01;
		cmos_wr(sensorSscSlaveAddr, 0x42, reg42);
	}
	else if ((mode == POWER_FREQ_60HZ) && (oldmode == 1)){
		reg42 &= 0xfe;
		cmos_wr(sensorSscSlaveAddr, 0x42, reg42);
	}
}


/**************************************************************************************
*  Function Name : sensor_BrightValueSet
*  Desprictions : Adjust sensor output brightness
*  Arguments :
*   value: adiust value
*  Returns :
*  See also :
***************************************************************************************/
void sensor_BrightValueSet(INT8 value)
{
	UINT32 regVal;

	printf("Brightness = %d\n", value);

	if (value >= 0)
		regVal = (UINT32)value;
	else
		regVal = (UINT32)(-value | 0x80);

	printf("0x55 = 0x%x\n", regVal);
	cmos_wr(sensorSscSlaveAddr, 0x55, (UINT32)regVal);



//	printf("Brightness = %d\n", value);

//	for(i=0; i<256; i++){
//		cmos_rd(sensorSscSlaveAddr, i, &regVal);
//		printf("0x%x = 0x%x\n", i, regVal);
//	}

//for get wb gain
/*
	cmos_rd(sensorSscSlaveAddr, 0x01, &regVal);
	printf("AWB B Gain = %d\n", regVal);
	cmos_rd(sensorSscSlaveAddr, 0x02, &regVal);
	printf("AWB R Gain = %d\n", regVal);
*/
}


/**************************************************************************************
*  Function Name : sensor_LedCtlSet
*  Desprictions :
*  Arguments :
*  Returns :
*  See also :
***************************************************************************************/
void sensor_LedCtlSet(UINT32 on_off)
{
}

#endif

