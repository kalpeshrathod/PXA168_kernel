/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          bsl.h
 *
 * \version       $Revision: 41 $
 *
 * \date          $Date: 17/10/07 14:11 $
 *
 * \brief         BSL driver component API for the TDA998x HDMI Transmitter
 *
 * \section refs  Reference Documents
 * HDMI Driver - Outline Architecture.doc,
 * HDMI Driver - bsl - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History: bsl.h $
 *
 * *****************  Version 41  ****************
 * User: B.Vereecke     Date: 17/10/07   Time: 14:11
 * Updated in $/Source/bsl/src
 * PR872 : add new formats, 1080p24/25/30
 *
 * *****************  Version 40  *****************
 * User: B.Vereecke      Date: 17/07/07   Time: 10:30
 * Updated in $/Source/bsl/Inc
 * PR217 - Add a new Pattern type in bslTestPattern_t
 *			it is used for set the bluescreen
 *
 * *****************  Version 39  *****************
 * User: J. Lamotte      Date: 13/06/07   Time: 12:00
 * Updated in $/Source/bsl/Inc
 * PR397 - Merge with PR322
 *       add 9981 API definitions for 9983
 *       (return NOT_SUPPORTED)
 *
 * *****************  Version 38  *****************
 * User: G. BURNOUF      Date: 08/06/07   Time: 10:40
 * Updated in $/Source/bsl/Inc
 * PR347 - Add new formatPC
 *
 * *****************  Version 37  *****************
 * User: J/ Lamotte         Date: 24/04/07   Time: 10:40
 * Updated in $/Source/bsl/Inc
 * PR50 - Change comment for bslSetAudioPortConfig
 *        and bslSetVideoPortConfig functions (TDA9981)
 *
 * *****************  Version 36  *****************
 * User: Burnouf         Date: 16/04/07   Time: 11:30
 * Updated in $/Source/bsl/Inc
 * PR50 - add new API bslRxSenseGetStatus for TDA9981
 *      - add new API only for debug bslFlagSwInt for TDA9981
 *      - add new index for the new callback interrupt of TDA9981
 *
 * *****************  Version 35  *****************
 * User: J. Lamotte      Date: 16/04/07   Time: 11:30
 * Updated in $/Source/bsl/Inc
 * PR50 - add video and audio port configuration for TDA9981
 *        bslSetAudioPortConfig and
 *        bslSetVideoPortConfig
 *      - define HDMITX_UNITS_MAX (in bsl_local.h before)
 *
 * *****************  Version 34  *****************
 * User: J. Lamotte    Date: 25/04/07   Time: 14:40
 * Updated in $/Source/bsl/Inc
 * PR273 - add PLL configuration before soft reset
 *         in function bslInit (2 more parameters)
 *
 * *****************  Version 33  *****************
 * User: C. Logiou      Date: 08/03/07   Time: 16:52
 * Updated in $/Source/bsl/Inc
 * PR214 - add new input format repeated 480i/576i
 *
 * *****************  Version 32  *****************
 * User: burnouf      Date: 06/02/07   Time: 16:52
 * Updated in $/Source/bsl/Inc
 * PR49 - add PC Formats
 *
 * *****************  Version 31  *****************
 * User: burnouf      Date: 08/01/07   Time: 15:33
 * Updated in $/Source/bsl/Inc
 * PR72 - add function bslHwGetCapabilities
 *
 * *****************  Version 30  *****************
 * User: Djw          Date: 17/11/06   Time: 18:48
 * Updated in $/Source/bsl/Inc
 * PNF72 - Corrections to doxygen info for
 * bslAudioOutSetChanStatus
 *
 * *****************  Version 28  *****************
 * User: Mayhew       Date: 10/11/06   Time: 10:35
 * Updated in $/Source/bsl/Inc
 * PNF68 Add note to bslHdcpConfigure for disabling authentication
 * checking
 *
 * *****************  Version 27  *****************
 * User: Mayhew       Date: 10/11/06   Time: 10:03
 * Updated in $/Source/bsl/Inc
 * PNF68 HdcpCheck params thisCheck and maxChecks replaced by
 * uTimeSinceLastCallMs, and Check_t values reordered
 * PNF68 Params uCheckIntervalMs and uChecksToDo added to
 * bslHdcpConfigure
 * PNF68 bslHwHandleTimer prototype removed
 *
 * *****************  Version 26  *****************
 * User: Djw          Date: 9/11/06    Time: 18:05
 * Updated in $/Source/bsl/Inc
 * PNF69 Add bslAudioOutSetChanStatus and
 * bslAudioOutSetChanStatusMapping
 *
 * *****************  Version 25  *****************
 * User: Mayhew       Date: 6/11/06    Time: 17:46
 * Updated in $/Source/bsl/Inc
 * PNF68 Add bslHwHandleTimer
 *
 * *****************  Version 23  *****************
 * User: Djw          Date: 25/10/06   Time: 13:41
 * Updated in $/Source/bsl/Inc
 * PNF58 Added vinMode parameter to bslVideoInSetSyncAuto
 *
 * *****************  Version 21  *****************
 * User: Mayhew       Date: 13/10/06   Time: 11:01
 * Updated in $/Source/bsl/Inc
 * PNF37 Move arguments from bslVideoInSetConfig,
 * bslVideoOutSetSync and bslVideoOutSetFine (deleted)
 * to new APIs bslVideoInSetSyncAuto/Manual
 *
 * *****************  Version 19  *****************
 * User: Mayhew       Date: 15/09/06   Time: 15:54
 * Updated in $/Source/bsl/Inc
 * PNF22 Add HdcpCheck API
 * PNF23 Add GetOtp API
 *
 * *****************  Version 17  *****************
 * User: Mayhew       Date: 4/09/06    Time: 10:03
 * Updated in $/Source/bsl/Inc
 * PNF20: Add HDMITX_HDCP_OPTION_FORCE_VSLOW_DDC
 *
 * *****************  Version 15  *****************
 * User: Djw          Date: 23/08/06   Time: 18:18
 * Updated in $/Source/bsl/Inc
 * Updated comments for bslctsRef_t enum.
 *
 * *****************  Version 14  *****************
 * User: Djw          Date: 21/08/06   Time: 14:02
 * Updated in $/Source/bsl/Inc
 * Correction to bslTmdsOut_t enum.
 *
 * *****************  Version 13  *****************
 * User: Mayhew       Date: 10/07/06   Time: 12:31
 * Updated in $/Source/bsl/Inc
 * Fix Doxygen comment warnings
 *
 * *****************  Version 11  *****************
 * User: Mayhew       Date: 30/06/06   Time: 12:40
 * Updated in $/Source/bsl/Inc
 * HDMITX_EDID_SINK_ type replaced with HDMITX_SINK_ type for clarity
 *
 * *****************  Version 9  *****************
 * User: Djw          Date: 16/06/06   Time: 12:05
 * Updated in $/Source/bsl/Inc
 * Added flag to Init to support use of alternate i2c address for EDID.
 *
 * *****************  Version 7  *****************
 * User: Mayhew       Date: 5/06/06    Time: 14:28
 * Updated in $/Source/bsl/Inc
 * Add bslHdcpGetT0FailState, bslSysTimerWait. Move error
 * codes from _local.h.
 *
 * *****************  Version 5  *****************
 * User: Djw          Date: 24/05/06   Time: 11:15
 * Updated in $/Source/bsl/Inc
 * Added Infoframe packet APIs
 *
 * *****************  Version 4  *****************
 * User: Mayhew       Date: 15/05/06   Time: 15:56
 * Updated in $/Source/bsl/Inc
 * Correct the order of values in bslVoutDbits_t. Add mask to
 * bslHdcpOptions_t.
 *
 * *****************  Version 3  *****************
 * User: Mayhew       Date: 10/05/06   Time: 16:58
 * Updated in $/Source/bsl/Inc
 * Added HDCP APIs, ScalerGetMode and HwStartup
 *
 * *****************  Version 2  *****************
 * User: Mayhew       Date: 11/04/06   Time: 14:20
 * Updated in $/Source/bsl/Inc
 * Add HDMITX_UPSAMPLE_AUTO
 *
 * *****************  Version 1  *****************
 * User: Mayhew       Date: 4/04/06    Time: 16:27
 * Created in $/Source/bsl/Inc
 * Driver API phase 2
 *
 *   \endverbatim
 *
 * */

#ifndef BSLHDMITX_H
#define BSLHDMITX_H

/*============================================================================*/
/*                       COMPILER COMMAND LINE BUILD OPTIONS                  */
/*                                                                            */
/* #define          PC Build            8051 Build                            */
/* CONST_DAT        const               code                                  */
/* RAM_DAT          (blank)             xdata                                 */
/* FUNC_PTR         (blank)             code                                  */
/*============================================================================*/

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/
#include "tmNxCompId.h"

/*============================================================================*/
/*                       MACRO DEFINITIONS                                    */
/*============================================================================*/

/**
 * The maximum number of supported HDMI Transmitter units
 * */
#define HDMITX_UNITS_MAX       2

/** \name Errors
 *  The group of error codes returned by all API and internal functions
 * */
/*@{*/
/** The base offset for all error codes.
 *  This needs defining as non-zero if this component is integrated with others
 *  and all component error ranges are to be kept separate.
 * */
#define ERR_HDMI_BASE	0x00

/** Define the OK code if not defined already */
#ifndef TM_OK
#define TM_OK   0
#endif

/** SW interface compatibility error */
#define ERR_HDMI_COMPATIBILITY            (ERR_HDMI_BASE + 0x001U)

/** SW major version error */
#define ERR_HDMI_MAJOR_VERSION            (ERR_HDMI_BASE + 0x002U)

/** SW component version error */
#define ERR_HDMI_COMP_VERSION             (ERR_HDMI_BASE + 0x003U)

/** Invalid device unit number */
#define ERR_HDMI_BAD_UNIT_NUMBER          (ERR_HDMI_BASE + 0x005U)

/** Invalid input parameter other than unit number */
#define ERR_HDMI_BAD_PARAMETER            (ERR_HDMI_BASE + 0x009U)

/** Inconsistent input parameters */
#define ERR_HDMI_INCONSISTENT_PARAMS      (ERR_HDMI_BASE + 0x010U)

/** Component is not initialized */
#define ERR_HDMI_NOT_INITIALIZED          (ERR_HDMI_BASE + 0x011U)

/** Command not supported for current device */
#define ERR_HDMI_NOT_SUPPORTED            (ERR_HDMI_BASE + 0x013U)

/** Initialization failed */
#define ERR_HDMI_INIT_FAILED              (ERR_HDMI_BASE + 0x014U)

/** Component is busy and cannot do a new operation */
#define ERR_HDMI_BUSY                     (ERR_HDMI_BASE + 0x015U)

/** I2C read error */
#define ERR_HDMI_I2C_READ                 (ERR_HDMI_BASE + 0x017U)

/** I2C write error */
#define ERR_HDMI_I2C_WRITE                (ERR_HDMI_BASE + 0x018U)

/** Assertion failure */
#define ERR_HDMI_ASSERTION                (ERR_HDMI_BASE + 0x049U)

/** Bad EDID block checksum */
#define ERR_HDMI_INVALID_STATE            (ERR_HDMI_BASE + 0x066U)
#define ERR_HDMI_INVALID_CHECKSUM         ERR_HDMI_INVALID_STATE

/** No connection to HPD pin */
#define ERR_HDMI_NULL_CONNECTION          (ERR_HDMI_BASE + 0x067U)

/** Not allowed in DVI mode */
#define ERR_HDMI_OPERATION_NOT_PERMITTED  (ERR_HDMI_BASE + 0x068U)

/* Maximum error code defined */
#define ERR_HDMI_MAX              ERR_HDMI_OPERATION_NOT_PERMITTED

/*@}*/

/** size descriptor block of monitor descriptor */
#define EDID_MONITOR_DESCRIPTOR_SIZE   13

#define NUMBER_DTD_STORED       10

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITIONS                             */
/*============================================================================*/

/** Sink category */
typedef enum {
	HDMITX_SINK_CAT_NOT_REPEATER    = 0, /**< Not repeater  */
	HDMITX_SINK_CAT_REPEATER        = 1, /**< repeater      */
	HDMITX_SINK_CAT_INVALID         = 3  /**< Invalid       */
} bsl_sink_category_t;

/*============================================================================*/
/**
 * basic display parameters structure
 * */
typedef struct _bsl_edid_bdparam_t {
	/**< Video Input Definition                       */
	u8   u_video_input_def;
	/**< Max. Horizontal Image Size in cm             */
	u8   u_max_horizontal_size;
	/**< Max. Vertical Image Size in cm               */
	u8   u_max_vertical_size;
	/**< Gamma                                        */
	u8   u_gamma;
	/**< Feature support                              */
	u8   u_feature_support;
} bsl_edid_bdparam_t;

/*============================================================================*/
/**
 * First monitor descriptor structure
 * */
typedef struct _bsl_edid_first_md_t {
	/**< true when parameters of struct are available   */
	bool    b_desc_record;
	/**< Monitor Name                                   */
	u8   u_monitor_name[EDID_MONITOR_DESCRIPTOR_SIZE];
} bsl_edid_first_md_t;

/*============================================================================*/
/**
 * Second monitor descriptor structure
 * */
typedef struct _bsl_edid_second_md_t {
	/**< true when parameters of struct are available   */
	bool    b_desc_record;
	/**< Min vertical rate in Hz                        */
	u8   u_min_vertical_rate;
	/**< Max vertical rate in Hz                        */
	u8   u_max_vertical_rate;
	/**< Min horizontal rate in Hz                      */
	u8   u_min_horizontal_rate;
	/**< Max horizontal rate in Hz                      */
	u8   u_max_horizontal_rate;
	/**< Max suuported pixel clock rate in MHz          */
	u8   u_max_supported_pixel_clk;
} bsl_edid_second_md_t;

/*============================================================================*/
/**
 * Other monitor descriptor structure
 * */
typedef struct _bsl_edid_other_md_t {
	/**< true when parameters of struct are available   */
	bool    b_desc_record;
	/**< Other monitor Descriptor                       */
	u8   u_other_descriptor[EDID_MONITOR_DESCRIPTOR_SIZE];
} bsl_edid_other_md_t;

/**
 * \brief System function pointer type, to call user I2C read/write functions
 * \param slaveAddr     The I2C slave address
 * \param firstRegister The first device register address to read or write
 * \param lenData       Length of data to read or write (i.e. no. of registers)
 * \param pData         Pointer to data to write, or to buffer to receive data
 * \return              The call result:
 *                      - 0: the call was successful
 *                      - ERR_HDMI_I2C_WRITE: failed when writing
 *                      - ERR_HDMI_I2C_READ:  failed when reading
 * */
typedef struct _bsl_sys_args_t {
	u8 slave_addr;
	u8 first_register;
	u8 len_data;
	u8 *p_data;
} bsl_sys_args_t;
typedef error_code_t (FUNC_PTR *pbsl_sys_func_t)
(bsl_sys_args_t *p_sys_args);

/**
 * \brief System function pointer type, to call user I2C EDID read function
 * \param segPtrAddr    The EDID segment pointer address 0 to 7Fh
 * \param segPtr        The EDID segment pointer 0 to 7Fh
 * \param dataRegAddr   The EDID data register address 0 to 7Fh
 * \param wordOffset    The first word offset 0 to FFh to read
 * \param lenData       Length of data to read (i.e. number of registers),
 *                        1 to max starting at wordOffset
 * \param pData         Pointer to buffer to receive lenData data bytes
 * \return              The call result:
 *                      - 0: the call was successful
 *                      - ERR_HDMI_I2C_WRITE: failed when writing
 *                      - ERR_HDMI_I2C_READ:  failed when reading
 * */
typedef struct _bsl_sys_args_edid_t {
	u8 seg_ptr_addr;
	u8 seg_ptr;
	u8 data_reg_addr;
	u8 word_offset;
	u8 len_data;
	u8 *p_data;
} bsl_sys_args_edid_t;
typedef error_code_t (FUNC_PTR *pbsl_sys_func_edid_t)
(bsl_sys_args_edid_t *p_sys_args);

/*============================================================================*/
/*
 * Timer function pointer type, to call an application timer
 * Parameter ms: Delay in milliseconds required
 * */
typedef void(FUNC_PTR *pbsl_sys_func_timer_t)(u16 ms);

/*============================================================================*/
/*
 * Callback function pointer type, to call a user interrupt handler
 * Parameter txUnit: The transmitter unit that interrupted, 0 to max
 * */
typedef void(FUNC_PTR *pbsl_callback_t)(unit_select_t tx_unit);

/*============================================================================*/
/**
 * EIA/CEA-861B video format type
 * */
typedef enum {
	/**< Not a valid format...        */
	HDMITX_VFMT_NULL               = 0,
	/**< ...or no change required     */
	HDMITX_VFMT_NO_CHANGE          = 0,
	/**< Lowest valid format          */
	HDMITX_VFMT_MIN                = 1,
	/**< Lowest valid TV format       */
	HDMITX_VFMT_TV_MIN             = 1,
	/**< Format 01 640  x 480p  60Hz  */
	hdmitx_vfmt_01_640x480p_60hz   = 1,
	/**< Format 02 720  x 480p  60Hz  */
	hdmitx_vfmt_02_720x480p_60hz   = 2,
	/**< Format 03 720  x 480p  60Hz  */
	hdmitx_vfmt_03_720x480p_60hz   = 3,
	/**< Format 04 1280 x 720p  60Hz  */
	hdmitx_vfmt_04_1280x720p_60hz  = 4,
	/**< Format 05 1920 x 1080i 60Hz  */
	hdmitx_vfmt_05_1920x1080i_60hz = 5,
	/**< Format 06 720  x 480i  60Hz  */
	hdmitx_vfmt_06_720x480i_60hz   = 6,
	/**< Format 07 720  x 480i  60Hz  */
	hdmitx_vfmt_07_720x480i_60hz   = 7,
	/**< Format 08 720  x 240p  60Hz  */
	hdmitx_vfmt_08_720x240p_60hz   = 8,
	/**< Format 09 720  x 240p  60Hz  */
	hdmitx_vfmt_09_720x240p_60hz   = 9,
	/**< Format 10 720  x 480i  60Hz  */
	hdmitx_vfmt_10_720x480i_60hz   = 10,
	/**< Format 11 720  x 480i  60Hz  */
	hdmitx_vfmt_11_720x480i_60hz   = 11,
	/**< Format 12 720  x 240p  60Hz  */
	hdmitx_vfmt_12_720x240p_60hz   = 12,
	/**< Format 13 720  x 240p  60Hz  */
	hdmitx_vfmt_13_720x240p_60hz   = 13,
	/**< Format 14 1440 x 480p  60Hz  */
	hdmitx_vfmt_14_1440x480p_60hz  = 14,
	/**< Format 15 1440 x 480p  60Hz  */
	hdmitx_vfmt_15_1440x480p_60hz  = 15,
	/**< Format 16 1920 x 1080p 60Hz  */
	hdmitx_vfmt_16_1920x1080p_60hz = 16,
	/**< Format 17 720  x 576p  50Hz  */
	hdmitx_vfmt_17_720x576p_50hz   = 17,
	/**< Format 18 720  x 576p  50Hz  */
	hdmitx_vfmt_18_720x576p_50hz   = 18,
	/**< Format 19 1280 x 720p  50Hz  */
	hdmitx_vfmt_19_1280x720p_50hz  = 19,
	/**< Format 20 1920 x 1080i 50Hz  */
	hdmitx_vfmt_20_1920x1080i_50hz = 20,
	/**< Format 21 720  x 576i  50Hz  */
	hdmitx_vfmt_21_720x576i_50hz   = 21,
	/**< Format 22 720  x 576i  50Hz  */
	hdmitx_vfmt_22_720x576i_50hz   = 22,
	/**< Format 23 720  x 288p  50Hz  */
	hdmitx_vfmt_23_720x288p_50hz   = 23,
	/**< Format 24 720  x 288p  50Hz  */
	hdmitx_vfmt_24_720x288p_50hz   = 24,
	/**< Format 25 720  x 576i  50Hz  */
	hdmitx_vfmt_25_720x576i_50hz   = 25,
	/**< Format 26 720  x 576i  50Hz  */
	hdmitx_vfmt_26_720x576i_50hz   = 26,
	/**< Format 27 720  x 288p  50Hz  */
	hdmitx_vfmt_27_720x288p_50hz   = 27,
	/**< Format 28 720  x 288p  50Hz  */
	hdmitx_vfmt_28_720x288p_50hz   = 28,
	/**< Format 29 1440 x 576p  50Hz  */
	hdmitx_vfmt_29_1440x576p_50hz  = 29,
	/**< Format 30 1440 x 576p  50Hz  */
	hdmitx_vfmt_30_1440x576p_50hz  = 30,
	/**< Format 31 1920 x 1080p 50Hz  */
	hdmitx_vfmt_31_1920x1080p_50hz = 31,
	/**< Format 32 1920 x 1080p 24Hz  */
	hdmitx_vfmt_32_1920x1080p_24hz = 32,
	/**< Format 33 1920 x 1080p 25Hz  */
	hdmitx_vfmt_33_1920x1080p_25hz = 33,
	/**< Format 34 1920 x 1080p 30Hz  */
	hdmitx_vfmt_34_1920x1080p_30hz = 34,
	/**< Highest valid TV format      */
	HDMITX_VFMT_TV_MAX             = 34,
	/**< Lowest TV format without prefetched table */
	HDMITX_VFMT_TV_NO_REG_MIN	   = 32,
	/**< Number of TV formats & null  */
	HDMITX_VFMT_TV_NUM             = 35,
	/**< Lowest valid PC format       */
	HDMITX_VFMT_PC_MIN             = 128,
	/**< PC format 128                */
	hdmitx_vfmt_pc_640x480p_60hz   = 128,
	/**< PC format 129                */
	hdmitx_vfmt_pc_800x600p_60hz   = 129,
	/**< PC format 130                */
	hdmitx_vfmt_pc_1152x960p_60hz  = 130,
	/**< PC format 131                */
	hdmitx_vfmt_pc_1024x768p_60hz  = 131,
	/**< PC format 132                */
	hdmitx_vfmt_pc_1280x768p_60hz  = 132,
	/**< PC format 133                */
	hdmitx_vfmt_pc_1280x1024p_60hz = 133,
	/**< PC format 134                */
	hdmitx_vfmt_pc_1360x768p_60hz  = 134,
	/**< PC format 135                */
	hdmitx_vfmt_pc_1400x1050p_60hz  = 135,
	/**< PC format 136                */
	hdmitx_vfmt_pc_1600x1200p_60hz = 136,
	/**< PC format 137                */
	hdmitx_vfmt_pc_1024x768p_70hz  = 137,
	/**< PC format 138                */
	hdmitx_vfmt_pc_640x480p_72hz   = 138,
	/**< PC format 139                */
	hdmitx_vfmt_pc_800x600p_72hz   = 139,
	/**< PC format 140                */
	hdmitx_vfmt_pc_640x480p_75hz   = 140,
	/**< PC format 141                */
	hdmitx_vfmt_pc_1024x768p_75hz  = 141,
	/**< PC format 142                */
	hdmitx_vfmt_pc_800x600p_75hz   = 142,
	/**< PC format 143                */
	hdmitx_vfmt_pc_1024x864p_75hz  = 143,
	/**< PC format 144                */
	hdmitx_vfmt_pc_1280x1024p_75hz = 144,
	/**< PC format 145                */
	hdmitx_vfmt_pc_640x350p_85hz   = 145,
	/**< PC format 146                */
	hdmitx_vfmt_pc_640x400p_85hz   = 146,
	/**< PC format 147                */
	hdmitx_vfmt_pc_720x400p_85hz   = 147,
	/**< PC format 148                */
	hdmitx_vfmt_pc_640x480p_85hz   = 148,
	/**< PC format 149                */
	hdmitx_vfmt_pc_800x600p_85hz   = 149,
	/**< PC format 150                */
	hdmitx_vfmt_pc_1024x768p_85hz  = 150,
	/**< PC format 151                */
	hdmitx_vfmt_pc_1152x864p_85hz  = 151,
	/**< PC format 152                */
	hdmitx_vfmt_pc_1280x960p_85hz  = 152,
	/**< PC format 153                */
	hdmitx_vfmt_pc_1280x1024p_85hz = 153,
	/**< PC format 154                */
	hdmitx_vfmt_pc_1024x768i_87hz  = 154,
	/**< Highest valid PC format      */
	HDMITX_VFMT_PC_MAX             = 154,
	/**< Number of PC formats         */
	HDMITX_VFMT_PC_NUM     = (1 + 154 - 128)
} bsl_vid_fmt_t;

/*============================================================================*/
/*                       EXTERN DATA DEFINITIONS                              */
/*============================================================================*/

typedef enum {
	BSLHDMITX_UNKNOWN = 0x00, /**< IC/IP is not recognized */
	BSLHDMITX_TDA9984,        /**< IC is a TDA9984         */
	BSLHDMITX_TDA9989,        /**< IC is a TDA9989         */
	BSLHDMITX_LIPP4200,       /**< IP is a LIPP4200        */
	BSLHDMITX_TDA9981,        /**< IC is a TDA9981         */
	BSLHDMITX_TDA9983         /**< IC is a TDA9983         */
} bsl_version_t;

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*============================================================================*/
/**
 *    \brief      Reset the Clock Time Stamp generator in HDMI mode only
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 * */
error_code_t
bsl_audio_in_reset_cts
(
    unit_select_t      tx_unit
);

/*============================================================================*/
/**
 * bslAudioInSetConfig() parameter types
 * */
/** Audio input formats */
typedef enum {
	HDMITX_AFMT_SPDIF      = 0,         /**< SPDIF */
	HDMITX_AFMT_I2S        = 1,         /**< I2S   */
	HDMITX_AFMT_OBA        = 2,         /**< One bit audio */
	HDMITX_AFMT_DSD        = 3,         /**< DSD           */
	HDMITX_AFMT_INVALID    = 4          /**< Invalid format*/
} bsla_fmt_t;

/** DSD clock polarities */
typedef enum {
	HDMITX_CLKPOLDSD_ACLK       = 0,    /**< Same as ACLK */
	HDMITX_CLKPOLDSD_NACLK      = 1,    /**< Not ACLK, i.e. inverted */
	HDMITX_CLKPOLDSD_NO_CHANGE  = 2,    /**< No change    */
	HDMITX_CLKPOLDSD_INVALID    = 3     /**< Invalid      */
} bsl_clk_pol_dsd_t;

/** DSD data swap values */
typedef enum {
	HDMITX_SWAPDSD_OFF          = 0,    /**< No swap   */
	HDMITX_SWAPDSD_ON           = 1,    /**< Swap      */
	HDMITX_SWAPDSD_NO_CHANGE    = 2,    /**< No change */
	HDMITX_SWAPDSD_INVALID      = 3     /**< Invalid   */
} bsl_swap_dsd_t;

/** I2S and DSD channel allocation values */
enum _bsl_chan {
	HDMITX_CHAN_MIN        = 0,
	HDMITX_CHAN_MAX        = 31,
	HDMITX_CHAN_NO_CHANGE  = 32,
	HDMITX_CHAN_INVALID    = 33
};

/** Audio layout values */
enum _bsl_layout {
	HDMITX_LAYOUT_MIN       = 0,
	HDMITX_LAYOUT_MAX       = 1,
	HDMITX_LAYOUT_NO_CHANGE = 2,
	HDMITX_LAYOUT_INVALID   = 3
};

/** Audio FIFO read latency values */
enum _bsllatency_rd {
	HDMITX_LATENCY_MIN         = 0x000,
	HDMITX_LATENCY_MAX         = 0x0FF,
	HDMITX_LATENCY_NO_CHANGE   = 0x100,
	HDMITX_LATENCY_INVALID     = 0x101
};

/**
 *    \brief      Set audio input configuration in HDMI mode only
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  aFmt        Audio input format
 *    \param[in]  chanI2s     I2S channel allocation
 *    \param[in]  chanDsd     DSD channel allocation
 *    \param[in]  clkPolDsd   DSD clock polarity
 *    \param[in]  swapDsd     DSD data swap
 *    \param[in]  layout      Sample layout
 *    \param[in]  latency_rd  Audio FIFO read latency
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 * */
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
    u16                   latency_rd
);

/*============================================================================*/
/**
 * bslAudioInSetCts() parameter types
 * */
/** Clock Time Stamp reference source */
typedef enum {
	HDMITX_CTSREF_ACLK      = 0,    /**< Clock input pin for I2S       */
	HDMITX_CTSREF_MCLK      = 1,    /**< Clock input pin for EXTREF    */
	HDMITX_CTSREF_FS64SPDIF = 2,    /**< 64xsample rate, for SPDIF     */
	HDMITX_CTSREF_INVALID   = 3     /**< Invalid value                 */
} bslcts_ref_t;

/** Audio sample rate kHz indexes */
typedef enum {
	hdmitx_afs_32k           = 0,     /**< 32kHz    */
	hdmitx_afs_44_1k         = 1,     /**< 44.1kHz  */
	HDMITX_AFS_48K           = 2,     /**< 48kHz    */
	HDMITX_AFS_88_2K         = 3,     /**< 88.2kHz  */
	HDMITX_AFS_96K           = 4,     /**< 96kHz    */
	HDMITX_AFS_176_4K        = 5,     /**< 176.4kHz */
	HDMITX_AFS_192K          = 6,     /**< 192kHz   */
	HDMITX_AFS_NOT_INDICATED = 7,     /**< Not Indicated (Channel Status) */
	HDMITX_AFS_INVALID       = 7,     /**< Invalid  */
	HDMITX_AFS_NUM           = 7      /**< # rates  */
} bslafs_t;

/** Vertical output frequencies */
typedef enum {
	hdmitx_vfreq_24hz      = 0,     /**< 24Hz          */
	hdmitx_vfreq_25hz      = 1,     /**< 25Hz          */
	hdmitx_vfreq_30hz      = 2,     /**< 30Hz          */
	hdmitx_vfreq_50hz      = 3,     /**< 50Hz         0 */
	hdmitx_vfreq_59hz      = 4,     /**< 59.94Hz      1 */
	hdmitx_vfreq_60hz      = 5,     /**< 60Hz         2 */
#ifndef FORMAT_PC
	HDMITX_VFREQ_INVALID   = 6,     /**< Invalid       */
	HDMITX_VFREQ_NUM       = 6      /**< No. of values */
#else /* FORMAT_PC */
	hdmitx_vfreq_70hz      = 6,     /**< 70Hz          */
	hdmitx_vfreq_72hz      = 7,     /**< 72Hz          */
	hdmitx_vfreq_75hz      = 8,     /**< 75Hz          */
	hdmitx_vfreq_85hz      = 9,     /**< 85Hz          */
	hdmitx_vfreq_87hz      = 10,     /**< 87Hz          */
	HDMITX_VFREQ_INVALID   = 11,     /**< Invalid       */
	HDMITX_VFREQ_NUM       = 11      /**< No. of values */
#endif /* FORMAT_PC */
} bsl_vfreq_t;

/** Clock Time Stamp predivider - scales N */
typedef enum {
	HDMITX_CTSK1           = 0,     /**< k=1 */
	HDMITX_CTSK2           = 1,     /**< k=2 */
	HDMITX_CTSK3           = 2,     /**< k=3 */
	HDMITX_CTSK4           = 3,     /**< k=4 */
	HDMITX_CTSK8           = 4,     /**< k=8 */
	HDMITX_CTSK_USE_CTSX   = 5,     /**< Calculate from ctsX factor */
	HDMITX_CTSK_INVALID    = 6      /**< Invalid */
} bslcts_k_t;

/** Clock Time Stamp postdivider measured time stamp */
typedef enum {
	HDMITX_CTSMTS          = 0,     /**< =mts   */
	HDMITX_CTSMTS2         = 1,     /**< =mts%2 */
	HDMITX_CTSMTS4         = 2,     /**< =mts%4 */
	HDMITX_CTSMTS8         = 3,     /**< =mts%8 */
	HDMITX_CTSMTS_USE_CTSX = 4,     /**< Calculate from ctsX factor */
	HDMITX_CTSMTS_INVALID  = 5      /**< Invalid */
} bslcts_m_t;

/** Cycle Time Stamp values */
enum _bsl_cts {
	HDMITX_CTS_AUTO        = 0,
	HDMITX_CTS_MIN         = 0x000001
};

/** Cycle Time Stamp X factors */
enum _bsl_cts_x {
	HDMITX_CTSX_16         = 0,
	HDMITX_CTSX_32         = 1,
	HDMITX_CTSX_48         = 2,
	HDMITX_CTSX_64         = 3,
	HDMITX_CTSX_128        = 4,
	HDMITX_CTSX_NUM        = 5,
	/**< CTX value unused when K and Mts used */
	HDMITX_CTSX_UNUSED     = 5,
	HDMITX_CTSX_INVALID    = 6
};

/**
 *    \brief      Set the Clock Time Stamp generator in HDMI mode only
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  ctsRef      Clock Time Stamp reference source
 *    \param[in]  afs         Audio input sample frequency
 *    \param[in]  voutFmt     Video output format
 *    \param[in]  voutFreq    Vertical output frequency
 *    \param[in]  uCts        Manual Cycle Time Stamp
 *    \param[in]  uCtsX       Clock Time Stamp factor x
 *    \param[in]  ctsK        Clock Time Stamp predivider k
 *    \param[in]  ctsM        Clock Time Stamp postdivider m
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 * */
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
);

/*============================================================================*/
/**
 * bslAudioOutSetChanStatus() parameter types
 * */
/** BYTE 0: Channel Status Format information */
typedef enum {
	/**< PCM 2 channels without pre-emphasis           */
	HDMITX_CSFI_PCM_2CHAN_NO_PRE    = 0,
	/**< PCM 2 channels with 50us/15us pre-emphasis    */
	HDMITX_CSFI_PCM_2CHAN_PRE       = 1,
	/**< PCM Reserved for 2 channels with pre-emphasis */
	HDMITX_CSFI_PCM_2CHAN_PRE_RSVD1 = 2,
	/**< PCM Reserved for 2 channels with pre-emphasis */
	HDMITX_CSFI_PCM_2CHAN_PRE_RSVD2 = 3,
	/**< Non-PCM Default state                         */
	HDMITX_CSFI_NOTPCM_DEFAULT      = 4,
	/**< Invalid value                                 */
	HDMITX_CSFI_INVALID             = 5
} bsl_csformat_info_t;

/** BYTE 0: Channel Status Copyright assertion */
typedef enum {
	HDMITX_CSCOPYRIGHT_PROTECTED   = 0,  /**< Copyright protected     */
	HDMITX_CSCOPYRIGHT_UNPROTECTED = 1,  /**< Not copyright protected */
	HDMITX_CSCOPYRIGHT_INVALID     = 2   /**< Invalid value           */
} bsl_cscopyright_t;

/** BYTE 3: Channel Status Clock Accuracy */
typedef enum {
	HDMITX_CSCLK_LEVEL_II    = 0,     /**< Level II                     */
	HDMITX_CSCLK_LEVEL_I     = 1,     /**< Level I                      */
	HDMITX_CSCLK_LEVEL_III   = 2,     /**< Level III                    */
	HDMITX_CSCLK_NOT_MATCHED = 3,     /**< Not matched to sample freq.  */
	HDMITX_CSCLK_INVALID     = 4      /**< Invalid                      */
} bsl_csclk_acc_t;

/** BYTE 4: Channel Status Maximum sample word length */
typedef enum {
	HDMITX_CSMAX_LENGTH_20   = 0,  /**< Max word length is 20 bits   */
	HDMITX_CSMAX_LENGTH_24   = 1,  /**< Max word length is 24 bits   */
	HDMITX_CSMAX_INVALID     = 2   /**< Invalid value                */
} bsl_csmax_word_length_t;

/** BYTE 4: Channel Status Sample word length */
typedef enum {
	/**< Word length is not indicated                    */
	HDMITX_CSWORD_DEFAULT          = 0,
	/**< Sample length is 20 bits out of max 24 possible */
	HDMITX_CSWORD_20_OF_24         = 1,
	/**< Sample length is 16 bits out of max 20 possible */
	HDMITX_CSWORD_16_OF_20         = 1,
	/**< Sample length is 22 bits out of max 24 possible */
	HDMITX_CSWORD_22_OF_24         = 2,
	/**< Sample length is 18 bits out of max 20 possible */
	HDMITX_CSWORD_18_OF_20         = 2,
	/**< Reserved - shall not be used */
	HDMITX_CSWORD_RESVD            = 3,
	/**< Sample length is 23 bits out of max 24 possible */
	HDMITX_CSWORD_23_OF_24         = 4,
	/**< Sample length is 19 bits out of max 20 possible */
	HDMITX_CSWORD_19_OF_20         = 4,
	/**< Sample length is 24 bits out of max 24 possible */
	HDMITX_CSWORD_24_OF_24         = 5,
	/**< Sample length is 20 bits out of max 20 possible */
	HDMITX_CSWORD_20_OF_20         = 5,
	/**< Sample length is 21 bits out of max 24 possible */
	HDMITX_CSWORD_21_OF_24         = 6,
	/**< Sample length is 17 bits out of max 20 possible */
	HDMITX_CSWORD_17_OF_20         = 6,
	HDMITX_CSWORD_INVALID          = 7      /**< Invalid */
} bsl_csword_length_t;

/** BYTE 4: Channel Status Original sample frequency */
typedef enum {
	HDMITX_CSOFREQ_NOT_INDICATED = 0,   /**< Not Indicated */
	hdmitx_csofreq_192k          = 1,   /**< 192kHz        */
	hdmitx_csofreq_12k           = 2,   /**< 12kHz         */
	hdmitx_csofreq_176_4k        = 3,   /**< 176.4kHz      */
	HDMITX_CSOFREQ_RSVD1         = 4,   /**< Reserved      */
	hdmitx_csofreq_96k           = 5,   /**< 96kHz         */
	hdmitx_csofreq_8k            = 6,   /**< 8kHz          */
	hdmitx_csofreq_88_2k         = 7,   /**< 88.2kHz       */
	hdmitx_csofreq_16k           = 8,   /**< 16kHz         */
	hdmitx_csofreq_24k           = 9,   /**< 24kHz         */
	hdmitx_csofreq_11_025k       = 10,  /**< 11.025kHz     */
	hdmitx_csofreq_22_05k        = 11,  /**< 22.05kHz      */
	hdmitx_csofreq_32k           = 12,  /**< 32kHz         */
	hdmitx_csofreq_48k           = 13,  /**< 48kHz         */
	HDMITX_CSOFREQ_RSVD2         = 14,  /**< Reserved      */
	hdmitx_csofreq_44_1k         = 15,  /**< 44.1kHz       */
	HDMITX_CSAFS_INVALID         = 16   /**< Invalid value */
} bsl_csorig_afs_t;

/**
 *    \brief      Set the Channel Status Bytes 0,1,3 & 4
 *
 *    \param[in]  txUnit              Transmitter unit number
 *    \param[in]  copyright           Byte 0 Copyright bit (bit2)
 *    \param[in]  formatInfo          Byte 0 Audio sample format (bit1) and additional info (bit345)
 *    \param[in]  categoryCode        Byte 1 Category code (bits8-15)
 *    \param[in]  sampleFreq          Byte 3 Sample Frequency (bits24-27)
 *    \param[in]  clockAccuracy       Byte 3 Clock Accuracy (bits38-31)
 *    \param[in]  maxWordLength       Byte 4 Maximum word length (bit32)
 *    \param[in]  wordLength          Byte 4 Word length (bits33-35)
 *    \param[in]  origSampleFreq      Byte 4 Original Sample Frequency (bits36-39)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 *
 *    \note       The consumer use bit (bit0) and Mode bits (bits6-7) are forced to zero.
 *                Use bslAudioOutSetChanStatusMapping to set CS Byte 2.
 *
 * */
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
);

/*============================================================================*/
/**
 * bslAudioOutSetChanStatusMapping() parameter types
 * */
/** Channel Status source/channel number limits */
enum _bsl_chan_status_chan_limits {
	HDMITX_CS_CHANNELS_MAX  = 0x0F,
	HDMITX_CS_SOURCES_MAX   = 0x0F
};

/**
 *    \brief      Set the Channel Status Byte2 for Audio Port 0
 *
 *    \param[in]  txUnit              Transmitter unit number
 *    \param[in]  sourceLeft          L Source Number: 0 don't take into account, 1-15
 *    \param[in]  channelLeft         L Channel Number: 0 don't take into account, 1-15
 *    \param[in]  sourceRight         R Source Number: 0 don't take into account, 1-15
 *    \param[in]  channelRight        R Channel Number: 0 don't take into account, 1-15
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 *
 *    \note       Use bslAudioOutSetChanStatus to set all other CS bytes
 *                This function only sets the mapping for Audio Port 0.
 *
 * */
error_code_t
bsl_audio_out_set_chan_status_mapping
(
    unit_select_t  tx_unit,
    u8           source_left,
    u8           channel_left,
    u8           source_right,
    u8           channel_right
);

/*============================================================================*/
/**
 * bslAudioOutSetMute() parameter type
 * */
typedef enum {
	HDMITX_AMUTE_OFF       = 0,    /**< Mute off */
	HDMITX_AMUTE_ON        = 1,    /**< Mute on  */
	HDMITX_AMUTE_INVALID   = 2     /**< Invalid  */
} bsla_mute_t;

/**
 *    \brief      Mute or un-mute the audio output by controlling the audio FIFO,
 *                in HDMI mode only
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  aMute       Audio mute: On, Off
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: in DVI mode
 *
 *    \note       bslPktSetGeneralCntrl must be used to control the audio
 *                mute in outgoing data island packets
 *
 * */
error_code_t
bsl_audio_out_set_mute
(
    unit_select_t      tx_unit,
    bsla_mute_t a_mute
);

/*============================================================================*/
/**
 *    \brief      Disable an HDMI Transmitter output and destroy its driver
 *                instance
 *
 *    \param[in]  txUnit Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: the unit is not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_deinit
(
    unit_select_t      tx_unit
);

/*============================================================================*/
/**
 * \brief The bslEdidGetAudioCapabilities() parameter type
 * */
typedef struct _bsl_edid_sad_t {
	u8   mode_chans;  /* Bits[6:3]: EIA/CEA861 mode; Bits[2:0]: channels */
	u8   freqs;      /* Bits for each supported frequency */
	u8   byte3;      /* EIA/CEA861B p83: data depending on audio mode */
} bsl_edid_sad_t;

/** Number of 3 byte Short Audio Descriptors stored in pEdidAFmts */
#define HDMI_TX_SAD_MAX_CNT     10

/**
 *    \brief      Get supported audio format(s) from previously read EDID
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pEdidAFmts  Pointer to the array of structures to receive the
 *                            supported Short Audio Descriptors
 *    \param[in]  aFmtLength  Number of SADs supported in buffer pEdidAFmts,
 *                            up to HDMI_TX_SAD_MAX_CNT
 *    \param[out] pAFmtsAvail Pointer to receive the number of SADs available
 *    \param[out] pAudioFlags Pointer to the byte to receive the Audio Capability
 *                            Flags
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \note \verbatim
 *                Supported Short Audio Descriptors array:
 *                EdidAFmts[0].ModeChans      SAD 1  - Mode byte
 *                EdidAFmts[0].Freqs	        SAD 1  - Frequencies byte
 *                EdidAFmts[0].Byte3          SAD 1  - Byte 3
 *                ...
 *                EdidAFmts[n-1].ModeChans    SAD n  - Mode byte
 *                EdidAFmts[n-1].Freqs	    SAD n  - Frequencies byte
 *                EdidAFmts[n-1].Byte3        SAD n  - Byte 3
 *                (Where n is the smaller of aFmtLength and pAFmtAvail)
 *    \endverbatim
 * */
error_code_t
bsl_edid_get_audio_capabilities
(
    unit_select_t          tx_unit,
    bsl_edid_sad_t    *p_edid_afmts,
    uint                    a_fmt_length,
    uint                    *p_afmts_avail,
    u8                   *p_audio_flags
);

/*============================================================================*/
/**
 *    \brief      Get the EDID block count
 *
 *    \param[in]  txUnit              Transmitter unit number
 *    \param[out] puEdidBlockCount    Pointer to data byte in which to return the
 *                                    block count
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 * */
error_code_t
bsl_edid_get_block_count
(
    unit_select_t  tx_unit,
    u8           *pu_edid_block_count
);

/*============================================================================*/
/**
 * bslEdidGetBlockData() parameter types
 * */
/** An enum to represent the current EDID status */
enum _bsl_edid_sta_t {
	HDMITX_EDID_READ                = 0,
	HDMITX_EDID_READ_INCOMPLETE     = 1,
	HDMITX_EDID_ERROR               = 2,
	HDMITX_EDID_ERROR_INCOMPLETE    = 3,
	HDMITX_EDID_NOT_READ            = 4};

/**
 *    \brief      Get raw EDID blocks from the sink device via DDC
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pRawEdid    Pointer to a buffer supplied by the caller to accept
 *                            the raw EDID data
 *    \param[in]  numBlocks   Number of blocks to read
 *    \param[in]  lenRawEdid  Length in bytes of the supplied buffer
 *    \param[out] pEdidStatus Pointer to status value E_EDID_READ or E_EDID_ERROR
 *                            valid only when the return value is 0
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \note       NA
 *
 *    \sa         bslEdidGetVideoPreferred,
 *                bslEdidGetVideoCapabilities
 * */
error_code_t
bsl_edid_get_block_data
(
    unit_select_t          tx_unit,
    u8                   *p_raw_edid,
    int                     num_blocks,
    int                     len_raw_edid,
    u8                   *p_edid_status
);

/*============================================================================*/
/**
 * bslEdidGetSinkType() parameter types
 * */
/** Sink device type */
typedef enum {
	HDMITX_SINK_DVI                = 0,    /**< DVI  */
	HDMITX_SINK_HDMI               = 1,    /**< HDMI */
	HDMITX_SINK_EDID               = 2,    /**< As currently defined in EDID */
	HDMITX_SINK_INVALID            = 3     /**< Invalid   */
} bsl_sink_type_t;

/**
 *    \brief      Get Sink Type by analysis of EDID content
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pSinkType   Pointer to returned Sink Type: DVI or HDMI
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \sa         bslEdidGetBlockData
 * */
error_code_t
bsl_edid_get_sink_type
(
    unit_select_t              tx_unit,
    bsl_sink_type_t      *p_sink_type
);

/*============================================================================*/
/**
 *    \brief      Get Source Physical Address by analysis of EDID content
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pSourceAddress  Pointer to returned Source Physical Address (ABCDh)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \sa         bslEdidGetBlockData
 * */
error_code_t
bsl_edid_get_source_address
(
    unit_select_t  tx_unit,
    u16          *p_source_address
);

/*============================================================================*/
/**
 *  bslEdidGetVideoCapabilities() parameter types
 * */
/** Number of 1 byte Short Video Descriptors stored in pEdidVFmts */
#define HDMI_TX_SVD_MAX_CNT     30

/** Flag set in Short Video Descriptor to indicate native format */
#define HDMI_TX_SVD_NATIVE_MASK 0x80
#define HDMI_TX_SVD_NATIVE_NOT  0x7F

/** Video capability flags */
enum _bsl_vid_cap_t {
	HDMITX_VIDCAP_UNDERSCAN   = 0x80,   /**< Underscan supported */
	HDMITX_VIDCAP_YUV444      = 0x40,   /**< YCbCr 4:4:4 supported */
	HDMITX_VIDCAP_YUV422      = 0x20,   /**< YCbCr 4:2:2 supported */
	HDMITX_VIDCAP_UNUSED      = 0x1F    /**< Unused flags */
};

/**
 *    \brief      Get supported video format(s) from previously read EDID
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pEdidVFmts  Pointer to the array to receive the supported Short
 *                            Video Descriptors
 *    \param[in]  vFmtLength  Number of SVDs supported in buffer pEdidVFmts,
 *                            up to HDMI_TX_SVD_MAX_CNT
 *    \param[out] pVFmtsAvail Pointer to receive the number of SVDs available
 *    \param[out] pVidFlags   Ptr to the byte to receive Video Capability Flags
 *                            b7: underscan supported
 *                            b6: YCbCr 4:4:4 supported
 *                            b5: YCbCr 4:2:2 supported
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \note \verbatim
 *                Supported Short Video Descriptors array:
 *                    (HDMI_TX_SVD_NATIVE_MASK bit set to indicate native format)
 *                EdidVFmts[0]   EIA/CEA Short Video Descriptor 1, or 0
 *                ...
 *                EdidVFmts[n-1]  EIA/CEA Short Video Descriptor 32, or 0
 *                (Where n is the smaller of vFmtLength and pVFmtAvail)
 *    \endverbatim
 *    \sa         bslEdidGetBlockData
 * */
error_code_t
bsl_edid_get_video_capabilities
(
    unit_select_t  tx_unit,
    u8           *p_edid_vfmts,
    uint            v_fmt_length,
    uint            *p_vfmts_avail,
    u8           *p_vid_flags
);

/*============================================================================*/
/**
 * \brief The bslEdidGetVideoPreferred() parameter type
 * */
typedef struct _bsl_edid_dtd_t {
	u16  u_pixel_clock;        /**< Pixel Clock/10,000         */
	u16  u_hactive_pixels;     /**< Horizontal Active Pixels   */
	u16  u_hblank_pixels;      /**< Horizontal Blanking Pixels */
	u16  u_vactive_lines;      /**< Vertical Active Lines      */
	u16  u_vblank_lines;       /**< Vertical Blanking Lines    */
	u16  u_hsync_offset;       /**< Horizontal Sync Offset     */
	u16  u_hsync_width;        /**< Horiz. Sync Pulse Width    */
	u16  u_vsync_offset;       /**< Vertical Sync Offset       */
	u16  u_vsync_width;        /**< Vertical Sync Pulse Width  */
	u16  u_himage_size;        /**< Horizontal Image Size      */
	u16  u_vimage_size;        /**< Vertical Image Size        */
	u16  u_hborder_pixels;     /**< Horizontal Border          */
	u16  u_vborder_pixels;     /**< Vertical Border            */
	u8   flags;              /**< Interlace/sync info        */
} bsl_edid_dtd_t;

/**
 *    \brief      Get preferred video format from previously read EDID
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pEdidDTD    Pointer to the structure to receive the Detailed
 *                            Timing Descriptor parameters of the preferred video
 *                            format
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_INVALID_STATE: EDID checksum failure
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NULL_CONNECTION: HPD pin is inactive
 *
 *    \note \verbatim
 *                Detailed Timing Descriptor parameters output structure:
 *                u16 uPixelClock      Pixel Clock (MHz/10,000)
 *                u16 uHActivePixels   Horizontal Active Pixels
 *                u16 uHBlankPixels    Horizontal Blanking Pixels
 *                u16 uVActiveLines    Vertical Active Lines
 *                u16 uVBlankLines     Vertical Blanking Lines
 *                u16 uHSyncOffset     Horizontal Sync Offset (Pixels)
 *                u16 uHSyncWidth      Horizontal Sync Pulse Width (Pixels)
 *                u16 uVSyncOffset     Vertical Sync Offset (Lines)
 *                u16 uVSyncWidth      Vertical Sync Pulse Width (Lines)
 *                u16 uHImageSize      Horizontal Image Size (mm)
 *                u16 uVImageSize      Vertical Image Size (mm)
 *                u16 uHBorderPixels   Horizontal Border (Pixels)
 *                u16 uVborderPixels   Vertical Border (Pixels)
 *                u8 Flags             Interlace/sync info
 *    \endverbatim
 *    \sa         bslEdidGetBlockData
 * */
error_code_t
bsl_edid_get_video_preferred
(
    unit_select_t          tx_unit,
    bsl_edid_dtd_t    *p_edid_dtd
);

/**
 *    \brief      Get detailed timing descriptor from previously read EDID
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pEdidDTD    Pointer to the array to receive the Detailed timing descriptor
 *
 *    \param[in]  nb_size     Number of DTD supported in buffer pEdidDTD
 *
 *    \param[out] pDTDAvail Pointer to receive the number of DTD available
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_INVALID_STATE: EDID not read
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_edid_get_detailed_timing_descriptors
(
    unit_select_t          tx_unit,
    bsl_edid_dtd_t    *p_edid_dtd,
    u8                   nb_size,
    u8                   *p_dtda_vail
);

/*============================================================================*/
/**
 * bslHdcpCheck() parameter type
 * */
/** HDCP check result */
typedef enum {
	HDMITX_HDCP_CHECK_NOT_STARTED       = 0,    /**< Check not started */
	HDMITX_HDCP_CHECK_IN_PROGRESS       = 1,    /**< No failures, more to do */
	HDMITX_HDCP_CHECK_PASS              = 2,    /**< Final check has passed */
	/**< First check failure code */
	HDMITX_HDCP_CHECK_FAIL_FIRST        = 3,
	/**< Driver not AUTHENTICATED */
	HDMITX_HDCP_CHECK_FAIL_DRIVER_STATE = 3,
	HDMITX_HDCP_CHECK_FAIL_DEVICE_T0    = 4,    /**< A T0 interrupt occurred */
	HDMITX_HDCP_CHECK_FAIL_DEVICE_RI    = 5,    /**< Device RI changed */
	HDMITX_HDCP_CHECK_FAIL_DEVICE_FSM   = 6,    /**< Device FSM not 10h */
	HDMITX_HDCP_CHECK_NUM               = 7     /**< Number of check results */
} bsl_hdcp_check_t;

/**
 *    \brief      Check the result of an HDCP encryption attempt, called at
 *                intervals (set by uTimeSinceLastCallMs) after bslHdcpRun
 *
 *    \param[in]  txUnit                  Transmitter unit number
 *    \param[in]  uTimeSinceLastCallMs    Time in ms since this was last called
 *    \param[out] pResult                 The outcome of the check
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 * */
error_code_t
bsl_hdcp_check
(
    unit_select_t	        tx_unit,
    u16                  u_time_since_last_call_ms,
    bsl_hdcp_check_t  *p_result
);

/*============================================================================*/
/**
 * bslHdcpConfigure() parameter type
 * */
/** HDCP DDC slave addresses */
enum _bsl_hdcp_slave_address {
	HDMITX_HDCP_SLAVE_PRIMARY    = 0x74,
	HDMITX_HDCP_SLAVE_SECONDARY  = 0x76
};

/** HDCP transmitter modes */
typedef enum {
	HDMITX_HDCP_TXMODE_NOT_SET   = 0,
	HDMITX_HDCP_TXMODE_REPEATER  = 1,
	HDMITX_HDCP_TXMODE_TOP_LEVEL = 2,
	HDMITX_HDCP_TXMODE_MAX       = 2
} bsl_hdcp_tx_mode_t;

/** HDCP option flags */
typedef enum {
	/* Not set: obey PJ result     */
	HDMITX_HDCP_OPTION_FORCE_PJ_IGNORED = 0x01,
	/* Not set: obey BCAPS setting */
	HDMITX_HDCP_OPTION_FORCE_SLOW_DDC   = 0x02,
	/* Not set: obey BCAPS setting */
	HDMITX_HDCP_OPTION_FORCE_NO_1_1     = 0x04,
	/* Not set: obey BCAPS setting */
	HDMITX_HDCP_OPTION_FORCE_REPEATER   = 0x08,
	/* Not set: obey BCAPS setting */
	HDMITX_HDCP_OPTION_FORCE_NO_REPEATER = 0x10,
	/* Not set: obey V=V' result   */
	HDMITX_HDCP_OPTION_FORCE_V_EQU_VBAR = 0x20,
	HDMITX_HDCP_OPTION_FORCE_VSLOW_DDC  = 0x40,/* Set: 50kHz DDC */
	HDMITX_HDCP_OPTION_DEFAULT          = 0x00,/* All the above Not Set vals */
	/* Only these bits are allowed */
	HDMITX_HDCP_OPTION_MASK             = 0x7F,
	/* These bits are not allowed  */
	HDMITX_HDCP_OPTION_MASK_BAD         = 0x80
} bsl_hdcp_options_t;

/**
 *    \brief      Configure various HDCP parameters
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  slaveAddress    DDC I2C slave address
 *    \param[in]  txMode          Mode of our transmitter device
 *    \param[in]  options         Options flags to control behaviour of HDCP
 *    \param[in]  uCheckIntervalMs HDCP check interval in milliseconds
 *    \param[in]  uChecksToDo     Number of HDCP checks to do after HDCP starts
 *                                A value of 2 or more is valid for checking
 *                                May be set to 0 to disabling checking
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 *    \note       Must be called before all other HDCP APIs
 * */
error_code_t
bsl_hdcp_configure
(
    unit_select_t	         tx_unit,
    u8                    slave_address,
    bsl_hdcp_tx_mode_t  tx_mode,
    bsl_hdcp_options_t options,
    u16                   u_check_interval_ms,
    u8                    u_checks_to_do
);

/*============================================================================*/
/**
 * bslHdcpDownloadKeys() parameter type
 * */
/** HDCP decryption mode */
typedef enum {
	HDMITX_HDCP_DECRYPT_DISABLE  = 0,
	HDMITX_HDCP_DECRYPT_ENABLE   = 1,
	HDMITX_HDCP_DECRYPT_MAX      = 1
} bsl_decrypt_t;

/**
 *    \brief      Download keys and AKSV data from OTP memory to the device
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  seed            Seed value
 *    \param[in]  keyDecryption   State of key decryption 0 to 1 (disabled, enabled)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 * */
error_code_t
bsl_hdcp_download_keys
(
    unit_select_t	        tx_unit,
    u16                  seed,
    bsl_decrypt_t    key_decryption
);

/*============================================================================*/
/**
 *    \brief      Switch HDCP encryption on or off without disturbing Infoframes
 *                (Not normally used)
 *
 *    \param[in]  txUnit  Transmitter unit number
 *    \param[in]  bOn     Encryption state: 1=on, 0=off
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 * */
error_code_t
bsl_hdcp_encryption_on
(
    unit_select_t	tx_unit,
    bool            b_on
);

/*============================================================================*/
/**
 *    \brief      Get HDCP OTP registers
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  otpAddress  OTP start address 0-FF
 *    \param[out] pOtpData    Ptr to a three-byte array to hold the data read:
 *                            [0] = OTP_DATA_MSB
 *                            [1] = OTP_DATA_ISB
 *                            [2] = OTP_DATA_LSB
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 * */
error_code_t
bsl_hdcp_get_otp
(
    unit_select_t	        tx_unit,
    u8                   otp_address,
    u8                   *p_otp_data
);

/*============================================================================*/
/**
 *    \brief      Return the failure state that caused the last T0 interrupt
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pFailState  Ptr to the unit's last T0 fail state
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 * */
error_code_t
bsl_hdcp_get_t0fail_state
(
    unit_select_t	tx_unit,
    u8           *p_fail_state
);

/*============================================================================*/
/**
 *    \brief      Handle BCAPS interrupt
 *
 *    \param[in]  txUnit  Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       The user BCAPS interrupt handler (registered with
 *                bslInit) calls this API before calling
 *                bslHdcpHandleBKSV
 * */
error_code_t
bsl_hdcp_handle_bcaps
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 *    \brief      Read BKSV registers
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pBksv           Pointer to 5-byte BKSV array returned to caller
 *                                (1st byte is MSB)
 *    \param[out] pbCheckRequired Pointer to a result variable to tell the caller
 *                                whether to check for BKSV in a revocation list:
 *                                0 or 1 (check not required, check required)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       The user BCAPS interrupt handler (registered with
 *                bslInit) calls this API after calling
 *                bslHdcpHandleBCAPS
 * */
error_code_t
bsl_hdcp_handle_bksv
(
    unit_select_t	tx_unit,
    u8           *p_bksv,
    bool            *pb_check_required
);

/*============================================================================*/
/**
 *    \brief      Declare BKSV result to be secure or not secure
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  bSecure         Result of user's check of BKSV against a
 *                                revocation list:
 *                                0 (not secure: BKSV found in revocation list)
 *                                1 (secure: BKSV not found in revocation list)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       The user BCAPS interrupt handler (registered with
 *                bslInit) calls this API after calling
 *                bslHdcpHandleBKSV
 * */
error_code_t
bsl_hdcp_handle_bksvresult
(
    unit_select_t	tx_unit,
    bool            b_secure
);

/*============================================================================*/
/**
 * bslHdcpHandleBSTATUS() parameter type
 * */
/** BSTATUS bit fields */
enum _bsl_hdcp_handle_bstatus {
	HDMITX_HDCP_BSTATUS_HDMI_MODE               = 0x1000,
	HDMITX_HDCP_BSTATUS_MAX_CASCADE_EXCEEDED    = 0x0800,
	HDMITX_HDCP_BSTATUS_CASCADE_DEPTH           = 0x0700,
	HDMITX_HDCP_BSTATUS_MAX_DEVS_EXCEEDED       = 0x0080,
	HDMITX_HDCP_BSTATUS_DEVICE_COUNT            = 0x007F
};

/**
 *    \brief      Handle BSTATUS interrupt
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pBstatus    Pointer to 16-bit BSTATUS value returned to caller
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       Called by user's BSTATUS interrupt handler registered with
 *                bslInit
 * */
error_code_t
bsl_hdcp_handle_bstatus
(
    unit_select_t	tx_unit,
    u16          *p_bstatus
);

/*============================================================================*/
/**
 *    \brief      Handle ENCRYPT interrupt
 *
 *    \param[in]  txUnit  Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       Called by user's ENCRYPT interrupt handler registered with
 *                bslInit
 * */
error_code_t
bsl_hdcp_handle_encrypt
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle PJ interrupt
 *
 *    \param[in]  txUnit  Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       Called by user's PJ interrupt handler registered with
 *                bslInit
 * */
error_code_t
bsl_hdcp_handle_pj
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 * bslHdcpHandleSHA_1() parameter types
 * */

/** KSV list sizes */
enum _bsl_hdcp_handle_sha_1 {
	HDMITX_KSV_LIST_MAX_DEVICES = 128,
	HDMITX_KSV_BYTES_PER_DEVICE = 5
};

/**
 *    \brief      Handle SHA-1 interrupt
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  maxKsvDevices   Maximum number of 5-byte devices that will fit
 *                                in *pKsvList: 0 to 128 devices
 *                                If 0, no KSV read is done and it is treated as
 *                                secure
 *    \param[out] pKsvList        Pointer to KSV list array supplied by caller:
 *                                Sets of 5-byte KSVs, 1 per device, 1st byte is
 *                                LSB of 1st device
 *                                May be null if maxKsvDevices is 0
 *    \param[out] pnKsvDevices    Pointer to number of KSV devices copied to
 *p_ksv_list: 0 to 128
 *                                If 0, no KSV check is needed and it is treated
 *                                as secure
 *                                May be null if maxKsvDevices is 0
 *
 *    \param[out] pDepth          Connection tree depth
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: two parameters disagree
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       Called by user's SHA-1 interrupt handler registered with
 *                bslInit
 * */
error_code_t
bsl_hdcp_handle_sha_1
(
    unit_select_t	tx_unit,
    u8           max_ksv_devices,
    u8           *p_ksv_list,
    u8           *pn_ksv_devices,
    /* Connection tree depth returned with KSV list */
    u8           *p_depth
);

/*============================================================================*/
/**
 *    \brief      Declare KSV list result to be secure or not secure
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  bSecure         Result of user's check of KSV list against a
 *                                revocation list:
 *                                0 (not secure: one or more KSVs are in r.list)
 *                                1 (secure: no KSV found in revocation list)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       The user SHA_1 interrupt handler (registered with
 *                bslInit) calls this API after calling
 *                bslHdcpHandleSHA_1
 * */
error_code_t
bsl_hdcp_handle_sha_1result
(
    unit_select_t	tx_unit,
    bool            b_secure
);

/*============================================================================*/
/**
 *    \brief      Handle T0 interrupt
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *
 *    \note       Called by user's T0 interrupt handler registered with
 *                bslInit
 * */
error_code_t
bsl_hdcp_handle_t0
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 *    \brief      Prepare for HDCP operation
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  voutFmt     Video output format
 *    \param[in]  voutFreq    Vertical output frequency
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 *    \note       Must be called before bslHdcpRun
 * */
error_code_t
bsl_hdcp_init
(
    unit_select_t	    tx_unit,
    bsl_vid_fmt_t vout_fmt,
    bsl_vfreq_t  vout_freq
);

/*============================================================================*/
/**
 *    \brief      Start HDCP operation
 *
 *    \param[in]  txUnit  Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 *    \note       Must be called after bslHdcpInit
 * */
error_code_t
bsl_hdcp_run
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 *    \brief      Stop HDCP operation, and cease encrypting the output
 *
 *    \param[in]  txUnit  Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *                  - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 *    \note       This will trigger an Encrypt interrupt
 * */
error_code_t
bsl_hdcp_stop
(
    unit_select_t	tx_unit
);

/*============================================================================*/
/**
 * bslHotPlugGetStatus() parameter type
 * */
typedef enum {
	HDMITX_HOTPLUG_INACTIVE    = 0,    /**< Hotplug inactive */
	HDMITX_HOTPLUG_ACTIVE      = 1,    /**< Hotplug active   */
	HDMITX_HOTPLUG_INVALID     = 2     /**< Invalid Hotplug  */
} bsl_hot_plug_t;

/**
 *    \brief      Get the hot plug input status last read by bslInit
 *                or bslHwHandleInterrupt
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pHotPlugStatus  Pointer to returned Hot Plug Detect status
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_hot_plug_get_status
(
    unit_select_t        tx_unit,
    bsl_hot_plug_t *p_hot_plug_status
);

/*============================================================================*/
/**
 * bslRxSenseGetStatus() parameter type
 * */
typedef enum {
	HDMITX_RX_SENSE_INACTIVE    = 0,    /**< RxSense inactive */
	HDMITX_RX_SENSE_ACTIVE      = 1,    /**< RxSense active   */
	HDMITX_RX_SENSE_INVALID     = 2     /**< Invalid RxSense  */
} bsl_rx_sense_t;

/**
 *    \brief      Get the rx sense input status last read by bslInit
 *                or bslHwHandleInterrupt
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pRxSenseStatus  Pointer to returned Rx Sense Detect status
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_rx_sense_get_status
(
    unit_select_t        tx_unit,
    bsl_rx_sense_t *p_rx_sense_status
);

/*============================================================================*/
/**
 *    \brief      Get one or more hardware I2C register values
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  regPage     The device register's page: 00h, 01h, 02h, 11h, 12h
 *    \param[in]  regAddr     The starting register address on the page: 0 to FFh
 *    \param[out] pRegBuf     Pointer to buffer to receive the register data
 *    \param[in]  nRegs       Number of contiguous registers to read: 1 to 254
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *                  - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 * */
error_code_t
bsl_hw_get_registers
(
    unit_select_t	tx_unit,
    int             reg_page,
    int             reg_addr,
    u8          *p_reg_buf,
    int             n_regs
);

/*============================================================================*/
/**
 *    \brief      Get the transmitter device version read at initialization
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] puDeviceVersion Pointer to returned hardware version
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_hw_get_version
(
    unit_select_t  tx_unit,
    u8          *pu_device_version
);

/*============================================================================*/
/**
 * bslHwGetCapabilities() parameter type
 * */
typedef enum {
	bsl_hw_none       = 0,    /**< None           feature */
	bsl_hw_hdcp       = 1,    /**< HDCP           feature */
	bsl_hw_scaler     = 2,    /**< Scaler         feature */
	bsl_hw_hdcpscaler = 3,    /**< HDCP & Scaler  feature  */
	bsl_hw_all        = 3     /**< All            feature  */
} bsl_hw_feature_t;

/**
 *    \brief      Get the transmitter device feature read at initialization
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pDeviceFeature  Pointer to returned hardware feature
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_hw_get_capabilities
(
    unit_select_t          tx_unit,
    bsl_hw_feature_t  *p_device_capabilities
);

/*============================================================================*/
/**
 *    \brief      Handle all hardware interrupts from a transmitter unit
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *    \note       This function must be called at task level not interrupt level,
 *                as I2C access is required
 * */
error_code_t
bsl_hw_handle_interrupt
(
    unit_select_t      tx_unit
);

/*============================================================================*/
/**
 *    \brief      Set one or more hardware I2C registers
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  regPage     The device register's page: 00h, 01h, 02h, 11h, 12h
 *    \param[in]  regAddr     The starting register address on the page: 0 to FFh
 *    \param[in]  pRegBuf     Ptr to buffer from which to write the register data
 *    \param[in]  nRegs       Number of contiguous registers to write: 0 to 254.
 *                            The page register (255) may not be written - it is
 *                            written to automatically here. If nRegs is 0, the
 *                            page register is the only register written.
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_hw_set_registers
(
    unit_select_t	tx_unit,
    int             reg_page,
    int             reg_addr,
    u8          *p_reg_buf,
    int             n_regs
);

/*============================================================================*/
/**
 *    \brief      Handle hardware startup by resetting Device Instance Data
 * */
void
bsl_hw_startup
(
    void
);

/*============================================================================*/
/**
 * bslInit() parameter types
 * */
/**
 * Supported range of I2C slave addresses
 * */
enum _bsl_slave_address {
	HDMITX_SLAVE_ADDRESS_MIN    = 1,
	HDMITX_SLAVE_ADDRESS_MAX    = 127
};

/**
 * Indexes into the funcCallback[] array of interrupt callback function pointers
 * */
enum _bsl_callback_int {
	HDMITX_CALLBACK_INT_ENCRYPT  = 0,  /**< HDCP encryption switched off     */
	HDMITX_CALLBACK_INT_HPD      = 1,  /**< Transition on HPD input          */
	HDMITX_CALLBACK_INT_T0       = 2,  /**< HDCP state machine in state T0   */
	HDMITX_CALLBACK_INT_BCAPS    = 3,  /**< BCAPS available                  */
	HDMITX_CALLBACK_INT_BSTATUS  = 4,  /**< BSTATUS available                */
	HDMITX_CALLBACK_INT_SHA_1    = 5,  /**< sha-1(ksv,bstatus,m0)=V'         */
	HDMITX_CALLBACK_INT_PJ       = 6,  /**< pj=pj' check fails               */
#ifndef TMFL_TDA9981_SUPPORT
	HDMITX_CALLBACK_INT_UNUSED7  = 7,  /**< Unused interrupt                 */
	HDMITX_CALLBACK_INT_NUM      = 8   /**< Number of callbacks              */
#else /* TMFL_TDA9981_SUPPORT */
	HDMITX_CALLBACK_INT_R0	     = 7,  /**< R0 interrupt                     */
	HDMITX_CALLBACK_INT_SW_INT   = 8,  /**< SW DEBUG interrupt               */
#ifdef TMFL_RX_SENSE_ON
	HDMITX_CALLBACK_INT_RX_SENSE = 9,  /**< RX SENSE interrupt               */
	HDMITX_CALLBACK_INT_NUM      = 10   /**< Number of callbacks             */
#else /* TMFL_RX_SENSE_ON */
	HDMITX_CALLBACK_INT_NUM      = 9   /**< Number of callbacks             */
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */
};

/** Pixel rate */
typedef enum {
	HDMITX_PIXRATE_DOUBLE               = 0,        /**< Double pixel rate */
	HDMITX_PIXRATE_SINGLE               = 1,        /**< Single pixel rate */
	/**< Single pixel repeated */
	HDMITX_PIXRATE_SINGLE_REPEATED      = 2,
	HDMITX_PIXRATE_NO_CHANGE            = 3,        /**< No Change */
	HDMITX_PIXRATE_INVALID              = 4         /**< Invalid   */
} bsl_pix_rate_t;

/**
 * \brief The bslInit() parameter structure
 * */
typedef struct _bsl_callback_list_t {
	/** Interrupt callback function pointers (each ptr if null = not used) */
	pbsl_callback_t func_callback[HDMITX_CALLBACK_INT_NUM];

} bsl_callback_list_t;

/**
 *    \brief      Create an instance of an HDMI Transmitter: initialize the
 *                driver, reset the transmitter device and get the current
 *                Hot Plug state
 *
 *    \param[in]  txUnit           Transmitter unit number
 *    \param[in]  uHwAddress       Device I2C slave address
 *    \param[in]  sysFuncWrite     System function to write I2C
 *    \param[in]  sysFuncRead      System function to read I2C
 *    \param[in]  sysFuncEdidRead  System function to read EDID blocks via I2C
 *    \param[in]  sysFuncTimer     System function to run a timer
 *    \param[in]  funcIntCallbacks Pointer to interrupt callback function list
 *                                 The list pointer is null for no callbacks;
 *                                 each pointer in the list may also be null.
 *    \param[in]  bEdidAltAddr     Use alternative i2c address for EDID data
 *                                 register between Driver and TDA9983/2:
 *                                 0 - use default address (A0)
 *                                 1 - use alternative address (A2)
 *    \param[in]  vinFmt           EIA/CEA Video input format: 1 to 31, 0 = No Change
 *    \param[in]  pixRate          Single data (repeated or not) or double data rate
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: the unit number is wrong or
 *                    the transmitter instance is already initialised
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter is invalid or out
 *                    of range
 *                  - ERR_HDMI_INIT_FAILED: the unit instance is already
 *                    initialised
 *                  - ERR_HDMI_COMPATIBILITY: the driver is not compatiable
 *                    with the internal device version code
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 * */
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
);

/*============================================================================*/
/**
 * bslMatrixSetCoeffs() parameter type
 * */
/** Parameter structure array size */
enum _bsl_mat_coeff {
	HDMITX_MAT_COEFF_NUM = 9
};

/** \brief The bslMatrixSetCoeffs() parameter structure */
typedef struct _bsl_mat_coeff_t {
	/** Array of coefficients (values -1024 to +1023) */
	s16 coeff[HDMITX_MAT_COEFF_NUM];
} bsl_mat_coeff_t;

/**
 *    \brief      Set colour space converter matrix coefficients
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pMatCoeff   Pointer to Matrix Coefficient structure
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       Matrix Coefficient parameter structure:
 *                s16 Coeff[9]: Array of coefficients (values -1024 to +1023)
 * */
error_code_t
bsl_matrix_set_coeffs
(
    unit_select_t          tx_unit,
    bsl_mat_coeff_t   *p_mat_coeff
);

/*============================================================================*/
/**
 * bslMatrixSetConversion() parameter type
 * */
/** Video input mode */
typedef enum {
	HDMITX_VINMODE_CCIR656     = 0,    /**< ccir656  */
	HDMITX_VINMODE_RGB444      = 1,    /**< RGB444    */
	HDMITX_VINMODE_YUV444      = 2,    /**< YUV444    */
	HDMITX_VINMODE_YUV422      = 3,    /**< YUV422    */
	HDMITX_VINMODE_NO_CHANGE   = 4,    /**< No change */
	HDMITX_VINMODE_INVALID     = 5     /**< Invalid   */
} bsl_vin_mode_t;

/** Video output mode */
typedef enum {
	HDMITX_VOUTMODE_RGB444      = 0,    /**< RGB444    */
	HDMITX_VOUTMODE_YUV422      = 1,    /**< YUV422    */
	HDMITX_VOUTMODE_YUV444      = 2,    /**< YUV444    */
	HDMITX_VOUTMODE_NO_CHANGE   = 3,    /**< No change */
	HDMITX_VOUTMODE_INVALID     = 4     /**< Invalid   */
} bsl_vout_mode_t;

/**
 * \brief Enum defining possible quantization range
 * */
typedef enum {
	HDMITX_VQR_DEFAULT = 0, /* Follow HDMI spec. */
	HDMITX_RGB_FULL    = 1, /* Force RGB FULL , DVI only */
	HDMITX_RGB_LIMITED = 2  /* Force RGB LIMITED , DVI only */
} bsl_vqr_t;

/**
 *    \brief      Set colour space conversion using preset values
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  vinFmt          Input video format
 *    \param[in]  vinMode         Input video mode
 *    \param[in]  voutFmt         Output video format
 *    \param[in]  voutMode        Output video mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_matrix_set_conversion
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vin_fmt,
    bsl_vin_mode_t    vin_mode,
    bsl_vid_fmt_t     vout_fmt,
    bsl_vout_mode_t   vout_mode,
    bsl_vqr_t        dvi_vqr
);

/*============================================================================*/
/**
 * bslMatrixSetInputOffset() parameter type
 * */
/** Parameter structure array size */
enum _bsl_mat_offset {
	HDMITX_MAT_OFFSET_NUM = 3
};

/** \brief The bslMatrixSetInputOffset() parameter structure */
typedef struct _bsl_mat_offset_t {
	/** Offset array  (values -1024 to +1023) */
	s16 offset[HDMITX_MAT_OFFSET_NUM];
} bsl_mat_offset_t;

/** Matrix numeric limits */
enum _bsl_mat_limits {
	HDMITX_MAT_OFFSET_MIN      = -1024,
	HDMITX_MAT_OFFSET_MAX      = 1023
};

/**
 *    \brief      Set colour space converter matrix offset at input
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pMatOffset  Pointer to Matrix Offset structure
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 *    \note       Matrix Offset structure parameter structure:
 *                s16 Offset[3]: Offset array (values -1024 to +1023)
 * */
error_code_t
bsl_matrix_set_input_offset
(
    unit_select_t          tx_unit,
    bsl_mat_offset_t  *p_mat_offset
);

/*============================================================================*/
/**
 * bslMatrixSetMode() parameter types
 * */
/** Matrix  control values */
typedef enum {
	HDMITX_MCNTRL_ON        = 0,   /**< Matrix on  */
	HDMITX_MCNTRL_OFF       = 1,   /**< Matrix off */
	HDMITX_MCNTRL_NO_CHANGE = 2,   /**< Matrix unchanged */
	HDMITX_MCNTRL_MAX       = 2,   /**< Max value  */
	HDMITX_MCNTRL_INVALID   = 3    /**< Invalid    */
} bslm_cntrl_t;

/** Matrix  scale values */
typedef enum {
	HDMITX_MSCALE_256       = 0,   /**< Factor 1/256  */
	HDMITX_MSCALE_512       = 1,   /**< Factor 1/512  */
	HDMITX_MSCALE_1024      = 2,   /**< Factor 1/1024 */
	HDMITX_MSCALE_NO_CHANGE = 3,   /**< Factor unchanged */
	HDMITX_MSCALE_MAX       = 3,   /**< Max value     */
	HDMITX_MSCALE_INVALID   = 4    /**< Invalid value */
} bslm_scale_t;

/**
 *    \brief      Set colour space converter matrix mode
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  mControl    Matrix Control: On, Off, No change
 *    \param[in]  mScale      Matrix Scale Factor: 1/256, 1/512, 1/1024, No change
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 *    \note       NA
 *
 *    \sa         NA
 * */
error_code_t
bsl_matrix_set_mode
(
    unit_select_t      tx_unit,
    bslm_cntrl_t m_control,
    bslm_scale_t m_scale
);

/*============================================================================*/
/**
 *    \brief      Set colour space converter matrix offset at output
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pMatOffset  Pointer to Matrix Offset structure
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 *    \note       Matrix Offset parameter structure:
 *                nt16 Offset[3]: Offset array (values -1024 to +1023)
 * */
error_code_t
bsl_matrix_set_output_offset
(
    unit_select_t          tx_unit,
    bsl_mat_offset_t  *p_mat_offset
);

/*============================================================================*/
/**
 *    \brief      Enable audio clock recovery packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: when in DVI mode
 *
 *    \note       bslAudioInSetCts sets CTS and N values
 * */
error_code_t
bsl_pkt_set_aclk_recovery
(
    unit_select_t	tx_unit,
    bool            b_enable
);

/*============================================================================*/
/**
 * Data Island Packet structure
 * */
/** Parameter structure array sizes */
enum _bsl_pkt {
	HDMITX_PKT_DATA_BYTE_CNT = 28
};

/** \brief The parameter structure for bslPkt*() APIs */
typedef struct _bsl_pkt_t {
	u8 data_byte[HDMITX_PKT_DATA_BYTE_CNT];       /**< Packet Data   */
} bsl_pkt_t;

/**
 *    \brief      Set audio content protection packet & enable/disable packet
 *                insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pPkt        Pointer to Data Island Packet structure
 *    \param[in]  byteCnt     Packet buffer byte count
 *    \param[in]  uAcpType    Content protection type
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: pointer suppied with byte count of zero
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Data Island Packet parameter structure:
 *                u8 dataByte[28]      Packet Data
 *
 *    \sa         NA
 * */
error_code_t
bsl_pkt_set_acp
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    u8               u_acp_type,
    bool                b_enable
);

/*============================================================================*/
/**
 * \brief The Audio Infoframe Parameter structure
 * */
typedef struct _bsl_pkt_aif_t {
	u8 coding_type;       /**< Coding Type 0 to 0Fh */
	u8 channel_count;     /**< Channel Count 0 to 07h */
	u8 sample_freq;       /**< Sample Frequency 0 to 07h */
	u8 sample_size;       /**< Sample Size 0 to 03h */
	u8 channel_alloc;     /**< Channel Allocation 0 to FFh */
	bool  down_mix_inhibit;   /**< Downmix inhibit flag 0/1 */
	u8 level_shift;       /**< Level Shift 0 to 0Fh */
} bsl_pkt_aif_t;

/**
 *    \brief      Set audio info frame packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit  Transmitter unit number
 *    \param[in]  pPkt    Pointer to Audio Infoframe structure
 *    \param[in]  bEnable Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Audio Infoframe structure:
 *                u8 CodingType
 *                u8 ChannelCount
 *                u8 SampleFreq
 *                u8 SampleSize
 *                u8 ChannelAlloc
 *                bool DownMixInhibit
 *                u8 LevelShift
 * */
error_code_t
bsl_pkt_set_audio_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_aif_t *p_pkt,
    bool                b_enable
);

/*============================================================================*/
/**
 *    \brief      Set contents of general control packet & enable/disable
 *                packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  paMute      Pointer to Audio Mute; if NULL, no change to packet
 *                            contents is made
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       bslAudioOutSetMute must be used to mute the audio output
 * */
error_code_t
bsl_pkt_set_general_cntrl
(
    unit_select_t      tx_unit,
    bsla_mute_t  *pa_mute,
    bool                b_enable
);

/*============================================================================*/
/**
 *    \brief      Set ISRC1 packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pPkt        Pointer to Data Island Packet structure
 *    \param[in]  byteCnt     Packet buffer byte count
 *    \param[in]  bIsrcCont   ISRC continuation flag
 *    \param[in]  bIsrcValid  ISRC valid flag
 *    \param[in]  uIsrcStatus ISRC Status
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: pointer suppied with byte count of zero
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Data Island Packet parameter structure:
 *                u8 dataByte[28]  Packet Data
 *
 *    \sa         NA
 * */
error_code_t
bsl_pkt_set_isrc1
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    bool                b_isrc_cont,
    bool                b_isrc_valid,
    u8               u_isrc_status,
    bool                b_enable
);

/*============================================================================*/
/**
 *    \brief      Set ISRC2 packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pPkt        Pointer to Data Island Packet structure
 *    \param[in]  byteCnt     Packet buffer byte count
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: pointer suppied with byte count of zero
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Data Island Packet parameter structure:
 *                u8 dataByte[28]      Packet Data
 *
 *    \sa         NA
 * */
error_code_t
bsl_pkt_set_isrc2
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    bool                b_enable
);

/*============================================================================*/
/**
 * bslPktSetMpegInfoframe() parameter types
 * */
/** MPEG frame types */
typedef enum {
	HDMITX_MPEG_FRAME_UNKNOWN   = 0,    /**< Unknown  */
	HDMITX_MPEG_FRAME_I         = 1,    /**< i-frame   */
	HDMITX_MPEG_FRAME_B         = 2,    /**< b-frame */
	HDMITX_MPEG_FRAME_P         = 3,    /**< p-frame */
	HDMITX_MPEG_FRAME_INVALID   = 4     /**< Invalid   */
} bsl_mpeg_frame_t;

/** \brief The MPEG Infoframe Parameter structure */
typedef struct _bsl_pkt_mpeg_t {
	u32                  bit_rate;        /**< MPEG bit rate in Hz */
	bsl_mpeg_frame_t  frame_type;      /**< MPEG frame type */
	/**< 0: new field, 1:repeated field */
	bool                    b_field_repeat;
} bsl_pkt_mpeg_t;

/**
 *    \brief      Set MPEG infoframe packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  pPkt            Pointer to MPEG Infoframe structure
 *    \param[in]  bEnable         Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       MPEG Infoframe structure:
 *                u32                  bitRate
 *                bslMpegFrame_t  frameType
 *                bool                    bFieldRepeat
 *
 *    \sa         NA
 * */
error_code_t
bsl_pkt_set_mpeg_infoframe
(
    unit_select_t          tx_unit,
    bsl_pkt_mpeg_t    *p_pkt,
    bool                    b_enable
);

/*============================================================================*/
/**
 *    \brief      Enable NULL packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 * */
error_code_t
bsl_pkt_set_null_insert
(
    unit_select_t  tx_unit,
    bool            b_enable
);

/*============================================================================*/
/**
 *    \brief      Set single NULL packet insertion (flag auto-resets after
 *                transmission)
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Operation resets after single transmission
 * */
error_code_t
bsl_pkt_set_null_single
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 * Source Product Description Infoframe Parameter types
 *  */
/** SDI frame types */
typedef enum {
	HDMITX_SPD_INFO_UNKNOWN     = 0,
	HDMITX_SPD_INFO_DIGITAL_STB = 1,
	HDMITX_SPD_INFO_DVD         = 2,
	HDMITX_SPD_INFO_DVHS        = 3,
	HDMITX_SPD_INFO_HDD_VIDEO   = 4,
	HDMITX_SPD_INFO_DVC         = 5,
	HDMITX_SPD_INFO_DSC         = 6,
	HDMITX_SPD_INFO_VIDEO_CD    = 7,
	HDMITX_SPD_INFO_GAME        = 8,
	HDMITX_SPD_INFO_PC          = 9,
	HDMITX_SPD_INFO_INVALID     = 10
} bsl_source_dev_t;

#define HDMI_TX_SPD_VENDOR_SIZE 8
#define HDMI_TX_SPD_DESCR_SIZE  16
#define HDMI_TX_SPD_LENGTH      25

/** \brief The Source Product Description Infoframe Parameter structure */
typedef struct _bsl_pkt_spd_t {
	/**< Vendor name */
	u8                   vendor_name[HDMI_TX_SPD_VENDOR_SIZE];
	/**< Product Description */
	u8                   prod_descr[HDMI_TX_SPD_DESCR_SIZE];
	/**< Source Device Info */
	bsl_source_dev_t  source_dev_info;
} bsl_pkt_spd_t;

/**
 *    \brief      Set audio info frame packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit  Transmitter unit number
 *    \param[in]  pPkt    Pointer to Audio Infoframe structure
 *    \param[in]  bEnable Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Audio Infoframe structure:
 *                u8                   VendorName[8]
 *                u8                   ProdDescr[16]
 *                bslSourceDev_t  SourceDevInfo
 * */
error_code_t
bsl_pkt_set_spd_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_spd_t *p_pkt,
    bool                b_enable
);

/*============================================================================*/
/**
 * \brief The Video Infoframe Parameter structure
 *  */
typedef struct _bsl_pkt_vif_t {
	u8  colour;               /**< 0 to 03h */
	bool   active_info;           /**< 0/1 */
	u8  bar_info;              /**< 0 to 03h */
	u8  scan_info;             /**< 0 to 03h */
	u8  colorimetry;          /**< 0 to 03h */
	u8  picture_aspect_ratio;   /**< 0 to 03h */
	u8  active_format_ratio;    /**< 0 to 0Fh */
	u8  scaling;              /**< 0 to 03h */
	u8  vid_format;            /**< 0 to 7Fh */
	u8  pixel_repeat;          /**< 0 to 0Fh */
	u16 end_top_bar_line;
	u16 start_bottom_bar_line;
	u16 end_left_bar_pixel;
	u16 start_right_bar_pixel;
} bsl_pkt_vif_t;

/**
 *    \brief      Set video infoframe packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit  Transmitter unit number
 *    \param[in]  pPkt    Pointer to Video Infoframe structure
 *    \param[in]  bEnable Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Video Infoframe structure:
 *                u8 Colour
 *                bool ActiveInfo
 *                u8 BarInfo
 *                u8 ScanInfo
 *                u8 Colorimetry
 *                u8 PictureAspectRatio
 *                u8 ActiveFormatRatio
 *                u8 Scaling
 *                u8 VidFormat
 *                u8 PixelRepeat
 *                u16 EndTopBarLine
 *                u16 StartBottomBarLine
 *                u16 EndLeftBarPixel
 *                u16 StartRightBarPixel   (incorrectly named in [HDMI1.2])
 * */
error_code_t
bsl_pkt_set_video_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_vif_t *p_pkt,
    bool                b_enable
);

/*============================================================================*/
/**
 *    \brief      Set Vendor Specific Infoframe packet & enable/disable packet insertion
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  pPkt        Pointer to Data Island Packet structure
 *    \param[in]  byteCnt     Packet buffer byte count
 *    \param[in]  uVersion    Version number for packet header
 *    \param[in]  bEnable     Enable or disable packet insertion
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: pointer suppied with byte count of zero
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_NOT_SUPPORTED: not possible with this device
 *                  - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 *    \note       Data Island Packet parameter structure:
 *                u8 dataByte[28]      Packet Data (only use 27 bytes max)
 *
 *    \sa         NA
 * */
error_code_t
bsl_pkt_set_vs_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    u8               u_version,
    bool                b_enable
);

/*============================================================================*/
/**
 *    \brief      Get the power state of the transmitter
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[out] pePowerState Pointer to the power state of the device now
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 *    \note       Power states:
 *                - tmPowerOn
 *                - tmPowerStandby
 * */
error_code_t
bsl_power_get_state
(
    unit_select_t      tx_unit,
    p_power_state_t		pe_power_state
);

/*============================================================================*/
/**
 *    \brief      Set the power state of the transmitter
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  ePowerState Power state to set
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       Power states (Off and Suspend are treated the same as Standby):
 *                - tmPowerOn
 *                - tmPowerStandby
 *                - tmPowerSuspend
 *                - tmPowerOff
 * */
error_code_t
bsl_power_set_state
(
    unit_select_t      tx_unit,
    power_state_t      e_power_state
);

/*============================================================================*/
/**
 *    \brief      Reset the HDMI transmitter
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       NA
 *
 *    \sa         bslInit
 * */
error_code_t
bsl_reset
(
    unit_select_t      tx_unit
);

/*============================================================================*/
/**
 * \brief The bslScalerGet() parameter type
 * */
typedef struct _bsl_scaler_diag_t {
	u16 max_buffill_p;     /**< Filling primary video buffer           */
	u16 max_buffill_d;     /**< Filling video deinterlaced buffer      */
	u8 max_fifofill_pi;    /**< Filling primary video input FIFO       */
	u8 min_fifofill_po1;   /**< Filling primary video output FIFO #1   */
	u8 min_fifofill_po2;   /**< Filling primary video output FIFO #2   */
	u8 min_fifofill_po3;   /**< Filling primary video output FIFO #3   */
	u8 min_fifofill_po4;   /**< Filling primary video output FIFO #4   */
	u8 max_fifofill_di;    /**< Filling deinterlaced video input FIFO  */
	u8 max_fifofill_do;    /**< Filling deinterlaced video output FIFO */
} bsl_scaler_diag_t;

/**
 *    \brief      Get diagnostic counters from the scaler
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pScalerDiag Pointer to structure to receive scaler diagnostic
 *                            registers
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *
 *    \note       scaler diagnostic registers structure:
 *                u16 maxBuffill_p     Filling primary video buffer
 *                u16 maxBuffill_d     Filling video deinterlaced buffer
 *                u8  maxFifofill_pi   Filling primary video input FIFO
 *                u8  minFifofill_po1  Filling primary video output FIFO #1
 *                u8  minFifofill_po2  Filling primary video output FIFO #2
 *                u8  minFifofill_po3  Filling primary video output FIFO #3
 *                u8  minFifofill_po4  Filling primary video output FIFO #4
 *                u8  maxFifofill_di   Filling deinterlaced video input FIFO
 *                u8  maxFifofill_do   Filling deinterlaced video output FIFO
 * */
error_code_t
bsl_scaler_get
(
    unit_select_t           tx_unit,
    bsl_scaler_diag_t *p_scaler_diag
);

/*============================================================================*/
/**
 * bslScalerGetMode() parameter types
 * */
/** Scaler modes */
typedef enum {
	HDMITX_SCAMODE_OFF       = 0,    /**< Off  */
	HDMITX_SCAMODE_ON        = 1,    /**< On   */
	HDMITX_SCAMODE_AUTO      = 2,    /**< Auto */
	HDMITX_SCAMODE_NO_CHANGE = 3,    /**< No change */
	HDMITX_SCAMODE_INVALID   = 4     /**< Invalid   */
} bsl_sca_mode_t;

/**
 *    \brief      Get the current scaler mode
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[out] pScalerMode Pointer to variable to receive scaler mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_scaler_get_mode
(
    unit_select_t		      tx_unit,
    bsl_sca_mode_t      *p_scaler_mode
);

/*============================================================================*/
/**
 *    \brief      Enable or disable scaler input frame
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  bDisable    Enable or disable scaler input
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_scaler_in_disable
(
    unit_select_t  tx_unit,
    bool            b_disable
);

/*============================================================================*/
/**
 * bslScalerSetCoeffs() parameter types
 * */
/** Scaler lookup table selection */
typedef enum {
	HDMITX_SCALUT_DEFAULT_TAB1     = 0,    /**< Use default table 1 */
	HDMITX_SCALUT_DEFAULT_TAB2     = 1,    /**< Use default table 2 */
	HDMITX_SCALUT_USE_VSLUT        = 2,    /**< Use vsLut parameter */
	HDMITX_SCALUT_INVALID          = 3     /**< Invalid value       */
} bsl_sca_lut_t;

/** Scaler control parameter structure array size */
enum _bslvs_lut {
	HDMITX_VSLUT_COEFF_NUM         = 45
};

/**
 *    \brief      Set the active coefficient lookup table for the vertical scaler
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  lutSel      Coefficient lookup table selection
 *    \param[in]  pVsLut      Table of HDMITX_VSLUT_COEFF_NUM coefficient values
 *                            (may be null if lutSel not HDMITX_SCALUT_USE_VSLUT)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: two parameters disagree
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_scaler_set_coeffs
(
    unit_select_t        tx_unit,
    bsl_sca_lut_t   lut_sel,
    u8                *p_vs_lut
);

/*============================================================================*/
/**
 * bslScalerSetFieldOrder() parameter types
 * */
/** IntExt values */
typedef enum {
	HDMITX_INTEXT_INTERNAL         = 0,    /**< Internal  */
	HDMITX_INTEXT_EXTERNAL         = 1,    /**< External  */
	HDMITX_INTEXT_NO_CHANGE        = 2,    /**< No change */
	HDMITX_INTEXT_INVALID          = 3     /**< Invalid   */
} bsl_int_ext_t;

/** TopSel values */
typedef enum {
	HDMITX_TOPSEL_INTERNAL         = 0,    /**< Internal  */
	HDMITX_TOPSEL_VRF              = 1,    /**< VRF       */
	HDMITX_TOPSEL_NO_CHANGE        = 2,    /**< No change */
	HDMITX_TOPSEL_INVALID          = 3     /**< Invalid   */
} bsl_top_sel_t;

/** TopTgl values */
typedef enum {
	HDMITX_TOPTGL_NO_ACTION        = 0,    /**< NO action */
	HDMITX_TOPTGL_TOGGLE           = 1,    /**< Toggle    */
	HDMITX_TOPTGL_NO_CHANGE        = 2,    /**< No change */
	HDMITX_TOPTGL_INVALID          = 3     /**< Invalid   */
} bsl_top_tgl_t;

/**
 *    \brief      Set scaler field positions
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  topExt      Internal, External, No Change
 *    \param[in]  deExt       Internal, External, No Change
 *    \param[in]  topSel      Internal, VRF, No Change
 *    \param[in]  topTgl      No Action, Toggle, No Change
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_scaler_set_field_order
(
    unit_select_t      tx_unit,
    bsl_int_ext_t top_ext,
    bsl_int_ext_t de_ext,
    bsl_top_sel_t top_sel,
    bsl_top_tgl_t top_tgl
);

/*============================================================================*/
/**
 * bslScalerSetFine() parameter types
 * */
/** Reference pixel values */
enum _bsl_scaler_fine_pixel_limits {
	HDMITX_SCALER_FINE_PIXEL_MIN        = 0x0000,
	HDMITX_SCALER_FINE_PIXEL_MAX        = 0x1FFF,
	HDMITX_SCALER_FINE_PIXEL_NO_CHANGE  = 0x2000,
	HDMITX_SCALER_FINE_PIXEL_INVALID    = 0x2001
};

/** Reference line values */
enum _bsl_scaler_fine_line_limits {
	HDMITX_SCALER_FINE_LINE_MIN         = 0x0000,
	HDMITX_SCALER_FINE_LINE_MAX         = 0x07FF,
	HDMITX_SCALER_FINE_LINE_NO_CHANGE   = 0x0800,
	HDMITX_SCALER_FINE_LINE_INVALID     = 0x0801
};

/**
 *    \brief      Set scaler fine adjustment options
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  uRefPix      Ref. pixel preset 0 to 1FFFh (2000h = No Change)
 *    \param[in]  uRefLine     Ref. line preset 0 to 7FFh (800h = No Change)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_scaler_set_fine
(
    unit_select_t  tx_unit,
    u16          u_ref_pix,
    u16          u_ref_line
);

/*============================================================================*/
/**
 * bslScalerSetSync() parameter types
 * */
/** Video sync method */
typedef enum {
	HDMITX_VSMETH_V_H          = 0,    /**< V and H    */
	HDMITX_VSMETH_V_XDE        = 1,    /**< V and X-DE */
	HDMITX_VSMETH_NO_CHANGE    = 2,    /**< No change  */
	HDMITX_VSMETH_INVALID      = 3     /**< Invalid    */
} bsl_vs_meth_t;

/** Line/pixel counters sync */
typedef enum {
	HDMITX_VSONCE_EACH_FRAME   = 0,    /**< Sync on each frame */
	HDMITX_VSONCE_ONCE         = 1,    /**< Sync once only     */
	HDMITX_VSONCE_NO_CHANGE    = 2,    /**< No change  */
	HDMITX_VSONCE_INVALID      = 3     /**< Invalid    */
} bsl_vs_once_t;

/**
 *    \brief      Set scaler synchronization options
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  method       Sync. combination method
 *    \param[in]  once         Line/pixel counters sync once or each frame
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_scaler_set_sync
(
    unit_select_t      tx_unit,
    bsl_vs_meth_t method,
    bsl_vs_once_t once
);

/*============================================================================*/
/**
 *    \brief      Get the driver software version and compatibility numbers
 *
 *    \param[out] pSWVersion   Pointer to the software version structure returned
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_sw_get_version
(
    p_swversion_t      p_swversion
);

/*============================================================================*/
/**
 *    \brief      Get the driver software version and compatibility numbers
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  waitMs       Period in milliseconds to wait
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_sys_timer_wait
(
    unit_select_t      tx_unit,
    u16              wait_ms
);

/*============================================================================*/
/**
 * bslTmdsSetOutputs() parameter types
 * */
/** TMDS output mode */
typedef enum {
	HDMITX_TMDSOUT_NORMAL      = 0,    /**< Normal outputs   */
	HDMITX_TMDSOUT_NORMAL1     = 1,    /**< Normal outputs, same as 0  */
	HDMITX_TMDSOUT_FORCED0     = 2,    /**< Forced 0 outputs */
	HDMITX_TMDSOUT_FORCED1     = 3,    /**< Forced 1 outputs */
	HDMITX_TMDSOUT_INVALID     = 4     /**< Invalid          */
} bsl_tmds_out_t;

/**
 *    \brief      Set the TMDS outputs to normal active operation or to a forced
 *                state
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  tmdsOut     TMDS output mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_tmds_set_outputs
(
    unit_select_t          tx_unit,
    bsl_tmds_out_t    tmds_out
);

/*============================================================================*/
/**
 * bslTmdsSetSerializer() parameter types
 * */
/** Serializer phase limits */
enum _bsl_tmds_phase {
	HDMITX_TMDSPHASE_MIN       = 0,
	HDMITX_TMDSPHASE_MAX       = 15,
	HDMITX_TMDSPHASE_INVALID   = 16
};

/**
 *    \brief      Fine-tune the TMDS serializer
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  uPhase2     Serializer phase 2
 *    \param[in]  uPhase3     Serializer phase 3
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 * */
error_code_t
bsl_tmds_set_serializer
(
    unit_select_t  tx_unit,
    u8           u_phase2,
    u8           u_phase3
);

/*============================================================================*/
/**
 * bslTestSetPattern() parameter types
 * */
/** Test pattern types */
typedef enum {
	HDMITX_PATTERN_OFF     = 0, /**< Insert test pattern     */
	HDMITX_PATTERN_CBAR4   = 1, /**< Insert 4-bar colour bar */
	HDMITX_PATTERN_CBAR8   = 2, /**< Insert 8-bar colour bar */
	HDMITX_PATTERN_BLUE	   = 3, /**< Insert Blue screen		 */
	HDMITX_PATTERN_INVALID = 4  /**< Invalid pattern		 */
} bsl_test_pattern_t;

/*
 *    \brief      Set a colour bar test pattern
 *
 *    \param[in]  txUnit     Transmitter unit number
 *    \param[in]  pattern    Test pattern
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_test_set_pattern
(
    unit_select_t           tx_unit,
    bsl_test_pattern_t pattern
);

/*============================================================================*/
/**
 * bslTestSetMode() parameter types
 * */
/** Test modes */
typedef enum {
	HDMITX_TESTMODE_PAT     = 0,/**< Insert test pattern                    */
	HDMITX_TESTMODE_656     = 1,/**< Inject CCIR-656 video via audio port   */
	HDMITX_TESTMODE_SERPHOE = 2,/**< Activate srl_tst_ph2_o & srl_tst_ph3_o */
	HDMITX_TESTMODE_NOSC    = 3,/**< Input nosc predivider = PLL-ref input  */
	HDMITX_TESTMODE_HVP     = 4,/**< Test high voltage protection cells     */
	HDMITX_TESTMODE_PWD     = 5,/**< Test PLLs in sleep mode                */
	HDMITX_TESTMODE_DIVOE   = 6,/**< Enable scaler PLL divider test output  */
	HDMITX_TESTMODE_INVALID = 7 /**< Invalid test							*/
} bsl_test_mode_t;

/** Test states */
typedef enum {
	HDMITX_TESTSTATE_OFF     = 0,  /**< Disable the selected test */
	HDMITX_TESTSTATE_ON      = 1,  /**< Enable the selected test  */
	HDMITX_TESTSTATE_INVALID = 2   /**< Invalid value */
} bsl_test_state_t;

/**
 *    \brief      Set or clear one or more simultaneous test modes
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  testMode    Mode: tst_pat, tst_656, tst_serphoe, tst_nosc,
 *                            tst_hvp, tst_pwd, tst_divoe
 *    \param[in]  testState   State: 1=On, 0=Off
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_test_set_mode
(
    unit_select_t          tx_unit,
    bsl_test_mode_t   test_mode,
    bsl_test_state_t  test_state
);

/*============================================================================*/
/**
 * bslVideoInSetBlanking() parameter types
 * */
/** Blankit Source */
typedef enum {
	HDMITX_BLNKSRC_NOT_DE       = 0,       /**< Source=Not DE        */
	HDMITX_BLNKSRC_VS_HS        = 1,       /**< Source=VS And HS     */
	HDMITX_BLNKSRC_VS_NOT_HS    = 2,       /**< Source=VS And Not HS */
	HDMITX_BLNKSRC_VS_HEMB_VEMB = 3,       /**< Source=Hemb And Vemb */
	HDMITX_BLNKSRC_NO_CHANGE    = 4,       /**< No change */
	HDMITX_BLNKSRC_INVALID      = 5        /**< Invalid   */
} bsl_blnk_src_t;

/** Blanking Codes */
typedef enum {
	HDMITX_BLNKCODE_ALL_0       = 0,       /**< Code=All Zero */
	HDMITX_BLNKCODE_RGB444      = 1,       /**< Code=RGB444   */
	HDMITX_BLNKCODE_YUV444      = 2,       /**< Code=YUV444   */
	HDMITX_BLNKCODE_YUV422      = 3,       /**< Code=YUV422   */
	HDMITX_BLNKCODE_NO_CHANGE   = 4,       /**< No change */
	HDMITX_BLNKCODE_INVALID     = 5        /**< Invalid   */
} bsl_blnk_code_t;

/**
 *    \brief      Enable blanking between active data
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  blankitSource   Blankit Source: Not DE, VS And HS,
 *                                VS And Not HS, Hemb And Vemb, No Change
 *    \param[in]  blankingCodes   Blanking Codes: All Zero, RGB444, YUV444,
 *                                YUV422, No Change
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       NA
 *
 *    \sa         NA
 * */
error_code_t
bsl_video_in_set_blanking
(
    unit_select_t         tx_unit,
    bsl_blnk_src_t   blankit_source,
    bsl_blnk_code_t  blanking_codes
);

/*============================================================================*/
/**
 * bslVideoInSetConfig() parameter types
 * */
/** Sample edge */
typedef enum {
	HDMITX_PIXEDGE_CLK_POS      = 0,        /**< Pixel Clock Positive Edge */
	HDMITX_PIXEDGE_CLK_NEG      = 1,        /**< Pixel Clock Negative Edge */
	HDMITX_PIXEDGE_NO_CHANGE    = 2,        /**< No Change */
	HDMITX_PIXEDGE_INVALID      = 3         /**< Invalid   */
} bsl_pix_edge_t;

/** Upsample modes */
typedef enum {
	HDMITX_UPSAMPLE_BYPASS      = 0,        /**< Bypass */
	HDMITX_UPSAMPLE_COPY        = 1,        /**< Copy */
	HDMITX_UPSAMPLE_INTERPOLATE = 2,        /**< Interpolate */
	/**< Auto: driver chooses best value */
	HDMITX_UPSAMPLE_AUTO        = 3,
	HDMITX_UPSAMPLE_NO_CHANGE   = 4,        /**< No Change */
	HDMITX_UPSAMPLE_INVALID     = 5         /**< Invalid   */
} bsl_upsample_mode_t;

/**
 *    \brief      Configure video input options and control the upsampler
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  vinMode         Video input mode
 *    \param[in]  sampleEdge      Sample edge:
 *                                Pixel Clock Positive Edge,
 *                                Pixel Clock Negative Edge, No Change
 *    \param[in]  pixRate         Single data or double data rate
 *    \param[in]  upsampleMode    Upsample mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_in_set_config
(
    unit_select_t            tx_unit,
    bsl_vin_mode_t      vin_mode,
    bsl_pix_edge_t      sample_edge,
    bsl_pix_rate_t      pix_rate,
    bsl_upsample_mode_t upsample_mode
);

/*============================================================================*/
/**
 * bslVideoInSetFine() parameter types
 * */
/** Subpacket count */
typedef enum {
	HDMITX_PIXSUBPKT_FIX_0      = 0,        /**< Fix At 0 */
	HDMITX_PIXSUBPKT_FIX_1      = 1,        /**< Fix At 1 */
	HDMITX_PIXSUBPKT_FIX_2      = 2,        /**< Fix At 2 */
	HDMITX_PIXSUBPKT_FIX_3      = 3,        /**< Fix At 3 */
	HDMITX_PIXSUBPKT_SYNC_FIRST = 4,        /**< First Sync value */
	HDMITX_PIXSUBPKT_SYNC_HEMB  = 4,        /**< Sync By Hemb */
	HDMITX_PIXSUBPKT_SYNC_DE    = 5,        /**< Sync By Rising Edge DE */
	HDMITX_PIXSUBPKT_SYNC_HS    = 6,        /**< Sync By Rising Edge HS */
	HDMITX_PIXSUBPKT_NO_CHANGE  = 7,        /**< No Change */
	HDMITX_PIXSUBPKT_INVALID    = 8,        /**< Invalid   */
	HDMITX_PIXSUBPKT_SYNC_FIXED = 3         /**< Not used as a parameter value,
 *  but used internally when
 *  Fix at 0/1/2/3 values are set */
} bsl_pix_subpkt_t;

/** Toggling */
typedef enum {
	HDMITX_PIXTOGL_NO_ACTION    = 0,        /**< No Action  */
	HDMITX_PIXTOGL_ENABLE       = 1,        /**< Toggle     */
	HDMITX_PIXTOGL_NO_CHANGE    = 2,        /**< No Change  */
	HDMITX_PIXTOGL_INVALID      = 3         /**< Invalid    */
} bsl_pix_togl_t;

/**
 *    \brief      Set fine image position
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  subpacketCount  Subpacket Count fixed values and sync options
 *    \param[in]  toggleClk1      Toggle clock 1 phase w.r.t. clock 2
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       NA
 *
 *    \sa         NA
 * */
error_code_t
bsl_video_in_set_fine
(
    unit_select_t            tx_unit,
    bsl_pix_subpkt_t    subpacket_count,
    bsl_pix_togl_t      toggle_clk1
);

/*============================================================================*/
/**
 * bslVideoInSetMapping() parameter types
 * */
/** Video input port parameter structure array size and limits */
enum _bsl_vin_port_map {
	HDMITX_VIN_PORT_MAP_TABLE_LEN    = 6,

	HDMITX_VIN_PORT_SWAP_NO_CHANGE   = 6,
	HDMITX_VIN_PORT_SWAP_INVALID     = 7,

	HDMITX_VIN_PORT_MIRROR_NO_CHANGE = 2,
	HDMITX_VIN_PORT_MIRROR_INVALID   = 3
};

/**
 *    \brief      Set video input port swapping and mirroring
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[in]  pSwapTable      Pointer to 6-byte port swap table
 *    \param[in]  pMirrorTable    Pointer to 6-byte port mirror table
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       u8 pSwapTable[6]
 *
 *                Each table position 0 to 5 represents a group of 4 port bits:
 *                [0]=23:20, [1]=16:19, [2]=15:12, [3]=11:8, [4]=4:7, [5]=0:3
 *                Table position values are 0 to 6, denoting the group of 4 port
 *                bits to swap to: 0=23:20, 1=16:19, 2=15:12, 3=11:8, 4=4:7, 5=0:3.
 *                For example, to swap port bits 15:12 to bits 4:7: pSwapTable[2]=4
 *
 *                u8 pMirrorTable[6]
 *
 *                Each table position 0 to 5 represents a group of 4 port bits:
 *                [0]=23:20, [1]=16:19, [2]=15:12, [3]=11:8, [4]=4:7, [5]=0:3.
 *                Cell values are 0 to 2 (Not Mirrored, Mirrored, No Change).
 *                For example, to mirror port bits 11:8 to bits 8:11:
 *                pMirrorTable[3]=1.
 * */
error_code_t
bsl_video_in_set_mapping
(
    unit_select_t  tx_unit,
    u8           *p_swap_table,
    u8           *p_mirror_table
);

/*============================================================================*/

#define HDMITX_ENABLE_VP_TABLE_LEN	3
#define HDMITX_GROUND_VP_TABLE_LEN	3

/**
 *    \brief      Set video input port (enable, ground)
 *
 *    \param[in]  txUnit                Transmitter unit number
 *    \param[in]  pEnaVideoPortTable    Pointer to 3-byte video port enable table
 *    \param[in]  pGndVideoPortTable    Pointer to 3-byte video port ground table
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       u8 pEnaVideoPortTable[3]
 *
 *                Each table position 0 to 2 represents a group of 8 port bits:
 *                [0]=7:0, [1]=15:8, [2]=23:16
 *                bitn = '1' means enable port n
 *				bitn = '0' means disable port n
 *				For example, to enable port 0 to 7 only : pEnaVideoPortTable[0]= 0xFF
 *                pEnaVideoPortTable[1]= 0x00, pEnaVideoPortTable[2]= 0x00
 *
 *				u8 pGndVideoPortTable[3]
 *
 *                Each table position 0 to 2 represents a group of 8 port bits:
 *                [0]=7:0, [1]=15:8, [2]=23:16
 *                bitn = '1' means pulldown port n
 *				bitn = '0' means not pulldown port n
 *				For example, to pulldown port 8 to 15 only : pEnaVideoPortTable[0]= 0x00
 *                pEnaVideoPortTable[1]= 0xFF, pEnaVideoPortTable[2]= 0x00
 * */
error_code_t
bsl_set_video_port_config
(
    unit_select_t  tx_unit,
    u8			*p_ena_video_port_table,
    u8			*p_gnd_video_port_table
);

/*============================================================================*/
/**
 *    \brief      Set audio input port (enable, ground)
 *
 *    \param[in]  txUnit                Transmitter unit number
 *    \param[in]  pEnaAudioPortTable    Pointer to 1-byte audio port enable configuration
 *    \param[in]  pGndAudioPortTable    Pointer to 1-byte audio port ground configuration
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 *    \note       u8 pEnaAudioPortTable[1]
 *                bitn = '1' means enable port n
 *				bitn = '0' means disable port n
 *				For example, to enable all audio port (0:7) : pEnaAudioPortTable[0]= 0xFF
 *
 *				u8 pGndAudioPortTable[1]
 *                bitn = '1' means pulldown port n
 *				bitn = '0' means not pulldown port n
 *				For example, to pulldown audio port (0:7) : pEnaAudioPortTable[0]= 0xFF
 * */
error_code_t
bsl_set_audio_port_config
(
    unit_select_t  tx_unit,
    u8			*p_ena_audio_port_table,
    u8			*p_gnd_audio_port_table
);

/*============================================================================*/
/**
 * bslVideoInSetSyncAuto() parameter types
 * */
/** Sync source - was Embedded sync HDMITX_PIXEMBSYNC_ */
typedef enum {
	HDMITX_SYNCSRC_EMBEDDED  = 0,        /**< Embedded sync */
	HDMITX_SYNCSRC_EXT_VREF  = 1,        /**< External sync Vref, Href, Fref */
	HDMITX_SYNCSRC_EXT_VS    = 2,        /**< External sync Vs, Hs */
	HDMITX_SYNCSRC_NO_CHANGE = 3,        /**< No Change     */
	HDMITX_SYNCSRC_INVALID   = 4         /**< Invalid       */
} bsl_sync_source_t;

/**
 *    \brief      Configure video input sync automatically
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  syncSource   Sync Source:
 *                             Embedded, External Vref, External Vs
 *                             No Change
 *    \param[in]  vinFmt       EIA/CEA Video input format: 1 to 31, 0 = No Change
 *    \param[in]  vinMode      Input video mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_in_set_sync_auto
(
    unit_select_t            tx_unit,
    bsl_sync_source_t   sync_source,
    bsl_vid_fmt_t       vin_fmt,
    bsl_vin_mode_t      vin_mode

);

/*============================================================================*/
/**
 * bslVideoInSetSyncManual() parameter types
 * */
/** Video output frame pixel values */
enum _bsl_vout_fine_pixel_limits {
	HDMITX_VOUT_FINE_PIXEL_MIN        = 0x0000,
	HDMITX_VOUT_FINE_PIXEL_MAX        = 0x1FFF,
	HDMITX_VOUT_FINE_PIXEL_NO_CHANGE  = 0x2000,
	HDMITX_VOUT_FINE_PIXEL_INVALID    = 0x2001
};

/** Video output frame line values */
enum _bsl_vout_fine_line_limits {
	HDMITX_VOUT_FINE_LINE_MIN         = 0x0000,
	HDMITX_VOUT_FINE_LINE_MAX         = 0x07FF,
	HDMITX_VOUT_FINE_LINE_NO_CHANGE   = 0x0800,
	HDMITX_VOUT_FINE_LINE_INVALID     = 0x0801
};
/**
 *    \brief      Configure video input sync with manual parameters
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  syncSource   Sync Source:
 *                             Embedded, External Vref, External Vs
 *                             No Change
 *    \param[in]  syncMethod   Sync method: V And H, V And X-DE, No Change
 *    \param[in]  toggleV      VS Toggle:
 *                             No Action, Toggle VS/Vref, No Change
 *    \param[in]  toggleH      HS Toggle:
 *                             No Action, Toggle HS/Href, No Change
 *    \param[in]  toggleX      DE/FREF Toggle:
 *                             No Action, Toggle DE/Fref, No Change
 *    \param[in]  uRefPix      Ref. pixel preset 0 to 1FFFh (2000h = No Change)
 *    \param[in]  uRefLine     Ref. line preset 0 to 7FFh (800h = No Change)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
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
);

/*============================================================================*/
/**
 *    \brief      Enable or disable output video frame
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  bDisable    Enable or disable scaler input
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_out_disable
(
    unit_select_t  tx_unit,
    bool            b_disable
);

/*============================================================================*/
/**
 * bslVideoOutSetConfig() parameter types
 * */
/** Prefilter */
typedef enum {
	HDMITX_VOUT_PREFIL_OFF         = 0,    /**< Off */
	HDMITX_VOUT_PREFIL_121         = 1,    /**< 121 */
	HDMITX_VOUT_PREFIL_109         = 2,    /**< 109 */
	HDMITX_VOUT_PREFIL_CCIR601     = 3,    /**< CCIR601   */
	HDMITX_VOUT_PREFIL_NO_CHANGE   = 4,    /**< No Change */
	HDMITX_VOUT_PREFIL_INVALID     = 5     /**< Invalid   */
} bsl_vout_prefil_t;

/** YUV blanking */
typedef enum {
	HDMITX_VOUT_YUV_BLNK_16        = 0,    /**< 16 */
	HDMITX_VOUT_YUV_BLNK_0         = 1,    /**< 0  */
	HDMITX_VOUT_YUV_BLNK_NO_CHANGE = 2,    /**< No Change */
	HDMITX_VOUT_YUV_BLNK_INVALID   = 3     /**< Invalid   */
} bsl_vout_yuv_blnk_t;

/** Video quantization range */
typedef enum {
	HDMITX_VOUT_QRANGE_FS          = 0,    /**< Full Scale */
	HDMITX_VOUT_QRANGE_RGB_YUV     = 1,    /**< RGB Or YUV */
	HDMITX_VOUT_QRANGE_YUV         = 2,    /**< YUV        */
	HDMITX_VOUT_QRANGE_NO_CHANGE   = 3,    /**< No Change  */
	HDMITX_VOUT_QRANGE_INVALID     = 4     /**< Invalid    */
} bsl_vout_qrange_t;

/**
 *    \brief      Configure sink type, configure video output colour and
 *                quantization, control the downsampler, and force RGB output
 *                and mute audio in DVI mode
 *
 *    \param[in]  txUnit          Transmitter unit number:
 *    \param[in]  sinkType        Sink device type: DVI or HDMI or copy from EDID
 *    \param[in]  voutMode        Video output mode
 *    \param[in]  preFilter       Prefilter: Off, 121, 109, CCIR601, No Change
 *    \param[in]  yuvBlank        YUV blanking: 16, 0, No Change
 *    \param[in]  quantization    Video quantization range:
 *                                Full Scale, RGB Or YUV, YUV, No Change
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_out_set_config
(
    unit_select_t            tx_unit,
    bsl_sink_type_t     sink_type,
    bsl_vout_mode_t     vout_mode,
    bsl_vout_prefil_t   pre_filter,
    bsl_vout_yuv_blnk_t  yuv_blank,
    bsl_vout_qrange_t   quantization
);

/*============================================================================*/
/**
 * bslVideoOutSetSync() parameter types
 * */
/** Video sync source */
typedef enum {
	HDMITX_VSSRC_INTERNAL      = 0,    /**< Internal  */
	HDMITX_VSSRC_EXTERNAL      = 1,    /**< External  */
	HDMITX_VSSRC_NO_CHANGE     = 2,    /**< No change */
	HDMITX_VSSRC_INVALID       = 3     /**< Invalid   */
} bsl_vs_src_t;

/** Video sync toggle */
typedef enum {
	HDMITX_VSTGL_TABLE         = 0,    /**< Vs/Hs polarity from table */
	HDMITX_VSTGL_UNUSED_1      = 1,    /**< Unused          */
	HDMITX_VSTGL_UNUSED_2      = 2,    /**< Unused          */
	HDMITX_VSTGL_UNUSED_3      = 3,    /**< Unused          */
	HDMITX_VSTGL_NO_ACTION     = 4,    /**< No toggle       */
	HDMITX_VSTGL_HS            = 5,    /**< Toggle Hs       */
	HDMITX_VSTGL_VS            = 6,    /**< Toggle Vs       */
	HDMITX_VSTGL_HS_VS         = 7,    /**< Toggle Hs & Vs  */
	HDMITX_VSTGL_NO_CHANGE     = 8,    /**< No change       */
	HDMITX_VSTGL_INVALID       = 9     /**< Invalid         */
} bsl_vs_tgl_t;

/**
 *    \brief      Set video synchronization
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  srcH        Horizontal sync source: Internal, Exter'l, No Change
 *    \param[in]  srcV        Vertical sync source: Internal, Exter'l, No Change
 *    \param[in]  srcX        X sync source: Internal, Exter'l, No Change
 *    \param[in]  toggle      Sync toggle: Hs, Vs, Off, No Change
 *    \param[in]  once        Line/pixel counters sync once or each frame
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_out_set_sync
(
    unit_select_t          tx_unit,
    bsl_vs_src_t      src_h,
    bsl_vs_src_t      src_v,
    bsl_vs_src_t      src_x,
    bsl_vs_tgl_t      toggle,
    bsl_vs_once_t     once
);

/*============================================================================*/
/**
 * bslVideoSetInOut() parameter types
 * */
/** Pixel repetition values */
enum _bsl_pix_repeat {
	HDMITX_PIXREP_NONE       = 0,    /**< No repetition  */
	HDMITX_PIXREP_MIN        = 0,    /**< 1 repetition   */
	HDMITX_PIXREP_MAX        = 9,    /**< 10 repetitions */
	/**< Default repetitions for output format */
	HDMITX_PIXREP_DEFAULT    = 10,
	HDMITX_PIXREP_NO_CHANGE  = 11,   /**< No change */
	HDMITX_PIXREP_INVALID    = 12    /**< Invalid   */
};

/** Matrix modes */
typedef enum {
	HDMITX_MATMODE_OFF       = 0,    /**< Off  */
	HDMITX_MATMODE_AUTO      = 1,    /**< Auto */
	HDMITX_MATMODE_NO_CHANGE = 2,    /**< No change */
	HDMITX_MATMODE_INVALID   = 3     /**< Invalid   */
} bsl_mat_mode_t;

/** Datapath bitwidth */
typedef enum {
	HDMITX_VOUT_DBITS_12           = 0,    /**< 12 bits */
	HDMITX_VOUT_DBITS_8            = 1,    /**< 8 bits  */
	HDMITX_VOUT_DBITS_10           = 2,    /**< 10 bits */
	HDMITX_VOUT_DBITS_NO_CHANGE    = 3,    /**< No change */
	HDMITX_VOUT_DBITS_INVALID      = 4     /**< Invalid   */
} bsl_vout_dbits_t;

/**
 *    \brief      Set main video input and output parameters
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  vinFmt       EIA/CEA Video input format: 1 to 31, 0 = No Change
 *    \param[in]  scaMode      Scaler mode: Off, On, Auto, No Change
 *    \param[in]  voutFmt      EIA/CEA Video output format: 1 to 31, 0 = No Change
 *    \param[in]  uPixelRepeat Pixel repetition factor: 0 to 9, 10 = default,
 *                             11 = no change
 *    \param[in]  matMode      Matrix mode: 0 = off, 1 = auto
 *    \param[in]  datapathBits Datapath bitwidth: 0 to 3 (8, 10, 12, No Change)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_INCONSISTENT_PARAMS: params are inconsistent
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_video_set_in_out
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vin_fmt,
    bsl_sca_mode_t    sca_mode,
    bsl_vid_fmt_t     vout_fmt,
    u8                   u_pixel_repeat,
    bsl_mat_mode_t    mat_mode,
    bsl_vout_dbits_t  datapath_bits,
    bsl_vqr_t        dvi_vqr
);

/**
 *    \brief      Use only for debug to flag the software debug interrupt
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected:
 *                  - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *                  - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *                  - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 * */
error_code_t
bsl_flag_sw_int
(
    unit_select_t	            tx_unit
)
;

/**
 *    \brief Return the category of equipement connected
 *
 *    \param txUnit   Transmitter unit number
 *    \param category return category type
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_HDMI_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_HDMI_INCONSISTENT_PARAMS: params are inconsistent
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED hdcp not started
 *
 * */
error_code_t
bsl_hdcp_get_sink_category
(
    unit_select_t              tx_unit,
    bsl_sink_category_t   *category
);

#endif /* BSLHDMITX_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

