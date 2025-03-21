#ifndef REG_LISTS_H
#define REG_LISTS_H

#include "common.h"
#include "reg_defs.h"



struct RegisterEntry {
	u8 address;
	u8 value;
};


/*
* Registers settings. Most of them are undocumented. Some documentation is
* is available in the OV2640 datasheet, the OV2640 hardware app notes and
* the OV2640 software app notes documents.
*/

#define ENDMARKER { 0xff, 0xff }

static const RegisterEntry ov2640_init_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ 0x2c,   0xff },
	{ 0x2e,   0xdf },
	{ BANK_SEL, BANK_SEL_SENS },
	{ 0x3c,   0x32 },
	{ CLKRC, CLKRC_DIV_SET(20) },
	{ COM2, COM2_OCAP_Nx_SET(3) },
	{ REG04, REG04_DEF | REG04_HREF_EN },
	{ COM8,  COM8_DEF | COM8_AGC_EN | COM8_AEC_EN },
	//~ { AEC,    0x00 },
	{ COM9, COM9_AGC_GAIN_8x | 0x08},
	{ 0x2c,   0x0c },
	{ 0x33,   0x78 },
	{ 0x3a,   0x33 },
	{ 0x3b,   0xfb },
	{ 0x3e,   0x00 },
	{ 0x43,   0x11 },
	{ 0x16,   0x10 },
	{ 0x39,   0x02 },
	{ 0x35,   0x88 },
	{ 0x22,   0x0a },
	{ 0x37,   0x40 },
	{ 0x23,   0x00 },
	{ ARCOM2, 0xa0 },
	{ 0x06,   0x02 },
	{ 0x06,   0x88 },
	{ 0x07,   0xc0 },
	{ 0x0d,   0xb7 },
	{ 0x0e,   0x01 },
	{ 0x4c,   0x00 },
	{ 0x4a,   0x81 },
	{ 0x21,   0x99 },
	{ AEW,    0x40 },
	{ AEB,    0x38 },
	{ VV,     VV_HIGH_TH_SET(0x08) | VV_LOW_TH_SET(0x02) },
	{ 0x5c,   0x00 },
	{ 0x63,   0x00 },
	{ FLL,    0x22 },
	{ COM3,   0x38 | COM3_BAND_AUTO },
	{ REG5D,  0x55 },
	{ REG5E,  0x7d },
	{ REG5F,  0x7d },
	{ REG60,  0x55 },
	{ HISTO_LOW,   0x70 },
	{ HISTO_HIGH,  0x80 },
	{ 0x7c,   0x05 },
	{ 0x20,   0x80 },
	{ 0x28,   0x30 },
	{ 0x6c,   0x00 },
	{ 0x6d,   0x80 },
	{ 0x6e,   0x00 },
	{ 0x70,   0x02 },
	{ 0x71,   0x94 },
	{ 0x73,   0xc1 },
	{ 0x3d,   0x34 },
	{ COM7, COM7_RES_UXGA | COM7_ZOOM_EN },
	{ 0x5a,   0x57 },
	{ BD50,   0xbb },
	{ BD60,   0x9c },
	{ BANK_SEL, BANK_SEL_DSP },
	{ 0xe5,   0x7f },
	{ MC_BIST, MC_BIST_RESET | MC_BIST_BOOT_ROM_SEL },
	{ 0x41,   0x24 },
	{ RESET, RESET_JPEG | RESET_DVP },
	{ 0x76,   0xff },
	{ 0x33,   0xa0 },
	{ 0x42,   0x20 },
	{ 0x43,   0x18 },
	{ 0x4c,   0x00 },
	{ CTRL3, CTRL3_BPC_EN | CTRL3_WPC_EN | 0x10 },
	{ 0x88,   0x3f },
	{ 0xd7,   0x03 },
	{ 0xd9,   0x10 },
	{ R_DVP_SP , R_DVP_SP_AUTO_MODE | 0x2 },
	{ 0xc8,   0x08 },
	{ 0xc9,   0x80 },
	{ BPADDR, 0x00 },
	{ BPDATA, 0x00 },
	{ BPADDR, 0x03 },
	{ BPDATA, 0x48 },
	{ BPDATA, 0x48 },
	{ BPADDR, 0x08 },
	{ BPDATA, 0x20 },
	{ BPDATA, 0x10 },
	{ BPDATA, 0x0e },
	{ 0x90,   0x00 },
	{ 0x91,   0x0e },
	{ 0x91,   0x1a },
	{ 0x91,   0x31 },
	{ 0x91,   0x5a },
	{ 0x91,   0x69 },
	{ 0x91,   0x75 },
	{ 0x91,   0x7e },
	{ 0x91,   0x88 },
	{ 0x91,   0x8f },
	{ 0x91,   0x96 },
	{ 0x91,   0xa3 },
	{ 0x91,   0xaf },
	{ 0x91,   0xc4 },
	{ 0x91,   0xd7 },
	{ 0x91,   0xe8 },
	{ 0x91,   0x20 },
	{ 0x92,   0x00 },
	{ 0x93,   0x06 },
	{ 0x93,   0xe3 },
	{ 0x93,   0x03 },
	{ 0x93,   0x03 },
	{ 0x93,   0x00 },
	{ 0x93,   0x02 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x93,   0x00 },
	{ 0x96,   0x00 },
	{ 0x97,   0x08 },
	{ 0x97,   0x19 },
	{ 0x97,   0x02 },
	{ 0x97,   0x0c },
	{ 0x97,   0x24 },
	{ 0x97,   0x30 },
	{ 0x97,   0x28 },
	{ 0x97,   0x26 },
	{ 0x97,   0x02 },
	{ 0x97,   0x98 },
	{ 0x97,   0x80 },
	{ 0x97,   0x00 },
	{ 0x97,   0x00 },
	{ 0xa4,   0x00 },
	{ 0xa8,   0x00 },
	{ 0xc5,   0x11 },
	{ 0xc6,   0x51 },
	{ 0xbf,   0x80 },
	{ 0xc7,   0x10 }, /* white balance */
	{ 0xb6,   0x66 },
	{ 0xb8,   0xA5 },
	{ 0xb7,   0x64 },
	{ 0xb9,   0x7C },
	{ 0xb3,   0xaf },
	{ 0xb4,   0x97 },
	{ 0xb5,   0xFF },
	{ 0xb0,   0xC5 },
	{ 0xb1,   0x94 },
	{ 0xb2,   0x0f },
	{ 0xc4,   0x5c },
	{ 0xa6,   0x00 },
	{ 0xa7,   0x20 },
	{ 0xa7,   0xd8 },
	{ 0xa7,   0x1b },
	{ 0xa7,   0x31 },
	{ 0xa7,   0x00 },
	{ 0xa7,   0x18 },
	{ 0xa7,   0x20 },
	{ 0xa7,   0xd8 },
	{ 0xa7,   0x19 },
	{ 0xa7,   0x31 },
	{ 0xa7,   0x00 },
	{ 0xa7,   0x18 },
	{ 0xa7,   0x20 },
	{ 0xa7,   0xd8 },
	{ 0xa7,   0x19 },
	{ 0xa7,   0x31 },
	{ 0xa7,   0x00 },
	{ 0xa7,   0x18 },
	{ 0x7f,   0x00 },
	{ 0xe5,   0x1f },
	{ 0xe1,   0x77 },
	{ 0xdd,   0x7f },
	{ CTRL0,  CTRL0_YUV422 | CTRL0_YUV_EN },
	ENDMARKER
};



/*
* Register settings for window size
* The preamble, setup the internal DSP to input an UXGA (1600x1200) image.
* Then the different zooming configurations will setup the output image size.
*/

#define W_UXGA	1600
#define H_UXGA	1200

static const RegisterEntry ov2640_size_change_preamble_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ RESET, RESET_DVP },
	{ HSIZE8, HSIZE8_SET(W_UXGA) },
	{ VSIZE8, VSIZE8_SET(H_UXGA) },
	{ CTRL2, CTRL2_DCW_EN | CTRL2_SDE_EN |
		 CTRL2_UV_AVG_EN | CTRL2_CMX_EN | CTRL2_UV_ADJ_EN },
	{ HSIZE, HSIZE_SET(W_UXGA) },
	{ VSIZE, VSIZE_SET(H_UXGA) },
	{ XOFFL, XOFFL_SET(0) },
	{ YOFFL, YOFFL_SET(0) },
	{ VHYX, VHYX_HSIZE_SET(W_UXGA) | VHYX_VSIZE_SET(H_UXGA) |
		VHYX_XOFF_SET(0) | VHYX_YOFF_SET(0)},
	{ TEST, TEST_HSIZE_SET(W_UXGA) },
	ENDMARKER
};

#define PER_SIZE_REG_SEQ(x, y, v_div, h_div, pclk_div)	\
	{ CTRLI, CTRLI_LP_DP | CTRLI_V_DIV_SET(v_div) |	\
		 CTRLI_H_DIV_SET(h_div)},		\
	{ ZMOW, ZMOW_OUTW_SET(x) },			\
	{ ZMOH, ZMOH_OUTH_SET(y) },			\
	{ ZMHH, ZMHH_OUTW_SET(x) | ZMHH_OUTH_SET(y) },	\
	{ R_DVP_SP, pclk_div },				\
	{ RESET, 0x00}



#define W_QCIF	176
#define H_QCIF	144
static const RegisterEntry ov2640_qcif_regs[] = {
	PER_SIZE_REG_SEQ(W_QCIF, H_QCIF, 3, 3, 4),
	ENDMARKER
};

#define W_QVGA	320
#define H_QVGA	240
static const RegisterEntry ov2640_qvga_regs[] = {
	PER_SIZE_REG_SEQ(W_QVGA, H_QVGA, 2, 2, 4),
	ENDMARKER
};

#define W_CIF	352
#define H_CIF	288
static const RegisterEntry ov2640_cif_regs[] = {
	PER_SIZE_REG_SEQ(W_CIF, H_CIF, 2, 2, 8),
	ENDMARKER
};

#define W_VGA	640
#define H_VGA	480
static const RegisterEntry ov2640_vga_regs[] = {
	PER_SIZE_REG_SEQ(W_VGA, H_VGA, 0, 0, 2),
	ENDMARKER
};

#define W_SVGA	800
#define H_SVGA	600
static const RegisterEntry ov2640_svga_regs[] = {
	PER_SIZE_REG_SEQ(W_SVGA, H_SVGA, 1, 1, 2),
	ENDMARKER
};

#define W_XGA	1024
#define H_XGA	768
static const RegisterEntry ov2640_xga_regs[] = {
	PER_SIZE_REG_SEQ(W_XGA, H_XGA, 0, 0, 2),
	{ CTRLI,    0x00},
	ENDMARKER
};

#define W_SXGA	1280
#define H_SXGA	1024
static const RegisterEntry ov2640_sxga_regs[] = {
	PER_SIZE_REG_SEQ(W_SXGA, H_SXGA, 0, 0, 2),
	{ CTRLI,    0x00},
	{ R_DVP_SP, 2 | R_DVP_SP_AUTO_MODE },
	ENDMARKER
};

// #define W_UXGA	1600
// #define H_UXGA	1200
static const RegisterEntry ov2640_uxga_regs[] = {
	PER_SIZE_REG_SEQ(W_UXGA, H_UXGA, 0, 0, 0),
	{ CTRLI,    0x00},
	{ R_DVP_SP, 0 | R_DVP_SP_AUTO_MODE },
	ENDMARKER
};




/*
* Register settings for pixel formats
*/

static const RegisterEntry ov2640_yuyv_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_YUV422 },
	{ 0xd7, 0x03 },
	{ 0x33, 0xa0 },
	{ 0xe5, 0x1f },
	{ 0xe1, 0x67 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	ENDMARKER
};

static const RegisterEntry ov2640_uyvy_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_LBYTE_FIRST | IMAGE_MODE_YUV422 },
	{ 0xd7, 0x01 },
	{ 0x33, 0xa0 },
	{ 0xe1, 0x67 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	ENDMARKER
};

static const RegisterEntry ov2640_rgb565_be_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_RGB565 },
	// { CTRL0, CTRL0_RGB_EN },
	{ 0xd7, 0x03 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	ENDMARKER
};

static const RegisterEntry ov2640_rgb565_le_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_LBYTE_FIRST | IMAGE_MODE_RGB565 },
	// { CTRL0, CTRL0_RGB_EN },
	{ 0xd7, 0x03 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	ENDMARKER
};

static const RegisterEntry ov2640_raw10_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_RAW10 },
	// { CTRL0,  CTRL0_RAW_EN },
	{ 0xd7, 0x03 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	ENDMARKER
};



static const RegisterEntry ov2640_jpeg_regs[] = {
	{ BANK_SEL, BANK_SEL_DSP },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	{ IMAGE_MODE, IMAGE_MODE_YUV422 },
	{ 0xd7, 0x03 },
	{ 0x33, 0xa0 },
	{ 0xe5, 0x1f },
	{ 0xe1, 0x67 },
	{ RESET,  0x00 },
	{ R_BYPASS, R_BYPASS_USE_DSP },
	
	// { BANK_SEL, BANK_SEL_DSP },
	{ 0xe0, 0x14 },
	{ 0xe1, 0x77 },
	{ 0xe5, 0x1f },
	{ 0xd7, 0x03 },
	{ IMAGE_MODE, IMAGE_MODE_JPEG_EN },
	{ 0xe0, 0x00 },
	{ BANK_SEL, BANK_SEL_SENS },
	{ 0x04, 0x08 },
	//~ { RESET,  RESET_DVP | RESET_JPEG },
	ENDMARKER
};


#endif