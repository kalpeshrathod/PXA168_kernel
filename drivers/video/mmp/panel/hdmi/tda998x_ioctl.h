/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * Version       Revision: 1.0
 *
 * Date          Date: 27/10/09
 *
 * Brief         API for the TDA1998x HDMI Transmitters
 *
 **/

#include <linux/types.h>

#ifndef __tx_ioctl__
#define __tx_ioctl__

#ifdef __tx_h__

#define TRANS_TYPE 1

#if TRANS_TYPE

#define EXAMPLE_MAX_SVD 30

/*
 *  trans-type
 * */
typedef swversion_t tda_version;
typedef power_state_t tda_power;
typedef tx_instance_setup_info_t tda_setup;
typedef tx_capabilities_t tda_capabilities;
typedef tx_video_out_config_t tda_video_out;
typedef tx_video_in_config_t tda_video_in;
typedef tx_sink_type_t tda_sink;
typedef tx_audio_in_config_t tda_audio_in;
typedef tx_edid_audio_desc_t tda_edid_audio_desc;
typedef tx_short_vid_desc_t tda_edid_video_desc;
typedef tx_event_t tda_event;
typedef tx_instance_setup_info_t tda_setup_info;
typedef tx_edid_video_timings_t tda_edid_video_timings;
typedef tx_pict_aspect_ratio_t tda_edid_tv_aspect_ratio;
typedef tx_hdcp_check_t tda_hdcp_status;
#if defined (TMFL_TDA19989) || defined (TMFL_TDA9984)
typedef tx_hdcp_status_t tda_hdcp_fail;
#endif
#ifdef TMFL_TDA19989
typedef tx_edid_latency_t tda_edid_latency;
#endif

typedef struct {
	bool enable;
	tx_gamut_data_t data;
} tda_gammut;

typedef struct {
	bool enable;
	tx_vs_pkt_data_t data;
} tda_vs_infoframe;

typedef struct {
	bool enable;
	tx_spd_if_data_t data;
} tda_spd_infoframe;

typedef struct {
	bool enable;
	tx_mps_if_data_t data;
} tda_mps_infoframe;

typedef struct {
	bool enable;
	tx_isrc1pkt_data_t data;
} tda_isrc1;

typedef struct {
	bool enable;
	tx_isrc2pkt_data_t data;
} tda_isrc2;

typedef struct {
	bool enable;
	tx_acp_pkt_data_t data;
} tda_acp;

typedef struct {
	bool enable;
	tx_gcp_pkt_data_t data;
} tda_gcp;

typedef struct {
	bool enable;
	tx_avi_if_data_t data;
} tda_video_infoframe;

typedef struct {
	bool enable;
	tx_aud_if_data_t data;
} tda_audio_infoframe;

typedef struct {
	tx_vid_fmt_t id;
	tx_vid_fmt_specs_t spec;
} tda_video_format;

typedef struct {
	tda_video_in video_in;
	tda_video_out video_out;
	tda_audio_in audio_in; /* Mind tda_set_audio_in if you change this */
	tda_sink sink; /* Mind tda_set_audio_in if you change this */
} tda_set_in_out;

typedef struct {
	tda_audio_in audio_in;
	tda_sink sink;
} tda_set_audio_in;

typedef struct {
	tda_edid_audio_desc desc[EXAMPLE_MAX_SVD];
	unsigned int max;
	unsigned int written;
	unsigned char flags;
} tda_edid_audio_caps;

typedef struct {
	tda_edid_video_desc desc[EXAMPLE_MAX_SVD];
	unsigned int max;
	unsigned int written;
	unsigned char flags;
} tda_edid_video_caps;

typedef struct {
	tx_edid_status_t status;
	unsigned char block_count;
} tda_edid;

typedef struct {
	tx_edid_video_timings_t desc[EXAMPLE_MAX_SVD];
	unsigned char max;
	unsigned char written;
} tda_edid_dtd;

typedef struct {
	tx_edid_first_md_t desc1[EXAMPLE_MAX_SVD];
	tx_edid_second_md_t desc2[EXAMPLE_MAX_SVD];
	tx_edid_other_md_t other[EXAMPLE_MAX_SVD];
	unsigned char max;
	unsigned char written;
} tda_edid_md;

#else

#error do not compiled this !

typedef enum {
	/**< HDCP encryption status switched to active */
	TDA_HDCP_ACTIVE         = 0,
	/**< HDCP encryption status switched to inactive */
	TDA_HDCP_INACTIVE       = 1,
	TDA_HPD_ACTIVE          = 2,    /**< Hotplug status switched to active */
	TDA_HPD_INACTIVE        = 3,    /**< Hotplug status switched to inactive */
	TDA_RX_KEYS_RECEIVED    = 4,    /**< Receiver(s) key(s) received */
	TDA_RX_DEVICE_ACTIVE    = 5,    /**< Rx device is connected and active */
	/**< Rx device is connected but inactive (standby) */
	TDA_RX_DEVICE_INACTIVE  = 6,
	TDA_EDID_RECEIVED       = 7,    /**< EDID has been received */
	TDA_VS_RPT_RECEIVED     = 8,    /**< VS interrupt has been received */
#ifdef HDMI_TX_REPEATER_ISR_MODE
	TDA_B_STATUS            = 9,    /**< TX received BStatus */
#endif /* HDMI_TX_REPEATER_ISR_MODE */
	TDA_DEBUG_EVENT_1       = 10     /**< This is a debug event */
} tda_event;

typedef struct {
	unsigned char format;         /* EIA/CEA861 mode */
	unsigned char channels;       /* number of channels */
	unsigned char supported_freqs; /* bitmask of supported frequencies */
	/* bitmask of supported resolutions (LPCM only) */
	unsigned char supported_res;
	/* Maximum bitrate divided by 8KHz (compressed formats only) */
	unsigned char max_bitrate;
} tda_edid_audio_desc;

typedef enum {
	TDA_EDID_READ                = 0,   /**< All blocks read OK */
	/**< All blocks read OK but buffer too small to return all of them */
	TDA_EDID_READ_INCOMPLETE     = 1,
	TDA_EDID_ERROR_CHK_BLOCK_0   = 2,   /**< Block 0 checksum error */
	/**< Block 0 OK, checksum error in one or more other blocks */
	TDA_EDID_ERROR_CHK           = 3,
	TDA_EDID_NOT_READ            = 4,   /**< EDID not read */
	TDA_EDID_STATUS_INVALID      = 5    /**< Invalid   */
} tda_edid_status;

typedef struct {
	int HBR;              /**< High Bitrate Audio packet */
	int DST;              /**< Direct Stream Transport audio packet */
	int one_bit_audio;      /**< One Bit Audio sample packet */
} tda_audio_packet;

typedef enum {
	TDA_AFMT_SPDIF      = 0, /**< SPDIF */
	TDA_AFMT_I2S        = 1, /**< I2S */
	TDA_AFMT_OBA        = 2, /**< One bit audio / DSD */
	TDA_AFMT_DST        = 3, /**< DST */
	TDA_AFMT_HBR        = 4  /**< HBR */
} tda_audio_format;

typedef enum {
	TDA_AFS_32K           = 0, /**< 32kHz    */
	TDA_AFS_44K           = 1, /**< 44.1kHz  */
	TDA_AFS_48K           = 2, /**< 48kHz    */
	TDA_AFS_88K           = 3, /**< 88.2kHz  */
	TDA_AFS_96K           = 4, /**< 96kHz    */
	TDA_AFS_176K          = 5, /**< 176.4kHz */
	TDA_AFS_192K          = 6  /**< 192kHz   */
} tda_audio_rate;

typedef enum {
	TDA_I2SQ_16BITS       = 16, /**< 16 bits */
	TDA_I2SQ_32BITS       = 32, /**< 32 bits */
	TDA_I2SQ_OTHERS       = 0   /**< for SPDIF and DSD */
} tda_audio_i2s_qualifier;

typedef enum {
	TDA_I2SFOR_PHILIPS_L   = 0,	/**< Philips like format */
	TDA_I2SFOR_OTH_L       = 2,	/**< Other non Philips left justified */
	TDA_I2SFOR_OTH_R	   = 3,	/**< Other non Philips right justified */
	TDA_I2SFOR_INVALID     = 4	/**< Invalid format */
} tda_audio_i2s_format;

typedef enum {
	TDA_DSTRATE_SINGLE  = 0,    /**< Single transfer rate */
	TDA_DSTRATE_DOUBLE  = 1     /**< Double data rate */
} tda_dst_rate;

typedef struct {
	int    simplay_hd;          /**< Enable simplayHD support */
	int    repeater_enable;     /**< Enable repeater mode */
	unsigned char   *p_edid_buffer;       /**< Pointer to raw EDID data */
	/**< Size of buffer for raw EDID data */
	unsigned long  edid_buffer_size;
} tda_instance_setup_info;

typedef enum {
	TDA_VFMT_NULL               = 0,    /**< Not a valid format...        */
	TDA_VFMT_NO_CHANGE          = 0,    /**< ...or no change required     */
	TDA_VFMT_MIN                = 1,    /**< Lowest valid format          */
	TDA_VFMT_TV_MIN             = 1,    /**< Lowest valid TV format       */
	tda_vfmt_01_640x480p_60hz   = 1,    /**< Format 01 640  x 480p  60Hz  */
	tda_vfmt_02_720x480p_60hz   = 2,    /**< Format 02 720  x 480p  60Hz  */
	tda_vfmt_03_720x480p_60hz   = 3,    /**< Format 03 720  x 480p  60Hz  */
	tda_vfmt_04_1280x720p_60hz  = 4,    /**< Format 04 1280 x 720p  60Hz  */
	tda_vfmt_05_1920x1080i_60hz = 5,    /**< Format 05 1920 x 1080i 60Hz  */
	tda_vfmt_06_720x480i_60hz   = 6,    /**< Format 06 720  x 480i  60Hz  */
	tda_vfmt_07_720x480i_60hz   = 7,    /**< Format 07 720  x 480i  60Hz  */
	tda_vfmt_08_720x240p_60hz   = 8,    /**< Format 08 720  x 240p  60Hz  */
	tda_vfmt_09_720x240p_60hz   = 9,    /**< Format 09 720  x 240p  60Hz  */
	tda_vfmt_10_720x480i_60hz   = 10,   /**< Format 10 720  x 480i  60Hz  */
	tda_vfmt_11_720x480i_60hz   = 11,   /**< Format 11 720  x 480i  60Hz  */
	tda_vfmt_12_720x240p_60hz   = 12,   /**< Format 12 720  x 240p  60Hz  */
	tda_vfmt_13_720x240p_60hz   = 13,   /**< Format 13 720  x 240p  60Hz  */
	tda_vfmt_14_1440x480p_60hz  = 14,   /**< Format 14 1440 x 480p  60Hz  */
	tda_vfmt_15_1440x480p_60hz  = 15,   /**< Format 15 1440 x 480p  60Hz  */
	tda_vfmt_16_1920x1080p_60hz = 16,   /**< Format 16 1920 x 1080p 60Hz  */
	tda_vfmt_17_720x576p_50hz   = 17,   /**< Format 17 720  x 576p  50Hz  */
	tda_vfmt_18_720x576p_50hz   = 18,   /**< Format 18 720  x 576p  50Hz  */
	tda_vfmt_19_1280x720p_50hz  = 19,   /**< Format 19 1280 x 720p  50Hz  */
	tda_vfmt_20_1920x1080i_50hz = 20,   /**< Format 20 1920 x 1080i 50Hz  */
	tda_vfmt_21_720x576i_50hz   = 21,   /**< Format 21 720  x 576i  50Hz  */
	tda_vfmt_22_720x576i_50hz   = 22,   /**< Format 22 720  x 576i  50Hz  */
	tda_vfmt_23_720x288p_50hz   = 23,   /**< Format 23 720  x 288p  50Hz  */
	tda_vfmt_24_720x288p_50hz   = 24,   /**< Format 24 720  x 288p  50Hz  */
	tda_vfmt_25_720x576i_50hz   = 25,   /**< Format 25 720  x 576i  50Hz  */
	tda_vfmt_26_720x576i_50hz   = 26,   /**< Format 26 720  x 576i  50Hz  */
	tda_vfmt_27_720x288p_50hz   = 27,   /**< Format 27 720  x 288p  50Hz  */
	tda_vfmt_28_720x288p_50hz   = 28,   /**< Format 28 720  x 288p  50Hz  */
	tda_vfmt_29_1440x576p_50hz  = 29,   /**< Format 29 1440 x 576p  50Hz  */
	tda_vfmt_30_1440x576p_50hz  = 30,   /**< Format 30 1440 x 576p  50Hz  */
	tda_vfmt_31_1920x1080p_50hz = 31,   /**< Format 31 1920 x 1080p 50Hz  */
	tda_vfmt_32_1920x1080p_24hz = 32,   /**< Format 32 1920 x 1080p 24Hz  */
	tda_vfmt_33_1920x1080p_25hz = 33,   /**< Format 33 1920 x 1080p 25Hz  */
	tda_vfmt_34_1920x1080p_30hz = 34,   /**< Format 34 1920 x 1080p 30Hz  */
	TDA_VFMT_TV_MAX             = 34,   /**< Highest valid TV format      */
	/**< Lowest TV format without prefetched table */
	TDA_VFMT_TV_NO_REG_MIN      = 32,
	TDA_VFMT_TV_NUM             = 35,   /**< Number of TV formats & null  */
	TDA_VFMT_PC_MIN             = 128,  /**< Lowest valid PC format       */
	tda_vfmt_pc_640x480p_60hz   = 128,  /**< PC format 128                */
	tda_vfmt_pc_800x600p_60hz   = 129,  /**< PC format 129                */
	tda_vfmt_pc_1152x960p_60hz  = 130,  /**< PC format 130                */
	tda_vfmt_pc_1024x768p_60hz  = 131,  /**< PC format 131                */
	tda_vfmt_pc_1280x768p_60hz  = 132,  /**< PC format 132                */
	tda_vfmt_pc_1280x1024p_60hz = 133,  /**< PC format 133                */
	tda_vfmt_pc_1360x768p_60hz  = 134,  /**< PC format 134                */
	tda_vfmt_pc_1400x1050p_60hz = 135,  /**< PC format 135                */
	tda_vfmt_pc_1600x1200p_60hz = 136,  /**< PC format 136                */
	tda_vfmt_pc_1024x768p_70hz  = 137,  /**< PC format 137                */
	tda_vfmt_pc_640x480p_72hz   = 138,  /**< PC format 138                */
	tda_vfmt_pc_800x600p_72hz   = 139,  /**< PC format 139                */
	tda_vfmt_pc_640x480p_75hz   = 140,  /**< PC format 140                */
	tda_vfmt_pc_1024x768p_75hz  = 141,  /**< PC format 141                */
	tda_vfmt_pc_800x600p_75hz   = 142,  /**< PC format 142                */
	tda_vfmt_pc_1024x864p_75hz  = 143,  /**< PC format 143                */
	tda_vfmt_pc_1280x1024p_75hz = 144,  /**< PC format 144                */
	tda_vfmt_pc_640x350p_85hz   = 145,  /**< PC format 145                */
	tda_vfmt_pc_640x400p_85hz   = 146,  /**< PC format 146                */
	tda_vfmt_pc_720x400p_85hz   = 147,  /**< PC format 147                */
	tda_vfmt_pc_640x480p_85hz   = 148,  /**< PC format 148                */
	tda_vfmt_pc_800x600p_85hz   = 149,  /**< PC format 149                */
	tda_vfmt_pc_1024x768p_85hz  = 150,  /**< PC format 150                */
	tda_vfmt_pc_1152x864p_85hz  = 151,  /**< PC format 151                */
	tda_vfmt_pc_1280x960p_85hz  = 152,  /**< PC format 152                */
	tda_vfmt_pc_1280x1024p_85hz = 153,  /**< PC format 153                */
	tda_vfmt_pc_1024x768i_87hz  = 154,  /**< PC format 154                */
	TDA_VFMT_PC_MAX             = 154,  /**< Highest valid PC format      */
	/**< Number of PC formats         */
	TDA_VFMT_PC_NUM             = (1 + 154 - 128)
} tda_video_fmt_id;

typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tda_video_fmt_id video_format;
	/**< true if format is the preferred video format */
	int              native_video_format;
} tda_edid_video_desc;

typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tda_video_fmt_id video_format;
	/**< true if format is the preferred video format */
	int              native_video_format;
} tda_short_video_desc;

typedef enum {
	/**< Undefined picture aspect ratio */
	TDA_P_ASPECT_RATIO_UNDEFINED    = 0,
	/**< 6:5 picture aspect ratio (PAR) */
	TDA_P_ASPECT_RATIO_6_5          = 1,
	TDA_P_ASPECT_RATIO_5_4          = 2,    /**< 5:4 PAR */
	TDA_P_ASPECT_RATIO_4_3          = 3,    /**< 4:3 PAR */
	TDA_P_ASPECT_RATIO_16_10        = 4,    /**< 16:10 PAR */
	TDA_P_ASPECT_RATIO_5_3          = 5,    /**< 5:3 PAR */
	TDA_P_ASPECT_RATIO_16_9         = 6,    /**< 16:9 PAR */
	TDA_P_ASPECT_RATIO_9_5          = 7     /**< 9:5 PAR */
} tda_pict_aspect_ratio;

typedef enum {
	tda_vfreq_24hz      = 0,    /**< 24Hz          */
	tda_vfreq_25hz      = 1,    /**< 25Hz          */
	tda_vfreq_30hz      = 2,    /**< 30Hz          */
	tda_vfreq_50hz      = 3,    /**< 50Hz          */
	tda_vfreq_59hz      = 4,    /**< 59.94Hz       */
	tda_vfreq_60hz      = 5,    /**< 60Hz          */
	tda_vfreq_70hz      = 6,    /**< 70Hz          */
	tda_vfreq_72hz      = 7,    /**< 72Hz          */
	tda_vfreq_75hz      = 8,    /**< 75Hz          */
	tda_vfreq_85hz      = 9,    /**< 85Hz          */
	tda_vfreq_87hz      = 10,   /**< 87Hz          */
	TDA_VFREQ_INVALID   = 11,   /**< Invalid       */
	TDA_VFREQ_NUM       = 11    /**< No. of values */
} tda_vfreq;

typedef struct {
	/**< Width of the frame in pixels */
	unsigned short                      width;
	/**< Height of the frame in pixels */
	unsigned short                      height;
	/**< Interlaced mode (true/false) */
	int                        interlaced;
	tda_vfreq           vfrequency;    /**< Vertical frequency in Hz */
	tda_pict_aspect_ratio aspect_ratio;   /**< Picture aspect ratio (H:V) */
} tda_video_fmt_specs;

typedef enum {
	TDA_VINMODE_CCIR656     = 0,    /**< CCIR656 */
	TDA_VINMODE_RGB444      = 1,    /**< RGB444  */
	TDA_VINMODE_YUV444      = 2,    /**< YUV444  */
	TDA_VINMODE_YUV422      = 3,    /**< YUV422  */
	TDA_VINMODE_NO_CHANGE   = 4,    /**< No change */
	TDA_VINMODE_INVALID     = 5     /**< Invalid */
} tda_vinmode;

typedef enum {
	TDA_SYNCSRC_EMBEDDED = 0, /**< Embedded sync */
	TDA_SYNCSRC_EXT_VREF = 1, /**< External sync Vref, Href, Fref */
	TDA_SYNCSRC_EXT_VS   = 2  /**< External sync Vs, Hs */
} tda_sync_source;

typedef enum {
	TDA_PIXRATE_DOUBLE          = 0,        /**< Double pixel rate */
	TDA_PIXRATE_SINGLE          = 1,        /**< Single pixel rate */
	TDA_PIXRATE_SINGLE_REPEATED = 2         /**< Single pixel repeated */
} tda_pix_rate;

typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tda_video_fmt_id       format;
	tda_vinmode      mode;       /**< Video mode (CCIR, RGB, YUV, etc.) */
	tda_sync_source   sync_source; /**< Sync source type */
	tda_pix_rate      pixel_rate;  /**< Pixel rate */
} tda_video_in;

typedef enum {
	TDA_VOUTMODE_RGB444     = 0,    /**< RGB444    */
	TDA_VOUTMODE_YUV422     = 1,    /**< YUV422    */
	TDA_VOUTMODE_YUV444     = 2     /**< YUV444    */
} tda_vout_mode;

typedef enum {
	TDA_VQR_DEFAULT = 0, /* Follow HDMI spec. */
	TDA_RGB_FULL    = 1, /* Force RGB FULL , DVI only */
	TDA_RGB_LIMITED = 2  /* Force RGB LIMITED , DVI only */
} tda_vqr;

typedef enum {
	TDA_COLORDEPTH_24   = 0,    /**< 8 bits per color */
	TDA_COLORDEPTH_30   = 1,    /**< 10 bits per color */
	TDA_COLORDEPTH_36   = 2,    /**< 12 bits per color */
	TDA_COLORDEPTH_48   = 3     /**< 16 bits per color */
} tda_color_depth;

typedef struct {
	/**< Video format as defined by EIA/CEA 861-D */
	tda_video_fmt_id       format;
	tda_vout_mode     mode;       /**< Video mode (CCIR, RGB, YUV, etc.) */
	tda_color_depth  color_depth; /**< Color depth */
	tda_vqr          dvi_vqr;     /**< VQR applied in DVI mode */
} tda_video_out;

typedef struct {
	/**< Audio format (I2S, SPDIF, etc.) */
	tda_audio_format          format;
	tda_audio_rate            rate;               /**< Audio sampling rate */
	/**< I2S format of the audio input */
	tda_audio_i2s_format       i2s_format;
	/**< I2S qualifier of the audio input (8,16,32 bits) */
	tda_audio_i2s_qualifier    i2s_qualifier;
	/**< DST data transfer rate */
	tda_dst_rate              dst_rate;
	/**< Ref to CEA-861D p85 */
	unsigned char                            channel_allocation;
} tda_audio_in;

typedef enum {
	TDA_SINK_DVI  = 0, /**< DVI  */
	TDA_SINK_HDMI = 1, /**< HDMI */
	TDA_SINK_EDID = 2  /**< As currently defined in EDID */
} tda_sink;

typedef enum {
	TDA_DEVICE_UNKNOWN,   /**< HW device is unknown */
	TDA_DEVICE_TDA9984,   /**< HW device is IC TDA9984 */
	TDA_DEVICE_TDA9989,   /**< HW device is IC TDA9989 */
	TDA_DEVICE_TDA9981,   /**< HW device is IC TDA9981 */
	TDA_DEVICE_TDA9983,   /**< HW device is IC TDA9983 */
	TDA_DEVICE_TDA19989   /**< HW device is IC TDA19989 */
} tda_device_version;

typedef enum {
	TDA_HDMI_VERSION_UNKNOWN, /**< Unknown   */
	TDA_HDMI_VERSION_1_1,     /**< HDMI 1.1  */
	tda_hdmi_version_1_2a,    /**< HDMI 1.2a */
	tda_hdmi_version_1_3a     /**< HDMI 1.3  */
} tda_hdmi_version;

typedef struct {
	int HBR;              /**< High Bitrate Audio packet */
	int DST;              /**< Direct Stream Transport audio packet */
	int one_bit_audio;      /**< One Bit Audio sample packet */
} tda_audio_packet;

typedef enum {
	TDA_COLORDEPTH_24   = 0,    /**< 8 bits per color */
	TDA_COLORDEPTH_30   = 1,    /**< 10 bits per color */
	TDA_COLORDEPTH_36   = 2,    /**< 12 bits per color */
	TDA_COLORDEPTH_48   = 3     /**< 16 bits per color */
} tda_color_depth;

typedef struct {
	tda_device_version device_version;  /**< HW device version */
	tda_hdmi_version hdmi_version;    /**< Supported HDMI standard version  */
	tda_audio_packet audio_packet;    /**< Supported audio packets */
	tda_color_depth color_depth;     /**< Supported color depth */
	int hdcp;           /**< Supported Hdcp encryption (true/false) */
	int scaler;         /**< Supported scaler (true/false) */
} tda_capabilities;

typedef struct {
	unsigned long compatibility_nr;        /* Interface compatibility number */
	/* Interface major version number */
	unsigned long major_version_nr;
	/* Interface minor version number */
	unsigned long minor_version_nr;
} tda_version;

typedef enum {
	power_on,                          /* Device powered on      (D0 state) */
	power_standby,                     /* Device power standby   (D1 state) */
	power_suspend,                     /* Device power suspended (D2 state) */
	power_off                          /* Device powered off     (D3 state) */
} tda_power_xxx;

typedef struct {
	unsigned int simplay_hd;          /**< Enable simplayHD support */
	unsigned int repeater_enable;     /**< Enable repeater mode */
	unsigned char *p_edid_buffer;       /**< Pointer to raw EDID data */
	/**< Size of buffer for raw EDID data */
	unsigned long edid_buffer_size;
} tda_setup;

typedef struct {
	tda_video_fmt_id id;
	tda_video_fmt_specs spec;
} tda_video_format;

typedef struct {
	tda_video_in video_in;
	tda_video_out video_out;
	tda_audio_in audio_in;
} tda_set_in_out;

typedef struct {
	tda_edid_audio_desc desc;
	unsigned int max;
	unsigned int written;
	unsigned char flags;
} tda_edid_audio_caps;

typedef struct {
	tda_edid_video_desc desc;
	unsigned int max;
	unsigned int written;
	unsigned char flags;
} tda_edid_video_caps;

typedef struct {
	tda_edid_status status;
	unsigned char block_count;
} tda_edid;

#endif

#define TDA_IOCTL_BASE 0x40
#define RELEASE 0xFF

enum {
	/* driver specific */
	TDA_VERBOSE_ON_CMD = 0,
	TDA_VERBOSE_OFF_CMD,
	TDA_BYEBYE_CMD,
	/* HDMI Tx */
	TDA_GET_SW_VERSION_CMD,
	TDA_SET_POWER_CMD,
	TDA_GET_POWER_CMD,
	TDA_SETUP_CMD,
	TDA_GET_SETUP_CMD,
	TDA_WAIT_EVENT_CMD,
	TDA_ENABLE_EVENT_CMD,
	TDA_DISABLE_EVENT_CMD,
	TDA_GET_VIDEO_SPEC_CMD,
	TDA_SET_INPUT_OUTPUT_CMD,
	TDA_SET_AUDIO_INPUT_CMD,
	TDA_SET_VIDEO_INFOFRAME_CMD,
	TDA_SET_AUDIO_INFOFRAME_CMD,
	TDA_SET_ACP_CMD,
	TDA_SET_GCP_CMD,
	TDA_SET_ISRC1_CMD,
	TDA_SET_ISRC2_CMD,
	TDA_SET_MPS_INFOFRAME_CMD,
	TDA_SET_SPD_INFOFRAME_CMD,
	TDA_SET_VS_INFOFRAME_CMD,
	TDA_SET_AUDIO_MUTE_CMD,
	TDA_RESET_AUDIO_CTS_CMD,
	TDA_GET_EDID_STATUS_CMD,
	TDA_GET_EDID_AUDIO_CAPS_CMD,
	TDA_GET_EDID_VIDEO_CAPS_CMD,
	TDA_GET_EDID_VIDEO_PREF_CMD,
	TDA_GET_EDID_SINK_TYPE_CMD,
	TDA_GET_EDID_SOURCE_ADDRESS_CMD,
	TDA_SET_GAMMUT_CMD,
	TDA_GET_EDID_DTD_CMD,
	TDA_GET_EDID_MD_CMD,
	TDA_GET_EDID_TV_ASPECT_RATIO_CMD,
	TDA_GET_EDID_LATENCY_CMD,
	TDA_SET_HDCP_CMD,
	TDA_GET_HDCP_STATUS_CMD,
};

/* driver specific */
#define TDA_IOCTL_VERBOSE_ON     _IO(TDA_IOCTL_BASE, TDA_VERBOSE_ON_CMD)
#define TDA_IOCTL_VERBOSE_OFF     _IO(TDA_IOCTL_BASE, TDA_VERBOSE_OFF_CMD)
#define TDA_IOCTL_BYEBYE     _IO(TDA_IOCTL_BASE, TDA_BYEBYE_CMD)
/* HDMI Tx */
#define TDA_IOCTL_GET_SW_VERSION     _IOWR(TDA_IOCTL_BASE, TDA_GET_SW_VERSION_CMD,tda_version)
#define TDA_IOCTL_SET_POWER     _IOWR(TDA_IOCTL_BASE, TDA_SET_POWER_CMD,tda_power)
#define TDA_IOCTL_GET_POWER     _IOWR(TDA_IOCTL_BASE, TDA_GET_POWER_CMD,tda_power)
#define TDA_IOCTL_SETUP     _IOWR(TDA_IOCTL_BASE, TDA_SETUP_CMD,tda_setup_info)
#define TDA_IOCTL_GET_SETUP     _IOWR(TDA_IOCTL_BASE, TDA_GET_SETUP_CMD,tda_setup_info)
#define TDA_IOCTL_WAIT_EVENT     _IOWR(TDA_IOCTL_BASE, TDA_WAIT_EVENT_CMD,tda_event)
#define TDA_IOCTL_ENABLE_EVENT     _IOWR(TDA_IOCTL_BASE, TDA_ENABLE_EVENT_CMD,tda_event)
#define TDA_IOCTL_DISABLE_EVENT     _IOWR(TDA_IOCTL_BASE, TDA_DISABLE_EVENT_CMD,tda_event)
#define TDA_IOCTL_GET_VIDEO_SPEC     _IOWR(TDA_IOCTL_BASE, TDA_GET_VIDEO_SPEC_CMD,tda_video_format)
#define TDA_IOCTL_SET_INPUT_OUTPUT     _IOWR(TDA_IOCTL_BASE, TDA_SET_INPUT_OUTPUT_CMD,tda_set_in_out)
#define TDA_IOCTL_SET_AUDIO_INPUT     _IOWR(TDA_IOCTL_BASE, TDA_SET_AUDIO_INPUT_CMD,tda_audio_in)
#define TDA_IOCTL_SET_VIDEO_INFOFRAME     _IOWR(TDA_IOCTL_BASE, TDA_SET_VIDEO_INFOFRAME_CMD,tda_video_infoframe)
#define TDA_IOCTL_SET_AUDIO_INFOFRAME     _IOWR(TDA_IOCTL_BASE, TDA_SET_AUDIO_INFOFRAME_CMD,tda_audio_infoframe)
#define TDA_IOCTL_SET_ACP     _IOWR(TDA_IOCTL_BASE, TDA_SET_ACP_CMD,tda_acp)
#define TDA_IOCTL_SET_GCP     _IOWR(TDA_IOCTL_BASE, TDA_SET_GCP_CMD,tda_gcp)
#define TDA_IOCTL_SET_ISRC1     _IOWR(TDA_IOCTL_BASE, TDA_SET_ISRC1_CMD,tda_isrc1)
#define TDA_IOCTL_SET_ISRC2     _IOWR(TDA_IOCTL_BASE, TDA_SET_ISRC2_CMD,tda_isrc2)
#define TDA_IOCTL_SET_MPS_INFOFRAME     _IOWR(TDA_IOCTL_BASE, TDA_SET_MPS_INFOFRAME_CMD,tda_mps_infoframe)
#define TDA_IOCTL_SET_SPD_INFOFRAME     _IOWR(TDA_IOCTL_BASE, TDA_SET_SPD_INFOFRAME_CMD,tda_spd_infoframe)
#define TDA_IOCTL_SET_VS_INFOFRAME     _IOWR(TDA_IOCTL_BASE, TDA_SET_VS_INFOFRAME_CMD,tda_vs_infoframe)
#define TDA_IOCTL_SET_AUDIO_MUTE     _IOWR(TDA_IOCTL_BASE, TDA_SET_AUDIO_MUTE_CMD,bool)
#define TDA_IOCTL_RESET_AUDIO_CTS     _IO(TDA_IOCTL_BASE, TDA_RESET_AUDIO_CTS_CMD)
#define TDA_IOCTL_GET_EDID_STATUS     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_STATUS_CMD,tda_edid)
#define TDA_IOCTL_GET_EDID_AUDIO_CAPS     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_AUDIO_CAPS_CMD,tda_edid_audio_caps)
#define TDA_IOCTL_GET_EDID_VIDEO_CAPS     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_VIDEO_CAPS_CMD,tda_edid_video_caps)
#define TDA_IOCTL_GET_EDID_VIDEO_PREF     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_VIDEO_PREF_CMD,tda_edid_video_timings)
#define TDA_IOCTL_GET_EDID_SINK_TYPE     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_SINK_TYPE_CMD,tda_sink)
#define TDA_IOCTL_GET_EDID_SOURCE_ADDRESS     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_SOURCE_ADDRESS_CMD,unsigned short)
#define TDA_IOCTL_SET_GAMMUT     _IOWR(TDA_IOCTL_BASE, TDA_SET_GAMMUT_CMD,tda_gammut)
#define TDA_IOCTL_GET_EDID_DTD     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_DTD_CMD,tda_edid_dtd)
#define TDA_IOCTL_GET_EDID_MD     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_MD_CMD,tda_edid_md)
#define TDA_IOCTL_GET_EDID_TV_ASPECT_RATIO     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_TV_ASPECT_RATIO_CMD,tda_edid_tv_aspect_ratio)
#ifdef TMFL_TDA19989
#define TDA_IOCTL_GET_EDID_LATENCY     _IOWR(TDA_IOCTL_BASE, TDA_GET_EDID_LATENCY_CMD,tda_edid_latency)
#define TDA_IOCTL_SET_HDCP     _IOWR(TDA_IOCTL_BASE, TDA_SET_HDCP_CMD,bool)
#define TDA_IOCTL_GET_HDCP_STATUS     _IOWR(TDA_IOCTL_BASE, TDA_GET_HDCP_STATUS_CMD,tda_hdcp_status)
#endif

/* --- Full list --- */

/* legend: */
/* ------- */
/* [ ] : not supported */
/* [x] : IOCTL */
/* [i] : open, init... */

/* [x] txGetSWVersion */
/* [ ] txGetNumberOfUnits */
/* [i] txGetCapabilities */
/* [ ] txGetCapabilitiesM */
/* [i] txOpen */
/* [ ] txOpenM */
/* [i] txClose */
/* [x] txSetPowerState */
/* [x] txGetPowerState */
/* [ ] txInstanceConfig */
/* [xi] txInstanceSetup */
/* [x] txGetInstanceSetup */
/* [x] txHandleInterrupt see IOCTL_WAIT_EVENT */
/* [i] txRegisterCallbacks */
/* [x] txEnableEvent */
/* [x] txDisableEvent */
/* [x] txGetVideoFormatSpecs */
/* [x] txSetInputOutput */
/* [x] txSetAudioInput */
/* [x] txSetVideoInfoframe */
/* [x] txSetAudioInfoframe */
/* [x] txSetACPPacket */
/* [x] txSetGeneralControlPacket */
/* [x] txSetISRC1Packet */
/* [x] txSetISRC2Packet */
/* [x] txSetMPSInfoframe */
/* [x] txSetSpdInfoframe */
/* [x] txSetVsInfoframe */
/* [ ] txDebugSetNullPacket */
/* [ ] txDebugSetSingleNullPacket */
/* [x] txSetAudioMute */
/* [x] txResetAudioCts */
/* [x] txGetEdidStatus */
/* [x] txGetEdidAudioCaps */
/* [x] txGetEdidVideoCaps */
/* [x] txGetEdidVideoPreferred */
/* [x] txGetEdidSinkType */
/* [x] txGetEdidSourceAddress */
/* [ ] txGetKsvList */
/* [ ] txGetDepth */
/* [ ] txGeneSHA_1_IT */
/* [ ] txSetHdcp */
/* [ ] txGetHdcpState */
/* [ ] txHdcpCheck */
/* [x] txSetGamutPacket */
/* [x] txGetEdidDetailledTimingDescriptors */
/* [x] txGetEdidMonitorDescriptors */
/* [x] txGetEdidTVPictureRatio */
/* [ ] txSetHDCPRevocationList */
/* [ ] txGetHdcpFailStatus */
/* [x] txGetEdidLatencyInfo */
/* [ ] txSetBScreen */
/* [ ] txRemoveBScreen */

#endif /* __tx_h__ */
#endif /* __tx_ioctl__ */

#ifndef __cec_ioctl__
#define __cec_ioctl__

#ifdef __cec_h__

typedef struct {
	u8 day_of_month;
	u8 month_of_year;
	u16 start_time;
	dl_hdmi_cecduration_t duration;
	u8 recording_sequence;
	dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type;
	u16 analogue_frequency;
	dl_hdmi_cecbroadcast_system_t broadcast_system;
} cec_analogue_timer;

typedef struct {
	u8 day_of_month;
	u8 month_of_year;
	u16 start_time;
	dl_hdmi_cecduration_t duration;
	u8 recording_sequence;
	dl_hdmi_cecdigital_service_identification_t service_identification;
} cec_digital_timer;

typedef struct {
	u8 day_of_month;
	u8 month_of_year;
	u16 start_time;
	dl_hdmi_cecduration_t duration;
	u8 recording_sequence;
	dl_hdmi_cecexternal_plug_t external_plug;
} cec_ext_timer_with_ext_plug;

typedef struct {
	u8 day_of_month;
	u8 month_of_year;
	u16 start_time;
	dl_hdmi_cecduration_t duration;
	u8 recording_sequence;
	dl_hdmi_cecexternal_physical_address_t external_physical_address;
} cec_ext_timer_with_phy_addr;

typedef struct {
	dl_hdmi_cecfeature_opcode_t feature_opcode;
	dl_hdmi_cecabort_reason_t abort_reason;
} cec_feature_abort;

typedef struct {
	dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type;
	u16 analogue_frequency;
	dl_hdmi_cecbroadcast_system_t broadcast_system;
} cec_analogue_service;

typedef struct {
	u16 original_address;
	u16 new_address;
} cec_routing_change;

typedef struct {
	char data[15];
	unsigned char length;
} cec_string;

typedef struct {
	dl_hdmi_cecdisplay_control_t display_control;
	char data[15];
	unsigned char length;
} cec_osd_string;

typedef struct {
	dl_hdmi_cecrecording_flag_t recording_flag;
	dl_hdmi_cectuner_display_info_t tuner_display_info;
	dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type;
	u16 analogue_frequency;
	dl_hdmi_cecbroadcast_system_t broadcast_system;
} cec_tuner_device_status_analogue;

typedef struct {
	dl_hdmi_cecrecording_flag_t recording_flag;
	dl_hdmi_cectuner_display_info_t tuner_display_info;
	dl_hdmi_cecdigital_service_identification_t service_identification;
} cec_tuner_device_status_digital;

typedef struct {
	unsigned long vendor_id;
	cec_string cmd;
} cec_vendor_command_with_id;

/*
 *  typedef struct {
 *  u8 *pData;
 *  u16 lenData;
 *  } cec_send_msg;
 * */

typedef struct {
	unsigned char count;
	unsigned char service;
	unsigned char addr;
	unsigned char data[15];
} cec_frame;
/* typedef dlHdmiCecFrameFormat_t cec_frame; */

typedef swversion_t cec_sw_version;
typedef power_state_t cec_power;
typedef dl_hdmi_cec_instance_setup_t cec_setup;
typedef dl_hdmi_cec_event_t cec_event;
typedef dl_hdmi_cec_clock_source_t cec_clock;
typedef dl_hdmi_cecsystem_audio_status_t cec_sys_audio_status;
typedef dl_hdmi_cecaudio_rate_t cec_audio_rate;
typedef dl_hdmi_cecdigital_service_identification_t cec_digital_service;
typedef dl_hdmi_cecversion_t cec_version;
typedef dl_hdmi_cecdec_control_mode_t cec_deck_ctrl;
typedef dl_hdmi_cecdec_info_t cec_deck_status;
typedef dl_hdmi_cecstatus_request_t cec_status_request;
typedef dl_hdmi_cecmenu_request_type_t cec_menu_request;
typedef dl_hdmi_cecmenu_state_t cec_menu_status;
typedef dl_hdmi_cecplay_mode_t cec_play;
typedef dl_hdmi_cecexternal_plug_t cec_ext_plug;
typedef dl_hdmi_cecrecord_status_info_t cec_rec_status;
typedef dl_hdmi_cecaudio_status_t cec_audio_status;
typedef dl_hdmi_cecpower_status_t cec_power_status;
typedef dl_hdmi_cectimer_cleared_status_data_t cec_timer_cleared_status;
typedef dl_hdmi_cectimer_status_data_t cec_timer_status;
typedef dl_hdmi_cecuser_remote_control_command_t cec_user_ctrl;
typedef dl_hdmi_cecchannel_identifier_t cec_user_ctrl_tune;
typedef dl_hdmi_cecdevice_type_t cec_device_type;

#define CEC_IOCTL_BASE 0x40

/* service */
enum {
	CEC_WAITING = 0x80,
	CEC_RELEASE,
	CEC_RX_DONE,
	CEC_TX_DONE
};

enum {
	/* driver specific */
	CEC_VERBOSE_ON_CMD = 0,
	CEC_VERBOSE_OFF_CMD,
	CEC_BYEBYE_CMD,

	/* CEC */
	CEC_IOCTL_RX_ADDR_CMD, /* receiver logical address selector */
	CEC_IOCTL_PHY_ADDR_CMD, /* physical address selector */
	CEC_IOCTL_WAIT_FRAME_CMD,
	CEC_IOCTL_ABORT_MSG_CMD,
	CEC_IOCTL_ACTIVE_SRC_CMD,
	CEC_IOCTL_VERSION_CMD,
	CEC_IOCTL_CLEAR_ANALOGUE_TIMER_CMD,
	CEC_IOCTL_CLEAR_DIGITAL_TIMER_CMD,
	CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG_CMD,
	CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR_CMD,
	CEC_IOCTL_DECK_CTRL_CMD,
	CEC_IOCTL_DECK_STATUS_CMD,
	CEC_IOCTL_DEVICE_VENDOR_ID_CMD,
	CEC_IOCTL_FEATURE_ABORT_CMD,
	CEC_IOCTL_GET_CEC_VERSION_CMD,
	CEC_IOCTL_GET_MENU_LANGUAGE_CMD,
	CEC_IOCTL_GIVE_AUDIO_STATUS_CMD,
	CEC_IOCTL_GIVE_DECK_STATUS_CMD,
	CEC_IOCTL_GIVE_DEVICE_POWER_STATUS_CMD,
	CEC_IOCTL_GIVE_DEVICE_VENDOR_ID_CMD,
	CEC_IOCTL_GIVE_OSD_NAME_CMD,
	CEC_IOCTL_GIVE_PHY_ADDR_CMD,
	CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS_CMD,
	CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS_CMD,
	CEC_IOCTL_IMAGE_VIEW_ON_CMD,
	CEC_IOCTL_INACTIVE_SRC_CMD,
	CEC_IOCTL_MENU_REQUEST_CMD,
	CEC_IOCTL_MENU_STATUS_CMD,
	CEC_IOCTL_PLAY_CMD,
	CEC_IOCTL_POLLING_MSG_CMD,
	CEC_IOCTL_REC_OFF_CMD,
	CEC_IOCTL_REC_ON_ANALOGUE_SERVICE_CMD,
	CEC_IOCTL_REC_ON_DIGITAL_SERVICE_CMD,
	CEC_IOCTL_REC_ON_EXT_PHY_ADDR_CMD,
	CEC_IOCTL_REC_ON_EXT_PLUG_CMD,
	CEC_IOCTL_REC_ON_OWN_SRC_CMD,
	CEC_IOCTL_REC_STATUS_CMD,
	CEC_IOCTL_REC_TV_SCREEN_CMD,
	CEC_IOCTL_REPORT_AUDIO_STATUS_CMD,
	CEC_IOCTL_REPORT_PHY_ADDR_CMD,
	CEC_IOCTL_REPORT_POWER_STATUS_CMD,
	CEC_IOCTL_REQUEST_ACTIVE_SRC_CMD,
	CEC_IOCTL_ROUTING_CHANGE_CMD,
	CEC_IOCTL_ROUTING_INFORMATION_CMD,
	CEC_IOCTL_SELECT_ANALOGUE_SERVICE_CMD,
	CEC_IOCTL_SELECT_DIGITAL_SERVICE_CMD,
	CEC_IOCTL_SET_ANALOGUE_TIMER_CMD,
	CEC_IOCTL_SET_AUDIO_RATE_CMD,
	CEC_IOCTL_SET_DIGITAL_TIMER_CMD,
	CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG_CMD,
	CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR_CMD,
	CEC_IOCTL_SET_MENU_LANGUAGE_CMD,
	CEC_IOCTL_SET_OSD_NAME_CMD,
	CEC_IOCTL_SET_OSD_STRING_CMD,
	CEC_IOCTL_SET_STREAM_PATH_CMD,
	CEC_IOCTL_SET_SYS_AUDIO_MODE_CMD,
	CEC_IOCTL_SET_TIMER_PROGRAM_TITLE_CMD,
	CEC_IOCTL_STANDBY_CMD,
	CEC_IOCTL_SYS_AUDIO_MODE_REQUEST_CMD,
	CEC_IOCTL_SYS_AUDIO_MODE_STATUS_CMD,
	CEC_IOCTL_TEXT_VIEW_ON_CMD,
	CEC_IOCTL_TIMER_CLEARED_STATUS_CMD,
	CEC_IOCTL_TIMER_STATUS_CMD,
	CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE_CMD,
	CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL_CMD,
	CEC_IOCTL_TUNER_STEP_DECREMENT_CMD,
	CEC_IOCTL_TUNER_STEP_INCREMENT_CMD,
	CEC_IOCTL_USER_CTRL_CMD,
	CEC_IOCTL_USER_CTRL_PLAY_CMD,
	CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT_CMD,
	CEC_IOCTL_USER_CTRL_SELECT_AVINPUT_CMD,
	CEC_IOCTL_USER_CTRL_SELECT_MEDIA_CMD,
	CEC_IOCTL_USER_CTRL_TUNE_CMD,
	CEC_IOCTL_USER_CTRL_RELEASED_CMD,
	CEC_IOCTL_VENDOR_COMMAND_CMD,
	CEC_IOCTL_VENDOR_COMMAND_WITH_ID_CMD,
	CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN_CMD,
	CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP_CMD,
	CEC_IOCTL_GET_SW_VERSION_CMD,
	CEC_IOCTL_SET_POWER_STATE_CMD,
	CEC_IOCTL_GET_POWER_STATE_CMD,
	CEC_IOCTL_INSTANCE_CONFIG_CMD,
	CEC_IOCTL_INSTANCE_SETUP_CMD,
	CEC_IOCTL_GET_INSTANCE_SETUP_CMD,
	CEC_IOCTL_ENABLE_EVENT_CMD,
	CEC_IOCTL_DISABLE_EVENT_CMD,
	CEC_IOCTL_ENABLE_CALIBRATION_CMD,
	CEC_IOCTL_DISABLE_CALIBRATION_CMD,
	CEC_IOCTL_SEND_MSG_CMD,
	CEC_IOCTL_SET_REGISTER_CMD
};

/* driver specific */
#define CEC_IOCTL_VERBOSE_ON       _IO(CEC_IOCTL_BASE, CEC_VERBOSE_ON_CMD)
#define CEC_IOCTL_VERBOSE_OFF      _IO(CEC_IOCTL_BASE, CEC_VERBOSE_OFF_CMD)
#define CEC_IOCTL_BYEBYE      _IO(CEC_IOCTL_BASE, CEC_BYEBYE_CMD)

/* CEC */
#define CEC_IOCTL_RX_ADDR      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_RX_ADDR_CMD,unsigned char)
#define CEC_IOCTL_PHY_ADDR      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_PHY_ADDR_CMD,unsigned short)
#define CEC_IOCTL_WAIT_FRAME      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_WAIT_FRAME_CMD,cec_frame)
#define CEC_IOCTL_ABORT_MSG      _IO(CEC_IOCTL_BASE,CEC_IOCTL_ABORT_MSG_CMD)
#define CEC_IOCTL_ACTIVE_SRC      _IO(CEC_IOCTL_BASE,CEC_IOCTL_ACTIVE_SRC_CMD)
#define CEC_IOCTL_VERSION      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_VERSION_CMD,cec_version)
#define CEC_IOCTL_CLEAR_ANALOGUE_TIMER      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_CLEAR_ANALOGUE_TIMER_CMD,cec_analogue_timer)
#define CEC_IOCTL_CLEAR_DIGITAL_TIMER      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_CLEAR_DIGITAL_TIMER_CMD,cec_digital_timer)
#define CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG_CMD,cec_ext_timer_with_ext_plug)
#define CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR_CMD,cec_ext_timer_with_phy_addr)
#define CEC_IOCTL_DECK_CTRL      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_DECK_CTRL_CMD,cec_deck_ctrl)
#define CEC_IOCTL_DECK_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_DECK_STATUS_CMD,cec_deck_status)
#define CEC_IOCTL_DEVICE_VENDOR_ID      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_DEVICE_VENDOR_ID_CMD,unsigned long)
#define CEC_IOCTL_FEATURE_ABORT      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_FEATURE_ABORT_CMD,cec_feature_abort)
#define CEC_IOCTL_GET_CEC_VERSION      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GET_CEC_VERSION_CMD,unsigned char)
#define CEC_IOCTL_GET_MENU_LANGUAGE      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GET_MENU_LANGUAGE_CMD)
#define CEC_IOCTL_GIVE_AUDIO_STATUS      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_AUDIO_STATUS_CMD)
#define CEC_IOCTL_GIVE_DECK_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_DECK_STATUS_CMD,cec_status_request)
#define CEC_IOCTL_GIVE_DEVICE_POWER_STATUS      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_DEVICE_POWER_STATUS_CMD)
#define CEC_IOCTL_GIVE_DEVICE_VENDOR_ID      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_DEVICE_VENDOR_ID_CMD)
#define CEC_IOCTL_GIVE_OSD_NAME      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_OSD_NAME_CMD)
#define CEC_IOCTL_GIVE_PHY_ADDR      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_PHY_ADDR_CMD)
#define CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS      _IO(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS_CMD)
#define CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS_CMD,cec_status_request)
#define CEC_IOCTL_IMAGE_VIEW_ON      _IO(CEC_IOCTL_BASE,CEC_IOCTL_IMAGE_VIEW_ON_CMD)
#define CEC_IOCTL_INACTIVE_SRC      _IO(CEC_IOCTL_BASE,CEC_IOCTL_INACTIVE_SRC_CMD)
#define CEC_IOCTL_MENU_REQUEST      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_MENU_REQUEST_CMD,cec_menu_request)
#define CEC_IOCTL_MENU_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_MENU_STATUS_CMD,cec_menu_status)
#define CEC_IOCTL_PLAY      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_PLAY_CMD,cec_play)
#define CEC_IOCTL_POLLING_MSG      _IO(CEC_IOCTL_BASE,CEC_IOCTL_POLLING_MSG_CMD)
#define CEC_IOCTL_REC_OFF      _IO(CEC_IOCTL_BASE,CEC_IOCTL_REC_OFF_CMD)
#define CEC_IOCTL_REC_ON_ANALOGUE_SERVICE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REC_ON_ANALOGUE_SERVICE_CMD,cec_analogue_service)
#define CEC_IOCTL_REC_ON_DIGITAL_SERVICE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REC_ON_DIGITAL_SERVICE_CMD,cec_digital_service)
#define CEC_IOCTL_REC_ON_EXT_PHY_ADDR      _IO(CEC_IOCTL_BASE,CEC_IOCTL_REC_ON_EXT_PHY_ADDR_CMD)
#define CEC_IOCTL_REC_ON_EXT_PLUG      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REC_ON_EXT_PLUG_CMD,cec_ext_plug)
#define CEC_IOCTL_REC_ON_OWN_SRC      _IO(CEC_IOCTL_BASE,CEC_IOCTL_REC_ON_OWN_SRC_CMD)
#define CEC_IOCTL_REC_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REC_STATUS_CMD,cec_rec_status)
#define CEC_IOCTL_REC_TV_SCREEN      _IO(CEC_IOCTL_BASE,CEC_IOCTL_REC_TV_SCREEN_CMD)
#define CEC_IOCTL_REPORT_AUDIO_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REPORT_AUDIO_STATUS_CMD,cec_audio_status)
#define CEC_IOCTL_REPORT_PHY_ADDR      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REPORT_PHY_ADDR_CMD,cec_device_type)
#define CEC_IOCTL_REPORT_POWER_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_REPORT_POWER_STATUS_CMD,cec_power_status)
#define CEC_IOCTL_REQUEST_ACTIVE_SRC      _IO(CEC_IOCTL_BASE,CEC_IOCTL_REQUEST_ACTIVE_SRC_CMD)
#define CEC_IOCTL_ROUTING_CHANGE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_ROUTING_CHANGE_CMD,cec_routing_change)
#define CEC_IOCTL_ROUTING_INFORMATION      _IO(CEC_IOCTL_BASE,CEC_IOCTL_ROUTING_INFORMATION_CMD)
#define CEC_IOCTL_SELECT_ANALOGUE_SERVICE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SELECT_ANALOGUE_SERVICE_CMD,cec_analogue_service)
#define CEC_IOCTL_SELECT_DIGITAL_SERVICE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SELECT_DIGITAL_SERVICE_CMD,cec_digital_service)
#define CEC_IOCTL_SET_ANALOGUE_TIMER      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_ANALOGUE_TIMER_CMD,cec_analogue_timer)
#define CEC_IOCTL_SET_AUDIO_RATE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_AUDIO_RATE_CMD,cec_audio_rate)
#define CEC_IOCTL_SET_DIGITAL_TIMER      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_DIGITAL_TIMER_CMD,cec_digital_timer)
#define CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG_CMD,cec_ext_timer_with_ext_plug)
#define CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR_CMD,cec_ext_timer_with_phy_addr)
#define CEC_IOCTL_SET_MENU_LANGUAGE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_MENU_LANGUAGE_CMD,cec_string)
#define CEC_IOCTL_SET_OSD_NAME      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_OSD_NAME_CMD,cec_string)
#define CEC_IOCTL_SET_OSD_STRING      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_OSD_STRING_CMD,cec_osd_string)
#define CEC_IOCTL_SET_STREAM_PATH      _IO(CEC_IOCTL_BASE,CEC_IOCTL_SET_STREAM_PATH_CMD)
#define CEC_IOCTL_SET_SYS_AUDIO_MODE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_SYS_AUDIO_MODE_CMD,cec_sys_audio_status)
#define CEC_IOCTL_SET_TIMER_PROGRAM_TITLE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_TIMER_PROGRAM_TITLE_CMD,cec_string)
#define CEC_IOCTL_STANDBY      _IO(CEC_IOCTL_BASE,CEC_IOCTL_STANDBY_CMD)
#define CEC_IOCTL_SYS_AUDIO_MODE_REQUEST      _IO(CEC_IOCTL_BASE,CEC_IOCTL_SYS_AUDIO_MODE_REQUEST_CMD)
#define CEC_IOCTL_SYS_AUDIO_MODE_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SYS_AUDIO_MODE_STATUS_CMD,cec_sys_audio_status)
#define CEC_IOCTL_TEXT_VIEW_ON      _IO(CEC_IOCTL_BASE,CEC_IOCTL_TEXT_VIEW_ON_CMD)
#define CEC_IOCTL_TIMER_CLEARED_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_TIMER_CLEARED_STATUS_CMD,cec_timer_cleared_status)
#define CEC_IOCTL_TIMER_STATUS      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_TIMER_STATUS_CMD,cec_timer_status)
#define CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE_CMD,cec_tuner_device_status_analogue)
#define CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL_CMD,cec_tuner_device_status_digital)
#define CEC_IOCTL_TUNER_STEP_DECREMENT      _IO(CEC_IOCTL_BASE,CEC_IOCTL_TUNER_STEP_DECREMENT_CMD)
#define CEC_IOCTL_TUNER_STEP_INCREMENT      _IO(CEC_IOCTL_BASE,CEC_IOCTL_TUNER_STEP_INCREMENT_CMD)
#define CEC_IOCTL_USER_CTRL_PRESSED      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_CMD,cec_user_ctrl)
#define CEC_IOCTL_USER_CTRL_PLAY      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_PLAY_CMD,cec_play)
#define CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT_CMD,unsigned char)
#define CEC_IOCTL_USER_CTRL_SELECT_AVINPUT      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_SELECT_AVINPUT_CMD,unsigned char)
#define CEC_IOCTL_USER_CTRL_SELECT_MEDIA      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_SELECT_MEDIA_CMD,unsigned char)
#define CEC_IOCTL_USER_CTRL_TUNE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_TUNE_CMD,cec_user_ctrl_tune)
#define CEC_IOCTL_USER_CTRL_RELEASED      _IO(CEC_IOCTL_BASE,CEC_IOCTL_USER_CTRL_RELEASED_CMD)
#define CEC_IOCTL_VENDOR_COMMAND      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_VENDOR_COMMAND_CMD,cec_string)
#define CEC_IOCTL_VENDOR_COMMAND_WITH_ID      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_VENDOR_COMMAND_WITH_ID_CMD,cec_vendor_command_with_id)
#define CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN_CMD,cec_string)
#define CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP      _IO(CEC_IOCTL_BASE,CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP_CMD)
#define CEC_IOCTL_GET_SW_VERSION      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GET_SW_VERSION_CMD,cec_sw_version)
#define CEC_IOCTL_SET_POWER_STATE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SET_POWER_STATE_CMD,cec_power)
#define CEC_IOCTL_GET_POWER_STATE      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GET_POWER_STATE_CMD,cec_power)
#define CEC_IOCTL_INSTANCE_CONFIG      _IO(CEC_IOCTL_BASE,CEC_IOCTL_INSTANCE_CONFIG_CMD)
#define CEC_IOCTL_INSTANCE_SETUP      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_INSTANCE_SETUP_CMD,cec_setup)
#define CEC_IOCTL_GET_INSTANCE_SETUP      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_GET_INSTANCE_SETUP_CMD,cec_setup)
#define CEC_IOCTL_ENABLE_EVENT      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_ENABLE_EVENT_CMD,cec_event)
#define CEC_IOCTL_DISABLE_EVENT      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_DISABLE_EVENT_CMD,cec_event)
#define CEC_IOCTL_ENABLE_CALIBRATION      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_ENABLE_CALIBRATION_CMD,cec_clock)
#define CEC_IOCTL_DISABLE_CALIBRATION      _IO(CEC_IOCTL_BASE,CEC_IOCTL_DISABLE_CALIBRATION_CMD)
/*#define CEC_IOCTL_SEND_MSG      _IOWR(CEC_IOCTL_BASE,CEC_IOCTL_SEND_MSG_CMD,cec_send_msg) */

/* --- Full list --- */

/* legend: */
/* ------- */
/* [ ] : not supported */
/* [x] : IOCTL */
/* [i] : open, init... */

/* [ ] dlHdmiCecAbortMessage */
/* [ ] dlHdmiCecActiveSource */
/* [ ] dlHdmiCecVersion */
/* [ ] dlHdmiCecClearAnalogueTimer */
/* [ ] dlHdmiCecClearDigitalTimer */
/* [ ] dlHdmiCecClearExternalTimerWithExternalPlug */
/* [ ] dlHdmiCecClearExternalTimerWithPhysicalAddress */
/* [ ] dlHdmiCecDeckControl */
/* [ ] dlHdmiCecDeckStatus */
/* [ ] dlHdmiCecDeviceVendorID */
/* [ ] dlHdmiCecFeatureAbort */
/* [ ] dlHdmiCecGetCecVersion */
/* [ ] dlHdmiCecGetMenuLanguage */
/* [ ] dlHdmiCecGiveAudioStatus */
/* [ ] dlHdmiCecGiveDeckStatus */
/* [ ] dlHdmiCecGiveDevicePowerStatus */
/* [ ] dlHdmiCecGiveDeviceVendorID */
/* [ ] dlHdmiCecGiveOsdName */
/* [ ] dlHdmiCecGivePhysicalAddress */
/* [ ] dlHdmiCecGiveSystemAudioModeStatus */
/* [ ] dlHdmiCecGiveTunerDeviceStatus */
/* [ ] dlHdmiCecImageViewOn */
/* [ ] dlHdmiCecInactiveSource */
/* [ ] dlHdmiCecMenuRequest */
/* [ ] dlHdmiCecMenuStatus */
/* [ ] dlHdmiCecPlay */
/* [ ] dlHdmiCecPollingMessage */
/* [ ] dlHdmiCecRecordOff */
/* [ ] dlHdmiCecRecordOnAnalogueService */
/* [ ] dlHdmiCecRecordOnDigitalService */
/* [ ] dlHdmiCecRecordOnExternalPhysicalAddress */
/* [ ] dlHdmiCecRecordOnExternalPlug */
/* [ ] dlHdmiCecRecordOnOwnSource */
/* [ ] dlHdmiCecRecordStatus */
/* [ ] dlHdmiCecRecordTvScreen */
/* [ ] dlHdmiCecReportAudioStatus */
/* [ ] dlHdmiCecReportPhysicalAddress */
/* [ ] dlHdmiCecReportPowerStatus */
/* [ ] dlHdmiCecRequestActiveSource */
/* [ ] dlHdmiCecRoutingChange */
/* [ ] dlHdmiCecRoutingInformation */
/* [ ] dlHdmiCecSelectAnalogueService */
/* [ ] dlHdmiCecSelectDigitalService */
/* [ ] dlHdmiCecSetAnalogueTimer */
/* [ ] dlHdmiCecSetAudioRate */
/* [ ] dlHdmiCecSetDigitalTimer */
/* [ ] dlHdmiCecSetExternalTimerWithExternalPlug */
/* [ ] dlHdmiCecSetExternalTimerWithPhysicalAddress */
/* [ ] dlHdmiCecSetMenuLanguage */
/* [ ] dlHdmiCecSetOsdName */
/* [ ] dlHdmiCecSetOsdString */
/* [ ] dlHdmiCecSetStreamPath */
/* [ ] dlHdmiCecSetSystemAudioMode */
/* [ ] dlHdmiCecSetTimerProgramTitle */
/* [ ] dlHdmiCecStandby */
/* [ ] dlHdmiCecSystemAudioModeRequest */
/* [ ] dlHdmiCecSystemAudioModeStatus */
/* [ ] dlHdmiCecTextViewOn */
/* [ ] dlHdmiCecTimerClearedStatus */
/* [ ] dlHdmiCecTimerStatus */
/* [ ] dlHdmiCecTunerDeviceStatusAnalogue */
/* [ ] dlHdmiCecTunerDeviceStatusDigital */
/* [ ] dlHdmiCecTunerStepDecrement */
/* [ ] dlHdmiCecTunerStepIncrement */
/* [ ] dlHdmiCecUserControlPressed */
/* [ ] dlHdmiCecUserControlPressedPlay */
/* [ ] dlHdmiCecUserControlPressedSelectAudioInput */
/* [ ] dlHdmiCecUserControlPressedSelectAVInput */
/* [ ] dlHdmiCecUserControlPressedSelectMedia */
/* [ ] dlHdmiCecUserControlPressedTune */
/* [ ] dlHdmiCecUserControlReleased */
/* [ ] dlHdmiCecVendorCommand */
/* [ ] dlHdmiCecVendorCommandWithID */
/* [ ] dlHdmiCecVendorRemoteButtonDown */
/* [ ] dlHdmiCecVendorRemoteButtonUp */
/* [ ] dlHdmiCecGetSWVersion */
/* [ ] dlHdmiCecGetNumberOfUnits */
/* [ ] dlHdmiCecGetCapabilities */
/* [ ] dlHdmiCecGetCapabilitiesM */
/* [ ] dlHdmiCecOpen */
/* [ ] dlHdmiCecOpenM */
/* [ ] dlHdmiCecClose */
/* [ ] dlHdmiCecSetPowerState */
/* [ ] dlHdmiCecGetPowerState */
/* [ ] dlHdmiCecInstanceConfig */
/* [ ] dlHdmiCecInstanceSetup */
/* [ ] dlHdmiCecGetInstanceSetup */
/* [ ] dlHdmiCecHandleInterrupt */
/* [ ] dlHdmiCecRegisterCallbacks */
/* [ ] dlHdmiCecSetAutoAnswer */
/* [ ] dlHdmiCecSetLogicalAddress */
/* [ ] dlHdmiCecEnableEvent */
/* [ ] dlHdmiCecDisableEvent */
/* [ ] dlHdmiCecEnableCalibration */
/* [ ] dlHdmiCecDisableCalibration */
/* [ ] dlHdmiCecSendMessage */
/* [ ] dlHdmiCecSetRegister */

#endif /* __cec_h__ */
#endif /* __cec_ioctl__ */
