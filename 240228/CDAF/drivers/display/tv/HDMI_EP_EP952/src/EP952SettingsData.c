/******************************************************************************\

          (c) Copyright Explore Semiconductor, Inc. Limited 2005
                           ALL RIGHTS RESERVED

--------------------------------------------------------------------------------

  File        :  EP952SettingsData.c

  Description :  EP952 Settings Table

\******************************************************************************/

#include "EP952SettingsData.h"

VDO_SETTINGS code EP952_VDO_Settings[] = {
	//                   HVRes_Type,             		DE_Gen(DLY,CNT,TOP,LIN),                E_Sync, 				AR_PR,       Pix_Freq_Type,
	{  0,{       0,   0,   0,    0},{  0,   0,  0,   0},{0x00,  0,  0,  0,  0,   0},  0x00,                  1}, // 0:
	// HDMI Mode
	{  1,{VNegHNeg, 800, 525,16666},{ 48, 640, 34, 480},{0x00, 12, 96, 10,  2,   0},  0x10,  PIX_FREQ_25175KHz}, // 1:    640 x  480p
	{  2,{VNegHNeg, 858, 525,16666},{ 60, 720, 31, 480},{0x00, 12, 62,  9,  6,   0},  0x10,  PIX_FREQ_27000KHz}, // 2:    720 x  480p  4:3
	{  3,{VNegHNeg, 858, 525,16666},{ 60, 720, 31, 480},{0x00, 12, 62,  9,  6,   0},  0x20,  PIX_FREQ_27000KHz}, // 3:    720 x  480p 16:9
	{  4,{VPosHPos,1650, 750,16666},{220,1280, 21, 720},{0x00,106, 40,  5,  5,   0},  0x20,  PIX_FREQ_74176KHz}, // 4:   1280 x  720p
	{  5,{VPosHPos,2200, 563,16666},{148,1920, 16, 540},{0x09, 84, 44,  2,  5,1100},  0x20,  PIX_FREQ_74176KHz}, // 5:   1920 x 1080i
	{  6,{VNegHNeg, 858, 262,16666},{ 57, 720, 16, 240},{0x09, 15, 62,  4,  3, 429},  0x15,  PIX_FREQ_27000KHz}, // 6:    720 x  480i, pix repl
	{  7,{VNegHNeg, 858, 262,16666},{ 57, 720, 16, 240},{0x09, 15, 62,  4,  3, 429},  0x25,  PIX_FREQ_27000KHz}, // 7:    720 x  480i, pix repl
	{  8,{VNegHNeg, 858, 262,16666},{ 57, 720, 16, 240},{0x00, 15, 62,  4,  3,   0},  0x15,  PIX_FREQ_27000KHz}, // 8:    720 x  240p, pix repl
	{  9,{VNegHNeg, 858, 262,16666},{ 57, 720, 16, 240},{0x00, 15, 62,  4,  3,   0},  0x25,  PIX_FREQ_27000KHz}, // 9:    720 x  240p, pix repl
	{ 10,{VNegHNeg,3432, 262,16666},{228,2880, 16, 240},{0x09, 72,248,  4,  3,1716},  0x10,  PIX_FREQ_54000KHz}, // 10:  2880 x  480i 4:3
	{ 11,{VNegHNeg,3432, 262,16666},{228,2880, 16, 240},{0x09, 72,248,  4,  3,1716},  0x20,  PIX_FREQ_54000KHz}, // 11:  2880 x  480i 16:9
	{ 12,{VNegHNeg,3432, 262,16666},{228,2880, 16, 240},{0x00, 72,248,  4,  3,   0},  0x10,  PIX_FREQ_54000KHz}, // 12:  2880 x  240p 4:3
	{ 13,{VNegHNeg,3432, 262,16666},{228,2880, 16, 240},{0x00, 72,248,  4,  3,   0},  0x20,  PIX_FREQ_54000KHz}, // 13:  2880 x  240p 16:9
	{ 14,{VNegHNeg,1716, 525,16666},{120,1440, 31, 480},{0x00, 28,124,  9,  6,   0},  0x10,  PIX_FREQ_54000KHz}, // 14:  1440 x  480p 4:3
	{ 15,{VNegHNeg,1716, 525,16666},{120,1440, 31, 480},{0x00, 28,124,  9,  6,   0},  0x20,  PIX_FREQ_54000KHz}, // 15:  1440 x  480p 16:9
	{ 16,{VPosHPos,2200,1125,16666},{148,1920, 37,1080},{0x00, 84, 44,  4,  5,   0},  0x20, PIX_FREQ_148500KHz}, // 16:  1920 x 1080p
	{ 17,{VNegHNeg, 864, 625,20000},{ 68, 720, 40, 576},{0x00,  8, 64,  5,  5,   0},  0x10,  PIX_FREQ_27000KHz}, // 17:   720 x  576p 4:3
	{ 18,{VNegHNeg, 864, 625,20000},{ 68, 720, 40, 576},{0x00,  8, 64,  5,  5,   0},  0x20,  PIX_FREQ_27000KHz}, // 18:   720 x  576p 16:9
	{ 19,{VPosHPos,1980, 750,20000},{220,1280, 21, 720},{0x00,436, 40,  5,  5,   0},  0x20,  PIX_FREQ_74250KHz}, // 19:  1280 x  720p, 50 Hz
	{ 20,{VPosHPos,2640, 563,20000},{148,1920, 16, 540},{0x09,524, 44,  2,  5,1320},  0x20,  PIX_FREQ_74250KHz}, // 20:  1920 x 1080i, 50 Hz
	{ 21,{VNegHNeg, 864, 313,20000},{ 69, 720, 20, 288},{0x09,  8, 63,  2,  3, 432},  0x15,  PIX_FREQ_27000KHz}, // 21:   720 x  576i, pix repl
	{ 22,{VNegHNeg, 864, 313,20000},{ 69, 720, 20, 288},{0x09,  8, 63,  2,  3, 432},  0x25,  PIX_FREQ_27000KHz}, // 22:   720 x  576i, pix repl
	{ 23,{VNegHNeg, 864, 313,20000},{ 69, 720, 20, 288},{0x00,  8, 63,  3,  3,   0},  0x15,  PIX_FREQ_27000KHz}, // 23:   720 x  288p, pix repl
	{ 24,{VNegHNeg, 864, 313,20000},{ 69, 720, 20, 288},{0x00,  8, 63,  3,  3,   0},  0x25,  PIX_FREQ_27000KHz}, // 24:   720 x  288p, pix repl
	{ 25,{VNegHNeg,3456, 313,20000},{276,2880, 20, 288},{0x09, 44,252,  2,  3,1728},  0x10,  PIX_FREQ_54000KHz}, // 25:  2880 x  576i
	{ 26,{VNegHNeg,3456, 313,20000},{276,2880, 20, 288},{0x09, 44,252,  2,  3,1728},  0x20,  PIX_FREQ_54000KHz}, // 26:  2880 x  576i
	{ 27,{VNegHNeg,3456, 313,20000},{276,2880, 20, 288},{0x00, 44,252,  3,  3,   0},  0x10,  PIX_FREQ_54000KHz}, // 27:  2880 x  288p
	{ 28,{VNegHNeg,3456, 313,20000},{276,2880, 20, 288},{0x00, 44,252,  3,  3,   0},  0x20,  PIX_FREQ_54000KHz}, // 28:  2880 x  288p
	{ 29,{VPosHNeg,1728, 625,20000},{136,1440, 40, 576},{0x00, 20,128,  5,  5,   0},  0x10,  PIX_FREQ_54000KHz}, // 29:  1440 x  576p
	{ 30,{VPosHNeg,1728, 625,20000},{136,1440, 40, 576},{0x00, 20,128,  5,  5,   0},  0x20,  PIX_FREQ_54000KHz}, // 30:  1440 x  576p
	{ 31,{VPosHPos,2640,1125,20000},{148,1920, 37,1080},{0x00,524, 44,  4,  5,   0},  0x20, PIX_FREQ_148500KHz}, // 31:  1920 x 1080p, 50 Hz
	{ 32,{VPosHPos,2750,1125,41666},{148,1920, 37,1080},{0x00,634, 44,  4,  5,   0},  0x20,  PIX_FREQ_74176KHz}, // 32:  1920 x 1080p
	{ 33,{VPosHPos,2640,1125,40000},{148,1920, 37,1080},{0x00,524, 44,  4,  5,   0},  0x20,  PIX_FREQ_74250KHz}, // 33:  1920 x 1080p, 25 Hz
	{ 34,{VPosHPos,2200,1125,33333},{148,1920, 37,1080},{0x00, 84, 44,  4,  5,   0},  0x20,  PIX_FREQ_74176KHz}, // 34:  1920 x 1080p

};

unsigned char EP952_VDO_Settings_Max = (sizeof(EP952_VDO_Settings)/sizeof(EP952_VDO_Settings[0]));

//
// Index = [Channel Number]
//
ADO_SETTINGS code EP952_ADO_Settings[] = {
	// SpeakerMapping, Flat
	{0x00, 0xF0}, // 0.0 - Flat All
	{0x00, 0x00}, // 2.0 - SD0
	{0x04, 0xA0}, // 3.0 - SD0 +SD2
	{0x08, 0xA0}, // 4.0 - SD0 +SD1 +SD2
	{0x07, 0x80}, // 4.1 - SD0 +SD3
	{0x0B, 0x80}, // 5.1 - SD0 +SD1 +SD3
	{0x0F, 0x00}, // 6.1 - SD0 +SD2 +SD3
	{0x13, 0x00}, // 7.1 - SD0 +SD1 +SD2 +SD3
};

//
// Index = [Pixel Freq Type]
//
N_CTS_SETTINGS code N_CTS_32K[] = {
	{ 4576, 28125},        // 25.20 MHz / 1.001
	{ 4096, 25200},        // 25.20 MHz

	{ 4096, 27000},        // 27.00 MHz
	{ 4096, 27027},        // 27.00 MHz * 1.001

	{ 4096, 54000},        // 54.00 MHz
	{ 4096, 54054},        // 54.00 MHz * 1.001

	{ 4096, 72000},        // 72.00 MHz

	{11648,210937},        // 74.25 MHz / 1.001
	{ 4096, 74250},        // 74.25 MHz

	{ 4096,108000},        // 108.00 MHz
	{ 4096,108108},        // 108.00 MHz * 1.001

	{11648,421875},        // 148.50 MHz / 1.001
	{ 4096,148500},        // 148.50 MHz

	{ 4096,148500},        // PC
};

N_CTS_SETTINGS code N_CTS_44K1[] = {
	{ 7007, 31250},        // 25.20 MHz / 1.001
	{ 6272, 28000},        // 25.20 MHz

	{ 6272, 30000},        // 27.00 MHz
	{ 6272, 30030},        // 27.00 MHz * 1.001

	{ 6272, 60000},        // 54.00 MHz
	{ 6272, 60060},        // 54.00 MHz * 1.001

	{ 6272, 80000},        // 72.00 MHz

	{17836,234375},        // 74.25 MHz / 1.001
	{ 6272, 82500},        // 74.25 MHz

	{ 6272,120000},        // 108.00 MHz
	{ 6272,120120},        // 108.00 MHz * 1.001

	{ 8918,234375},        // 148.50 MHz / 1.001
	{ 6272,165000},        // 148.50 MHz

	{ 6272,165000},        // PC
};

N_CTS_SETTINGS code N_CTS_48K[] = {
	{ 6864, 28125},        // 25.20 MHz / 1.001
	{ 6144, 25200},        // 25.20 MHz

	{ 6144, 27000},        // 27.00 MHz
	{ 6144, 27027},        // 27.00 MHz * 1.001

	{ 6144, 54000},        // 54.00 MHz
	{ 6144, 54054},        // 54.00 MHz * 1.001

	{ 6144, 72000},        // 72.00 MHz

	{11648,140625},        // 74.25 MHz / 1.001
	{ 6144, 74250},        // 74.25 MHz

	{ 6144, 108000},        // 108.00 MHz
	{ 6144, 108108},        // 108.00 MHz * 1.001

	{ 5824, 140625},        // 148.50 MHz / 1.001
	{ 6144, 148500},        // 148.50 MHz

	{ 6144, 148500},        // PC
};
