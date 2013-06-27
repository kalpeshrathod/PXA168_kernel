/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_cfg.h
 *
 * \version       $Revision: 1 $
 *
 * \date          $Date: 08/08/07 11:00 $
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
 *   $History: bsl_cfg.h $
 *
 * *****************  Version 1  *****************
 * User: J. Lamotte Date: 08/08/07  Time: 11:00
 * initial version
 *
 *
 *   \endverbatim
 *
 * */

/*****************************************************************************/
/*****************************************************************************/
/*                THIS FILE MUST NOT BE MODIFIED BY CUSTOMER                 */
/*****************************************************************************/
/*****************************************************************************/

#ifndef DLHDMITX_CFG_H
#define DLHDMITX_CFG_H

#include "tmNxTypes.h"
#include "tmbslHdmiTx.h"
#include "tmdlHdmiTx_Types.h"

/*============================================================================*/
/*                          TYPES DECLARATIONS                                */
/*============================================================================*/
typedef struct _tx_cfg_resolution_t {
	tx_vid_fmt_t          resolution_id;
	u16                      width;
	u16                      height;
	bool                        interlaced;
	tx_vfreq_t           vfrequency;
	tx_pict_aspect_ratio_t aspect_ratio;
} tx_cfg_resolution_t, *ptx_cfg_resolution_t;

/**
 * \brief Video signals that can be input to video ports in RGB/YUV 4:4:4 mode
 * */
typedef enum {
	/**< Video signal G/Y, bits 4 to 7 */
	DL_HDMITX_VID444_GY_4_TO_7 = 0x00,
	/**< Video signal G/Y, bits 0 to 3 */
	DL_HDMITX_VID444_GY_0_TO_3 = 0x01,
	/**< Video signal B/U, bits 4 to 7 */
	DL_HDMITX_VID444_BU_4_TO_7 = 0x02,
	/**< Video signal B/U, bits 0 to 3 */
	DL_HDMITX_VID444_BU_0_TO_3 = 0x03,
	/**< Video signal V/R, bits 4 to 7 */
	DL_HDMITX_VID444_VR_4_TO_7 = 0x04,
	/**< Video signal V/R, bits 0 to 3 */
	DL_HDMITX_VID444_VR_0_TO_3 = 0x05,
	/**< Video signal G/Y, bits 7 to 4 (mirrored) */
	DL_HDMITX_VID444_GY_7_TO_4 = 0x80,
	/**< Video signal G/Y, bits 3 to 0 (mirrored) */
	DL_HDMITX_VID444_GY_3_TO_0 = 0x81,
	/**< Video signal B/U, bits 7 to 4 (mirrored) */
	DL_HDMITX_VID444_BU_7_TO_4 = 0x82,
	/**< Video signal B/U, bits 3 to 0 (mirrored) */
	DL_HDMITX_VID444_BU_3_TO_0 = 0x83,
	/**< Video signal V/R, bits 7 to 4 (mirrored) */
	DL_HDMITX_VID444_VR_7_TO_4 = 0x84,
	/**< Video signal V/R, bits 3 to 0 (mirrored) */
	DL_HDMITX_VID444_VR_3_TO_0 = 0x85,
	DL_HDMITX_VID444_NOT_CONNECTED = 0x100    /**< No signal connected */
} tx_cfg_video_signal444;

/**
 * \brief Video signals that can be input to video ports in semi-planar YUV 4:2:2 mode
 * */
typedef enum {
	/**< Video signal G/Y, bits 8 to 11 */
	DL_HDMITX_VID422_Y_8_TO_11     = 0x00,
	/**< Video signal G/Y, bits 4 to 7 */
	DL_HDMITX_VID422_Y_4_TO_7      = 0x01,
	/**< Video signal G/Y, bits 0 to 3 */
	DL_HDMITX_VID422_Y_0_TO_3      = 0x02,
	/**< Video signal B/U, bits 8 to 11 */
	DL_HDMITX_VID422_UV_8_TO_11    = 0x03,
	/**< Video signal B/U, bits 4 to 7 */
	DL_HDMITX_VID422_UV_4_TO_7     = 0x04,
	/**< Video signal B/U, bits 0 to 3 */
	DL_HDMITX_VID422_UV_0_TO_3     = 0x05,
	/**< Video signal G/Y, bits 11 to 8 (mirrored) */
	DL_HDMITX_VID422_Y_11_TO_8     = 0x80,
	/**< Video signal G/Y, bits 7 to 4 (mirrored) */
	DL_HDMITX_VID422_Y_7_TO_4      = 0x81,
	/**< Video signal G/Y, bits 3 to 0 (mirrored) */
	DL_HDMITX_VID422_Y_3_TO_0      = 0x82,
	/**< Video signal B/U, bits 11 to 8 (mirrored) */
	DL_HDMITX_VID422_UV_11_TO_8    = 0x83,
	/**< Video signal B/U, bits 7 to 4 (mirrored) */
	DL_HDMITX_VID422_UV_7_TO_4     = 0x84,
	/**< Video signal B/U, bits 3 to 0 (mirrored) */
	DL_HDMITX_VID422_UV_3_TO_0     = 0x85,
	DL_HDMITX_VID422_NOT_CONNECTED = 0x100    /**< No signal connected */
} tx_cfg_video_signal422;

/**
 *
 * \brief Video signals that can be input to video ports in semi-planar CCIR 656 mode
 * */
typedef enum {
	/**< Video signal CCIR, bits 8 to 11 */
	DL_HDMITX_VIDCCIR_8_TO_11       = 0x00,
	/**< Video signal CCIR, bits 4 to 7 */
	DL_HDMITX_VIDCCIR_4_TO_7        = 0x01,
	/**< Video signal CCIR, bits 0 to 3 */
	DL_HDMITX_VIDCCIR_0_TO_3        = 0x02,
	/**< Video signal CCIR, bits 11 to 8 (mirrored) */
	DL_HDMITX_VIDCCIR_11_TO_8       = 0x80,
	/**< Video signal CCIR, bits 7 to 4 (mirrored) */
	DL_HDMITX_VIDCCIR_7_TO_4        = 0x81,
	/**< Video signal CCIR, bits 3 to 0 (mirrored) */
	DL_HDMITX_VIDCCIR_3_TO_0        = 0x82,
	DL_HDMITX_VIDCCIR_NOT_CONNECTED = 0x100   /**< No signal connected */
} tx_cfg_video_signal_ccir656;

typedef struct {
	u8                       command_task_priority;
	u8                       command_task_stack_size;
	u8                       command_task_queue_size;
	u8                       hdcp_task_priority;
	u8                       hdcp_task_stack_size;
	u8                       i2c_address;
	pbsl_sys_func_t       i2c_read_function;
	pbsl_sys_func_t       i2c_write_function;
	pbsl_sys_func_edid_t   edid_read_function;
	ptx_cfg_resolution_t  p_resolution_info;
	u8                       resolution_nb;
	u8                       *p_mirror_table_ccir656;
	u8                       *p_swap_table_ccir656;
	u8                       *p_enable_video_port_ccir656;
	u8                       *p_ground_video_port_ccir656;
	u8                       *p_mirror_table_yuv422;
	u8                       *p_swap_table_yuv422;
	u8                       *p_enable_video_port_yuv422;
	u8                       *p_ground_video_port_yuv422;
	u8                       *p_mirror_table_yuv444;
	u8                       *p_swap_table_yuv444;
	u8                       *p_enable_video_port_yuv444;
	u8                       *p_ground_video_port_yuv444;
	u8                       *p_mirror_table_rgb444;
	u8                       *p_swap_table_rgb444;
	u8                       *p_enable_video_port_rgb444;
	u8                       *p_ground_video_port_rgb444;
	u8                       *p_enable_audio_port_spdif;
	u8                       *p_ground_audio_port_spdif;
	u8                       *p_enable_audio_clock_port_spdif;
	u8                       *p_ground_audio_clock_port_spdif;
	u8                       *p_enable_audio_port_i2s;
	u8                       *p_ground_audio_port_i2s;
	u8                       *p_enable_audio_port_i2s8c;
	u8                       *p_ground_audio_port_i2s8c;
	u8                       *p_enable_audio_clock_port_i2s;
	u8                       *p_ground_audio_clock_port_i2s;
	u8                       *p_enable_audio_port_oba;
	u8                       *p_ground_audio_port_oba;
	u8                       *p_enable_audio_clock_port_oba;
	u8                       *p_ground_audio_clock_port_oba;
	u8                       *p_enable_audio_port_dst;
	u8                       *p_ground_audio_port_dst;
	u8                       *p_enable_audio_clock_port_dst;
	u8                       *p_ground_audio_clock_port_dst;
	u8                       *p_enable_audio_port_hbr;
	u8                       *p_ground_audio_port_hbr;
	u8                       *p_enable_audio_clock_port_hbr;
	u8                       *p_ground_audio_clock_port_hbr;
	u16                      key_seed;
	tx_test_pattern_t     pattern;
	/* 0 DE is NOT available, 1 DE is there */
	u8                       data_enable_signal_available;
} tx_driver_config_table_t;

/*============================================================================*/
/*                       FUNCTIONS DECLARATIONS                               */
/*============================================================================*/

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
);

#ifdef TMFL_CEC_AVAILABLE

#include "tmdlHdmiCEC_Types.h"

typedef struct {
	u8                         command_task_priority;
	u8                         command_task_stack_size;
	u8                         command_task_queue_size;
	u8                         i2c_address;
	ptmdl_hdmi_cec_sys_func_t         i2c_read_function;
	ptmdl_hdmi_cec_sys_func_t         i2c_write_function;
	dl_hdmi_cec_capabilities_t     *p_capabilities_list;
} dl_hdmi_cec_driver_config_table_t;

error_code_t dl_hdmi_cec_cfg_get_config
(
    unit_select_t                 unit,
    dl_hdmi_cec_driver_config_table_t *p_config
);
#endif

#endif /* DLHDMITX_CFG_H */

/*============================================================================*/
/*                               END OF FILE                                  */
/*============================================================================*/
