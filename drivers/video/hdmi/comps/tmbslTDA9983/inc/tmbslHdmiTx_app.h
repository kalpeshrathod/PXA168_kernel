/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          bsl_app.h
 *
 * \version       $Revision: 23 $
 *
 * \date          $Date: 10/10/07 11:11 $
 *
 * \brief         Application-level API for BSL driver component for the
 *                TDA998x HDMI Transmitter
 *
 * \section refs  Reference Documents
 * HDMI Driver - Outline Architecture.doc,
 * HDMI Driver - bsl - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History: bsl_app.h $
 *
 ******************  version 23  ****************
 * User: G. Burnouf     Date: 18/02/08
 * Updated in $/Source/bsl/Inc
 * PR1355 : Set audio channel allocation
 *
 * ****************  Version 22  ****************
 * User: B.Vereecke     Date: 10/10/07   Time: 11:11
 * Updated in $/Source/bsl/Inc
 * PR815 : Update bluescreen infoframes
 *         According to the output mode
 *
 * * **************  Version 21  *****************
 * User: B.Vereecke   Date: 30/08/07   Time: 14:45
 * Updated in $/Source/bsl/Inc
 * PR4   - add new appHdmiTx_ReadEdidAtPowerOn()
 *		   function, calling result after resuming
 *		   from power_down is an EDID reading.
 *
 * * **************  Version 20  *****************
 * User: B.Vereecke   Date: 20/07/07   Time: 17:30
 * Updated in $/Source/bsl/Inc
 * PR502 - Add new appHdmiTx_setAudio() function
 *		   for change Audio parameters without update
 *		   video input/output
 *
 *****************  version 19  ******************
 * User: B.Vereecke   Date: 19/07/07   Time: 10:30
 * Updated in $/Source/bsl/Inc
 * PR511 - add new PixelEdge argument in
 *			appHdmiTx_setInputOutput
 *
 * *****************  Version 18  ******************
 * User: B.Vereecke   Date: 17/07/07   Time: 10:30
 * Updated in $/Source/bsl/Inc
 * PR217 - Add Pattern type parameter to
 *         appHdmiTx_test_pattern_on API in order
 *         to add blue pattern functionality
 *
 * *****************  Version 17  ******************
 * User: J. Lamotte   Date: 29/06/07   Time: 15:50
 * Updated in $/Source/bsl/Inc
 * PR210 - Add video output format parameter to
 *         appHdmiTx_test_pattern_on API in order
 *         to update AVI info frame for color bar.
 *
 * *****************  Version 16  *****************
 * User: Mayhew       Date: 27/10/06   Time: 12:34
 * Updated in $/Source/bsl/Inc
 * PNF59 Test pattern API split into _on and _off APIs
 * PNF59 appHdmiTx_handleBCAPS has new pbBksvSecure parameter
 *
 * *****************  Version 14  *****************
 * User: Mayhew       Date: 13/10/06   Time: 11:02
 * Updated in $/Source/bsl/Inc
 * PNF37 appHdmiTx_setInputOutput syncIn arg. is now syncSource
 *
 * *****************  Version 12  *****************
 * User: Mayhew       Date: 15/09/06   Time: 15:56
 * Updated in $/Source/bsl/Inc
 * PNF19 Add i2sQualifier arg to setInputOutput
 * PNF25 Add pEdidVidFlags arg to handleHPD
 *
 * *****************  Version 11  *****************
 * User: Mayhew       Date: 10/07/06   Time: 12:33
 * Updated in $/Source/bsl/Inc
 * Add pbVerified parameter to _setInputOutput. Fix Doxygen comment
 * warnings.
 *
 * *****************  Version 9  *****************
 * User: Mayhew       Date: 30/06/06   Time: 12:42
 * Updated in $/Source/bsl/Inc
 * Add audioFmt parameter to appHdmiTx_setInputOutput
 *
 * *****************  Version 7  *****************
 * User: Mayhew       Date: 5/06/06    Time: 14:38
 * Updated in $/Source/bsl/Inc
 * Rename syncSrc to syncIn. Remove 2nd param from handleBCAPS. Add
 * pVidFmtNew param to appHdmiTx_nextEdidVidFmt.
 *
 * *****************  Version 5  *****************
 * User: Mayhew       Date: 22/05/06   Time: 15:55
 * Updated in $/Source/bsl/Inc
 * Add pixRate to appHdmiTx_setInputOutput. Add appHdmiTx_nextEdidVidFmt.
 *
 * *****************  Version 3  *****************
 * User: Mayhew       Date: 19/05/06   Time: 11:29
 * Updated in $/Source/bsl/Inc
 * Add options parameter to appHdmiTx_Hdcp_On
 *
 * *****************  Version 2  *****************
 * User: Mayhew       Date: 10/05/06   Time: 17:01
 * Updated in $/Source/bsl/Inc
 * New APIs for HDCP and parameterised format setting
 *
 * *****************  Version 1  *****************
 * User: Mayhew       Date: 4/04/06    Time: 16:27
 * Created in $/Source/bsl/Inc
 * Driver demo app API phase 2
 *
 *   \endverbatim
 *
 * */

#ifndef BSLHDMITX_APP_H
#define BSLHDMITX_APP_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

/*============================================================================*/
/*                       MACRO DEFINITIONS                                    */
/*============================================================================*/

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                       EXTERN DATA DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/*============================================================================*/
/**
 *    \brief      Initialise demo application
 * */
void
app_hdmi_tx_init(void);

/*============================================================================*/
/**
 *    \brief      Set colourbar test pattern on with RGB infoframe
 *
 *    \param[in]  txUnit      Transmitter unit number
 *    \param[in]  voutFmt     Video output format
 *	\param[in]  pattern		type of pattern
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_test_pattern_on
(
    unit_select_t				tx_unit,
    bsl_vid_fmt_t			vout_fmt,
    bsl_vout_mode_t       vout_mode,
    bsl_test_pattern_t	pattern
);

/*============================================================================*/
/**
 *    \brief      Set colourbar test pattern off with previous infoframe
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  voutFmt      Video output format
 *    \param[in]  voutMode     Video output mode
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_test_pattern_off
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vout_fmt,
    bsl_vout_mode_t   vout_mode
);

/*============================================================================*/
/**
 *    \brief      Set input and output formats, modes, sync source, sink type
 *                and audio rate
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  vinFmt       Video input format
 *    \param[in]  vinMode      Video input mode
 *    \param[in]  voutFmt      Video output format
 *    \param[in]  voutMode     Video output mode
 *    \param[in]  syncSource   Video input sync source
 *    \param[in]  sinkType     Downstream receiver sink type
 *    \param[in]  audioFmt     Audio format
 *    \param[in]  audioRate    Audio sample rate
 *    \param[in]  i2sQualifier Audio I2S qualifier: 8=channels; 16,32=bits
 *    \param[in]  pixRate      Pixel rate
 *	\param[in]  pixelEdge    Pixel edge
 *    \param[out] pbVerified   Pointer to flag set when the requested combination
 *                             of formats and modes has previously been verified
 *                             during testing
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_set_input_output
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vin_fmt,
    bsl_vin_mode_t    vin_mode,
    bsl_vid_fmt_t     vout_fmt,
    bsl_vout_mode_t   vout_mode,
    bsl_sync_source_t sync_source,
    bsl_sink_type_t   sink_type,
    bsla_fmt_t       audio_fmt,
    bslafs_t        audio_rate,
    u8                   i2s_qualifier,
    bsl_pix_rate_t    pix_rate,
    bsl_pix_edge_t    pixel_edge,
    /* Returns true if requested combination
     *     bool                    *pb_verified
     * has been verified */
);

/*============================================================================*/
/**
 *    \brief      Set audio format and audio rate
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \param[in]  sinkType     Downstream receiver sink type
 *    \param[in]  audioFmt     Audio format
 *    \param[in]  audioRate    Audio sample rate
 *    \param[in]  i2sQualifier Audio I2S qualifier: 8=channels; 16,32=bits
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_set_audio
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vout_fmt,
    bsl_sink_type_t   sink_type,
    bsla_fmt_t       audio_fmt,
    bslafs_t        audio_rate,
    u8                   i2s_qualifier
);

/*============================================================================*/
/**
 *    \brief      Set audio channel allocation
 *
 *    \param[in]  ChannelAllocation     audio channel allocation
 *
 *    \return     The call result:
 *                - void
 * */
void
app_hdmi_tx_set_audio_channel_allocation
(
    u8					channel_allocation
);

/*============================================================================*/
/**
 *    \brief      Switch on HDCP
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[in]  voutFmt      Current CEA output format
 *    \param[in]  options      HDCP options (HDMITX_HDCP_OPTION_FORCE_ values)
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_hdcp_on
(
    unit_select_t           tx_unit,
    bsl_vid_fmt_t      vout_fmt,
    bsl_hdcp_options_t options
);

/*============================================================================*/
/**
 *    \brief      Switch off HDCP
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_hdcp_off
(
    unit_select_t      tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP BCAPS interrupt as a callback
 *
 *    \param[in]  txUnit       Transmitter unit number
 *    \param[out] pbBksvSecure BKSV check result
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_bcaps
(
    unit_select_t tx_unit,
    bool *pb_bksv_secure
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP BSTATUS interrupt as a callback
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_bstatus
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP ENCRYPT interrupt as a callback
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_encrypt
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP PJ interrupt as a callback
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_pj
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP SHA_1 interrupt as a callback
 *
 *    \param[in]  txUnit       Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_sha_1
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle HDCP T0 interrupt as a callback
 *
 *    \param[in]  txUnit      Transmitter unit number
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_t0
(
    unit_select_t  tx_unit
);

/*============================================================================*/
/**
 *    \brief      Handle Hot Plug Detect interrupt as a callback
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pHotPlugStatus  Pointer to hot plug status
 *    \param[out] pEdidStatus     Copy of pDis->EdidStatus
 *    \param[out] pEdidVidFlags   Ptr to video capability flags
 *                                See enum _bslVidCap_t
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_handle_hpd
(
    unit_select_t          tx_unit,
    bsl_hot_plug_t    *p_hot_plug_status,
    u8                   *p_edid_status,
    u8                   *p_edid_vid_flags
);

/*============================================================================*/
/**
 *    \brief      Get the pointer to the KSV list used by the App unit
 *
 *    \param[in]  ppKsvList      Pointer to pointer to KSV list
 *    \param[in]  pKsvDevices    Pointer to device count 0 to 128
 *
 *    \return     None
 * */
void
app_hdmi_tx_get_ksv_list
(
    u8          **pp_ksv_list,
    u8          *p_ksv_devices
);

/*============================================================================*/
/**
 *    \brief      Get the pointer to the BKSV used by the App unit
 *
 *    \param[in]  ppBksv      Pointer to pointer to BKSV
 *
 *    \return     None
 * */
void
app_hdmi_tx_get_bksv
(
    u8          **pp_bksv
);

/*============================================================================*/
/**
 *    \brief      Check a video format against the Short Video
 *                Descriptors last read from EDID
 *
 *    \param[in]  vidFmt  Video format to check
 *
 *    \return     The call result:
 *                - true:  vidFmt is in SVD list
 *                - false: not in list
 * */
bool
app_hdmi_tx_check_vid_fmt
(
    bsl_vid_fmt_t     vid_fmt
);

/*============================================================================*/
/**
 *    \brief      Find the next video format in the Short Video Descriptors list
 *                last read from the EDID
 *
 *    \param[in]  vidFmtOld      Video format whose successor must be found
 *    \param[in]  *pVidFmtNew    Ptr to a variable to receive the next higher
 *                               video format or the first format in SVD list
 *
 *    \return     The call result:
 *                - true:  a format was found
 *                - false: no format was found because the list was empty
 * */
bool
app_hdmi_tx_next_edid_vid_fmt
(
    bsl_vid_fmt_t     vid_fmt_old,
    bsl_vid_fmt_t     *p_vid_fmt_new
);

/*============================================================================*/
/**
 *    \brief      Read Edid after PowerOn
 *
 *    \param[in]  txUnit          Transmitter unit number
 *    \param[out] pEdidStatus     Copy of pDis->EdidStatus
 *    \param[out] pEdidVidFlags   Ptr to video capability flags
 *                                See enum _bslVidCap_t
 *
 *    \return     The call result:
 *                - 0: the call was successful
 *                - Else a problem has been detected
 * */
error_code_t
app_hdmi_tx_read_edid_at_power_on
(
    unit_select_t          tx_unit,
    u8                   *p_edid_status,
    u8                   *p_edid_vid_flags
);

/*============================================================================*/
/**
 *    \brief      set revocation list
 *
 *    \param[in]  listPtr                 pointer on revocation list
 *    \param[in]  Length                  number of bksv in revocation list
 * */
error_code_t
app_hdmi_tx_set_hdcprevocation_list
(
    void           *list_ptr,
    u32          length
);

#endif /* BSLHDMITX_APP_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

