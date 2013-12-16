/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_cfg.c
 *
 * \version       Revision: 1
 *
 * \date          Date: 08/08/07 11:00
 *
 * \brief         devlib driver component API for the TDA998x HDMI Transmitters
 *
 * \section refs  Reference Documents
 * HDMI Tx Driver - FRS.doc,
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   History:       tx_cfg.c
 *
 * *****************  Version 1  *****************
 * User: J. Lamotte Date: 08/08/07  Time: 11:00
 * initial version
 *
 *
 *   \endverbatim
 *
 * */

/*============================================================================*/
/*                             INCLUDE FILES                                  */
/*============================================================================*/
#include "tmdlHdmiTx_IW.h"
#include "tmNxTypes.h"
#include "tmdlHdmiTx.h"
#include "tmdlHdmiTx_cfg.h"

#ifdef TMFL_OS_WINDOWS      /* OS Windows */
#include "infra_i2c.h"
#else                       /* OS ARM7 */
#include "I2C.h"
#include <LPC21xx.H>
#endif                      /* endif TMFL_OS_WINDOWS */

#include "tmbslHdmiTx.h"

/******************************************************************************
 ******************************************************************************
 *                 THIS PART CAN BE MODIFIED BY CUSTOMER                      *
 ******************************************************************************
 *****************************************************************************/

/*============================================================================*/
/*                          INTERNAL PROTOTYPE                                */
/*============================================================================*/
#ifdef TMFL_OS_WINDOWS  /* OS Windows */
error_code_t windows_i2c_read_function(bsl_sys_args_t *p_sys_args);
error_code_t windows_i2c_write_function(bsl_sys_args_t *p_sys_args);
#else                   /* OS ARM7 */
error_code_t rtx_tx_i2c_read_function(bsl_sys_args_t *p_sys_args);
error_code_t rtx_tx_i2c_write_function(bsl_sys_args_t *p_sys_args);
#endif                  /* endif TMFL_OS_WINDOWS */

error_code_t edid_read_function(bsl_sys_args_edid_t *p_sys_args);

/*============================================================================*/
/*                                MACRO                                       */
/*============================================================================*/

/* macro for quick error handling */
#define RETIF(cond, rslt) if ((cond)){return (rslt);}

/*============================================================================*/
/*                          TYPES DECLARATIONS                                */
/*============================================================================*/

/*============================================================================*/
/*                       CONSTANTS DECLARATIONS                               */
/*============================================================================*/

/* Configuration for unit 0 *************************** */
/* priority of the command tasks */
#ifdef TMFL_OS_WINDOWS          /* OS Windows */
#define COMMAND_TASK_PRIORITY_0  THREAD_PRIORITY_HIGHEST
#else                           /* OS ARM7 */
#define COMMAND_TASK_PRIORITY_0  250
#endif                          /* endif TMFL_OS_WINDOWS */
/* stack size of the command tasks */
#define COMMAND_TASK_STACKSIZE_0 128
/* size of the message queues for command tasks */
#define COMMAND_TASK_QUEUESIZE_0 128

/* priority of the hdcp check tasks */
#ifdef TMFL_OS_WINDOWS          /* OS Windows */
#define HDCP_CHECK_TASK_PRIORITY_0  THREAD_PRIORITY_HIGHEST
#else                           /* OS ARM7 */
#define HDCP_CHECK_TASK_PRIORITY_0  250
#endif                          /* endif TMFL_OS_WINDOWS */

/* stack size of the hdcp check tasks */
#define HDCP_CHECK_TASK_STACKSIZE_0 128
/* I2C adress of the unit */
#define UNIT_I2C_ADDRESS_0 0x70

/* Configuration for unit 1 *************************** */
/* priority of the command tasks */
#ifdef TMFL_OS_WINDOWS          /* OS Windows */
#define COMMAND_TASK_PRIORITY_1  THREAD_PRIORITY_HIGHEST
#else                           /* OS ARM7 */
#define COMMAND_TASK_PRIORITY_1  250
#endif                          /* endif TMFL_OS_WINDOWS */
/* stack size of the command tasks */
#define COMMAND_TASK_STACKSIZE_1 128
/* size of the message queues for command tasks */
#define COMMAND_TASK_QUEUESIZE_1 128

/* priority of the hdcp check tasks */
#ifdef TMFL_OS_WINDOWS          /* OS Windows */
#define HDCP_CHECK_TASK_PRIORITY_1  THREAD_PRIORITY_HIGHEST
#else                           /* OS ARM7 */
#define HDCP_CHECK_TASK_PRIORITY_1  250
#endif                          /* endif TMFL_OS_WINDOWS */

/* stack size of the hdcp check tasks */
#define HDCP_CHECK_TASK_STACKSIZE_1 128
/* I2C adress of the unit */
#define UNIT_I2C_ADDRESS_1 0x71

/* Resolution supported */
#ifndef FORMAT_PC
#define RESOLUTION_NB   34
#else
#define RESOLUTION_NB   61
#endif /* FORMAT_PC */

/* HDCP key */
#define KEY_SEED    0x1234

/* Audio port configuration, bitn = 1 to enable port n, = 0 to disable port n */
#define ENABLE_ALL_AUDIO_PORT       0xFF

/* Audio clock port configuration */
#define ENABLE_AUDIO_CLOCK_PORT     true
#define DISABLE_AUDIO_CLOCK_PORT    false

/* Audio port configuration, bitn = 1 to pulldown port n, = 0 to pulldown port n */
#define DISABLE_ALL_AUDIO_PORT_PULLDOWN 0x00

/* Audio clock port pulldown configuration */
#define ENABLE_AUDIO_CLOCK_PORT_PULLDOWN    true
#define DISABLE_AUDIO_CLOCK_PORT_PULLDOWN   false

/*============================================================================*/
/*                        DEFINES DECLARATIONS                                */
/*============================================================================*/

/*============================================================================*/
/*                       VARIABLES DECLARATIONS                               */
/*============================================================================*/

/**
 * \brief List of the resolution to be detected by the device library
 * */

#ifdef TMFL_OS_WINDOWS          /* OS Windows */
tx_cfg_resolution_t resolution_info_tx[RESOLUTION_NB] = {
#else                           /* OS ARM7 */
const tx_cfg_resolution_t resolution_info_tx[RESOLUTION_NB] = {
#endif                          /* endif TMFL_OS_WINDOWS */
	/* TV Formats */
	/* 60 HZ */
	{dl_hdmitx_vfmt_01_640x480p_60hz,     640,   480,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_02_720x480p_60hz,     720,   480,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_03_720x480p_60hz,     720,   480,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_04_1280x720p_60hz,    1280,  720,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_05_1920x1080i_60hz,   1920,  1080,   true,   dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_06_720x480i_60hz,     720,   480,    true,   dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_07_720x480i_60hz,     720,   480,    true,   dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_08_720x240p_60hz,     720,   240,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_09_720x240p_60hz,     720,   240,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_10_720x480i_60hz,     720,   480,    true,   dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_11_720x480i_60hz,     720,   480,    true,   dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_12_720x240p_60hz,     720,   240,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_13_720x240p_60hz,     720,   240,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_14_1440x480p_60hz,    1440,  480,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_15_1440x480p_60hz,    1440,  480,    false,  dl_hdmitx_vfreq_59hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_16_1920x1080p_60hz,   1920,  1080,   false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	/* 50 HZ */
	{dl_hdmitx_vfmt_17_720x576p_50hz,     720,   576,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_18_720x576p_50hz,     720,   576,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_19_1280x720p_50hz,    1280,  720,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_20_1920x1080i_50hz,   1920,  1080,   true,   dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_21_720x576i_50hz,     720,   576,    true,   dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_22_720x576i_50hz,     720,   576,    true,   dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_23_720x288p_50hz,     720,   288,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_24_720x288p_50hz,     720,   288,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_25_720x576i_50hz,     720,   576,    true,   dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_26_720x576i_50hz,     720,   576,    true,   dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_27_720x288p_50hz,     720,   288,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_28_720x288p_50hz,     720,   288,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_29_1440x576p_50hz,    1440,  576,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_30_1440x576p_50hz,    1440,  576,    false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_31_1920x1080p_50hz,   1920,  1080,   false,  dl_hdmitx_vfreq_50hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_32_1920x1080p_24hz,   1920,  1080,   false,  dl_hdmitx_vfreq_24hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_33_1920x1080p_25hz,   1920,  1080,   false,  dl_hdmitx_vfreq_25hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_34_1920x1080p_30hz,   1920,  1080,   false,  dl_hdmitx_vfreq_30hz, DL_HDMITX_P_ASPECT_RATIO_16_9}
#ifdef FORMAT_PC
	/* PC Formats */
	/* 60 HZ */
	, {dl_hdmitx_vfmt_pc_640x480p_60hz,     640,   480,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_800x600p_60hz,     800,   600,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1152x960p_60hz,    1152,  960,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_6_5},
	{dl_hdmitx_vfmt_pc_1024x768p_60hz,    1024,  768,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1280x768p_60hz,    1280,  768,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_5_3},
	{dl_hdmitx_vfmt_pc_1280x1024p_60hz,   1280,  1024,   false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_5_4},
	{dl_hdmitx_vfmt_pc_1360x768p_60hz,    1360,  768,    false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_16_9},
	{dl_hdmitx_vfmt_pc_1400x1050p_60hz,   1400,  1050,   false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1600x1200p_60hz,   1600,  1200,   false,  dl_hdmitx_vfreq_60hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	/* 70 HZ */
	{dl_hdmitx_vfmt_pc_1024x768p_70hz,    1024,  768,    false,  dl_hdmitx_vfreq_70hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	/* 72 HZ */
	{dl_hdmitx_vfmt_pc_640x480p_72hz,     640,   480,    false,  dl_hdmitx_vfreq_72hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_800x600p_72hz,     800,   600,    false,  dl_hdmitx_vfreq_72hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	/* 75 HZ */
	{dl_hdmitx_vfmt_pc_640x480p_75hz,     640,   480,    false,  dl_hdmitx_vfreq_75hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1024x768p_75hz,    1024,  768,    false,  dl_hdmitx_vfreq_75hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_800x600p_75hz,     800,   600,    false,  dl_hdmitx_vfreq_75hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1024x864p_75hz,    1024,  864,    false,  dl_hdmitx_vfreq_75hz, DL_HDMITX_P_ASPECT_RATIO_UNDEFINED},
	{dl_hdmitx_vfmt_pc_1280x1024p_75hz,   1280,  1024,   false,  dl_hdmitx_vfreq_75hz, DL_HDMITX_P_ASPECT_RATIO_5_4},
	/* 85 HZ */
	{dl_hdmitx_vfmt_pc_640x350p_85hz,     640,   350,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_UNDEFINED},
	{dl_hdmitx_vfmt_pc_640x400p_85hz,     640,   400,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_16_10},
	{dl_hdmitx_vfmt_pc_720x400p_85hz,     720,   400,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_9_5},
	{dl_hdmitx_vfmt_pc_640x480p_85hz,     640,   480,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_800x600p_85hz,     800,   600,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1024x768p_85hz,    1024,  768,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1152x864p_85hz,    1152,  864,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1280x960p_85hz,    1280,  960,    false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_4_3},
	{dl_hdmitx_vfmt_pc_1280x1024p_85hz,   1280,  1024,   false,  dl_hdmitx_vfreq_85hz, DL_HDMITX_P_ASPECT_RATIO_5_4},
	/* 87 HZ */
	{dl_hdmitx_vfmt_pc_1024x768i_87hz,    1024,  768,    true,   dl_hdmitx_vfreq_87hz, DL_HDMITX_P_ASPECT_RATIO_4_3}
#endif /* FORMAT_PC */
};

#ifdef TMFL_OS_WINDOWS          /* OS Windows */
tx_cfg_video_signal444 video_port_mapping_yuv444[MAX_UNITS][6] = {
#else                           /* OS ARM7 */
const tx_cfg_video_signal444 video_port_mapping_yuv444[MAX_UNITS][6] = {
#endif                          /* endif TMFL_OS_WINDOWS */
	{
		DL_HDMITX_VID444_BU_0_TO_3,   /* Signals connected to VPA[0..3] */
		DL_HDMITX_VID444_BU_4_TO_7,   /* Signals connected to VPA[4..7] */
		DL_HDMITX_VID444_GY_0_TO_3,   /* Signals connected to VPB[0..3] */
		DL_HDMITX_VID444_GY_4_TO_7,   /* Signals connected to VPB[4..7] */
		DL_HDMITX_VID444_VR_0_TO_3,   /* Signals connected to VPC[0..3] */
		DL_HDMITX_VID444_VR_4_TO_7    /* Signals connected to VPC[4..7] */
	}
};

#ifdef TMFL_OS_WINDOWS          /* OS Windows */
tx_cfg_video_signal444 video_port_mapping_rgb444[MAX_UNITS][6] = {
#else                           /* OS ARM7 */
const tx_cfg_video_signal444 video_port_mapping_rgb444[MAX_UNITS][6] = {
#endif                          /* endif TMFL_OS_WINDOWS */
	{
		DL_HDMITX_VID444_BU_0_TO_3,   /* Signals connected to VPA[0..3] */
		DL_HDMITX_VID444_BU_4_TO_7,   /* Signals connected to VPA[4..7] */
		DL_HDMITX_VID444_GY_0_TO_3,   /* Signals connected to VPB[0..3] */
		DL_HDMITX_VID444_GY_4_TO_7,   /* Signals connected to VPB[4..7] */
		DL_HDMITX_VID444_VR_0_TO_3,   /* Signals connected to VPC[0..3] */
		DL_HDMITX_VID444_VR_4_TO_7    /* Signals connected to VPC[4..7] */
	}
};

#ifdef TMFL_OS_WINDOWS          /* OS Windows */
tx_cfg_video_signal422 video_port_mapping_yuv422[MAX_UNITS][6] = {
#else                           /* OS ARM7 */
const tx_cfg_video_signal422 video_port_mapping_yuv422[MAX_UNITS][6] = {
#endif                          /* endif TMFL_OS_WINDOWS */

	{
		/* Signals connected to VPA[0..3] */
		DL_HDMITX_VID422_Y_4_TO_7,
		/* Signals connected to VPA[4..7] */
		DL_HDMITX_VID422_Y_8_TO_11,
		/* Signals connected to VPB[0..3] */
		DL_HDMITX_VID422_UV_4_TO_7,
		/* Signals connected to VPB[4..7] */
		DL_HDMITX_VID422_UV_8_TO_11,
		/* Signals connected to VPC[0..3] */
		DL_HDMITX_VID422_NOT_CONNECTED,
		/* Signals connected to VPC[4..7] */
		DL_HDMITX_VID422_NOT_CONNECTED
	}
};

#ifdef TMFL_OS_WINDOWS          /* OS Windows */
tx_cfg_video_signal_ccir656 video_port_mapping_ccir656[MAX_UNITS][6] = {
#else                           /* OS ARM7 */
const tx_cfg_video_signal_ccir656 video_port_mapping_ccir656[MAX_UNITS][6] = {
#endif                          /* endif TMFL_OS_WINDOWS */
	{
		/* Signals connected to VPA[0..3] */
		DL_HDMITX_VIDCCIR_4_TO_7,
		/* Signals connected to VPA[4..7] */
		DL_HDMITX_VIDCCIR_8_TO_11,
		/* Signals connected to VPB[0..3] */
		DL_HDMITX_VIDCCIR_NOT_CONNECTED,
		/* Signals connected to VPB[4..7] */
		DL_HDMITX_VIDCCIR_NOT_CONNECTED,
		/* Signals connected to VPC[0..3] */
		DL_HDMITX_VIDCCIR_NOT_CONNECTED,
		/* Signals connected to VPC[4..7] */
		DL_HDMITX_VIDCCIR_NOT_CONNECTED
	}
};

/* Audio port configuration for SPDIF                                         */
/* Here you can specify the audio port routing configuration for SPDIF input. */
/* enableAudioPortSPDIF and groundAudioPortSPDIF should be filled with a      */
/* value build as follows : each bit represent an audio port, LSB is port 0.  */
/* enableAudioClockPortSPDIF and groundAudioClockPortSPDIF can be configured  */
/* with the corresponding enums (See file tx_cfg.h for more details). */
uint8 enable_audio_port_spdif[MAX_UNITS]      = {0x40};
uint8 enable_audio_clock_port_spdif[MAX_UNITS] = {DISABLE_AUDIO_CLOCK_PORT};
uint8 ground_audio_port_spdif[MAX_UNITS]      = {0xBF};
uint8 ground_audio_clock_port_spdif[MAX_UNITS] = {ENABLE_AUDIO_CLOCK_PORT_PULLDOWN};

/* Audio port configuration for I2S                                           */
/* Here you can specify the audio port routing configuration for SPDIF input. */
/* enableAudioPortI2S and groundAudioPortI2S should be filled with a          */
/* value build as follows : each bit represent an audio port, LSB is port 0.  */
/* enableAudioClockPortI2S and groundAudioClockPortI2S can be configured      */
/* with the corresponding enums (See file tx_cfg.h for more details). */
uint8 enable_audio_port_i2s[MAX_UNITS]        = {0x03};
uint8 enable_audio_port_i2s8c[MAX_UNITS]      = {0x1F};
uint8 enable_audio_clock_port_i2s[MAX_UNITS]   = {ENABLE_AUDIO_CLOCK_PORT};
uint8 ground_audio_port_i2s[MAX_UNITS]        = {0xFC};
uint8 ground_audio_port_i2s8c[MAX_UNITS]      = {0xE0};
uint8 ground_audio_clock_port_i2s[MAX_UNITS]   = {DISABLE_AUDIO_CLOCK_PORT_PULLDOWN};

/* Audio port configuration for OBA                                           */
/* Here you can specify the audio port routing configuration for SPDIF input. */
/* enableAudioPortOBA and groundAudioPortOBA should be filled with a          */
/* value build as follows : each bit represent an audio port, LSB is port 0.  */
/* enableAudioClockPortOBA and groundAudioClockPortOBA can be configured      */
/* with the corresponding enums (See file tx_cfg.h for more details). */
uint8 enable_audio_port_oba[MAX_UNITS]        = {0xFF};
uint8 enable_audio_clock_port_oba[MAX_UNITS]   = {ENABLE_AUDIO_CLOCK_PORT};
uint8 ground_audio_port_oba[MAX_UNITS]        = {0x00};
uint8 ground_audio_clock_port_oba[MAX_UNITS]   = {DISABLE_AUDIO_CLOCK_PORT_PULLDOWN};

/* Audio port configuration for DST                                           */
/* Here you can specify the audio port routing configuration for SPDIF input. */
/* enableAudioPortDST and groundAudioPortDST should be filled with a          */
/* value build as follows : each bit represent an audio port, LSB is port 0.  */
/* enableAudioClockPortDST and groundAudioClockPortDST can be configured      */
/* with the corresponding enums (See file tx_cfg.h for more details). */
uint8 enable_audio_port_dst[MAX_UNITS]        = {0xFF};
uint8 enable_audio_clock_port_dst[MAX_UNITS]   = {ENABLE_AUDIO_CLOCK_PORT};
uint8 ground_audio_port_dst[MAX_UNITS]        = {0x00};
uint8 ground_audio_clock_port_dst[MAX_UNITS]   = {DISABLE_AUDIO_CLOCK_PORT_PULLDOWN};

/* Audio port configuration for HBR                                           */
/* Here you can specify the audio port routing configuration for SPDIF input. */
/* enableAudioPortHBR and groundAudioPortHBR should be filled with a          */
/* value build as follows : each bit represent an audio port, LSB is port 0.  */
/* enableAudioClockPortHBR and groundAudioClockPortHBR can be configured      */
/* with the corresponding enums (See file tx_cfg.h for more details). */
uint8 enable_audio_port_hbr[MAX_UNITS]        = {0x1F};
uint8 enable_audio_clock_port_hbr[MAX_UNITS]   = {ENABLE_AUDIO_CLOCK_PORT};
uint8 ground_audio_port_hbr[MAX_UNITS]        = {0xE0};
uint8 ground_audio_clock_port_hbr[MAX_UNITS]   = {DISABLE_AUDIO_CLOCK_PORT_PULLDOWN};

/* Do not modify, those tables are filled dynamically by dlHdmiTxGenerateCfgVideoPortTables API */
uint8 mirror_table_ccir656[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 swap_table_ccir656[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 enable_video_port_ccir656[MAX_UNITS][3] = {{0x00, 0x00, 0x00}};
uint8 ground_video_port_ccir656[MAX_UNITS][3] = {{0xFF, 0xFF, 0xFF}};
uint8 mirror_table_yuv422[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 swap_table_yuv422[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 enable_video_port_yuv422[MAX_UNITS][3] = {{0x00, 0x00, 0x00}};
uint8 ground_video_port_yuv422[MAX_UNITS][3] = {{0xFF, 0xFF, 0xFF}};
uint8 mirror_table_yuv444[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 swap_table_yuv444[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 enable_video_port_yuv444[MAX_UNITS][3] = {{0x00, 0x00, 0x00}};
uint8 ground_video_port_yuv444[MAX_UNITS][3] = {{0xFF, 0xFF, 0xFF}};
uint8 mirror_table_rgb444[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 swap_table_rgb444[MAX_UNITS][6] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
uint8 enable_video_port_rgb444[MAX_UNITS][3] = {{0x00, 0x00, 0x00}};
uint8 ground_video_port_rgb444[MAX_UNITS][3] = {{0xFF, 0xFF, 0xFF}};

/**
 * \brief Configuration Tables. This table can be modified by the customer
 *            to choose its prefered configuration.
 * */
#ifdef TMFL_OS_WINDOWS         /* OS Windows */
tx_driver_config_table_t driver_config_table_tx[MAX_UNITS] = {
	{
		COMMAND_TASK_PRIORITY_0,
		COMMAND_TASK_STACKSIZE_0,
		COMMAND_TASK_QUEUESIZE_0,
		HDCP_CHECK_TASK_PRIORITY_0,
		HDCP_CHECK_TASK_STACKSIZE_0,
		UNIT_I2C_ADDRESS_0,
		windows_i2c_read_function,
		windows_i2c_write_function,
		edid_read_function,
		NULL,                           /* filled dynamically, do not modify */
		RESOLUTION_NB,
		/* filled dynamically, do not modify */
		&mirror_table_ccir656[0][0],
		/* filled dynamically, do not modify */
		&swap_table_ccir656[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_ccir656[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_ccir656[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_yuv422[0][0],
		/* filled dynamically, do not modify */
		&swap_table_yuv422[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_yuv422[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_yuv422[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_yuv444[0][0],
		/* filled dynamically, do not modify */
		&swap_table_yuv444[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_yuv444[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_yuv444[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_rgb444[0][0],
		/* filled dynamically, do not modify */
		&swap_table_rgb444[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_rgb444[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_rgb444[0][0],
		&enable_audio_port_spdif[0],
		&ground_audio_port_spdif[0],
		&enable_audio_clock_port_spdif[0],
		&ground_audio_clock_port_spdif[0],
		&enable_audio_port_i2s[0],
		&ground_audio_port_i2s[0],
		&enable_audio_port_i2s8c[0],
		&ground_audio_port_i2s8c[0],
		&enable_audio_clock_port_i2s[0],
		&ground_audio_clock_port_i2s[0],
		&enable_audio_port_oba[0],
		&ground_audio_port_oba[0],
		&enable_audio_clock_port_oba[0],
		&ground_audio_clock_port_oba[0],
		&enable_audio_port_dst[0],
		&ground_audio_port_dst[0],
		&enable_audio_clock_port_dst[0],
		&ground_audio_clock_port_dst[0],
		&enable_audio_port_hbr[0],
		&ground_audio_port_hbr[0],
		&enable_audio_clock_port_hbr[0],
		&ground_audio_clock_port_hbr[0],
		KEY_SEED,
		DL_HDMITX_PATTERN_BLUE,
		/* by default, the DE signal is considered available */
		1
	}
};
#else            /* OS ARM7 */
tx_driver_config_table_t driver_config_table_tx[MAX_UNITS] = {
	{
		COMMAND_TASK_PRIORITY_0,
		COMMAND_TASK_STACKSIZE_0,
		COMMAND_TASK_QUEUESIZE_0,
		HDCP_CHECK_TASK_PRIORITY_0,
		HDCP_CHECK_TASK_STACKSIZE_0,
		UNIT_I2C_ADDRESS_0,
		rtx_tx_i2c_read_function,
		rtx_tx_i2c_write_function,
		edid_read_function,
		NULL,                           /* filled dynamically, do not modify */
		RESOLUTION_NB,
		/* filled dynamically, do not modify */
		&mirror_table_ccir656[0][0],
		/* filled dynamically, do not modify */
		&swap_table_ccir656[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_ccir656[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_ccir656[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_yuv422[0][0],
		/* filled dynamically, do not modify */
		&swap_table_yuv422[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_yuv422[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_yuv422[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_yuv444[0][0],
		/* filled dynamically, do not modify */
		&swap_table_yuv444[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_yuv444[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_yuv444[0][0],
		/* filled dynamically, do not modify */
		&mirror_table_rgb444[0][0],
		/* filled dynamically, do not modify */
		&swap_table_rgb444[0][0],
		/* filled dynamically, do not modify */
		&enable_video_port_rgb444[0][0],
		/* filled dynamically, do not modify */
		&ground_video_port_rgb444[0][0],
		&enable_audio_port_spdif[0],
		&ground_audio_port_spdif[0],
		&enable_audio_clock_port_spdif[0],
		&ground_audio_clock_port_spdif[0],
		&enable_audio_port_i2s[0],
		&ground_audio_port_i2s[0],
		&enable_audio_port_i2s8c[0],
		&ground_audio_port_i2s8c[0],
		&enable_audio_clock_port_i2s[0],
		&ground_audio_clock_port_i2s[0],
		&enable_audio_port_oba[0],
		&ground_audio_port_oba[0],
		&enable_audio_clock_port_oba[0],
		&ground_audio_clock_port_oba[0],
		&enable_audio_port_dst[0],
		&ground_audio_port_dst[0],
		&enable_audio_clock_port_dst[0],
		&ground_audio_clock_port_dst[0],
		&enable_audio_port_hbr[0],
		&ground_audio_port_hbr[0],
		&enable_audio_clock_port_hbr[0],
		&ground_audio_clock_port_hbr[0],
		KEY_SEED,
		DL_HDMITX_PATTERN_BLUE,
		/* by default, the DE signal is considered available */
		1
	}
};
#endif              /* endif TMFL_OS_WINDOWS */

/*============================================================================*/
/*                              FUNCTIONS                                     */
/*============================================================================*/
#ifdef TMFL_OS_WINDOWS          /* OS Windows */
/******************************************************************************
 *    \brief Read to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C read structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t windows_i2c_read_function
(
    bsl_sys_args_t    *p_sys_args
)
{
	error_code_t   err;

	err = i2c_read(reg_tda_998x, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}

/******************************************************************************
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C write structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t windows_i2c_write_function
(
    bsl_sys_args_t    *p_sys_args
)
{
	error_code_t   err;

	err = i2c_write(reg_tda_998x, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}

#else         /* OS ARM7 */
/*============================================================================*/

/******************************************************************************
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C read structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
rtx_tx_i2c_read_function
(
    bsl_sys_args_t        *p_sys_args
)
{
	error_code_t               err;

	err = i2c_read(reg_tda_998x, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}

/******************************************************************************
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C write structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
rtx_tx_i2c_write_function
(
    bsl_sys_args_t *p_sys_args
)
{
	error_code_t               err;

	err = i2c_write(reg_tda_998x, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}

#endif      /* endif TMFL_OS_WINDOWS */

/*****************************************************************************
 ******************************************************************************
 *                THIS PART MUST NOT BE MODIFIED BY CUSTOMER                  *
 ******************************************************************************
 *****************************************************************************/

/******************************************************************************
 *    \brief  Generate swap and mirror tables in function
 *            of video port mapping tables.
 *
 *    \param unit     Unit identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_generate_cfg_video_port_tables
(
    unit_select_t                  unit,
    tx_driver_config_table_t   *p_config
)
{
	u8   i;

	for(i = 0; i < 6; i++) {
		/* CCIR656 */
		if(video_port_mapping_ccir656[unit][i] != DL_HDMITX_VIDCCIR_NOT_CONNECTED) {
			p_config->p_swap_table_ccir656[video_port_mapping_ccir656[unit][i] & 0x07F] = 5 - i;
			p_config->p_mirror_table_ccir656[video_port_mapping_ccir656[unit][i] & 0x07F] = (u8)(video_port_mapping_ccir656[unit][i] >> 7);
			/* Enable port and disable ground port */
			if(((5 - i) % 2) == 0) {
				p_config->p_enable_video_port_ccir656[i/2] |= 0x0F;
				p_config->p_ground_video_port_ccir656[i/2] &= 0xF0;
			} else {
				p_config->p_enable_video_port_ccir656[i/2] |= 0xF0;
				p_config->p_ground_video_port_ccir656[i/2] &= 0x0F;
			}
		}

		/* YUV422 */
		if(video_port_mapping_yuv422[unit][i] != DL_HDMITX_VID422_NOT_CONNECTED) {
			p_config->p_swap_table_yuv422[video_port_mapping_yuv422[unit][i] & 0x07F] = 5 - i;
			p_config->p_mirror_table_yuv422[video_port_mapping_yuv422[unit][i] & 0x07F] = (u8)(video_port_mapping_yuv422[unit][i] >> 7);
			/* Enable port and disable ground port */
			if(((5 - i) % 2) == 0) {
				p_config->p_enable_video_port_yuv422[i/2] |= 0x0F;
				p_config->p_ground_video_port_yuv422[i/2] &= 0xF0;
			} else {
				p_config->p_enable_video_port_yuv422[i/2] |= 0xF0;
				p_config->p_ground_video_port_yuv422[i/2] &= 0x0F;
			}
		}

		/* YUV444 */
		if(video_port_mapping_yuv444[unit][i] != DL_HDMITX_VID444_NOT_CONNECTED) {
			p_config->p_swap_table_yuv444[video_port_mapping_yuv444[unit][i] & 0x07F] = 5 - i;
			p_config->p_mirror_table_yuv444[video_port_mapping_yuv444[unit][i] & 0x07F] = (u8)(video_port_mapping_yuv444[unit][i] >> 7);
			/* Enable port and disable ground port */
			if(((5 - i) % 2) == 0) {
				p_config->p_enable_video_port_yuv444[i/2] |= 0x0F;
				p_config->p_ground_video_port_yuv444[i/2] &= 0xF0;
			} else {
				p_config->p_enable_video_port_yuv444[i/2] |= 0xF0;
				p_config->p_ground_video_port_yuv444[i/2] &= 0x0F;
			}
		}

		/* RGB444 */
		if(video_port_mapping_rgb444[unit][i] != DL_HDMITX_VID444_NOT_CONNECTED) {
			p_config->p_swap_table_rgb444[video_port_mapping_rgb444[unit][i] & 0x07F] = 5 - i;
			p_config->p_mirror_table_rgb444[video_port_mapping_rgb444[unit][i] & 0x07F] = (u8)(video_port_mapping_rgb444[unit][i] >> 7);
			/* Enable port and disable ground port */
			if(((5 - i) % 2) == 0) {
				p_config->p_enable_video_port_rgb444[i/2] |= 0x0F;
				p_config->p_ground_video_port_rgb444[i/2] &= 0xF0;
			} else {
				p_config->p_enable_video_port_rgb444[i/2] |= 0xF0;
				p_config->p_ground_video_port_rgb444[i/2] &= 0x0F;
			}
		}
	}
}

/******************************************************************************
 *    \brief This function allows to the main driver to retrieve its
 *           configuration parameters.
 *
 *    \param pConfig Pointer to the config structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_cfg_get_config
(
    unit_select_t                  unit,
    tx_driver_config_table_t   *p_config
)
{
	/* Check if unit number is in range */
	RETIF((unit < 0) || (unit >= MAX_UNITS), ERR_DLHDMITX_BAD_UNIT_NUMBER)

	/* Check if pointer is NULL */
	RETIF(p_config == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	*p_config = driver_config_table_tx[unit];

	/* Done here because of const declaration of tables in ARM7 case */
	p_config->p_resolution_info = (ptx_cfg_resolution_t)resolution_info_tx;

	/* Generate swap and mirror tables in function of video port mapping tables */
	dl_hdmi_tx_generate_cfg_video_port_tables(unit, p_config);

	return 0;
}

/******************************************************************************
 *    \brief Driver callback function for i2c EDID read (REQUIRED SW INTERFACE)
 *
 *    \param
 *    \param
 *
 *    \return The call result:
 *            0: the call was successfull
 *            ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *
 ******************************************************************************/
error_code_t edid_read_function(bsl_sys_args_edid_t *p_sys_args)
{
	u8 seg_address = p_sys_args->seg_ptr_addr;
	u8 segptr = p_sys_args->seg_ptr;
	u8 data_address = p_sys_args->data_reg_addr;
	u8 offset = p_sys_args->word_offset;
	u8 *ptr = p_sys_args->p_data;
	u8 nb_char = p_sys_args->len_data;
	u8 i = 0;
	error_code_t       err;

	do {
		err = (i2c_read_edid(seg_address, segptr, data_address, offset, nb_char, ptr)) ? ERR_HDMI_I2C_READ : 0;
		i++;
	} while((err != 0) && (i < 3));

	return err;
}

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
