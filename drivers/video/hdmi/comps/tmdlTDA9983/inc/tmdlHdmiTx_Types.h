/**
 * copyright (C) 2007 NXP N.V., all rights reserved.
 * this source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. under no circumstances
 * is this software to be  exposed to or placed under an open source license of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_types.h
 *
 * \version       $revision: 1 $
 *
 * \date          $date: 02/08/07 08:32 $
 *
 * \brief         devlib driver component API for the tda_998x HDMI transmitters
 *
 * \section refs  reference documents
 * HDMI tx driver - FRS.doc,
 * HDMI tx driver - tx - SCS.doc
 *
 * \section info  change information
 *
 * \verbatim
 *
 *  $history: tx_types.h $
 *
 * *****************  version 1  *****************
 * user: demoment     date: 02/08/07   time: 08:32
 * updated in $/source/tx/inc
 * initial version
 *
 *  \endverbatim
 *
 * */

#ifndef DLHDMITX_TYPES_H
#define DLHDMITX_TYPES_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#include "tmNxTypes.h"

/*============================================================================*/
/*                       MACRO DEFINITIONS                                    */
/*============================================================================*/

/*============================================================================*/
/*                                DEFINES                                     */
/*============================================================================*/

/**< Error Codes */
#define ERR_DLHDMITX_BASE                      0x100
#define ERR_DLHDMITX_COMPATIBILITY             (ERR_DLHDMITX_BASE + ERR_COMPATIBILITY)             /**< SW Interface compatibility   */
#define ERR_DLHDMITX_MAJOR_VERSION             (ERR_DLHDMITX_BASE + ERR_MAJOR_VERSION)             /**< SW Major Version error       */
#define ERR_DLHDMITX_COMP_VERSION              (ERR_DLHDMITX_BASE + ERR_COMP_VERSION)              /**< SW component version error   */
#define ERR_DLHDMITX_BAD_UNIT_NUMBER           (ERR_DLHDMITX_BASE + ERR_BAD_UNIT_NUMBER)           /**< Invalid device unit number   */
#define ERR_DLHDMITX_BAD_INSTANCE              (ERR_DLHDMITX_BASE + ERR_BAD_INSTANCE)              /**< Bad input instance value     */
#define ERR_DLHDMITX_BAD_HANDLE                (ERR_DLHDMITX_BASE + ERR_BAD_HANDLE)                /**< Bad input handle             */
#define ERR_DLHDMITX_BAD_PARAMETER             (ERR_DLHDMITX_BASE + ERR_BAD_PARAMETER)             /**< Invalid input parameter      */
#define ERR_DLHDMITX_NO_RESOURCES              (ERR_DLHDMITX_BASE + ERR_NO_RESOURCES)              /**< Resource is not available    */
#define ERR_DLHDMITX_RESOURCE_OWNED            (ERR_DLHDMITX_BASE + ERR_RESOURCE_OWNED)            /**< Resource is already in use   */
#define ERR_DLHDMITX_RESOURCE_NOT_OWNED        (ERR_DLHDMITX_BASE + ERR_RESOURCE_NOT_OWNED)        /**< Caller does not own resource */
#define ERR_DLHDMITX_INCONSISTENT_PARAMS       (ERR_DLHDMITX_BASE + ERR_INCONSISTENT_PARAMS)       /**< Inconsistent input params    */
#define ERR_DLHDMITX_NOT_INITIALIZED           (ERR_DLHDMITX_BASE + ERR_NOT_INITIALIZED)           /**< Component is not initialized */
#define ERR_DLHDMITX_NOT_SUPPORTED             (ERR_DLHDMITX_BASE + ERR_NOT_SUPPORTED)             /**< Function is not supported    */
#define ERR_DLHDMITX_INIT_FAILED               (ERR_DLHDMITX_BASE + ERR_INIT_FAILED)               /**< Initialization failed        */
#define ERR_DLHDMITX_BUSY                      (ERR_DLHDMITX_BASE + ERR_BUSY)                      /**< Component is busy            */
#define ERR_DLHDMITX_I2C_READ                  (ERR_DLHDMITX_BASE + ERR_READ)                      /**< Read error                   */
#define ERR_DLHDMITX_I2C_WRITE                 (ERR_DLHDMITX_BASE + ERR_WRITE)                     /**< Write error                  */
#define ERR_DLHDMITX_FULL                      (ERR_DLHDMITX_BASE + ERR_FULL)                      /**< Queue is full                */
#define ERR_DLHDMITX_NOT_STARTED               (ERR_DLHDMITX_BASE + ERR_NOT_STARTED)               /**< Function is not started      */
#define ERR_DLHDMITX_ALREADY_STARTED           (ERR_DLHDMITX_BASE + ERR_ALREADY_STARTED)           /**< Function is already started  */
#define ERR_DLHDMITX_ASSERTION                 (ERR_DLHDMITX_BASE + ERR_ASSERTION)                 /**< Assertion failure            */
#define ERR_DLHDMITX_INVALID_STATE             (ERR_DLHDMITX_BASE + ERR_INVALID_STATE)             /**< Invalid state for function   */
#define ERR_DLHDMITX_OPERATION_NOT_PERMITTED   (ERR_DLHDMITX_BASE + ERR_OPERATION_NOT_PERMITTED)   /**< Corresponds to posix EPERM   */
#define ERR_DLHDMITX_RESOLUTION_UNKNOWN        (ERR_DLHDMITX_BASE + ERR_BAD_FORMAT)                /**< Bad format                   */

/* Defined here for tx_CmpTst */
#define ERR_DLHDMITX_COMP                      ERR_DLHDMITX_BASE | ERR_COMP_UNIQUE_START
#define DL_DLHDMITX_HDCP_SECURE                   (ERR_DLHDMITX_COMP + 0x0001) /**< Revocation list is secure */
#define DL_DLHDMITX_HDCP_NOT_SECURE               (ERR_DLHDMITX_COMP + 0x0002) /**< Revocation list is NOT secure */

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITIONS                             */
/*============================================================================*/

/**
 * \brief Enum listing all events that can be signalled to application
 * */
typedef enum {
	/**< HDCP encryption status switched to active */
	DL_HDMITX_HDCP_ACTIVE         = 0,
	/**< HDCP encryption status switched to inactive */
	DL_HDMITX_HDCP_INACTIVE       = 1,
	/**< Hotplug status switched to active */
	DL_HDMITX_HPD_ACTIVE          = 2,
	/**< Hotplug status switched to inactive */
	DL_HDMITX_HPD_INACTIVE        = 3,
	DL_HDMITX_RX_KEYS_RECEIVED    = 4,    /**< Receiver(s) key(s) received */
	/**< Rx device is connected and active */
	DL_HDMITX_RX_DEVICE_ACTIVE    = 5,
	/**< Rx device is connected but inactive (standby) */
	DL_HDMITX_RX_DEVICE_INACTIVE  = 6,
	DL_HDMITX_EDID_RECEIVED       = 7,    /**< EDID has been received */
	/**< VS interrupt has been received */
	DL_HDMITX_VS_RPT_RECEIVED     = 8
} tx_event_t;

/**
 * \brief Enum listing all available event status
 * */
typedef enum {
	DL_HDMITX_EVENT_ENABLED,  /**< Event is enabled */
	DL_HDMITX_EVENT_DISABLED  /**< Event is disabled */
} tx_event_status_t;

/**
 * \brief Callback function pointer type, used to allow driver to callback
 *          application when activity status is changing at input.
 * \param Event Identifier of the source event.
 * */
typedef void (*ptx_callback_t)(tx_event_t event);

/**
 * \brief Enum listing all supported device versions
 * */
typedef enum {
	DL_HDMITX_DEVICE_UNKNOWN,  /**< HW device is unknown */
	DL_HDMITX_DEVICE_TDA9984,  /**< HW device is IC TDA9984 */
	DL_HDMITX_DEVICE_TDA9989,  /**< HW device is IC TDA9989 */
	DL_HDMITX_DEVICE_LIPP4200, /**< HW device is IP LIPP4200 */
	DL_HDMITX_DEVICE_TDA9981,  /**< HW device is IC TDA9981 */
	DL_HDMITX_DEVICE_TDA9983   /**< HW device is IC TDA9983 */
} tx_device_version_t;

/**
 * \brief Enum defining the supported HDMI standard version
 * */
typedef enum {
	DL_HDMITX_HDMI_VERSION_UNKNOWN, /**< Unknown   */
	DL_HDMITX_HDMI_VERSION_1_1,     /**< HDMI 1.1  */
	dl_hdmitx_hdmi_version_1_2a,    /**< HDMI 1.2a */
	dl_hdmitx_hdmi_version_1_3a     /**< HDMI 1.3  */
} tx_hdmi_version_t;

/**
 * \brief Enum listing all color depth (8 bits/color, 10 bits/color, etc.)
 * */
typedef enum {
	DL_HDMITX_COLORDEPTH_24   = 0,    /**< 8 bits per color */
	DL_HDMITX_COLORDEPTH_30   = 1,    /**< 10 bits per color */
	DL_HDMITX_COLORDEPTH_36   = 2,    /**< 12 bits per color */
	DL_HDMITX_COLORDEPTH_48   = 3,    /**< 16 bits per color */
} tx_color_depth_t;

/**
 * \brief Enum defining the EDID Status
 * */
typedef enum {
	DL_HDMITX_EDID_READ                = 0,   /**< All blocks read OK */
	/**< All blocks read OK but buffer too small to return all of them */
	DL_HDMITX_EDID_READ_INCOMPLETE     = 1,
	DL_HDMITX_EDID_ERROR_CHK_BLOCK_0   = 2,   /**< Block 0 checksum error */
	/**< Block 0 OK, checksum error in one or more other blocks */
	DL_HDMITX_EDID_ERROR_CHK           = 3,
	DL_HDMITX_EDID_NOT_READ            = 4,   /**< EDID not read */
	DL_HDMITX_EDID_STATUS_INVALID      = 5    /**< Invalid   */
} tx_edid_status_t;

/**
 * \brief Structure defining the supported audio packets
 * */
typedef struct {
	bool HBR;              /**< High Bitrate Audio packet */
	bool DST;              /**< Direct Stream Transport audio packet */
	bool one_bit_audio;      /**< One Bit Audio sample packet */
} tx_audio_packet_t;

/**
 * \brief Enum listing all possible audio input formats
 * */
typedef enum {
	DL_HDMITX_AFMT_SPDIF      = 0, /**< SPDIF */
	DL_HDMITX_AFMT_I2S        = 1, /**< I2S */
	DL_HDMITX_AFMT_OBA        = 2, /**< One bit audio / DSD */
	DL_HDMITX_AFMT_DST        = 3, /**< DST */
	DL_HDMITX_AFMT_HBR        = 4  /**< HBR */
} tx_audio_format_t;

/**
 * \brief Enum listing all possible audio input sample rates
 * */
typedef enum {
	DL_HDMITX_AFS_32K           = 0, /**< 32kHz    */
	DL_HDMITX_AFS_44K           = 1, /**< 44.1kHz  */
	DL_HDMITX_AFS_48K           = 2, /**< 48kHz    */
	DL_HDMITX_AFS_88K           = 3, /**< 88.2kHz  */
	DL_HDMITX_AFS_96K           = 4, /**< 96kHz    */
	DL_HDMITX_AFS_176K          = 5, /**< 176.4kHz */
	DL_HDMITX_AFS_192K          = 6  /**< 192kHz   */
} tx_audio_rate_t;

/**
 * \brief Enum listing all possible audio input sample rates
 * */
typedef enum {
	DL_HDMITX_I2SQ_16BITS       = 16, /**< 16 bits */
	DL_HDMITX_I2SQ_32BITS       = 32, /**< 32 bits */
	DL_HDMITX_I2SQ_OTHERS       = 0,  /**< for SPDIF and DSD */
} tx_audio_i2squalifier_t;

/**
 * \brief Enum listing all possible audio I2S formats
 * */
typedef enum {
	DL_HDMITX_I2SFOR_PHILIPS_L   = 0,	/**< Philips like format */
	/**< Other non Philips left justified */
	DL_HDMITX_I2SFOR_OTH_L       = 2,
	/**< Other non Philips right justified */
	DL_HDMITX_I2SFOR_OTH_R	   = 3,
	DL_HDMITX_I2SFOR_INVALID     = 4	/**< Invalid format */
} tx_audio_i2sformat_t;

/**
 * \brief Enum listing all possible DST data transfer rates
 * */
typedef enum {
	DL_HDMITX_DSTRATE_SINGLE  = 0,    /**< Single transfer rate */
	DL_HDMITX_DSTRATE_DOUBLE  = 1     /**< Double data rate */
} tx_dst_rate_t;

/**
 * \brief Structure describing unit capabilities
 * */
typedef struct {
	tx_device_version_t   device_version;  /**< HW device version */
	/**< Supported HDMI standard version  */
	tx_hdmi_version_t     hdmi_version;
	tx_audio_packet_t     audio_packet;    /**< Supported audio packets */
	tx_color_depth_t      color_depth;     /**< Supported color depth */
	/**< Supported Hdcp encryption (true/false) */
	bool                        hdcp;
	/**< Supported scaler (true/false) */
	bool                        scaler;
} tx_capabilities_t;

/**
 * \brief Structure gathering all instance setup parameters
 * */
typedef struct {
	bool    simplay_hd;          /**< Enable simplayHD support */
	bool    repeater_enable;     /**< Enable repeater mode */
	u8   *p_edid_buffer;       /**< Pointer to raw EDID data */
	u32  edid_buffer_size;     /**< Size of buffer for raw EDID data */
} tx_instance_setup_info_t;

/**
 * \brief Enum listing all IA/CEA 861-D video formats
 * */
typedef enum {
	/**< Not a valid format...        */
	DL_HDMITX_VFMT_NULL               = 0,
	/**< ...or no change required     */
	DL_HDMITX_VFMT_NO_CHANGE          = 0,
	/**< Lowest valid format          */
	DL_HDMITX_VFMT_MIN                = 1,
	/**< Lowest valid TV format       */
	DL_HDMITX_VFMT_TV_MIN             = 1,
	/**< Format 01 640  x 480p  60Hz  */
	dl_hdmitx_vfmt_01_640x480p_60hz   = 1,
	/**< Format 02 720  x 480p  60Hz  */
	dl_hdmitx_vfmt_02_720x480p_60hz   = 2,
	/**< Format 03 720  x 480p  60Hz  */
	dl_hdmitx_vfmt_03_720x480p_60hz   = 3,
	/**< Format 04 1280 x 720p  60Hz  */
	dl_hdmitx_vfmt_04_1280x720p_60hz  = 4,
	/**< Format 05 1920 x 1080i 60Hz  */
	dl_hdmitx_vfmt_05_1920x1080i_60hz = 5,
	/**< Format 06 720  x 480i  60Hz  */
	dl_hdmitx_vfmt_06_720x480i_60hz   = 6,
	/**< Format 07 720  x 480i  60Hz  */
	dl_hdmitx_vfmt_07_720x480i_60hz   = 7,
	/**< Format 08 720  x 240p  60Hz  */
	dl_hdmitx_vfmt_08_720x240p_60hz   = 8,
	/**< Format 09 720  x 240p  60Hz  */
	dl_hdmitx_vfmt_09_720x240p_60hz   = 9,
	/**< Format 10 720  x 480i  60Hz  */
	dl_hdmitx_vfmt_10_720x480i_60hz   = 10,
	/**< Format 11 720  x 480i  60Hz  */
	dl_hdmitx_vfmt_11_720x480i_60hz   = 11,
	/**< Format 12 720  x 240p  60Hz  */
	dl_hdmitx_vfmt_12_720x240p_60hz   = 12,
	/**< Format 13 720  x 240p  60Hz  */
	dl_hdmitx_vfmt_13_720x240p_60hz   = 13,
	/**< Format 14 1440 x 480p  60Hz  */
	dl_hdmitx_vfmt_14_1440x480p_60hz  = 14,
	/**< Format 15 1440 x 480p  60Hz  */
	dl_hdmitx_vfmt_15_1440x480p_60hz  = 15,
	/**< Format 16 1920 x 1080p 60Hz  */
	dl_hdmitx_vfmt_16_1920x1080p_60hz = 16,
	/**< Format 17 720  x 576p  50Hz  */
	dl_hdmitx_vfmt_17_720x576p_50hz   = 17,
	/**< Format 18 720  x 576p  50Hz  */
	dl_hdmitx_vfmt_18_720x576p_50hz   = 18,
	/**< Format 19 1280 x 720p  50Hz  */
	dl_hdmitx_vfmt_19_1280x720p_50hz  = 19,
	/**< Format 20 1920 x 1080i 50Hz  */
	dl_hdmitx_vfmt_20_1920x1080i_50hz = 20,
	/**< Format 21 720  x 576i  50Hz  */
	dl_hdmitx_vfmt_21_720x576i_50hz   = 21,
	/**< Format 22 720  x 576i  50Hz  */
	dl_hdmitx_vfmt_22_720x576i_50hz   = 22,
	/**< Format 23 720  x 288p  50Hz  */
	dl_hdmitx_vfmt_23_720x288p_50hz   = 23,
	/**< Format 24 720  x 288p  50Hz  */
	dl_hdmitx_vfmt_24_720x288p_50hz   = 24,
	/**< Format 25 720  x 576i  50Hz  */
	dl_hdmitx_vfmt_25_720x576i_50hz   = 25,
	/**< Format 26 720  x 576i  50Hz  */
	dl_hdmitx_vfmt_26_720x576i_50hz   = 26,
	/**< Format 27 720  x 288p  50Hz  */
	dl_hdmitx_vfmt_27_720x288p_50hz   = 27,
	/**< Format 28 720  x 288p  50Hz  */
	dl_hdmitx_vfmt_28_720x288p_50hz   = 28,
	/**< Format 29 1440 x 576p  50Hz  */
	dl_hdmitx_vfmt_29_1440x576p_50hz  = 29,
	/**< Format 30 1440 x 576p  50Hz  */
	dl_hdmitx_vfmt_30_1440x576p_50hz  = 30,
	/**< Format 31 1920 x 1080p 50Hz  */
	dl_hdmitx_vfmt_31_1920x1080p_50hz = 31,
	/**< Format 32 1920 x 1080p 24Hz  */
	dl_hdmitx_vfmt_32_1920x1080p_24hz = 32,
	/**< Format 33 1920 x 1080p 25Hz  */
	dl_hdmitx_vfmt_33_1920x1080p_25hz = 33,
	/**< Format 34 1920 x 1080p 30Hz  */
	dl_hdmitx_vfmt_34_1920x1080p_30hz = 34,
	/**< Highest valid TV format      */
	DL_HDMITX_VFMT_TV_MAX             = 34,
	/**< Lowest TV format without prefetched table */
	DL_HDMITX_VFMT_TV_NO_REG_MIN      = 32,
	/**< Number of TV formats & null  */
	DL_HDMITX_VFMT_TV_NUM             = 35,
	/**< Lowest valid PC format       */
	DL_HDMITX_VFMT_PC_MIN             = 128,
	/**< PC format 128                */
	dl_hdmitx_vfmt_pc_640x480p_60hz   = 128,
	/**< PC format 129                */
	dl_hdmitx_vfmt_pc_800x600p_60hz   = 129,
	/**< PC format 130                */
	dl_hdmitx_vfmt_pc_1152x960p_60hz  = 130,
	/**< PC format 131                */
	dl_hdmitx_vfmt_pc_1024x768p_60hz  = 131,
	/**< PC format 132                */
	dl_hdmitx_vfmt_pc_1280x768p_60hz  = 132,
	/**< PC format 133                */
	dl_hdmitx_vfmt_pc_1280x1024p_60hz = 133,
	/**< PC format 134                */
	dl_hdmitx_vfmt_pc_1360x768p_60hz  = 134,
	/**< PC format 135                */
	dl_hdmitx_vfmt_pc_1400x1050p_60hz = 135,
	/**< PC format 136                */
	dl_hdmitx_vfmt_pc_1600x1200p_60hz = 136,
	/**< PC format 137                */
	dl_hdmitx_vfmt_pc_1024x768p_70hz  = 137,
	/**< PC format 138                */
	dl_hdmitx_vfmt_pc_640x480p_72hz   = 138,
	/**< PC format 139                */
	dl_hdmitx_vfmt_pc_800x600p_72hz   = 139,
	/**< PC format 140                */
	dl_hdmitx_vfmt_pc_640x480p_75hz   = 140,
	/**< PC format 141                */
	dl_hdmitx_vfmt_pc_1024x768p_75hz  = 141,
	/**< PC format 142                */
	dl_hdmitx_vfmt_pc_800x600p_75hz   = 142,
	/**< PC format 143                */
	dl_hdmitx_vfmt_pc_1024x864p_75hz  = 143,
	/**< PC format 144                */
	dl_hdmitx_vfmt_pc_1280x1024p_75hz = 144,
	/**< PC format 145                */
	dl_hdmitx_vfmt_pc_640x350p_85hz   = 145,
	/**< PC format 146                */
	dl_hdmitx_vfmt_pc_640x400p_85hz   = 146,
	/**< PC format 147                */
	dl_hdmitx_vfmt_pc_720x400p_85hz   = 147,
	/**< PC format 148                */
	dl_hdmitx_vfmt_pc_640x480p_85hz   = 148,
	/**< PC format 149                */
	dl_hdmitx_vfmt_pc_800x600p_85hz   = 149,
	/**< PC format 150                */
	dl_hdmitx_vfmt_pc_1024x768p_85hz  = 150,
	/**< PC format 151                */
	dl_hdmitx_vfmt_pc_1152x864p_85hz  = 151,
	/**< PC format 152                */
	dl_hdmitx_vfmt_pc_1280x960p_85hz  = 152,
	/**< PC format 153                */
	dl_hdmitx_vfmt_pc_1280x1024p_85hz = 153,
	/**< PC format 154                */
	dl_hdmitx_vfmt_pc_1024x768i_87hz  = 154,
	/**< Highest valid PC format      */
	DL_HDMITX_VFMT_PC_MAX             = 154,
	/**< Number of PC formats         */
	DL_HDMITX_VFMT_PC_NUM             = (1 + 154 - 128)
} tx_vid_fmt_t;

/**
 * \brief Structure defining the EDID short video descriptor
 * */
typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tx_vid_fmt_t  video_format;
	/**< true if format is the preferred video format */
	bool                native_video_format;
} tx_short_vid_desc_t;

/**
 * \brief Enum listing all picture aspect ratio (H:V) (4:3, 16:9)
 * */
typedef enum {
	/**< Undefined picture aspect ratio */
	DL_HDMITX_P_ASPECT_RATIO_UNDEFINED    = 0,
	/**< 6:5 picture aspect ratio (PAR) */
	DL_HDMITX_P_ASPECT_RATIO_6_5          = 1,
	DL_HDMITX_P_ASPECT_RATIO_5_4          = 2,    /**< 5:4 PAR */
	DL_HDMITX_P_ASPECT_RATIO_4_3          = 3,    /**< 4:3 PAR */
	DL_HDMITX_P_ASPECT_RATIO_16_10        = 4,    /**< 16:10 PAR */
	DL_HDMITX_P_ASPECT_RATIO_5_3          = 5,    /**< 5:3 PAR */
	DL_HDMITX_P_ASPECT_RATIO_16_9         = 6,    /**< 16:9 PAR */
	DL_HDMITX_P_ASPECT_RATIO_9_5          = 7,    /**< 9:5 PAR */
} tx_pict_aspect_ratio_t;

/**
 * \brief Enum listing all vertical frequency
 * */
typedef enum {
	dl_hdmitx_vfreq_24hz      = 0,    /**< 24Hz          */
	dl_hdmitx_vfreq_25hz      = 1,    /**< 25Hz          */
	dl_hdmitx_vfreq_30hz      = 2,    /**< 30Hz          */
	dl_hdmitx_vfreq_50hz      = 3,    /**< 50Hz          */
	dl_hdmitx_vfreq_59hz      = 4,    /**< 59.94Hz       */
	dl_hdmitx_vfreq_60hz      = 5,    /**< 60Hz          */
#ifndef FORMAT_PC
	DL_HDMITX_VFREQ_INVALID   = 6,    /**< Invalid       */
	DL_HDMITX_VFREQ_NUM       = 6     /**< No. of values */
#else /* FORMAT_PC */
	dl_hdmitx_vfreq_70hz      = 6,    /**< 70Hz          */
	dl_hdmitx_vfreq_72hz      = 7,    /**< 72Hz          */
	dl_hdmitx_vfreq_75hz      = 8,    /**< 75Hz          */
	dl_hdmitx_vfreq_85hz      = 9,    /**< 85Hz          */
	dl_hdmitx_vfreq_87hz      = 10,   /**< 87Hz          */
	DL_HDMITX_VFREQ_INVALID   = 11,   /**< Invalid       */
	DL_HDMITX_VFREQ_NUM       = 11    /**< No. of values */
#endif /* FORMAT_PC */
} tx_vfreq_t;

/**
 * \brief Structure storing specifications of a video resolution
 * */
typedef struct {
	/**< Width of the frame in pixels */
	u16                      width;
	/**< Height of the frame in pixels */
	u16                      height;
	/**< Interlaced mode (true/false) */
	bool                        interlaced;
	tx_vfreq_t           vfrequency;    /**< Vertical frequency in Hz */
	tx_pict_aspect_ratio_t aspect_ratio;   /**< Picture aspect ratio (H:V) */
} tx_vid_fmt_specs_t;

/**
 * \brief Enum listing all video input modes (CCIR, RGB, etc.)
 * */
typedef enum {
	DL_HDMITX_VINMODE_CCIR656     = 0,    /**< CCIR656 */
	DL_HDMITX_VINMODE_RGB444      = 1,    /**< RGB444  */
	DL_HDMITX_VINMODE_YUV444      = 2,    /**< YUV444  */
	DL_HDMITX_VINMODE_YUV422      = 3,    /**< YUV422  */
	DL_HDMITX_VINMODE_NO_CHANGE   = 4,    /**< No change */
	DL_HDMITX_VINMODE_INVALID     = 5     /**< Invalid */
} tx_vin_mode_t;

/**
 * \brief Enum listing all possible sync sources
 * */
typedef enum {
	DL_HDMITX_SYNCSRC_EMBEDDED = 0, /**< Embedded sync */
	DL_HDMITX_SYNCSRC_EXT_VREF = 1, /**< External sync Vref, Href, Fref */
	DL_HDMITX_SYNCSRC_EXT_VS   = 2, /**< External sync Vs, Hs */
} tx_sync_source_t;

/**
 * \brief Enum listing all output pixel rate (Single, Double, etc.)
 * */
typedef enum {
	DL_HDMITX_PIXRATE_DOUBLE          = 0,        /**< Double pixel rate */
	DL_HDMITX_PIXRATE_SINGLE          = 1,        /**< Single pixel rate */
	/**< Single pixel repeated */
	DL_HDMITX_PIXRATE_SINGLE_REPEATED = 2,
} tx_pix_rate_t;

/**
 * \brief Structure defining the video input configuration
 * */
typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tx_vid_fmt_t       format;
	tx_vin_mode_t      mode;       /**< Video mode (CCIR, RGB, YUV, etc.) */
	tx_sync_source_t   sync_source; /**< Sync source type */
	tx_pix_rate_t      pixel_rate;  /**< Pixel rate */
} tx_video_in_config_t;

/**
 * \brief Enum listing all video output modes (YUV, RGB, etc.)
 * */
typedef enum {
	DL_HDMITX_VOUTMODE_RGB444     = 0,    /**< RGB444    */
	DL_HDMITX_VOUTMODE_YUV422     = 1,    /**< YUV422    */
	DL_HDMITX_VOUTMODE_YUV444     = 2,    /**< YUV444    */
} tx_vout_mode_t;

/**
 * \brief Enum defining possible quantization range
 * */
typedef enum {
	DL_HDMITX_VQR_DEFAULT = 0, /* Follow HDMI spec. */
	DL_HDMITX_RGB_FULL    = 1, /* Force RGB FULL , DVI only */
	DL_HDMITX_RGB_LIMITED = 2  /* Force RGB LIMITED , DVI only */
} tx_vqr_t;

/**
 * \brief Structure defining the video output configuration
 * */
typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tx_vid_fmt_t       format;
	tx_vout_mode_t     mode;       /**< Video mode (CCIR, RGB, YUV, etc.) */
	tx_color_depth_t   color_depth; /**< Color depth */
	tx_vqr_t          dvi_vqr;     /**< VQR applied in DVI mode */
} tx_video_out_config_t;

/**
 * \brief Structure defining the audio input configuration
 * */
typedef struct {
	/**< Audio format (I2S, SPDIF, etc.) */
	tx_audio_format_t          format;
	tx_audio_rate_t            rate;           /**< Audio sampling rate */
	/**< I2S format of the audio input */
	tx_audio_i2sformat_t       i2s_format;
	/**< I2S qualifier of the audio input (8,16,32 bits) */
	tx_audio_i2squalifier_t    i2s_qualifier;
	tx_dst_rate_t              dst_rate;        /**< DST data transfer rate */
	/**< Ref to CEA-861D p85 */
	u8                            channel_allocation;
} tx_audio_in_config_t;

/**
 * \brief Enum listing all the type of sunk
 * */
typedef enum {
	DL_HDMITX_SINK_DVI  = 0, /**< DVI  */
	DL_HDMITX_SINK_HDMI = 1, /**< HDMI */
	DL_HDMITX_SINK_EDID = 2, /**< As currently defined in EDID */
} tx_sink_type_t;

/**
 * \brief Structure defining the content of a gamut packet
 * */
typedef struct {
	/**< Gamut relevant for field following packet insertion */
	bool   next_field;
	/**< Profile of the gamut packet : 0 = P0, 1 = P1 */
	u8  gbd_profile;
	/**< Gamut sequence number of the field that have to be affected by this gamut packet */
	u8  affected_gamut_seq_num;
	/**< Current field not using specific gamut */
	bool   no_current_gbd;
	/**< Gamut sequence number of the current field */
	u8  current_gamut_seq_num;
	/**< Sequence of the packet inside a multiple packet gamut */
	u8  packet_sequence;
	u8  payload[28];         /**< Payload of the gamut packet */
} tx_gamut_data_t;

/**
 * \brief Type defining the content of a generic packet
 * */
typedef u8 tx_generic_packet[28];

/**
 * \brief Structure defining the content of an ACP packet
 * */
typedef struct {
	u8 acp_type;
	u8 acp_data[28];
} tx_acp_pkt_data_t;

/**
 * \brief Structure defining the content of an AVI infoframe
 * */
typedef struct {
	/**< RGB or YCbCr indicator. See CEA-861-B table 8 for details */
	u8  color_indicator;
	/**< Active information present. Indicates if activeFormatAspectRatio field is valid */
	u8  active_info_present;
	u8  bar_information_data_valid;       /**< Bar information data valid */
	/**< Scan information. See CEA-861-B table 8 for details */
	u8  scan_information;
	/**< Colorimetry. See CEA-861-B table 9 for details */
	u8  colorimetry;
	/**< Picture aspect ratio. See CEA-861-B table 9 for details */
	u8  picture_aspect_ratio;
	/**< Active Format aspect ratio. See CEA-861-B table 10 and Annex H for details */
	u8  active_format_aspect_ratio;
	/**< Non-uniform picture scaling. See CEA-861-B table 11 for details */
	u8  non_uniform_picture_scaling;
	/**< Video format indentification code. See CEA-861-B section 6.3 for details */
	u8  video_format_identification_code;
	/**< Pixel repetition factor. See CEA-861-B table 11 for details */
	u8  pixel_repetition_factor;
	u16 line_number_end_top_bar;
	u16 line_number_start_bottom_bar;
	u16 line_number_end_left_bar;
	u16 line_number_start_right_bar;
} tx_avi_if_data_t;

/**
 * \brief Structure defining the content of an ACP packet
 * */
typedef struct {
	bool av_mute;
} tx_gcp_pkt_data_t;

/**
 * \brief Structure defining the content of an AUD infoframe
 * */
typedef struct {
	u8 coding_type;        /**< Coding type (always set to zero) */
	/**< Channel count. See CEA-861-B table 17 for details */
	u8 channel_count;
	/**< Sample frequency. See CEA-861-B table 18 for details */
	u8 samplefrequency;
	/**< Sample frequency. See CEA-861-B table 18 for details */
	u8 sample_size;
	/**< Channel allocation. See CEA-861-B section 6.3.2 for details */
	u8 channel_allocation;
	/**< Downmix inhibit. See CEA-861-B section 6.3.2 for details */
	bool  downmix_inhibit;
	/**< level shift value for downmixing. See CEA-861-B section 6.3.2 and table 23 for details */
	u8 level_shift_value;
} tx_aud_if_data_t;

/**
 * \brief Structure defining the content of an ISRC1 packet
 * */
typedef struct {
	bool  isrc_cont;         /**< ISRC packet continued in next packet */
	/**< Set to one when ISRCStatus and UPC_EAN_ISRC_xx are valid */
	bool  isrc_valid;
	u8 isrc_status;       /**< ISRC status */
	u8 UPC_EAN_ISRC[16]; /**< ISRC packet data */
} tx_isrc1pkt_data_t;

/**
 * \brief Structure defining the content of an ISRC2 packet
 * */
typedef struct {
	u8 UPC_EAN_ISRC[16];  /**< ISRC packet data */
} tx_isrc2pkt_data_t;

/**
 * \brief Structure defining the content of an MPS infoframe
 * */
typedef struct {
	u32 bit_rate;         /**< MPEG bit rate in Hz */
	u32 frame_type;       /**< MPEG frame type */
	bool   field_repeat;     /**< 0: new field, 1:repeated field */
} tx_mps_if_data_t;

/**
 * \brief Structure defining the content of an SPD infoframe
 * */
typedef struct {
	u8   vendor_name[8];   /**< Vendor name */
	u8   product_desc[16]; /**< Product Description */
	u32  source_dev_info;   /**< Source Device Info */
} tx_spd_if_data_t;

/**
 * \brief Structure defining the content of a VS packet
 * */
typedef struct {
	u8 version;
	u8 vs_data[27];
} tx_vs_pkt_data_t;

/**
 * \brief Structure defining the Edid audio descriptor
 * */
typedef struct {
	u8 format;         /* EIA/CEA861 mode */
	u8 channels;       /* number of channels */
	u8 supported_freqs; /* bitmask of supported frequencies */
	u8 supported_res;   /* bitmask of supported resolutions (LPCM only) */
	/* Maximum bitrate divided by 8KHz (compressed formats only) */
	u8 max_bitrate;
} tx_edid_audio_desc_t;

/**
 * \brief Structure defining detailed timings of a video format
 * */
typedef struct {
	u16  pixel_clock;        /**< Pixel Clock/10 000         */
	u16  h_active_pixels;     /**< Horizontal Active Pixels   */
	u16  h_blank_pixels;      /**< Horizontal Blanking Pixels */
	u16  v_active_lines;      /**< Vertical Active Lines      */
	u16  v_blank_lines;       /**< Vertical Blanking Lines    */
	u16  h_sync_offset;       /**< Horizontal Sync Offset     */
	u16  h_sync_width;        /**< Horiz. Sync Pulse Width    */
	u16  v_sync_offset;       /**< Vertical Sync Offset       */
	u16  v_sync_width;        /**< Vertical Sync Pulse Width  */
	u16  h_image_size;        /**< Horizontal Image Size      */
	u16  v_image_size;        /**< Vertical Image Size        */
	u16  h_border_pixels;     /**< Horizontal Border          */
	u16  v_border_pixels;     /**< Vertical Border            */
	u8   flags;             /**< Interlace/sync info        */
} tx_edid_video_timings_t;

/** size descriptor block of monitor descriptor */
#define EDID_MONITOR_DESCRIPTOR_SIZE   13

/**
 * \brief Structure defining the first monitor descriptor
 * */
typedef struct {
	/**< true when parameters of struct are available   */
	bool    desc_record;
	/**< Monitor Name                                   */
	u8   monitor_name[EDID_MONITOR_DESCRIPTOR_SIZE];
} tx_edid_first_md_t;

/**
 * \brief Structure defining the second monitor descriptor
 * */
typedef struct {
	/**< true when parameters of struct are available   */
	bool    desc_record;
	/**< Min vertical rate in Hz                        */
	u8   min_vertical_rate;
	/**< Max vertical rate in Hz                        */
	u8   max_vertical_rate;
	/**< Min horizontal rate in Hz                      */
	u8   min_horizontal_rate;
	/**< Max horizontal rate in Hz                      */
	u8   max_horizontal_rate;
	/**< Max suuported pixel clock rate in MHz          */
	u8   max_supported_pixel_clk;
} tx_edid_second_md_t;

/**
 * \brief Structure defining the other monitor descriptor
 * */
typedef struct {
	/**< true when parameters of struct are available   */
	bool    desc_record;
	/**< Other monitor Descriptor                       */
	u8   other_descriptor[EDID_MONITOR_DESCRIPTOR_SIZE];
} tx_edid_other_md_t;

/**
 * \brief Test pattern types
 * */
typedef enum {
	DL_HDMITX_PATTERN_OFF     = 0, /**< Insert test pattern       */
	DL_HDMITX_PATTERN_CBAR4   = 1, /**< Insert 4-bar colour bar   */
	DL_HDMITX_PATTERN_CBAR8   = 2, /**< Insert 8-bar colour bar   */
	DL_HDMITX_PATTERN_BLUE	= 3, /**< Insert Blue screen        */
	DL_HDMITX_PATTERN_BLACK   = 4, /**< Insert Black screen       */
	DL_HDMITX_PATTERN_INVALID = 5  /**< Invalid pattern		    */
} tx_test_pattern_t;

/**
 * \brief Enum listing all hdcp state
 * */
typedef enum {
	DL_HDMITX_HDCP_CHECK_NOT_STARTED       = 0,    /**< Check not started */
	/**< No failures, more to do */
	DL_HDMITX_HDCP_CHECK_IN_PROGRESS       = 1,
	/**< Final check has passed */
	DL_HDMITX_HDCP_CHECK_PASS              = 2,
	/**< First check failure code */
	DL_HDMITX_HDCP_CHECK_FAIL_FIRST        = 3,
	/**< Driver not AUTHENTICATED */
	DL_HDMITX_HDCP_CHECK_FAIL_DRIVER_STATE = 3,
	/**< A T0 interrupt occurred */
	DL_HDMITX_HDCP_CHECK_FAIL_DEVICE_T0    = 4,
	DL_HDMITX_HDCP_CHECK_FAIL_DEVICE_RI    = 5,    /**< Device RI changed */
	DL_HDMITX_HDCP_CHECK_FAIL_DEVICE_FSM   = 6,    /**< Device FSM not 10h */
	/**< Number of check results */
	DL_HDMITX_HDCP_CHECK_NUM               = 7
} tx_hdcp_check_t;

/**
 * \brief Enum listing all hdcp option flags
 * */
typedef enum {
	/* Not set: obey PJ result     */
	DL_HDMITX_HDCP_OPTION_FORCE_PJ_IGNORED    = 0x01,
	/* Not set: obey BCAPS setting */
	DL_HDMITX_HDCP_OPTION_FORCE_SLOW_DDC      = 0x02,
	/* Not set: obey BCAPS setting */
	DL_HDMITX_HDCP_OPTION_FORCE_NO_1_1        = 0x04,
	/* Not set: obey BCAPS setting */
	DL_HDMITX_HDCP_OPTION_FORCE_REPEATER      = 0x08,
	/* Not set: obey BCAPS setting */
	DL_HDMITX_HDCP_OPTION_FORCE_NO_REPEATER   = 0x10,
	/* Not set: obey V=V' result   */
	DL_HDMITX_HDCP_OPTION_FORCE_V_EQU_VBAR    = 0x20,
	DL_HDMITX_HDCP_OPTION_FORCE_VSLOW_DDC     = 0x40, /* Set: 50kHz DDC */
	/* All the above Not Set vals */
	DL_HDMITX_HDCP_OPTION_DEFAULT             = 0x00,
	/* Only these bits are allowed */
	DL_HDMITX_HDCP_OPTION_MASK                = 0x7F,
	/* These bits are not allowed  */
	DL_HDMITX_HDCP_OPTION_MASK_BAD            = 0x80
} tx_hdcp_options_t;

typedef enum {
	DL_HDMITX_HOTPLUG_INACTIVE    = 0,    /**< Hotplug inactive */
	DL_HDMITX_HOTPLUG_ACTIVE      = 1,    /**< Hotplug active   */
	DL_HDMITX_HOTPLUG_INVALID     = 2     /**< Invalid Hotplug  */
} tx_hot_plug_t;

#endif /* DLHDMITX_TYPES_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

