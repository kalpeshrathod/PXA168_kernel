/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx.c
 *
 * \version       Revision: 1
 *
 * \date          Date: 10/08/07 10:00
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
 *   History:       tx.c
 *
 * *****************  Version 1  *****************
 * User: J. Lamotte Date: 10/08/07   Time: 10:00
 * Updated in $/Source/tx/inc
 * initial version
 *
 *   \endverbatim
 *
 * */

/*============================================================================*/
/*                             INCLUDE FILES                                  */
/*============================================================================*/
#include "tmdlHdmiTx_IW.h"
#include "tmdlHdmiTx.h"
#include "tmdlHdmiTx_local.h"
#include "tmbslHdmiTx.h"
#include "tmdlHdmiTx_cfg.h"

/*============================================================================*/
/*                          TYPES DECLARATIONS                                */
/*============================================================================*/

/* Macro to avoid compilation warnings */
#ifdef TMFL_OS_WINDOWS
#define DUMMY_ACCESS(x) x
#else
#define DUMMY_ACCESS(x)
#endif

/*============================================================================*/
/*                       CONSTANTS DECLARATIONS                               */
/*============================================================================*/

/*============================================================================*/
/*                         FUNCTION PROTOTYPES                                */
/*============================================================================*/

extern error_code_t bsl_debug_write_fake_reg_page(unit_select_t tx_unit);

/* Prototypes of internal functions */
/* Task functions */
#ifndef TMFL_NO_RTOS
static void command_task_unit0(void);
static void hdcp_task_unit0(void);
#endif /* TMFL_NO_RTOS */

/* Interrupt callback functions */
static void dl_hdmi_tx_handle_encrypt(instance_t instance);
static void dl_hdmi_tx_handle_hpd(instance_t instance);
static void dl_hdmi_tx_handle_t0(instance_t instance);
static void dl_hdmi_tx_handle_bcaps(instance_t instance);
static void dl_hdmi_tx_handle_bstatus(instance_t instance);
static void dl_hdmi_tx_handle_sha_1(instance_t instance);
static void dl_hdmi_tx_handle_pj(instance_t instance);
#ifdef TMFL_TDA9981_SUPPORT
static void dl_hdmi_tx_handle_r0(instance_t instance);
static void dl_hdmi_tx_handle_sw_int(instance_t instance);
#endif
#ifdef TMFL_RX_SENSE_ON
static void dl_hdmi_tx_handle_rx_sense(instance_t instance);
#endif
static void dl_hdmi_tx_handle_edid_read(instance_t instance);
#if 0
static void dl_hdmi_tx_handle_vs_rpt(instance_t instance);
#endif

/* Devlib internal color bar management functions */
static void dl_hdmi_tx_check_color_bar(instance_t instance);
static void dl_hdmi_tx_check_hdcp_color_bar(instance_t instance);

/* Set the state machine of device library */
static void dl_hdmi_tx_set_state
(
    instance_t instance,
    tx_driver_state_t state
);

/* Get the event status (enable or disable) in order to known
 *   if event should be signaled */
static tx_event_status_t dl_hdmi_tx_get_event_status
(
    instance_t        instance,
    tx_event_t   event
);

/* Use by txSetInputOutput in scaler mode */
static bool dl_hdmi_tx_get_refline_refpix
(
    tx_vid_fmt_t      vin_fmt,
    tx_vin_mode_t     vin_mode,
    tx_vid_fmt_t      vout_fmt,
    u8                   sync_in,
    tx_pix_rate_t     pix_rate,
    u16                  *p_ref_pix,
    u16                  *p_ref_line,
    u16                  *p_sc_ref_pix,
    u16                  *p_sc_ref_line,
    bool                    *pb_verified
);

/* Use by txSetInputOutput to set AVI infoframe */
static error_code_t dl_hdmi_tx_set_video_infoframe
(
    instance_t            instance,
    tx_vid_fmt_t      vout_fmt,
    tx_vout_mode_t    vout_mode
);

/* Get DTD from BSL */
static error_code_t dl_hdmi_tx_edid_get_dtd
(
    instance_t                    instance,
    tx_edid_video_timings_t    *p_dtdescriptors,
    u8                           max_dtdesc,
    u8                           *p_written_dtdesc
);

/* Convert DTD to CEA */
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea
(
    tx_edid_video_timings_t    *p_dtdescriptors
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_640hap
(
    tx_edid_video_timings_t    *p_dtdescriptors
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_720hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1280hap
(
    tx_edid_video_timings_t    *p_dtdescriptors
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1920hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    bool                            format_interlaced
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1440hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio,
    bool                            format_interlaced
);

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_2880hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio,
    bool                            format_interlaced
);

static tx_pict_aspect_ratio_t dl_hdmi_tx_calc_aspect_ratio(
    u16    himage_size,
    u16    vimage_size
);

/* IMPORTANT: The 3 functions define below should not be declared in static
 *   in order to allow applicative API to call them. Those functions are not
 *   in tx_Functions.h but are in txCore.def */

/* Get the device library state */
tx_driver_state_t dl_hdmi_tx_get_state(instance_t instance);

/* Set pattern ON (Blue screen or color bar) */
error_code_t dl_hdmi_tx_set_test_pattern_on
(
    instance_t            instance,
    tx_vid_fmt_t		vout_fmt,
    tx_vout_mode_t    vout_mode,
    tx_test_pattern_t pattern
);

/* Set pattern OFF */
error_code_t dl_hdmi_tx_set_test_pattern_off
(
    instance_t            instance,
    tx_vid_fmt_t      vout_fmt,
    tx_vout_mode_t    vout_mode
);

static void dl_hdmi_tx_check_hdcp_bksv

(
    instance_t    instance,
    u8          *p_hdcp_bksv_tested,
    bool           *pb_bksv_secure,
    bool            b_big_endian
);

/*============================================================================*/
/*                       VARIABLES DECLARATIONS                               */
/*============================================================================*/

tx_iwsem_handle_t dl_hdmi_tx_it_semaphore[MAX_UNITS];

/* Unit configuration structure (device library system configuration) */
unit_config_t unit_table_tx[MAX_UNITS] = {
	{
		false,
		false,
		(tx_hdcp_options_t)HDCP_OPT_DEFAULT,
		false,
		false,
		DL_HDMITX_DEVICE_UNKNOWN,
		0,
		0,
		(tx_iwtask_handle_t)0,
		(tx_iwqueue_handle_t)0,
		(tx_iwtask_handle_t)0,
		STATE_NOT_INITIALIZED,
		(ptx_callback_t)0,
		{NULL, 0}
	}
};

#ifndef TMFL_NO_RTOS

tx_iwfunc_ptr_t command_task_table_tx[MAX_UNITS] = {
	command_task_unit0
};

tx_iwfunc_ptr_t hdcp_task_table_tx[MAX_UNITS] = {
	hdcp_task_unit0
};

#endif /* TMFL_NO_RTOS */

bsl_callback_list_t callback_func_table_tx;

/* Device library configuration structure completed by txCfgGetConfig with
 *   informations contained in config file */
tx_driver_config_table_t gtx_driver_config_table[MAX_UNITS];

/* Video info (see instanceStatusInfoTx) */
tx_video_info_t video_info_list_tx = {
	false,
	{dl_hdmitx_vfmt_03_720x480p_60hz, DL_HDMITX_VINMODE_YUV422, DL_HDMITX_SYNCSRC_EXT_VS, DL_HDMITX_PIXRATE_SINGLE},
	{dl_hdmitx_vfmt_03_720x480p_60hz, DL_HDMITX_VOUTMODE_YUV422, DL_HDMITX_COLORDEPTH_24}
};

/* Audio info (see instanceStatusInfoTx) */
tx_audio_info_t audio_info_list_tx = {
	false,
	{
		DL_HDMITX_AFMT_SPDIF, DL_HDMITX_AFS_48K,
		DL_HDMITX_I2SFOR_PHILIPS_L, DL_HDMITX_I2SQ_16BITS, DL_HDMITX_DSTRATE_SINGLE, 0x00
	}
};

/* Event state (see instanceStatusInfoTx) */
tx_event_state_t event_state_list_tx[EVENT_NB] = {
	{DL_HDMITX_HDCP_ACTIVE,           DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_HDCP_INACTIVE,         DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_HPD_ACTIVE,            DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_HPD_INACTIVE,          DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_RX_KEYS_RECEIVED,      DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_RX_DEVICE_ACTIVE,      DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_RX_DEVICE_INACTIVE,    DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_EDID_RECEIVED,         DL_HDMITX_EVENT_DISABLED},
	{DL_HDMITX_VS_RPT_RECEIVED,       DL_HDMITX_EVENT_DISABLED}
};

/* Color bars state (see instanceStatusInfoTx) */
tx_col_bar_state_t color_bar_state_tx = {
	false,
	true,
	true,
	false,
	false,
	true,
	false
};

/* Instance status (save the actual configuration) */
instance_status_t instance_status_info_tx[MAX_UNITS] = {
	{
		(ptx_video_info_t)  &video_info_list_tx,
		(ptx_audio_info_t)  &audio_info_list_tx,
		(ptx_event_state_t) event_state_list_tx,
		(ptx_col_bar_state_t) &color_bar_state_tx,
		0
	}
	/*     &videoInfoListTx, */
	/*     &audioInfoListTx, */
	/*     eventStateListTx, */
	/*     &colorBarStateTx, */
	/*     0 */
};

/* HDCP seed table, arranged as pairs of 16-bit integers: lookup value, seed value.
 * If no table is programmed and if KEY_SEED in config file is null, HDCP will be disabled */
#define SEED_TABLE_LEN 10
static const u16 k_seed_table[SEED_TABLE_LEN][2] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

/* EDID status */
u8 edid_data_status = HDMITX_EDID_NOT_READ;

/* Flag set in case HDP ACTIVE is detected during initialization */
bool event_hpdactive_flag = false;
bool event_call_hpdactive_cback    = false;
bool event_call_rxsens_active_cback = false;

/* To remove the warnings */
u8 g_no_warning;

/* For debug purpose only, used to manage underlying I2C accessed */
static bool g_i2cdebug_accesses_enabled = true;

static u8 g_ksv_secure_timer_counter = 0;

/*============================================================================*/
/*                              FUNCTIONS                                     */
/*============================================================================*/

/******************************************************************************
 *    \brief Get the software version of the driver.
 *
 *    \param pSWVersion Pointer to the version structure.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_get_swversion
(
    swversion_t   *p_swversion
)
{
	/* Check if SWVersion pointer is NULL */
	RETIF(p_swversion == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Copy SW version */
	p_swversion->compatibility_nr = VERSION_COMPATIBILITY;
	p_swversion->major_version_nr = VERSION_MAJOR;
	p_swversion->minor_version_nr = VERSION_MINOR;

	return 0;
}

/******************************************************************************
 *    \brief Get the number of available HDMI transmitters devices in the system.
 *           A unit directly represents a physical device.
 *
 *    \param pUnitCount Pointer to the number of available units.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_get_number_of_units
(
    u32  *p_unit_count
)
{
	/* Check if UnitCount pointer is NULL */
	RETIF(p_unit_count == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Copy the maximum number of units */
	*p_unit_count = MAX_UNITS;

	return 0;
}

/******************************************************************************
 *    \brief Get the capabilities of unit 0. Capabilities are stored into a
 *           dedicated structure and are directly read from the HW device.
 *
 *    \param pCapabilities Pointer to the capabilities structure.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_capabilities
(
    tx_capabilities_t    *p_capabilities
)
{
	/* Directly call GetCapabilitiesM function for unit 0 and return the result */
	return(tx_get_capabilities_m((unit_select_t)0, p_capabilities));
}

/******************************************************************************
 *    \brief Get the capabilities of a specific unit. Capabilities are stored
 *           into a dedicated structure and are directly read from the HW
 *           device.
 *
 *    \param unit          Unit to be probed.
 *    \param pCapabilities Pointer to the capabilities structure.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_capabilities_m
(
    unit_select_t           unit,
    tx_capabilities_t *p_capabilities
)
{
	error_code_t   err = 0;
	bsl_hw_feature_t  bsl_device_capabilities;

	/* Check if unit number is in range */
	RETIF((unit < 0) || (unit >= MAX_UNITS), ERR_DLHDMITX_BAD_UNIT_NUMBER)

	/* Check if Capalities pointer is NULL */
	RETIF(p_capabilities == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Device version */
	p_capabilities->device_version = unit_table_tx[unit].device_version ;

	/* Retrieve the capabilities from the BSL layer */
	RETIF((err = bsl_hw_get_capabilities(unit, &bsl_device_capabilities)) != 0, err)

	switch(bsl_device_capabilities) {
	case bsl_hw_none:    /* None           feature */
		p_capabilities->hdcp = false;
		p_capabilities->scaler = false;
		p_capabilities->audio_packet.HBR = false;
		p_capabilities->audio_packet.one_bit_audio = true;
		p_capabilities->audio_packet.DST = false;
		p_capabilities->hdmi_version = dl_hdmitx_hdmi_version_1_2a;
		p_capabilities->color_depth = DL_HDMITX_COLORDEPTH_24;
		break;

	case bsl_hw_hdcp:    /* HDCP           feature */
		p_capabilities->hdcp = true;
		p_capabilities->scaler = false;
		p_capabilities->audio_packet.HBR = false;
		p_capabilities->audio_packet.one_bit_audio = true;
		p_capabilities->audio_packet.DST = false;
		p_capabilities->hdmi_version = dl_hdmitx_hdmi_version_1_2a;
		p_capabilities->color_depth = DL_HDMITX_COLORDEPTH_24;
		break;

	case bsl_hw_scaler:  /* Scaler         feature */
		p_capabilities->hdcp = false;
		p_capabilities->scaler = true;
		p_capabilities->audio_packet.HBR = false;
		p_capabilities->audio_packet.one_bit_audio = true;
		p_capabilities->audio_packet.DST = false;
		p_capabilities->hdmi_version = dl_hdmitx_hdmi_version_1_2a;
		p_capabilities->color_depth = DL_HDMITX_COLORDEPTH_24;
		break;

	case bsl_hw_hdcpscaler:   /* HDCP & Scaler  feature  */
		p_capabilities->hdcp = true;
		p_capabilities->scaler = true;
		p_capabilities->audio_packet.HBR = false;
		p_capabilities->audio_packet.one_bit_audio = true;
		p_capabilities->audio_packet.DST = false;
		p_capabilities->hdmi_version = dl_hdmitx_hdmi_version_1_2a;
		p_capabilities->color_depth = DL_HDMITX_COLORDEPTH_24;
		break;
	}

	return err;
}

/******************************************************************************
 *    \brief Open unit 0 of HdmiTx driver and provides the instance number to
 *           the caller. Note that one unit of HdmiTx represents one physical
 *           HDMI transmitter and that only one instance per unit can be opened.
 *
 *    \param pInstance Pointer to the variable that will receive the instance
 *                     identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the transmitter instance is not initialised
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_RESOURCE_OWNED: the resource is already in use
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_DLHDMITX_INIT_FAILED: the unit instance is already
 *              initialised or something wrong happened at lower level.
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_NOT_INITIALIZED: the unit is not initialized
 *            - ERR_HDMI_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_HDMI_INIT_FAILED: the unit instance is already
 *              initialised
 *            - ERR_HDMI_COMPATIBILITY: the driver is not compatiable
 *              with the internal device version code
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *
 ******************************************************************************/
error_code_t tx_open
(
    instance_t   *p_instance
)
{
	/* Directly call OpenM function for unit 0 and return the result */
	return(tx_open_m(p_instance, (unit_select_t)0));
}

/******************************************************************************
 *    \brief Open a specific unit of HdmiTx driver and provides the instance
 *           number to the caller. Note that one unit of HdmiTx represents one
 *           physical HDMI transmitter and that only one instance per unit can be
 *           opened. This function switches driver's state machine to
 *           "initialized" state.
 *
 *    \param pInstance Pointer to the structure that will receive the instance
 *                     identifier.
 *    \param unit      Unit number to be opened.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the transmitter instance is not initialised
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_RESOURCE_OWNED: the resource is already in use
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_DLHDMITX_INIT_FAILED: the unit instance is already
 *              initialised or something wrong happened at lower level.
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_NOT_INITIALIZED: the unit is not initialized
 *            - ERR_HDMI_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_HDMI_INIT_FAILED: the unit instance is already
 *              initialised
 *            - ERR_HDMI_COMPATIBILITY: the driver is not compatiable
 *              with the internal device version code
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *
 ******************************************************************************/
error_code_t tx_open_m
(
    instance_t   *p_instance,
    unit_select_t  unit
)
{
	error_code_t           err;
	u16                  i;
	u8                   device_version;
	bsl_hw_feature_t  feature_supported;

	/* Check if unit number is in range */
	RETIF((unit < 0) || (unit >= MAX_UNITS), ERR_DLHDMITX_BAD_UNIT_NUMBER)

	/* Check if Instance pointer is NULL */
	RETIF(p_instance == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Create the semaphore to protect variables modified under interruption */
	RETIF((err = tx_iwsemaphore_create(&dl_hdmi_tx_it_semaphore[unit])) != 0, err)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[unit])) != 0, err)

	/* Check if unit is already instanciated */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          unit_table_tx[unit].opened == true, ERR_DLHDMITX_RESOURCE_OWNED)

	/* Check the state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          dl_hdmi_tx_get_state(unit) != STATE_NOT_INITIALIZED, ERR_DLHDMITX_INVALID_STATE)

	/* Instanciate unit and return corresponding instance number */
	/* Since HW unit are only instanciable once, instance = unit */
	unit_table_tx[unit].opened          = true;
	unit_table_tx[unit].hdcp_enable      = false;
	unit_table_tx[unit].repeater_enable  = false;
	unit_table_tx[unit].device_version   = DL_HDMITX_DEVICE_UNKNOWN;
	unit_table_tx[unit].simplay_hd       = false;
	unit_table_tx[unit].p_callback       = NULL;
	unit_table_tx[unit].revocation_list.p_list  = NULL;

	unit_table_tx[unit].revocation_list.length = 0;

	/* Recover the configuration of the device library */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_cfg_get_config(unit, &gtx_driver_config_table[unit]) != 0, ERR_DLHDMITX_INIT_FAILED)

#ifndef TMFL_NO_RTOS
	/* Create message queue associated to this instance/unit */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_iwqueue_create(gtx_driver_config_table[unit].command_task_queue_size,
	                            &(unit_table_tx[unit].queue_handle)) != 0, ERR_DLHDMITX_NO_RESOURCES)

	/* Create the command task associated to this instance/unit */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_iwtask_create(command_task_table_tx[unit],
	                           gtx_driver_config_table[unit].command_task_priority,
	                           gtx_driver_config_table[unit].command_task_stack_size,
	                           &(unit_table_tx[unit].command_task_handle)) != 0, ERR_DLHDMITX_NO_RESOURCES)

	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_iwtask_start(unit_table_tx[unit].command_task_handle) != 0, ERR_DLHDMITX_NO_RESOURCES)

	/* Create the hdcp check task associated to this instance/unit */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_iwtask_create(hdcp_task_table_tx[unit],
	                           gtx_driver_config_table[unit].hdcp_task_priority,
	                           gtx_driver_config_table[unit].hdcp_task_stack_size,
	                           &(unit_table_tx[unit].hdcp_task_handle)) != 0, ERR_DLHDMITX_NO_RESOURCES)

#endif /* TMFL_NO_RTOS */

	*p_instance = (instance_t)unit;

	/* Init the BSL */
	/* Make sure all events are disabled */
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_HDCP_ACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_HDCP_INACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_HPD_ACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_HPD_INACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_RX_KEYS_RECEIVED].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_RX_DEVICE_ACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_RX_DEVICE_INACTIVE].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_EDID_RECEIVED].status = DL_HDMITX_EVENT_DISABLED;
	instance_status_info_tx[unit].p_event_state[DL_HDMITX_VS_RPT_RECEIVED].status = DL_HDMITX_EVENT_DISABLED;

	instance_status_info_tx[unit].p_col_bar_state->disable_color_bar_on_r0 = false;
	instance_status_info_tx[unit].p_col_bar_state->hdcp_colbar_change = false;
	instance_status_info_tx[unit].p_col_bar_state->hdcp_encrypt_or_t0 = true;
	instance_status_info_tx[unit].p_col_bar_state->hdcp_secure_or_t0 = false;
	instance_status_info_tx[unit].p_col_bar_state->in_out_first_set_done = false;
	instance_status_info_tx[unit].p_col_bar_state->color_bar_on = true;
	instance_status_info_tx[unit].p_col_bar_state->change_color_bar_now = true;

	/* use buffer 0 by default */
	instance_status_info_tx[unit].gamut_buf_num = 0;

	/* The funcCallback is not the same between BSL, so fill it dynamically */
	for(i = 0; i < HDMITX_CALLBACK_INT_NUM; i++) {
		callback_func_table_tx.func_callback[i] = NULL;
	}
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_ENCRYPT] = dl_hdmi_tx_handle_encrypt;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_HPD] = dl_hdmi_tx_handle_hpd;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_T0] = dl_hdmi_tx_handle_t0;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_BCAPS] = dl_hdmi_tx_handle_bcaps;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_BSTATUS] = dl_hdmi_tx_handle_bstatus;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_SHA_1] = dl_hdmi_tx_handle_sha_1;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_PJ] = dl_hdmi_tx_handle_pj;

#ifdef TMFL_TDA9981_SUPPORT
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_R0] = dl_hdmi_tx_handle_r0;
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_SW_INT] = dl_hdmi_tx_handle_sw_int;
#endif

#ifdef TMFL_RX_SENSE_ON
	callback_func_table_tx.func_callback[HDMITX_CALLBACK_INT_RX_SENSE] = dl_hdmi_tx_handle_rx_sense;
#endif

	/* Prepare static TDA9984 driver data as the compiler doesn't seem to */
	bsl_hw_startup();

	err = bsl_init(*p_instance,
	               gtx_driver_config_table[unit].i2c_address,
	               gtx_driver_config_table[unit].i2c_write_function,
	               gtx_driver_config_table[unit].i2c_read_function,
	               /* required for TDA9983 */
	               (pbsl_sys_func_edid_t)gtx_driver_config_table[unit].edid_read_function,
	               (pbsl_sys_func_timer_t)tx_iwwait,
	               &callback_func_table_tx,
	               false, /* Alternate EDID address not used */
	               (bsl_vid_fmt_t)instance_status_info_tx[unit].p_video_info->video_in_config.format,
	               (bsl_pix_rate_t)instance_status_info_tx[unit].p_video_info->video_in_config.pixel_rate);

	if(err != 0) {
		/* Init failed */
		bsl_deinit(unit);

		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[unit])) != 0, err)

		return err;
	} else {
		/* Init passed, continue */
		/* Start by forcing the TMDS ouputs off */
		err = bsl_tmds_set_outputs(unit,
		                           HDMITX_TMDSOUT_FORCED0);
		RETIF_SEM(dl_hdmi_tx_it_semaphore[unit], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
		          (err = bsl_hw_get_capabilities(unit, &feature_supported)) != 0, err)

		/* Retrieve the hardware device version from the BSL layer */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
		          (err = bsl_hw_get_version(unit,
		                                    &device_version)) != 0, err)

		/* Store the hardware device version in the global variable */

		switch(device_version) {
		case BSLHDMITX_TDA9981:
			unit_table_tx[unit].device_version = DL_HDMITX_DEVICE_TDA9981;
			break;

		case BSLHDMITX_TDA9983:
			unit_table_tx[unit].device_version = DL_HDMITX_DEVICE_TDA9983;
			break;

		default:
			unit_table_tx[unit].device_version = DL_HDMITX_DEVICE_UNKNOWN;
			break;
		}

	}

#ifndef TMFL_NO_RTOS
	/* Start HDCP check task */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[unit],
	          tx_iwtask_start(unit_table_tx[unit].hdcp_task_handle) != 0, ERR_DLHDMITX_NO_RESOURCES)

#endif /* TMFL_NO_RTOS */

	/* Set the state machine to initialized */
	dl_hdmi_tx_set_state(unit, STATE_INITIALIZED);

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[unit])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Close an instance of HdmiTx driver.
 *
 *    \param instance Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_close
(
    instance_t    instance
)
{
	error_code_t   err = 0;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check if unit corresponding to instance is opened */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          unit_table_tx[instance].opened == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	/* Close instance */
	unit_table_tx[instance].opened = false;

	/* Set the state machine */
	dl_hdmi_tx_set_state(instance, STATE_NOT_INITIALIZED);

	/* Destroy resources allocated for this instance/unit */

#ifndef TMFL_NO_RTOS
	tx_iwtask_destroy(unit_table_tx[instance].hdcp_task_handle);
	tx_iwtask_destroy(unit_table_tx[instance].command_task_handle);
	tx_iwqueue_destroy(unit_table_tx[instance].queue_handle);

#endif /* TMFL_NO_RTOS */

	/* Reset an instance of an HDMI transmitter */
	bsl_deinit(instance);

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Close the handle to the semaphore */
	RETIF((err == tx_iwsemaphore_destroy(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Set the power state of an instance of the HDMI transmitter.
 *
 *    \param instance   Instance identifier.
 *    \param powerState Power state to set.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *
 ******************************************************************************/
error_code_t tx_set_power_state
(
    instance_t    instance,
    power_state_t  power_state
)
{
	error_code_t           err;
	bsl_hot_plug_t    hpd_status;  /* HPD status */

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(unit_table_tx[instance].device_version == DL_HDMITX_DEVICE_TDA9984) {
		if(power_state == power_suspend) {
			return ERR_DLHDMITX_NOT_SUPPORTED;
		}
	}

	/* Switch off HDCP */
	if((power_state == power_off) && (unit_table_tx[instance].hdcp_enable == true)) {
		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)
		/* Switch off HDCP */
		RETIF((err = tx_set_hdcp(instance, false)) != 0, err)
		/* Take the sempahore */
		RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)
	}

	/* CBE */
	if(power_state == power_off) {
		dl_hdmi_tx_set_state(instance, STATE_INITIALIZED);
	}

	/* Set the power state of the transmitter */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_power_set_state(instance,
	                                     power_state)) != 0, err)

	/* Get Hot Plug status */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_hot_plug_get_status(instance,
	                  &hpd_status)) != 0, err)

	if(power_state == power_on) {
		if((hpd_status == HDMITX_HOTPLUG_ACTIVE) && (dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE)) {
			/* Yes: Wait for DDC line to settle before reading EDID */
			bsl_sys_timer_wait(instance, 500); /* ms */

			/* EDID read */

			RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
			          (err = bsl_edid_get_block_data(instance,
			                  unit_table_tx[instance].p_edid_buffer, (u32)((unit_table_tx[instance].edid_buffer_size) >> 7),
			                  unit_table_tx[instance].edid_buffer_size, &edid_data_status)) != 0, err)

			dl_hdmi_tx_handle_edid_read(instance);
		}
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Get the power state of an instance of the HDMI transmitter.
 *
 *    \param instance    Instance identifier.
 *    \param pPowerState Pointer to the power state.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_power_state
(
    instance_t    instance,
    power_state_t  *p_power_state
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if PowerState pointer is NULL */
	RETIF(p_power_state == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Get the power state of the transmitter */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_power_get_state(instance,
	                                     p_power_state)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Set the configuration of instance attributes. This function is
 *           required by DVP architecture rules but actually does nothing in this
 *           driver.
 *
 *    \param instance    Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *
 ******************************************************************************/
error_code_t tx_instance_config
(
    instance_t    instance
)
{
	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	return 0;
}

/******************************************************************************
 *    \brief Setup the instance with its configuration parameters. This function
 *           allows basic instance configuration like enabling HDCP, choosing
 *           HDCP encryption mode or enabling HDCP repeater mode.
 *
 *    \param instance   Instance identifier.
 *    \param pSetupInfo Pointer to the structure containing all setup parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *
 ******************************************************************************/
error_code_t tx_instance_setup
(
    instance_t                    instance,
    tx_instance_setup_info_t   *p_setup_info
)
{
	error_code_t   err;

	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if SetupInfo pointer is NULL */
	RETIF(p_setup_info == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check if unit corresponding to instance is opened */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          unit_table_tx[instance].opened == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	/* Check the state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_INITIALIZED, ERR_DLHDMITX_INVALID_STATE)

	unit_table_tx[instance].repeater_enable = p_setup_info->repeater_enable;
	unit_table_tx[instance].simplay_hd = p_setup_info->simplay_hd;
	unit_table_tx[instance].p_edid_buffer = p_setup_info->p_edid_buffer;
	unit_table_tx[instance].edid_buffer_size = p_setup_info->edid_buffer_size;

	/* Set state machine to Unplugged */
	dl_hdmi_tx_set_state(instance, STATE_UNPLUGGED);

	if(event_hpdactive_flag == true) {
		/* if the HPD active was rased during the initialisation, raise the event HPD */
		dl_hdmi_tx_handle_hpd(instance);
		event_hpdactive_flag = false;
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Get instance setup parameters.
 *
 *    \param instance   Instance identifier.
 *    \param pSetupInfo Pointer to the structure that will receive setup
 *                      parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 ******************************************************************************/
error_code_t tx_get_instance_setup
(
    instance_t                    instance,
    tx_instance_setup_info_t   *p_setup_info
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if SetupInfo pointer is NULL */
	RETIF(p_setup_info == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check if unit corresponding to instance is opened */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          unit_table_tx[instance].opened == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	p_setup_info->simplay_hd          = unit_table_tx[instance].simplay_hd;
	p_setup_info->repeater_enable     = unit_table_tx[instance].repeater_enable;
	/* JL, TODO */

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Make device library handle an incoming interrupt. This function is
 *           used by application to tell the device library that the hardware
 *           sent an interrupt.
 *
 *    \param instance   Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_FULL: the queue is full
 *
 ******************************************************************************/
error_code_t tx_handle_interrupt
(
    instance_t    instance
)
{
#ifndef TMFL_NO_RTOS
	error_code_t err;
	u8         message = 0;
#endif /* TMFL_NO_RTOS */

	error_code_t err     = 0;

	(void)err;
	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

#ifndef TMFL_NO_RTOS
	RETIF((err = tx_iwqueue_send(unit_table_tx[instance].queue_handle, message)) != 0, err)

	/* Disable interrupts for Tx until the callbacks have been done by the command task */
	switch(instance) {
	case INSTANCE_0:
		tx_iwdisable_interrupts(DL_HDMI_IW_TX_1);
		break;
	case INSTANCE_1:
		tx_iwdisable_interrupts(DL_HDMI_IW_TX_2);
		break;
	default:
		return ERR_DLHDMITX_BAD_INSTANCE;
	}

#endif /* TMFL_NO_RTOS */

	return 0;
}

/******************************************************************************
 *    \brief Register event callbacks. Only one callback is registered through
 *           this API. This callback will received the type of event that
 *           occured throug a dedicated parameter and will be called as many
 *           times as there is pending events.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param instance   Instance identifier.
 *    \param pCallback  Pointer to the callback function that will handle events
 *                      from the devlib.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *
 ******************************************************************************/
error_code_t tx_register_callbacks
(
    instance_t            instance,
    ptx_callback_t   p_callback
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check if unit corresponding to instance is opened */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          unit_table_tx[instance].opened == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	/* Check if instance state is correct */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_INITIALIZED, ERR_DLHDMITX_INVALID_STATE)

	/* Store callback pointers */
	unit_table_tx[instance].p_callback = p_callback;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief This function allows enabling a specific event. By default, all
 *           events are disabled, except input lock.
 *
 *    \param instance Instance identifier.
 *    \param event    Event to enable.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_enable_event
(
    instance_t        instance,
    tx_event_t   event
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if the event exists */
	RETIF_BADPARAM(event >= EVENT_NB)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Protect the access to this ressource */
	instance_status_info_tx[instance].p_event_state[event].status = DL_HDMITX_EVENT_ENABLED;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	switch(event) {
	case DL_HDMITX_HPD_ACTIVE:
		if(event_call_hpdactive_cback) {
			event_call_hpdactive_cback = false;
			unit_table_tx[instance].p_callback(DL_HDMITX_HPD_ACTIVE);
		}
		break;

	case DL_HDMITX_RX_DEVICE_ACTIVE:
		if(event_call_rxsens_active_cback) {
			event_call_rxsens_active_cback = false;
			unit_table_tx[instance].p_callback(DL_HDMITX_RX_DEVICE_ACTIVE);
		}
		break;

	default:
		break;

	}

	return 0;
}

/******************************************************************************
 *    \brief This function allows disabling a specific event. By default, all
 *           events are disabled, except input lock.
 *
 *    \param instance Instance identifier.
 *    \param event    Event to disable.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_disable_event
(
    instance_t        instance,
    tx_event_t   event
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if the event exists */
	RETIF_BADPARAM(event >= EVENT_NB)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Protect the access to this ressource */
	instance_status_info_tx[instance].p_event_state[event].status = DL_HDMITX_EVENT_DISABLED;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Get specifications of a given video format. Application can use
 *           this function to retreives all specifications (frequencies,
 *           resolution, etc.) of a given IA/CEA 861-D video format.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param resolutionID     ID of the resolution to retrieve specs from.
 *    \param pResolutionSpecs Pointer to the structure receiving specs.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_RESOLUTION_UNKNOWN: the resolution is unknown
 *
 ******************************************************************************/
error_code_t tx_get_video_format_specs
(
    instance_t            instance,
    tx_vid_fmt_t      resolution_id,
    tx_vid_fmt_specs_t *p_resolution_specs
)
{
	u8	i;
	bool    find = false;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if ResolutionSpecs pointer is NULL */
	RETIF(p_resolution_specs == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	for(i = 0; i < gtx_driver_config_table[instance].resolution_nb; i++) {
		if(resolution_id == gtx_driver_config_table[instance].p_resolution_info[i].resolution_id) {
			find = true;
			p_resolution_specs->height      = gtx_driver_config_table[instance].p_resolution_info[i].height;
			p_resolution_specs->width       = gtx_driver_config_table[instance].p_resolution_info[i].width;
			p_resolution_specs->interlaced  = gtx_driver_config_table[instance].p_resolution_info[i].interlaced;
			p_resolution_specs->vfrequency  = gtx_driver_config_table[instance].p_resolution_info[i].vfrequency;
			p_resolution_specs->aspect_ratio = gtx_driver_config_table[instance].p_resolution_info[i].aspect_ratio;
			break;
		}
	}

	/* Resolution not found in table */
	RETIF(find == false, ERR_DLHDMITX_RESOLUTION_UNKNOWN)

	return 0;
}

/******************************************************************************
 *    \brief Configures all input and output parameters : format, modes, rates,
 *           etc. This is the main configuration function of the driver. Here
 *           are transmitted all crucial input and output parameters of the
 *           device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance          Instance identifier.
 *    \param videoInputConfig  Configuration of the input video.
 *    \param videoOutputConfig Configuration of the output video.
 *    \param audioInputConfig  Configuration of the input audio.
 *    \param sinkType          Type of sink connected to the output of the Tx.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_input_output
(
    instance_t                instance,
    tx_video_in_config_t   video_input_config,
    tx_video_out_config_t  video_output_config,
    tx_audio_in_config_t   audio_input_config,
    tx_sink_type_t        sink_type
)
{
	error_code_t           err;
	u8                   pix_repeat;                  /* Pixel repetition */
	bsl_vout_dbits_t  path_bits;                   /* Data path bit width */
	bsl_pix_edge_t    pixel_edge;                  /* Pixel sampling edge */
	bsl_vs_meth_t     sync_method;                 /* Sync method */
	bsl_pix_togl_t    toggle;                     /* Toggling */
	/* Embedded or external */
	u8                   sync_in;
	bsl_pix_subpkt_t  sp_sync;                     /* Subpacket sync */
	bsl_blnk_src_t    blankit;                    /* Blanking */
	/* HDMITX_PIXRATE_SINGLE */
	bsl_pix_rate_t    pix_rate_single_double;
	u16                  u_ref_pix;                    /* REFPIX for output */
	u16                  u_ref_line;                   /* REFLINE for output */
	u16                  u_sc_ref_pix;                  /* REFPIX for scaler */
	/* REFLINE for scaler */
	u16                  u_sc_ref_line;
	/* Scaler setting verified */
	bool                    b_verified;
	/* Adjustment for interlaced output */
	bsl_top_sel_t     top_sel;
	/* Line/pixel counters sync */
	bsl_vs_once_t     once;
	bsl_sca_mode_t    scaler_mode;                 /* Current scaler mode */

	tx_capabilities_t audio_capabilities;

	/* Initialized after (depend on video mode used) */
	u8                   *p_swap_table = NULL;
	/* Initialized after (depend on video mode used) */
	u8                   *p_mirror_table = NULL;
	/* Initialized after (depend on video mode used) */
	u8                   *p_ena_video_port_table = NULL;
	/* Initialized after (depend on video mode used) */
	u8                   *p_gnd_video_port_table = NULL;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Update the instance status information */
	instance_status_info_tx[instance].p_video_info->video_in_config.format       = video_input_config.format;
	instance_status_info_tx[instance].p_video_info->video_in_config.mode         = video_input_config.mode;
	instance_status_info_tx[instance].p_video_info->video_in_config.sync_source   = video_input_config.sync_source;
	instance_status_info_tx[instance].p_video_info->video_in_config.pixel_rate    = video_input_config.pixel_rate;

	instance_status_info_tx[instance].p_video_info->video_out_config.format      = video_output_config.format;
	instance_status_info_tx[instance].p_video_info->video_out_config.mode        = video_output_config.mode;
	instance_status_info_tx[instance].p_video_info->video_out_config.color_depth  = video_output_config.color_depth;

	/* TODO */
	/*instanceStatusInfoTx[instance].pVideoInfo->videoMuteState            = */

	/* Audio support */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = tx_get_capabilities_m(instance, &audio_capabilities)) != 0, err)

	/* Test if audio input format is supported */
	if(((audio_input_config.format == DL_HDMITX_AFMT_OBA) && (audio_capabilities.audio_packet.one_bit_audio == false)) ||
	   ((audio_input_config.format == DL_HDMITX_AFMT_DST) && (audio_capabilities.audio_packet.DST == false)) ||
	   ((audio_input_config.format == DL_HDMITX_AFMT_HBR) && (audio_capabilities.audio_packet.HBR == false))) {
		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

		return ERR_DLHDMITX_NOT_SUPPORTED;
	}

	instance_status_info_tx[instance].p_audio_info->audio_in_cfg.format            = audio_input_config.format;
	instance_status_info_tx[instance].p_audio_info->audio_in_cfg.i2s_format         = audio_input_config.i2s_format;
	instance_status_info_tx[instance].p_audio_info->audio_in_cfg.i2s_qualifier      = audio_input_config.i2s_qualifier;
	instance_status_info_tx[instance].p_audio_info->audio_in_cfg.rate              = audio_input_config.rate;
	instance_status_info_tx[instance].p_audio_info->audio_in_cfg.channel_allocation = audio_input_config.channel_allocation;

	/* TODO */
	/*instanceStatusInfoTx[instance].pAudioInfo->audioMuteState            = */

	if(sink_type == DL_HDMITX_SINK_EDID) {
		/* Change sink type with the currently defined in EDID */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_edid_get_sink_type(instance,
		                                        (bsl_sink_type_t *)&sink_type)) != 0, err)
	}

	if((sink_type == DL_HDMITX_SINK_DVI) &&
	   ((video_output_config.format ==  dl_hdmitx_vfmt_06_720x480i_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_07_720x480i_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_10_720x480i_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_11_720x480i_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_21_720x576i_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_22_720x576i_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_25_720x576i_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_26_720x576i_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_08_720x240p_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_09_720x240p_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_12_720x240p_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_13_720x240p_60hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_23_720x288p_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_24_720x288p_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_27_720x288p_50hz) ||
	    (video_output_config.format ==  dl_hdmitx_vfmt_28_720x288p_50hz)
	   )
	  )
		/* forbid format with pixel repetition in DVI */
	{
		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

		return ERR_DLHDMITX_BAD_PARAMETER;
	}

	/* Set the TMDS outputs to a forced state */
	err = bsl_tmds_set_outputs(instance,
	                           HDMITX_TMDSOUT_FORCED0);
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

	/* Fine-tune the TMDS serializer */
	err = bsl_tmds_set_serializer(instance,
	                              4, 8);
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

	/* Set video output configuration */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_video_out_set_config(instance,
	                  (bsl_sink_type_t)sink_type, (bsl_vout_mode_t)video_output_config.mode, HDMITX_VOUT_PREFIL_OFF,
	                  HDMITX_VOUT_YUV_BLNK_16, HDMITX_VOUT_QRANGE_FS)) != 0, err)

	/* Set default config */
	pix_repeat   = HDMITX_PIXREP_DEFAULT;
	path_bits    = HDMITX_VOUT_DBITS_12;
	pixel_edge   = HDMITX_PIXEDGE_CLK_POS;
	sync_method  = HDMITX_VSMETH_V_H;
	toggle      = HDMITX_PIXTOGL_ENABLE;

	/* Set sync details */
	if(video_input_config.sync_source == DL_HDMITX_SYNCSRC_EMBEDDED) {
		/* Embedded sync */
		sync_in      = EMB;
		sp_sync      = HDMITX_PIXSUBPKT_SYNC_HEMB;
		blankit     = HDMITX_BLNKSRC_VS_HEMB_VEMB;
		sync_method  = HDMITX_VSMETH_V_XDE;
	} else {
		/* External sync */
		sync_in  = EXT;

		if(gtx_driver_config_table[instance].data_enable_signal_available == 1) {
			/* DE is available */
			sp_sync  = HDMITX_PIXSUBPKT_SYNC_DE;
		} else {
			/* DE is NOT available */
			sp_sync  = HDMITX_PIXSUBPKT_SYNC_HS;
		}

		blankit = HDMITX_BLNKSRC_NOT_DE;
	}

	/* Port swap table */
	switch(video_input_config.mode) {
	case DL_HDMITX_VINMODE_CCIR656:
		path_bits = HDMITX_VOUT_DBITS_8;
		pixel_edge = HDMITX_PIXEDGE_CLK_NEG;
		p_swap_table = gtx_driver_config_table[instance].p_swap_table_ccir656;
		p_mirror_table = gtx_driver_config_table[instance].p_mirror_table_ccir656;
		p_ena_video_port_table = gtx_driver_config_table[instance].p_enable_video_port_ccir656;
		p_gnd_video_port_table = gtx_driver_config_table[instance].p_ground_video_port_ccir656;
		break;

	case DL_HDMITX_VINMODE_RGB444:
		p_swap_table = gtx_driver_config_table[instance].p_swap_table_rgb444;
		p_mirror_table = gtx_driver_config_table[instance].p_mirror_table_rgb444;
		p_ena_video_port_table = gtx_driver_config_table[instance].p_enable_video_port_rgb444;
		p_gnd_video_port_table = gtx_driver_config_table[instance].p_ground_video_port_rgb444;
		break;

	case DL_HDMITX_VINMODE_YUV444:
		p_swap_table = gtx_driver_config_table[instance].p_swap_table_yuv444;
		p_mirror_table = gtx_driver_config_table[instance].p_mirror_table_yuv444;
		p_ena_video_port_table = gtx_driver_config_table[instance].p_enable_video_port_yuv444;
		p_gnd_video_port_table = gtx_driver_config_table[instance].p_ground_video_port_yuv444;
		break;

	case DL_HDMITX_VINMODE_YUV422:
		p_swap_table = gtx_driver_config_table[instance].p_swap_table_yuv422;
		p_mirror_table = gtx_driver_config_table[instance].p_mirror_table_yuv422;
		p_ena_video_port_table = gtx_driver_config_table[instance].p_enable_video_port_yuv422;
		p_gnd_video_port_table = gtx_driver_config_table[instance].p_ground_video_port_yuv422;
		break;

	default:
		break;
	}

	/* Set the audio and video input port configuration */
	err = bsl_set_video_port_config(instance,
	                                p_ena_video_port_table, p_gnd_video_port_table);
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

	/* No possible to set the audio clock port config on TDA9983 */

	/* Video input mapping */
	err = bsl_video_in_set_mapping(instance,
	                               p_swap_table, p_mirror_table);
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

	/* Set fine image position */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err = bsl_video_in_set_fine(instance, sp_sync, HDMITX_PIXTOGL_NO_ACTION)) != 0, err)

	/* Set input blanking */
	err = bsl_video_in_set_blanking(instance, blankit, HDMITX_BLNKCODE_ALL_0);
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

	/* Configure video input options and control the upsampler */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_video_in_set_config(instance,
	                  /* (bslVidFmt_t)videoOutputConfig.format ,*/
	                  (bsl_vin_mode_t)video_input_config.mode,  pixel_edge,
	                  (bsl_pix_rate_t)video_input_config.pixel_rate, HDMITX_UPSAMPLE_AUTO)) != 0, err)

	/* Only set audio for HDMI, not DVI */
	if(sink_type == DL_HDMITX_SINK_HDMI) {
		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)
		/* Set audio parameters */
		RETIF((err = tx_set_audio_input(instance, audio_input_config, sink_type)) != 0, err)
		/* Take the sempahore */
		RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)
	}

	/* Output fine adjustment */
	pix_rate_single_double = (bsl_pix_rate_t)video_input_config.pixel_rate;
	if(video_input_config.pixel_rate == HDMITX_PIXRATE_SINGLE_REPEATED) {
		pix_rate_single_double = HDMITX_PIXRATE_SINGLE;
	}

	/* Set input ouput - may give NOT_SUPPORTED error */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_video_set_in_out(instance,
	                                      (bsl_vid_fmt_t)video_input_config.format, HDMITX_SCAMODE_AUTO, (bsl_vid_fmt_t)video_output_config.format,
	                                      pix_repeat, HDMITX_MATMODE_AUTO, path_bits, (bsl_vqr_t) video_output_config.dvi_vqr)) != 0, err)

	if(dl_hdmi_tx_get_refline_refpix(video_input_config.format, video_input_config.mode, video_output_config.format,
	                                 sync_in, (tx_pix_rate_t)pix_rate_single_double, &u_ref_pix, &u_ref_line,
	                                 &u_sc_ref_pix, &u_sc_ref_line, &b_verified) > 0) {
		/* From 720p50/60 or 1080i50/60 up-scaling to 1080p50/60, when external sync,
		 *		   toggleV, toggleH and toggleX need to be set to 0 */
		if(sync_in == EXT) {
			switch(video_input_config.format) {
			case hdmitx_vfmt_04_1280x720p_60hz:
			case hdmitx_vfmt_19_1280x720p_50hz:
			case hdmitx_vfmt_05_1920x1080i_60hz:
			case hdmitx_vfmt_20_1920x1080i_50hz:
				if((video_output_config.format == hdmitx_vfmt_16_1920x1080p_60hz)
				   || (video_output_config.format == hdmitx_vfmt_31_1920x1080p_50hz)) {
					toggle = HDMITX_PIXTOGL_NO_ACTION;
				}
				break;
			default:
				toggle = HDMITX_PIXTOGL_ENABLE;
				break;
			}
		}

		/* Combination found in table for scaler: configure input manually */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_video_in_set_sync_manual(instance,
		                  (bsl_sync_source_t)video_input_config.sync_source, sync_method, toggle, toggle, toggle, u_ref_pix, u_ref_line)) != 0, err)
	} else {
		/* Not found so assume non-scaler and auto-configure input */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_video_in_set_sync_auto(instance,
		                  (bsl_sync_source_t)video_input_config.sync_source, (bsl_vid_fmt_t)video_input_config.format,
		                  (bsl_vin_mode_t)video_input_config.mode)) != 0, err)
	}

	/* Only set infoframes for HDMI, not DVI */
	if(sink_type == DL_HDMITX_SINK_HDMI) {
		/* Set avi infoframe */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = dl_hdmi_tx_set_video_infoframe(instance, video_output_config.format, video_output_config.mode)) != 0,
		          err)
	}

	err = bsl_scaler_get_mode(instance, &scaler_mode);

	/* Ignore scaler ERR_HDMI_NOT_SUPPORTED error */
	if((err == 0) && (scaler_mode == HDMITX_SCAMODE_ON)) {
		/* Enable scaler mode */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_scaler_in_disable(instance,
		                                       false)) != 0, err)

		/* Correction to interlace */
		top_sel = HDMITX_TOPSEL_INTERNAL;
		if((video_output_config.format == dl_hdmitx_vfmt_05_1920x1080i_60hz)
		   || (video_output_config.format == dl_hdmitx_vfmt_20_1920x1080i_50hz)) {
			/* video input format is range-checked by bslVideoSetInOut above */
			if((k_vfmt_to_short_fmt_tv[video_input_config.format] == TV_480p_60HZ)
			   || (k_vfmt_to_short_fmt_tv[video_input_config.format] == TV_576p_50HZ)) {
				/* Correct for 1080i output for p->i conversion only */
				top_sel = HDMITX_TOPSEL_VRF;
			}
		}

		/* Set scaler field positions */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_scaler_set_field_order(instance,
		                  HDMITX_INTEXT_NO_CHANGE, HDMITX_INTEXT_NO_CHANGE, top_sel, HDMITX_TOPTGL_NO_CHANGE)) != 0, err)

		/* Scaler fine adjustment */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_scaler_set_fine(instance,
		                                     u_sc_ref_pix, u_sc_ref_line)) != 0, err)

		/* Not possible to set the scaler latency & phase on TDA9983 */

		/* Set scaler synchronisation option */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_scaler_set_sync(instance,
		                                     sync_method, HDMITX_VSONCE_EACH_FRAME)) != 0, err)

		/* With scaler, use Only Once setting for bslVideoOutSetSync */
		once = HDMITX_VSONCE_ONCE;
	} else {
		once = HDMITX_VSONCE_EACH_FRAME;
	}

	/* Set video synchronisation */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_video_out_set_sync(instance,
	                                        HDMITX_VSSRC_INTERNAL, HDMITX_VSSRC_INTERNAL, HDMITX_VSSRC_INTERNAL,
	                                        HDMITX_VSTGL_TABLE, once)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	instance_status_info_tx[instance].p_col_bar_state->in_out_first_set_done = true;

	/* Test if pattern is already on */
	if(instance_status_info_tx[instance].p_col_bar_state->color_bar_on == true) {
		/* If pattern is On, apply new settings */
		instance_status_info_tx[instance].p_col_bar_state->change_color_bar_now = true;
	}

	return 0;
}

/*****************************************************************************/
/**
 *    \brief Configures audio input parameters : format, rate, etc.
 *           This function is similar to txSetInputOutput except that
 *           video is not reconfigured.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance          Instance identifier.
 *    \param audioInputConfig  Configuration of the input audio.
 *    \param sinkType          Type of sink connected to the output of the Tx.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_audio_input
(
    instance_t                instance,
    tx_audio_in_config_t   audio_input_config,
    tx_sink_type_t        sink_type
)
{
	error_code_t           err;
	/* Used to convert video format to video frequency */
	tx_vid_fmt_specs_t resolution_specs;
	u8                   layout;                     /* 0 or 1 */
	/* audio info frame channels */
	u8                   aif_channel_count_code = 0;
	/* Vertical output frequency */
	bsl_vfreq_t      v_out_freq;
	bslcts_ref_t     cts_ref;                     /* CTS ref source */
	u16                  u_cts_x;                      /* CtsX value */
	bsl_pkt_aif_t     pkt_aif;                     /* Audio infoframe packet */
	tx_capabilities_t audio_capabilities;
	u8                   *p_ena_audio_port_cfg;
	u8                   *p_gnd_audio_port_cfg;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Audio support */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = tx_get_capabilities_m(instance, &audio_capabilities)) != 0, err)

	/* Test if audio input format is supported */
	if(((audio_input_config.format == DL_HDMITX_AFMT_OBA) && (audio_capabilities.audio_packet.one_bit_audio == false)) ||
	   ((audio_input_config.format == DL_HDMITX_AFMT_DST) && (audio_capabilities.audio_packet.DST == false)) ||
	   ((audio_input_config.format == DL_HDMITX_AFMT_HBR) && (audio_capabilities.audio_packet.HBR == false)))

	{
		/* Release the sempahore */
		RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

		return ERR_DLHDMITX_NOT_SUPPORTED;
	}

	if(sink_type == DL_HDMITX_SINK_EDID) {
		/* Change sink type with the currently defined in EDID */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_edid_get_sink_type(instance, (bsl_sink_type_t *)&sink_type)) != 0, err)
	}

	if(sink_type == DL_HDMITX_SINK_HDMI) {

		layout = 1;

		if(audio_input_config.channel_allocation == 0x00) {
			layout = 0;
		}

		aif_channel_count_code = k_chan_alloc_chan_num[audio_input_config.channel_allocation] - 1;

		/* Port audio configuration */
		switch(audio_input_config.format) {
		case DL_HDMITX_AFMT_SPDIF:
			p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_spdif;
			p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_spdif;
			break;

		case DL_HDMITX_AFMT_I2S:

			if(aif_channel_count_code >= 1) { /* For multi-channel */
				p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_i2s8c;
				p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_i2s8c;
			} else {
				p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_i2s;
				p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_i2s;
			}
			break;

		case DL_HDMITX_AFMT_OBA:
			p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_oba;
			p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_oba;
			break;

		case DL_HDMITX_AFMT_DST:
			p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_dst;
			p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_dst;
			break;

		case DL_HDMITX_AFMT_HBR:
			p_ena_audio_port_cfg = gtx_driver_config_table[instance].p_enable_audio_port_hbr;
			p_gnd_audio_port_cfg = gtx_driver_config_table[instance].p_ground_audio_port_hbr;
			break;

		default:
			/* Release the sempahore */
			RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

			return ERR_DLHDMITX_BAD_PARAMETER;
		}

		err = bsl_set_audio_port_config(instance, p_ena_audio_port_cfg, p_gnd_audio_port_cfg);
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], (err != 0) && (err != ERR_HDMI_NOT_SUPPORTED), err)

		/* Not possible to set the Clock port configuration on TDA9983 */

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_audio_in_set_config(instance, (bsla_fmt_t)audio_input_config.format,
		                  audio_input_config.channel_allocation, HDMITX_CHAN_NO_CHANGE, HDMITX_CLKPOLDSD_NO_CHANGE, HDMITX_SWAPDSD_NO_CHANGE,
		                  layout, 0x80)) != 0, err)

		/* Find output vertical frequency from output format */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = tx_get_video_format_specs(instance, instance_status_info_tx[instance].p_video_info->video_out_config.format,
		                  &resolution_specs)) != 0, err)
		v_out_freq = (bsl_vfreq_t)resolution_specs.vfrequency;

		if((audio_input_config.format == DL_HDMITX_AFMT_SPDIF)
		   || (audio_input_config.format == DL_HDMITX_AFMT_OBA)) {
			cts_ref = HDMITX_CTSREF_FS64SPDIF;
			u_cts_x = HDMITX_CTSX_64;
		} else { /* I2S */
			cts_ref = HDMITX_CTSREF_ACLK;
			if(audio_input_config.i2s_qualifier == DL_HDMITX_I2SQ_32BITS) {
				u_cts_x = HDMITX_CTSX_64;
			} else {
				u_cts_x = HDMITX_CTSX_32;
			}
		}

		/* Set the Clock Time Stamp generator in HDMI mode only */

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_audio_in_set_cts(instance, cts_ref, (bslafs_t)audio_input_config.rate,
		                                      (bsl_vid_fmt_t)instance_status_info_tx[instance].p_video_info->video_out_config.format,
		                                      v_out_freq, HDMITX_CTS_AUTO, u_cts_x, HDMITX_CTSK_USE_CTSX, HDMITX_CTSMTS_USE_CTSX)) != 0, err)

		/* Set Channel Status registers
		 *		    No need to call bslTDA9984AudioOutSetChanStatusMapping, since default Byte 2
		 *		    values of "Do not take into account" are adequate */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_audio_out_set_chan_status(instance, HDMITX_CSFI_PCM_2CHAN_NO_PRE,
		                  HDMITX_CSCOPYRIGHT_PROTECTED, 0x00, (bslafs_t)audio_input_config.rate,
		                  HDMITX_CSCLK_LEVEL_II, HDMITX_CSMAX_LENGTH_20, HDMITX_CSWORD_DEFAULT,
		                  HDMITX_CSOFREQ_NOT_INDICATED)) != 0, err)

		/* Set reset_fifo to 1 */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_audio_out_set_mute(instance, HDMITX_AMUTE_ON)) != 0, err)
		/* Wait for 20 ms */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = tx_iwwait(20)) != 0, err)
		/* Set reset_fifo to 0 */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_audio_out_set_mute(instance, HDMITX_AMUTE_OFF)) != 0, err)

		/* Set audio infoframe */
		pkt_aif.channel_count     = aif_channel_count_code;
		pkt_aif.coding_type       = 0; /* refer to stream header */
		pkt_aif.sample_size       = 0; /* refer to stream header */
		pkt_aif.channel_alloc     = audio_input_config.channel_allocation;
		pkt_aif.level_shift       = 0; /* 0dB level shift */
		pkt_aif.down_mix_inhibit   = 0; /* down-mix stereo permitted */
		/* refer to stream header */
		pkt_aif.sample_freq       = AIF_SF_REFER_TO_STREAM_HEADER;

		/* SampleFreq parameter need to be set for OBA and DST audio stream */
		if((audio_input_config.format == DL_HDMITX_AFMT_OBA) ||
		   (audio_input_config.format == DL_HDMITX_AFMT_DST)) {
			switch(audio_input_config.rate) {
			case DL_HDMITX_AFS_32K:
				pkt_aif.sample_freq = AIF_SF_32K; /* see table 18 of CEA-861 */
				break;
			case DL_HDMITX_AFS_44K:
				pkt_aif.sample_freq = AIF_SF_44K;
				break;
			case DL_HDMITX_AFS_48K:
				pkt_aif.sample_freq = AIF_SF_48K;
				break;
			case DL_HDMITX_AFS_88K:
				pkt_aif.sample_freq = AIF_SF_88K;
				break;
			case DL_HDMITX_AFS_96K:
				pkt_aif.sample_freq = AIF_SF_96K;
				break;
			case DL_HDMITX_AFS_176K:
				pkt_aif.sample_freq = AIF_SF_176K;
				break;
			case DL_HDMITX_AFS_192K:
				pkt_aif.sample_freq = AIF_SF_192K;
				break;
			default:
				/* refer to stream header */
				pkt_aif.sample_freq = AIF_SF_REFER_TO_STREAM_HEADER;
				break;
			}
		}

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_audio_infoframe(instance, &pkt_aif, true)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of AVI infoframe to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pAviIfData   Pointer to the structure containing AVI infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_video_infoframe
(
    instance_t            instance,
    bool                    enable,
    tx_avi_if_data_t   *p_avi_if_data
)
{
	error_code_t       err;
	bsl_pkt_vif_t pkt_vif;
	bsl_vid_fmt_t vif_info_frame;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if AviIfData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_avi_if_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		/* 3rd api_set_avi_infoframe param */
		pkt_vif.colour             = p_avi_if_data->color_indicator;
		pkt_vif.active_info         = p_avi_if_data->active_info_present;
		pkt_vif.bar_info            = p_avi_if_data->bar_information_data_valid;
		pkt_vif.scan_info           = p_avi_if_data->scan_information;
		pkt_vif.colorimetry        = p_avi_if_data->colorimetry;
#ifndef FORMAT_PC
		pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[p_avi_if_data->video_format_identification_code];
#else /* FORMAT_PC */
		if((p_avi_if_data->video_format_identification_code >= DL_HDMITX_VFMT_PC_MIN)
		   && (p_avi_if_data->video_format_identification_code <= DL_HDMITX_VFMT_PC_MAX)) {
			pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[p_avi_if_data->video_format_identification_code - DL_HDMITX_VFMT_PC_MIN + 1];
		} else
			pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[p_avi_if_data->video_format_identification_code];
#endif /* FORMAT_PC */
		pkt_vif.active_format_ratio  = p_avi_if_data->active_format_aspect_ratio;
		pkt_vif.scaling            = p_avi_if_data->non_uniform_picture_scaling;

#ifdef FORMAT_PC
		if(p_avi_if_data->video_format_identification_code >= HDMITX_VFMT_PC_MIN) {
			if(p_avi_if_data->video_format_identification_code == hdmitx_vfmt_pc_640x480p_60hz) {
				vif_info_frame = hdmitx_vfmt_01_640x480p_60hz;
			} else {
				/* Format PC not Valid in EIA861b */
				vif_info_frame = HDMITX_VFMT_NULL;
			}
		} else {
#endif /* FORMAT_PC */

			vif_info_frame = (bsl_vid_fmt_t)p_avi_if_data->video_format_identification_code;

#ifdef FORMAT_PC
		}
#endif /* FORMAT_PC */

		pkt_vif.vid_format = vif_info_frame;

		if(((p_avi_if_data->video_format_identification_code >= hdmitx_vfmt_06_720x480i_60hz)
		    && (p_avi_if_data->video_format_identification_code <= hdmitx_vfmt_09_720x240p_60hz))
		   || ((p_avi_if_data->video_format_identification_code >= hdmitx_vfmt_21_720x576i_50hz)
		       && (p_avi_if_data->video_format_identification_code <= hdmitx_vfmt_24_720x288p_50hz))) {
			/* Force pixel repeat for formats where it's mandatory */
			pkt_vif.pixel_repeat    = 1;
		} else {
			/* Default to no repeat for all other formats */
			pkt_vif.pixel_repeat    = HDMITX_PIXREP_NONE;
		}

		pkt_vif.end_top_bar_line      = p_avi_if_data->line_number_end_top_bar;
		pkt_vif.start_bottom_bar_line = p_avi_if_data->line_number_start_bottom_bar;
		pkt_vif.end_left_bar_pixel    = p_avi_if_data->line_number_end_left_bar;
		pkt_vif.start_right_bar_pixel = p_avi_if_data->line_number_start_right_bar;

		err = bsl_pkt_set_video_infoframe(instance,
		                                  &pkt_vif, enable);
	} else {
		err = bsl_pkt_set_video_infoframe(instance,
		                                  NULL, enable);
	}

	/* Ignore infoframe interlock in DVI mode */
	if(err == ERR_HDMI_OPERATION_NOT_PERMITTED) {
		err = 0;
	}

	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], err != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of AUD infoframe to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pAudIfData   Pointer to the structure containing AUD infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_audio_infoframe
(
    instance_t            instance,
    bool                    enable,
    tx_aud_if_data_t   *p_aud_if_data
)
{
	error_code_t       err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if AudIfData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_aud_if_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_audio_infoframe(instance,
		                  (bsl_pkt_aif_t *)p_aud_if_data, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_audio_infoframe(instance,
		                  NULL, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of the audio content protection packet to be
 *           sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pAcpPktData  Pointer to the structure containing ACP infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_acppacket
(
    instance_t            instance,
    bool                    enable,
    tx_acp_pkt_data_t  *p_acp_pkt_data
)
{
	error_code_t       err;
	bsl_pkt_t    pkt;
	u8               i;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if AcpPktData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_acp_pkt_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		switch(p_acp_pkt_data->acp_type) {
			/* Make sure bytes reserved are 0 */
		case 0 : /* Generic audio */
			for(i = 0; i < 28; i++) {
				pkt.data_byte[i] = 0;
			}
			break;

		case 1 : /* IEC 60958 identified audio */
			for(i = 0; i < 28; i++) {
				pkt.data_byte[i] = 0;
			}
			break;

		case 2 : /* DVD Audio */
			for(i = 0; i < 2; i++) {
				pkt.data_byte[i] = p_acp_pkt_data->acp_data[i];
			}
			for(i = 2; i < 28; i++) {
				pkt.data_byte[i] = 0;
			}
			break;

		case 3 : /* SuperAudio CD */
			for(i = 0; i < 17; i++) {
				pkt.data_byte[i] = p_acp_pkt_data->acp_data[i];
			}
			for(i = 17; i < 28; i++) {
				pkt.data_byte[i] = 0;
			}
			break;

		default :
			/* Release the sempahore */
			RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)
			return ERR_DLHDMITX_INCONSISTENT_PARAMS;
		}

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_acp(instance,
		                                 &pkt, 28, p_acp_pkt_data->acp_type, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_acp(instance,
		                                 NULL, 0, 0, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of the General Control packet to be sent by Tx
 *           device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pGcpPktData  Pointer to the structure containing GCP packet parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_general_control_packet
(
    instance_t            instance,
    bool                    enable,
    tx_gcp_pkt_data_t  *p_gcp_pkt_data
)
{
	error_code_t       err;
	bsla_mute_t  a_mute;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if GcpPktData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_gcp_pkt_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		if(p_gcp_pkt_data->av_mute == false) {
			a_mute = HDMITX_AMUTE_OFF;
		} else {
			a_mute = HDMITX_AMUTE_ON;
		}

		/* Set contents of general control packet & enable/disable packet insertion */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_general_cntrl(instance,
		                  &a_mute, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_general_cntrl(instance,
		                  NULL, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of ISRC1 packet to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param enable           Enable/disable infoframe insertion.
 *    \param pIsrc1PktData    Pointer to the structure containing GCP packet parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_isrc1packet
(
    instance_t                instance,
    bool                        enable,
    tx_isrc1pkt_data_t    *p_isrc1pkt_data
)
{
	error_code_t       err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if Isrc1PktData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_isrc1pkt_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_isrc1(instance,
		                                   (bsl_pkt_t *)p_isrc1pkt_data->UPC_EAN_ISRC, 16, p_isrc1pkt_data->isrc_cont,
		                                   p_isrc1pkt_data->isrc_valid, p_isrc1pkt_data->isrc_status, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_isrc1(instance,
		                                   NULL, 0, 0, 0, 0, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of ISRC2 packet to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param enable           Enable/disable infoframe insertion.
 *    \param pIsrc2PktData    Pointer to the structure containing GCP packet parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_isrc2packet
(
    instance_t                instance,
    bool                        enable,
    tx_isrc2pkt_data_t    *p_isrc2pkt_data
)
{
	error_code_t       err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if Isrc1PktData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_isrc2pkt_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_isrc2(instance,
		                                   (bsl_pkt_t *)p_isrc2pkt_data->UPC_EAN_ISRC, 16, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_isrc2(instance,
		                                   NULL, 0, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of MPS infoframe to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pMpsIfData   Pointer to the structure containing MPS infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_mpsinfoframe
(
    instance_t            instance,
    bool                    enable,
    tx_mps_if_data_t   *p_mps_if_data
)
{
	error_code_t           err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if MpsIfData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_mps_if_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_mpeg_infoframe(instance,
		                  (bsl_pkt_mpeg_t *)p_mps_if_data, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_mpeg_infoframe(instance,
		                  NULL, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of SPD infoframe to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pSpdIfData   Pointer to the structure containing SPD infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_spd_infoframe
(
    instance_t            instance,
    bool                    enable,
    tx_spd_if_data_t   *p_spd_if_data
)
{
	error_code_t       err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if SpdIfData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_spd_if_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_spd_infoframe(instance,
		                  (bsl_pkt_spd_t *)p_spd_if_data, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_spd_infoframe(instance,
		                  NULL, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Defines the content of VS infoframe to be sent by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable infoframe insertion.
 *    \param pVsIfData    Pointer to the structure containing VS infoframe
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_vs_infoframe
(
    instance_t            instance,
    bool                    enable,
    tx_vs_pkt_data_t   *p_vs_if_data
)
{
	error_code_t       err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	if(enable == true) {
		/* Check if VsIfData pointer is NULL */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_vs_if_data == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_vs_infoframe(instance,
		                  (bsl_pkt_t *)p_vs_if_data->vs_data, 27, p_vs_if_data->version, enable)) != 0, err)
	} else {
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_pkt_set_vs_infoframe(instance,
		                  NULL, 0, p_vs_if_data->version, enable)) != 0, err)
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Enables/disables NULL packet sending (only used for debug purpose).
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance  Instance identifier.
 *    \param enable    Enable/disable packet insertion.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_debug_set_null_packet
(
    instance_t    instance,
    bool            enable
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_pkt_set_null_insert(instance,
	                  enable)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Send one single NULL packet (only used for debug purpose).
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance  Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_debug_set_single_null_packet
(
    instance_t    instance
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_pkt_set_null_single(instance))
	          != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Set the audio output mute status. This function can be used to mute
 *           audio output, without muting video. This can be typically used when
 *           reconfiguring the audio HW after a sample rate change.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance   Instance identifier.
 *    \param muteStatus Mute status (true/false).
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_audio_mute
(
    instance_t    instance,
    bool            audio_mute
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Mute or Un-mute the audio output */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_audio_out_set_mute(instance,
	                                        (bsla_mute_t)audio_mute)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Reset audio CTS.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance   Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_reset_audio_cts
(
    instance_t    instance
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Reset the audio Clock Time Stamp generator */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_audio_in_reset_cts(instance)
	          ) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retrieve EDID Status from driver.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param pEdidStatus      Pointer to the array that will receive the EDID Status.
 *    \param pEdidBlkCount    Pointer to the integer that will receive the number of
 *                            read EDID block.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_status
(
    instance_t            instance,
    tx_edid_status_t  *p_edid_status,
    u8                   *p_edid_blk_count
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if EdidStatus and pReadBytesNumber pointers are NULL */
	RETIF(p_edid_status == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_edid_blk_count == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Get the EDID status from BSL driver */
	*p_edid_status = (tx_edid_status_t)edid_data_status;
	*p_edid_blk_count = 0;

	if(edid_data_status == DL_HDMITX_EDID_READ) {
		/* Get the read EDID block number from BSL driver */
		RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
		          (err = bsl_edid_get_block_count(instance,
		                  p_edid_blk_count)) != 0, err)

	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retrieves audio descriptors from receiver's EDID. This function
 *           parses the EDID of Tx device to get the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance             Instance identifier.
 *    \param pAudioDescs          Pointer to the array that will receive audio
 *                                descriptors.
 *    \param maxAudioDescs        Size of the array.
 *    \param pWrittenAudioDescs   Pointer to the integer that will receive the actual
 *                                number of written descriptors.
 *    \param pAudioFlags          Pointer to the byte to receive Audio Capabilities Flags.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_audio_caps
(
    instance_t                instance,
    tx_edid_audio_desc_t   *p_audio_descs,
    uint                        max_audio_descs,
    uint                        *p_written_audio_descs,
    u8                       *p_audio_flags
)
{
	error_code_t           err;
	bsl_edid_sad_t    edid_sad[HDMI_TX_SAD_MAX_CNT];
	uint                    i;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if AudioDescs, WrittenAudioDescs and AudioFlags pointers are NULL */
	RETIF(p_audio_descs == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_written_audio_descs == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_audio_flags == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get video capabilities from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_audio_capabilities(instance,
	                  edid_sad, max_audio_descs, p_written_audio_descs, p_audio_flags)) != 0, err)

	for(i = 0; i < *p_written_audio_descs; i++) {
		/* Bits[6:3]: EIA/CEA861 mode */
		p_audio_descs[i].format            = (edid_sad[i].mode_chans & 0x78) >> 3;
		/* Bits[2:0]: channels */
		p_audio_descs[i].channels          = edid_sad[i].mode_chans & 0x07;
		/* Supported frequencies */
		p_audio_descs[i].supported_freqs    = edid_sad[i].freqs;

		if(p_audio_descs[i].format == 1) {            /* LPCM format */
			p_audio_descs[i].supported_res = edid_sad[i].byte3 & 0x07;
			p_audio_descs[i].max_bitrate   = 0x00;
		} else if((p_audio_descs[i].format >= 2) &&   /* Compressed format */
		          (p_audio_descs[i].format <= 8)) {
			p_audio_descs[i].supported_res = 0x00;
			p_audio_descs[i].max_bitrate   = edid_sad[i].byte3;
		} else {
			p_audio_descs[i].supported_res = 0x00;
			p_audio_descs[i].max_bitrate   = 0x00;
		}
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retrieves supported video formats (short descriptors) from
 *           receiver's EDID. This function parses the EDID of Rx device to get
 *           the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance             Instance identifier.
 *    \param pVideoDesc           Pointer to the structure that will receive short
 *                                video descriptors.
 *    \param maxVideoFormats      Size of the array.
 *    \param pWrittenVideoFormats Pointer to the integer that will receive the actual
 *                                number of written descriptors.
 *    \param pVideoFlags          Pointer to the byte to receive Video Capability Flags.
 *                                b7: underscan supported
 *                                b6: YCbCr 4:4:4 supported
 *                                b5: YCbCr 4:2:2 supported
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_video_caps
(
    instance_t                instance,
    tx_short_vid_desc_t    *p_video_desc,
    uint                        max_video_formats,
    uint                        *p_written_video_formats,
    u8                       *p_video_flags
)
{
	error_code_t                   err;
	u8                           edid_vfmts_buffer[HDMI_TX_SVD_MAX_CNT];
	tx_edid_video_timings_t    edid_dtdbuffer[NUMBER_DTD_STORED];
	u8                           i;
	u8                           written_dtd = 0;
	u8                           dtd_counter = 0;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if Videoformats, WrittenVideoFormats and VideoFlags pointers are NULL */
	RETIF(p_video_desc == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_written_video_formats == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_video_flags == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(max_video_formats == 0, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get video capabilities from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_video_capabilities(instance,
	                  edid_vfmts_buffer, HDMI_TX_SVD_MAX_CNT, p_written_video_formats, p_video_flags)) != 0, err)

	/* Get detailled descriptors from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = dl_hdmi_tx_edid_get_dtd(instance, edid_dtdbuffer, NUMBER_DTD_STORED, &written_dtd)) != 0, err)

	dtd_counter = 0;
	if(written_dtd > 0) {
		/* Write first DTD in first position of table video desc  */
		p_video_desc[0].video_format = dl_hdmi_tx_convert_dtdto_cea(&(edid_dtdbuffer[dtd_counter]));
		dtd_counter++;

		p_video_desc[0].native_video_format = false;
	}

	/* Start with i = 1 keep the first position for the first DTD */
	for(i = dtd_counter; i < max_video_formats ; i++) {
		if((i < (HDMI_TX_SVD_MAX_CNT + dtd_counter)) && (i < ((*p_written_video_formats) + dtd_counter))) {
			/* Store SVD */
			p_video_desc[i].video_format = (tx_vid_fmt_t)((int)edid_vfmts_buffer[i - dtd_counter] & 0x7F);
			/* if bit 7 is true, it means that is a preferred video format */
			if((edid_vfmts_buffer[i] & 0x80) == 0x80) {
				p_video_desc[i].native_video_format = true;
			} else {
				p_video_desc[i].native_video_format = false;
			}
		} else {
			if((dtd_counter < NUMBER_DTD_STORED) && (dtd_counter < written_dtd)) {
				/* Store DTD except first DTD */
				p_video_desc[i].video_format = dl_hdmi_tx_convert_dtdto_cea(&(edid_dtdbuffer[dtd_counter]));
				dtd_counter++;

				p_video_desc[i].native_video_format = false;
			} else {
				/* VGA is always supported */
				p_video_desc[i].video_format = dl_hdmitx_vfmt_01_640x480p_60hz;
				p_video_desc[i].native_video_format = false;
				/* Last format supported exit from loop for */
				break;
			}
		}
	}

	/* + 1 for VGA format */
	*p_written_video_formats = *p_written_video_formats + dtd_counter + 1;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retrieves supported video formats (short descriptors) from
 *           receiver's EDID. This function parses the EDID of Rx device to get
 *           the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance                 Instance identifier.
 *    \param pPreferredVideoFormat    Pointer to the array that will receive video
 *                                    timing descriptor.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_video_preferred
(
    instance_t                    instance,
    tx_edid_video_timings_t    *p_preferred_video_format
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if PreferredVideoFormat pointer is NULL */
	RETIF(p_preferred_video_format == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get preferred video format from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_video_preferred(instance,
	                  (bsl_edid_dtd_t *)p_preferred_video_format)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/*****************************************************************************/
/**
 *    \brief Retrieves supported detailled video descriptors from
 *           receiver's EDID. This function parses the EDID of Rx device to get
 *           the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param pDTDescriptors   Pointer to the array that will receive detailled
 *                            timing descriptors.
 *    \param maxDTDesc        Size of the array.
 *    \param pWrittenDesc     Pointer to the integer that will receive the actual
 *                            number of written descriptors.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_detailled_timing_descriptors
(
    instance_t                    instance,
    tx_edid_video_timings_t    *p_dtdescriptors,
    u8                           max_dtdesc,
    u8                           *p_written_dtdesc
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if DTDescriptors, WrittenDTDesc pointers are NULL */
	RETIF(p_dtdescriptors == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_written_dtdesc == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get detailled descriptors from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_detailed_timing_descriptors(instance,
	                  (bsl_edid_dtd_t *)p_dtdescriptors, max_dtdesc, p_written_dtdesc)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/*****************************************************************************/
/**
 *    \brief Retrieves supported monitor descriptor from receiver's EDID.
 *           This function parses the EDID of Rx device to get
 *           the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param pEdidFirstMD     Pointer to the array that will receive the first monitor
 *                            descriptors.
 *    \param pEdidSecondMD    Pointer to the array that will receive the second monitor
 *                            descriptors.
 *    \param pEdidOtherMD     Pointer to the array that will receive the other monitor
 *                            descriptors.
 *    \param maxOtherMD       Size of the array.
 *    \param pWrittenOtherMD  Pointer to the integer that will receive the actual
 *                            number of written descriptors.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_monitor_descriptors
(
    instance_t                instance,
    tx_edid_first_md_t     *p_edid_first_md,
    tx_edid_second_md_t    *p_edid_second_md,
    tx_edid_other_md_t     *p_edid_other_md,
    u8                       max_other_md,
    u8                       *p_written_other_md
)
{
#ifdef IMPLEMENTED_IN_BSL

	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if DTDescriptors, WrittenDTDesc pointers are NULL */
	RETIF(p_edid_first_md == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_edid_second_md == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_edid_other_md == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get monitor descriptors from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_monitor_descriptors(instance,
	                  (bsl_edid_first_md_t *)p_edid_first_md, (bsl_edid_second_md_t *)p_edid_second_md,
	                  (bsl_edid_other_md_t *)p_edid_other_md, max_other_md, p_written_other_md)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;

#else /* IMPLEMENTED_IN_BSL */

	g_no_warning = (u8)instance;
	if(p_edid_first_md) g_no_warning = 0;
	if(p_edid_second_md) g_no_warning = 0;
	if(p_edid_other_md) g_no_warning = 0;
	g_no_warning = (u8)max_other_md;
	if(p_written_other_md) g_no_warning = 0;

	return ERR_DLHDMITX_NOT_SUPPORTED;

#endif /* IMPLEMENTED_IN_BSL */

}

/******************************************************************************
 *    \brief Retrieves the sink type from receiver's EDID (HDMI or DVI). This
 *           function parses the EDID of Rx device to get the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance  Instance identifier.
 *    \param pSinkType Pointer to the array that will receive sink type.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_sink_type
(
    instance_t            instance,
    tx_sink_type_t    *p_sink_type
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if SinkType pointer is NULL */
	RETIF(p_sink_type == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Read the source address from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_sink_type(instance,
	                                        (bsl_sink_type_t *)p_sink_type)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retrieves source address from receivers's EDID. This
 *           function parses the EDID of Rx device to get the relevant data.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance         Instance identifier.
 *    \param pSourceAddress   Pointer to the integer that will receive the EDID source
 *                            address.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_RESOURCE_NOT_AVAILABLE : EDID not read
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *
 ******************************************************************************/
error_code_t tx_get_edid_source_address
(
    instance_t    instance,
    u16          *p_source_address
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if SourceAddress pointer is NULL */
	RETIF(p_source_address == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Read the source address from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_edid_get_source_address(instance,
	                  p_source_address)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief Retreives KSV list received by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param pKsv         Pointer to the array that will receive the KSV list.
 *    \param maxKsv       Maximum number of KSV that the array can store.
 *    \param pWrittenKsv  Actual number of KSV written into the array.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_DLHDMITX_NOT_SUPPORTED: device does not support HDCP
 *
 ******************************************************************************/
error_code_t tx_get_ksv_list
(
    instance_t    instance,
    u8           *p_ksv,
    u8           max_ksv,
    u8           *p_written_ksv,
    u8           *p_depth,
    bool            *p_max_casc_exd,
    bool            *p_max_devs_exd
)
{

	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check if pDepth, Ksv and WrittenKsv pointers are NULL */
	RETIF(p_ksv == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_written_ksv == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_depth == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_max_casc_exd == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)
	RETIF(p_max_devs_exd == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Maximum Ksv is HDMITX_KSV_LIST_MAX_DEVICES, 128 devices */
	RETIF_BADPARAM(max_ksv > HDMITX_KSV_LIST_MAX_DEVICES)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Make sure that *pWrittenKsv is 0 */
	*p_written_ksv = 0;

	/* Make sure that *pDepth is 0 */
	*p_depth = 0;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return ERR_DLHDMITX_NOT_SUPPORTED;

}

/******************************************************************************
 *    \brief Enable/Disable HDCP encryption.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance   Instance identifier.
 *    \param hdcpEnable HDCP On/Off (true = On, false = Off).
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_DLHDMITX_RESOLUTION_UNKNOWN: the resolution is unknown
 *            - ERR_DLHDMITX_NOT_SUPPORTED: device does not support HDCP
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 ******************************************************************************/
error_code_t tx_set_hdcp
(
    instance_t    instance,
    bool            hdcp_enable
)
{
	error_code_t               err;
	bsl_rx_sense_t        rx_sense_status;  /* Rx Sense status */
	bsl_hot_plug_t        hpd_status;      /* HPD status */

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check the current state */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Hdcp is not supported if keySeed is null */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          gtx_driver_config_table[instance].key_seed == HDCP_SEED_NULL, ERR_DLHDMITX_NOT_SUPPORTED)

	/* Read rxSenseStatus and hpdStatus to authorize HDCP only if active */
#ifdef TMFL_RX_SENSE_ON
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_rx_sense_get_status(instance,
	                  &rx_sense_status)) != 0, err)
#else
	rx_sense_status = HDMITX_RX_SENSE_ACTIVE;
#endif

	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_hot_plug_get_status(instance,
	                  &hpd_status)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return ERR_DLHDMITX_NOT_SUPPORTED;
}

/******************************************************************************
 *    \brief Get the driver HDCP state.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance          Instance identifier.
 *    \param pHdcpCheckState   Pointer to the integer that will receive the HDCP check state.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_NOT_SUPPORTED: device does not support HDCP
 *
 ******************************************************************************/
error_code_t tx_get_hdcp_state
(
    instance_t            instance,
    tx_hdcp_check_t   *p_hdcp_check_state
)
{
	error_code_t               err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Check if HdcpCheckState pointer is NULL */
	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance], p_hdcp_check_state == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	*p_hdcp_check_state = DL_HDMITX_HDCP_CHECK_NOT_STARTED;

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return ERR_DLHDMITX_NOT_SUPPORTED;
}

/******************************************************************************
 *    \brief Check the result of an HDCP encryption attempt, called at
 *           intervals (set by timeSinceLastCall) after txSetHdcp(true).
 *           This API must be used only in case of No Operating System. if OS,
 *           this is manage internally of this device library.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance             Instance identifier.
 *    \param timeSinceLastCall    Time passed in milliseconds since last call,
 *                                must be shorter than 600 ms.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_NOT_SUPPORTED: device does not support HDCP
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_READ: failed when reading the I2C bus
 *            - ERR_HDMI_I2C_WRITE: failed when writing the I2C bus
 *            - ERR_HDMI_NOT_SUPPORTED: device does not support HDCP
 *
 ******************************************************************************/
error_code_t tx_hdcp_check
(
    instance_t    instance,
    u16          time_since_last_call
)
{
	error_code_t           err;
	bsl_hw_feature_t feature_supported;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	RETIF_SEM(dl_hdmi_tx_it_semaphore[instance],
	          (err = bsl_hw_get_capabilities(instance, &feature_supported)) != 0, err)

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return ERR_DLHDMITX_NOT_SUPPORTED;
}

/******************************************************************************
 *    \brief This function loads a gamut metadata packet into the HW. HW will
 *           actually send it at the beginning of next VS, during the vertical
 *           blanking.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param enable       Enable/disable gamut metadata packet insertion.
 *    \param pGamutData   Pointer to the structure containing gamut metadata
 *                        parameters.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
error_code_t tx_set_gamut_packet
(
    instance_t            instance,
    bool                    enable,
    tx_gamut_data_t   *p_gamut_data
)
{
	/* Not supported for TDA9983 */

	g_no_warning = (u8)instance;
	g_no_warning = (u8)enable;
	if(p_gamut_data) g_no_warning = 0;

	return ERR_DLHDMITX_NOT_SUPPORTED;

}

/******************************************************************************
 *
 *    \brief This function set the revocation list use for HDCP
 *
 *           This function is synchronous.
 *
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *
 *    \param listPtr      Pointer on revocation list provide by application.
 *
 *    \param length       length of revocation list.
 *
 *    \return The call result:
 *
 *            - 0: the call was successful
 *
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *
 *              out of range
 *
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *
 *              inconsistent
 *
 ******************************************************************************/

error_code_t tx_set_hdcprevocation_list(

    instance_t    instance,

    void            *list_ptr,

    u32          length

)

{

	error_code_t           err;

	/* Check if instance number is in range */

	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* Check parameters */

	RETIF((list_ptr == NULL) || (length == 0), ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */

	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	unit_table_tx[instance].revocation_list.p_list  = list_ptr;

	unit_table_tx[instance].revocation_list.length = length;

	/* Release the sempahore */

	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;

}

/*============================================================================*/
/*                           INTERNAL FUNCTION                                */
/*============================================================================*/

/******************************************************************************
 *    \brief Get the REFPIX and REFLINE for output and scaler
 *            for the current settings.
 *
 *    \param vinFmt       Video input format.
 *    \param vinMode      Video input mode.
 *    \param voutFmt      Video output format.
 *    \param syncIn       Type of synchro (ext or emb).
 *    \param pixRate      Video pixel rate.
 *    \param pRefPix      RefPix for output.
 *    \param pRefLine     RefLine for output.
 *    \param pScRefPix    RefPix for scaler.
 *    \param pScRefLine   RefLine for scaler.
 *    \param pbVerified   Pointer to the boolean that will receive the fact that
 *                        this scaler setting was verified.
 *
 *    \return true (Found) or false (Not found).
 *
 ******************************************************************************/
static bool dl_hdmi_tx_get_refline_refpix
(
    tx_vid_fmt_t      vin_fmt,
    tx_vin_mode_t     vin_mode,
    tx_vid_fmt_t      vout_fmt,
    u8                   sync_in,
    tx_pix_rate_t     pix_rate,
    u16                  *p_ref_pix,
    u16                  *p_ref_line,
    u16                  *p_sc_ref_pix,
    u16                  *p_sc_ref_line,
    bool                    *pb_verified
)
{
	u8 short_vin_fmt;
	u8 short_vout_fmt;
	int i;
	bool b_found;

	/* Search for all values to match in table, until table end is reached
	 * when both refPix values are zero */
	*p_ref_pix    = 0;
	*p_ref_line   = 0;
	*p_sc_ref_pix  = 0;
	*p_sc_ref_line = 0;

	/* If match is not found in table, we can assume a verified non-scaler
	 * combination */
	*pb_verified = 1;
	b_found = false;

	if(vout_fmt  <  DL_HDMITX_VFMT_TV_NO_REG_MIN) {
		short_vin_fmt  = k_vfmt_to_short_fmt_tv[vin_fmt];
		short_vout_fmt = k_vfmt_to_short_fmt_tv[vout_fmt];

		for(i = 0; k_refpix_refline[i].short_vin_fmt != TV_INVALID; i++) {
			if((k_refpix_refline[i].short_vin_fmt == short_vin_fmt)
			   && (UNPKMODE(k_refpix_refline[i].mode_rate_sync_verf) == vin_mode)
			   && (k_refpix_refline[i].short_vout_fmt == short_vout_fmt)
			   && (UNPKRATE(k_refpix_refline[i].mode_rate_sync_verf) == pix_rate)
			   && (UNPKSYNC(k_refpix_refline[i].mode_rate_sync_verf) == sync_in)) {
				*p_ref_pix    = k_refpix_refline[i].ref_pix;
				*p_ref_line   = k_refpix_refline[i].ref_line;
				*p_sc_ref_pix  = k_refpix_refline[i].sc_ref_pix;
				*p_sc_ref_line = k_refpix_refline[i].sc_ref_line;
				*pb_verified = UNPKVERF(k_refpix_refline[i].mode_rate_sync_verf);
				b_found = true;
				break;
			}
		}
	}

	return b_found;
}

/******************************************************************************
 *    \brief Set the video infoframe.
 *
 *    \param instance     Instance identifier.
 *    \param voutFmt      Video output format.
 *    \param voutMode     Video output mode.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *            - ERR_HDMI_I2C_READ: failed when reading to the I2C bus
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_OPERATION_NOT_PERMITTED: not allowed in DVI mode
 *
 ******************************************************************************/
static error_code_t dl_hdmi_tx_set_video_infoframe
(
    instance_t            instance,
    tx_vid_fmt_t      vout_fmt,
    tx_vout_mode_t    vout_mode
)
{
	error_code_t       err;
	bsl_pkt_vif_t pkt_vif;
	bsl_vid_fmt_t vif_info_frame;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	/* 3rd api_set_avi_infoframe param */
	pkt_vif.colour             = vout_mode;
	pkt_vif.active_info         = 0;
	pkt_vif.bar_info            = 0;
	pkt_vif.scan_info           = 0;
#ifndef FORMAT_PC
	pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[vout_fmt];
#else /* FORMAT_PC */
	if((vout_fmt >= DL_HDMITX_VFMT_PC_MIN)
	   && (vout_fmt <= DL_HDMITX_VFMT_PC_MAX)) {
		pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[vout_fmt - DL_HDMITX_VFMT_PC_MIN + 1];
	} else
		pkt_vif.picture_aspect_ratio = k_vfmt_to_aspect_tv[vout_fmt];
#endif /* FORMAT_PC */
	pkt_vif.active_format_ratio  = 8;
	pkt_vif.scaling            = 0;

#ifdef FORMAT_PC
	if(vout_fmt >= DL_HDMITX_VFMT_PC_MIN) {
		if(vout_fmt == dl_hdmitx_vfmt_pc_640x480p_60hz) {
			vif_info_frame = (bsl_vid_fmt_t)dl_hdmitx_vfmt_01_640x480p_60hz;
		} else {
			/* Format PC not Valid in EIA861b */
			vif_info_frame = (bsl_vid_fmt_t)DL_HDMITX_VFMT_NULL;
		}
	} else {
#endif /* FORMAT_PC */

		vif_info_frame = (bsl_vid_fmt_t)vout_fmt;

#ifdef FORMAT_PC
	}
#endif /* FORMAT_PC */

	pkt_vif.vid_format = vif_info_frame;
	if(((vout_fmt >= dl_hdmitx_vfmt_06_720x480i_60hz) && (vout_fmt <= dl_hdmitx_vfmt_09_720x240p_60hz))
	   || ((vout_fmt >= dl_hdmitx_vfmt_21_720x576i_50hz) && (vout_fmt <= dl_hdmitx_vfmt_24_720x288p_50hz))) {
		/* Force pixel repeat for formats where it's mandatory */
		pkt_vif.pixel_repeat = 1;
	} else {
		/* Default to no repeat for all other formats */
		pkt_vif.pixel_repeat = HDMITX_PIXREP_NONE;
	}

	switch(vout_fmt) {
	case dl_hdmitx_vfmt_04_1280x720p_60hz:
	case dl_hdmitx_vfmt_05_1920x1080i_60hz:
	case dl_hdmitx_vfmt_16_1920x1080p_60hz:
	case dl_hdmitx_vfmt_19_1280x720p_50hz:
	case dl_hdmitx_vfmt_20_1920x1080i_50hz:
	case dl_hdmitx_vfmt_31_1920x1080p_50hz:
		pkt_vif.colorimetry = (u8)DL_HDMITX_COLORIMETRY_ITU709;
		break;

	default:
		pkt_vif.colorimetry = (u8)DL_HDMITX_COLORIMETRY_ITU601;
		break;
	}

	pkt_vif.end_top_bar_line      = 0;
	pkt_vif.start_bottom_bar_line = 0;
	pkt_vif.end_left_bar_pixel    = 0;
	pkt_vif.start_right_bar_pixel = 0;

	err = bsl_pkt_set_video_infoframe(instance,
	                                  &pkt_vif, true);

	/* Ignore infoframe interlock in DVI mode */
	if(err == ERR_HDMI_OPERATION_NOT_PERMITTED) {
		err = 0;
	}

	return err;
}

/******************************************************************************
 *    \brief Set colourbar test pattern on with RGB infoframe
 *
 *    \param instance     Instance identifier.
 *    \param voutFmt      Video output format.
 *    \param voutMode     Video output mode.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 ******************************************************************************/
error_code_t dl_hdmi_tx_set_test_pattern_on
(
    instance_t            instance,
    tx_vid_fmt_t		vout_fmt,
    tx_vout_mode_t    vout_mode,
    tx_test_pattern_t pattern
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	RETIF((err = bsl_test_set_pattern(instance,
	                                  (bsl_test_pattern_t)pattern)) != 0, err)

	if(pattern > DL_HDMITX_PATTERN_CBAR8) {
		RETIF((err = dl_hdmi_tx_set_video_infoframe(instance, vout_fmt, vout_mode)) != 0, err)
	} else {
		/* For DL_HDMITX_PATTERN_CBAR8 and DL_HDMITX_PATTERN_CBAR4, video mode in infoframe should be RGB */
		RETIF((err = dl_hdmi_tx_set_video_infoframe(instance, vout_fmt, DL_HDMITX_VOUTMODE_RGB444)) != 0, err)
	}
	return 0;
}

/******************************************************************************
 *    \brief Set colourbar test pattern off with previous infoframe
 *
 *    \param instance     Instance identifier.
 *    \param voutFmt      Video output format.
 *    \param voutMode     Video output mode.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_HDMI_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_HDMI_BAD_PARAMETER: a parameter was out of range
 *            - ERR_HDMI_NOT_INITIALIZED: transmitter not initialized
 *            - ERR_HDMI_I2C_WRITE: failed when writing to the I2C bus
 *
 ******************************************************************************/
error_code_t dl_hdmi_tx_set_test_pattern_off
(
    instance_t            instance,
    tx_vid_fmt_t      vout_fmt,
    tx_vout_mode_t    vout_mode
)
{
	error_code_t err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	RETIF((err = bsl_test_set_pattern(instance,
	                                  (bsl_test_pattern_t)DL_HDMITX_PATTERN_OFF)) != 0, err)

	/* Restore video infoframe */
	RETIF((err = dl_hdmi_tx_set_video_infoframe(instance, vout_fmt, vout_mode)) != 0, err)

	return 0;
}

/******************************************************************************
 *    \brief HDCP ENCRYPT interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_encrypt
(
    instance_t    instance
)
{
   (void)instance; /* Remove compiler warning */
}

/******************************************************************************
 *    \brief HPD interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_hpd
(
    instance_t    instance
)
{
	error_code_t           err;
	bsl_hot_plug_t    hpd_status;  /* HPD status */
	power_state_t          power_state; /* Power state of transmitter */

	/* Get the current state */

	/* If the current state is NOT_INITALIZED, set a flag */
	if(dl_hdmi_tx_get_state(instance) == STATE_NOT_INITIALIZED) {
		/* Get Hot Plug status */
		err = bsl_hot_plug_get_status(instance, &hpd_status);

		if(err != 0) return;

		if(hpd_status == HDMITX_HOTPLUG_ACTIVE) {
			event_hpdactive_flag = true;
		} else {
			event_hpdactive_flag = false;
		}
	} else {
		hpd_status = HDMITX_HOTPLUG_INVALID;

		/* Reset the EDID status */
		edid_data_status = HDMITX_EDID_NOT_READ;

		/* Get Hot Plug status */
		err = bsl_hot_plug_get_status(instance, &hpd_status);

		if(err != 0) return;

		/* Get the power state of the transmitter */
		err = bsl_power_get_state(instance, &power_state);

		if(err != 0) return;

		/* Has hot plug changed to Active? */
		if(hpd_status == HDMITX_HOTPLUG_ACTIVE) {
			/* Set state machine to Plugged */
			dl_hdmi_tx_set_state(instance, STATE_PLUGGED);

			if(dl_hdmi_tx_get_event_status(instance, DL_HDMITX_HPD_ACTIVE) == DL_HDMITX_EVENT_ENABLED) {
				event_call_hpdactive_cback = false;
				/* Release the sempahore */
				(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance]);
				unit_table_tx[instance].p_callback(DL_HDMITX_HPD_ACTIVE);
				/* Take the sempahore */
				(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance]);
			} else {
				event_call_hpdactive_cback = true;
			}

			if(power_state == power_on) {
				/* Yes: Wait for DDC line to settle before reading EDID */
				bsl_sys_timer_wait(instance, 500); /* ms */

				/* EDID read */
				err = bsl_edid_get_block_data(instance,
				                              unit_table_tx[instance].p_edid_buffer, (u32)((unit_table_tx[instance].edid_buffer_size) >> 7),
				                              unit_table_tx[instance].edid_buffer_size, &edid_data_status);

				dl_hdmi_tx_handle_edid_read(instance);

			}
		} else {
			event_call_hpdactive_cback = false;
			/* Set state machine to Unplugged */
			dl_hdmi_tx_set_state(instance, STATE_UNPLUGGED);

			if(dl_hdmi_tx_get_event_status(instance, DL_HDMITX_HPD_INACTIVE) == DL_HDMITX_EVENT_ENABLED) {
				/* Release the sempahore */
				(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance]);
				unit_table_tx[instance].p_callback(DL_HDMITX_HPD_INACTIVE);
				/* Take the sempahore */
				(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance]);
			}
		}
	}
}

/******************************************************************************
 *    \brief T0 interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_t0
(
    instance_t    instance
)
{
	(void)instance;
}

/******************************************************************************
 *    \brief BCAPS interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_bcaps
(
    instance_t    instance
)
{
	(void)instance;
}

/******************************************************************************
 *    \brief BSTATUS interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_bstatus
(
    instance_t    instance
)
{
	(void)instance;
}

/******************************************************************************
 *    \brief SHA_1 interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_sha_1
(
    instance_t    instance
)
{
	(void)instance;
}

/******************************************************************************
 *    \brief PJ interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_pj
(
    instance_t    instance
)
{
	(void)instance;
}

#ifdef TMFL_TDA9981_SUPPORT
/******************************************************************************
 *    \brief R0 interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_r0
(
    instance_t    instance
)
{
	instance_status_info_tx[instance].p_col_bar_state->hdcp_secure_or_t0      = false;
	instance_status_info_tx[instance].p_col_bar_state->disable_color_bar_on_r0 = true;
	instance_status_info_tx[instance].p_col_bar_state->hdcp_colbar_change    = true;
}

/******************************************************************************
 *    \brief SW_INT interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_sw_int
(
    instance_t    instance
)
{
	DUMMY_ACCESS(instance);
}
#endif

/******************************************************************************
 *    \brief RX_SENSE interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
#ifdef TMFL_RX_SENSE_ON
static void dl_hdmi_tx_handle_rx_sense
(
    instance_t    instance
)
{
	error_code_t           err;
	bsl_rx_sense_t    rx_sense_status;  /* Rx Sense status */
	bsl_hot_plug_t    hpd_status;      /* HPD status */

	err = bsl_rx_sense_get_status(instance,
	                              &rx_sense_status);

	if(err != 0) return;

	err = bsl_hot_plug_get_status(instance,
	                              &hpd_status);

	if(err != 0) return;

	/*    if (hpdStatus == HDMITX_HOTPLUG_ACTIVE)
	 *	    {*/
	if(rx_sense_status == HDMITX_RX_SENSE_ACTIVE) {
		if(dl_hdmi_tx_get_event_status(instance, DL_HDMITX_RX_DEVICE_ACTIVE) == DL_HDMITX_EVENT_ENABLED) {
			event_call_rxsens_active_cback = false;
			/* Release the sempahore */
			(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance]);
			unit_table_tx[instance].p_callback(DL_HDMITX_RX_DEVICE_ACTIVE);
			/* Take the sempahore */
			(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance]);
		} else {
			event_call_rxsens_active_cback = true;
		}
	} else if(rx_sense_status == HDMITX_RX_SENSE_INACTIVE) {
		event_call_rxsens_active_cback = false;
	}
	/*    }*/
}
#endif

/******************************************************************************
 *    \brief EDID_READ interrupt callback.
 *
 *    \param instance     Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_handle_edid_read
(
    instance_t    instance
)
{
	error_code_t           err = 0;
	u8                   edid_status = DL_HDMITX_EDID_NOT_READ;

	/* Get the edid status and read the connected device's EDID */

	/* Get Edid status */
	edid_status = edid_data_status;

	if(err != 0) {
		/* Set state machine to Plugged */
		dl_hdmi_tx_set_state(instance, STATE_PLUGGED);
		return;
	}

	/* Has hot plug changed to Active? */
	if((edid_status == DL_HDMITX_EDID_READ) ||
	   (edid_status == DL_HDMITX_EDID_ERROR_CHK)) {
		/* Set state machine to EDID available */
		dl_hdmi_tx_set_state(instance, STATE_EDID_AVAILABLE);
	} else {
		/* Set state machine to Plugged */
		dl_hdmi_tx_set_state(instance, STATE_PLUGGED);
	}

	if(dl_hdmi_tx_get_event_status(instance, DL_HDMITX_EDID_RECEIVED) == DL_HDMITX_EVENT_ENABLED) {
		/* Release the sempahore */
		(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance]);
		unit_table_tx[instance].p_callback(DL_HDMITX_EDID_RECEIVED);
		/* Take the sempahore */
		(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance]);
	}
}

/******************************************************************************
 *    \brief Command processing task, dedicated to unit/instance 0.
 *
 *    \param NA.
 *
 *    \return NA.
 *
 ******************************************************************************/
#ifndef TMFL_NO_RTOS
static void command_task_unit0()
{
	u8                   command;
	/* Just to avoid compiler warning */
	bool                    loop = true;

	while(loop) {
		tx_iwqueue_receive(unit_table_tx[0].queue_handle, &command);

		/* Take the sempahore */
		(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[0]);

		/* Clear T0 flag before polling for interrupts */
		instance_status_info_tx[0].p_col_bar_state->hdcp_secure_or_t0 = false;

		if(g_i2cdebug_accesses_enabled == true) {
			bsl_hw_handle_interrupt(0);

		} /* gI2CDebugAccessesEnabled */

		/* Enable interrupts for Tx (interrupts are disabled in the HandleInterrupt function) */
		tx_iwenable_interrupts(DL_HDMI_IW_TX_1);

		/* Release the sempahore */
		(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[0]);
	};
}

/******************************************************************************
 *    \brief Command processing task, dedicated to unit/instance 1.
 *
 *    \param NA.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void command_task_unit1()
{
	u8                   command;
	/* Just to avoid compiler warning */
	bool                    loop = true;

	while(loop) {
		tx_iwqueue_receive(unit_table_tx[1].queue_handle, &command);

		/* Take the sempahore */
		(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[1]);

		/* Clear T0 flag before polling for interrupts */
		instance_status_info_tx[1].p_col_bar_state->hdcp_secure_or_t0 = false;

		bsl_hw_handle_interrupt(1);

		/* Enable interrupts for Tx (interrupts are disabled in the HandleInterrupt function) */
		tx_iwenable_interrupts(DL_HDMI_IW_TX_2);

		/* Release the sempahore */
		(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[1]);
	};
}

/******************************************************************************
 *    \brief Hdcp check task, dedicated to unit/instance 0.
 *
 *    \param NA.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void hdcp_task_unit0()
{
	bool loop = true;    /* Just to avoid compiler warning */
	bsl_hw_feature_t feature_supported;

	bsl_hw_get_capabilities(0, &feature_supported);

	while(loop) {
		(void)tx_iwwait(35);

		/* Take the sempahore */
		(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[0]);

		if(g_i2cdebug_accesses_enabled == true) {

			if(g_ksv_secure_timer_counter) {
				g_ksv_secure_timer_counter++;

				if(g_ksv_secure_timer_counter == 5) {
#ifdef TMFL_TDA9981_SUPPORT
					dl_hdmi_tx_handle_r0(0);
#endif
					g_ksv_secure_timer_counter = 0;
				}

			}

			dl_hdmi_tx_check_color_bar(0);
			dl_hdmi_tx_check_hdcp_color_bar(0);

		} /*  gI2CDebugAccessesEnabled == true*/

		/* Release the sempahore */
		(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[0]);
	};
}

/******************************************************************************
 *    \brief Hdcp check task, dedicated to unit/instance 1.
 *
 *    \param NA.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void hdcp_task_unit1()
{
	bool loop = true;    /* Just to avoid compiler warning */
	bsl_hw_feature_t feature_supported;

	bsl_hw_get_capabilities(1, &feature_supported);

	while(loop) {
		(void)tx_iwwait(35);

		/* Take the sempahore */
		(void)tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[1]);

		dl_hdmi_tx_check_color_bar(1);
		dl_hdmi_tx_check_hdcp_color_bar(1);

		/* Release the sempahore */
		(void)tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[1]);
	};
}
#endif /* TMFL_NO_RTOS */

/******************************************************************************
 *    \brief Check hdcp state to manage color bar.
 *
 *    \param instance Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_check_hdcp_color_bar
(
    instance_t instance
)
{
	/* Use HDCP check result to control HDCP colour bars */
	if((instance_status_info_tx[instance].p_col_bar_state->disable_color_bar_on_r0 == true)
	   && (instance_status_info_tx[instance].p_col_bar_state->hdcp_colbar_change == true)
	   && (instance_status_info_tx[instance].p_col_bar_state->hdcp_secure_or_t0 == false)) {
		/* Remove test pattern once if Authenticated with no error interrupts */
		if(instance_status_info_tx[instance].p_col_bar_state->color_bar_on != false) {
			instance_status_info_tx[instance].p_col_bar_state->color_bar_on = false;
			instance_status_info_tx[instance].p_col_bar_state->change_color_bar_now = true;
		}
		/* Reset state flags */
		instance_status_info_tx[instance].p_col_bar_state->hdcp_colbar_change = false;
		instance_status_info_tx[instance].p_col_bar_state->hdcp_secure_or_t0 = true;
	}

	if((instance_status_info_tx[instance].p_col_bar_state->hdcp_encrypt_or_t0 == true)
	   && (instance_status_info_tx[instance].p_col_bar_state->in_out_first_set_done == true)) {
		/* Set test pattern once if not Authenticated, to mask HDCP failure */
		if(instance_status_info_tx[instance].p_col_bar_state->color_bar_on != true) {
			instance_status_info_tx[instance].p_col_bar_state->color_bar_on = true;
			instance_status_info_tx[instance].p_col_bar_state->change_color_bar_now = true;
		}
		/* Reset state flag */
		instance_status_info_tx[instance].p_col_bar_state->hdcp_encrypt_or_t0 = false;
	}
}

/******************************************************************************
 *    \brief Show color bars or restore the last video format.
 *
 *    \param instance Instance identifier.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_check_color_bar
(
    instance_t instance
)
{
	if((instance_status_info_tx[instance].p_col_bar_state->in_out_first_set_done == true)
	   && (instance_status_info_tx[instance].p_col_bar_state->change_color_bar_now == true)) {
		instance_status_info_tx[instance].p_col_bar_state->change_color_bar_now = false;

		if(unit_table_tx[instance].simplay_hd == true) {
			if(instance_status_info_tx[instance].p_col_bar_state->color_bar_on == true) {
				/* Set service mode colour bar on/off (also used as HDCP logo pattern) */
				(void)dl_hdmi_tx_set_test_pattern_on(instance, instance_status_info_tx[instance].p_video_info->video_out_config.format,
				                                     instance_status_info_tx[instance].p_video_info->video_out_config.mode,
				                                     gtx_driver_config_table[instance].pattern);
			} else {
				/* Restore last output format and mode */
				(void)dl_hdmi_tx_set_test_pattern_off(instance,
				                                      instance_status_info_tx[instance].p_video_info->video_out_config.format,
				                                      instance_status_info_tx[instance].p_video_info->video_out_config.mode);
			}
		}
	}
}

/******************************************************************************
 *    \brief Set the state of the state machine.
 *
 *    \param instance Instance identifier.
 *    \param state    State of the state machine.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_set_state
(
    instance_t            instance,
    tx_driver_state_t state
)
{
	/* Set the state */
	unit_table_tx[instance].state = state;
}

/******************************************************************************
 *    \brief Get the state of the state machine.
 *
 *    \param instance Instance identifier.
 *
 *    \return txDriverState_t Current State of the state machine.
 *
 ******************************************************************************/
tx_driver_state_t dl_hdmi_tx_get_state
(
    instance_t instance
)
{
	tx_driver_state_t state;

	/* Get the state */
	state = unit_table_tx[instance].state;

	return (state);
}

/******************************************************************************
 *    \brief Get the state of the event (enabled or disabled).
 *
 *    \param instance Instance identifier.
 *    \param event    Event to give the state.
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_event_status_t dl_hdmi_tx_get_event_status
(
    instance_t        instance,
    tx_event_t   event
)
{
	tx_event_status_t   event_status;

	/* Get the event status */
	event_status = instance_status_info_tx[instance].p_event_state[event].status;

	return (event_status);
}

error_code_t tx_debug_enable_i2caccesses(instance_t instance,
        bool         enable_i2c)
{
	error_code_t   err = 0;

	/* Check if instance number is in range */
	if((instance < 0) || (instance >= MAX_UNITS)) {
		err = ERR_DLHDMITX_BAD_INSTANCE;
		return err;
	}

	if(enable_i2c == true) {
		err = bsl_debug_write_fake_reg_page(instance);
		g_i2cdebug_accesses_enabled = true;
	} else {
		g_i2cdebug_accesses_enabled = false;
	}

	return err;
}

error_code_t dl_hdmi_tx_edid_get_dtd
(
    instance_t                    instance,
    tx_edid_video_timings_t    *p_dtdescriptors,
    u8                           max_dtdesc,
    u8                           *p_written_dtdesc
)
{
	error_code_t   err;

	/* Check the current state */
	RETIF(dl_hdmi_tx_get_state(instance) != STATE_EDID_AVAILABLE, ERR_DLHDMITX_INVALID_STATE)

	/* Get detailled descriptors from EDID, return ERR_DLHDMITX_NO_RESOURCES if EDID are not read */
	RETIF((err = bsl_edid_get_detailed_timing_descriptors(
	                 instance, (bsl_edid_dtd_t *)p_dtdescriptors, max_dtdesc, p_written_dtdesc)) != 0, err);

	return 0;
}

static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea
(
    tx_edid_video_timings_t    *p_dtdescriptors
)
{

	tx_vid_fmt_t          code_cea;
	tx_pict_aspect_ratio_t picture_aspect_ratio;
	bool                        format_interlaced;

	format_interlaced = false;

	if((p_dtdescriptors->flags) & 0x80) {
		format_interlaced = true;
	}

	picture_aspect_ratio = dl_hdmi_tx_calc_aspect_ratio(p_dtdescriptors->h_image_size, p_dtdescriptors->v_image_size);

	switch(p_dtdescriptors->h_active_pixels) {
	case 640:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_640hap(p_dtdescriptors);
		break;

	case 720:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_720hap(p_dtdescriptors, picture_aspect_ratio);
		break;

	case 1280:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_1280hap(p_dtdescriptors);
		break;

	case 1920:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_1920hap(p_dtdescriptors, format_interlaced);
		break;

	case 1440:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_1440hap(p_dtdescriptors, picture_aspect_ratio, format_interlaced);
		break;

	case 2880:
		code_cea = dl_hdmi_tx_convert_dtdto_cea_2880hap(p_dtdescriptors, picture_aspect_ratio, format_interlaced);
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;

}

/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_640HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_640hap
(
    tx_edid_video_timings_t    *p_dtdescriptors
)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->v_active_lines) {
	case 480:
		code_cea = dl_hdmitx_vfmt_01_640x480p_60hz;
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;

}

/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_720HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_720hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio
)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->v_active_lines) {
	case 480:
		if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
			code_cea = dl_hdmitx_vfmt_02_720x480p_60hz;
		} else {
			code_cea = dl_hdmitx_vfmt_03_720x480p_60hz;
		}
		break;

	case 576:
		if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
			code_cea = dl_hdmitx_vfmt_17_720x576p_50hz;
		} else {
			code_cea = dl_hdmitx_vfmt_18_720x576p_50hz;
		}
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;

}
/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_1280HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1280hap
(
    tx_edid_video_timings_t    *p_dtdescriptors
)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->v_active_lines) {
	case 720:
		switch(p_dtdescriptors->h_blank_pixels) {
		case 370:
			code_cea = dl_hdmitx_vfmt_04_1280x720p_60hz;
			break;

		case 700:
			code_cea = dl_hdmitx_vfmt_19_1280x720p_50hz;
			break;

		default:
			/* Not a valid format */
			code_cea = DL_HDMITX_VFMT_NULL;
			break;
		}
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;
}

/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_1920HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1920hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    bool                            format_interlaced

)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->h_blank_pixels) {
	case 280:
		if(format_interlaced) {
			code_cea = dl_hdmitx_vfmt_05_1920x1080i_60hz;
		} else {
			if(p_dtdescriptors->pixel_clock == 14850) {
				code_cea = dl_hdmitx_vfmt_16_1920x1080p_60hz;
			} else {
				code_cea = dl_hdmitx_vfmt_34_1920x1080p_30hz;
			}
		}
		break;

	case 720:
		if(format_interlaced) {
			code_cea = dl_hdmitx_vfmt_20_1920x1080i_50hz;
		} else {
			switch(p_dtdescriptors->pixel_clock) {
			case 14850:
				code_cea = dl_hdmitx_vfmt_31_1920x1080p_50hz;
				break;

			case 7425:
				code_cea = dl_hdmitx_vfmt_33_1920x1080p_25hz;
				break;
			default:
				/* Not a valid format */
				code_cea = DL_HDMITX_VFMT_NULL;
				break;
			}
		}
		break;

	case 830:
		code_cea = dl_hdmitx_vfmt_32_1920x1080p_24hz;
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;
}

/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_1440HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_1440hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio,
    bool                            format_interlaced

)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->v_active_lines) {
	case 240:
		if(format_interlaced) {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_06_720x480i_60hz;
			} else {
				code_cea = dl_hdmitx_vfmt_07_720x480i_60hz;
			}
		} else {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_08_720x240p_60hz;
			} else {
				code_cea = dl_hdmitx_vfmt_09_720x240p_60hz;
			}
		}
		break;

	case 288:
		if(format_interlaced) {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_21_720x576i_50hz;
			} else {
				code_cea = dl_hdmitx_vfmt_22_720x576i_50hz;
			}
		} else {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_23_720x288p_50hz;
			} else {
				code_cea = dl_hdmitx_vfmt_24_720x288p_50hz;
			}
		}
		break;

	case 480:
		if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
			code_cea = dl_hdmitx_vfmt_14_1440x480p_60hz;
		} else {
			code_cea = dl_hdmitx_vfmt_15_1440x480p_60hz;
		}
		break;

	case 576:
		if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
			code_cea = dl_hdmitx_vfmt_29_1440x576p_50hz;
		} else {
			code_cea = dl_hdmitx_vfmt_30_1440x576p_50hz;
		}
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;
}

/******************************************************************************
 *    \brief dlHdmiTxConvertDTDtoCEA_2880HAP .
 *
 *    \param  pDTDescriptors      DTD to convert.
 *            pictureAspectRatio  aspect ratio of DTD
 *            formatInterlaced    DTD Interlaced or progressif
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_vid_fmt_t dl_hdmi_tx_convert_dtdto_cea_2880hap
(
    tx_edid_video_timings_t    *p_dtdescriptors,
    tx_pict_aspect_ratio_t     picture_aspect_ratio,
    bool                            format_interlaced
)
{
	tx_vid_fmt_t          code_cea;

	switch(p_dtdescriptors->v_active_lines) {
	case 240:
		if(format_interlaced) {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_10_720x480i_60hz;
			} else {
				code_cea = dl_hdmitx_vfmt_11_720x480i_60hz;
			}
		} else {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_12_720x240p_60hz;
			} else {
				code_cea = dl_hdmitx_vfmt_13_720x240p_60hz;
			}
		}
		break;

	case 288:
		if(format_interlaced) {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_25_720x576i_50hz;
			} else {
				code_cea = dl_hdmitx_vfmt_26_720x576i_50hz;
			}
		} else {
			if(picture_aspect_ratio == DL_HDMITX_P_ASPECT_RATIO_4_3) {
				code_cea = dl_hdmitx_vfmt_27_720x288p_50hz;
			} else {
				code_cea = dl_hdmitx_vfmt_28_720x288p_50hz;
			}
		}
		break;

	default:
		/* Not a valid format */
		code_cea = DL_HDMITX_VFMT_NULL;
		break;
	}

	return code_cea;
}

/******************************************************************************
 *    \brief Caculation of aspect ratio.
 *
 *    \param HImageSize Horizontal image size.
 *    \param VImageSize Vertical image size.
 *
 *    \return NA.
 *
 ******************************************************************************/
static tx_pict_aspect_ratio_t dl_hdmi_tx_calc_aspect_ratio(
    u16    himage_size,
    u16    vimage_size
)
{
	tx_pict_aspect_ratio_t    picture_aspect_ratio;
	u16                         calc_picture_aspect_ratio;

	/* Define picture Aspect Ratio                                          */
	/* 16/9 = 1.77777 so the result approach is 2                           */
	/* 4/3 = 1.33333 so the result approach is 1                            */
	/*  operation :                                                         */
	/* ImageSize + (vImageSize/2)                                           */
	/* -------------------------- > vImageSize     ->true 16/9 false 4/3    */
	/*           2                                                          */

	calc_picture_aspect_ratio = ((u16)(himage_size + ((vimage_size) >> 1))) >> 1;

	if(calc_picture_aspect_ratio > vimage_size) {
		picture_aspect_ratio = DL_HDMITX_P_ASPECT_RATIO_16_9;
	} else {
		picture_aspect_ratio = DL_HDMITX_P_ASPECT_RATIO_4_3;
	}

	return picture_aspect_ratio;

}

/******************************************************************************
 *    \brief dlHdmiTxCheckHdcpBksv .
 *
 *    \param pHdcpBksvTested  ksv To test.
 *    \param pbBksvSecure     Test result.
 *    \param bBigEndian       ksv provide by hardware are in little or big endian.
 *
 *    \return NA.
 *
 ******************************************************************************/
static void dl_hdmi_tx_check_hdcp_bksv
(
    instance_t    instance,
    u8          *p_hdcp_bksv_tested,
    bool           *pb_bksv_secure,
    bool            b_big_endian
)
{
	u32 nb_in_revocation_list;

	nb_in_revocation_list = 0;

	if((unit_table_tx[instance].revocation_list.p_list != NULL) && (unit_table_tx[instance].revocation_list.length > 0)) {
		while((*pb_bksv_secure == true) && (nb_in_revocation_list < unit_table_tx[instance].revocation_list.length)) {
			if(b_big_endian) {
				if((p_hdcp_bksv_tested[0] == unit_table_tx[instance].revocation_list.p_list[nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE])
				   &&
				   (p_hdcp_bksv_tested[1] == unit_table_tx[instance].revocation_list.p_list[1 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[2] == unit_table_tx[instance].revocation_list.p_list[2 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[3] == unit_table_tx[instance].revocation_list.p_list[3 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[4] == unit_table_tx[instance].revocation_list.p_list[4 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				  ) {
					*pb_bksv_secure = false;
				}
			} else {
				if((p_hdcp_bksv_tested[4] == unit_table_tx[instance].revocation_list.p_list[nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE])
				   &&
				   (p_hdcp_bksv_tested[3] == unit_table_tx[instance].revocation_list.p_list[1 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[2] == unit_table_tx[instance].revocation_list.p_list[2 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[1] == unit_table_tx[instance].revocation_list.p_list[3 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				   &&
				   (p_hdcp_bksv_tested[0] == unit_table_tx[instance].revocation_list.p_list[4 + (nb_in_revocation_list * HDMITX_KSV_BYTES_PER_DEVICE)])
				  ) {
					*pb_bksv_secure = false;
				}
			}
			nb_in_revocation_list++;
		}
	}
}

error_code_t tx_get_hpdstatus
(
    instance_t            instance,
    tx_hot_plug_t    *p_hpdstatus
)
{
	error_code_t   err;

	/* Check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMITX_BAD_INSTANCE)

	RETIF(p_hpdstatus == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* Take the sempahore */
	RETIF((err = tx_iwsemaphore_p(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	/* Get the HPD status from BSL driver */
	err = bsl_hot_plug_get_status(instance, (bsl_hot_plug_t *)p_hpdstatus);

	if(err == 0) {
		/* do nothing */
	} else {
		*p_hpdstatus = DL_HDMITX_HOTPLUG_INVALID;
	}

	/* Release the sempahore */
	RETIF((err = tx_iwsemaphore_v(dl_hdmi_tx_it_semaphore[instance])) != 0, err)

	return 0;

} /* txGetHPDStatus */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
