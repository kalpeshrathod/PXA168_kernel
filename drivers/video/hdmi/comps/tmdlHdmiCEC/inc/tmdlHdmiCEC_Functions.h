/*============================================================================= */
/* Copyright (C) 2007 NXP N.V., All Rights Reserved. */
/* This source code and any compilation or derivative thereof is the proprietary */
/* information of NXP N.V. and is confidential in nature. Under no circumstances */
/* is this software to be  exposed to or placed under an Open Source License of */
/* any type without the expressed written permission of NXP N.V. */
/*============================================================================= */
/*!
 *    \file    dlHdmiCEC_Functions.h
 *
 *	\version 1.0
 *
 *	\date    04/07/2007
 *
 * 	\brief   This provides interfaces description of CEC messages.
 *
 *    \section refs  Reference Documents
 *             TDA998X Driver - dlHdmiCec - SCS.doc
 *    \note    None.
 *
 *    HISTORY :
 *    \verbatim
 *     Date          Modified by CRPRNr  TASKNr  Maintenance description
 *     -------------|-----------|-------|-------|-----------------------------------
 *     04/07/2007   |  F.G      |       |       | Creation.
 *     -------------|-----------|-------|-------|-----------------------------------
 *    \endverbatim
 * */
/*========================================================================== */

#ifndef DLHDMICEC_FUNCTIONS_H
#define DLHDMICEC_FUNCTIONS_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#include "tmNxTypes.h"

/*============================================================================*/
/*                       PUBLIC FUNCTION DECLARATION                          */
/*============================================================================*/

/*========================================================================== */
/*!
 *    \brief This message is reserved for testing purposes
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *	\param u8 ReceiverLogicalAddress\n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_abort_message
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used by a new source to indicate that it has started
 *           to transmit a stream OR used in reponse to a <Request Active Source>
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 PhysicalAddress \n
 *           Physical address of the device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_active_source
(
    instance_t   instance,
    u16         physical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate the supported CEC version in response
 *           to a <Get CEC Version>
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *	\param u8 ReceiverLogicalAddress\n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECVersion_t CECVersion \n
 *           Supported CEC Version.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_version
(
    instance_t         instance,
    u8                receiver_logical_address,
    dl_hdmi_cecversion_t cecversion
);

/*========================================================================== */
/*!
 *    \brief This message is used to clear an Analogue timer block of a device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECAnalogueBroadcastType_t AnalogueBroadcastType \n
 *           "Cable,Sattellite,Terrestrial".\n
 *
 *    \param u16 AnalogueFrequency \n
 *           Specify frequency used by analogue tuner (0x0000<=N<=0xFFFF).\n
 *
 *    \param dlHdmiCECBroadcastSystem_t BroadcastSystem \n
 *           Specify information about the colour system, the sound carrier and
 *           the IF-frequency.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_clear_analogue_timer
(
    instance_t                        instance,
    u8                               receiver_logical_address,
    u8                               day_of_month,
    u8                               month_of_year,
    u16                              start_time,
    dl_hdmi_cecduration_t              *p_duration,
    u8                               recording_sequence,
    dl_hdmi_cecanalogue_broadcast_type_t  analogue_broadcast_type,
    u16                              analogue_frequency,
    dl_hdmi_cecbroadcast_system_t        broadcast_system
);

/*========================================================================== */
/*!
 *    \brief This message is used to clear a digital timer block of a device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECDigitalServiceIdentification_t *pServiceIdentification \n
 *           Pointer to the structure Digital Service Identification
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_clear_digital_timer
(
    instance_t                               instance,
    u8                                      receiver_logical_address,
    u8                                      day_of_month,
    u8                                      month_of_year,
    u16                                     start_time,
    dl_hdmi_cecduration_t                     *p_duration,
    u8                                      recording_sequence,
    dl_hdmi_cecdigital_service_identification_t *p_service_identification
);

/*========================================================================== */
/*!
 *    \brief This message is used to clear a digital timer block of a device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECExternalPlug_t  ExternalPlug \n
 *           indicates external plug number (1 to 255 )on the recording device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_clear_external_timer_with_external_plug
(
    instance_t                instance,
    u8                       receiver_logical_address,
    u8                       day_of_month,
    u8                       month_of_year,
    u16                      start_time,
    dl_hdmi_cecduration_t      *p_duration,
    u8                       recording_sequence,
    dl_hdmi_cecexternal_plug_t  external_plug
);

/*========================================================================== */
/*!
 *    \brief This message is used to clear a digital timer block of a device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECExternalPhysicalAddress_t  PhysicalAddress \n
 *           Defines the path between the TV an a device-thus giving it a physical
 *           address within the cluster.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_clear_external_timer_with_physical_address
(
    instance_t                           instance,
    u8                                  receiver_logical_address,
    u8                                  day_of_month,
    u8                                  month_of_year,
    u16                                 start_time,
    dl_hdmi_cecduration_t                 *p_duration,
    u8                                  recording_sequence,
    dl_hdmi_cecexternal_physical_address_t  external_physical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to conrol a device's media functions
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECDecControlMode_t DeckControlMode \n
 *           Used in message <Deck Control>\n
 *
 *    \note  The "Skip Forward / Wind" and "Skip Reverse / Rewind" values are
 *           used for example in a DVD as next xhapter and previous chapter and
 *           in a VCR as wind and rewind. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_deck_control
(
    instance_t                instance,
    u8                       receiver_logical_address,
    dl_hdmi_cecdec_control_mode_t deck_control_mode
);

/*========================================================================== */
/*!
 *    \brief This message is used to provide a deck's status to the initiator
 *           of the <Give Deck Status> message
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECDecInfo_t DeckInfo \n
 *           Information on the device's current status \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_deck_status
(
    instance_t         instance,
    u8                receiver_logical_address,
    dl_hdmi_cecdec_info_t deck_info
);

/*========================================================================== */
/*!
 *    \brief This message report the vendor ID of this device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u32 VendorID \n
 *           Indentifier for a specific Vendor \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_device_vendor_id
(
    instance_t instance,
    u32       vendor_id
);

/*========================================================================== */
/*!
 *    \brief This message is used as a reponse to indicate that the device does
 *           not support the requested message type, or that it cannot execute it
 *           at the present time.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECFeatureOpcode_t FeatureOpcode \n
 *           Opcode of the aborted message. \n
 *
 *    \param dlHdmiCECAbortReason_t   AbortReason \n
 *           The reason why message cannot respond. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_feature_abort
(
    instance_t               instance,
    u8                      receiver_logical_address,
    dl_hdmi_cecfeature_opcode_t feature_opcode,
    dl_hdmi_cecabort_reason_t   abort_reason
);

/*========================================================================== */
/*!
 *    \brief This message is used by a device to enquire which version of CEC
 *           the target supports
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_cec_version
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is sent by a device capable of character generation
 *           (for OSD and Menus) to a TV in order to discover the currently selected
 *           Menu Language. Also used by a TV during installation to dicover the
 *           currently set menu language of other devices.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_menu_language
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is requests an amplifier to send its volume and mute status
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_audio_status
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to request the status of a device regardless
 *           of whether or not it is the current active source.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECStatusRequest_t StatusRequest \n
 *           Allows the initiator to request the status once or on all future state
 *           change. Or to cancel a previous <Give Deck Status > ["On"] request. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_deck_status
(
    instance_t               instance,
    u8                      receiver_logical_address,
    dl_hdmi_cecstatus_request_t status_request
);

/*========================================================================== */
/*!
 *    \brief This message is used to determine the current power status of a
 *           target device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_device_power_status
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is request the vendor ID from a device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_device_vendor_id
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to request preferred OSD name of a device
 *           for use in menus associated with that device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_osd_name
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is a request to a device to return its physical Address
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_physical_address
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message request the status of the system audio mode
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_system_audio_mode_status
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to request the status of a tuner device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECStatusRequest_t StatusRequest \n
 *           Allows the initiator to request the status once or on all future state
 *           change. Or to cancel a previous <Give Tuner Device Status > ["On"] request. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_give_tuner_device_status
(
    instance_t               instance,
    u8                      receiver_logical_address,
    dl_hdmi_cecstatus_request_t status_request
);

/*========================================================================== */
/*!
 *    \brief This message sent by a source device to the TV whenever it enters
 *           the active state
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receivers. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_image_view_on
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used by the currently active source to inform the
 *           TV that it has no video to be presented to the user, or is going
 *           into standby as the result of a lcoal user command on the device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress, \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECExternalPhysicalAddress_t  PhysicalAddress \n
 *           Physical Address of the device. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_inactive_source
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    dl_hdmi_cecexternal_physical_address_t  physical_address
);

/*========================================================================== */
/*!
 *    \brief This message request from the TV for a device to show/remove a
 *           menu or to query if a device is currently showing a menu
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECMenuRequestType_t  MenuRequestType \n
 *           Indicates if the menu request is to activate or deactivate the
 *           devices menu or simply query the devices menu status. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_menu_request
(
    instance_t                  instance,
    u8                         receiver_logical_address,
    dl_hdmi_cecmenu_request_type_t  menu_request_type
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate to the TV that the device is
 *           showing/has removed a menu and requets the remote control keys to
 *           be passed though
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECMenuState_t  MenuState \n
 *           Indicates if the device is in the 'Device Menu Active' state or
 *           'Device Menu Inactive' state. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_menu_status
(
    instance_t            instance,
    u8                   receiver_logical_address,
    dl_hdmi_cecmenu_state_t  menu_state
);

/*========================================================================== */
/*!
 *    \brief This message is used to control the playback behaviour of a source
 *           device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECPlayMode_t  PlayMode \n
 *           In which mode to play media. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_play
(
    instance_t           instance,
    u8                  receiver_logical_address,
    dl_hdmi_cecplay_mode_t  play_mode
);

/*========================================================================== */
/*!
 *    \brief This message is used by any device for device discovery - similar to
 *           ping in other protocols
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_polling_message
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message request a device to stop a recording
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_off
(
    instance_t  instance,
    u8         receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message attempt to record analogue source
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECAnalogueBroadcastType_t AnalogueBroadcastType \n
 *           "Cable,Sattellite,Terrestrial".\n
 *
 *    \param u16 AnalogueFrequency \n
 *           Specify frequency used by analogue tuner (0x0000<=N<=0xFFFF).\n
 *
 *    \param dlHdmiCECBroadcastSystem_t BroadcastSystem \n
 *           Specify information about the colour system, the sound carrier and
 *           the IF-frequency.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_on_analogue_service
(
    instance_t                        instance,
    u8                               receiver_logical_address,
    dl_hdmi_cecanalogue_broadcast_type_t  analogue_broadcast_type,
    u16                              analogue_frequency,
    dl_hdmi_cecbroadcast_system_t        broadcast_system
);

/*========================================================================== */
/*!
 *    \brief This message attempt to record digital source
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECDigitalServiceIdentification_t *pServiceIdentification \n
 *           Pointer to the structure Digital Service Identification
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_on_digital_service
(
    instance_t                              instance,
    u8                                     receiver_logical_address,
    dl_hdmi_cecdigital_service_identification_t *p_service_identification
);

/*========================================================================== */
/*!
 *    \brief This message attempt to record an external physical address source
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECExternalPhysicalAddress_t  PhysicalAddress \n
 *           Defines the path between the TV an a device-thus giving it a physical
 *           address within the cluster.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_on_external_physical_address
(
    instance_t                         instance,
    u8                                receiver_logical_address,
    dl_hdmi_cecexternal_physical_address_t external_physical_address
);

/*========================================================================== */
/*!
 *    \brief This message attempt to record an external plug source
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECExternalPlug_t  ExternalPlug \n
 *           indicates external plug number (1 to 255 )on the recording device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_on_external_plug
(
    instance_t                   instance,
    u8                          receiver_logical_address,
    dl_hdmi_cecexternal_plug_t      external_plug
);

/*========================================================================== */
/*!
 *    \brief This message attempt to record an external plug source
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_on_own_source
(
    instance_t                   instance,
    u8                          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used by a recording device to inform the initiator
 *           of the message <Record On> about its status.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param dlHdmiCECRecordStatusInfo_t  RecordStatusInfo \n
 *           The recording status of the device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_status
(
    instance_t                   instance,
    u8                          receiver_logical_address,
    dl_hdmi_cecrecord_status_info_t  record_status_info
);

/*========================================================================== */
/*!
 *    \brief This message request by the recording device to record the presently
 *           displayed source.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_record_tv_screen
(
    instance_t  instance,
    u8         receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message report an amplifier's volume and mute.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECAudioStatus_t AudioStatus \n
 *           Volume and mute status. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_audio_status
(
    instance_t              instance,
    u8                     receiver_logical_address,
    ptmdl_hdmi_cecaudio_status_t p_audio_status
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportShortAudioDescriptor( )
 *    \brief This message Report Audio Capability.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u32 ShortAudioDecriptor \n
 *           Audio Descriptor. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_short_audio_descriptor
(
    instance_t   instance,
    u8          receiver_logical_address,
    u32         short_audio_decriptor
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRequestShortAudioDescriptor( )
 *    \brief This message Request Audio Capability.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8 AudioFormatID \n
 *
 *    \param u8 AudioFormatCode \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_request_short_audio_descriptor
(
    instance_t   instance,
    u8          receiver_logical_address,
    u8          audio_format_id,
    u8          audio_format_code

);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecIniateARC( )
 *    \brief This message Used by an ARC RX device to activate the
 *           ARC functionality in an ARC TX device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_iniate_arc
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportArcInitiated( )
 *    \brief This message Used by an ARC TX device to indicate that
 *           its ARC functionality has been activated
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_arc_initiated
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportArcTerminated( )
 *    \brief This message Used by an ARC TX device to indicate that its ARC functionality
 *           has been deactivated.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_arc_terminated
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRequestArcInitiation( )
 *    \brief This message Used by an ARC TX device to request an ARC RX device to
 *           activate the ARC functionality in the ARC TX device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_request_arc_initiation
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRequestArcTerminiation( )
 *    \brief Used by an ARC TX device to request an ARC RX device to deactivate
 *           the ARC functionality in the ARC TX device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_request_arc_terminiation
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTerminateARC( )
 *    \brief Used by an ARC TX device to request an ARC RX device to deactivate
 *           the ARC functionality in the ARC TX device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_terminate_arc
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to inform all other devices of the mapping
 *           between physical and logical address of the initiator.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 PhysicalAddress \n
 *           Device physical address within the cluster. \n
 *
 *    \param dlHdmiCECDeviceType_t DeviceType \n
 *           Type of the device (TV, Playback, tuner,...). \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_physical_address
(
    instance_t             instance,
    u16                   physical_address,
    dl_hdmi_cecdevice_type_t  device_type
);

/*========================================================================== */
/*!
 *    \brief This message is used to inform a requesting device of the current
 *           power status.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECPowerStatus_t PowerStatus \n
 *           Current power status. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_report_power_status
(
    instance_t             instance,
    u8                    receiver_logical_address,
    dl_hdmi_cecpower_status_t power_status
);

/*========================================================================== */
/*!
 *    \brief This message is used by a new device to discover the status of
 *           the system.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_request_active_source
(
    instance_t  instance
);

/*========================================================================== */
/*!
 *    \brief This message is sent by a CEC switch when it is manually switched to
 *           inform all other devices on the network that the active route below
 *           the switch has changed.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 OriginalAddress \n
 *           Previous address that the switch was switched to. \n
 *
 *    \param u16 NewAddress \n
 *           The new address it has been moved to. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_routing_change
(
    instance_t  instance,
    u16        original_address,
    u16        new_address
);

/*========================================================================== */
/*!
 *    \brief This message is sent by a CEC switch to indicate the active route
 *           below the switch.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 PhysicalAddress \n
 *           The current active route to the sink in the CEC switch. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_routing_information
(
    instance_t  instance,
    u16        physical_address
);

/*========================================================================== */
/*!
 *    \brief This message select directly an analogue TV Service.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECAnalogueBroadcastType_t AnalogueBroadcastType \n
 *           "Cable,Sattellite,Terrestrial".\n
 *
 *    \param u16 AnalogueFrequency \n
 *           Specify frequency used by analogue tuner (0x0000<=N<=0xFFFF).\n
 *
 *    \param dlHdmiCECBroadcastSystem_t BroadcastSystem \n
 *           Specify information about the colour system, the sound carrier and
 *           the IF-frequency.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_select_analogue_service
(
    instance_t                       instance,
    u8                              receiver_logical_address,
    dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type,
    u16                             analogue_frequency,
    dl_hdmi_cecbroadcast_system_t       broadcast_system
);

/*========================================================================== */
/*!
 *    \brief This message select directly a digital TV, Radio or Data Broadcast
 *           Service.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECDigitalServiceIdentification_t *pServiceIdentification \n
 *           Pointer to the structure Digital Service Identification
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_select_digital_service
(
    instance_t                                instance,
    u8                                       receiver_logical_address,
    ptmdl_hdmi_cecdigital_service_identification_t  p_service_identification
);

/*========================================================================== */
/*!
 *    \brief This message is used to set asingle timer block on an analogue
 *           recording device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECAnalogueBroadcastType_t AnalogueBroadcastType \n
 *           "Cable,Sattellite,Terrestrial".\n
 *
 *    \param u16 AnalogueFrequency \n
 *           Specify frequency used by analogue tuner (0x0000<=N<=0xFFFF).\n
 *
 *    \param dlHdmiCECBroadcastSystem_t BroadcastSystem \n
 *           Specify information about the colour system, the sound carrier and
 *           the IF-frequency.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_analogue_timer
(
    instance_t                        instance,
    u8                               receiver_logical_address,
    u8                               day_of_month,
    u8                               month_of_year,
    u16                              start_time,
    dl_hdmi_cecduration_t              *p_duration,
    u8                               recording_sequence,
    dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type,
    u16                              analogue_frequency,
    dl_hdmi_cecbroadcast_system_t       broadcast_system
);

/*========================================================================== */
/*!
 *    \brief This message is used to control audio rate from Source device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECAudioRate_t AudioRate \n
 *           The audio rate requested. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_audio_rate
(
    instance_t           instance,
    u8                  receiver_logical_address,
    dl_hdmi_cecaudio_rate_t audio_rate
);

/*========================================================================== */
/*!
 *    \brief This message is used to set a digital timer block on a digital
 *           recording device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECDigitalServiceIdentification_t *pServiceIdentification \n
 *           Pointer to the structure Digital Service Identification
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_digital_timer
(
    instance_t                               instance,
    u8                                     receiver_logical_address,
    u8                                      day_of_month,
    u8                                      month_of_year,
    u16                                     start_time,
    dl_hdmi_cecduration_t                     *p_duration,
    u8                                      recording_sequence,
    dl_hdmi_cecdigital_service_identification_t *p_service_identification
);

/*========================================================================== */
/*!
 *    \brief This message is used to set a single timer block to record from an
 *           external device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECExternalPlug_t  ExternalPlug \n
 *           indicates external plug number (1 to 255 )on the recording device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_external_timer_with_external_plug
(
    instance_t                instance,
    u8                       receiver_logical_address,
    u8                       day_of_month,
    u8                       month_of_year,
    u16                      start_time,
    dl_hdmi_cecduration_t      *p_duration,
    u8                       recording_sequence,
    dl_hdmi_cecexternal_plug_t  external_plug
);

/*========================================================================== */
/*!
 *    \brief This message is used to set a single timer block to record from an
 *           external device
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8  DayOfMonth \n
 *           Day of the month.\n
 *
 *    \param u8  MonthOfYear \n
 *           Month of the year.\n
 *
 *    \param u16 StartTime \n
 *           Start time for a timer based recording.\n
 *
 *    \param u16 Duration \n
 *           Pointer to the structure dlHdmiCECDuration_t in BCD format.\n
 *
 *    \param u8 Recording Sequence \n
 *           Indicates if recording is repeated and, if so, on which day
 *           For repeated recording the recording sequence value is the
 *           bitwise OR of the days when recordings are required
 *           Shall be set to 0x00 when recording is not repeated.\n
 *
 *    \param dlHdmiCECExternalPhysicalAddress_t  PhysicalAddress \n
 *           Defines the path between the TV an a device-thus giving it a physical
 *           address within the cluster.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_external_timer_with_physical_address
(
    instance_t                           instance,
    u8                                  receiver_logical_address,
    u8                                  day_of_month,
    u8                                  month_of_year,
    u16                                 start_time,
    dl_hdmi_cecduration_t                 *p_duration,
    u8                                  recording_sequence,
    dl_hdmi_cecexternal_physical_address_t  external_physical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used by a TV or another device to indicate the menu
 *           Language.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param const char *pLanguage \n
 *           Pointer on the user's menu language choice. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_menu_language
(
    instance_t instance,
    const char   *p_language
);

/*========================================================================== */
/*!
 *    \brief This message is used to set the preferred OSD name of a device
 *           for use in manus associated with that device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param const char *pOsdName \n
 *           Pointer on the preferred name of the device. \n
 *
 *    \param u8    OsdNameLength \n
 *           Length of Osd Name String. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_osd_name
(
    instance_t instance,
    u8        receiver_logical_address,
    const char   *p_osd_name,
    u8        osd_name_length
);

/*========================================================================== */
/*!
 *    \brief This message is used to send a test message to output on a TV.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECDisplayControl_t DisplayControl \n
 *           Display timing. \n
 *
 *    \param const char *pOsdString \n
 *           Pointer on the Text to display. \n
 *
 *    \param u8    OsdStringLength \n
 *           Length of Osd String. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_osd_string
(
    instance_t                 instance,
    u8                        receiver_logical_address,
    dl_hdmi_cecdisplay_control_t  display_control,
    const char                   *p_osd_string,
    u8                        osd_string_length
);

/*========================================================================== */
/*!
 *    \brief This message is used by a TV to request a streaming path from
 *           the specified physical address.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 PhysicalAddress \n
 *           Physical address of the device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_stream_path
(
    instance_t  instance,
    u16        physical_address
);

/*========================================================================== */
/*!
 *    \brief This message turn the system audio Mode ON or OFF.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECSystemAudioStatus_t SystemAudioStatus \n
 *           Specifies if the system audio mode is ON or OFF.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_system_audio_mode
(
    instance_t                   instance,
    u8                          receiver_logical_address,
    dl_hdmi_cecsystem_audio_status_t system_audio_status
);

/*========================================================================== */
/*!
 *    \brief This message is used to set the name of a program associated
 *           with a timer block.Sent directly after sending a
 *           <Set analogue Timer> or <Set Digital Timer> message. The name
 *           is then associated with that timer block.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \param const char *pProgramTitleString \n
 *           Pointer on the program title. \n
 *
 *    \param u8    ProgramTitleLength \n
 *           Length of Program Title String. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_timer_program_title
(
    instance_t instance,
    u8        receiver_logical_address,
    const char   *p_program_title_string,
    u8        program_title_length
);

/*========================================================================== */
/*!
 *    \brief This message switches one or all devices into standby mode.Can be
 *           be used as a broadcast message o be addressed to a specific device.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_standby
(
    instance_t instance,
    u8        receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief A device implementing System Audio Control and which has volume
 *           control RC button(eg TV or STB) request to use System Audio Mode
 *           to the amplifier.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u16 PhysicalAddress \n
 *           Physical address of the device.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_system_audio_mode_request
(
    instance_t  instance,
    u8         receiver_logical_address,
    u16        physical_address
);

/*========================================================================== */
/*!
 *    \brief Reports the current status of the System Audio Mode.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECSystemAudioStatus_t SystemAudioStatus \n
 *           Current system audio mode.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_system_audio_mode_status
(
    instance_t                   instance,
    u8                          receiver_logical_address,
    dl_hdmi_cecsystem_audio_status_t system_audio_status
);

/*========================================================================== */
/*!
 *    \brief This message as <Image View On>, but should also remove any text,
 *           menus and PIP windows from the TV's display
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_text_view_on
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to give the status of a <Cleared Analogue Timer>,
 *           <Clear Digital Timer> or <Clear External Timer> message.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECTimerClearedStatusData_t TimerClearedStatusData \n
 *           Indicates if the timer was cleared successfully. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_timer_cleared_status
(
    instance_t                        instance,
    u8                               receiver_logical_address,
    dl_hdmi_cectimer_cleared_status_data_t timer_cleared_status_data
);

/*========================================================================== */
/*!
 *    \brief This message is used to send timer status to the initiator of a
 *           <Set Timer> message.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECTimerStatusData_t *pTimerStatusData \n
 *           Pointer on the Timer status. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_timer_status
(
    instance_t                 instance,
    u8                        receiver_logical_address,
    dl_hdmi_cectimer_status_data_t *p_timer_status_data
);

/*========================================================================== */
/*!
 *    \brief This message is used by a tuner device to provide its status to the
 *           initiator of the <Give Tuner Device Status> message.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECRecordingFlag_t RecordingFlag \n
 *           Indicates if the tuner is being used as a source of a recording. \n
 *
 *    \param dlHdmiCECTunerDisplayInfo_t TunerDisplayInfo \n
 *           Indicates if the the device is currently deplaying its tuner or not. \n
 *
 *    \param dlHdmiCECAnalogueBroadcastType_t AnalogueBroadcastType \n
 *           "Cable,Sattellite,Terrestrial".\n
 *
 *    \param u16 AnalogueFrequency \n
 *           Specify frequency used by analogue tuner (0x0000<=N<=0xFFFF).\n
 *
 *    \param dlHdmiCECBroadcastSystem_t BroadcastSystem \n
 *           Specify information about the colour system, the sound carrier and
 *           the IF-frequency.\n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_tuner_device_status_analogue
(
    instance_t                       instance,
    u8                              receiver_logical_address,
    dl_hdmi_cecrecording_flag_t         recording_flag,
    dl_hdmi_cectuner_display_info_t      tuner_display_info,
    dl_hdmi_cecanalogue_broadcast_type_t analogue_broadcast_type,
    u16                             analogue_frequency,
    dl_hdmi_cecbroadcast_system_t       broadcast_system
);

/*========================================================================== */
/*!
 *    \brief This message is used by a tuner device to provide its status to the
 *           initiator of the <Give Tuner Device Status> message.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECRecordingFlag_t RecordingFlag \n
 *           Indicates if the tuner is being used as a source of a recording. \n
 *
 *    \param dlHdmiCECTunerDisplayInfo_t TunerDisplayInfo \n
 *           Indicates if the the device is currently deplaying its tuner or not. \n
 *
 *    \param dlHdmiCECDigitalServiceIdentification_t *pServiceIdentification \n
 *           Pointer to the structure Digital Service Identification
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_tuner_device_status_digital
(
    instance_t                               instance,
    u8                                      receiver_logical_address,
    dl_hdmi_cecrecording_flag_t                 recording_flag,
    dl_hdmi_cectuner_display_info_t              tuner_display_info,
    ptmdl_hdmi_cecdigital_service_identification_t p_service_identification
);

/*========================================================================== */
/*!
 *    \brief This message is used to tune to next lowest service in a tuner's
 *           service list.Can be used for PIP.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_tuner_step_decrement
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to tune to next highest service in a tuner's
 *           service list.Can be used for PIP.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_tuner_step_increment
(
    instance_t   instance,
    u8          receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECUserRemoteControlCommand_t UICommand \n
 *           Relevant UI command issued by user. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    dl_hdmi_cecuser_remote_control_command_t uicommand
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECPlayMode_t PlayMode \n
 *           In which mode to play media. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed_play
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    dl_hdmi_cecplay_mode_t                 play_mode
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 SelectAudioInput \n
 *           Number of the Audio Input (Audio input number between 1 and 255). \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed_select_audio_input
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    u8                                 select_audio_input
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 SelectAVInput \n
 *           Number of the A/V Input (A/V input number between 1 and 255). \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed_select_avinput
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    u8                                 select_avinput
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 SelectMedia \n
 *           Number of Media (Media number between 1 and 255). \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed_select_media
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    u8                                 select_media
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user pressed a remote button
 *           or switched from one remote control button to another.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param dlHdmiCECChannelIdentifier_t  *pChannelIdentifier \n
 *           Pointer to the structure of Major and Minor Channel number
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_pressed_tune
(
    instance_t                          instance,
    u8                                 receiver_logical_address,
    dl_hdmi_cecchannel_identifier_t        *p_channel_identifier
);

/*========================================================================== */
/*!
 *    \brief This message is used to indicate that the user released a remote button
 *           The last one indicated by the <User Control Pressed> Message.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_user_control_released
(
    instance_t  instance,
    u8         receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief This message is allows vendor specific commands to be sent between
 *           two devices.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8  *pVendorSpecificData \n
 *           Pointer to the Vendor Specific datas
 *
 *    \param u8    VendorSpecificDataLength \n
 *           Length of VendorSpecificData. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_vendor_command
(
    instance_t  instance,
    u8         receiver_logical_address,
    u8         *p_vendor_specific_data,
    u8         vendor_specific_data_length
);

/*========================================================================== */
/*!
 *    \brief This message is allows vendor specific commands to be sent between
 *           two devices or broadcast.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u32 VendorID \n
 *           Indentifier for a specific Vendor \n
 *
 *    \param u8  *pVendorSpecificData \n
 *           Pointer to the Vendor Specific datas
 *
 *    \param u8    VendorSpecificDataLength \n
 *           Length of VendorSpecificData. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_vendor_command_with_id
(
    instance_t  instance,
    u8         receiver_logical_address,
    u32        vendor_id,
    u8         *p_vendor_specific_data,
    u8         vendor_specific_data_length
);

/*========================================================================== */
/*!
 *    \brief This message indicates that a remote control button has been depressed.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8  *pVendorSpecificRcCode \n
 *           Pointer to the Vendor Specific remote control code.
 *           its recommended t keep this to a minimum size.
 *           The maximum length shall not exceed 14 data blocks to avoid saturating bus
 *
 *    \param u8    VendorSpecificRcCodeLength \n
 *           Length of VendorSpecificRcCode. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_vendor_remote_button_down
(
    instance_t  instance,
    u8         receiver_logical_address,
    u8         *p_vendor_specific_rc_code,
    u8         vendor_specific_rc_code_length
);

/*========================================================================== */
/*!
 *    \brief This message indicates that a remote control button (the last button
 *           pressed indicated by the <Vendor remote button down > message) has
 *           been released.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_vendor_remote_button_up
(
    instance_t  instance,
    u8         receiver_logical_address
);

/*========================================================================== */
/*!
 *    \brief Get the software version of the driver.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param pSWVersion Pointer to the version structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_swversion
(
    swversion_t  *p_swversion
);

/*========================================================================== */
/*!
 *    \brief Get the number of available CEC devices in the system.
 *           A unit directly represents a physical device.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param pUnitCount Pointer to the number of available units.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_number_of_units
(
    u32  *p_unit_count
);

/*========================================================================== */
/*!
 *    \brief Get the capabilities of unit 0. Capabilities are stored into a
 *           dedicated structure.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param pCapabilities Pointer to the capabilities structure.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_capabilities
(
    dl_hdmi_cec_capabilities_t *p_capabilities
);

/*========================================================================== */
/*!
 *    \brief Get the capabilities of a specific unit. Capabilities are stored
 *           into a dedicated structure
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param unit          Unit to be probed.
 *    \param pCapabilities Pointer to the capabilities structure.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_capabilities_m
(
    unit_select_t            unit,
    dl_hdmi_cec_capabilities_t *p_capabilities
);

/*========================================================================== */
/*!
 *    \brief Open unit 0 of CEC driver and provides the instance number to
 *           the caller. Note that one unit of CEC represents one physical
 *           CEC device and that only one instance per unit can be opened.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param pInstance Pointer to the variable that will receive the instance
 *                     identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMICEC_RESOURCE_OWNED: the resource is already in use
 *            - ERR_DLHDMICEC_INIT_FAILED: the unit instance is already
 *              initialised or something wrong happened at lower level.
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMICEC_NO_RESOURCES: the resource is not available
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_open
(
    instance_t   *p_instance
);

/*========================================================================== */
/*!
 *    \brief Open a specific unit of CEC driver and provides the instance
 *           number to the caller. Note that one unit of CEC represents one
 *           physical CEC device and that only one instance per unit can be
 *           opened.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param pInstance Pointer to the structure that will receive the instance
 *                     identifier.
 *    \param unit      Unit number to be opened.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMICEC_RESOURCE_OWNED: the resource is already in use
 *            - ERR_DLHDMICEC_INIT_FAILED: the unit instance is already
 *              initialised or something wrong happened at lower level.
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMICEC_NO_RESOURCES: the resource is not available
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_open_m
(
    instance_t   *p_instance,
    unit_select_t  unit
);

/*========================================================================== */
/*!
 *    \brief Close an instance of CEC driver.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_close
(
    instance_t   instance
);

/*========================================================================== */
/*!
 *    \brief Set the power state of an instance of the CEC device. ON
 *           state corresponds to a fully supplied, up and running device. Other
 *           modes correspond to the powerdown state of the device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance   Instance identifier.
 *    \param powerState Power state to set.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_power_state
(
    instance_t   instance,
    power_state_t power_state
);

/*========================================================================== */
/*!
 *    \brief Get the power state of an instance of the CEC device. ON
 *           state corresponds to a fully supplied, up and running device. Other
 *           modes correspond to the powerdown state of the device.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance    Instance identifier.
 *    \param pPowerState Pointer to the power state.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_power_state
(
    instance_t    instance,
    power_state_t *p_power_state
);

/*========================================================================== */
/*!
 *    \brief Set the configuration of instance attributes. This function is
 *           required by DVP architecture rules but actually does nothing in this
 *           driver
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param instance    Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_instance_config
(
    instance_t    instance
);

/*========================================================================== */
/*!
 *    \brief Setup the instance with its configuration parameters. This function
 *           allows basic instance configuration like Logical Address or device
 *           state.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance   Instance identifier.
 *    \param pSetupInfo Pointer to the structure containing all setup parameters
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_instance_setup
(
    instance_t                instance,
    ptmdl_hdmi_cec_instance_setup_t p_setup_info
);

/*========================================================================== */
/*!
 *    \brief Get instance setup parameters.
 *
 *    \param instance   Instance identifier.
 *    \param pSetupInfo Pointer to the structure that will receive setup
 *                      parameters
 *                      This function is synchronous.
 *                      This function is not ISR friendly.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_instance_setup
(
    instance_t                instance,
    ptmdl_hdmi_cec_instance_setup_t p_setup_info
);

/*========================================================================== */
/*!
 *    \brief Make device library handle an incoming interrupt. This function is
 *           used by application to tell the device library that the hardware
 *           sent an interrupt. It can also be used to poll the interrupt status
 *           of the device if the interrupt line is not physically connected to
 *           the CPU.
 *           This function is synchronous.
 *           This function is ISR friendly.
 *
 *    \param instance   Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_FULL: the queue is full
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_handle_interrupt
(
    instance_t    instance
);

/*========================================================================== */
/*!
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
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_INVALID_STATE: the state is invalid for
 *              the function
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_register_callbacks
(
    instance_t                instance,
    ptmdl_hdmi_cec_callback_func_t  pk_callback
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetLogicalAddress( )
 *    \brief Set Device Logical Address
 *
 *    \param instance Instance identifier.
 *    \param LogicalAddress Logical Address value.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_logical_address
(
    instance_t   instance,
    dl_hdmi_ceclogical_address_t   logical_address
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetRetry( )
 *    \brief Change the number of retransmission
 *
 *    \param instance Instance identifier.
 *    \param NbRetry Number of retry.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_retry
(
    instance_t   instance,
    u8   nb_retry
);

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t getCecLastMessage( )
 *    \brief Return the Addresses and the Opcode of the last CEC
 *	       transmitted message
 *
 *    \param pSaveMessage   Pointer to the CEC Save Message
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t get_cec_last_message
(
    dl_hdmi_cec_save_message_t  *p_save_message
);

/*========================================================================== */
/*!
 *    \brief This function allows enabling a specific event of devlib.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance Instance identifier.
 *    \param event    Event to enable
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_enable_event
(
    instance_t      instance,
    dl_hdmi_cec_event_t event
);

/*========================================================================== */
/*!
 *    \brief This function allows disabling a specific event of devlib.
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance Instance identifier.
 *    \param event    Event to disable
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_disable_event
(
    instance_t      instance,
    dl_hdmi_cec_event_t event
);

/*========================================================================== */
/*!
 *    \brief This function enables calibration depending on CEC clock source
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance          Instance identifier.
 *    \param cecClockSource    CEC clock source
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_enable_calibration
(
    instance_t instance,
    dl_hdmi_cec_clock_source_t cec_clock_source
);

/*========================================================================== */
/*!
 *    \brief This function disable calibration depending on CEC clock source
 *           This function is synchronous.
 *           This function is not ISR friendly.
 *
 *    \param instance          Instance identifier.
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_disable_calibration(
    instance_t instance
);

/*========================================================================== */
/*!
 *    \brief This function allow to send a generic CEC message
 *           This function has to be used when CEC messages are construct in
 *	   the middleware
 *
 *    \param instance  Instance identifier.
 *
 *    \param *pData    Pointer to the CEC data buffer
 *
 *    \param lenData   Lenght of I2C data buffer
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_send_message(

    instance_t instance,
    u8        *p_data,
    u16       len_data
);

unsigned char dl_hdmi_cec_get_register(instance_t   instance, u32 offset);
error_code_t dl_hdmi_cec_set_register(instance_t  instance, u32 offset, u32 value);

#endif /* DLHDMICEC_FUNCTIONS_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

