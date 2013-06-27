/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          bsl_1.c
 *
 * \version       $Revision: 85 $
 *
 * \date          $Date: 21/01/08 $
 *
 * \brief         BSL driver component for the TDA998x HDMI Transmitter
 *
 * \section refs  Reference Documents
 * HDMI Driver - Outline Architecture.doc
 * HDMI Driver - bsl - SCS.doc
 *
 * */

/*============================================================================*/
/*                   FILE CONFIGURATION                                       */
/*============================================================================*/

#include <linux/kernel.h>

/* Defining this symbol on the compiler command line excludes some API checks */
/* #define NO_RETIF_BADPARAM */

/* Defining this symbol on the compiler command line excludes some API checks */
/* #define NO_RETIF_REG_FAIL */

/* Defining this symbol on the compiler command line excludes unused code */
/* #define DEMO_BUILD */

/* Defining this symbol on the compiler command line adapts code for testing */
/* #define UNIT_TEST */

/*============================================================================*/
/*                   STANDARD INCLUDE FILES                                   */
/*============================================================================*/

/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/
#include "tmbslHdmiTx.h"
#include "tmbslHdmiTx_local.h"

/*============================================================================*/
/*                   MACRO DEFINITIONS                                        */
/*============================================================================*/

/*============================================================================*/
/*                   TYPE DEFINITIONS                                         */
/*============================================================================*/

#define SSD_UNUSED_VALUE 0xF0

#ifdef FORMAT_PC
#define DEPTH_COLOR_PC 1  /* PC_FORMAT only 8 bits available */
#endif /* FORMAT_PC */

/*============================================================================*/
/*                   PUBLIC VARIABLE DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC CONSTANT DECLARATIONS                             */
/*============================================================================*/

/** Preset default values for an object instance */
static CONST_DAT hdmi_txobject_t k_hdmi_tx_instance_default
= {
	ST_UNINITIALIZED,                   /* state */
	0,                                  /* nIgnoredEvents */
	unit0,                            /* txUnit */
	0,                                  /* uHwAddress */
	(pbsl_sys_func_t)0,           /* sysFuncWrite */
	(pbsl_sys_func_t)0,           /* sysFuncRead */
	(pbsl_sys_func_edid_t)0,       /* sysFuncEdidRead */
	(pbsl_sys_func_timer_t)0,      /* sysFuncTimer */
	{                                   /* funcIntCallbacks[] */
		(pbsl_callback_t)0
	},
	{                                   /* uSupportedVersions[] */
		E_DEV_VERSION_N4, /* <=== Applies to N5 also */
		E_DEV_VERSION_LIST_END
	},
	E_DEV_VERSION_LIST_END,             /* uDeviceVersion */
	E_DEV_VERSION_LIST_END,             /* uDeviceFeatures */
	power_on,                         /* ePowerState */
	false,                              /* EdidAlternateAddr */
	HDMITX_SINK_DVI,                    /* sinkType */
	HDMITX_SINK_DVI,                    /* EdidSinkType */
	false,                              /* EdidSinkAi */
	0,                                  /* EdidCeaFlags */
	HDMITX_EDID_NOT_READ,               /* EdidStatus */
	0,                                  /* NbDTDStored */
	{                                   /* EdidDTD: */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 1 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*2 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*3 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*4 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*5 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*6 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*7 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*8 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*9 */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /*10*/
	},
	{                               /* EdidMonitorDescriptor */
		false,                      /* bDescRecord */
		/* uMonitorName[EDID_MONITOR_DESCRIPTOR_SIZE]       */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
		false,                      /* bDescRecord */
		/* uMinVerticalRate                                 */
		0,
		/* uMaxVerticalRate                                 */
		0,
		/* uMinHorizontalRate                               */
		0,
		/* uMaxHorizontalRate                               */
		0,
		/* uMaxSupportedPixelClk                            */
		0
	},
	{
		false,                      /* bDescRecord */
		/* uOtherDescriptor[EDID_MONITOR_DESCRIPTOR_SIZE]   */
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{                                   /* EdidVFmts[] */
		HDMITX_VFMT_NULL
	},
	0,                                  /* EdidSvdCnt */
	{                                   /* EdidAFmts[]. */
		{0, 0, 0}                           /* {ModeChans, Freqs, Byte3} */
	},
	0,                                  /* EdidSadCnt */
	{
		0                               /* EdidBlock[ ] */
	},
	0,                                  /* EdidBlockCnt */
	0,                                  /* EdidSourceAddress */
	{                                   /* EDIDBasicDisplayParam */
		0,                              /* uVideoInputDef */
		0,                              /* uMaxHorizontalSize */
		0,                              /* uMaxVerticalSize */
		0,                              /* uGamma */
		0,                              /* uFeatureSupport */
	},
	HDMITX_VFMT_NULL,                   /* vinFmt */
	HDMITX_VFMT_NULL,                   /* voutFmt */
	HDMITX_PIXRATE_DOUBLE,              /* pixRate */
	HDMITX_VINMODE_RGB444,              /* vinMode */
	HDMITX_VOUTMODE_RGB444,             /* voutMode */
	HDMITX_VFREQ_INVALID,               /* voutFreq */
	HDMITX_SCAMODE_OFF,                 /* scaMode */
	HDMITX_UPSAMPLE_AUTO,               /* upsampleMode */
	HDMITX_PIXREP_MIN,                  /* pixelRepeatCount */
	HDMITX_HOTPLUG_INVALID,             /* hotPlugStatus */
#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON
	HDMITX_RX_SENSE_INVALID,            /* rxSenseStatus */
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */
	E_PAGE_INVALID,                     /* curRegPage */
	{
		/* These match power-up defaults.  shadowReg[]: */
		0x00,                           /* E_SP00_MAIN_CNTRL0 */
		0x00,                           /* E_SP00_INT_FLAGS_0 */
		0x00,                           /* E_SP00_INT_FLAGS_1 */
#ifdef TMFL_TDA9981_SUPPORT
		0x00,                           /* E_SP00_INT_FLAGS_2 */
#endif /* TMFL_TDA9981_SUPPORT */
		0x01,                           /* E_SP00_VIP_CNTRL_0 */
		0x24,                           /* E_SP00_VIP_CNTRL_1 */
		0x56,                           /* E_SP00_VIP_CNTRL_2 */
		0x17,                           /* E_SP00_VIP_CNTRL_3 */
		0x01,                           /* E_SP00_VIP_CNTRL_4 */
		0x00,                           /* E_SP00_VIP_CNTRL_5 */
		0x05,                           /* E_SP00_MAT_CONTRL  */
		0x00,                           /* E_SP00_TBG_CNTRL_0 */
		0x00,                           /* E_SP00_TBG_CNTRL_1 */
		0x00,                           /* E_SP00_HVF_CNTRL_0 */
		0x00,                           /* E_SP00_HVF_CNTRL_1 */
		0x00,                           /* E_SP00_TIMER_H     */
		0x00,                           /* E_SP00_DEBUG_PROBE */
		0x00                            /* E_SP00_AIP_CLKSEL  */
#ifndef TMFL_TDA9981_SUPPORT
		, 0x00                          /* E_SP01_SC_VIDFORMAT*/
		, 0x00                          /* E_SP01_SC_CNTRL    */
		, 0x00                          /* E_SP01_TBG_CNTRL_0 */
#endif /* TMFL_TDA9981_SUPPORT */
	},
	false,								/* Init prevBluescreen to false */
	false,								/* Init prevPattern to false */
	false                              /* bInitialized */
};

/**
 * Lookup table to convert from EIA/CEA TV video formats used in the EDID and
 * in API parameters to pixel clock frequencies, according to SCS Table
 * "HDMI Pixel Clock Frequencies per EIA/CEA-861B Video Output Format".
 * The other index is the veritical frame frequency.
 * */

static CONST_DAT u8 k_vfmt_to_pix_clk_tv[HDMITX_VFMT_TV_NUM][HDMITX_VFREQ_NUM] = {
	/* HDMITX_VFREQ_24Hz HDMITX_VFREQ_25Hz HDMITX_VFREQ_30Hz HDMITX_VFREQ_50Hz HDMITX_VFREQ_59Hz HDMITX_VFREQ_60Hz */
	/* HDMITX_VFMT_NULL               */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_01_640x480p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_25175,   E_PIXCLK_25200},
	/* HDMITX_VFMT_02_720x480p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_03_720x480p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_04_1280x720p_60Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_74175,   E_PIXCLK_74250},
	/* HDMITX_VFMT_05_1920x1080i_60Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_74175,   E_PIXCLK_74250},
	/* HDMITX_VFMT_06_720x480i_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_07_720x480i_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_08_720x240p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_09_720x240p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_27027},
	/* HDMITX_VFMT_10_720x480i_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_11_720x480i_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_12_720x240p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_13_720x240p_60Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_14_1440x480p_60Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_15_1440x480p_60Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_54054},
	/* HDMITX_VFMT_16_1920x1080p_60Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_148350,  E_PIXCLK_148500},
	/* HDMITX_VFMT_17_720x576p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_18_720x576p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_19_1280x720p_50Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_74250,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_20_1920x1080i_50Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_74250,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_21_720x576i_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_22_720x576i_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_23_720x288p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_24_720x288p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_27000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_25_720x576i_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_26_720x576i_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_27_720x288p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_28_720x288p_50Hz   */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_29_1440x576p_50Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_30_1440x576p_50Hz  */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_54000,   E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_31_1920x1080p_50Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_148500,  E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_32_1920x1080p_24Hz */
	{E_PIXCLK_74250,   E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_33_1920x1080p_25Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_74250,   E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID},
	/* HDMITX_VFMT_34_1920x1080p_30Hz */
	{E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_74250,   E_PIXCLK_INVALID, E_PIXCLK_INVALID, E_PIXCLK_INVALID}
};

/**
 * Lookup table to convert PC formats used in API parameters to pixel clock
 * frequencies.
 * The other index is the veritical frame frequency.
 * */
#ifdef FORMAT_PC
static CONST_DAT u8 k_vfmt_to_pix_clk_pc[HDMITX_VFMT_PC_NUM+1] = {
	/* HDMITX_VFREQ_60Hz HDMITX_VFREQ_70Hz HDMITX_VFREQ_72Hz HDMITX_VFREQ_75Hz HDMITX_VFREQ_85Hz HDMITX_VFREQ_87Hz*/
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_NULL                */
	E_PIXCLK_25175  ,   /* HDMITX_VFMT_PC_640x480p_60Hz   */
	E_PIXCLK_40000  ,   /* HDMITX_VFMT_PC_800x600p_60Hz   */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1152x960p_60Hz  */
	E_PIXCLK_65000  ,   /* HDMITX_VFMT_PC_1024x768p_60Hz */
	E_PIXCLK_79500  ,   /* HDMITX_VFMT_PC_1280x768p_60Hz */
	E_PIXCLK_108000 ,   /* HDMITX_VFMT_PC_1280x1024p_60Hz */
	E_PIXCLK_85500  ,   /* HDMITX_VFMT_PC_1360x768p_60Hz  */
	E_PIXCLK_121750 ,   /* HDMITX_VFMT_PC_1400x1050p_60Hz */
	E_PIXCLK_162000 ,   /* HDMITX_VFMT_PC_1600x1200p_60Hz */
	E_PIXCLK_75000  ,   /* HDMITX_VFMT_PC_1024x768p_70Hz  */
	E_PIXCLK_31500  ,   /* HDMITX_VFMT_PC_640x480p_72Hz   */
	E_PIXCLK_50000  ,   /* HDMITX_VFMT_PC_800x600p_72Hz   */
	E_PIXCLK_31500  ,   /* HDMITX_VFMT_PC_640x480p_75Hz   */
	E_PIXCLK_78750  ,   /* HDMITX_VFMT_PC_1024x768p_75Hz  */
	E_PIXCLK_49500  ,   /* HDMITX_VFMT_PC_800x600p_75Hz   */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1024x864p_75Hz  */
	E_PIXCLK_135000 ,   /* HDMITX_VFMT_PC_1280x1024p_75Hz */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_640x350p_85Hz   */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_640x400p_85Hz   */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_720x400p_85Hz   */
	E_PIXCLK_36000  ,   /* HDMITX_VFMT_PC_640x480p_85Hz   */
	E_PIXCLK_56250  ,   /* HDMITX_VFMT_PC_800x600p_85Hz   */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1024x768p_85Hz  */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1152x864p_85Hz  */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1280x960p_85Hz  */
	E_PIXCLK_INVALID,   /* HDMITX_VFMT_PC_1280x1024p_85Hz */
	E_PIXCLK_INVALID    /* HDMITX_VFMT_PC_1024x768i_87Hz  */
};
#endif /* FORMAT_PC */

/**
 * Lookup table to convert from EIA/CEA TV video formats used in the EDID and in
 * API parameters to the format used in the E_REG_P00_VIDFORMAT_W register
 * */

#ifndef FORMAT_PC
static CONST_DAT u8 k_vfmt_to_regvfmt_tv[HDMITX_VFMT_TV_NUM] =
#else /* FORMAT_PC */
static CONST_DAT u8 k_vfmt_to_regvfmt_tv[HDMITX_VFMT_TV_NUM + HDMITX_VFMT_PC_NUM] =
#endif /* FORMAT_PC */
{
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_NULL               */
	e_regvfmt_640x480p_60hz,        /* HDMITX_VFMT_01_640x480p_60Hz   */
	e_regvfmt_720x480p_60hz,        /* HDMITX_VFMT_02_720x480p_60Hz   */
	e_regvfmt_720x480p_60hz,        /* HDMITX_VFMT_03_720x480p_60Hz   */
	e_regvfmt_1280x720p_60hz,       /* HDMITX_VFMT_04_1280x720p_60Hz  */
	e_regvfmt_1920x1080i_60hz,      /* HDMITX_VFMT_05_1920x1080i_60Hz */
	e_regvfmt_720x480i_60hz,        /* HDMITX_VFMT_06_720x480i_60Hz   */
	e_regvfmt_720x480i_60hz,        /* HDMITX_VFMT_07_720x480i_60Hz   */
	e_regvfmt_720x240p_60hz,        /* HDMITX_VFMT_08_720x240p_60Hz   */
	e_regvfmt_720x240p_60hz,        /* HDMITX_VFMT_09_720x240p_60Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_10_720x480i_60Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_11_720x480i_60Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_12_720x240p_60Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_13_720x240p_60Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_14_1440x480p_60Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_15_1440x480p_60Hz  */
	e_regvfmt_1920x1080p_60hz,      /* HDMITX_VFMT_16_1920x1080p_60Hz */
	e_regvfmt_720x576p_50hz,        /* HDMITX_VFMT_17_720x576p_50Hz   */
	e_regvfmt_720x576p_50hz,        /* HDMITX_VFMT_18_720x576p_50Hz   */
	e_regvfmt_1280x720p_50hz,       /* HDMITX_VFMT_19_1280x720p_50Hz  */
	e_regvfmt_1920x1080i_50hz,      /* HDMITX_VFMT_20_1920x1080i_50Hz */
	e_regvfmt_720x576i_50hz,        /* HDMITX_VFMT_21_720x576i_50Hz   */
	e_regvfmt_720x576i_50hz,        /* HDMITX_VFMT_22_720x576i_50Hz   */
	e_regvfmt_720x288p_50hz,        /* HDMITX_VFMT_23_720x288p_50Hz   */
	e_regvfmt_720x288p_50hz,        /* HDMITX_VFMT_24_720x288p_50Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_25_720x576i_50Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_26_720x576i_50Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_27_720x288p_50Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_28_720x288p_50Hz   */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_29_1440x576p_50Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_30_1440x576p_50Hz  */
	e_regvfmt_1920x1080p_50hz,      /* HDMITX_VFMT_31_1920x1080p_50Hz */
	e_regvfmt_1920x1080p_24hz,      /* HDMITX_VFMT_32_1920x1080p_24Hz */
	e_regvfmt_1920x1080p_25hz,      /* HDMITX_VFMT_33_1920x1080p_25Hz */
	e_regvfmt_1920x1080p_30hz       /* HDMITX_VFMT_34_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, e_regvfmt_640x480p_60hz,       /* HDMITX_VFMT_PC_640x480p_60Hz  */
	e_regvfmt_800x600p_60hz,        /* HDMITX_VFMT_PC_800x600p_60Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1152x960p_60Hz  */
	e_regvfmt_1024x768p_60hz,       /* HDMITX_VFMT_PC_1024x768p_60Hz  */
	e_regvfmt_1280x768p_60hz,       /* HDMITX_VFMT_PC_1280x768p_60Hz */
	e_regvfmt_1280x1024p_60hz,      /* HDMITX_VFMT_PC_1280x1024p_60Hz  */
	e_regvfmt_1360x768p_60hz,       /* HDMITX_VFMT_PC_1360x768p_60Hz */
	e_regvfmt_1400x1050p_60hz,      /* HDMITX_VFMT_PC_1400x1050p_60Hz */
	e_regvfmt_1600x1200p_60hz,      /* HDMITX_VFMT_PC_1600x1200p_60Hz  */
	e_regvfmt_1024x768p_70hz,       /* HDMITX_VFMT_PC_1024x768p_70Hz  */
	e_regvfmt_640x480p_72hz,        /* HDMITX_VFMT_PC_640x480p_72Hz  */
	e_regvfmt_800x600p_72hz,        /* HDMITX_VFMT_PC_800x600p_72Hz  */
	e_regvfmt_640x480p_75hz,        /* HDMITX_VFMT_PC_640x480p_75Hz  */
	e_regvfmt_1024x768p_75hz,       /* HDMITX_VFMT_PC_1024x768p_75Hz  */
	e_regvfmt_800x600p_75hz,        /* HDMITX_VFMT_PC_800x600p_75Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1024x864p_75Hz  */
	e_regvfmt_1280x1024p_75hz,      /* HDMITX_VFMT_PC_1280x1024p_75Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_640x350p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_640x400p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_720x400p_85Hz  */
	e_regvfmt_640x480p_85hz,        /* HDMITX_VFMT_PC_640x480p_85Hz  */
	e_regvfmt_800x600p_85hz,        /* HDMITX_VFMT_PC_800x600p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1024x768p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1152x864p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1280x960p_85Hz  */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1280x1024p_85Hz */
	E_REGVFMT_INVALID,              /* HDMITX_VFMT_PC_1024x768i_87Hz  */
#endif /* FORMAT PC */

};

/**
 * Lookup table to convert from EIA/CEA TV video formats used in API
 * parameters to the vid_format_i and vid_format_o values used in the
 * E_REG_P01_SC_VIDFORMAT_W register
 * */
#define SCIO(scin,scout)            (((scin)<<4)|(scout))
#define SCIO2SCIN(scio)             (u8)(((scio)>>4)&0xF)
#define SCIO2SCOUT(scio)            (u8)((scio)&0xF)

static CONST_DAT u8 k_vfmt_to_regvfmt_scio_tv[HDMITX_VFMT_TV_NUM] = {
	/* HDMITX_VFMT_NULL               */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_01_640x480p_60Hz   */
	SCIO(e_regvfmt_scin_480p_60hz, e_regvfmt_scout_480p_60hz),
	/* HDMITX_VFMT_02_720x480p_60Hz   */
	SCIO(e_regvfmt_scin_480p_60hz, e_regvfmt_scout_480p_60hz),
	/* HDMITX_VFMT_03_720x480p_60Hz   */
	SCIO(e_regvfmt_scin_480p_60hz, e_regvfmt_scout_480p_60hz),
	/* HDMITX_VFMT_04_1280x720p_60Hz  */
	SCIO(E_REGVFMT_SCIN_INVALID,   e_regvfmt_scout_720p_50hz_60hz),
	/* HDMITX_VFMT_05_1920x1080i_60Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   e_regvfmt_scout_1080i_50hz_60hz),
	/* HDMITX_VFMT_06_720x480i_60Hz   */
	SCIO(e_regvfmt_scin_480i_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_07_720x480i_60Hz   */
	SCIO(e_regvfmt_scin_480i_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_08_720x240p_60Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_09_720x240p_60Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_10_720x480i_60Hz   */
	SCIO(e_regvfmt_scin_480i_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_11_720x480i_60Hz   */
	SCIO(e_regvfmt_scin_480i_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_12_720x240p_60Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_13_720x240p_60Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_14_1440x480p_60Hz  */
	SCIO(e_regvfmt_scin_480p_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_15_1440x480p_60Hz  */
	SCIO(e_regvfmt_scin_480p_60hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_16_1920x1080p_60Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_17_720x576p_50Hz   */
	SCIO(e_regvfmt_scin_576p_50hz, e_regvfmt_scout_576ip_50hz),
	/* HDMITX_VFMT_18_720x576p_50Hz   */
	SCIO(e_regvfmt_scin_576p_50hz, e_regvfmt_scout_576ip_50hz),
	/* HDMITX_VFMT_19_1280x720p_50Hz  */
	SCIO(E_REGVFMT_SCIN_INVALID,   e_regvfmt_scout_720p_50hz_60hz),
	/* HDMITX_VFMT_20_1920x1080i_50Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   e_regvfmt_scout_1080i_50hz_60hz),
	/* HDMITX_VFMT_21_720x576i_50Hz   */
	SCIO(e_regvfmt_scin_576i_50hz, e_regvfmt_scout_576ip_50hz),
	/* HDMITX_VFMT_22_720x576i_50Hz   */
	SCIO(e_regvfmt_scin_576i_50hz, e_regvfmt_scout_576ip_50hz),
	/* HDMITX_VFMT_23_720x288p_50Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_24_720x288p_50Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_25_720x576i_50Hz   */
	SCIO(e_regvfmt_scin_576i_50hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_26_720x576i_50Hz   */
	SCIO(e_regvfmt_scin_576i_50hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_27_720x288p_50Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_28_720x288p_50Hz   */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_29_1440x576p_50Hz  */
	SCIO(e_regvfmt_scin_576p_50hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_30_1440x576p_50Hz  */
	SCIO(e_regvfmt_scin_576p_50hz, E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_31_1920x1080p_50Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_32_1920x1080p_24Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_33_1920x1080p_25Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID),
	/* HDMITX_VFMT_34_1920x1080p_30Hz */
	SCIO(E_REGVFMT_SCIN_INVALID,   E_REGVFMT_SCOUT_INVALID)
};

/**
 * Macro to pack output format flags as bits in a u16 for the following table
 * */
#define PKOPF(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17) \
	((p1)|((p2)<<1)|((p3)<<2)|((p4)<<3)|((p5)<<4)|((p6)<<5)|((p7)<<6)|((p8)<<7)| \
	 ((p9)<<8)|((p10)<<9)|((p11)<<10)|((p12)<<11)|((p13)<<12)|((p14)<<13)|((p15)<<14)|((p16)<<15)|((p17)<<16))

/* Macro to test for input and output format = '1' in following table */
#define CAN_FMTS_SCALE(fin,fout) (kCanSclInToOut[(fin)]&(1<<(fout)))

/**
 * Lookup table to determine if an input format can be scaled to an
 * output format, including scaler off conditions (in=out)
 * Indexed by [input format][output format]
 * */
static CONST_DAT u16 k_can_scl_in_to_out[E_REGVFMT_INVALID+1] = {
	/* -----OUTPUT FORMATS------ */
	/* E_REGVFMT_640x480p_60Hz   */
	/*    E_REGVFMT_720x480p_60Hz   */
	/*       E_REGVFMT_1280x720p_60Hz  */
	/*          E_REGVFMT_1920x1080i_60Hz */
	/*             E_REGVFMT_720x480i_60Hz   */
	/*                E_REGVFMT_720x240p_60Hz   */
	/*                   E_REGVFMT_1920x1080p_60Hz */
	/*                      E_REGVFMT_720x576p_50Hz   */
	/*                         E_REGVFMT_1280x720p_50Hz  */
	/*                            E_REGVFMT_1920x1080i_50Hz */
	/*                               E_REGVFMT_720x576i_50Hz   */
	/*                                  E_REGVFMT_720x288p_50Hz   */
	/*                                     E_REGVFMT_1920x1080p_50Hz */
	/*										  E_REGVFMT_1920x1080p_24Hz */
	/*											 E_REGVFMT_1920x1080p_25Hz */
	/*												E_REGVFMT_1920x1080p_30Hz */
	/*												   E_REGVFMT_INVALID		   */
	/* ------INPUT FORMATS------ */
	/* E_REGVFMT_640x480p_60Hz   */
	PKOPF(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x480p_60Hz   */
	PKOPF(0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1280x720p_60Hz  */
	PKOPF(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1920x1080i_60Hz */
	PKOPF(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x480i_60Hz   */
	PKOPF(0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x240p_60Hz   */
	PKOPF(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1920x1080p_60Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x576p_50Hz   */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1280x720p_50Hz  */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1920x1080i_50Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x576i_50Hz   */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0),
	/* E_REGVFMT_720x288p_50Hz   */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0),
	/* E_REGVFMT_1920x1080p_50Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0),
	/* E_REGVFMT_1920x1080p_24Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0),
	/* E_REGVFMT_1920x1080p_25Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0),
	/* E_REGVFMT_1920x1080p_30Hz */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0),
	/* E_REGVFMT_INVALID         */
	PKOPF(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
};

/**
 * Lookup tables to convert scaler input and output modes
 * to PLL scaling values
 * */
static CONST_DAT u16 k_scl_mode_to_scg_n[E_REGVFMT_SCIN_NUM][E_REGVFMT_SCOUT_NUM] = {
	/* E_REGVFMT_SCOUT_480p_60Hz */
	/*        E_REGVFMT_SCOUT_576ip_50Hz */
	/*                E_REGVFMT_SCOUT_720p_50Hz_60Hz */
	/*                        E_REGVFMT_SCOUT_1080i_50Hz_60Hz */
	{ 20,     0,    500,    500},   /* E_REGVFMT_SCIN_480i_60Hz */
	{  0,    20,     88,     88},   /* E_REGVFMT_SCIN_576i_50Hz */
	{  0,     0,    250,    250},   /* E_REGVFMT_SCIN_480p_60Hz */
	{  0,     0,     44,     44},   /* E_REGVFMT_SCIN_576p_50Hz */
};
static CONST_DAT u16 k_scl_mode_to_scg_r[E_REGVFMT_SCIN_NUM][E_REGVFMT_SCOUT_NUM] = {
	/* E_REGVFMT_SCOUT_480p_60Hz */
	/*        E_REGVFMT_SCOUT_576ip_50Hz */
	/*                E_REGVFMT_SCOUT_720p_50Hz_60Hz */
	/*                        E_REGVFMT_SCOUT_1080i_50Hz_60Hz */
	{ 10,     0,     91,     91},   /* E_REGVFMT_SCIN_480i_60Hz */
	{  0,    10,     16,     16},   /* E_REGVFMT_SCIN_576i_50Hz */
	{  0,     0,     91,     91},   /* E_REGVFMT_SCIN_480p_60Hz */
	{  0,     0,     16,     16},   /* E_REGVFMT_SCIN_576p_50Hz */
};

/**
 * Lookup tables to hide the scaler issue on side effect
 * by adding blanking pixel on right and left side of the screen
 * */
static CONST_DAT u8 k_scl_add_blk_pix[E_REGVFMT_SCIN_NUM][E_REGVFMT_SCOUT_NUM] = {
	/* E_REGVFMT_SCOUT_480p_60Hz */
	/*        E_REGVFMT_SCOUT_576ip_50Hz */
	/*                E_REGVFMT_SCOUT_720p_50Hz_60Hz */
	/*                        E_REGVFMT_SCOUT_1080i_50Hz_60Hz */
	{  0,     0,      4,      8},   /* E_REGVFMT_SCIN_480i_60Hz */
	{  0,     0,      4,      8},   /* E_REGVFMT_SCIN_576i_50Hz */
	{  0,     0,      4,      8},   /* E_REGVFMT_SCIN_480p_60Hz */
	{  0,     0,      4,      8},   /* E_REGVFMT_SCIN_576p_50Hz */
};
static CONST_DAT u8 k_scl_clear_blk_pix[] = {
	0,    /* HDMITX_VOUTMODE_RGB444 */
	1,    /* HDMITX_VOUTMODE_YUV422 */
	1,    /* HDMITX_VOUTMODE_YUV444 */
	0,    /* HDMITX_VOUTMODE_NO_CHANGE */
	0     /* HDMITX_VOUTMODE_INVALID */
};

/**
 * Lookup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to corresponding VS_PIX_STRT_2
 * register values, to correct the output window for interlaced
 * output formats, with or without the scaler.
 *
 * The correction is VS_PIX_STRT_2=VS_PIX_STRT_2+VS_PIX_STRT_1.
 * The same value is also applied to VS_PIX_END_2.
 * */
static CONST_DAT u16 k_regvfmt_to_vs2[E_REGVFMT_NUM] = {
	0,            /* E_REGVFMT_640x480p_60Hz   */
	0,            /* E_REGVFMT_720x480p_60Hz   */
	0,            /* E_REGVFMT_1280x720p_60Hz  */
	1100 + 88,    /* E_REGVFMT_1920x1080i_60Hz */
	429  + 19,    /* E_REGVFMT_720x480i_60Hz   */
	0,            /* E_REGVFMT_720x240p_60Hz   */
	0,            /* E_REGVFMT_1920x1080p_60Hz */
	0,            /* E_REGVFMT_720x576p_50Hz   */
	0,            /* E_REGVFMT_1280x720p_50Hz  */
	1320 + 528,   /* E_REGVFMT_1920x1080i_50Hz */
	432  + 12,    /* E_REGVFMT_720x576i_50Hz   */
	0,            /* E_REGVFMT_720x288p_50Hz   */
	0,            /* E_REGVFMT_1920x1080p_50Hz */
	0,            /* E_REGVFMT_1920x1080p_24Hz */
	0,            /* E_REGVFMT_1920x1080p_25Hz */
	0             /* E_REGVFMT_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, 0, /* E_REGVFMT_640x480p_72Hz   */
	0, /* E_REGVFMT_640x480p_75Hz   */
	0, /* E_REGVFMT_640x480p_85Hz   */
	0, /* E_REGVFMT_800x600p_60Hz   */
	0, /* E_REGVFMT_800x600p_72Hz   */
	0, /* E_REGVFMT_800x600p_75Hz   */
	0, /* E_REGVFMT_800x600p_85Hz   */
	0, /* E_REGVFMT_1024x768p_60Hz  */
	0, /* E_REGVFMT_1024x768p_70Hz  */
	0, /* E_REGVFMT_1024x768p_75Hz  */
	0, /* E_REGVFMT_1280x768p_60Hz  */
	0, /* E_REGVFMT_1280x1024p_60Hz */
	0, /* E_REGVFMT_1280x1024p_75Hz */
	0, /* E_REGVFMT_1360x768p_60Hz  */
	0, /* E_REGVFMT_1400x1050p_60Hz */
	0, /* E_REGVFMT_1600x1200p_60Hz */
#endif /* FORMAT_PC */
};

/**
 * Lookup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to corresponding
 * pixel repetition values in the PLL_SERIAL_2 register.
 * 0=no repetition (pixel sent once)
 * 1=one repetition (pixel sent twice) etc
 * */
static CONST_DAT u8 k_regvfmt_to_pix_rep[E_REGVFMT_NUM] = {
	0, /* E_REGVFMT_640x480p_60Hz   */
	0, /* E_REGVFMT_720x480p_60Hz   */
	0, /* E_REGVFMT_1280x720p_60Hz  */
	0, /* E_REGVFMT_1920x1080i_60Hz */
	1, /* E_REGVFMT_720x480i_60Hz   */
	1, /* E_REGVFMT_720x240p_60Hz   */
	0, /* E_REGVFMT_1920x1080p_60Hz */
	0, /* E_REGVFMT_720x576p_50Hz   */
	0, /* E_REGVFMT_1280x720p_50Hz  */
	0, /* E_REGVFMT_1920x1080i_50Hz */
	1, /* E_REGVFMT_720x576i_50Hz   */
	1, /* E_REGVFMT_720x288p_50Hz   */
	0, /* E_REGVFMT_1920x1080p_50Hz */
	0, /* E_REGVFMT_1920x1080p_24Hz */
	0, /* E_REGVFMT_1920x1080p_25Hz */
	0  /* E_REGVFMT_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, 0, /* E_REGVFMT_640x480p_72Hz   */
	0, /* E_REGVFMT_640x480p_75Hz   */
	0, /* E_REGVFMT_640x480p_85Hz   */
	0, /* E_REGVFMT_800x600p_60Hz   */
	0, /* E_REGVFMT_800x600p_72Hz   */
	0, /* E_REGVFMT_800x600p_75Hz   */
	0, /* E_REGVFMT_800x600p_85Hz   */
	0, /* E_REGVFMT_1024x768p_60Hz  */
	0, /* E_REGVFMT_1024x768p_70Hz  */
	0, /* E_REGVFMT_1024x768p_75Hz  */
	0, /* E_REGVFMT_1280x768p_60Hz  */
	0, /* E_REGVFMT_1280x1024p_60Hz */
	0, /* E_REGVFMT_1280x1024p_75Hz */
	0, /* E_REGVFMT_1360x768p_60Hz  */
	0, /* E_REGVFMT_1400x1050p_60Hz */
	0, /* E_REGVFMT_1600x1200p_60Hz */
#endif /* FORMAT_PC */

};

/**
 * Lookup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to corresponding
 * trios of 2-bit values in the srl_nosc, scg_nosc and de_nosc
 * PLL control registers
 * */
#define SSD(srl,scg,de) (((srl)<<4)|((scg)<<2)|(de))
#define SSD2SRL(ssd)    (u8)(((ssd)>>4)&3)
#define SSD2SCG(ssd)    (u8)(((ssd)>>2)&3)
#define SSD2DE(ssd)     (u8)((ssd)&3)

static CONST_DAT struct {
	u8 sca_off_ccir_off_dbl_edge_off[E_REGVFMT_NUM_TV];

	u8 sca_off_ccir_on_dbl_edge_off[E_REGVFMT_NUM_TV];

	u8 sca_off_ccir_on_dbl_edge_on[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_off_dbl_edge_off_interlace[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_on_dbl_edge_off_interlace[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_on_dbl_edge_on_interlace[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_off_dbl_edge_off_progressif[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_on_dbl_edge_off_progressif[E_REGVFMT_NUM_TV];

	u8 sca_on_ccir_on_dbl_edge_on_progressif[E_REGVFMT_NUM_TV];

#ifdef FORMAT_PC
	u8 settings_format_pc[E_REGVFMT_NUM_PC];
#endif /* FORMAT_PC */

} k_regvfmt_to_pll_ssd = {

	{   /*ScaOffCCIROffDblEdgeOff*/
		/* SRL,SCG,DE */
		SSD(2, 2, 2), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 2, 2), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 1, 1), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 1, 1), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(3, 3, 3), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 2, 2), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 1, 1), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 1, 1), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(3, 3, 3), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 1, 1), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 1, 1), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 1, 1) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /*ScaOffCCIROnDblEdgeOff*/
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 1, 1), /* E_REGVFMT_720x480p_60Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(2, 2, 2), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 1, 1), /* E_REGVFMT_720x576p_50Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(2, 2, 2), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(0xF, 0, 0) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /*ScaOffCcirONDblEdgeOn*/
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 1, 1), /* E_REGVFMT_720x480p_60Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(2, 2, 2), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 1, 1), /* E_REGVFMT_720x576p_50Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(2, 2, 2), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(0xF, 0, 0) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /* ScaOnCcirOffDblEdgeOffInterlace */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 2, 3), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 1, 3), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 1, 3), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 2, 3), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 1, 3), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 1, 3), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 1, 3), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 1, 3), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 1, 3) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /* ScaOnCcirONDblEdgeOffInterlace */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 1, 2), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 0, 2), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 1, 2), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 0, 2), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 0, 2) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{  /* ScaOnCcirONDblEdgeOnInterlace */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(2, 1, 2), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 0, 2), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(2, 1, 2), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 0, 2), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 0, 2), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 0, 2) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{ /* ScaOnCcirOffDblEdgeOffProgressif */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 1, 2), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 1, 2), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 1, 2), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 1, 2), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 1, 2), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 1, 2), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 1, 2) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /* ScaOnCcirOnDblEdgeOffProgressif */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 0, 1), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 0, 1), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 0, 1) /* E_REGVFMT_1920x1080p_30Hz */
	},

	{   /* ScaOnCcirONDblEdgeOnProgressif */
		/* SRL,SCG,DE */
		SSD(0xF, 0, 0), /* E_REGVFMT_640x480p_60Hz   */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480p_60Hz   */
		SSD(1, 0, 1), /* E_REGVFMT_1280x720p_60Hz  */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080i_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x480i_60Hz   */
		/* E_REGVFMT_720x240p_60Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_60Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576p_50Hz   */
		SSD(1, 0, 1), /* E_REGVFMT_1280x720p_50Hz  */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080i_50Hz */
		SSD(0xF, 0, 0), /* E_REGVFMT_720x576i_50Hz   */
		/* E_REGVFMT_720x288p_50Hz    / * \todo Need nosc PLL value */
		SSD(0xF, 0, 0),
		SSD(0xF, 0, 0), /* E_REGVFMT_1920x1080p_50Hz */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080p_24Hz */
		SSD(1, 0, 1), /* E_REGVFMT_1920x1080p_25Hz */
		SSD(1, 0, 1) /* E_REGVFMT_1920x1080p_30Hz */
	}

#ifdef FORMAT_PC
	, {
		/* SRL,SCG,DE */
		SSD(2, 0, 0), /* E_REGVFMT_640x480p_72Hz   */
		SSD(2, 0, 0), /* E_REGVFMT_640x480p_75Hz   */
		SSD(2, 0, 0), /* E_REGVFMT_640x480p_85Hz   */
		SSD(1, 0, 0), /* E_REGVFMT_800x600p_60Hz   */
		SSD(1, 0, 0), /* E_REGVFMT_800x600p_72Hz   */
		SSD(1, 0, 0), /* E_REGVFMT_800x600p_75Hz   */
		SSD(1, 0, 0), /* E_REGVFMT_800x600p_85Hz   */
		SSD(1, 0, 0), /* E_REGVFMT_1024x768p_60Hz  */
		SSD(1, 0, 0), /* E_REGVFMT_1024x768p_70Hz  */
		SSD(1, 0, 0), /* E_REGVFMT_1024x768p_75Hz  */
		SSD(0, 0, 0), /* E_REGVFMT_1280x768p_60Hz  */
		SSD(0, 0, 0), /* E_REGVFMT_1280x1024p_60Hz */
		SSD(0, 0, 0), /* E_REGVFMT_1280x1024p_75Hz */
		SSD(0, 0, 0), /* E_REGVFMT_1360x768p_60Hz  */
		SSD(0, 0, 0), /* E_REGVFMT_1400x1050p_60Hz */
		SSD(0, 0, 0)  /* E_REGVFMT_1600x1200p_60Hz */
	}
#endif /* FORMAT_PC */

};

/**
 * Lookup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to corresponding
 * V_toggle values for Vs external sync.
 * 0=no toggle
 * 1=toggle
 * */
static CONST_DAT u8 k_regvfmt_to_vtoggle[E_REGVFMT_NUM] = {
	1, /* E_REGVFMT_640x480p_60Hz   */
	1, /* E_REGVFMT_720x480p_60Hz   */
	0, /* E_REGVFMT_1280x720p_60Hz  */
	0, /* E_REGVFMT_1920x1080i_60Hz */
	1, /* E_REGVFMT_720x480i_60Hz   */
	1, /* E_REGVFMT_720x240p_60Hz   */
	0, /* E_REGVFMT_1920x1080p_60Hz */
	1, /* E_REGVFMT_720x576p_50Hz   */
	0, /* E_REGVFMT_1280x720p_50Hz  */
	0, /* E_REGVFMT_1920x1080i_50Hz */
	1, /* E_REGVFMT_720x576i_50Hz   */
	1, /* E_REGVFMT_720x288p_50Hz   */
	0, /* E_REGVFMT_1920x1080p_50Hz */
	0, /* E_REGVFMT_1920x1080p_24Hz */
	0, /* E_REGVFMT_1920x1080p_25Hz */
	0  /* E_REGVFMT_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, 1, /* E_REGVFMT_640x480p_72Hz   */
	1, /* E_REGVFMT_640x480p_75Hz   */
	1, /* E_REGVFMT_640x480p_85Hz   */
	0, /* E_REGVFMT_800x600p_60Hz   */
	0, /* E_REGVFMT_800x600p_72Hz   */
	0, /* E_REGVFMT_800x600p_75Hz   */
	0, /* E_REGVFMT_800x600p_85Hz   */
	1, /* E_REGVFMT_1024x768p_60Hz  */
	1, /* E_REGVFMT_1024x768p_70Hz  */
	0, /* E_REGVFMT_1024x768p_75Hz  */
	0, /* E_REGVFMT_1280x768p_60Hz  */
	0, /* E_REGVFMT_1280x1024p_60Hz */
	0, /* E_REGVFMT_1280x1024p_75Hz */
	0, /* E_REGVFMT_1360x768p_60Hz  */
	0, /* E_REGVFMT_1400x1050p_60Hz */
	0, /* E_REGVFMT_1600x1200p_60Hz */
#endif /* FORMAT_PC */
};

/**
 * Lookup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to corresponding
 * H_toggle values for Vs external sync.
 * 0=no toggle
 * 1=toggle
 * */
static CONST_DAT u8 k_regvfmt_to_htoggle[E_REGVFMT_NUM] = {
	1, /* E_REGVFMT_640x480p_60Hz   */
	1, /* E_REGVFMT_720x480p_60Hz   */
	0, /* E_REGVFMT_1280x720p_60Hz  */
	0, /* E_REGVFMT_1920x1080i_60Hz */
	1, /* E_REGVFMT_720x480i_60Hz   */
	1, /* E_REGVFMT_720x240p_60Hz   */
	0, /* E_REGVFMT_1920x1080p_60Hz */
	1, /* E_REGVFMT_720x576p_50Hz   */
	0, /* E_REGVFMT_1280x720p_50Hz  */
	0, /* E_REGVFMT_1920x1080i_50Hz */
	1, /* E_REGVFMT_720x576i_50Hz   */
	1, /* E_REGVFMT_720x288p_50Hz   */
	0, /* E_REGVFMT_1920x1080p_50Hz */
	0, /* E_REGVFMT_1920x1080p_24Hz */
	0, /* E_REGVFMT_1920x1080p_25Hz */
	0  /* E_REGVFMT_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, 1, /* E_REGVFMT_640x480p_72Hz   */
	1, /* E_REGVFMT_640x480p_75Hz   */
	1, /* E_REGVFMT_640x480p_85Hz   */
	0, /* E_REGVFMT_800x600p_60Hz   */
	0, /* E_REGVFMT_800x600p_72Hz   */
	0, /* E_REGVFMT_800x600p_75Hz   */
	0, /* E_REGVFMT_800x600p_85Hz   */
	1, /* E_REGVFMT_1024x768p_60Hz  */
	1, /* E_REGVFMT_1024x768p_70Hz  */
	0, /* E_REGVFMT_1024x768p_75Hz  */
	1, /* E_REGVFMT_1280x768p_60Hz  */
	0, /* E_REGVFMT_1280x1024p_60Hz */
	0, /* E_REGVFMT_1280x1024p_75Hz */
	0, /* E_REGVFMT_1360x768p_60Hz  */
	1, /* E_REGVFMT_1400x1050p_60Hz */
	0, /* E_REGVFMT_1600x1200p_60Hz */
#endif /* FORMAT_PC */
};

/**
 * Lokup table to convert from video format codes used in the
 * E_REG_P00_VIDFORMAT_W register to RefPix and RefLine values
 * according to sync source
 * */
static CONST_DAT struct {
	u16  ref_pix_vs_sync;    /* Output values for Vs/Hs input sync */
	u16  ref_line_vs_sync;
	/* Output values for all other input sync sources */
	u16  ref_pix_other_sync;
	u16  ref_line_other_sync;
} k_vid_fmt_to_refpix_refline[E_REGVFMT_NUM] = {
	/*  PixVs   LineVs  PixOther    LineOther   vinFmt */
	{17,    2,      161,        36},     /* E_REGVFMT_640x480p_60Hz   */
	{17,    8,      139,        43},     /* E_REGVFMT_720x480p_60Hz   */
	{111,   2,      371,        26},     /* E_REGVFMT_1280x720p_60Hz  */
	{89,    2,      281,        21},     /* E_REGVFMT_1920x1080i_60Hz */
	{20,    5,      139,        22},     /* E_REGVFMT_720x480i_60Hz   */
	{20,    5,      139,        22},     /* E_REGVFMT_720x240p_60Hz   */
	{89,    2,      281,        42},     /* E_REGVFMT_1920x1080p_60Hz */
	{13,    2,      145,        45},     /* E_REGVFMT_720x576p_50Hz   */
	{441,   2,      701,        26},     /* E_REGVFMT_1280x720p_50Hz  */
	{529,   2,      721,        21},     /* E_REGVFMT_1920x1080i_50Hz */
	{13,    2,      145,        23},     /* E_REGVFMT_720x576i_50Hz   */
	{13,    2,      145,        23},     /* E_REGVFMT_720x288p_50Hz   */
	{529,   2,      721,        42},     /* E_REGVFMT_1920x1080p_50Hz */
	{639,   2,		831,        42},     /* E_REGVFMT_1920x1080p_24Hz */
	{529,   2,		721,        42},     /* E_REGVFMT_1920x1080p_25Hz */
	{89,    2,		281,        42}      /* E_REGVFMT_1920x1080p_30Hz */
#ifdef FORMAT_PC
	, {25,    2,      195,        32}, /* E_REGVFMT_640x480p_72Hz   */
	{17,    2,      203,        20}, /* E_REGVFMT_640x480p_75Hz   */
	{57,    2,      195,        29}, /* E_REGVFMT_640x480p_85Hz   */
	{41,    2,      259,        28}, /* E_REGVFMT_800x600p_60Hz   */
	{57,    2,      243,        30}, /* E_REGVFMT_800x600p_72Hz   */
	{17,    2,      259,        25}, /* E_REGVFMT_800x600p_75Hz   */
	{33,    2,      251,        31}, /* E_REGVFMT_800x600p_85Hz   */
	{25,    2,      323,        36}, /* E_REGVFMT_1024x768p_60Hz  */
	{25,    2,      307,        36}, /* E_REGVFMT_1024x768p_70Hz  */
	{17,    2,      291,        32}, /* E_REGVFMT_1024x768p_75Hz  */
	{65,    2,      387,        28}, /* E_REGVFMT_1280x768p_60Hz */
	{49,    2,      501,        42}, /* E_REGVFMT_1280x1024p_60Hz */
	{17,    2,      411,        42}, /* E_REGVFMT_1280x1024p_75Hz */
	{65,    2,      435,        25}, /* E_REGVFMT_1360x768p_60Hz */
	{89,    2,      467,        37}, /* E_REGVFMT_1400x1050p_60Hz */
	{65,	2,		563,		50}  /* E_REGVFMT_1600x1200p_60Hz */
#endif/* FORMAT_PC */
};

static CONST_DAT hdmi_tx_vid_reg_t k_vid_fmt_format_param[E_REGVFMT_NUM_TV_NO_REG] = {
	/*  NPIX    NLINE  VsLineStart	VsPixStart	VsLineEnd	VsPixEnd	HsStart		HsEnd	vWinStart	vWinEnd	DeStart	DeEnd */
	/* E_REGVFMT_1920x1080p_24Hz */
	{2750,	1125,	  1,	      638,	       6,	        638,        638,    682,    41,         1121,   830,   2750},
	/* E_REGVFMT_1920x1080p_25Hz */
	{2640,	1125,	  1,	      528,	       6,	        528,	    528,	572,    41,         1121,   720,   2640},
	/* E_REGVFMT_1920x1080p_30Hz */
	{2200,	1125,	  1,	      88,	       6,	        88,	        88,	    132,    41,         1121,   280,   2200},

};

#ifdef FORMAT_PC
static CONST_DAT hdmi_tx_vid_reg_t k_vid_fmt_to_pcformat_param[E_REGVFMT_NUM_PC] = {
	/*  NPIX    NLINE  VsLineStart	VsPixStart	VsLineEnd	VsPixEnd	HsStart		HsEnd	vWinStart	vWinEnd	DeStart	DeEnd */
	/* E_REGVFMT_640x480p_72Hz   */
	{832,	520,      1,          24,	       4,	        24,	        24,	    64,	    31,	        511,    192,    832},
	/* E_REGVFMT_640x480p_75Hz   */
	{840,	500,      1,	      16,	       4,	        16,         16,	    80,	    19,	        499,    200,    840},
	/* E_REGVFMT_640x480p_85Hz   */
	{832,	509,      1,	      56,	       4,	        56,	        56,	    112,    28,         508,    192,    832},
	/* E_REGVFMT_800x600p_60Hz   */
	{1056,	628,	  1,	      40,	       5,	        40,	        40,    	168,    27,         627,    256,   1056},
	/* E_REGVFMT_800x600p_72Hz   */
	{1040,	666,	  1,	      56,	       7,	        56,	        56,	    176,    29,         619,    240,   1040},
	/* E_REGVFMT_800x600p_75Hz   */
	{1056,	625,	  1,	      16,	       4,           16,	        16,	    96,     24,         624,    256,   1056},
	/* E_REGVFMT_800x600p_85Hz   */
	{1048,	631,	  1,	      32,	       4,	        32,	        32,	    96,     30,         630,    248,   1048},
	/* E_REGVFMT_1024x768p_60Hz  */
	{1344,	806,	  1,	      24,	       7,	        24,	        24,	    160,    35,         803,    320,   1344},
	/* E_REGVFMT_1024x768p_70Hz  */
	{1328,	806,	  1,	      24,	       7,	        24,	        24,	    160,    35,         803,    304,   1328},
	/* E_REGVFMT_1024x768p_75Hz  */
	{1312,	800,	  1,	      16,	       4,	        16,	        16,	    112,    31,         799,    288,   1312},
	/* E_REGVFMT_1280x768p_60Hz  */
	{1664,	798,	  1,	      64,	       8,	        64,	        64,	    192,    27,         795,    384,   1664},
	/* E_REGVFMT_1280x1024p_60Hz */
	{1688,	1066,	  1,	      48,	       4,	        48,	        48,	    160,    41,         1065,   408,   1688},
	/* E_REGVFMT_1280x1024p_75Hz */
	{1688,	1066,	  1,	      16,	       4,	        16,	        16,	    160,    41,         1065,   408,   1688},
	/* E_REGVFMT_1360x768p_60Hz  */
	{1792,	795,	  1,	      64,	       7,	        64,	        64,	    176,    24,         792,    432,   1792},
	/* E_REGVFMT_1400x1050p_60Hz */
	{1864,	1089,	  1,	      88,	       5,	        88,	        88,	    232,    36,         1086,   464,   1864},
	/* E_REGVFMT_1600x1200p_60Hz */
	{2160,	1250,	  1,	      64,	       4,	        64,	        64,	    256,    49,         1249,   560,   2160}
};
#endif/* FORMAT_PC */

/**
 *  lookup table for each pixel clock frequency's CTS value in k_hz
 *  according to SCS table "audio clock recovery CTS values"
 * */
static CONST_DAT u32 k_pix_clk_to_acr_cts[E_PIXCLK_NUM][HDMITX_AFS_NUM] = {
	/* HDMITX_AFS_32k  _AFS_48K       _AFS_96K        _AFS_192K */
	/*         _AFS_44_1k      _AFS_88_2K      _AFS_176_4K       */
	/* E_PIXCLK_25175 */
	{ 28125,  31250,  28125,  31250,  28125,  31250,  28125},
	/* E_PIXCLK_25200 */
	{ 25200,  28000,  25200,  28000,  25200,  28000,  25200},
	/* E_PIXCLK_27000 */
	{ 27000,  30000,  27000,  30000,  27000,  30000,  27000},
	/* E_PIXCLK_27027 */
	{ 27027,  30030,  27027,  30030,  27027,  30030,  27027},
	/* E_PIXCLK_54000 */
	{ 54000,  60000,  54000,  60000,  54000,  60000,  54000},
	/* E_PIXCLK_54054 */
	{ 54054,  60060,  54054,  60060,  54054,  60060,  54054},
	/* E_PIXCLK_74175 */
	{210937, 234375, 140625, 234375, 140625, 234375, 140625},
	/* E_PIXCLK_74250 */
	{ 74250,  82500,  74250,  82500,  74250,  82500,  74250},
	/* E_PIXCLK_148350*/
	{421875, 234375, 140625, 234375, 140625, 234375, 140625},
	/* E_PIXCLK_148500*/
	{148500, 165000, 148500, 165000, 148500, 165000, 148500}
#ifdef FORMAT_PC
	/* E_PIXCLK_31500 */
	, { 31500,  35000,  31500,  35000,  31500,  35000,  31500},
	/* E_PIXCLK_36000 */
	{ 36000,  40000,  36000,  40000,  36000,  40000,  36000},
	/* E_PIXCLK_40000 */
	{ 40000,  44444,  40000,  44444,  40000,  44444,  40000},
	/* E_PIXCLK_49500 */
	{ 49500,  55000,  49500,  55000,  49500,  55000,  49500},
	/* E_PIXCLK_50000 */
	{ 50000,  55556,  50000,  55556,  50000,  55556,  50000},
	/* E_PIXCLK_56250 */
	{ 56250,  62500,  56250,  62500,  56250,  62500,  56250},
	/* E_PIXCLK_65000 */
	{ 65000,  72222,  65000,  72222,  65000,  72222,  65000},
	/* E_PIXCLK_75000 */
	{ 75000,  83333,  75000,  83333,  75000,  83333,  75000},
	/* E_PIXCLK_78750 */
	{ 78750,  87500,  78750,  87500,  78750,  87500,  78750},
	/* E_PIXCLK_135000 */
	{135000, 150000, 135000, 150000, 135000, 150000, 135000},
	/* E_PIXCLK_162000*/
	{162000, 180000, 162000, 180000, 162000, 180000, 162000}
#endif /* FORMAT_PC */
};

/**
 *  Lookup table for each pixel clock frequency's Audio Clock Regeneration N,
 *  according to SCS Table "Audio Clock Recovery N Values"
 * */

static CONST_DAT u32 k_pix_clk_to_acr_n[E_PIXCLK_NUM][HDMITX_AFS_NUM] = {
	/* HDMITX_AFS_32k  _AFS_48K       _AFS_96K        _AFS_192K */
	/*         _AFS_44_1k      _AFS_88_2K      _AFS_176_4K       */
	/* E_PIXCLK_25175 */
	{ 4576,   7007,   6864,  14014,  13728,  28028,  27456},
	/* E_PIXCLK_25200 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_27000 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_27027 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_54000 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_54054 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_74175 */
	{11648,  17836,  11648,  35672,  23296,  71344,  46592},
	/* E_PIXCLK_74250 */
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576},
	/* E_PIXCLK_148350*/
	{11648,   8918,   5824,  17836,  11648,  35672,  23296},
	/* E_PIXCLK_148500*/
	{ 4096,   6272,   6144,  12544,  12288,  25088,  24576}
#ifdef FORMAT_PC
	/* E_PIXCLK_31500 */
	, { 4096,  6272,  6144,  12544,  12288,  25088,  24576},
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_36000 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_40000 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_49500 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_50000 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_56250 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_65000 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_75000 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_78750 */
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}, /* E_PIXCLK_135000*/
	{ 4096,  6272,  6144,  12544,  12288,  25088,  24576}  /* E_PIXCLK_162000*/
#endif /* FORMAT_PC */
};

/**
 *  Lookup table for each pixel clock frequency's Audio Divider, according to
 *  SCS Table "Audio Clock Recovery Divider Values"
 * */
static CONST_DAT u8 k_pix_clk_to_adiv[E_PIXCLK_NUM][HDMITX_AFS_NUM] = {
	/* HDMITX_AFS_32k  _AFS_48K       _AFS_96K        _AFS_192K */
	/*         _AFS_44_1k      _AFS_88_2K      _AFS_176_4K       */
	/* E_PIXCLK_25175 */
	{2,      2,      2,      1,      1,      0,      0},
	/* E_PIXCLK_25200 */
	{2,      2,      2,      1,      1,      0,      0},
	/* E_PIXCLK_27000 */
	{2,      2,      2,      1,      1,      0,      0},
	/* E_PIXCLK_27027 */
	{2,      2,      2,      1,      1,      0,      0},
	/* E_PIXCLK_54000 */
	{3,      3,      3,      2,      2,      1,      1},
	/* E_PIXCLK_54054 */
	{3,      3,      3,      2,      2,      1,      1},
	/* E_PIXCLK_74175 */
	{4,      3,      3,      2,      2,      1,      1},
	/* E_PIXCLK_74250 */
	{4,      3,      3,      2,      2,      1,      1},
	/* E_PIXCLK_148350*/
	{5,      4,      4,      3,      3,      2,      2},
	/* E_PIXCLK_148500*/
	{5,      4,      4,      3,      3,      2,      2}
#ifdef FORMAT_PC
	, {2,      2,      2,      1,      1,      0,      0}, /* E_PIXCLK_31500 */
	{3,      2,      2,      1,      1,      0,      0}, /* E_PIXCLK_36000 */
	{3,      2,      2,      1,      1,      0,      0}, /* E_PIXCLK_40000 */
	{3,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_49500 */
	{3,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_50000 */
	{3,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_56250 */
	{4,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_65000 */
	{4,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_75000 */
	{4,      3,      3,      2,      2,      1,      1}, /* E_PIXCLK_78750 */
	{5,      4,      4,      3,      3,      2,      2}, /* E_PIXCLK_135000*/
	{5,      4,      4,      3,      3,      2,      2}  /* E_PIXCLK_162000*/
#endif /* FORMAT_PC */

};

/**
 *  Lookup table for converting a sampling frequency into the values
 *  required in channel status byte 3 according to IEC60958-3
 * */
static CONST_DAT u8 k_afs_to_csbyte3[HDMITX_AFS_NUM+1] = {
	3,      /* HDMITX_AFS_32k */
	0,      /* HDMITX_AFS_44_1k */
	2,      /* HDMITX_AFS_48k */
	8,      /* HDMITX_AFS_88_2k */
	10,     /* HDMITX_AFS_96k */
	12,     /* HDMITX_AFS_176_4k */
	14,     /* HDMITX_AFS_192k */
	1,      /* HDMITX_AFS_NOT_INDICATED */
};

/**
 *  Lookup table for each pixel clock frequency index's nearest MHz value
 * */
static CONST_DAT u8 k_pix_clk_to_mhz[E_PIXCLK_NUM] = {
	25,     /* E_PIXCLK_25175 */
	25,     /* E_PIXCLK_25200 */
	27,     /* E_PIXCLK_27000 */
	27,     /* E_PIXCLK_27027 */
	54,     /* E_PIXCLK_54000 */
	54,     /* E_PIXCLK_54054 */
	74,     /* E_PIXCLK_74175 */
	74,     /* E_PIXCLK_74250 */
	148,    /* E_PIXCLK_148350*/
	148     /* E_PIXCLK_148500*/
#ifdef FORMAT_PC
	, 31, /* E_PIXCLK_31500 */
	36, /* E_PIXCLK_36000 */
	40, /* E_PIXCLK_40000 */
	49, /* E_PIXCLK_49500 */
	50, /* E_PIXCLK_50000 */
	56, /* E_PIXCLK_56250 */
	65, /* E_PIXCLK_65000 */
	75, /* E_PIXCLK_75000 */
	79, /* E_PIXCLK_78750 */
	79, /* E_PIXCLK_79500 */
	85, /* E_PIXCLK_85500 */
	108, /* E_PIXCLK_108000 */
	122, /* E_PIXCLK_121750 */
	135, /* E_PIXCLK_135000 */
	162  /* E_PIXCLK_162000 */
#endif /* FORMAT_PC */
};

/**
 *  Lookup table for each CTS X factor's k and m register values
 * */
static CONST_DAT u8 k_cts_xto_mk[HDMITX_CTSX_NUM][2] = {
	/*   Register values    Actual values */
	/*   m  k               m, k */
	{3, 0},          /* 8, 1 */
	{3, 1},          /* 8, 2 */
	{3, 2},          /* 8, 3 */
	{3, 3},          /* 8, 4 */
	{0, 0}           /* 1, 1 */
};

/**
 * Table of shadow registers, as packed Shad/Page/Addr codes.
 * This allows shadow index values to be searched for using register page
 * and address values.
 * */
static CONST_DAT u16 k_shadow_reg[E_SNUM] = {
	/* Shadow Index                  Packed Shad/Page/Addr */
	E_REG_P00_MAIN_CNTRL0_W ,    /* E_SP00_MAIN_CNTRL0  */
	E_REG_P00_INT_FLAGS_0_RW,    /* E_SP00_INT_FLAGS_0  */
	E_REG_P00_INT_FLAGS_1_RW,    /* E_SP00_INT_FLAGS_1  */
#ifdef TMFL_TDA9981_SUPPORT
	E_REG_P00_INT_FLAGS_2_RW,    /* E_SP00_INT_FLAGS_2  */
#endif /* TMFL_TDA9981_SUPPORT */
	E_REG_P00_VIP_CNTRL_0_W ,    /* E_SP00_VIP_CNTRL_0  */
	E_REG_P00_VIP_CNTRL_1_W ,    /* E_SP00_VIP_CNTRL_1  */
	E_REG_P00_VIP_CNTRL_2_W ,    /* E_SP00_VIP_CNTRL_2  */
	E_REG_P00_VIP_CNTRL_3_W ,    /* E_SP00_VIP_CNTRL_3  */
	E_REG_P00_VIP_CNTRL_4_W ,    /* E_SP00_VIP_CNTRL_4  */
	E_REG_P00_VIP_CNTRL_5_W ,    /* E_SP00_VIP_CNTRL_5  */
	E_REG_P00_MAT_CONTRL_W  ,    /* E_SP00_MAT_CONTRL   */
	E_REG_P00_TBG_CNTRL_0_W ,    /* E_SP00_TBG_CNTRL_0  */
	E_REG_P00_TBG_CNTRL_1_W ,    /* E_SP00_TBG_CNTRL_1  */
	E_REG_P00_HVF_CNTRL_0_W ,    /* E_SP00_HVF_CNTRL_0  */
	E_REG_P00_HVF_CNTRL_1_W ,    /* E_SP00_HVF_CNTRL_1  */
	E_REG_P00_TIMER_H_W     ,    /* E_SP00_TIMER_H      */
	E_REG_P00_DEBUG_PROBE_W ,    /* E_SP00_DEBUG_PROBE  */
	E_REG_P00_AIP_CLKSEL_W       /* E_SP00_AIP_CLKSEL   */
#ifndef TMFL_TDA9981_SUPPORT
	, E_REG_P01_SC_VIDFORMAT_W   /* E_SP01_SC_VIDFORMAT */
	, E_REG_P01_SC_CNTRL_W       /* E_SP01_SC_CNTRL     */
	, E_REG_P01_TBG_CNTRL_0_W    /* E_SP01_TBG_CNTRL_0  */
#endif /* TMFL_TDA9981_SUPPORT */
};

/**
 * Lookup table of input port control registers and their swap and mirror masks
 * */
static CONST_DAT struct _bsl_reg_vip {
	u16 reg;
	u8  mask_swap;
	u8  mask_mirror;
} k_reg_vip[HDMITX_VIN_PORT_MAP_TABLE_LEN] = {
	{
		E_REG_P00_VIP_CNTRL_0_W,
		e_maskreg_p00_vip_cntrl_0_swap_a,
		e_maskreg_p00_vip_cntrl_0_mirr_a
	}, /* Port group 0 */
	{
		E_REG_P00_VIP_CNTRL_0_W,
		e_maskreg_p00_vip_cntrl_0_swap_b,
		e_maskreg_p00_vip_cntrl_0_mirr_b
	}, /* Port group 1 */
	{
		E_REG_P00_VIP_CNTRL_2_W,
		e_maskreg_p00_vip_cntrl_1_swap_c,
		e_maskreg_p00_vip_cntrl_1_mirr_c
	}, /* Port group 2 */
	{
		E_REG_P00_VIP_CNTRL_2_W,
		e_maskreg_p00_vip_cntrl_1_swap_d,
		e_maskreg_p00_vip_cntrl_1_mirr_d
	}, /* Port group 3 */
	{
		E_REG_P00_VIP_CNTRL_1_W,
		e_maskreg_p00_vip_cntrl_2_swap_e,
		e_maskreg_p00_vip_cntrl_2_mirr_e
	}, /* Port group 4 */
	{
		E_REG_P00_VIP_CNTRL_1_W,
		e_maskreg_p00_vip_cntrl_2_swap_f,
		e_maskreg_p00_vip_cntrl_2_mirr_f
	}  /* Port group 5 */
};

/**
 * Table of registers to switch scaler off
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_scaler_off[] = {
	/* Scaler is bypassed */
	{E_REG_P00_MAIN_CNTRL0_W,   e_maskreg_p00_main_cntrl0_scaler,   0},
	/* Bypass PLL scaling */
	{E_REG_P02_PLL_SCG2_RW,     e_maskreg_p02_pll_scg2_bypass_scg,  1},
	/* Disable scaler clock */
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_ena_sc_clk,   0},
	/* PLL loop open (standby) */
	{E_REG_P02_PLL_SCG1_RW,     e_maskreg_p02_pll_scg1_scg_fdn,     1},
	{0, 0, 0}
};

/**
 * Table of registers to switch scaler on
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_scaler_on[] = {
	/* Scaler is on */
	{E_REG_P00_MAIN_CNTRL0_W,   e_maskreg_p00_main_cntrl0_scaler,   1},
	/* Don't bypass PLL scaling */
	{E_REG_P02_PLL_SCG2_RW,     e_maskreg_p02_pll_scg2_bypass_scg,  0},
	/* Enable scaler clock */
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_ena_sc_clk,   1},
	/* PLL loop active */
	{E_REG_P02_PLL_SCG1_RW,     e_maskreg_p02_pll_scg1_scg_fdn,     0},
	{0, 0, 0}
};

/**
 * Table of registers to switch to low power (standby)
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_power_off[] = {
	{E_REG_P02_TEST2_RW,        e_maskreg_p02_test2_pwd1v8,         1},
	{E_REG_P02_PLL_SCG1_RW,     e_maskreg_p02_pll_scg1_scg_fdn,     1},
	{E_REG_P02_PLL_SERIAL_1_RW, e_maskreg_p02_pll_serial_1_srl_fdn, 1},
	{E_REG_P02_PLL_DE_RW,       e_maskreg_p02_pll_de_pllde_fdn,     1},
	{E_REG_P02_BUFFER_OUT_RW,   e_maskreg_p02_buffer_out_srl_force, 2},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_ena_sc_clk,   0},
#ifdef TMFL_TDA9981_SUPPORT
	{E_REG_P00_CCLK_ON_RW,		e_maskreg_p00_cclk_on_cclk_on,		0},
#endif /* TMFL_TDA9981_SUPPORT */
	{0, 0, 0}
};

/**
 * Table of registers to switch to normal power (resume)
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_power_on[] = {
	{E_REG_P02_TEST2_RW,        e_maskreg_p02_test2_pwd1v8,         0},
	{E_REG_P02_PLL_SERIAL_1_RW, e_maskreg_p02_pll_serial_1_srl_fdn, 0},
	{E_REG_P02_PLL_DE_RW,       e_maskreg_p02_pll_de_pllde_fdn,     0},
	{E_REG_P02_PLL_SCG1_RW,     e_maskreg_p02_pll_scg1_scg_fdn,     0},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_ena_sc_clk,   1},
	{E_REG_P02_BUFFER_OUT_RW,   e_maskreg_p02_buffer_out_srl_force, 0},
	{E_REG_P00_TBG_CNTRL_0_W,   e_maskreg_p00_tbg_cntrl_0_sync_once, 0},
#ifdef TMFL_TDA9981_SUPPORT
	{E_REG_P00_CCLK_ON_RW,		e_maskreg_p00_cclk_on_cclk_on,		1},
#endif /* TMFL_TDA9981_SUPPORT */
	{0, 0, 0}
};

/**
 * Table of registers to switch HDMI HDCP mode off for DVI
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vout_hdcp_off[] = {
	{E_REG_P00_TBG_CNTRL_1_W,   e_maskreg_p00_tbg_cntrl_1_dwin_dis, 1},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_hdmi,       0},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_eess,       0},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_ctl_sel,    0},
	{0, 0, 0}
};

/**
 * Table of registers to switch HDMI HDCP mode on for HDMI
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vout_hdcp_on[] = {
	{E_REG_P00_TBG_CNTRL_1_W,   e_maskreg_p00_tbg_cntrl_1_dwin_dis, 0},
	{E_REG_P11_ENC_CNTRL_RW,    e_maskreg_p11_enc_cntrl_ctl_code,   1},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_hdmi,       1},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_eess,       1},
	{E_REG_P12_HDCP_TX33_RW,    e_maskreg_p12_hdcp_tx_33_ctl_sel,    1},
	{0, 0, 0}
};

/**
 * Table of registers to reset and release the CTS generator
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_reset_cts_generator[] = {
	{E_REG_P11_AIP_CNTRL_0_RW,  e_maskreg_p11_aip_cntrl_0_rst_cts,  1},
	{E_REG_P11_AIP_CNTRL_0_RW,  e_maskreg_p11_aip_cntrl_0_rst_cts,  0},
	{0, 0, 0}
};

/**
 * Table of registers to set 5s watchdog timer
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_set5sec_watchdog[] = {
	/* Watchdog timer reference = CCLK divide by 32 / 100 = 10 kHz */
	/* PNF57 {E_REG_P00_NDIV_PF_W,       E_MASKREG_ALL,  100}, */
	/* CCLK divide by 32 = 1MHz */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_wd_clksel,    1},
	/* Set 5s watchdog (registers set to 100,000) */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_tim_h,        0x02},
	{E_REG_P00_TIMER_M_W,       E_MASKREG_ALL,                      0xb0},
	{E_REG_P00_TIMER_L_W,       E_MASKREG_ALL,                      0x00},
	{0, 0, 0}
};

/**
 * Table of registers to set 120ms watchdog timer based on HDMI clock
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_set120ms_watchdog[] = {
	/* Set HDMI clock as watchdog */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_wd_clksel,    0},
	/* Set 120ms watchdog (registers set to 120,000us) */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_tim_h,        0x01},
	{E_REG_P00_TIMER_M_W,       E_MASKREG_ALL,                      0xD4},
	{E_REG_P00_TIMER_L_W,       E_MASKREG_ALL,                      0xC0},
	{0, 0, 0}
};

/**
 * Table of registers to set 1s watchdog timer based on HDMI clock use for phase3
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_set1s_watchdog[] = {
	/* PNF57 Set HDMI clock as watchdog */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_wd_clksel,    0},
	/* Set 600ms watchdog for link check */
	{E_REG_P00_TIMER_H_W,       e_maskreg_p00_timer_h_tim_h,        0x00},
	{E_REG_P00_TIMER_M_W,       E_MASKREG_ALL,                      0x75},
	{E_REG_P00_TIMER_L_W,       E_MASKREG_ALL,                      0x30},
	{0, 0, 0}
};

/**
 * Table of registers to bypass colour processing (up/down sampler & matrix)
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_bypass_colour_proc[] = {
	/* Bypass upsampler for RGB colourbars */
	{E_REG_P00_HVF_CNTRL_0_W,   e_maskreg_p00_hvf_cntrl_0_intpol,   0},
	/* Bypass matrix for RGB colourbars */
	{E_REG_P00_MAT_CONTRL_W,    e_maskreg_p00_mat_contrl_mat_bp,    1},
	/* Bypass downsampler for RGB colourbars */
	{E_REG_P00_HVF_CNTRL_1_W,   e_maskreg_p00_hvf_cntrl_1_for,      0},
	{0, 0, 0}
};

/**
 * Table of registers to configure video input mode CCIR656
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vin_mode_ccir656[] = {
	{E_REG_P00_VIP_CNTRL_4_W,   e_maskreg_p00_vip_cntrl_4_ccir656,      1},
	{E_REG_P00_HVF_CNTRL_1_W,   e_maskreg_p00_hvf_cntrl_1_semi_planar,  1},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_sel_clk1,         1},
	{E_REG_P02_PLL_SERIAL_3_RW, e_maskreg_p02_pll_serial_3_srl_ccir,    1},
	{0, 0, 0}
};

/**
 * Table of registers to configure video input mode RGB444 or YUV444
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vin_mode444[] = {
	{E_REG_P00_VIP_CNTRL_4_W,   e_maskreg_p00_vip_cntrl_4_ccir656,      0},
	{E_REG_P00_HVF_CNTRL_1_W,   e_maskreg_p00_hvf_cntrl_1_semi_planar,  0},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_sel_clk1,         0},
	{E_REG_P02_PLL_SERIAL_3_RW, e_maskreg_p02_pll_serial_3_srl_ccir,    0},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_sel_vrf_clk,      0},
	{0, 0, 0}
};

/**
 * Table of registers to configure video input mode YUV422
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vin_mode_yuv422[] = {
	{E_REG_P00_VIP_CNTRL_4_W,   e_maskreg_p00_vip_cntrl_4_ccir656,      0},
	{E_REG_P00_HVF_CNTRL_1_W,   e_maskreg_p00_hvf_cntrl_1_semi_planar,  1},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_sel_clk1,         0},
	{E_REG_P02_PLL_SERIAL_3_RW, e_maskreg_p02_pll_serial_3_srl_ccir,    0},
	{E_REG_P02_SEL_CLK_RW,      e_maskreg_p02_sel_clk_sel_vrf_clk,      0},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure for all video input format (vinFmt)
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_common_pll_cfg[] = {
	{E_REG_P02_PLL_SERIAL_1_RW, E_MASKREG_ALL,  0x00},
	{E_REG_P02_PLL_SERIAL_2_RW, E_MASKREG_ALL,  0x01},
	{E_REG_P02_PLL_SERIAL_3_RW, E_MASKREG_ALL,  0x01},
	{E_REG_P02_SERIALIZER_RW,   E_MASKREG_ALL,  0x00},
	{E_REG_P02_BUFFER_OUT_RW,   E_MASKREG_ALL,  0x00},
	{E_REG_P02_CCIR_DIV_RW,     E_MASKREG_ALL,  0x00},
	{E_REG_P02_PLL_SCG1_RW,     E_MASKREG_ALL,  0x00},
	{E_REG_P02_AUDIO_DIV_RW,    E_MASKREG_ALL,  0x03},
	{E_REG_P02_TEST2_RW,        E_MASKREG_ALL,  0x00},
	{E_REG_P02_SEL_CLK_RW,      E_MASKREG_ALL,  0x09},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure for 480i and 576i vinFmt
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vfmt480i576i_pll_cfg[] = {
	{E_REG_P02_PLL_SCGN1_RW,    E_MASKREG_ALL,  0x14},
	{E_REG_P02_PLL_SCGN2_RW,    E_MASKREG_ALL,  0x00},
	{E_REG_P02_PLL_SCGR1_RW,    E_MASKREG_ALL,  0x0A},
	{E_REG_P02_PLL_SCGR2_RW,    E_MASKREG_ALL,  0x00},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure for other vinFmt than 480i and 576i
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_vfmt_other_pll_cfg[] = {
	{E_REG_P02_PLL_SCGN1_RW,    E_MASKREG_ALL,  0xFA},
	{E_REG_P02_PLL_SCGN2_RW,    E_MASKREG_ALL,  0x00},
	{E_REG_P02_PLL_SCGR1_RW,    E_MASKREG_ALL,  0x5B},
	{E_REG_P02_PLL_SCGR2_RW,    E_MASKREG_ALL,  0x00},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure for single mode pixel rate,
 * vinFmt 480i or 576i only
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_single_prate_vfmt480i576i_pll_cfg[] = {
	{E_REG_P02_PLL_SCG2_RW,     E_MASKREG_ALL,  0x11},
	{E_REG_P02_PLL_DE_RW,       E_MASKREG_ALL,  0xA1},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure for single repeated mode pixel rate,
 * vinFmt 480i or 576i only
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_srepeated_prate_vfmt480i576i_pll_cfg[] = {
	{E_REG_P02_PLL_SCG2_RW,     E_MASKREG_ALL,  0x01},
	{E_REG_P02_PLL_DE_RW,       E_MASKREG_ALL,  0xA1},
	{E_REG_P02_CCIR_DIV_RW,     E_MASKREG_ALL,  0x01},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure single mode pixel rate,
 * vinFmt other than 480i or 576i
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_single_prate_vfmt_other_pll_cfg[] = {
	{E_REG_P02_PLL_SCG2_RW,     E_MASKREG_ALL,  0x10},
	{E_REG_P02_PLL_DE_RW,       E_MASKREG_ALL,  0x91},
	{0, 0, 0}
};

/**
 * Table of PLL settings registers to configure double mode pixel rate,
 * vinFmt other than 480i or 576i
 * */
static CONST_DAT hdmi_tx_reg_mask_val_t k_double_prate_vfmt_other_pll_cfg[] = {
	{E_REG_P02_PLL_SCG2_RW,     E_MASKREG_ALL,  0x00},
	{E_REG_P02_PLL_DE_RW,       E_MASKREG_ALL,  0x10},
	{0, 0, 0}
};

/**
 *  Blue filter Lookup table for colour space conversion.
 *  Each array consists of 31 register values from MAT_CONTROL through
 *  to MAT_OO3_LSB
 * */
static CONST_DAT u8 matrix_coeff_blue_screen[][MATRIX_PRESET_SIZE] = {
	{
		0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0
	},  /* blue screen for RGB output color space */

	{
		0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x1, 0x0, 0x3, 0x0
	},  /* blue screen for YCbCr422 output color space */

	{
		0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x1, 0x0, 0x3, 0x0
	},  /* blue screen for YCbCr444 output color space */
};

/*============================================================================*/
/*                   STATIC VARIABLE DECLARATIONS                             */
/*============================================================================*/

/* Flag per device used to ignore 1st Encrypt interrupt after HdcpInit */
static RAM_DAT  bool    g_ignore_encrypt[HDMITX_UNITS_MAX];

/* Register values per device to restore colour processing after test pattern */
static RAM_DAT  u8   g_mat_contrl[HDMITX_UNITS_MAX];
static RAM_DAT  u8   g_hvf_cntrl0[HDMITX_UNITS_MAX];
static RAM_DAT  u8   g_hvf_cntrl1[HDMITX_UNITS_MAX];

/*============================================================================*/
/*                   STATIC FUNCTION DECLARATIONS                             */
/*============================================================================*/
static error_code_t    set_de_vs(hdmi_txobject_t *p_dis,
                                 bsl_vid_fmt_t vout_fmt);
static error_code_t    set_pixel_repeat(hdmi_txobject_t *p_dis,
                                        bsl_vid_fmt_t vout_fmt,
                                        u8 u_pixel_repeat);
static error_code_t    set_sampling(hdmi_txobject_t *p_dis);
static error_code_t    set_scaler_format(hdmi_txobject_t *p_dis,
        bsl_vid_fmt_t vin_fmt,
        bsl_vid_fmt_t vout_fmt,
        u8 pixel_repeat);

/*============================================================================*/
/*                   PUBLIC FUNCTION DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/* bslAudioInResetCts                                                 */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_audio_in_reset_cts
(
    unit_select_t	            tx_unit
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Reset and release the CTS generator */
	err = set_hw_register_field_table(p_dis, &k_reset_cts_generator[0]);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslAudioInSetConfig                                                */
/*============================================================================*/
error_code_t
bsl_audio_in_set_config
(
    unit_select_t           tx_unit,
    bsla_fmt_t        a_fmt,
    u8                    chan_i2s,
    u8                    chan_dsd,
    bsl_clk_pol_dsd_t   clk_pol_dsd,
    bsl_swap_dsd_t     swap_dsd,
    u8                    layout,
    u16                   u_latency_rd
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(a_fmt        >= HDMITX_AFMT_INVALID)
	RETIF_BADPARAM(chan_i2s     >= HDMITX_CHAN_INVALID)
	RETIF_BADPARAM(chan_dsd     >= HDMITX_CHAN_INVALID)
	RETIF_BADPARAM(clk_pol_dsd   >= HDMITX_CLKPOLDSD_INVALID)
	RETIF_BADPARAM(swap_dsd     >= HDMITX_SWAPDSD_INVALID)
	RETIF_BADPARAM(layout      >= HDMITX_LAYOUT_INVALID)
	RETIF_BADPARAM(u_latency_rd >= HDMITX_LATENCY_INVALID)

	/* Fold OBA onto DSD */
	if(a_fmt == HDMITX_AFMT_OBA) {
		a_fmt = HDMITX_AFMT_DSD;
	}

	/* Set the audio input processor format to aFmt. */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_AIP_CLKSEL_W,
	                            e_maskreg_p00_aip_clksel_sel_aip,
	                            (u8)a_fmt);
	RETIF_REG_FAIL(err)

	/* Select the audio format */
	if(a_fmt == HDMITX_AFMT_I2S) {
		if(chan_i2s != HDMITX_CHAN_NO_CHANGE) {
			err = set_hw_register(p_dis, E_REG_P11_CA_I2S_RW, chan_i2s);
			RETIF_REG_FAIL(err)
		}
	} else if(a_fmt == HDMITX_AFMT_DSD) {
		if(chan_dsd != HDMITX_CHAN_NO_CHANGE) {
			err = set_hw_register(p_dis, E_REG_P11_CA_DSD_RW, chan_dsd);
			RETIF_REG_FAIL(err)
		}
		if(clk_pol_dsd != HDMITX_CLKPOLDSD_NO_CHANGE) {
			err = set_hw_register_field(p_dis,
			                            E_REG_P00_AIP_CLKSEL_W,
			                            e_maskreg_p00_aip_clksel_sel_pol_clk,
			                            (u8)clk_pol_dsd);
			RETIF_REG_FAIL(err)
		}
		if(swap_dsd != HDMITX_SWAPDSD_NO_CHANGE) {
			err = set_hw_register_field(p_dis,
			                            E_REG_P11_AIP_CNTRL_0_RW,
			                            e_maskreg_p11_aip_cntrl_0_swap,
			                            (u8)swap_dsd);
			RETIF_REG_FAIL(err)
		}
	}

	/* Set layout and latency */
	if(layout != HDMITX_LAYOUT_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_AIP_CNTRL_0_RW,
		                            e_maskreg_p11_aip_cntrl_0_layout,
		                            layout);
		RETIF_REG_FAIL(err)
	}
	if(u_latency_rd != HDMITX_LATENCY_NO_CHANGE) {
		err = set_hw_register(p_dis, E_REG_P11_LATENCY_RD_RW, (u8)u_latency_rd);
		RETIF_REG_FAIL(err)
	}
	return 0;
}

/*============================================================================*/
/* bslAudioInSetCts                                                   */
/*============================================================================*/
error_code_t
bsl_audio_in_set_cts
(
    unit_select_t       tx_unit,
    bslcts_ref_t  cts_ref,
    bslafs_t     afs,
    bsl_vid_fmt_t  vout_fmt,
    bsl_vfreq_t   vout_freq,
    u32               u_cts,
    u16               u_cts_x,
    bslcts_k_t    cts_k,
    bslcts_m_t    cts_m
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t   err;        /* Error code */
	u8           reg_val;     /* Register value */
	u8           pix_clk;     /* Pixel clock index */
	u32          acr_n;       /* Audio clock recovery N */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(cts_ref   >= HDMITX_CTSREF_INVALID)
	RETIF_BADPARAM(afs      >= HDMITX_AFS_INVALID)

#ifdef FORMAT_PC
	RETIF_BADPARAM(vout_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM((vout_fmt > HDMITX_VFMT_TV_MAX) && (vout_fmt < HDMITX_VFMT_PC_MIN))
	RETIF_BADPARAM(vout_fmt       > HDMITX_VFMT_PC_MAX)
#else /* FORMAT_PC */
	/*FORMAT TV only*/
	RETIF_BADPARAM(vout_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vout_fmt       > HDMITX_VFMT_TV_MAX)
#endif /* FORMAT_PC */

	RETIF_BADPARAM(vout_freq >= HDMITX_VFREQ_INVALID)
	RETIF_BADPARAM(u_cts_x    >= HDMITX_CTSX_INVALID)
	RETIF_BADPARAM(cts_k     >= HDMITX_CTSK_INVALID)
	RETIF_BADPARAM(cts_m     >= HDMITX_CTSMTS_INVALID)

	if((vout_fmt  >=  HDMITX_VFMT_TV_MIN) && (vout_fmt  <=  HDMITX_VFMT_TV_MAX)) {
		if(vout_freq == hdmitx_vfreq_50hz) {
			RETIF(((vout_fmt < hdmitx_vfmt_17_720x576p_50hz) || (vout_fmt > hdmitx_vfmt_31_1920x1080p_50hz)),
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_24hz) {
			RETIF(vout_fmt != hdmitx_vfmt_32_1920x1080p_24hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_25hz) {
			RETIF(vout_fmt != hdmitx_vfmt_33_1920x1080p_25hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_30hz) {
			RETIF(vout_fmt != hdmitx_vfmt_34_1920x1080p_30hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else {
			RETIF(vout_fmt >= hdmitx_vfmt_17_720x576p_50hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		}

	}

#ifdef FORMAT_PC
	if((vout_fmt  >=  HDMITX_VFMT_PC_MIN) && (vout_fmt  <=  HDMITX_VFMT_PC_MAX)) {
		if(vout_freq == hdmitx_vfreq_60hz) {
			RETIF(vout_fmt > hdmitx_vfmt_pc_1600x1200p_60hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_70hz) {
			RETIF(vout_fmt != hdmitx_vfmt_pc_1024x768p_70hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_72hz) {
			RETIF(((vout_fmt < hdmitx_vfmt_pc_640x480p_72hz) ||
			       (vout_fmt > hdmitx_vfmt_pc_800x600p_72hz)),
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_75hz) {
			RETIF(((vout_fmt < hdmitx_vfmt_pc_640x480p_75hz) ||
			       (vout_fmt > hdmitx_vfmt_pc_1280x1024p_75hz)),
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else if(vout_freq == hdmitx_vfreq_85hz) {
			RETIF(((vout_fmt < hdmitx_vfmt_pc_640x350p_85hz) ||
			       (vout_fmt > hdmitx_vfmt_pc_1280x1024p_85hz)),
			      ERR_HDMI_INCONSISTENT_PARAMS)
		} else {
			RETIF(vout_fmt != hdmitx_vfmt_pc_1024x768i_87hz,
			      ERR_HDMI_INCONSISTENT_PARAMS)
		}
	}
#endif /* FORMAT_PC */

	/* Check for auto or manual CTS */
	if(u_cts == HDMITX_CTS_AUTO) {
		/* Auto */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_AIP_CNTRL_0_RW,
		                            e_maskreg_p11_aip_cntrl_0_acr_man,
		                            0);
		RETIF_REG_FAIL(err)
	} else {
		/* Manual */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_AIP_CNTRL_0_RW,
		                            e_maskreg_p11_aip_cntrl_0_acr_man,
		                            1);
		RETIF_REG_FAIL(err)
	}

	/* Derive M and K from X? */
	if((cts_m == HDMITX_CTSMTS_USE_CTSX) || (cts_k == HDMITX_CTSK_USE_CTSX)) {
		RETIF_BADPARAM(u_cts_x == HDMITX_CTSX_UNUSED)
		cts_m = (bslcts_m_t)k_cts_xto_mk[u_cts_x][0];
		cts_k = (bslcts_k_t)k_cts_xto_mk[u_cts_x][1];
	}

	/* Set the Post-divider measured timestamp factor */
	reg_val = (u8)cts_m;
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_CTS_N_RW,
	                            e_maskreg_p11_cts_n_m_sel,
	                            reg_val);
	RETIF_REG_FAIL(err)

	/* Set the Pre-divider scale */
	reg_val = (u8)cts_k;
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_CTS_N_RW,
	                            e_maskreg_p11_cts_n_k_sel,
	                            reg_val);
	RETIF_REG_FAIL(err)

	/* Use voutFmt and voutFreq to index into a lookup table to get
	 * the current pixel clock value. */

#ifdef FORMAT_PC
	if((vout_fmt  >=  HDMITX_VFMT_PC_MIN) && (vout_fmt  <=  HDMITX_VFMT_PC_MAX)) {
		pix_clk = k_vfmt_to_pix_clk_pc[(vout_fmt - HDMITX_VFMT_PC_MIN + 1)];
	} else {
#endif /* FORMAT_PC */
		pix_clk = k_vfmt_to_pix_clk_tv[vout_fmt][vout_freq];
#ifdef FORMAT_PC
	}
#endif /* FORMAT_PC */

	if(pix_clk != E_PIXCLK_INVALID) {
		/* Set the Audio Clock Recovery N multiplier based on the audio sample
		 * frequency afs and current pixel clock. */
		acr_n = k_pix_clk_to_acr_n[pix_clk][afs];

		/* Set ACR N multiplier [19 to 16] */
		reg_val = (u8)(acr_n >> 16);
		err = set_hw_register(p_dis, E_REG_P11_ACR_N_2_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set ACR N multiplier [15 to 8] */
		reg_val = (u8)(acr_n >> 8);
		err = set_hw_register(p_dis, E_REG_P11_ACR_N_1_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set ACR N multiplier [7 to 0] */
		reg_val = (u8)acr_n;
		err = set_hw_register(p_dis, E_REG_P11_ACR_N_0_RW, reg_val);
		RETIF_REG_FAIL(err)

		/* Set the CDC Audio Divider register based on the audio sample frequency
		 * afs and current pixel clock. */
		reg_val = k_pix_clk_to_adiv[pix_clk][afs];
		err = set_hw_register(p_dis, E_REG_P02_AUDIO_DIV_RW, reg_val);
		RETIF_REG_FAIL(err)

		/* If auto CTS, get CTS value based on the audio sample
		 * frequency afs and current pixel clock. */
		if(u_cts == HDMITX_CTS_AUTO) {
			u_cts = k_pix_clk_to_acr_cts[pix_clk][afs];
		}
	}

	/* Set manual or pixel clock CTS */
	if(u_cts != HDMITX_CTS_AUTO) {
		/* Set manual ACR CTS [19 to 16 */
		reg_val = (u8)(u_cts >> 16);
		err = set_hw_register(p_dis, E_REG_P11_ACR_CTS_2_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set manual ACR CTS [15 to 8] */
		reg_val = (u8)(u_cts >> 8);
		err = set_hw_register(p_dis, E_REG_P11_ACR_CTS_1_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set manual ACR CTS [7 to 0] */
		reg_val = (u8)u_cts;
		err = set_hw_register(p_dis, E_REG_P11_ACR_CTS_0_RW, reg_val);
		RETIF_REG_FAIL(err)
	}

	/* Set the CTS clock reference register according to ctsRef */
	reg_val = (u8)cts_ref;
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_AIP_CLKSEL_W,
	                            e_maskreg_p00_aip_clksel_sel_fs,
	                            reg_val);
	RETIF_REG_FAIL(err)

	/* Reset and release the CTS generator */
	err = set_hw_register_field_table(p_dis, &k_reset_cts_generator[0]);
	return err;
}

/*============================================================================*/
/* bslAudioOutSetChanStatus                                           */
/*============================================================================*/
error_code_t
bsl_audio_out_set_chan_status
(
    unit_select_t               tx_unit,
    bsl_csformat_info_t    format_info,
    bsl_cscopyright_t     copyright,
    u8                        category_code,
    bslafs_t             sample_freq,
    bsl_csclk_acc_t        clock_accuracy,
    bsl_csmax_word_length_t max_word_length,
    bsl_csword_length_t    word_length,
    bsl_csorig_afs_t       orig_sample_freq
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */
	u8             buf[4];   /* Buffer to hold channel status data */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(format_info     >= HDMITX_CSFI_INVALID)
	RETIF_BADPARAM(copyright      >= HDMITX_CSCOPYRIGHT_INVALID)
	RETIF_BADPARAM(sample_freq     >  HDMITX_AFS_NOT_INDICATED)
	RETIF_BADPARAM(clock_accuracy  >= HDMITX_CSCLK_INVALID)
	RETIF_BADPARAM(max_word_length  >= HDMITX_CSMAX_INVALID)
	RETIF_BADPARAM(word_length     >= HDMITX_CSWORD_INVALID)
	RETIF_BADPARAM(word_length     == HDMITX_CSWORD_RESVD)
	RETIF_BADPARAM(orig_sample_freq >= HDMITX_CSAFS_INVALID)

	/* Prepare Byte 0 */
	if(format_info == HDMITX_CSFI_NOTPCM_DEFAULT) {
		buf[0] = ((u8)copyright << 2) | 2;   /* Set NOT_PCM bit2 */
	} else {
		buf[0] = ((u8)format_info << 3) | ((u8)copyright << 2);
	}

	/* Prepare Byte 1 */
	buf[1] = category_code;

	/* Prepare Byte 3  - note Byte 2 not in sequence in TDA9983 register map */
	buf[2] = ((u8)clock_accuracy << 4) | k_afs_to_csbyte3[sample_freq];

	/* Prepare Byte 4 */
	buf[3] = ((u8)orig_sample_freq << 4) | ((u8)word_length << 1) |
	         (u8)max_word_length;

	/* Write 4 Channel Status bytes */
	err = set_hw_registers(p_dis, E_REG_P11_CH_STAT_B_0_RW, &buf[0], 4);
	return err;
}

/*============================================================================*/
/* bslAudioOutSetChanStatusMapping                                    */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_audio_out_set_chan_status_mapping
(
    unit_select_t  tx_unit,
    u8           source_left,
    u8           channel_left,
    u8           source_right,
    u8           channel_right
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */
	u8             buf[2];   /* Buffer to hold channel status data */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(source_left   > HDMITX_CS_SOURCES_MAX)
	RETIF_BADPARAM(channel_left  > HDMITX_CS_CHANNELS_MAX)
	RETIF_BADPARAM(source_right  > HDMITX_CS_SOURCES_MAX)
	RETIF_BADPARAM(channel_right > HDMITX_CS_CHANNELS_MAX)

	/* Prepare Left byte */
	buf[0] = ((u8)channel_left << 4) | (u8)source_left;

	/* Prepare Right byte */
	buf[1] = ((u8)channel_right << 4) | (u8)source_right;

	/* Write 2 Channel Status bytes */
	err = set_hw_registers(p_dis, e_reg_p11_ch_stat_b_2_ap0_l_rw, &buf[0], 2);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslAudioOutSetMute                                                 */
/*============================================================================*/
error_code_t
bsl_audio_out_set_mute
(
    unit_select_t      tx_unit,
    bsla_mute_t  a_mute
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if sink is not an HDMI device */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(a_mute >= HDMITX_AMUTE_INVALID)

	/* Reset the audio FIFO to mute audio */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_AIP_CNTRL_0_RW,
	                            e_maskreg_p11_aip_cntrl_0_rst_fifo,
	                            (u8)a_mute);
	return err;
}

/*============================================================================*/
/* bslDeinit                                                          */
/*============================================================================*/
error_code_t
bsl_deinit
(
    unit_select_t tx_unit
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Hold the device in reset to disable it */
	(void)set_hw_register_field(p_dis, E_REG_P00_MAIN_CNTRL0_W,
	                            e_maskreg_p00_main_cntrl0_sr, 1);

	/* Clear the Initialized flag to destroy the device instance */
	p_dis->b_initialized = false;

	set_state(p_dis, EV_DEINIT);
	return 0;
}

/*============================================================================*/
/* bslHotPlugGetStatus                                                */
/*============================================================================*/
error_code_t
bsl_hot_plug_get_status
(
    unit_select_t        tx_unit,
    bsl_hot_plug_t *p_hot_plug_status
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(p_hot_plug_status == (bsl_hot_plug_t *)0)

	/* Read the hot plug status flag register last read at interrupt */
	*p_hot_plug_status = p_dis->hot_plug_status;

	return 0;
}

/*============================================================================*/
/* bslRxSenseGetStatus                                                */
/*============================================================================*/
error_code_t
bsl_rx_sense_get_status
(
    unit_select_t        tx_unit,
    bsl_rx_sense_t *p_rx_sense_status
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(p_rx_sense_status == (bsl_rx_sense_t *)0)

#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
	/* Read the Rx sense status flag register last read at interrupt */
	*p_rx_sense_status = p_dis->rx_sense_status;
	return 0;
#else /* TMFL_TDA9981_SUPPORT && TMFL_RX_SENSE_ON */
	(void)*p_rx_sense_status; /* else not referenced */
	return ERR_HDMI_NOT_SUPPORTED;
#endif /* TMFL_TDA9981_SUPPORT && TMFL_RX_SENSE_ON */
}

/*============================================================================*/
/* bslHwGetRegisters                                                  */
/*============================================================================*/
error_code_t
bsl_hw_get_registers
(
    unit_select_t	tx_unit,
    int             reg_page,
    int             reg_addr,
    u8          *p_reg_buf,
    int             n_regs
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t *p_dis;
	error_code_t   err;            /* Error code */
	int             i;              /* Loop index */
	u8           new_reg_page;     /* The register's new page number */
	u8           reg_shad;        /* Index to the register's shadow copy */
	u16          reg_shad_page_addr;/* Packed shadowindex/page/address */
	bsl_sys_args_t sys_args;   /* Arguments passed to system function     */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM((reg_page < k_page_index_to_page[E_PAGE_00])
	               || ((reg_page > k_page_index_to_page[E_PAGE_02])
	                   && (reg_page < k_page_index_to_page[E_PAGE_11]))
	               || (reg_page > k_page_index_to_page[E_PAGE_12]))
	RETIF_BADPARAM((reg_addr < E_REG_MIN_ADR) || (reg_addr >= E_REG_CURPAGE_ADR_W))
	RETIF_BADPARAM(p_reg_buf == (p_uint8)0)
	RETIF_BADPARAM((n_regs < 1) || ((n_regs + reg_addr) > E_REG_CURPAGE_ADR_W))

	/* Set page register if required */
	new_reg_page = (u8)reg_page;
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* Read each register in the range. nRegs must start at 1 or more */
	for(; n_regs > 0; p_reg_buf++, reg_addr++, n_regs--) {
		/* Find shadow register index.
		 * This loop is not very efficient, but it is assumed that this API
		 * will not be used often. The alternative is to use a huge sparse
		 * array indexed by page and address and containing the shadow index.
		 *		 */
		reg_shad = E_SNONE;
		for(i = 0; i < E_SNUM; i++) {
			/* Check lookup table for match with page and address */
			reg_shad_page_addr = k_shadow_reg[i];
			if((SPA2PAGE(reg_shad_page_addr) == new_reg_page)
			   && (SPA2ADDR(reg_shad_page_addr) == reg_addr)) {
				/* Found page and address - look up the shadow index */
				reg_shad = SPA2SHAD(reg_shad_page_addr);
				break;
			}
		}
		/* Read the shadow register if available, as device registers that
		 * are shadowed cannot be read directly */
		if(reg_shad != E_SNONE) {
			*p_reg_buf = p_dis->shadow_reg[reg_shad];
		} else {
			/* Read the device register - all non-OK results are errors.
			 * Note that some non-shadowed registers are also write-only and
			 * cannot be read. */
			sys_args.slave_addr       = p_dis->u_hw_address;
			sys_args.first_register   = (u8)reg_addr;
			sys_args.len_data         = 1;
			sys_args.p_data           = p_reg_buf;
			err = p_dis->sys_func_read(&sys_args);
			RETIF(err != 0, ERR_HDMI_I2C_READ)
		}
	}

	return 0;
}

/*============================================================================*/
/* bslHwGetVersion                                                    */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_hw_get_version
(
    unit_select_t	tx_unit,
    p_uint8          p_hw_version
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t   err;            /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(p_hw_version == (p_uint8)0)

#ifdef TMFL_TDA9981_SUPPORT

	*p_hw_version = (u8) BSLHDMITX_TDA9981;

#else

	*p_hw_version = (u8) BSLHDMITX_TDA9983;

#endif /* TMFL_TDA9981_SUPPORT */

	return 0;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslHwGetCapabilities                                                    */
/*============================================================================*/
error_code_t
bsl_hw_get_capabilities
(
    unit_select_t          tx_unit,
    bsl_hw_feature_t  *p_device_capabilities
)

{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t   err;            /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(p_device_capabilities == (bsl_hw_feature_t *)0)

	*p_device_capabilities = bsl_hw_none;

	if((p_dis->u_device_features & e_maskreg_p00_version_not_h) == 0) {
		*p_device_capabilities = (bsl_hw_feature_t)(*p_device_capabilities + bsl_hw_hdcp);
	}

	if((p_dis->u_device_features & e_maskreg_p00_version_not_s) == 0) {
		*p_device_capabilities = (bsl_hw_feature_t)(*p_device_capabilities + bsl_hw_scaler);
	}

	return 0;
}

/*============================================================================*/
/* bslHwHandleInterrupt                                               */
/* RETIF_REG_FAIL NOT USED HERE AS ALL ERRORS SHOULD BE TRAPPED IN ALL BUILDS */
/*============================================================================*/
error_code_t
bsl_hw_handle_interrupt
(
    unit_select_t		tx_unit
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t *p_dis;
	error_code_t     err;              /* Error code */
	u8             reg_val;           /* Register value */
#ifdef TMFL_TDA9981_SUPPORT
	u16            f_interrupt_status; /* Interrupt flags */
	u16            f_interrupt_mask;   /* Mask to test each interrupt bit */
#ifdef TMFL_RX_SENSE_ON
	bsl_rx_sense_t  new_rxs_fil;       /* Latest copy of rx_sense */
#endif /* TMFL_RX_SENSE_ON */
#else /* TMFL_TDA9981_SUPPORT */
	u8             f_interrupt_status; /* Interrupt flags */
	u8             f_interrupt_mask;   /* Mask to test each interrupt bit */
#endif /* TMFL_TDA9981_SUPPORT */
	int               i;                /* Loop counter */
	bsl_hot_plug_t new_hpd_in;      /* Latest copy of hpd input */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	f_interrupt_status = 0;

	/* Read the main interrupt flags register to determine the source(s)
	 * of the interrupt. (The device resets these register flags after they
	 * have been read.)
	 *	 */
#ifdef TMFL_TDA9981_SUPPORT
	err = get_hw_register(p_dis, E_REG_P00_INT_FLAGS_0_RW, &reg_val);
	RETIF(err != 0, err)
	f_interrupt_status = reg_val;

#else /* TMFL_TDA9981_SUPPORT */
	err = get_hw_register(p_dis, E_REG_P00_INT_FLAGS_0_RW, &f_interrupt_status);
	RETIF(err != 0, err)
#endif /* TMFL_TDA9981_SUPPORT */

	/* Read Hot Plug input status to unit object
	 *	   DJW do this after reading INT flags so we get the actual
	 *	   level that caused the interrupt */
	err = get_hw_register(p_dis, E_REG_P00_INT_FLAGS_1_RW, &reg_val);
	RETIF(err != 0, err)
	new_hpd_in = (reg_val & e_maskreg_p00_int_flags_1_hpd_in) ?
	             HDMITX_HOTPLUG_ACTIVE : HDMITX_HOTPLUG_INACTIVE;

#ifdef TMFL_TDA9981_SUPPORT
	/* Read the software interrupt flag */
	if((reg_val & e_maskreg_p00_int_flags_1_sw_int) != 0) {
		f_interrupt_status = f_interrupt_status | (1 << HDMITX_CALLBACK_INT_SW_INT);
	}

#ifdef TMFL_RX_SENSE_ON
	/* Read INT_FLAGS_2 interrupt flag register.
	 *(the device resets these register flags after they
	 * have been read.) */
	err = get_hw_register(p_dis, E_REG_P00_INT_FLAGS_2_RW, &reg_val);
	RETIF(err != 0, err)

	/* Has the rx_sense interrupt occurs */
	if((reg_val & e_maskreg_p00_int_flags_2_rx_sense) != 0) {
		f_interrupt_status = f_interrupt_status | (1 << HDMITX_CALLBACK_INT_RX_SENSE);
	}
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */

	/* Has the hpd input changed? (Ignore the HPD interrupt status flag in
	 * fInterruptStatus as this may have latched a connect/disconnect glitch) */
	if(new_hpd_in != p_dis->hot_plug_status) {
		/* Yes: save new HPD level */
		p_dis->hot_plug_status = new_hpd_in;

		/* Reset EDID status */
		p_dis->edid_status = HDMITX_EDID_NOT_READ;

#ifdef TMFL_TDA9981_SUPPORT
		/* Set HPD flag to 1 although it certainly already done just a security */
		f_interrupt_status |= e_maskreg_p00_int_flags_0_hpd;
		/* Reset all simultaneous HDCP interrupts on hot plug,
		 * preserving only the high-priority hpd interrupt rx_sense and sw interrupt for debug*/
		f_interrupt_status &= e_maskreg_p00_int_flags_0_hpd |
#ifdef TMFL_RX_SENSE_ON
		                      (1 << HDMITX_CALLBACK_INT_RX_SENSE) |
#endif /* TMFL_RX_SENSE_ON */
		                      (1 << HDMITX_CALLBACK_INT_SW_INT);
#else
		/* Reset all simultaneous HDCP interrupts on hot plug/unplug,
		 * preserving only the high-priority hpd interrupt */
		f_interrupt_status = e_maskreg_p00_int_flags_0_hpd;
#endif /* TMFL_TDA9981_SUPPORT */

		if(p_dis->hot_plug_status == HDMITX_HOTPLUG_ACTIVE) {
			set_state(p_dis, EV_PLUGGEDIN);
		} else {
			set_state(p_dis, EV_UNPLUGGED);
		}
	} else {
		/* Clear HPD status if level has not changed */
		f_interrupt_status &= ~e_maskreg_p00_int_flags_0_hpd;
	}

#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON

	/* Read INT_FLAGS_3 interrupt flag register. */
	err = get_hw_register(p_dis, E_REG_P00_INT_FLAGS_3_R, &reg_val);
	RETIF(err != 0, err)

	/*	Read RXS_FIL status to know the actual
	 *		level that caused the interrupt */
	new_rxs_fil = (reg_val & e_maskreg_p00_int_flags_3_rxs_fil) ?
	              HDMITX_RX_SENSE_ACTIVE : HDMITX_RX_SENSE_INACTIVE;

	/* Has the Rxs Fil changed? (Ignore the RxSense interrupt status flag in
	 * fInterruptStatus as this may have latched a on/off glitch) */
	if(new_rxs_fil != p_dis->rx_sense_status) {
		/* Yes: save new rxSense level */
		p_dis->rx_sense_status = new_rxs_fil;

		/* Set RX_Sense flag to 1 although it certainly already done just a security */
		f_interrupt_status |= (1 << HDMITX_CALLBACK_INT_RX_SENSE);

		f_interrupt_status &= e_maskreg_p00_int_flags_0_hpd |
		                      (1 << HDMITX_CALLBACK_INT_RX_SENSE) |
		                      (1 << HDMITX_CALLBACK_INT_SW_INT);

		if(p_dis->rx_sense_status == HDMITX_RX_SENSE_ACTIVE) {
			set_state(p_dis, EV_SINKON);
		} else {
			set_state(p_dis, EV_SINKOFF);
		}
	} else {
		/* Clear RX_sense IT if level has not changed */
		f_interrupt_status &= ~(1 << HDMITX_CALLBACK_INT_RX_SENSE);
	}
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */

	/* Ignore other simultaneous HDCP interrupts if T0 interrupt,
	 * preserving any hpd interrupt */
	if(f_interrupt_status & e_maskreg_p00_int_flags_0_t0) {
		f_interrupt_status &=
		    (
		        e_maskreg_p00_int_flags_0_hpd |
		        e_maskreg_p00_int_flags_0_t0
#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON
		        | (1 << HDMITX_CALLBACK_INT_RX_SENSE)
#endif /* TMFL_RX_SENSE_ON */
		        | (1 << HDMITX_CALLBACK_INT_SW_INT)
#endif /* TMFL_TDA9981_SUPPORT */
		    );
	}

	/* For each interrupt flag that is set, check the corresponding registered
	 * callback function pointer in the Device Instance Structure
	 * funcIntCallbacks array.
	 *	 */
	f_interrupt_mask = 1;
	for(i = 0; i < HDMITX_CALLBACK_INT_NUM; i++) {
		if(f_interrupt_status & f_interrupt_mask) {
			/* IF a registered callback pointer is non-null THEN call it. */
			if(p_dis->func_int_callbacks[i] != (pbsl_callback_t)0) {
				p_dis->func_int_callbacks[i](tx_unit);
			}
		}
		f_interrupt_mask <<= 1;
	}

	return 0;
}

/*============================================================================*/
/* bslFlagSwInt                                                       */
/* Use only for debug to flag the software debug interrupt                    */
/*============================================================================*/
error_code_t
bsl_flag_sw_int
(
    unit_select_t	            tx_unit
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	err = set_hw_register(p_dis, E_REG_P00_SW_INT_W,
	                      e_maskreg_p00_sw_int_sw_int);

	return err;
#else /* TMFL_TDA9981_SUPPORT */
	return ERR_HDMI_NOT_SUPPORTED;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslHwSetRegisters                                                  */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_hw_set_registers
(
    unit_select_t	tx_unit,
    int             reg_page,
    int             reg_addr,
    u8          *p_reg_buf,
    int             n_regs
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t *p_dis;
	error_code_t   err;            /* Error code */
	int             i;              /* Loop index */
	u8           new_reg_page;     /* The register's new page number */
	u8           reg_shad;        /* Index to the register's shadow copy */
	u16          reg_shad_page_addr;/* Packed shadowindex/page/address */
	bsl_sys_args_t sys_args;   /* Arguments passed to system function */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM((reg_page < k_page_index_to_page[E_PAGE_00])
	               || ((reg_page > k_page_index_to_page[E_PAGE_02])
	                   && (reg_page < k_page_index_to_page[E_PAGE_11]))
	               || (reg_page > k_page_index_to_page[E_PAGE_12]))
	RETIF_BADPARAM((reg_addr < E_REG_MIN_ADR) || (reg_addr >= E_REG_CURPAGE_ADR_W))
	RETIF_BADPARAM(p_reg_buf == (p_uint8)0)
	RETIF_BADPARAM((n_regs < 0) || ((n_regs + reg_addr) > E_REG_CURPAGE_ADR_W))

	/* Set page register if required */
	new_reg_page = (u8)reg_page;
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* Write each register in the range. nRegs = 0 is ok, to allow only
	 * the page register to be written if required (above)
	 *	 */
	for(; n_regs > 0; p_reg_buf++, reg_addr++, n_regs--) {
		/* Find shadow register index.
		 * This loop is not very efficient, but it is assumed that this API
		 * will not be used often. The alternative is to use a huge sparse
		 * array indexed by page and address and containing the shadow index.
		 *		 */
		for(i = 0; i < E_SNUM; i++) {
			/* Check lookup table for match with page and address */
			reg_shad_page_addr = k_shadow_reg[i];
			if((SPA2PAGE(reg_shad_page_addr) == new_reg_page)
			   && (SPA2ADDR(reg_shad_page_addr) == reg_addr)) {
				/* Found index - write the shadow register */
				reg_shad = SPA2SHAD(reg_shad_page_addr);
				p_dis->shadow_reg[reg_shad] = *p_reg_buf;
				break;
			}
		}
		/* Write the device register - all non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = (u8)reg_addr;
		sys_args.len_data         = 1;
		sys_args.p_data           = p_reg_buf;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
	}

	return 0;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslHwStartup                                                       */
/*============================================================================*/
void
bsl_hw_startup
(
    void
)
{
	/* Reset device instance data for when compiler doesn't do it */
	lmemset(&g_hdmi_tx_instance, 0, sizeof(g_hdmi_tx_instance));
}

/*============================================================================*/
/* bslInit                                                            */
/* RETIF_REG_FAIL NOT USED HERE AS ALL ERRORS SHOULD BE TRAPPED IN ALL BUILDS */
/*============================================================================*/
error_code_t
bsl_init
(
    unit_select_t	           tx_unit,
    u8                      u_hw_address,
    pbsl_sys_func_t      sys_func_write,
    pbsl_sys_func_t      sys_func_read,
    pbsl_sys_func_edid_t  sys_func_edid_read,
    pbsl_sys_func_timer_t sys_func_timer,
    bsl_callback_list_t *func_int_callbacks,
    bool                       b_edid_alt_addr,
    bsl_vid_fmt_t        vin_fmt,
    bsl_pix_rate_t       pix_rate
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t     err;      /* Error code */
	int               i;        /* Loop index */
	bool              b_found;   /* T=found, F=not found */
	u8             reg_val;   /* Value read from register */

	/* Check unit parameter and point to its object */
	RETIF(tx_unit < unit0, ERR_HDMI_BAD_UNIT_NUMBER)
	RETIF(tx_unit >= HDMITX_UNITS_MAX, ERR_HDMI_BAD_UNIT_NUMBER)
	p_dis = &g_hdmi_tx_instance[tx_unit];

	/* IF the bInitialized flag is set THEN return (only Init does this) */
	RETIF(p_dis->b_initialized, ERR_HDMI_INIT_FAILED)

	/* Check remaining parameters */
	RETIF_BADPARAM(u_hw_address < HDMITX_SLAVE_ADDRESS_MIN)
	RETIF_BADPARAM(u_hw_address > HDMITX_SLAVE_ADDRESS_MAX)
	RETIF_BADPARAM(sys_func_write == (pbsl_sys_func_t)0)
	RETIF_BADPARAM(sys_func_read == (pbsl_sys_func_t)0)
	RETIF_BADPARAM(sys_func_edid_read == (pbsl_sys_func_edid_t)0)
	RETIF_BADPARAM(sys_func_timer == (pbsl_sys_func_timer_t)0)
	RETIF_BADPARAM((b_edid_alt_addr != true) && (b_edid_alt_addr != false))
#ifdef FORMAT_PC
	RETIF_BADPARAM(vin_fmt  < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM((vin_fmt > HDMITX_VFMT_TV_MAX) && (vin_fmt < HDMITX_VFMT_PC_MIN))
	RETIF_BADPARAM(vin_fmt  > HDMITX_VFMT_PC_MAX)
#else /* FORMAT_PC */
	/* FORMAT TV only */
	RETIF_BADPARAM(vin_fmt  < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vin_fmt  > HDMITX_VFMT_TV_MAX)
#endif /* FORMAT_PC */
	RETIF_BADPARAM(pix_rate >= HDMITX_PIXRATE_INVALID)

	/* Set all Device Instance Structure members to default values */
	lmemcpy(p_dis, &k_hdmi_tx_instance_default, sizeof(*p_dis));

	/* Copy txUnit, uHwAddress, sysFuncWrite and sysFuncRead values to
	 * the defaulted Device Instance Structure BEFORE FIRST DEVICE ACCESS.
	 *	 */
	p_dis->tx_unit          = tx_unit;
#ifdef UNIT_TEST
	/* Unit test build can't support 127 device sets of dummy registers, so use
	 * smaller range instead, indexed by unit number not I2C address */
	p_dis->u_hw_address      = (u8)tx_unit;
#else
	/* Store actual I2C address */
	p_dis->u_hw_address      = u_hw_address;
#endif
	p_dis->sys_func_write    = sys_func_write;
	p_dis->sys_func_read     = sys_func_read;
	p_dis->sys_func_edid_read = sys_func_edid_read;
	p_dis->sys_func_timer    = sys_func_timer;

	/* Reset Hdcp interrupt flag */
	g_ignore_encrypt[tx_unit] = false;

	/* Ensure that DDC is not connected to I2C at start */
	err = set_hw_register(p_dis, E_REG_P00_GHOST_ADDR_W, 0x01);
	RETIF(err != 0, err)

	/* Read the device version register to uDeviceVersion in the
	 * Device Instance Structure
	 *	 */
	err = get_hw_register(p_dis, E_REG_P00_VERSION_R, &p_dis->u_device_version);
	RETIF(err != 0, err)
	/* Copy N4 features bits to DIS */
	p_dis->u_device_features = p_dis->u_device_version &
	                           (e_maskreg_p00_version_not_h | e_maskreg_p00_version_not_s);
	/* Mask off N4 features bits in version */
	p_dis->u_device_version &= (u8)~p_dis->u_device_features;

#ifdef TMFL_TDA9981_SUPPORT
	/* If version reads zero, test not_s to '1' for TDA9981 */
	if(p_dis->u_device_version == E_DEV_VERSION_N4) {
		if((p_dis->u_device_features & e_maskreg_p00_version_not_s) !=
		   e_maskreg_p00_version_not_s) {
			p_dis->u_device_version = E_DEV_VERSION_LIST_END;
		}
	}
#endif /* TMFL_TDA9981_SUPPORT */

	/* Quit if version reads zero */
	RETIF(p_dis->u_device_version == E_DEV_VERSION_LIST_END,
	      ERR_HDMI_COMPATIBILITY)

	/* Search for the device version in the Supported Version
	 * List in the Device Instance Structure.
	 *	 */
	for(i = 0, b_found = false; i < E_DEV_VERSION_LIST_NUM; i++) {
		if(p_dis->u_device_version == p_dis->u_supported_versions[i]) {
			b_found = true;
		}
	}

	/* IF the device version is not found in the Supported Version List THEN
	 * this driver component is not compatible with the device.
	 *	 */
	RETIF(!b_found, ERR_HDMI_COMPATIBILITY);

	/* If version is N4 then detect N5 device (both have same version) */
	if(p_dis->u_device_version == E_DEV_VERSION_N4) {
		/* Read N5-specific registers to detect N5 */
		err = get_hw_register(p_dis, E_REG_P00_DWIN_RE_DE_W, &reg_val);
		RETIF(err != 0, err)
		if(reg_val == 0x11) {
			/* Default value for N5 */
			err = get_hw_register(p_dis, E_REG_P00_DWIN_FE_DE_W, &reg_val);
			RETIF(err != 0, err)
			if(reg_val == 0x7A) {
				/* Both N5 registers have correct default value for N5,
				 * so set the N5 'feature' flag */
				p_dis->u_device_features |= E_MASKREG_P00_FEATURE_N5;
			}
		}
	}

	/* IF the funcIntCallbacks array pointer is defined
	 * THEN for each funcIntCallbacks pointer that is not null:
	 * - Copy the pointer to the Device Instance Structure
	 *   funcIntCallbacks array.
	 *	 */
	for(i = 0; i < HDMITX_CALLBACK_INT_NUM; i++) {
		if((func_int_callbacks != (bsl_callback_list_t *)0)
		   && (func_int_callbacks->func_callback[i] != (pbsl_callback_t)0)) {
			p_dis->func_int_callbacks[i] = func_int_callbacks->func_callback[i];
		} else {
			p_dis->func_int_callbacks[i] = (pbsl_callback_t)0;
		}
	}

	/* Set the EDID alternate address flag if needed*/
	p_dis->b_edid_alternate_addr = b_edid_alt_addr;

	/* Set the bInitialized flag to enable other APIs */
	p_dis->b_initialized = true;

	/* Set the PLL before resetting the device */
	/* PLL registers common configuration */
	err = set_hw_register_field_table(p_dis, &k_common_pll_cfg[0]);
	RETIF_REG_FAIL(err)

	switch(vin_fmt) {
		/* 480i or 576i video input format */
	case hdmitx_vfmt_06_720x480i_60hz:
	case hdmitx_vfmt_07_720x480i_60hz:
	case hdmitx_vfmt_21_720x576i_50hz:
	case hdmitx_vfmt_22_720x576i_50hz:
		err = set_hw_register_field_table(p_dis, &k_vfmt480i576i_pll_cfg[0]);
		RETIF_REG_FAIL(err)

		switch(pix_rate) {
		case HDMITX_PIXRATE_SINGLE:
			/* Single edge mode, vinFmt 480i or 576i */
			err = set_hw_register_field_table(p_dis, &k_single_prate_vfmt480i576i_pll_cfg[0]);
			RETIF_REG_FAIL(err)
			break;
		case HDMITX_PIXRATE_SINGLE_REPEATED:
			/* Single repeated edge mode, vinFmt 480i or 576i */
			err = set_hw_register_field_table(p_dis, &k_srepeated_prate_vfmt480i576i_pll_cfg[0]);
			RETIF_REG_FAIL(err)
			break;
		default:
			/* Double edge mode doesn't exist for vinFmt 480i or 576i */
			return(ERR_HDMI_INCONSISTENT_PARAMS);
			break;
		}

		break;

		/* Others video input format */
	default:
		err = set_hw_register_field_table(p_dis, &k_vfmt_other_pll_cfg[0]);
		RETIF_REG_FAIL(err)

		switch(pix_rate) {
		case HDMITX_PIXRATE_SINGLE:
			/* Single edge mode, vinFmt other than 480i or 576i */
			err = set_hw_register_field_table(p_dis, &k_single_prate_vfmt_other_pll_cfg[0]);
			RETIF_REG_FAIL(err)
			break;
		case HDMITX_PIXRATE_DOUBLE:
			/* Double edge mode, vinFmt other than 480i or 576i */
			err = set_hw_register_field_table(p_dis, &k_double_prate_vfmt_other_pll_cfg[0]);
			RETIF_REG_FAIL(err)
			break;
		default:
			/* Single repeated edge mode doesn't exist for other vinFmt */
			return(ERR_HDMI_INCONSISTENT_PARAMS);
			break;
		}

		break;
	}

	/* Reset the device */
	err = bsl_reset(tx_unit);
	RETIF(err != 0, err)

	/* The DIS hotplug status is HDMITX_HOTPLUG_INVALID, so call the main
	 * interrupt handler to read the current Hot Plug status and run any
	 * registered HPD callback before interrupts are enabled below */
	err = bsl_hw_handle_interrupt(tx_unit);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	/* enable sw _interrupt for debug */
	err = set_hw_register(p_dis, E_REG_P00_INT_FLAGS_1_RW,
	                      e_maskreg_p00_int_flags_1_sw_int);

#ifdef TMFL_RX_SENSE_ON
	/* enable sw _interrupt for debug */
	err = set_hw_register(p_dis, E_REG_P00_INT_FLAGS_2_RW,
	                      e_maskreg_p00_int_flags_2_rx_sense);
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */

	/* Enable only the Hot Plug detect interrupt */
	err = set_hw_register(p_dis, E_REG_P00_INT_FLAGS_0_RW,
	                      e_maskreg_p00_int_flags_0_hpd);
	return err;
}

/*============================================================================*/
/* bslPowerGetState                                                   */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_power_get_state
(
    unit_select_t		tx_unit,
    p_power_state_t		pe_power_state
)
{
	hdmi_txobject_t *p_dis;   /* Pointer to Device Instance Structure */
	error_code_t     err;    /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(pe_power_state == (p_power_state_t)0)

	/* Read the device power status and set the pePowerState
	 * return parameter
	 *	 */
	*pe_power_state = p_dis->e_power_state;

	return 0;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslPowerSetState                                                   */
/*============================================================================*/
error_code_t
bsl_power_set_state
(
    unit_select_t		tx_unit,
    power_state_t      e_power_state
)
{
	hdmi_txobject_t *p_dis;   /* Pointer to Device Instance Structure */
	error_code_t     err;    /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameters */
	RETIF_BADPARAM(e_power_state > power_off)

	/* Treat Off and Suspend the same as Standby */
	if((e_power_state == power_off) || (e_power_state == power_suspend)) {
		e_power_state = power_standby;
	}

	/* Set the device power control register to the requested ePowerState
	 * state
	 *	 */
	if(e_power_state == power_on) {
		/** Set power On state control registers */
		err = set_hw_register_field_table(p_dis, &k_power_on[0]);
		RETIF_REG_FAIL(err)

		if(p_dis->e_power_state == power_standby) {
			if(p_dis->hot_plug_status == HDMITX_HOTPLUG_ACTIVE) {
				set_state(p_dis, EV_RESUME_PLUGGEDIN);
			} else {
				set_state(p_dis, EV_RESUME_UNPLUGGED);
			}
		}
	} else {
		/** Set power standby state control registers */
		err = set_hw_register_field_table(p_dis, &k_power_off[0]);
		RETIF_REG_FAIL(err)

		set_state(p_dis, EV_STANDBY);
	}

	/* Save the ePowerState value in the Device Instance Structure */
	p_dis->e_power_state = e_power_state;

	return 0;
}

/*============================================================================*/
/* bslReset                                                           */
/*============================================================================*/
error_code_t
bsl_reset
(
    unit_select_t		tx_unit
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Write to the transmitter to do a soft reset. Don't abort after any
	 * error here, to ensure full reset.
	 *	 */
	(void)set_hw_register_field(p_dis, E_REG_P00_MAIN_CNTRL0_W,
	                            e_maskreg_p00_main_cntrl0_sr, 1);
	p_dis->sys_func_timer(50); /* ms */
	(void)set_hw_register_field(p_dis, E_REG_P00_MAIN_CNTRL0_W,
	                            e_maskreg_p00_main_cntrl0_sr, 0);

	/* Clear any colourbars */
	(void)set_hw_register_field(p_dis, E_REG_P00_HVF_CNTRL_0_W,
	                            e_maskreg_p00_hvf_cntrl_0_sm, 0);

	/* Transmitter is now inactive so treat as if sink has been unplugged */
	set_state(p_dis, EV_UNPLUGGED);
	return 0;
}

/*============================================================================*/
/* bslScalerGet                                                       */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_scaler_get
(
    unit_select_t		      tx_unit,
    bsl_scaler_diag_t  *p_scaler_diag
)
{
	hdmi_txobject_t *p_dis;     /* Pointer to Device Instance Structure */
	error_code_t   err;        /* Error code */
	u8           reg_val;     /* Register value */

	/* Check unit parameter and point to its object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)p_scaler_diag; /* else not referenced */
	(void)reg_val; /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check remaining parameters */
	RETIF_BADPARAM(p_scaler_diag == (bsl_scaler_diag_t *)0)

	/* Read from dummy register to update MAX_BUFFILL_P registers */
	err = get_hw_register(p_dis, E_REG_P01_SC_SAMPLE_BUFFILL_R, &reg_val);
	RETIF_REG_FAIL(err)

	/* Read the 16-bit MAX_BUFFILL_P register pair and copy to pScalerDiag */
	err = get_hw_register(p_dis, E_REG_P01_SC_MAX_BUFFILL_P_0_R, &reg_val);
	RETIF_REG_FAIL(err)
	p_scaler_diag->max_buffill_p = reg_val;
	err = get_hw_register(p_dis, E_REG_P01_SC_MAX_BUFFILL_P_1_R, &reg_val);
	RETIF_REG_FAIL(err)
	p_scaler_diag->max_buffill_p |= ((u16)reg_val << 8);

	/* Read the 16-bit MAX_BUFFILL_D register pair and copy to pScalerDiag */
	err = get_hw_register(p_dis, E_REG_P01_SC_MAX_BUFFILL_D_0_R, &reg_val);
	RETIF_REG_FAIL(err)
	p_scaler_diag->max_buffill_d = reg_val;
	err = get_hw_register(p_dis, E_REG_P01_SC_MAX_BUFFILL_D_1_R, &reg_val);
	RETIF_REG_FAIL(err)
	p_scaler_diag->max_buffill_d |= ((u16)reg_val << 8);

	/* Read from dummy register to update MAX_FIFOFILL registers */
	err = get_hw_register(p_dis, E_REG_P01_SC_SAMPLE_FIFOFILL_R, &reg_val);
	RETIF_REG_FAIL(err)

	/* Read the 8-bit FIFOFILL registers directly to pScalerDiag */
	err = get_hw_registers(p_dis, E_REG_P01_SC_MAX_FIFOFILL_PI_R,
	                       &p_scaler_diag->max_fifofill_pi, 7);
	return err;
#endif /* TMFL_TDA9981_SUPPORT */
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslScalerGetMode                                                   */
/*============================================================================*/
error_code_t
bsl_scaler_get_mode
(
    unit_select_t		      tx_unit,
    bsl_sca_mode_t      *p_scaler_mode
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t   err;          /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)p_scaler_mode; /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(p_scaler_mode == NULL)

	/* Set output parameter */
	*p_scaler_mode = p_dis->sca_mode;
	return 0;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslScalerInDisable                                                 */
/*============================================================================*/
error_code_t
bsl_scaler_in_disable
(
    unit_select_t  tx_unit,
    bool            b_disable
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)b_disable; /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(b_disable > true)

	/* Set or clear frame_dis in the scaler Timebase Control 0 register
	 * according to bDisable
	 *	 */
	err = set_hw_register_field(p_dis,
	                            E_REG_P01_TBG_CNTRL_0_W,
	                            e_maskreg_p01_tbg_cntrl_0_frame_dis,
	                            (u8)b_disable);
	return err;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslScalerSetCoeffs                                                 */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_scaler_set_coeffs
(
    unit_select_t        tx_unit,
    bsl_sca_lut_t   lut_sel,
    u8                *p_vs_lut
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)lut_sel; /* else not referenced */
	(void)p_vs_lut; /* else not referenced */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(lut_sel >= HDMITX_SCALUT_INVALID)

	if(lut_sel == HDMITX_SCALUT_USE_VSLUT) {
		/* Table pointer must be valid here */
		RETIF(p_vs_lut == NULL, ERR_HDMI_INCONSISTENT_PARAMS)

		/* Set LUT coefficient table */
		err = set_hw_registers(p_dis,
		                       E_REG_P01_SC_VS_LUT_0_W,
		                       p_vs_lut,
		                       HDMITX_VSLUT_COEFF_NUM);
		RETIF_REG_FAIL(err)
	} else {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_SC_VIDFORMAT_W,
		                            e_maskreg_p01_sc_vidformat_lut_sel,
		                            (u8)lut_sel);
		RETIF_REG_FAIL(err)
	}
	return 0;
#endif /* TMFL_TDA9981_SUPPORT */
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslScalerSetFieldOrder                                             */
/*============================================================================*/
error_code_t
bsl_scaler_set_field_order
(
    unit_select_t      tx_unit,
    bsl_int_ext_t top_ext,
    bsl_int_ext_t de_ext,
    bsl_top_sel_t top_sel,
    bsl_top_tgl_t top_tgl
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)top_ext; /* else is declared but not used */
	(void)de_ext;  /* else is declared but not used */
	(void)top_sel; /* else is declared but not used */
	(void)top_tgl; /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(top_ext > HDMITX_INTEXT_NO_CHANGE)
	RETIF_BADPARAM(de_ext  > HDMITX_INTEXT_NO_CHANGE)
	RETIF_BADPARAM(top_sel > HDMITX_TOPSEL_NO_CHANGE)
	RETIF_BADPARAM(top_tgl > HDMITX_TOPTGL_NO_CHANGE)

	/* Set each optional parameter */
	if(top_ext != HDMITX_INTEXT_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_top_ext,
		                            (u8)top_ext);
		RETIF_REG_FAIL(err)
	}
	if(de_ext != HDMITX_INTEXT_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_de_ext,
		                            (u8)de_ext);
		RETIF_REG_FAIL(err)
	}
	if(top_sel != HDMITX_TOPSEL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_top_sel,
		                            (u8)top_sel);
		RETIF_REG_FAIL(err)
	}
	if(top_tgl != HDMITX_TOPTGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_top_tgl,
		                            (u8)top_tgl);
		RETIF_REG_FAIL(err)
	}
	return 0;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslScalerSetFine                                                   */
/*============================================================================*/
error_code_t
bsl_scaler_set_fine
(
    unit_select_t  tx_unit,
    u16          u_ref_pix,
    u16          u_ref_line
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)u_ref_pix;  /* else is declared but not used */
	(void)u_ref_line; /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(u_ref_pix  >= HDMITX_SCALER_FINE_PIXEL_INVALID)
	RETIF_BADPARAM(u_ref_line >= HDMITX_SCALER_FINE_LINE_INVALID)

	if(u_ref_pix < HDMITX_SCALER_FINE_PIXEL_NO_CHANGE) {
		err = set_hw_register_msb_lsb(p_dis, E_REG_P01_REFPIX_MSB_W, u_ref_pix);
		RETIF_REG_FAIL(err)
	}
	if(u_ref_line < HDMITX_SCALER_FINE_LINE_NO_CHANGE) {
		err = set_hw_register_msb_lsb(p_dis, E_REG_P01_REFLINE_MSB_W, u_ref_line);
		RETIF_REG_FAIL(err)
	}
	return 0;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslScalerSetSync                                                   */
/*============================================================================*/
error_code_t
bsl_scaler_set_sync
(
    unit_select_t      tx_unit,
    bsl_vs_meth_t method,
    bsl_vs_once_t once
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

#ifdef TMFL_TDA9981_SUPPORT
	(void)method; /* else is declared but not used */
	(void)once;   /* else is declared but not used */
	return ERR_HDMI_NOT_SUPPORTED;
#else
	/* Check if this device has a Scaler */
	RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
	      ERR_HDMI_NOT_SUPPORTED)

	/* Check parameters */
	RETIF_BADPARAM(method >= HDMITX_VSMETH_INVALID)
	RETIF_BADPARAM(once   >= HDMITX_VSONCE_INVALID)

	/* Set each optional parameter */
	if(method != HDMITX_VSMETH_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_sync_mthd,
		                            (u8)method);
		RETIF_REG_FAIL(err)
	}
	if(once != HDMITX_VSONCE_NO_CHANGE) {
		/* Must be last register set */
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_TBG_CNTRL_0_W,
		                            e_maskreg_p01_tbg_cntrl_0_sync_once,
		                            (u8)once);
		RETIF_REG_FAIL(err)
		if(once == HDMITX_VSONCE_ONCE) {
			/* Toggle output Sync Once flag for settings to take effect */
			err = set_hw_register_field(p_dis,
			                            E_REG_P01_TBG_CNTRL_0_W,
			                            e_maskreg_p01_tbg_cntrl_0_sync_once,
			                            (u8)HDMITX_VSONCE_EACH_FRAME);
			RETIF_REG_FAIL(err)
			err = set_hw_register_field(p_dis,
			                            E_REG_P01_TBG_CNTRL_0_W,
			                            e_maskreg_p01_tbg_cntrl_0_sync_once,
			                            (u8)HDMITX_VSONCE_ONCE);
			RETIF_REG_FAIL(err)
		}
	}
	return 0;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslSwGetVersion                                                    */
/*============================================================================*/
error_code_t
bsl_sw_get_version
(
    p_swversion_t      p_swversion
)
{
	/* Check parameters */
	RETIF_BADPARAM(p_swversion == (p_swversion_t)0)

	/* Get the version details of the component. */
	p_swversion->compatibility_nr = HDMITX_BSL_COMP_NUM;
	p_swversion->major_version_nr  = HDMITX_BSL_MAJOR_VER;
	p_swversion->minor_version_nr  = HDMITX_BSL_MINOR_VER;

	return 0;
}

/*============================================================================*/
/* bslSysTimerWait                                                    */
/*============================================================================*/
error_code_t
bsl_sys_timer_wait
(
    unit_select_t      tx_unit,
    u16              wait_ms
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return if this device timer is not set up */
	RETIF(!p_dis->sys_func_timer, ERR_HDMI_NOT_INITIALIZED)

	/* Wait for the requested time */
	p_dis->sys_func_timer(wait_ms);

	return 0;
}

/*============================================================================*/
/* bslTmdsSetOutputs                                                  */
/*============================================================================*/
error_code_t
bsl_tmds_set_outputs
(
    unit_select_t          tx_unit,
    bsl_tmds_out_t    tmds_out
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(tmds_out >= HDMITX_TMDSOUT_INVALID)

	/* Set the TMDS output mode */
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_BUFFER_OUT_RW,
	                            e_maskreg_p02_buffer_out_srl_force,
	                            (u8)tmds_out);
	return err;
}

/*============================================================================*/
/* bslTmdsSetSerializer                                               */
/*============================================================================*/
error_code_t
bsl_tmds_set_serializer
(
    unit_select_t  tx_unit,
    u8           u_phase2,
    u8           u_phase3
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(u_phase2 >= HDMITX_TMDSPHASE_INVALID)
	RETIF_BADPARAM(u_phase3 >= HDMITX_TMDSPHASE_INVALID)

	/* Set the serializer phase 2 & 3 counts */
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_SERIALIZER_RW,
	                            e_maskreg_p02_serializer_srl_phase2,
	                            u_phase2);
	RETIF_REG_FAIL(err)
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_SERIALIZER_RW,
	                            e_maskreg_p02_serializer_srl_phase3,
	                            u_phase3);
	return err;
}

/*============================================================================*/
/* bslTestSetPattern                                                  */
/*============================================================================*/
error_code_t
bsl_test_set_pattern
(
    unit_select_t           tx_unit,
    bsl_test_pattern_t pattern
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t *p_dis;
	error_code_t     err;						/* Error code */
	u8             service_mode;				/* Register value */
	u8             bars8;					/* Register value */
	u8			  buf[MATRIX_PRESET_SIZE];	/* Temp buffer */
	u8			  i;						/* Loop index */
	u8			  hbl_off;					/* FB: define hbl offset*/
	u8			  sc_in_fmt = 0;				/* Scaler input format */
	u8			  sc_out_fmt = 0;				/* Scaler output format */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check pattern parameters */
	switch(pattern) {
	case HDMITX_PATTERN_CBAR4:
		service_mode = 1;
		bars8 = 0;
		break;
	case HDMITX_PATTERN_BLUE:
	case HDMITX_PATTERN_CBAR8:
		service_mode = 1;
		bars8 = 1;
		break;
	case HDMITX_PATTERN_OFF:
		service_mode = 0;
		bars8 = 0;
		break;
	default:
		return ERR_HDMI_BAD_PARAMETER;
	}

	if(service_mode) {
		/* if pattern is on, register is already saved */
		if(!p_dis->prev_pattern) {
			/*  The kBypassColourProc registers are saved in bslTDA9984VideoSetInOut API */
			/* Bypass up/down sampler and matrix for RGB colourbars */
			set_hw_register_field_table(p_dis, &k_bypass_colour_proc[0]);

			/* disable scaler blanking if necessary */
			if(p_dis->sca_mode == HDMITX_SCAMODE_ON) {
				/* remove blanking */
				err = set_hw_register(p_dis, E_REG_P00_HBL_OFFSET_START_W, 0);
				RETIF_REG_FAIL(err)
			}
		}
		if(pattern == HDMITX_PATTERN_BLUE) {  /* blue screen pattern */

			/* To create blue screen, we use the internal color bar 8 on which we apply a matrix to change it to blue */
			/* Set the first block byte separately, as it is shadowed and can't
			 * be set by setHwRegisters */

			/* Set the first block byte separately, as it is shadowed and can't
			 * be set by setHwRegisters */
			err = set_hw_register(p_dis,
			                      E_REG_P00_MAT_CONTRL_W,
			                      matrix_coeff_blue_screen[p_dis->vout_mode][0]);
			RETIF_REG_FAIL(err)

			for(i = 0; i < MATRIX_PRESET_SIZE; i++) {
				buf[i] = matrix_coeff_blue_screen[p_dis->vout_mode][i];
			}

			/* Set the rest of the block */
			err = set_hw_registers(p_dis,
			                       E_REG_P00_MAT_OI1_MSB_W,
			                       &buf[1],
			                       MATRIX_PRESET_SIZE - 1);
			RETIF_REG_FAIL(err)

			if(p_dis->vout_mode == HDMITX_VOUTMODE_YUV422) {
				/* pattern is 444 -> convert to 422 */
				err = set_hw_register_field(p_dis,
				                            E_REG_P00_HVF_CNTRL_1_W,
				                            e_maskreg_p00_hvf_cntrl_1_for,
				                            1);
			}

			p_dis->prev_bluescreen = true;
		} else { /* colour bars patterns */
			/* Set number of colour bars */
			err = set_hw_register_field(p_dis,
			                            E_REG_P00_HVF_CNTRL_0_W,
			                            e_maskreg_p00_hvf_cntrl_0_rwb,
			                            bars8);
			RETIF_REG_FAIL(err)

			/* Bypass up/down sampler and matrix for RGB colourbars */
			set_hw_register_field_table(p_dis, &k_bypass_colour_proc[0]);
		}
		p_dis->prev_pattern = true;
	} else { /* serviceMode == 0 */
		if(p_dis->prev_bluescreen) {
			/* Restore the previous Matrix when pattern goes off */
			err = bsl_matrix_set_conversion(tx_unit, p_dis->vin_fmt, p_dis->vin_mode, p_dis->vout_fmt, p_dis->vout_mode, p_dis->dvi_vqr);
			RETIF_REG_FAIL(err)

			/* Restore the correct output sampler mode */
			err = set_sampling(p_dis);
			RETIF(err != 0, err)

			p_dis->prev_bluescreen = false;
		}

		/* Restore kBypassColourProc registers when pattern goes off */
		set_hw_register(p_dis, E_REG_P00_MAT_CONTRL_W,  g_mat_contrl[tx_unit]);
		set_hw_register(p_dis, E_REG_P00_HVF_CNTRL_0_W, g_hvf_cntrl0[tx_unit]);
		set_hw_register(p_dis, E_REG_P00_HVF_CNTRL_1_W, g_hvf_cntrl1[tx_unit]);
		p_dis->prev_pattern = false;

		/* restore scaler blanking if necessary */
		if(p_dis->sca_mode == HDMITX_SCAMODE_ON) {
			/* Look up scaler register formats from table */
			sc_in_fmt = SCIO2SCIN(k_vfmt_to_regvfmt_scio_tv[p_dis->vin_fmt]);
			sc_out_fmt = SCIO2SCOUT(k_vfmt_to_regvfmt_scio_tv[p_dis->vout_fmt]);

			/* Look-up blanking re-insertion */
			hbl_off = k_scl_add_blk_pix[sc_in_fmt][sc_out_fmt];
			err = set_hw_register(p_dis, E_REG_P00_HBL_OFFSET_START_W, hbl_off);
			RETIF_REG_FAIL(err)
			/* clean green lines for none RGB output */
			err = set_hw_register_field(p_dis,
			                            E_REG_P00_HVF_CNTRL_1_W,
			                            e_maskreg_p00_hvf_cntrl_1_yuvblk,
			                            k_scl_clear_blk_pix[p_dis->vout_mode]);
			RETIF_REG_FAIL(err)
		}
	}

	/* Set Service Mode on or off */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_HVF_CNTRL_0_W,
	                            e_maskreg_p00_hvf_cntrl_0_sm,
	                            service_mode);

	return err;
}

/*============================================================================*/
/* bslTestSetMode                                                     */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_test_set_mode
(
    unit_select_t          tx_unit,
    bsl_test_mode_t   test_mode,
    bsl_test_state_t  test_state
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */
	/* Register used to activate a test */
	u16            test_reg = E_REG_P00_VIP_CNTRL_4_W;
	/* Register bitfield mask used */
	u8             test_mask = e_maskreg_p00_vip_cntrl_4_tst_pat;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(test_mode  >= HDMITX_TESTMODE_INVALID)
	RETIF_BADPARAM(test_state >= HDMITX_TESTSTATE_INVALID)

	/* Set the mode selected by testMode to the state indicated by testState */
	switch(test_mode) {
	case HDMITX_TESTMODE_PAT:
		test_reg  = E_REG_P00_VIP_CNTRL_4_W;
		test_mask = e_maskreg_p00_vip_cntrl_4_tst_pat;
		break;
	case HDMITX_TESTMODE_656:
		test_reg  = E_REG_P00_VIP_CNTRL_4_W;
		test_mask = e_maskreg_p00_vip_cntrl_4_tst_656;
		break;
	case HDMITX_TESTMODE_SERPHOE:
		test_reg  = E_REG_P02_TEST1_RW;
		test_mask = e_maskreg_p02_test1_tstserphoe;
		break;
	case HDMITX_TESTMODE_NOSC:
		test_reg  = E_REG_P02_TEST1_RW;
		test_mask = e_maskreg_p02_test1_tst_nosc;
		break;
	case HDMITX_TESTMODE_HVP:
		test_reg  = E_REG_P02_TEST1_RW;
		test_mask = e_maskreg_p02_test1_tst_hvp;
		break;
	case HDMITX_TESTMODE_PWD:
		test_reg  = E_REG_P02_TEST2_RW;
		test_mask = e_maskreg_p02_test2_pwd1v8;
		break;
	case HDMITX_TESTMODE_DIVOE:
		test_reg  = E_REG_P02_TEST2_RW;
		test_mask = e_maskreg_p02_test2_divtestoe;
		break;
	case HDMITX_TESTMODE_INVALID:
		break;
	}
	err = set_hw_register_field(p_dis, test_reg, test_mask, (u8)test_state);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslVideoInSetBlanking                                              */
/*============================================================================*/
error_code_t
bsl_video_in_set_blanking
(
    unit_select_t         tx_unit,
    bsl_blnk_src_t   blankit_source,
    bsl_blnk_code_t  blanking_codes
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(blankit_source >= HDMITX_BLNKSRC_INVALID)
	RETIF_BADPARAM(blanking_codes >= HDMITX_BLNKCODE_INVALID)

	/* For each parameter that is not No Change, set its register */
	if(blankit_source != HDMITX_BLNKSRC_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_4_W,
		                            e_maskreg_p00_vip_cntrl_4_blankit,
		                            (u8)blankit_source);
		RETIF_REG_FAIL(err)
	}
	if(blanking_codes != HDMITX_BLNKCODE_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_4_W,
		                            e_maskreg_p00_vip_cntrl_4_blc,
		                            (u8)blanking_codes);
		RETIF_REG_FAIL(err)
	}

	return 0;
}

/*============================================================================*/
/* bslVideoInSetConfig                                                */
/*============================================================================*/
error_code_t
bsl_video_in_set_config
(
    unit_select_t            tx_unit,
    bsl_vin_mode_t      vin_mode,
    bsl_pix_edge_t      sample_edge,
    bsl_pix_rate_t      pix_rate,
    bsl_upsample_mode_t upsample_mode
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(vin_mode      >= HDMITX_VINMODE_INVALID)
	RETIF_BADPARAM(sample_edge   >= HDMITX_PIXEDGE_INVALID)
	RETIF_BADPARAM(pix_rate      >= HDMITX_PIXRATE_INVALID)
	RETIF_BADPARAM(upsample_mode >= HDMITX_UPSAMPLE_INVALID)

	if(vin_mode != HDMITX_VINMODE_NO_CHANGE) {
		switch(vin_mode) {
		case HDMITX_VINMODE_CCIR656:
			err = set_hw_register_field_table(p_dis, &k_vin_mode_ccir656[0]);
			RETIF_REG_FAIL(err)
			break;
		case HDMITX_VINMODE_YUV422:
			err = set_hw_register_field_table(p_dis, &k_vin_mode_yuv422[0]);
			RETIF_REG_FAIL(err)
			break;
		case HDMITX_VINMODE_RGB444:
		case HDMITX_VINMODE_YUV444:
		default:
			err = set_hw_register_field_table(p_dis, &k_vin_mode444[0]);
			RETIF_REG_FAIL(err)
			break;
		}
		err = set_hw_register_field(p_dis,
		                            E_REG_P02_CCIR_DIV_RW,
		                            e_maskreg_p02_ccir_div_refdiv2,
		                            0);
		RETIF_REG_FAIL(err)
		p_dis->vin_mode = vin_mode;
	}
	if(sample_edge != HDMITX_PIXEDGE_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_edge,
		                            (u8)sample_edge);
		RETIF_REG_FAIL(err)
	}
	if(pix_rate != HDMITX_PIXRATE_NO_CHANGE) {
		p_dis->pix_rate = pix_rate;

		if(pix_rate == HDMITX_PIXRATE_SINGLE_REPEATED) {

			err = set_hw_register_field(p_dis,
			                            E_REG_P02_CCIR_DIV_RW,
			                            e_maskreg_p02_ccir_div_refdiv2,
			                            1);
			RETIF_REG_FAIL(err)

			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_SCG2_RW,
			                            e_maskreg_p02_pll_scg2_selpllclkin,
			                            0);
			RETIF_REG_FAIL(err)

			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_DE_RW,
			                            e_maskreg_p02_pll_de_pllde_fdn,
			                            1);
			RETIF_REG_FAIL(err)

			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_DE_RW,
			                            e_maskreg_p02_pll_de_bypass_pllde,
			                            1);
			RETIF_REG_FAIL(err)
		} else {

			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_SCG2_RW,
			                            e_maskreg_p02_pll_scg2_selpllclkin,
			                            (u8)pix_rate);
			RETIF_REG_FAIL(err)
			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_DE_RW,
			                            e_maskreg_p02_pll_de_bypass_pllde,
			                            (u8)pix_rate);
			RETIF_REG_FAIL(err)
			err = set_hw_register_field(p_dis,
			                            E_REG_P02_PLL_DE_RW,
			                            e_maskreg_p02_pll_de_pllde_fdn,
			                            (u8)pix_rate);
			RETIF_REG_FAIL(err)
		}
	}
	if(upsample_mode != HDMITX_UPSAMPLE_NO_CHANGE) {
		p_dis->upsample_mode = upsample_mode;
	}
	return 0;
}

/*============================================================================*/
/* bslVideoInSetFine                                                  */
/*============================================================================*/
error_code_t
bsl_video_in_set_fine
(
    unit_select_t            tx_unit,
    bsl_pix_subpkt_t    subpacket_count,
    bsl_pix_togl_t      toggle_clk1
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(subpacket_count >= HDMITX_PIXSUBPKT_INVALID)
	RETIF_BADPARAM(toggle_clk1     >= HDMITX_PIXTOGL_INVALID)

	/* IF subpacketCount is Fix at 0/1/2/3 THEN set subpacket count register
	 * to 0/1/2/3 and set subpacket sync register to 3
	 *	 */
	if(subpacket_count <= HDMITX_PIXSUBPKT_FIX_3) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_5_W,
		                            e_maskreg_p00_vip_cntrl_5_sp_cnt,
		                            (u8)subpacket_count);
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_sp_sync,
		                            HDMITX_PIXSUBPKT_SYNC_FIXED);
		RETIF_REG_FAIL(err)
	}
	/* ELSE IF subpacketCount is Sync by Hemb/ Sync by Rising Edge DE/
	 * Sync by Rising Edge HS THEN set the unused subpacket count to zero and
	 * set subpacket sync register to 0/1/2
	 *	 */
	else if(subpacket_count != HDMITX_PIXSUBPKT_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_5_W,
		                            e_maskreg_p00_vip_cntrl_5_sp_cnt,
		                            HDMITX_PIXSUBPKT_FIX_0);
		RETIF_REG_FAIL(err)

		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_sp_sync,
		                            (u8)(subpacket_count - HDMITX_PIXSUBPKT_SYNC_FIRST));
		RETIF_REG_FAIL(err)
	}

	/* IF toggleClk1 is not No Change THEN set ckcase bitfield */
	if(toggle_clk1 != HDMITX_PIXTOGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_5_W,
		                            e_maskreg_p00_vip_cntrl_5_ckcase,
		                            (u8)toggle_clk1);
		RETIF_REG_FAIL(err)
	}
	return 0;
}

/*============================================================================*/
/* bslVideoInSetMapping                                               */
/*============================================================================*/
error_code_t
bsl_video_in_set_mapping
(
    unit_select_t  tx_unit,
    u8           *p_swap_table,
    u8           *p_mirror_table
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */
	int               i;          /* Loop counter */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(p_swap_table == NULL)
	RETIF_BADPARAM(p_mirror_table == NULL)
	for(i = 0; i < HDMITX_VIN_PORT_MAP_TABLE_LEN; i++) {
		RETIF_BADPARAM(p_swap_table[i] >= HDMITX_VIN_PORT_SWAP_INVALID)
		RETIF_BADPARAM(p_mirror_table[i] >= HDMITX_VIN_PORT_MIRROR_INVALID)
	}

	/* IF pswapTable[n] is not No Change THEN set the port swap registers from
	 * pswapTable[n]
	 *	 */
	for(i = 0; i < HDMITX_VIN_PORT_MAP_TABLE_LEN; i++) {
		if(p_swap_table[0] < HDMITX_VIN_PORT_SWAP_NO_CHANGE) {
			err = set_hw_register_field(p_dis,
			                            k_reg_vip[i].reg,
			                            k_reg_vip[i].mask_swap,
			                            p_swap_table[i]);
			RETIF_REG_FAIL(err)
		}
	}

	/* IF pMirrorTable[n] is not No Change THEN set the port mirror registers
	 * from pMirrorTable[n]
	 *	 */
	for(i = 0; i < HDMITX_VIN_PORT_MAP_TABLE_LEN; i++) {
		if(p_mirror_table[0] < HDMITX_VIN_PORT_MIRROR_NO_CHANGE) {
			err = set_hw_register_field(p_dis,
			                            k_reg_vip[i].reg,
			                            k_reg_vip[i].mask_mirror,
			                            p_mirror_table[i]);
			RETIF_REG_FAIL(err)
		}
	}

	return 0;
}

/*============================================================================*/
/* bslSetVideoPortConfig                                              */
/*============================================================================*/
error_code_t
bsl_set_video_port_config
(
    unit_select_t  tx_unit,
    u8			*p_ena_video_port_table,
    u8			*p_gnd_video_port_table
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(p_ena_video_port_table == NULL)
	RETIF_BADPARAM(p_gnd_video_port_table == NULL)

#ifdef TMFL_TDA9981_SUPPORT
	err = set_hw_register(p_dis,
	                      E_REG_P00_ENA_VP_0_RW,
	                      p_ena_video_port_table[0]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_ENA_VP_1_RW,
	                      p_ena_video_port_table[1]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_ENA_VP_2_RW,
	                      p_ena_video_port_table[2]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_GND_VP_0_RW,
	                      p_gnd_video_port_table[0]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_GND_VP_1_RW,
	                      p_gnd_video_port_table[1]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_GND_VP_2_RW,
	                      p_gnd_video_port_table[2]);
	RETIF_REG_FAIL(err)

	return 0;
#else /* TMFL_TDA9981_SUPPORT */
	(void)p_ena_video_port_table;
	(void)p_gnd_video_port_table;
	return ERR_HDMI_NOT_SUPPORTED;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslSetAudioPortConfig                                              */
/*============================================================================*/
error_code_t
bsl_set_audio_port_config
(
    unit_select_t  tx_unit,
    u8			*p_ena_audio_port_table,
    u8			*p_gnd_audio_port_table
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(p_ena_audio_port_table == NULL)
	RETIF_BADPARAM(p_gnd_audio_port_table == NULL)

#ifdef TMFL_TDA9981_SUPPORT
	err = set_hw_register(p_dis,
	                      E_REG_P00_ENA_AP_RW,
	                      p_ena_audio_port_table[0]);
	RETIF_REG_FAIL(err)

	err = set_hw_register(p_dis,
	                      E_REG_P00_GND_AP_RW,
	                      p_gnd_audio_port_table[0]);
	RETIF_REG_FAIL(err)

	return 0;
#else /* TMFL_TDA9981_SUPPORT */
	(void)p_ena_audio_port_table;
	(void)p_gnd_audio_port_table;
	return ERR_HDMI_NOT_SUPPORTED;
#endif /* TMFL_TDA9981_SUPPORT */
}

/*============================================================================*/
/* bslVideoInSetSyncAuto                                              */
/*============================================================================*/
error_code_t
bsl_video_in_set_sync_auto
(
    unit_select_t            tx_unit,
    bsl_sync_source_t   sync_source,
    bsl_vid_fmt_t       vin_fmt,
    bsl_vin_mode_t      vin_mode
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */
	u8             reg_vin_fmt;  /* Video i/p fmt value used for comparison */
	u8             embedded;   /* Register value */
	u8             sync_method; /* Sync method */
	u8             toggle_v;    /* V toggle */
	u8             toggle_h;    /* H toggle */
	u8             toggle_x;    /* X toggle */
	u16            u_ref_pix;    /* Output refpix */
	u16            u_ref_line;   /* Output refline */
	bsl_vid_fmt_t  vin_fmt_index; /* Index in tab kVfmtToRegvfmt_TV*/
	u8             reg_vin_fmt_no_reg;
	u8             reg_val;
#ifdef FORMAT_PC
	u8             reg_vin_fmt_pc;
#endif /* FORMAT_PC */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters - syncSource must be specified */
	RETIF_BADPARAM(sync_source   >= HDMITX_SYNCSRC_NO_CHANGE)

#ifdef FORMAT_PC
	RETIF_BADPARAM(vin_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM((vin_fmt > HDMITX_VFMT_TV_MAX) && (vin_fmt < HDMITX_VFMT_PC_MIN))
	RETIF_BADPARAM(vin_fmt       > HDMITX_VFMT_PC_MAX)
#else /* FORMAT_PC */
	/*FORMAT TV only*/
	RETIF_BADPARAM(vin_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vin_fmt       > HDMITX_VFMT_TV_MAX)
#endif /* FORMAT_PC */

	vin_fmt_index = vin_fmt;
#ifdef FORMAT_PC
	if(vin_fmt_index > HDMITX_VFMT_PC_MIN) {
		vin_fmt_index = (bsl_vid_fmt_t)(vin_fmt_index - HDMITX_VFMT_PC_MIN + HDMITX_VFMT_TV_MAX + 1);
	}
#endif /* FORMAT_PC */

	/* Look up the VIDFORMAT register format from the register format table */
	reg_vin_fmt = k_vfmt_to_regvfmt_tv[vin_fmt_index];
	/* Quit if the input format does not map to the register format */
	RETIF_BADPARAM(reg_vin_fmt == E_REGVFMT_INVALID)

	/* Select values according to sync source */
	embedded = 0;
	switch(sync_source) {
	case HDMITX_SYNCSRC_EXT_VS:
		sync_method = 0;
		toggle_v    = k_regvfmt_to_vtoggle[reg_vin_fmt];
		toggle_h    = k_regvfmt_to_htoggle[reg_vin_fmt];
		toggle_x    = 0;
		u_ref_pix    = k_vid_fmt_to_refpix_refline[reg_vin_fmt].ref_pix_vs_sync;
		u_ref_line   = k_vid_fmt_to_refpix_refline[reg_vin_fmt].ref_line_vs_sync;
		break;
	case HDMITX_SYNCSRC_EMBEDDED:
		embedded++;
		/* fall thru */
	case HDMITX_SYNCSRC_EXT_VREF:
	default:
		sync_method = 1;
		toggle_v    = 1;
		toggle_h    = 1;
		toggle_x    = 1;
		u_ref_pix    = k_vid_fmt_to_refpix_refline[reg_vin_fmt].ref_pix_other_sync;
		u_ref_line   = k_vid_fmt_to_refpix_refline[reg_vin_fmt].ref_line_other_sync;
		break;
	}
	/* Table has +1 added to refpix values which are not needed in
	 *	   RGB444, YUV444 and YUV422 modes, but +2 is required in those cases */
	if(vin_mode != HDMITX_VINMODE_CCIR656) {
		u_ref_pix = u_ref_pix + 2;
	}

	/* Set embedded sync */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_VIP_CNTRL_3_W,
	                            e_maskreg_p00_vip_cntrl_3_emb,
	                            embedded);
	RETIF_REG_FAIL(err)

	/* Set sync method */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_TBG_CNTRL_0_W,
	                            e_maskreg_p00_tbg_cntrl_0_sync_mthd,
	                            sync_method);
	RETIF_REG_FAIL(err)

	/* Set VH toggle */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_VIP_CNTRL_3_W,
	                            e_maskreg_p00_vip_cntrl_3_v_tgl,
	                            toggle_v);
	RETIF_REG_FAIL(err)
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_VIP_CNTRL_3_W,
	                            e_maskreg_p00_vip_cntrl_3_h_tgl,
	                            toggle_h);
	RETIF_REG_FAIL(err)

	/* Set X toggle */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_VIP_CNTRL_3_W,
	                            e_maskreg_p00_vip_cntrl_3_x_tgl,
	                            toggle_x);
	RETIF_REG_FAIL(err)

	if((vin_fmt >= HDMITX_VFMT_TV_NO_REG_MIN)
	   && (vin_fmt < HDMITX_VFMT_PC_MIN)) {
		RETIF_BADPARAM(reg_vin_fmt > E_REGVFMT_NUM_TV)

		/* Check parameters */
		reg_vin_fmt_no_reg = (u8)(reg_vin_fmt - E_REGVFMT_FIRST_TV_NO_REG);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].n_pix;
		err = set_hw_register(p_dis, E_REG_P00_NPIX_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].n_pix >> 8);
		err = set_hw_register(p_dis, E_REG_P00_NPIX_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].n_line;
		err = set_hw_register(p_dis, E_REG_P00_NLINE_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].n_line >> 8);
		err = set_hw_register(p_dis, E_REG_P00_NLINE_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_line_start;
		err = set_hw_register(p_dis, E_REG_P00_VS_LINE_STRT_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_pix_start;
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_STRT_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_pix_start >> 8);
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_STRT_1_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_line_end;
		err = set_hw_register(p_dis, E_REG_P00_VS_LINE_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_pix_end;
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].vs_pix_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_END_1_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].hs_start;
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_START_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].hs_start >> 8);
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_START_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].hs_end;
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_STOP_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].hs_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_STOP_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].v_win_start;
		err = set_hw_register(p_dis, E_REG_P00_VWIN_START_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].v_win_end;
		err = set_hw_register(p_dis, E_REG_P00_VWIN_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].v_win_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_VWIN_END_1_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].de_start;
		err = set_hw_register(p_dis, E_REG_P00_DE_START_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].de_start >> 8);
		err = set_hw_register(p_dis, E_REG_P00_DE_START_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_format_param[reg_vin_fmt_no_reg].de_end;
		err = set_hw_register(p_dis, E_REG_P00_DE_STOP_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_format_param[reg_vin_fmt_no_reg].de_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_DE_STOP_MSB_W, reg_val);
		RETIF_REG_FAIL(err);
	}

#ifdef FORMAT_PC

	if(vin_fmt > HDMITX_VFMT_PC_MIN) {

		/* Check parameters */
		RETIF_BADPARAM(reg_vin_fmt < E_REGVFMT_NUM_TV)

		reg_vin_fmt_pc = (u8)(reg_vin_fmt - E_REGVFMT_FIRST_PC_FORMAT);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].n_pix;
		err = set_hw_register(p_dis, E_REG_P00_NPIX_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].n_pix >> 8);
		err = set_hw_register(p_dis, E_REG_P00_NPIX_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].n_line;
		err = set_hw_register(p_dis, E_REG_P00_NLINE_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].n_line >> 8);
		err = set_hw_register(p_dis, E_REG_P00_NLINE_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].vs_line_start;
		err = set_hw_register(p_dis, E_REG_P00_VS_LINE_STRT_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].vs_pix_start;
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_STRT_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].vs_line_end;
		err = set_hw_register(p_dis, E_REG_P00_VS_LINE_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].vs_pix_end;
		err = set_hw_register(p_dis, E_REG_P00_VS_PIX_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].hs_start;
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_START_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].hs_end;
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_STOP_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].hs_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_HS_PIX_STOP_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].v_win_start;
		err = set_hw_register(p_dis, E_REG_P00_VWIN_START_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].v_win_end;
		err = set_hw_register(p_dis, E_REG_P00_VWIN_END_1_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].v_win_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_VWIN_END_1_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].de_start;
		err = set_hw_register(p_dis, E_REG_P00_DE_START_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].de_start >> 8);
		err = set_hw_register(p_dis, E_REG_P00_DE_START_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].de_end;
		err = set_hw_register(p_dis, E_REG_P00_DE_STOP_LSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = (u8)(k_vid_fmt_to_pcformat_param[reg_vin_fmt_pc].de_end >> 8);
		err = set_hw_register(p_dis, E_REG_P00_DE_STOP_MSB_W, reg_val);
		RETIF_REG_FAIL(err);

		reg_val = DEPTH_COLOR_PC;
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_1_W,
		                            e_maskreg_p00_hvf_cntrl_1_pad,
		                            reg_val);

		RETIF_REG_FAIL(err);
	}
#endif /* FORMAT_PC */

	/* Set refpix, refline */
	/* tweak for gplugD */
	u_ref_pix = 0x99;
	u_ref_line = 0x07;
	err = set_hw_register_msb_lsb(p_dis, E_REG_P00_REFPIX_MSB_W, u_ref_pix);
	RETIF_REG_FAIL(err)
	err = set_hw_register_msb_lsb(p_dis, E_REG_P00_REFLINE_MSB_W, u_ref_line);
	return err;
}

/*============================================================================*/
/* bslVideoInSetSyncManual                                            */
/*============================================================================*/

error_code_t
bsl_video_in_set_sync_manual
(
    unit_select_t            tx_unit,
    bsl_sync_source_t   sync_source,
    bsl_vs_meth_t       sync_method,
    bsl_pix_togl_t      toggle_v,
    bsl_pix_togl_t      toggle_h,
    bsl_pix_togl_t      toggle_x,
    u16                    u_ref_pix,
    u16                    u_ref_line
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */
	u8             embedded;   /* Register value */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(sync_source   >= HDMITX_SYNCSRC_INVALID)
	RETIF_BADPARAM(sync_method   >= HDMITX_VSMETH_INVALID)
	RETIF_BADPARAM(toggle_v      >= HDMITX_PIXTOGL_INVALID)
	RETIF_BADPARAM(toggle_h      >= HDMITX_PIXTOGL_INVALID)
	RETIF_BADPARAM(toggle_x      >= HDMITX_PIXTOGL_INVALID)
	RETIF_BADPARAM(u_ref_pix  >= HDMITX_VOUT_FINE_PIXEL_INVALID)
	RETIF_BADPARAM(u_ref_line >= HDMITX_VOUT_FINE_LINE_INVALID)

	if(sync_source != HDMITX_SYNCSRC_NO_CHANGE) {
		if(sync_source == HDMITX_SYNCSRC_EMBEDDED) {
			embedded = 1;
		} else {
			embedded = 0;
		}
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_emb,
		                            embedded);
		RETIF_REG_FAIL(err)
	}
	if(sync_method != HDMITX_VSMETH_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_0_W,
		                            e_maskreg_p00_tbg_cntrl_0_sync_mthd,
		                            (u8)sync_method);
		RETIF_REG_FAIL(err)
	}
	if(toggle_v != HDMITX_PIXTOGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_v_tgl,
		                            (u8)toggle_v);
		RETIF_REG_FAIL(err)
	}
	if(toggle_h != HDMITX_PIXTOGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_h_tgl,
		                            (u8)toggle_h);
		RETIF_REG_FAIL(err)
	}
	if(toggle_x != HDMITX_PIXTOGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_VIP_CNTRL_3_W,
		                            e_maskreg_p00_vip_cntrl_3_x_tgl,
		                            (u8)toggle_x);
		RETIF_REG_FAIL(err)
	}
	if(u_ref_pix < HDMITX_VOUT_FINE_PIXEL_NO_CHANGE) {
		err = set_hw_register_msb_lsb(p_dis, E_REG_P00_REFPIX_MSB_W, u_ref_pix);
		RETIF_REG_FAIL(err)
	}
	if(u_ref_line < HDMITX_VOUT_FINE_LINE_NO_CHANGE) {
		err = set_hw_register_msb_lsb(p_dis, E_REG_P00_REFLINE_MSB_W, u_ref_line);
		RETIF_REG_FAIL(err)
	}

	return 0;
}

/*============================================================================*/
/* bslVideoOutDisable                                                 */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_video_out_disable
(
    unit_select_t  tx_unit,
    bool            b_disable
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(b_disable > true)

	/* Set or clear frame_dis in the scaler Timebase Control 0 register
	 * according to bDisable
	 *	 */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_TBG_CNTRL_0_W,
	                            e_maskreg_p00_tbg_cntrl_0_frame_dis,
	                            (u8)b_disable);
	if(b_disable) {
		set_state(p_dis, EV_OUTDISABLE);
	}
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslVideoOutSetConfig                                               */
/*============================================================================*/
error_code_t
bsl_video_out_set_config
(
    unit_select_t            tx_unit,
    bsl_sink_type_t     sink_type,
    bsl_vout_mode_t     vout_mode,
    bsl_vout_prefil_t   pre_filter,
    bsl_vout_yuv_blnk_t  yuv_blank,
    bsl_vout_qrange_t   quantization
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */
	u8             reg_val;     /* Register value */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(sink_type     >= HDMITX_SINK_INVALID)
	RETIF_BADPARAM(vout_mode     >= HDMITX_VOUTMODE_INVALID)
	RETIF_BADPARAM(pre_filter    >= HDMITX_VOUT_PREFIL_INVALID)
	RETIF_BADPARAM(yuv_blank     >= HDMITX_VOUT_YUV_BLNK_INVALID)
	RETIF_BADPARAM(quantization >= HDMITX_VOUT_QRANGE_INVALID)

	(void)DUMMY_ACCESS(quantization);

	if(sink_type == HDMITX_SINK_EDID) {
		if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
			/* EDID has not been read so assume simplest sink */
			p_dis->sink_type = HDMITX_SINK_DVI;
		} else {
			/* EDID has been read so set sink to the type that was read */
			p_dis->sink_type = p_dis->edid_sink_type;
		}
	} else {
		/* Set demanded sink type */
		p_dis->sink_type = sink_type;
	}

	/* Is DVI sink required? */
	if(p_dis->sink_type == HDMITX_SINK_DVI) {
		/* Mute the audio FIFO */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_AIP_CNTRL_0_RW,
		                            e_maskreg_p11_aip_cntrl_0_rst_fifo,
		                            1);
		RETIF_REG_FAIL(err)

		/* Force RGB mode for DVI sink */
		vout_mode = HDMITX_VOUTMODE_RGB444;

		/* Set HDMI HDCP mode off for DVI */
		err = set_hw_register_field_table(p_dis, &k_vout_hdcp_off[0]);
		RETIF_REG_FAIL(err)

		reg_val = 0;
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_ENC_CNTRL_RW,
		                            e_maskreg_p11_enc_cntrl_ctl_code,
		                            reg_val);
		RETIF_REG_FAIL(err)
	} else {
		/* Unmute the audio FIFO */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_AIP_CNTRL_0_RW,
		                            e_maskreg_p11_aip_cntrl_0_rst_fifo,
		                            0);
		RETIF_REG_FAIL(err)

		/* Set HDMI HDCP mode on for HDMI */
		/* Also sets E_MASKREG_P11_ENC_CNTRL_ctl_code */
		err = set_hw_register_field_table(p_dis, &k_vout_hdcp_on[0]);
		RETIF_REG_FAIL(err)
	}

	/* For each parameter that is not No Change, set its register */
	if(vout_mode != HDMITX_VOUTMODE_NO_CHANGE) {
		/* Save the output mode for later use by the matrix & downsampler */
		p_dis->vout_mode = vout_mode;
	}
	if(pre_filter < HDMITX_VOUT_PREFIL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_0_W,
		                            e_maskreg_p00_hvf_cntrl_0_prefil,
		                            (u8)pre_filter);
		RETIF_REG_FAIL(err)
	}
	if(yuv_blank < HDMITX_VOUT_YUV_BLNK_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_1_W,
		                            e_maskreg_p00_hvf_cntrl_1_yuvblk,
		                            (u8)yuv_blank);
		RETIF_REG_FAIL(err)
	}

	return 0;
}

/*============================================================================*/
/* bslVideoOutSetSync                                                 */
/*============================================================================*/
error_code_t
bsl_video_out_set_sync
(
    unit_select_t          tx_unit,
    bsl_vs_src_t      src_h,
    bsl_vs_src_t      src_v,
    bsl_vs_src_t      src_x,
    bsl_vs_tgl_t      toggle,
    bsl_vs_once_t     once
)
{
	hdmi_txobject_t *p_dis;       /* Pointer to Device Instance Structure */
	error_code_t     err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
	RETIF_BADPARAM(src_h   >= HDMITX_VSSRC_INVALID)
	RETIF_BADPARAM(src_v   >= HDMITX_VSSRC_INVALID)
	RETIF_BADPARAM(src_x   >= HDMITX_VSSRC_INVALID)
	RETIF_BADPARAM(toggle >= HDMITX_VSTGL_INVALID)
	RETIF_BADPARAM(once   >= HDMITX_VSONCE_INVALID)

	/* For each parameter that is not No Change, set its register */
	if(src_h != HDMITX_VSSRC_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_1_W,
		                            e_maskreg_p00_tbg_cntrl_1_vhx_ext_hs,
		                            (u8)src_h);
		RETIF_REG_FAIL(err)
	}
	if(src_v != HDMITX_VSSRC_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_1_W,
		                            e_maskreg_p00_tbg_cntrl_1_vhx_ext_vs,
		                            (u8)src_v);
		RETIF_REG_FAIL(err)
	}
	if(src_x != HDMITX_VSSRC_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_1_W,
		                            e_maskreg_p00_tbg_cntrl_1_vhx_ext_de,
		                            (u8)src_x);
		RETIF_REG_FAIL(err)
	}
	if(toggle != HDMITX_VSTGL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_1_W,
		                            e_maskreg_p00_tbg_cntrl_1_vh_tgl,
		                            (u8)toggle);
		RETIF_REG_FAIL(err)
	}
	if(once != HDMITX_VSONCE_NO_CHANGE) {
		/* Must be last register set */
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_0_W,
		                            e_maskreg_p00_tbg_cntrl_0_sync_once,
		                            (u8)once);
		RETIF_REG_FAIL(err)
	}

	/* Toggle TMDS serialiser force flags - stability fix */
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_BUFFER_OUT_RW,
	                            e_maskreg_p02_buffer_out_srl_force,
	                            (u8)HDMITX_TMDSOUT_FORCED0);
	RETIF_REG_FAIL(err)
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_BUFFER_OUT_RW,
	                            e_maskreg_p02_buffer_out_srl_force,
	                            (u8)HDMITX_TMDSOUT_NORMAL);
	RETIF_REG_FAIL(err)

	if(once == HDMITX_VSONCE_ONCE) {
		/* Toggle output Sync Once flag for settings to take effect */
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_0_W,
		                            e_maskreg_p00_tbg_cntrl_0_sync_once,
		                            (u8)HDMITX_VSONCE_EACH_FRAME);
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_TBG_CNTRL_0_W,
		                            e_maskreg_p00_tbg_cntrl_0_sync_once,
		                            (u8)HDMITX_VSONCE_ONCE);
		RETIF_REG_FAIL(err)
	}
	return 0;
}

/*============================================================================*/
/* bslVideoSetInOut                                                   */
/*============================================================================*/
error_code_t
bsl_video_set_in_out
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vin_fmt,
    bsl_sca_mode_t    sca_mode_request,
    bsl_vid_fmt_t     vout_fmt,
    u8                   u_pixel_repeat,
    bsl_mat_mode_t    mat_mode,
    bsl_vout_dbits_t  datapath_bits,
    bsl_vqr_t        dvi_vqr
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t        *p_dis;
	error_code_t           err;         /* Error code */
	bsl_vid_fmt_t     sca_in_fmt;    /* Scaler input format */
	bsl_vid_fmt_t     sca_out_fmt;   /* Scaler output format */
	bsl_sca_mode_t    sca_mode;     /* Scaler mode */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check parameters */
#ifdef FORMAT_PC
	RETIF_BADPARAM(vin_fmt        < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vout_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM((vin_fmt  > HDMITX_VFMT_TV_MAX) && (vin_fmt < HDMITX_VFMT_PC_MIN))
	RETIF_BADPARAM((vout_fmt > HDMITX_VFMT_TV_MAX) && (vout_fmt < HDMITX_VFMT_PC_MIN))
	RETIF_BADPARAM(vin_fmt        > HDMITX_VFMT_PC_MAX)
	RETIF_BADPARAM(vout_fmt       > HDMITX_VFMT_PC_MAX)
#else /* FORMAT_PC */
	/*FORMAT TV only*/
	RETIF_BADPARAM(vin_fmt        < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vout_fmt       < HDMITX_VFMT_TV_MIN)
	RETIF_BADPARAM(vin_fmt        > HDMITX_VFMT_TV_MAX)
	RETIF_BADPARAM(vout_fmt       > HDMITX_VFMT_TV_MAX)
#endif /* FORMAT_PC */

	RETIF_BADPARAM(sca_mode_request >= HDMITX_SCAMODE_INVALID)
	RETIF_BADPARAM(u_pixel_repeat   >= HDMITX_PIXREP_INVALID)
	RETIF_BADPARAM(mat_mode        >= HDMITX_MATMODE_INVALID)
	RETIF_BADPARAM(datapath_bits   >= HDMITX_VOUT_DBITS_INVALID)

	sca_mode = sca_mode_request;

#ifdef TMFL_TDA9981_SUPPORT
	sca_mode = HDMITX_SCAMODE_OFF;
#else /* TMFL_TDA9981_SUPPORT */
	if(vout_fmt >= HDMITX_VFMT_TV_NO_REG_MIN) {
		/* Disable Scaler mode for PC_FORMAT and for 24/25/30Hz formats */
		sca_mode = HDMITX_SCAMODE_OFF;
	}
#endif /* TMFL_TDA9981_SUPPORT */

	/* Get current input format if it must not change */
	if(vin_fmt == HDMITX_VFMT_NO_CHANGE) {
		RETIF(p_dis->vin_fmt == HDMITX_VFMT_NULL,
		      ERR_HDMI_INCONSISTENT_PARAMS)
		vin_fmt = p_dis->vin_fmt;
	} else {
		p_dis->vin_fmt = vin_fmt;
	}

	/* Get current output format if it must not change */
	if(vout_fmt == HDMITX_VFMT_NO_CHANGE) {
		RETIF(p_dis->vout_fmt == HDMITX_VFMT_NULL,
		      ERR_HDMI_INCONSISTENT_PARAMS)
		vout_fmt = p_dis->vout_fmt;
	} else {
		p_dis->vout_fmt = vout_fmt;
	}
	/* Force RGB mode for VGA output format */
	if(vout_fmt == HDMITX_VFMT_TV_MIN) {
		p_dis->vout_mode = HDMITX_VOUTMODE_RGB444;
	}

	if(p_dis->vout_mode == HDMITX_VOUTMODE_RGB444) {
		if((p_dis->vout_fmt >= hdmitx_vfmt_02_720x480p_60hz) && (p_dis->vout_fmt <= HDMITX_VFMT_TV_MAX)) {
			err = set_hw_register_field(p_dis,
			                            E_REG_P00_HVF_CNTRL_1_W,
			                            e_maskreg_p00_hvf_cntrl_1_vqr,
			                            (u8) HDMITX_VOUT_QRANGE_RGB_YUV);
			RETIF_REG_FAIL(err)
		} else { /*Format PC or VGA*/
			err = set_hw_register_field(p_dis,
			                            E_REG_P00_HVF_CNTRL_1_W,
			                            e_maskreg_p00_hvf_cntrl_1_vqr,
			                            (u8) HDMITX_VOUT_QRANGE_FS);
			RETIF_REG_FAIL(err)
		}
	} else {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_1_W,
		                            e_maskreg_p00_hvf_cntrl_1_vqr,
		                            (u8) HDMITX_VOUT_QRANGE_YUV);
		RETIF_REG_FAIL(err)
	}

	/* If scaler mode is auto then set mode based on input and output format */
	if(sca_mode != HDMITX_SCAMODE_NO_CHANGE) {
		if(sca_mode == HDMITX_SCAMODE_AUTO) {
			/* If both formats map to the same device output format then
			 * the scaler can be switched off */
			if(k_vfmt_to_regvfmt_tv[vout_fmt] == k_vfmt_to_regvfmt_tv[vin_fmt]) {
				sca_mode = HDMITX_SCAMODE_OFF;
			} else {
				sca_mode = HDMITX_SCAMODE_ON;
			}
		}
		sca_in_fmt  = vin_fmt;
		if(sca_mode == HDMITX_SCAMODE_ON) {
			sca_out_fmt = vout_fmt;
		} else {
			sca_out_fmt = vin_fmt;
		}
		err = set_scaler_format(p_dis, sca_in_fmt, sca_out_fmt, u_pixel_repeat);
		RETIF(err != 0, err)
	} else {
		/* Set pixel repetition - sets pixelRepeatCount, used by setScalerFormat */
		err = set_pixel_repeat(p_dis, vout_fmt, u_pixel_repeat);
		RETIF(err != 0, err)
	}

	/* Set VS and optional DE */
	err = set_de_vs(p_dis, vout_fmt);
	RETIF(err != 0, err)

	/* If matrix mode is auto then set mode based on input and output format */
	if(mat_mode != HDMITX_MATMODE_NO_CHANGE) {
		if(mat_mode == HDMITX_MATMODE_AUTO) {
			err = bsl_matrix_set_conversion(tx_unit, vin_fmt,
			                                p_dis->vin_mode, vout_fmt, p_dis->vout_mode, p_dis->dvi_vqr);
		} else {
			err = bsl_matrix_set_mode(tx_unit, HDMITX_MCNTRL_OFF,
			                          HDMITX_MSCALE_NO_CHANGE);
		}
		RETIF(err != 0, err)
	}

	/* Set upsampler and downsampler */
	err = set_sampling(p_dis);
	RETIF(err != 0, err)

	/* Set colour component bit depth */
	if(datapath_bits != HDMITX_VOUT_DBITS_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_1_W,
		                            e_maskreg_p00_hvf_cntrl_1_pad,
		                            (u8)datapath_bits);
		RETIF_REG_FAIL(err)
	}

	/* Save kBypassColourProc registers before pattern goes on */
	get_hw_register(p_dis, E_REG_P00_MAT_CONTRL_W,  &g_mat_contrl[tx_unit]);
	get_hw_register(p_dis, E_REG_P00_HVF_CNTRL_0_W, &g_hvf_cntrl0[tx_unit]);
	get_hw_register(p_dis, E_REG_P00_HVF_CNTRL_1_W, &g_hvf_cntrl1[tx_unit]);

	set_state(p_dis, EV_SETINOUT);
	return 0;
}

/*============================================================================*/
/*                   STATIC FUNCTION DEFINTIONS                               */
/*============================================================================*/

/*============================================================================*/
/* setDeVs                                                                    */
/*============================================================================*/
static error_code_t
set_de_vs
(
    hdmi_txobject_t    *p_dis,
    bsl_vid_fmt_t  vout_fmt
)
{
	error_code_t   err;        /* Error code */
	u16          vs_pix_strt2; /* VS pixel number for start pulse in field 2 */
	u8           reg_vfmt;    /* Video format value used for register */
	bsl_vid_fmt_t  vout_fmt_index; /* Index in tab kVfmtToRegvfmt_TV*/

	/* Parameters already checked by caller */

	/* IF voutFmt = No Change THEN return 0 */
	RETIF(vout_fmt == HDMITX_VFMT_NO_CHANGE, 0)

	vout_fmt_index = vout_fmt;
#ifdef FORMAT_PC
	if(vout_fmt_index > HDMITX_VFMT_PC_MIN) {
		vout_fmt_index = (bsl_vid_fmt_t)(vout_fmt_index - HDMITX_VFMT_PC_MIN + HDMITX_VFMT_TV_MAX + 1);
	}
#endif /* FORMAT_PC */

	/* Quit if the output format does not map to the register format */
	reg_vfmt = k_vfmt_to_regvfmt_tv[vout_fmt_index];
	RETIF_BADPARAM(reg_vfmt == E_REGVFMT_INVALID)

	/* DE_START & DE_STOP no longer set because N2 device no longer supported */

	/* Adjust VS_PIX_STRT_2 and VS_PIX_END_2 for interlaced output formats */
	vs_pix_strt2 = k_regvfmt_to_vs2[reg_vfmt];
	err = set_hw_register_msb_lsb(p_dis, E_REG_P00_VS_PIX_STRT_2_MSB_W, vs_pix_strt2);
	RETIF_REG_FAIL(err)
	err = set_hw_register_msb_lsb(p_dis, E_REG_P00_VS_PIX_END_2_MSB_W, vs_pix_strt2);

	return err;
}

/*============================================================================*/
/* setPixelRepeat                                                             */
/*============================================================================*/
static error_code_t
set_pixel_repeat
(
    hdmi_txobject_t    *p_dis,
    bsl_vid_fmt_t vout_fmt,
    u8               u_pixel_repeat
)
{
	error_code_t err = 0;  /* Error code */
	u8         reg_vfmt;      /* Video format value used for register */
	u8         ssd;          /* Packed srl, scg and de */
	bsl_vid_fmt_t  vout_fmt_index; /* Index in tab kVfmtToRegvfmt_TV*/
	/* Pointer on a tab of kRegvfmtToPllSsd*/
	u8         *tab_setting_pll_ssd = NULL;
	/* true if TV_FORMAT with internal chip confiration  found (default) */
	bool          b_is_tv_format;

	b_is_tv_format = true;

	/* IF voutFmt = No Change THEN return 0 */
	RETIF(vout_fmt == HDMITX_VFMT_NO_CHANGE, 0)

	vout_fmt_index = vout_fmt;
#ifdef FORMAT_PC
	if(vout_fmt_index > HDMITX_VFMT_PC_MIN) {
		vout_fmt_index = (bsl_vid_fmt_t)(vout_fmt_index - HDMITX_VFMT_PC_MIN + HDMITX_VFMT_TV_MAX + 1);
		b_is_tv_format = false; /* PC_FORMAT found */
	}
#endif /* FORMAT_PC */

	/* Quit if the output format does not map to the register format */
	reg_vfmt = k_vfmt_to_regvfmt_tv[vout_fmt_index];
	RETIF_BADPARAM(reg_vfmt == E_REGVFMT_INVALID)

#ifdef FORMAT_PC
	if(b_is_tv_format) {
#endif /* FORMAT_PC */
		switch(p_dis->sca_mode) {
		case HDMITX_SCAMODE_OFF:

			switch(p_dis->vin_mode) {
			case HDMITX_VINMODE_RGB444:
			case HDMITX_VINMODE_YUV444:
			case HDMITX_VINMODE_YUV422:

				switch(p_dis->pix_rate) {
				case HDMITX_PIXRATE_SINGLE:
				case HDMITX_PIXRATE_SINGLE_REPEATED:
					tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_off_ccir_off_dbl_edge_off;
					break;

				case HDMITX_PIXRATE_DOUBLE:
					return ERR_HDMI_INCONSISTENT_PARAMS;
					break;

				default:
					return ERR_HDMI_BAD_PARAMETER;
				}

				break;

			case HDMITX_VINMODE_CCIR656:

				switch(p_dis->pix_rate) {
				case HDMITX_PIXRATE_SINGLE:
				case HDMITX_PIXRATE_SINGLE_REPEATED:
					tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_off_ccir_on_dbl_edge_off;
					break;

				case HDMITX_PIXRATE_DOUBLE:
					tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_off_ccir_on_dbl_edge_on;
					break;

				default:
					return ERR_HDMI_BAD_PARAMETER;
				}

				break;

			default:
				return ERR_HDMI_BAD_PARAMETER;
			}

			break;
		case HDMITX_SCAMODE_ON:

			switch(p_dis->vin_mode) {
			case HDMITX_VINMODE_RGB444:
			case HDMITX_VINMODE_YUV444:
			case HDMITX_VINMODE_YUV422:

				switch(p_dis->pix_rate) {
				case HDMITX_PIXRATE_SINGLE:
				case HDMITX_PIXRATE_SINGLE_REPEATED:
					switch(p_dis->vin_fmt) {
					case hdmitx_vfmt_06_720x480i_60hz:
					case hdmitx_vfmt_07_720x480i_60hz:
					case hdmitx_vfmt_10_720x480i_60hz:
					case hdmitx_vfmt_11_720x480i_60hz:
					case hdmitx_vfmt_21_720x576i_50hz:
					case hdmitx_vfmt_22_720x576i_50hz:
					case hdmitx_vfmt_25_720x576i_50hz:
					case hdmitx_vfmt_26_720x576i_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_off_dbl_edge_off_interlace;
						break;

					case hdmitx_vfmt_02_720x480p_60hz:
					case hdmitx_vfmt_03_720x480p_60hz:
					case hdmitx_vfmt_17_720x576p_50hz:
					case hdmitx_vfmt_18_720x576p_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_off_dbl_edge_off_progressif;
						break;
					default:
						return ERR_HDMI_BAD_PARAMETER;
					}

					break;
				case HDMITX_PIXRATE_DOUBLE:
					break;
				default:
					return ERR_HDMI_BAD_PARAMETER;
				}

				break;

			case HDMITX_VINMODE_CCIR656:

				switch(p_dis->pix_rate) {
				case HDMITX_PIXRATE_SINGLE:
				case HDMITX_PIXRATE_SINGLE_REPEATED:
					switch(p_dis->vin_fmt) {
					case hdmitx_vfmt_06_720x480i_60hz:
					case hdmitx_vfmt_07_720x480i_60hz:
					case hdmitx_vfmt_10_720x480i_60hz:
					case hdmitx_vfmt_11_720x480i_60hz:
					case hdmitx_vfmt_21_720x576i_50hz:
					case hdmitx_vfmt_22_720x576i_50hz:
					case hdmitx_vfmt_25_720x576i_50hz:
					case hdmitx_vfmt_26_720x576i_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_on_dbl_edge_off_interlace;
						break;

					case hdmitx_vfmt_02_720x480p_60hz:
					case hdmitx_vfmt_03_720x480p_60hz:
					case hdmitx_vfmt_17_720x576p_50hz:
					case hdmitx_vfmt_18_720x576p_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_on_dbl_edge_off_progressif;
						break;

					default:
						return ERR_HDMI_BAD_PARAMETER;
					}

					break;

				case HDMITX_PIXRATE_DOUBLE:

					switch(p_dis->vin_fmt) {
					case hdmitx_vfmt_06_720x480i_60hz:
					case hdmitx_vfmt_07_720x480i_60hz:
					case hdmitx_vfmt_10_720x480i_60hz:
					case hdmitx_vfmt_11_720x480i_60hz:
					case hdmitx_vfmt_21_720x576i_50hz:
					case hdmitx_vfmt_22_720x576i_50hz:
					case hdmitx_vfmt_25_720x576i_50hz:
					case hdmitx_vfmt_26_720x576i_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_on_dbl_edge_on_interlace;
						break;

					case hdmitx_vfmt_02_720x480p_60hz:
					case hdmitx_vfmt_03_720x480p_60hz:
					case hdmitx_vfmt_17_720x576p_50hz:
					case hdmitx_vfmt_18_720x576p_50hz:
						tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.sca_on_ccir_on_dbl_edge_on_progressif;
						break;

					default:
						return ERR_HDMI_BAD_PARAMETER;
					}

					break;

				default:
					return ERR_HDMI_BAD_PARAMETER;
				}

				break;

			default:
				return ERR_HDMI_BAD_PARAMETER;
			}

			break;

		default:
			return ERR_HDMI_BAD_PARAMETER;
		}

		/* Check parameters */
		RETIF_BADPARAM(reg_vfmt >= E_REGVFMT_NUM_TV)

		/* Table pointer must be valid here */
		if(tab_setting_pll_ssd) {
			/* Set PLLs based on output format */
			ssd = tab_setting_pll_ssd[reg_vfmt];
		} else {
			return ERR_HDMI_ASSERTION;
		}

#ifdef FORMAT_PC
	} else {
		tab_setting_pll_ssd = (u8 *)k_regvfmt_to_pll_ssd.settings_format_pc;

		/* Check parameters */
		RETIF_BADPARAM(reg_vfmt < E_REGVFMT_NUM_TV)

		ssd = tab_setting_pll_ssd[reg_vfmt - E_REGVFMT_NUM_TV];
	}

#endif /* FORMAT_PC */

	if(ssd < SSD_UNUSED_VALUE) {

		err = set_hw_register_field(p_dis, E_REG_P02_PLL_SERIAL_2_RW,
		                            e_maskreg_p02_pll_serial_2_srl_nosc,
		                            SSD2SRL(ssd));
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis, E_REG_P02_PLL_SCG2_RW,
		                            e_maskreg_p02_pll_scg2_scg_nosc,
		                            SSD2SCG(ssd));
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis, E_REG_P02_PLL_DE_RW,
		                            e_maskreg_p02_pll_de_pllde_nosc,
		                            SSD2DE(ssd));
	}

	/* Set pixel repetition */
	if(u_pixel_repeat != HDMITX_PIXREP_NO_CHANGE) {
		if(u_pixel_repeat == HDMITX_PIXREP_DEFAULT) {
			/* Look up default pixel repeat value for this output format */
			u_pixel_repeat = k_regvfmt_to_pix_rep[reg_vfmt];
		}

		/* Update current pixel repetition count */
		p_dis->pixel_repeat_count = u_pixel_repeat;

		err = set_hw_register_field(p_dis,
		                            E_REG_P02_PLL_SERIAL_2_RW,
		                            e_maskreg_p02_pll_serial_2_srl_pr,
		                            u_pixel_repeat);
		RETIF_REG_FAIL(err)
		/* Set pixel repetition count for Repetitor module */
		err = set_hw_register(p_dis, E_REG_P00_RPT_CNTRL_W, u_pixel_repeat);
	}
	return err;
}

/*============================================================================*/
/* setSampling                                                                */
/*============================================================================*/
static error_code_t
set_sampling
(
    hdmi_txobject_t   *p_dis
)
{
	error_code_t err;          /* Error code */
	u8         up_sample;     /* 1 if upsampler must be enabled */
	u8         down_sample;   /* 1 if downsampler must be enabled */
	u8         matrix_bypass; /*>0 if matrix has been bypassed */

	if((p_dis->vin_mode == HDMITX_VINMODE_YUV422)
	   || (p_dis->vin_mode == HDMITX_VINMODE_CCIR656)) {
		if(p_dis->vout_mode == HDMITX_VOUTMODE_YUV422) {
			/* Input 422/656, output 422 */
			err = get_hw_register(p_dis, E_REG_P00_MAT_CONTRL_W, &matrix_bypass);
			RETIF_REG_FAIL(err)
			matrix_bypass &= e_maskreg_p00_mat_contrl_mat_bp;
			/* Has matrix been bypassed? */
			if(matrix_bypass > 0) {
				up_sample = 0;
				down_sample = 0;
			} else {
				up_sample = 1;
				down_sample = 1;
			}
		} else {
			/* Input 422/656, output not 422 */
			up_sample = 1;
			down_sample = 0;
		}
	} else {
		if(p_dis->vout_mode == HDMITX_VOUTMODE_YUV422) {
			/* Input not 422/656, output 422 */
			up_sample = 0;
			down_sample = 1;
		} else {
			/* Input not 422/656, output not 422 */
			up_sample = 0;
			down_sample = 0;
		}
	}

	/* Check upsample mode saved by bslVideoInSetConfig */
	if(p_dis->upsample_mode != HDMITX_UPSAMPLE_AUTO) {
		/* Saved upsample mode overrides local one */
		up_sample = p_dis->upsample_mode;
	}

	/* Set upsampler */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_HVF_CNTRL_0_W,
	                            e_maskreg_p00_hvf_cntrl_0_intpol,
	                            up_sample);
	RETIF_REG_FAIL(err)

	/* Set downsampler */
	err = set_hw_register_field(p_dis,
	                            E_REG_P00_HVF_CNTRL_1_W,
	                            e_maskreg_p00_hvf_cntrl_1_for,
	                            down_sample);
	return err;
}

/*============================================================================*/
/* setScalerFormat                                                            */
/*============================================================================*/
static error_code_t
set_scaler_format
(
    hdmi_txobject_t   *p_dis,
    bsl_vid_fmt_t vin_fmt,
    bsl_vid_fmt_t vout_fmt,
    u8 pixel_repeat
)
{
	error_code_t err;          /* Error code */
	u8         reg_vout_fmt;   /* Video o/p format value used for register */
	u8         reg_vin_fmt;    /* Video i/p format value used for comparison */
	u8         reg_val;       /* Register value */
#ifndef TMFL_TDA9981_SUPPORT
	u8         sc_in_fmt = 0;  /* Scaler input format */
	u8         sc_out_fmt = 0; /* Scaler output format */
	u16        pll_scg_n;      /* PLL scaling values */
	u16        pll_scg_r;
	u8		  hbl_off;		/* FB: define hbl offset*/

#endif /* TMFL_TDA9981_SUPPORT */

#ifdef FORMAT_PC
	RETIF_BADPARAM(vin_fmt        > HDMITX_VFMT_PC_MAX)
	RETIF_BADPARAM(vout_fmt        > HDMITX_VFMT_PC_MAX)
#else
	RETIF_BADPARAM(vin_fmt        > HDMITX_VFMT_TV_MAX)
	RETIF_BADPARAM(vout_fmt        > HDMITX_VFMT_TV_MAX)
#endif

	/* Look up the VIDFORMAT register formats from the register format table */
	reg_vin_fmt = k_vfmt_to_regvfmt_tv[vin_fmt];
	reg_vout_fmt = k_vfmt_to_regvfmt_tv[vout_fmt];
	/* Quit if the output format does not map to the register format */
	RETIF_BADPARAM(reg_vout_fmt == E_REGVFMT_INVALID)

	if((vin_fmt > HDMITX_VFMT_TV_MAX)
	   || (vout_fmt > HDMITX_VFMT_TV_MAX)
	   || (reg_vin_fmt == reg_vout_fmt)) {
		/* Disable scaler for PC formats or if same input and output formats */
		err = set_hw_register_field_table(p_dis, &k_scaler_off[0]);
		RETIF_REG_FAIL(err)

#ifndef TMFL_TDA9981_SUPPORT
		if(p_dis->sca_mode == HDMITX_SCAMODE_ON) {
			/* remove blanking */
			err = set_hw_register(p_dis, E_REG_P00_HBL_OFFSET_START_W, 0);
			RETIF_REG_FAIL(err)
		}
#endif	/* TMFL_TDA9981_SUPPORT */

		p_dis->sca_mode = HDMITX_SCAMODE_OFF;
	} else {
#ifdef TMFL_TDA9981_SUPPORT
		return ERR_HDMI_NOT_SUPPORTED;
#else
		/* Need to scale: quit if no scaler */
		RETIF(p_dis->u_device_features & e_maskreg_p00_version_not_s,
		      ERR_HDMI_NOT_SUPPORTED)

		/* Look up scaler register formats from table */
		sc_in_fmt = SCIO2SCIN(k_vfmt_to_regvfmt_scio_tv[vin_fmt]);
		sc_out_fmt = SCIO2SCOUT(k_vfmt_to_regvfmt_scio_tv[vout_fmt]);

		/* Do these formats individually support scaling? */
		RETIF(sc_in_fmt  == E_REGVFMT_SCIN_INVALID,
		      ERR_HDMI_INCONSISTENT_PARAMS)
		RETIF(sc_out_fmt == E_REGVFMT_SCOUT_INVALID,
		      ERR_HDMI_INCONSISTENT_PARAMS)

		/* Can the i/p register format be scaled to the o/p register format? */
		RETIF(CAN_FMTS_SCALE(reg_vin_fmt, reg_vout_fmt) == 0,
		      ERR_HDMI_INCONSISTENT_PARAMS)

		/* Scaling is possible: Enable scaler */
		err = set_hw_register_field_table(p_dis, &k_scaler_on[0]);
		RETIF_REG_FAIL(err)

		/* Set scaler input format */
		err = set_hw_register(p_dis, E_REG_P01_VIDFORMAT_W, sc_in_fmt);
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_SC_VIDFORMAT_W,
		                            e_maskreg_p01_sc_vidformat_vid_format_i,
		                            sc_in_fmt);
		RETIF_REG_FAIL(err)

		/* Set scaler output format */
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_SC_VIDFORMAT_W,
		                            e_maskreg_p01_sc_vidformat_vid_format_o,
		                            sc_out_fmt);
		RETIF_REG_FAIL(err)

		p_dis->sca_mode = HDMITX_SCAMODE_ON;

		/* Look up PLL scaling */
		pll_scg_n = k_scl_mode_to_scg_n[sc_in_fmt][sc_out_fmt];

		/* Set bits [10 to 8] */
		reg_val = (u8)(pll_scg_n >> 8) & 7;
		err = set_hw_register(p_dis, E_REG_P02_PLL_SCGN2_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set bits [7 to 0] */
		reg_val = (u8)pll_scg_n;
		err = set_hw_register(p_dis, E_REG_P02_PLL_SCGN1_RW, reg_val);
		RETIF_REG_FAIL(err)

		/* Look up PLL scaling */
		pll_scg_r = k_scl_mode_to_scg_r[sc_in_fmt][sc_out_fmt];

		/* Set bit [8] */
		reg_val = (u8)(pll_scg_r >> 8) & 1;
		err = set_hw_register(p_dis, E_REG_P02_PLL_SCGR2_RW, reg_val);
		RETIF_REG_FAIL(err)
		/* Set bits [7 to 0] */
		reg_val = (u8)pll_scg_r;
		err = set_hw_register(p_dis, E_REG_P02_PLL_SCGR1_RW, reg_val);
		RETIF_REG_FAIL(err)

		/* Look-up blanking insertion */
		hbl_off = k_scl_add_blk_pix[sc_in_fmt][sc_out_fmt];
		err = set_hw_register(p_dis, E_REG_P00_HBL_OFFSET_START_W, hbl_off);
		RETIF_REG_FAIL(err)
		/* clean green lines for none RGB output */
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_HVF_CNTRL_1_W,
		                            e_maskreg_p00_hvf_cntrl_1_yuvblk,
		                            k_scl_clear_blk_pix[p_dis->vout_mode]);
		RETIF_REG_FAIL(err)
#endif /* TMFL_TDA9981_SUPPORT */
	}

	/* PR 207  call function setPixelRepeat before wite in E_REG_P00_VIDFORMAT_W*/
	/* Set pixel repetition - sets pixelRepeatCount, used by setScalerFormat */
	err = set_pixel_repeat(p_dis, p_dis->vout_fmt, pixel_repeat);
	RETIF(err != 0, err)

	/* Set scaler clock */
	reg_val = 0;
	if((p_dis->pixel_repeat_count > HDMITX_PIXREP_MIN) &&
	   (p_dis->pixel_repeat_count <= HDMITX_PIXREP_MAX)) {
		reg_val = 2;
	} else if(p_dis->vin_mode == HDMITX_VINMODE_CCIR656) {
		reg_val = (u8)((p_dis->sca_mode == HDMITX_SCAMODE_ON) ? 0 : 1);
	}
	err = set_hw_register_field(p_dis,
	                            E_REG_P02_SEL_CLK_RW,
	                            e_maskreg_p02_sel_clk_sel_vrf_clk,
	                            reg_val);
	RETIF_REG_FAIL(err)

	/* Set format register for the selected output format voutFmt */
	/* MUST BE AFTER SCALER CLOCK or sometimes the 9983 won't take the value */
	err = set_hw_register(p_dis, E_REG_P00_VIDFORMAT_W, reg_vout_fmt);
	RETIF_REG_FAIL(err)

	/** \todo Evaluation of customer fix for 576i -> 720 p issue *************/

#ifndef TMFL_TDA9981_SUPPORT
	if(p_dis->sca_mode == HDMITX_SCAMODE_ON) {
		/* Set scaler input format again */
		/* MUST BE AFTER SCALER CLOCK or sometimes the 9983 won't take value */
		err = set_hw_register(p_dis, E_REG_P01_VIDFORMAT_W, sc_in_fmt);
		RETIF_REG_FAIL(err)
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_SC_VIDFORMAT_W,
		                            e_maskreg_p01_sc_vidformat_vid_format_i,
		                            sc_in_fmt);
		RETIF_REG_FAIL(err)

		/* Set scaler output format again */
		/* MUST BE AFTER SCALER CLOCK or sometimes the 9983 won't take value */
		err = set_hw_register_field(p_dis,
		                            E_REG_P01_SC_VIDFORMAT_W,
		                            e_maskreg_p01_sc_vidformat_vid_format_o,
		                            sc_out_fmt);
		RETIF_REG_FAIL(err)
	}
#endif /* TMFL_TDA9981_SUPPORT */
	return err;
}

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/
