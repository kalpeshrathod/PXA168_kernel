/**
 * Copyright (C) 2007 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          bsl_types.h
 *
 * \version       $Revision: 18 $
 *
 * \date          $Date: 17/03/08 $
 *
 * \brief         HDMI Transmitter common types
 *
 * \section refs  Reference Documents
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History: bsl_types.h $
 *
 *
 * **************** Version 18  ******************
 * User: G.Burnouf     Date: 01/04/08
 * Updated in $/Source/bsl/inc
 * PR1468 : add new function bslTDA9984GetSinkCategory
 *
 *
 * **************** Version 17  ******************
 * User: G.Burnouf     Date: 17/03/08
 * Updated in $/Source/bsl/inc
 * PR1430 : Increase size of table for
 *          Short Audio Descriptor
 *
 * **************** Version 16  ******************
 * User: G.Burnouf     Date: 06/03/08
 * Updated in $/Source/bsl/inc
 * PR1406 : new reset audio fifo sequence
 *
 * **************** Version 15  ******************
 * User: G.Burnouf     Date: 05/02/08
 * Updated in $/Source/bsl/inc
 * PR1251 : add new type for function
 *          bslTDA9984EdidGetBasicDisplayParam
 *
 ******************  version 14  ******************
 * User: G.Burnouf     Date: 14/01/08
 * Updated in $/Source/bsl/inc
 * PR580 - Change BSL error base address
 *
 ******************  version 13  ******************
 * User: G.Burnouf     Date: 10/01/08
 * Updated in $/Source/bsl/inc
 * PR606 - Apply audio port config in function
 *         of audio format
 *
 * **************** Version 12  ******************
 * User: G.Burnouf     Date: 10/12/07   Time: 08:30
 * Updated in $/Source/bsl/inc
 * PR1145 : return DTD and monitor description
 *
 * *****************  Version 11  *****************
 * User: G.Burnouf     Date: 04/12/07
 * Updated in $/Source/bsl/inc
 * PR948 : add new formats, 1080p24/25/30
 *
 * *****************  Version 10 *****************
 * User: C. Diehl	  Date: 27/11/07
 * Updated in $/Source/bsl/inc
 * PR1030 : - Align with the common interface
 *            reworked for the LIPP4200
 *
 * *****************  Version 9  *****************
 * User: J.Lamotte	  Date: 23/11/07   Time: 09:35
 * Updated in $/Source/bsl/src
 * PR1078 : - update HDMI_TX_SVD_MAX_CNT from 30
 *            to 113
 *
 * *****************  Version 8  *****************
 * User: G.Burnouf	  Date: 13/11/07   Time: 09:29
 * Updated in $/Source/bsl/src
 * PR1008 : - update type bslHwFeature_t
 *
 * *****************  Version 7  *****************
 * User: G.Burnouf	  Date: 16/10/07   Time: 14:32
 * Updated in $/Source/bsl/src
 * PR882 : - add type bslPowerState_t
 *         - add type bslPktGmt_t for gamut
 *         - add new interrupt callback for VS
 *
 * *****************  Version 6  *****************
 * User: G.Burnouf	  Date: 05/10/07   Time: 14:32
 * Updated in $/Source/bsl/src
 * PR824 : add type for enum _bslCallbackInt
 *
 * *****************  Version 5  *****************
 * User: J.Turpin	  Date: 13/09/07   Time: 14:32
 * Updated in $/Source/bsl/src
 * PR693 : add black pattern functionality
 *		 - add HDMITX_PATTERN_BLACK in
 *			enum bslTestPattern_t
 *
 * *****************  Version 4  *****************
 * User: G.Burnouf   Date: 06/09/07   Time: 17:22
 * Updated in $/Source/bslTDA9984/Inc
 * PR656 : - add HBR format
 *         - add format I2s Philips left and right justified
 *
 * *****************  Version 3  *****************
 * User: G. Burnouf      Date: 07/08/07   Time: 10:30
 * Updated in $/Source/bslTDA9984/Inc
 * PR572 - change type name of bslTDA9984_ to bsl_
 *
 * *****************  Version 2  *****************
 * User: B.Vereecke      Date: 07/08/07   Time: 10:30
 * Updated in $/Source/bslTDA9984/Inc
 * PR551 - Add a new Pattern type in bslTestPattern_t
 *			it is used for set the bluescreen
 *
 * *****************  Version 1  *****************
 * User: G. Burnouf    Date: 05/07/07   Time: 17:00
 * Updated in $/Source/bslTDA9984/Inc
 * PR 414 : Add new edid management
 *
 *   \endverbatim
 *
 * */

#ifndef BSLHDMITX_TYPES_H
#define BSLHDMITX_TYPES_H

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
#define ERR_HDMI_BASE	CID_BSL_HDMITX

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

/* Ressource not available */
#define ERR_HDMI_RESOURCE_NOT_AVAILABLE   (ERR_HDMI_BASE + 0x00CU)

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

/** Maximum error code defined */
#define ERR_HDMI_MAX              ERR_HDMI_OPERATION_NOT_PERMITTED

/*============================================================================*/

#define HDMITX_ENABLE_VP_TABLE_LEN	3
#define HDMITX_GROUND_VP_TABLE_LEN	3

/** EDID block size */
#define EDID_BLOCK_SIZE		    128

/** size descriptor block of monitor descriptor */
#define EDID_MONITOR_DESCRIPTOR_SIZE   13

/*@}*/

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITIONS                             */
/*============================================================================*/

/**
 * \brief TX IP/IC versions
 * */
typedef enum {
	BSLHDMITX_UNKNOWN = 0x00, /**< IC/IP is not recognized */
	BSLHDMITX_TDA9984,        /**< IC is a TDA9984         */
	/**< IC is a TDA9989 (TDA9989N2 64 balls)         */
	BSLHDMITX_TDA9989,
	BSLHDMITX_TDA9981,        /**< IC is a TDA9981         */
	BSLHDMITX_TDA9983,        /**< IC is a TDA9983         */
	BSLHDMITX_TDA19989        /**< IC is a TDA19989        */
} bsl_version_t;

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

/**
 * \brief EDID function pointer type, to call application EDID read function
 * \param pSysArgs pointer to the structure containing necessary information to read EDID
 * */
typedef error_code_t (FUNC_PTR *pbsl_sys_func_edid_t)
(bsl_sys_args_edid_t *p_sys_args);

/**
 * \brief Timer function pointer type, to call an application timer
 * \param ms delay in milliseconds
 * */
typedef void(FUNC_PTR *pbsl_sys_func_timer_t)(u16 ms);

/*============================================================================*/
/**
 * \brief Callback function pointer type, to call a user interrupt handler
 * \param txUnit: The transmitter unit that interrupted, 0 to max
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

	/**< Format 35 2880 x 480p  60Hz 4:3  */
	hdmitx_vfmt_35_2880x480p_60hz  = 35,
	/**< Format 36 2880 x 480p  60Hz 16:9 */
	hdmitx_vfmt_36_2880x480p_60hz  = 36,
	/**< Format 37 2880 x 576p  50Hz 4:3  */
	hdmitx_vfmt_37_2880x576p_50hz  = 37,
	/**< Format 38 2880 x 576p  50Hz 16:9 */
	hdmitx_vfmt_38_2880x576p_50hz  = 38,

	/**< Highest valid TV format      */
	HDMITX_VFMT_TV_MAX             = 38,
	/**< Lowest TV format without prefetched table */
	HDMITX_VFMT_TV_NO_REG_MIN      = 32,
	/**< Number of TV formats & null  */
	HDMITX_VFMT_TV_NUM             = 39,

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
/**
 * bslTDA9984AudioInSetConfig() parameter types
 * */
/** Audio input formats */
typedef enum {
	HDMITX_AFMT_SPDIF      = 0,         /**< SPDIF               */
	HDMITX_AFMT_I2S        = 1,         /**< I2S                 */
	HDMITX_AFMT_OBA        = 2,         /**< One bit audio / DSD */
	HDMITX_AFMT_DST        = 3,         /**< DST                 */
	HDMITX_AFMT_HBR        = 4,         /**< HBR                 */
	HDMITX_AFMT_NUM        = 5,         /**< Number of Audio input format */
	HDMITX_AFMT_INVALID    = 5          /**< Invalid format      */
} bsla_fmt_t;

/** I2s formats */
typedef enum {
	HDMITX_I2SFOR_PHILIPS_L   = 0,	/**< Philips like format */
	HDMITX_I2SFOR_OTH_L       = 2,	/**< Other non Philips left justified */
	HDMITX_I2SFOR_OTH_R	      = 3,	/**< Other non Philips right justified */
	HDMITX_I2SFOR_INVALID     = 4	/**< Invalid format*/
} bsl_i2s_for_t;

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

/** DST data transfer rates */
typedef enum {
	HDMITX_DSTRATE_SINGLE       = 0,    /**< Single transfer rate */
	HDMITX_DSTRATE_DOUBLE       = 1,    /**< Double data rate */
	HDMITX_DSTRATE_NO_CHANGE    = 2,    /**< No change */
	HDMITX_DSTRATE_INVALID      = 3     /**< Invalid   */
} bsl_dst_rate_t;

/** I2S, SPDIF and DSD channel allocation values */
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
	HDMITX_LATENCY_CURRENT     = 0x080,
	HDMITX_LATENCY_MAX         = 0x0FF,
	HDMITX_LATENCY_NO_CHANGE   = 0x100,
	HDMITX_LATENCY_INVALID     = 0x101
};

/*============================================================================*/
/**
 * bslTDA9984AudioInSetCts() parameter types
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
	HDMITX_AFS_768K          = 7,     /**< 768kHz   */
	HDMITX_AFS_NOT_INDICATED = 8,     /**< Not Indicated (Channel Status) */
	HDMITX_AFS_INVALID       = 8,     /**< Invalid  */
	HDMITX_AFS_NUM           = 8      /**< # rates  */
} bslafs_t;

/** Vertical output frequencies */
typedef enum {
	hdmitx_vfreq_24hz      = 0,     /**< 24Hz          */
	hdmitx_vfreq_25hz      = 1,     /**< 25Hz          */
	hdmitx_vfreq_30hz      = 2,     /**< 30Hz          */
	hdmitx_vfreq_50hz      = 3,     /**< 50Hz          */
	hdmitx_vfreq_59hz      = 4,     /**< 59.94Hz       */
	hdmitx_vfreq_60hz      = 5,     /**< 60Hz          */
#ifndef FORMAT_PC
	HDMITX_VFREQ_INVALID   = 6,     /**< Invalid       */
	HDMITX_VFREQ_NUM       = 6      /**< No. of values */
#else /* FORMAT_PC */
	hdmitx_vfreq_70hz      = 6,     /**< 70Hz          */
	hdmitx_vfreq_72hz      = 7,     /**< 72Hz          */
	hdmitx_vfreq_75hz      = 8,     /**< 75Hz          */
	hdmitx_vfreq_85hz      = 9,     /**< 85Hz          */
	hdmitx_vfreq_87hz      = 10,    /**< 87Hz          */
	HDMITX_VFREQ_INVALID   = 11,    /**< Invalid       */
	HDMITX_VFREQ_NUM       = 11     /**< No. of values */
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

/*============================================================================*/
/**
 * bslTDA9984AudioOutSetChanStatus() parameter types
 * */

typedef enum {
	/**< Main data field represents linear PCM samples.    */
	HDMITX_AUDIO_DATA_PCM     = 0,
	/**< Main data field used for purposes other purposes. */
	HDMITX_AUDIO_DATA_OTHER   = 1,
	HDMITX_AUDIO_DATA_INVALID = 2   /**< Invalid value */
} bsl_audio_data_t;

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

/*============================================================================*/
/**
 * bslTDA9984AudioOutSetChanStatusMapping() parameter types
 * */
/** Channel Status source/channel number limits */
enum _bsl_chan_status_chan_limits {
	HDMITX_CS_CHANNELS_MAX  = 0x0F,
	HDMITX_CS_SOURCES_MAX   = 0x0F
};

/*============================================================================*/
/**
 * bslTDA9984AudioOutSetMute() parameter type
 * */
/** Audio mute state */
typedef enum {
	HDMITX_AMUTE_OFF       = 0,    /**< Mute off */
	HDMITX_AMUTE_ON        = 1,    /**< Mute on  */
	HDMITX_AMUTE_INVALID   = 2     /**< Invalid  */
} bsla_mute_t;

/** Number of 3 byte Short Audio Descriptors stored in pEdidAFmts */
#define HDMI_TX_SAD_MAX_CNT     30

/*============================================================================*/
/**
 * bslTDA9984EdidGetBlockData() parameter types
 * */
/** An enum to represent the current EDID status */
enum _bsl_edid_sta_t {
	HDMITX_EDID_READ                = 0,    /* All blocks read OK */
	/* All blocks read OK but buffer too
	 *	 HDMITX_EDID_READ_INCOMPLETE     = 1,
	 *                                               small to return all of them */
	HDMITX_EDID_ERROR_CHK_BLOCK_0   = 2,    /* Block 0 checksum error */

	/* Block 0 OK, checksum error in one
	 *	 HDMITX_EDID_ERROR_CHK           = 3,
	 *                                               or more other blocks */
	HDMITX_EDID_NOT_READ            = 4,    /* EDID not read */

	HDMITX_EDID_STATUS_INVALID      = 5     /**< Invalid   */

};

/*============================================================================*/
/**
 * bslTDA9984EdidGetSinkType() parameter types
 * */
/** Sink device type */
typedef enum {
	HDMITX_SINK_DVI                = 0,    /**< DVI  */
	HDMITX_SINK_HDMI               = 1,    /**< HDMI */
	HDMITX_SINK_EDID               = 2,    /**< As currently defined in EDID */
	HDMITX_SINK_INVALID            = 3     /**< Invalid   */
} bsl_sink_type_t;

/*============================================================================*/
/**
 * \brief The bslTDA9984EdidGetVideoPreferred() parameter type
 * Detailed timining description structure
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
 * \brief The bslTDA9984EdidGetAudioCapabilities() parameter type
 * */
typedef struct _bsl_edid_sad_t {
	u8   mode_chans;  /* Bits[6:3]: EIA/CEA861 mode; Bits[2:0]: channels */
	u8   freqs;      /* Bits for each supported frequency */
	u8   byte3;      /* EIA/CEA861B p83: data depending on audio mode */
} bsl_edid_sad_t;

/*============================================================================*/
/**
 * \brief struc to store parameter provide by function bslTDA9984EdidRequestBlockData()
 * */
typedef struct _bsl_edid_to_app_t {
	/* pointer on a tab to store edid requested by application */
	u8   *p_raw_edid;
	int     num_blocks;  /* number of edid block requested by application */
} bsl_edid_to_app_t;

/*============================================================================*/
/**
 *  bslTDA9984EdidGetVideoCapabilities() parameter types
 * */
/** Number of 1 byte Short Video Descriptors stored in pEdidVFmts */
#define HDMI_TX_SVD_MAX_CNT     113

/** number of detailed timing descriptor stored in BSL */
#define NUMBER_DTD_STORED       10

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

/*============================================================================*/
/**
 * bslTDA9984HdcpCheck() parameter type
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

/*============================================================================*/
/**
 * bslTDA9984HdcpConfigure() parameter type
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

/*============================================================================*/
/**
 * bslTDA9984HdcpDownloadKeys() parameter type
 * */
/** HDCP decryption mode */
typedef enum {
	HDMITX_HDCP_DECRYPT_DISABLE  = 0,
	HDMITX_HDCP_DECRYPT_ENABLE   = 1,
	HDMITX_HDCP_DECRYPT_MAX      = 1
} bsl_decrypt_t;

/*============================================================================*/
/**
 * bslTDA9984HdcpHandleBSTATUS() parameter type
 * */
/** BSTATUS bit fields */
enum _bsl_hdcp_handle_bstatus {
	HDMITX_HDCP_BSTATUS_HDMI_MODE               = 0x1000,
	HDMITX_HDCP_BSTATUS_MAX_CASCADE_EXCEEDED    = 0x0800,
	HDMITX_HDCP_BSTATUS_CASCADE_DEPTH           = 0x0700,
	HDMITX_HDCP_BSTATUS_MAX_DEVS_EXCEEDED       = 0x0080,
	HDMITX_HDCP_BSTATUS_DEVICE_COUNT            = 0x007F
};

/*============================================================================*/
/**
 * bslTDA9984HdcpHandleSHA_1() parameter types
 * */
/** KSV list sizes */
enum _bsl_hdcp_handle_sha_1 {
	HDMITX_KSV_LIST_MAX_DEVICES = 128,
	HDMITX_KSV_BYTES_PER_DEVICE = 5
};

/*============================================================================*/
/**
 * bslTDA9984HotPlugGetStatus() parameter type
 * */
/** Current hotplug status */
typedef enum {
	HDMITX_HOTPLUG_INACTIVE    = 0,    /**< Hotplug inactive */
	HDMITX_HOTPLUG_ACTIVE      = 1,    /**< Hotplug active   */
	HDMITX_HOTPLUG_INVALID     = 2     /**< Invalid Hotplug  */
} bsl_hot_plug_t;

/*============================================================================*/
/**
 * bslTDA9984RxSenseGetStatus() parameter type
 * */
/** Current RX Sense status */
typedef enum {
	HDMITX_RX_SENSE_INACTIVE    = 0,    /**< RxSense inactive */
	HDMITX_RX_SENSE_ACTIVE      = 1,    /**< RxSense active   */
	HDMITX_RX_SENSE_INVALID     = 2     /**< Invalid RxSense  */
} bsl_rx_sense_t;

/*============================================================================*/
/**
 * bslTDA9984HwGetCapabilities() parameter type
 * */
/** List of HW features that may be supported by HW */
typedef enum {
	HDMITX_FEATURE_HW_HDCP             = 0,  /**< HDCP   feature */
	HDMITX_FEATURE_HW_SCALER           = 1,  /**< Scaler feature */
	HDMITX_FEATURE_HW_AUDIO_OBA        = 2,  /**< One Bit Audio feature */
	HDMITX_FEATURE_HW_AUDIO_DST        = 3,  /**< DST Audio feature */
	HDMITX_FEATURE_HW_AUDIO_HBR        = 4,  /**< HBR Audio feature */
	HDMITX_FEATURE_HW_HDMI_1_1         = 5,  /**< HDMI 1.1 feature */
	HDMITX_FEATURE_HW_HDMI_1_2A        = 6,  /**< HDMI 1.2a feature */
	HDMITX_FEATURE_HW_HDMI_1_3A        = 7,  /**< HDMI 1.3a feature */
	HDMITX_FEATURE_HW_DEEP_COLOR_30    = 8,  /**< 30 bits deep color support */
	HDMITX_FEATURE_HW_DEEP_COLOR_36    = 9,  /**< 36 bits deep color support */
	HDMITX_FEATURE_HW_DEEP_COLOR_48    = 11, /**< 48 bits deep color support */
	HDMITX_FEATURE_HW_UPSAMPLER        = 12, /**< Up sampler feature */
	HDMITX_FEATURE_HW_DOWNSAMPLER      = 13, /**< Down sampler feature */
	HDMITX_FEATURE_HW_COLOR_CONVERSION = 14  /**< Color conversion matrix */
} bsl_hw_feature_t;

/*============================================================================*/
/**
 * bslTDA9984Init() parameter types
 * */
/** Supported range of I2C slave addresses */
enum _bsl_slave_address {
	HDMITX_SLAVE_ADDRESS_MIN    = 1,
	HDMITX_SLAVE_ADDRESS_MAX    = 127
};

/**
 * Indexes into the funcCallback[] array of interrupt callback function pointers
 * */
typedef enum _bsl_callback_int {
	/**< HDCP encryption switched off     */
	HDMITX_CALLBACK_INT_SECURITY        = 0,
	/**< HDCP encrypt as above (Obsolete) */
	HDMITX_CALLBACK_INT_ENCRYPT         = 0,
	/**< Transition on HPD input          */
	HDMITX_CALLBACK_INT_HPD             = 1,
	/**< HDCP state machine in state T0   */
	HDMITX_CALLBACK_INT_T0              = 2,
	/**< BCAPS available                  */
	HDMITX_CALLBACK_INT_BCAPS           = 3,
	/**< BSTATUS available                */
	HDMITX_CALLBACK_INT_BSTATUS         = 4,
	/**< sha-1(ksv,bstatus,m0)=V'         */
	HDMITX_CALLBACK_INT_SHA_1           = 5,
	/**< pj=pj' check fails               */
	HDMITX_CALLBACK_INT_PJ              = 6,
	/**< R0 interrupt                     */
	HDMITX_CALLBACK_INT_R0              = 7,
	/**< SW DEBUG interrupt               */
	HDMITX_CALLBACK_INT_SW_INT          = 8,
	/**< RX SENSE interrupt               */
	HDMITX_CALLBACK_INT_RX_SENSE        = 9,
	/**< EDID BLK READ interrupt          */
	HDMITX_CALLBACK_INT_EDID_BLK_READ   = 10,
	/**< Pll Lock (Serial or Formatter)   */
	HDMITX_CALLBACK_INT_PLL_LOCK        = 11,
	/**< VS Interrupt for Gamut packets   */
	HDMITX_CALLBACK_INT_VS_RPT          = 12,
	/**< Number of callbacks              */
	HDMITX_CALLBACK_INT_NUM             = 13
} bsl_callback_int_t;

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
 * \brief The bslTDA9984Init() parameter structure
 * */
typedef struct _bsl_callback_list_t {
	/** Interrupt callback function pointers (each ptr if null = not used) */
	pbsl_callback_t func_callback[HDMITX_CALLBACK_INT_NUM];

} bsl_callback_list_t;

/*============================================================================*/
/**
 * bslTDA9984MatrixSetCoeffs() parameter type
 * */
/** Parameter structure array size */
enum _bsl_mat_coeff {
	HDMITX_MAT_COEFF_NUM = 9
};

/** \brief The bslTDA9984MatrixSetCoeffs() parameter structure */
/** Array of coefficients (values -1024 to +1023) */
typedef struct _bsl_mat_coeff_t {
	/** Array of coefficients (values -1024 to +1023) */
	s16 coeff[HDMITX_MAT_COEFF_NUM];
} bsl_mat_coeff_t;

/*============================================================================*/
/**
 * bslTDA9984MatrixSetConversion() parameter type
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

/*============================================================================*/
/**
 * bslTDA9984MatrixSetMode() parameter types
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

/*============================================================================*/
/**
 * Data Island Packet structure
 * */
/** Parameter structure array sizes */
enum _bsl_pkt {
	HDMITX_PKT_DATA_BYTE_CNT = 28
};

/** \brief The parameter structure for bslTDA9984Pkt*() APIs */
typedef struct _bsl_pkt_t {
	u8 data_byte[HDMITX_PKT_DATA_BYTE_CNT];       /**< Packet Data   */
} bsl_pkt_t;

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

/*============================================================================*/
/**
 * bslTDA9984PktSetMpegInfoframe() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984ScalerGetMode() parameter types
 * */
/** Scaler modes */
typedef enum {
	HDMITX_SCAMODE_OFF       = 0,    /**< Off  */
	HDMITX_SCAMODE_ON        = 1,    /**< On   */
	HDMITX_SCAMODE_AUTO      = 2,    /**< Auto */
	HDMITX_SCAMODE_NO_CHANGE = 3,    /**< No change */
	HDMITX_SCAMODE_INVALID   = 4     /**< Invalid   */
} bsl_sca_mode_t;

/*============================================================================*/
/**
 * \brief The bslTDA9984ScalerGet() parameter type
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

/*============================================================================*/
/**
 * bslTDA9984ScalerSetCoeffs() parameter types
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
/*============================================================================*/
/**
 * bslTDA9984ScalerSetFieldOrder() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984ScalerSetPhase() parameter types
 * */
/** Phases_h values */
typedef enum {
	HDMITX_H_PHASES_16        = 0,    /**< 15 horizontal phases */
	HDMITX_H_PHASES_15        = 1,    /**< 16 horizontal phases */
	HDMITX_H_PHASES_INVALID   = 2     /**< Invalid   */
} bsl_hphases_t;

/*============================================================================*/
/**
 * bslTDA9984ScalerSetFine() parameter types
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
/*============================================================================*/
/**
 * bslTDA9984ScalerSetSync() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984TmdsSetOutputs() parameter types
 * */
/** TMDS output mode */
typedef enum {
	HDMITX_TMDSOUT_NORMAL      = 0,    /**< Normal outputs   */
	HDMITX_TMDSOUT_NORMAL1     = 1,    /**< Normal outputs, same as 0  */
	HDMITX_TMDSOUT_FORCED0     = 2,    /**< Forced 0 outputs */
	HDMITX_TMDSOUT_FORCED1     = 3,    /**< Forced 1 outputs */
	HDMITX_TMDSOUT_INVALID     = 4     /**< Invalid          */
} bsl_tmds_out_t;

/*============================================================================*/
/**
 * bslTDA9984TmdsSetSerializer() parameter types
 * */
/** Serializer phase limits */
enum _bsl_tmds_phase {
	HDMITX_TMDSPHASE_MIN       = 0,
	HDMITX_TMDSPHASE_MAX       = 15,
	HDMITX_TMDSPHASE_INVALID   = 16
};

/*============================================================================*/
/**
 * bslTDA9984TestSetPattern() parameter types
 * */
/** Test pattern types */
typedef enum {
	HDMITX_PATTERN_OFF     = 0, /**< Insert test pattern     */
	HDMITX_PATTERN_CBAR4   = 1, /**< Insert 4-bar colour bar */
	HDMITX_PATTERN_CBAR8   = 2, /**< Insert 8-bar colour bar */
	HDMITX_PATTERN_BLUE	   = 3, /**< Insert Blue screen		 */
	HDMITX_PATTERN_BLACK   = 4, /**< Insert Blue screen		 */
	HDMITX_PATTERN_INVALID = 5  /**< Invalid pattern		 */
} bsl_test_pattern_t;

/*============================================================================*/
/**
 * bslTDA9984TestSetMode() parameter types
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
	HDMITX_TESTMODE_INVALID = 7 /**< Invalid test */
} bsl_test_mode_t;

/** Test states */
typedef enum {
	HDMITX_TESTSTATE_OFF     = 0,  /**< Disable the selected test */
	HDMITX_TESTSTATE_ON      = 1,  /**< Enable the selected test  */
	HDMITX_TESTSTATE_INVALID = 2   /**< Invalid value */
} bsl_test_state_t;

/*============================================================================*/
/**
 * bslTDA9984VideoInSetBlanking() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoInSetConfig() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoInSetFine() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoInSetMapping() parameter types
 * */
/** Video input port parameter structure array size and limits */
enum _bsl_vin_port_map {
	HDMITX_VIN_PORT_MAP_TABLE_LEN    = 6,

	HDMITX_VIN_PORT_SWAP_NO_CHANGE   = 6,
	HDMITX_VIN_PORT_SWAP_INVALID     = 7,

	HDMITX_VIN_PORT_MIRROR_NO_CHANGE = 2,
	HDMITX_VIN_PORT_MIRROR_INVALID   = 3
};

/*============================================================================*/
/**
 * bslTDA9984VideoInSetSyncAuto() parameter types
 * */
/** Sync source - was Embedded sync HDMITX_PIXEMBSYNC_ */
typedef enum {
	HDMITX_SYNCSRC_EMBEDDED  = 0,        /**< Embedded sync */
	HDMITX_SYNCSRC_EXT_VREF  = 1,        /**< External sync Vref, Href, Fref */
	HDMITX_SYNCSRC_EXT_VS    = 2,        /**< External sync Vs, Hs */
	HDMITX_SYNCSRC_NO_CHANGE = 3,        /**< No Change     */
	HDMITX_SYNCSRC_INVALID   = 4         /**< Invalid       */
} bsl_sync_source_t;

/*============================================================================*/
/**
 * bslTDA9984VideoInSetSyncManual() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoOutSetConfig() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoOutSetSync() parameter types
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

/*============================================================================*/
/**
 * bslTDA9984VideoSetInOut() parameter types
 * */
/** Pixel repetition values */
enum _bsl_pix_repeat {
	HDMITX_PIXREP_NONE       = 0,    /**< No repetition  */
	HDMITX_PIXREP_MIN        = 0,    /**< 1 repetition   */

	HDMITX_PIXREP_0 = 0,
	HDMITX_PIXREP_1 = 1,
	HDMITX_PIXREP_2 = 2,
	HDMITX_PIXREP_3 = 3,
	HDMITX_PIXREP_4 = 4,
	HDMITX_PIXREP_5 = 5,
	HDMITX_PIXREP_6 = 6,
	HDMITX_PIXREP_7 = 7,
	HDMITX_PIXREP_8 = 8,
	HDMITX_PIXREP_9 = 9,

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

/** Color depth */
typedef enum {
	HDMITX_COLORDEPTH_24           = 0,    /**< 24 bits per pixel */
	HDMITX_COLORDEPTH_30           = 1,    /**< 30 bits per pixel */
	HDMITX_COLORDEPTH_36           = 2,    /**< 36 bits per pixel */
	HDMITX_COLORDEPTH_48           = 3,    /**< 48 bits per pixel */
	HDMITX_COLORDEPTH_NO_CHANGE    = 4,    /**< No change */
	HDMITX_COLORDEPTH_INVALID      = 5     /**< Invalid   */
} bsl_color_depth;

/*============================================================================*/
/**
 * bslTDA9984MatrixSetInputOffset() parameter type
 * */
/** Parameter structure array size */
enum _bsl_mat_offset {
	HDMITX_MAT_OFFSET_NUM = 3
};

/** \brief The bslTDA9984MatrixSetInputOffset() parameter structure */
typedef struct _bsl_mat_offset_t {
	/** Offset array  (values -1024 to +1023) */
	s16 offset[HDMITX_MAT_OFFSET_NUM];
} bsl_mat_offset_t;

/** Matrix numeric limits */
enum _bsl_mat_limits {
	HDMITX_MAT_OFFSET_MIN      = -1024,
	HDMITX_MAT_OFFSET_MAX      = 1023
};

/*============================================================================*/
/**
 * tdaPowerSetState() and tdaPowerGetState() parameter types
 * */
typedef enum {
	HDMITX_POWER_STATE_STAND_BY             = 0,   /**< Stand by mode        */
	HDMITX_POWER_STATE_SLEEP_MODE           = 1,   /**< Sleep mode           */
	HDMITX_POWER_STATE_ON                   = 2,   /**< On mode      */
	HDMITX_POWER_STATE_INVALID              = 3    /**< Invalid format       */
} bsl_power_state_t;

/**
 * \brief Structure describing gamut metadata packet (P0 or P1 profiles)
 * */
typedef struct {
	u8  HB[3];  /**< Header bytes (HB0, HB1 & HB2) */
	u8  PB[28]; /**< Payload bytes 0..27 */
} bsl_pkt_gamut_t;

/**
 * \brief Structure describing RAW AVI infoframe
 * */
typedef struct {
	u8  HB[3];  /**< Header bytes (HB0, HB1 & HB2) */
	u8  PB[28]; /**< Payload bytes 0..27 */
} bsl_pkt_raw_avi_t;

/** Sink category */
typedef enum {
	HDMITX_SINK_CAT_NOT_REPEATER    = 0, /**< Not repeater  */
	HDMITX_SINK_CAT_REPEATER        = 1, /**< repeater      */
	HDMITX_SINK_CAT_INVALID         = 3  /**< Invalid       */
} bsl_sink_category_t;

typedef struct {
	bool   latency_available;
	bool   ilatency_available;
	u8  edidvideo_latency;
	u8  edidaudio_latency;
	u8  edid_ivideo_latency;
	u8  edid_iaudio_latency;

} bsl_edid_latency_t;

/**
 * \brief Structure defining additional VSDB data
 * */
typedef struct {
	u8 max_tmds_clock;      /* maximum supported TMDS clock */
	u8 cnc0;              /* content type Graphics (text) */
	u8 cnc1;              /* content type Photo */
	u8 cnc2;              /* content type Cinema */
	u8 cnc3;              /* content type Game */
	u8 hdmi_video_present;  /* additional video format */
	u8 h3dpresent;        /* 3D support by the HDMI Sink */
	u8 h3dmulti_present;   /* 3D multi strctures present */
	/* additional info for the values in the image size area */
	u8 image_size;
	u8 hdmi3dlen;         /* total length of 3D video formats */
	u8 hdmi_vic_len;        /* total length of extended video formats */
	u8 ext3ddata[21];     /* max_len-10, ie: 31-10=21 */
} bsl_edid_extra_vsdb_data_t;

/**
 * \brief Enum defining possible quantization range
 * */
typedef enum {
	HDMITX_VQR_DEFAULT = 0, /* Follow HDMI spec. */
	HDMITX_RGB_FULL    = 1, /* Force RGB FULL , DVI only */
	HDMITX_RGB_LIMITED = 2  /* Force RGB LIMITED , DVI only */
} bsl_vqr_t;

#endif /* BSLHDMITX_TYPES_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

