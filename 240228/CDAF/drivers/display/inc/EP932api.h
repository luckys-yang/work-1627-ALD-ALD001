#ifndef EP932_API_H
#define EP932_API_H

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// for EP Driver
//
#include "type.h"
#include "general.h"

typedef unsigned char  BYTE;

#ifndef K_SLASH
#define K_SLASH /
#endif

#define DBG_Level    0

#if (DBG_Level == 0)
#define DBG_printf(x)
#define printf K_SLASH/
#elif (DBG_Level == 1)
#define DBG_printf(x)
#else (DBG_Level == 2)
#define DBG_printf(x)   printf x
#endif

typedef enum {
	// Master
	SMBUS_STATUS_Success = 0x00,
	SMBUS_STATUS_Pending,//	SMBUS_STATUS_Abort,
	SMBUS_STATUS_NoAct = 0x02,
	SMBUS_STATUS_TimeOut,
	SMBUS_STATUS_ArbitrationLoss = 0x04
} SMBUS_STATUS;


typedef enum {
	AUD_Mute = 0,
	AUD_SF_32000Hz = 1,
	AUD_SF_44100Hz =2,
	AUD_SF_48000Hz =3,
	AUD_SF_88200Hz,
	AUD_SF_96000Hz,
	AUD_SF_176400Hz,
	AUD_SF_192000Hz
}HDMI_AudFreq;

typedef enum {
	Video_OUT_Auto	= 0,
	Video_OUT_YUV_444,
	Video_OUT_YUV_422,
	Video_OUT_RGB_444,
}HDMI_Out_format;

//
////////////////////////////////////////////////////////////////////////////////
/*
typedef enum{
	AudioMute_Disable = 0,
	AudioMute_Enable = 1
}CSTVOUT_Audio_MODE;
*/	

typedef enum {
	AUD_I2S = 0,
	AUD_SPDIF	
}HDMI_AudFmt_t;

#define DSEL_Dual_Edge		0x08
#define BSEL_24bit			0x04
#define EDGE_rising		0x02
#define FMT_12				0x01




void EP_EP932M_Reset(void);
void  EP_HDMI_Set_Audio_Interface(HDMI_AudFmt_t  Audfmt, HDMI_AudFreq  Audfreq);
void  EP_HDMI_Set_Video_Timing(int Timing);
void EP_HDMI_Set_Video_Output(int HDMI_output);
void  EP_HDMI_SetAudFmt(HDMI_AudFmt_t  Audfmt, HDMI_AudFreq  Audfreq);

unsigned char EP_HDMI_Init(void); 


#endif
