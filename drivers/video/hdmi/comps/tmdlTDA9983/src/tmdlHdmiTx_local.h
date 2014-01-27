/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_local.h
 *
 * \version       $Revision: 1 $
 *
 * \date          $Date: 02/08/07 08:32 $
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
 *   $History: tx_local.h $
 *
 * *****************  Version 13  *****************
 * User: J. Lamotte Date: 02/08/07   Time: 08:32
 * Updated in $/Source/tx/inc
 * initial version
 *
 *
 *   \endverbatim
 *
 * */

#ifndef DLHDMITX_LOCAL_H
#define DLHDMITX_LOCAL_H

#include "tmdlHdmiTx_IW.h"
#include "tmNxTypes.h"
#include "tmdlHdmiTx_Types.h"

/*============================================================================*/
/*                            MACRO DEFINITIONS                               */
/*============================================================================*/

/* Version of the SW driver */
#define VERSION_COMPATIBILITY 0
#define VERSION_MAJOR         4
#define VERSION_MINOR         18

/* Invalid HDCP seed */
#define HDCP_SEED_NULL      0

/* A default seed value may be defined here, or set to HDCP_SEED_NULL.
 * If HDCP_SEED_NULL, a table of seeds may instead be programmed separately
 * into flash at the location of kSeedTable, below */
#define HDCP_SEED_DEFAULT   HDCP_SEED_NULL

/* Default SHA-1 test handling */
#define HDCP_OPT_DEFAULT    ( DL_HDMITX_HDCP_OPTION_FORCE_PJ_IGNORED \
                              | DL_HDMITX_HDCP_OPTION_FORCE_VSLOW_DDC \
                              | DL_HDMITX_HDCP_OPTION_FORCE_NO_1_1 )

/**
 * A macro to check a condition and if true return a result
 * */
#define RETIF(cond, rslt)       if ((cond)){return (rslt);}

/**
 * A macro to check a condition and if true return
 * ERR_DLHDMITX_BAD_PARAMETER.
 * To save code space, it can be compiled out by defining NO_RETIF_BADPARAM on
 * the compiler command line.
 * */
#ifdef NO_RETIF_BADPARAM
#define RETIF_BADPARAM(cond)
#else
#define RETIF_BADPARAM(cond)  if ((cond)){return ERR_DLHDMITX_BAD_PARAMETER;}
#endif

/**
 * A macro to check a condition and if true, release the semaphore describe by handle and return a result
 * */
#define RETIF_SEM(handle, cond, rslt)       if ((cond)){tx_iwsemaphore_v(handle); return (rslt);}

/* Instance number */
#define INSTANCE_0  0
#define INSTANCE_1  1

/* Number of event */
#define EVENT_NB    9

/* Size of a KSV is five bytes */
#define KSV_SIZE  5

/* Arbitrary short TV format values */
#define TV_INVALID      0
#define TV_VGA_60HZ     1
#define TV_240p_60HZ    2
#define TV_480p_60HZ    3
#define TV_480i_60HZ    4
#define TV_720p_60HZ    5
#define TV_1080p_60HZ   6
#define TV_1080i_60HZ   7
#define TV_288p_50HZ    8
#define TV_576p_50HZ    9
#define TV_576i_50HZ    10
#define TV_720p_50HZ    11
#define TV_1080p_50HZ   12
#define TV_1080i_50HZ   13

/* Shorthands for vinMode values in bslTDA9984.h */
#define iINVALID DL_HDMITX_VINMODE_INVALID
#define iCCIR656 DL_HDMITX_VINMODE_CCIR656
#define iRGB444  DL_HDMITX_VINMODE_RGB444
#define iYUV444  DL_HDMITX_VINMODE_YUV444
#define iYUV422  DL_HDMITX_VINMODE_YUV422

/* Shorthands for input sync */
#define EMB      1
#define EXT      0

/* Shorthands for single/double pixel rate in bslTDA9984.h */
#define SINGLE   DL_HDMITX_PIXRATE_SINGLE
#define DOUBLE   DL_HDMITX_PIXRATE_DOUBLE

/* Shorthands for sampling frequency in txSetAudioInput API */
#define AIF_SF_REFER_TO_STREAM_HEADER   0
#define AIF_SF_32K                      1
#define AIF_SF_44K                      2
#define AIF_SF_48K                      3
#define AIF_SF_88K                      4
#define AIF_SF_96K                      5
#define AIF_SF_176K                     6
#define AIF_SF_192K                     7

/* HDCP check interval in milliseconds */
#define HDCP_CHECK_INTERVAL_MS  2500

/* Number of HDCP checks to carry out after HDCP is started */
#define HDCP_NUM_CHECKS         5

#define DL_HDMITX_CHANNELALLOC_LUT_SIZE 32

static CONST_DAT u8 k_chan_alloc_chan_num[DL_HDMITX_CHANNELALLOC_LUT_SIZE] = \
{2, 3, 3, 4, 3, 4, 4, 5, 4, 5, 5, 6, 5, 6, 6, 7, 6, 7, 7, 8, 4, 5, 5, 6, 5, 6, 6, 7, 6, 7, 7, 8 };

/**
 * Lookup table to convert from EIA/CEA TV video format to
 * aspect ratio used in video infoframe:
 * Aspect ratio 1=4:3, 2=16:9
 * */
#ifndef FORMAT_PC
static CONST_DAT u8 k_vfmt_to_aspect_tv[DL_HDMITX_VFMT_TV_NUM] =
#else /* FORMAT_PC */
static CONST_DAT u8 k_vfmt_to_aspect_tv[DL_HDMITX_VFMT_TV_NUM + DL_HDMITX_VFMT_PC_NUM] =
#endif /* FORMAT_PC */
{
	0,  /* HDMITX_VFMT_NULL                */
	1,  /* HDMITX_VFMT_01_640x480p_60HZ    */
	1,  /* HDMITX_VFMT_02_720x480p_60HZ    */
	2,  /* HDMITX_VFMT_03_720x480p_60HZ    */
	2,  /* HDMITX_VFMT_04_1280x720p_60HZ   */
	2,  /* HDMITX_VFMT_05_1920x1080i_60HZ  */
	1,  /* HDMITX_VFMT_06_720x480i_60HZ    */
	2,  /* HDMITX_VFMT_07_720x480i_60HZ    */
	1,  /* HDMITX_VFMT_08_720x240p_60HZ    */
	2,  /* HDMITX_VFMT_09_720x240p_60HZ    */
	1,  /* HDMITX_VFMT_10_720x480i_60HZ    */
	2,  /* HDMITX_VFMT_11_720x480i_60HZ    */
	1,  /* HDMITX_VFMT_12_720x240p_60HZ    */
	2,  /* HDMITX_VFMT_13_720x240p_60HZ    */
	1,  /* HDMITX_VFMT_14_1440x480p_60HZ   */
	2,  /* HDMITX_VFMT_15_1440x480p_60HZ   */
	2,  /* HDMITX_VFMT_16_1920x1080p_60HZ  */
	1,  /* HDMITX_VFMT_17_720x576p_50HZ    */
	2,  /* HDMITX_VFMT_18_720x576p_50HZ    */
	2,  /* HDMITX_VFMT_19_1280x720p_50HZ   */
	2,  /* HDMITX_VFMT_20_1920x1080i_50HZ  */
	1,  /* HDMITX_VFMT_21_720x576i_50HZ    */
	2,  /* HDMITX_VFMT_22_720x576i_50HZ    */
	1,  /* HDMITX_VFMT_23_720x288p_50HZ    */
	2,  /* HDMITX_VFMT_24_720x288p_50HZ    */
	1,  /* HDMITX_VFMT_25_720x576i_50HZ    */
	2,  /* HDMITX_VFMT_26_720x576i_50HZ    */
	1,  /* HDMITX_VFMT_27_720x288p_50HZ    */
	2,  /* HDMITX_VFMT_28_720x288p_50HZ    */
	1,  /* HDMITX_VFMT_29_1440x576p_50HZ   */
	2,  /* HDMITX_VFMT_30_1440x576p_50HZ   */
	2,  /* HDMITX_VFMT_31_1920x1080p_50HZ  */
	2,  /* HDMITX_VFMT_32_1920x1080p_24HZ  */
	2,  /* HDMITX_VFMT_33_1920x1080p_25HZ  */
	2   /* HDMITX_VFMT_34_1920x1080p_30HZ  */
#ifdef FORMAT_PC
	, 1, /* HDMITX_VFMT_PC_640x480p_60HZ    */
	1,  /* HDMITX_VFMT_PC_800x600p_60HZ    */
	1,  /* HDMITX_VFMT_PC_1152x960p_60HZ   */
	1,  /* HDMITX_VFMT_PC_1024x768p_60HZ   */
	1,  /* HDMITX_VFMT_PC_1280x768p_60HZ   */
	1,  /* HDMITX_VFMT_PC_1280x1024p_60HZ  */
	1,  /* HDMITX_VFMT_PC_1360x768p_60HZ   */
	1,  /* HDMITX_VFMT_PC_1400x1050p_60HZ  */
	1,  /* HDMITX_VFMT_PC_1600x1200p_60HZ  */
	1,  /* HDMITX_VFMT_PC_1024x768p_70HZ   */
	1,  /* HDMITX_VFMT_PC_640x480p_72HZ    */
	1,  /* HDMITX_VFMT_PC_800x600p_72HZ    */
	1,  /* HDMITX_VFMT_PC_640x480p_75HZ    */
	1,  /* HDMITX_VFMT_PC_1024x768p_75HZ   */
	1,  /* HDMITX_VFMT_PC_800x600p_75HZ    */
	1,  /* HDMITX_VFMT_PC_1024x864p_75HZ   */
	1,  /* HDMITX_VFMT_PC_1280x1024p_75HZ  */
	1,  /* HDMITX_VFMT_PC_640x350p_85HZ    */
	1,  /* HDMITX_VFMT_PC_640x400p_85HZ    */
	1,  /* HDMITX_VFMT_PC_720x400p_85HZ    */
	1,  /* HDMITX_VFMT_PC_640x480p_85HZ    */
	1,  /* HDMITX_VFMT_PC_800x600p_85HZ    */
	1,  /* HDMITX_VFMT_PC_1024x768p_85HZ   */
	1,  /* HDMITX_VFMT_PC_1152x864p_85HZ   */
	1,  /* HDMITX_VFMT_PC_1280x960p_85HZ   */
	1,  /* HDMITX_VFMT_PC_1280x1024p_85HZ  */
	1   /* HDMITX_VFMT_PC_1024x768i_87HZ   */
#endif /* FORMAT_PC */
};

/**
 * Lookup table to convert from EIA/CEA TV video format to
 * the short format of resolution/interlace/frequency
 * */
static CONST_DAT u8 k_vfmt_to_short_fmt_tv[DL_HDMITX_VFMT_TV_NUM] = {
	TV_INVALID,         /* HDMITX_VFMT_NULL               */
	TV_VGA_60HZ,        /* HDMITX_VFMT_01_640x480p_60HZ   */
	TV_480p_60HZ,       /* HDMITX_VFMT_02_720x480p_60HZ   */
	TV_480p_60HZ,       /* HDMITX_VFMT_03_720x480p_60HZ   */
	TV_720p_60HZ,       /* HDMITX_VFMT_04_1280x720p_60HZ  */
	TV_1080i_60HZ,      /* HDMITX_VFMT_05_1920x1080i_60HZ */
	TV_480i_60HZ,       /* HDMITX_VFMT_06_720x480i_60HZ   */
	TV_480i_60HZ,       /* HDMITX_VFMT_07_720x480i_60HZ   */
	TV_240p_60HZ,       /* HDMITX_VFMT_08_720x240p_60HZ   */
	TV_240p_60HZ,       /* HDMITX_VFMT_09_720x240p_60HZ   */
	TV_480i_60HZ,       /* HDMITX_VFMT_10_720x480i_60HZ   */
	TV_480i_60HZ,       /* HDMITX_VFMT_11_720x480i_60HZ   */
	TV_240p_60HZ,       /* HDMITX_VFMT_12_720x240p_60HZ   */
	TV_240p_60HZ,       /* HDMITX_VFMT_13_720x240p_60HZ   */
	TV_480p_60HZ,       /* HDMITX_VFMT_14_1440x480p_60HZ  */
	TV_480p_60HZ,       /* HDMITX_VFMT_15_1440x480p_60HZ  */
	TV_1080p_60HZ,      /* HDMITX_VFMT_16_1920x1080p_60HZ */
	TV_576p_50HZ,       /* HDMITX_VFMT_17_720x576p_50HZ   */
	TV_576p_50HZ,       /* HDMITX_VFMT_18_720x576p_50HZ   */
	TV_720p_50HZ,       /* HDMITX_VFMT_19_1280x720p_50HZ  */
	TV_1080i_50HZ,      /* HDMITX_VFMT_20_1920x1080i_50HZ */
	TV_576i_50HZ,       /* HDMITX_VFMT_21_720x576i_50HZ   */
	TV_576i_50HZ,       /* HDMITX_VFMT_22_720x576i_50HZ   */
	TV_288p_50HZ,       /* HDMITX_VFMT_23_720x288p_50HZ   */
	TV_288p_50HZ,       /* HDMITX_VFMT_24_720x288p_50HZ   */
	TV_576i_50HZ,       /* HDMITX_VFMT_25_720x576i_50HZ   */
	TV_576i_50HZ,       /* HDMITX_VFMT_26_720x576i_50HZ   */
	TV_288p_50HZ,       /* HDMITX_VFMT_27_720x288p_50HZ   */
	TV_288p_50HZ,       /* HDMITX_VFMT_28_720x288p_50HZ   */
	TV_576p_50HZ,       /* HDMITX_VFMT_29_1440x576p_50HZ  */
	TV_576p_50HZ,       /* HDMITX_VFMT_30_1440x576p_50HZ  */
	TV_1080p_50HZ       /* HDMITX_VFMT_31_1920x1080p_50HZ */
};

/**
 * Macro to pack vinMode(0-5), pixRate(0-1), syncIn(0-1) and bVerified(0-1)
 * into a byte
 * */
#define PKBYTE(mode,rate,sync,verf) (((rate)<<7)|((sync)<<6)|((verf)<<5)|((mode)&0x1F))

/**
 * Macros to unpack vinMode(0-5), pixRate(0-1), syncIn(0-1) and bVerified(0-1)
 * from a byte
 * */
#define UNPKRATE(byte) (((byte)>>7)&1)
#define UNPKSYNC(byte) (((byte)>>6)&1)
#define UNPKVERF(byte) (((byte)>>5)&1)
#define UNPKMODE(byte) ((byte)&0x1F)

/**
 * Lookup table to match main video settings and look up sets of
 * Refpix and Refline values
 * */
static CONST_DAT struct {
	/* Values to match */
	/* Packed vinMode, pixRate, syncIn, bVerified */
	u8   mode_rate_sync_verf;
	u8   short_vin_fmt;
	u8   short_vout_fmt;
	/* Values to look up */
	u16  ref_pix;             /* Output values */
	u16  ref_line;
	u16  sc_ref_pix;           /* Scaler values */
	u16  sc_ref_line;
} k_refpix_refline [] = {
	/*************************************************************/
	/** Rows formatted in "Refpix_Refline.xls" and pasted here  **/
	/** DO NOT DELETE ANY ROWS, to keep all scaler combinations **/
	/*************************************************************/
	/*        mode_____Rate___Sync_Verf  shortVinFmt     shortVoutFmt    refPix  refLine  scRefPix  scRefLine  Test ID  */
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x08b,  0x024,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x08b,  0x012,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x08b,  0x00e,   0x078,    0x017},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x08b,  0x021,   0x078,    0x017},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x08b,  0x017,   0x078,    0x02c},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x08b,  0x013,   0x078,    0x02c},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x08b,  0x027,   0x078,    0x02c},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x091,  0x026,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x091,  0x013,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x091,  0x00f,   0x085,    0x018},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x091,  0x022,   0x085,    0x018},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x091,  0x019,   0x085,    0x02e},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x091,  0x014,   0x085,    0x02e},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x091,  0x028,   0x085,    0x02e},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x014,  0x20d,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x014,  0x2cb,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x014,  0x44c,   0x359,    0x004},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x014,  0x436,   0x359,    0x004},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x011,  0x2d3,   0x358,    0x007},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x011,  0x452,   0x358,    0x007},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x011,  0x43e,   0x358,    0x007},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x00d,  0x26b,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x00d,  0x2cb,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x00d,  0x44b,   0x35f,    0x001},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x00d,  0x435,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x00d,  0x2d1,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x00d,  0x451,   0x35f,    0x001},
	/*  */
	{PKBYTE(iCCIR656, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x00d,  0x43d,   0x35f,    0x001},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x08b,  0x024,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x08b,  0x012,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x08b,  0x00e,   0x078,    0x017},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x08b,  0x021,   0x078,    0x017},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x08b,  0x017,   0x078,    0x02c},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x08b,  0x013,   0x078,    0x02c},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x08b,  0x027,   0x078,    0x02c},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x091,  0x026,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x091,  0x013,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x091,  0x00f,   0x085,    0x018},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x091,  0x022,   0x085,    0x018},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x091,  0x019,   0x085,    0x02e},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x091,  0x014,   0x085,    0x02e},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EMB, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x091,  0x028,   0x085,    0x02e},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x014,  0x20d,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x014,  0x2cb,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x014,  0x44c,   0x359,    0x004},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x014,  0x436,   0x359,    0x004},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x011,  0x2d3,   0x358,    0x007},
	/* VID_F_01 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x011,  0x452,   0x358,    0x007},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x011,  0x43e,   0x358,    0x007},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x00d,  0x26b,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x00d,  0x2cb,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x00d,  0x44b,   0x35f,    0x001},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x00d,  0x435,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x00d,  0x2d1,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x00d,  0x451,   0x35f,    0x001},
	/*  */
	{PKBYTE(iCCIR656, DOUBLE, EXT, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x00d,  0x43d,   0x35f,    0x001},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x08d,  0x028,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x08d,  0x014,   0x078,    0x017},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x08d,  0x010,   0x078,    0x017},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x08d,  0x021,   0x078,    0x017},
	/* VID_F_01 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x08d,  0x017,   0x078,    0x02c},
	/* VID_F_01 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x08d,  0x014,   0x078,    0x02c},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x08d,  0x027,   0x078,    0x02c},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x093,  0x02a,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x093,  0x013,   0x085,    0x018},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x093,  0x00e,   0x085,    0x018},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x093,  0x022,   0x085,    0x018},
	/* VID_F_06 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x093,  0x019,   0x085,    0x02e},
	/* VID_F_06 */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x093,  0x014,   0x085,    0x02e},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x093,  0x028,   0x085,    0x02e},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_720p_50HZ,   TV_1080p_50HZ,  0x2bf,  0x024,   0x105,    0x019},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_720p_60HZ,   TV_1080p_60HZ,  0x175,  0x024,   0x105,    0x019},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_1080i_50HZ,  TV_1080p_50HZ,  0x2d3,  0x023,   0x0c3,    0x014},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EMB, 1),   TV_1080i_60HZ,  TV_1080p_60HZ,  0x11b,  0x023,   0x0c3,    0x014},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_480p_60HZ,   0x016,  0x20d,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_720p_60HZ,   0x016,  0x2cb,   0x359,    0x004},
	/* VID_F_04 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_1080i_60HZ,  0x016,  0x44c,   0x359,    0x004},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480i_60HZ,   TV_1080p_60HZ,  0x016,  0x436,   0x359,    0x004},
	/* VID_F_01 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_720p_60HZ,   0x013,  0x2d3,   0x358,    0x007},
	/* VID_F_01 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_1080i_60HZ,  0x013,  0x452,   0x358,    0x007},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_480p_60HZ,   TV_1080p_60HZ,  0x013,  0x43e,   0x358,    0x007},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_576p_50HZ,   0x00f,  0x26b,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_720p_50HZ,   0x00f,  0x2cb,   0x35f,    0x001},
	/* VID_F_09 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_1080i_50HZ,  0x00f,  0x44b,   0x35f,    0x001},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576i_50HZ,   TV_1080p_50HZ,  0x00f,  0x435,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_720p_50HZ,   0x00f,  0x2d1,   0x35f,    0x001},
	/* VID_F_06 */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_1080i_50HZ,  0x00f,  0x451,   0x35f,    0x001},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_576p_50HZ,   TV_1080p_50HZ,  0x00f,  0x43d,   0x35f,    0x001},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_720p_50HZ,   TV_1080p_50HZ,  0x1bb,  0x463,   0x7bb,    0x000},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_720p_60HZ,   TV_1080p_60HZ,  0x071,  0x463,   0x671,    0x000},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_1080i_50HZ,  TV_1080p_50HZ,  0x213,  0x460,   0xa4f,    0x000},
	/*  */
	{PKBYTE(iYUV422, SINGLE, EXT, 1),   TV_1080i_60HZ,  TV_1080p_60HZ,  0x05b,  0x460,   0x897,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV422, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV422, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, SINGLE, EMB, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, SINGLE, EXT, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iYUV444, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, SINGLE, EMB, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_480p_60HZ,   TV_VGA_60HZ,    0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, SINGLE, EXT, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, DOUBLE, EMB, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_480p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_04 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_480i_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_720p_60HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_01 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_480p_60HZ,   TV_1080i_60HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_576p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_09 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_576i_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_720p_50HZ,   0x000,  0x000,   0x000,    0x000},
	/* VID_F_06 */
	{PKBYTE(iRGB444, DOUBLE, EXT, 0),   TV_576p_50HZ,   TV_1080i_50HZ,  0x000,  0x000,   0x000,    0x000},
	/* EndTable */
	{PKBYTE(iINVALID, DOUBLE, EMB, 0),   TV_INVALID,     TV_INVALID,     0x000,  0x000,   0x000,    0x000}
};

/*============================================================================*/
/*                         ENUM OR TYPE DEFINITIONS                           */
/*============================================================================*/
/* Enum listing all the type of colorimetry */
typedef enum {
	DL_HDMITX_COLORIMETRY_NO_DATA  = 0,
	DL_HDMITX_COLORIMETRY_ITU601   = 1,
	DL_HDMITX_COLORIMETRY_ITU709   = 2,
	DL_HDMITX_COLORIMETRY_EXTENDED = 3
} tx_colorimetry_t;

/* Possible states of the state machine */
typedef enum {
	STATE_NOT_INITIALIZED,  /**< Driver is not initialized */
	STATE_INITIALIZED,      /**< Driver is initialized */
	STATE_UNPLUGGED,        /**< Receiver device not connected */
	STATE_PLUGGED,          /**< Receiver device connected, clock lock */
	STATE_EDID_AVAILABLE    /**< Managed to read receiver's EDID */
} tx_driver_state_t;

/* revocation list structure */
typedef struct {
	u8                      *p_list;
	u32                      length;
} revocation_list_t;

/* unit configuration structure */
typedef struct {
	/**< Is unit instanciated ? */
	bool                        opened;
	/**< Is HDCP enabled ? */
	bool                        hdcp_enable;
	tx_hdcp_options_t     hdcp_options;			/**< HDCP options */
	/**< Is repeater enabled ? */
	bool                        repeater_enable;
	/**< Enable simplayHD support */
	bool                        simplay_hd;
	/**< Version of the HW device */
	tx_device_version_t   device_version;
	/**< Pointer to raw EDID data */
	u8                       *p_edid_buffer;
	/**< Size of buffer for raw EDID data */
	u32                      edid_buffer_size;
	/**< Handle of the command task associated to this unit */
	tx_iwtask_handle_t    command_task_handle;
	/**< Handle of the message queue associated to this unit */
	tx_iwqueue_handle_t   queue_handle;
	/**< Handle of the hdcp check task associated to this unit */
	tx_iwtask_handle_t	hdcp_task_handle;
	/**< Current state of the driver */
	tx_driver_state_t     state;
	ptx_callback_t       p_callback;				/**< Data callback */
	/**< Revolation List */
	revocation_list_t            revocation_list;
} unit_config_t;

/* Instance status */

/* Video information structure */
typedef struct _tx_video_info_t {
	/* Video mute state: on/off */
	bool                        video_mute_state;
	tx_video_in_config_t   video_in_config;  /* Video input configuration */
	tx_video_out_config_t  video_out_config; /* Video output configuration */
} tx_video_info_t, *ptx_video_info_t;

/* Audio information structure */
typedef struct _tx_audio_info_t {
	/* Audio mute state: on/off */
	bool                        audio_mute_state;
	tx_audio_in_config_t   audio_in_cfg;     /* Audio input configuration */
} tx_audio_info_t, *ptx_audio_info_t;

/* Event state structure */
typedef struct _tx_event_state_t {
	tx_event_t       event;  /* Event */
	tx_event_status_t status; /* Event status: enabled or disabled */
} tx_event_state_t, *ptx_event_state_t;

/* Color bars state structure */
typedef struct _tx_col_bar_state_t {
	/* To be able to disable colorBar */
	bool disable_color_bar_on_r0;
	/* Used to auto-reset colour bars */
	bool hdcp_colbar_change;
	/* true when ENCRYPT or T0 interrupt */
	bool hdcp_encrypt_or_t0;
	/* true when BKSV secure or T0 */
	bool hdcp_secure_or_t0;
	/* API txSetInputOutput call at least one time*/
	bool in_out_first_set_done;
	bool color_bar_on;
	bool change_color_bar_now;
} tx_col_bar_state_t, *ptx_col_bar_state_t;

/* instance status structure */
typedef struct {
	/* Video information: current mode and format... */
	ptx_video_info_t      p_video_info;
	/* Audio information: current mode and format... */
	ptx_audio_info_t      p_audio_info;
	/* Event state: enabled or disabled */
	ptx_event_state_t     p_event_state;
	ptx_col_bar_state_t    p_col_bar_state;   /* Color bars state */
	/* Numero of the buffer used for Gamut metadata (0 or 1) */
	u8                       gamut_buf_num;
} instance_status_t;

#endif /* DLHDMITX_LOCAL_H */

/*============================================================================*/
/*                               END OF FILE                                  */
/*============================================================================*/
