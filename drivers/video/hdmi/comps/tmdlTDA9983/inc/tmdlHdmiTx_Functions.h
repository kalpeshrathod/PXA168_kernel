/**
 * Copyright (C) 2007 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_Functions.h
 *
 * \version       $Revision: 1 $
 *
 * \date          $Date: 02/08/07 8:32 $
 *
 * \brief         devlib driver component API for the TDA998x HDMI Transmitters
 *
 * \section refs  Reference Documents
 * HDMI Tx Driver - FRS.doc,
 * HDMI Tx Driver - tx - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History: tx_Functions.h $
 *
 * *****************  Version 1  *****************
 * User: Demoment     Date: 02/08/07   Time: 8:32
 * Updated in $/Source/tx/inc
 * initial version
 *
 *
 *   \endverbatim
 *
 * */

#ifndef DLHDMITX_FUNCTIONS_H
#define DLHDMITX_FUNCTIONS_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#include "tmNxTypes.h"
#include "tmdlHdmiTx_Types.h"

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*****************************************************************************/
/**
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
    swversion_t  *p_swversion
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
    tx_capabilities_t *p_capabilities
);

/*****************************************************************************/
/**
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
    unit_select_t            unit,
    tx_capabilities_t *p_capabilities
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
    instance_t   instance
);

/*****************************************************************************/
/**
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
 *
 ******************************************************************************/
error_code_t tx_set_power_state
(
    instance_t    instance,
    power_state_t  power_state
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
 *    \brief Set the configuration of instance attributes. This function is
 *           required by DVP architecture rules but actually does nothing in this
 *           driver
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
    instance_t      instance,
    tx_event_t event
);

/*****************************************************************************/
/**
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
    instance_t      instance,
    tx_event_t event
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
    instance_t               instance,
    tx_video_in_config_t  video_input_config,
    tx_video_out_config_t video_output_config,
    tx_audio_in_config_t  audio_input_config,
    tx_sink_type_t       sink_type
);

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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
    instance_t instance,
    bool         audio_mute
);

/*****************************************************************************/
/**
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
    instance_t instance
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
 *            - ERR_DLHDMITX_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_DLHDMITX_BAD_UNIT_NUMBER: bad transmitter unit number
 *            - ERR_DLHDMITX_NOT_INITIALIZED: the transmitter is not initialized
 *            - ERR_DLHDMITX_INVALID_STATE: the state is invalid for
 *              the function
 *
 ******************************************************************************/
error_code_t tx_get_edid_sink_type
(
    instance_t            instance,
    tx_sink_type_t    *p_sink_type
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
 *    \brief Retreives KSV list received by Tx device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param pKsv         Pointer to the array that will receive the KSV list.
 *    \param maxKsv       Maximum number of KSV that the array can store.
 *    \param pWrittenKsv  Actual number of KSV written into the array.
 *    \param pDepth       Connection tree depth.
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
);

/*****************************************************************************/
/**
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
    instance_t instance,
    bool         hdcp_enable
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

/*****************************************************************************/
/**
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
);

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
);

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
);

/******************************************************************************
 *    \brief This function set the revocation list use for HDCP
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance     Instance identifier.
 *    \param listPtr      Pointer on revocation list provide by application.
 *    \param length       length of revocation list.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 ******************************************************************************/

error_code_t tx_set_hdcprevocation_list(
    instance_t    instance,
    void            *list_ptr,
    u32          length
);

error_code_t tx_get_hpdstatus
(
    instance_t            instance,
    tx_hot_plug_t    *p_hpdstatus
);

#endif /* DLHDMITX_FUNCTIONS_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

