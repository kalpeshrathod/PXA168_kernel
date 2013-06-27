/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          bsl_2.c
 *
 * \version       $Revision: 29 $
 *
 * \date          $Date: 29/10/07 14:11 $
 *
 * \brief         BSL driver component for the TDA9983 HDMI Transmitter
 *
 * \section refs  Reference Documents
 * HDMI Driver - Outline Architecture.doc
 * HDMI Driver - bsl - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *   $History: bsl_2.c $
 *
 * *******************  Version 29  *****************
 * User: B.Vereecke     Date: 29/10/07   Time: 14:11
 * Updated in $/Source/bsl/src
 * PR852 : remove external library dependancy
 *
 * * *****************  Version 28  *****************
 * User: B.Vereecke     Date: 17/10/07   Time: 14:11
 * Updated in $/Source/bsl/src
 * PR872 : add new formats, 1080p24/25/30
 *
 * * *****************  Version 27  *****************
 * User: J. Lamotte      Date: 01/06/07   Time: 12:00
 * Updated in $/Source/bsl/src
 * PR359 (PR176) : Modify SetHwRegisters call in
 *				bslMatrixSetConversion API
 *
 * * *****************  Version 26  *****************
 * User: J. Lamotte      Date: 14/05/07   Time: 10:30
 * Updated in $/Source/bsl/src
 * PR322 (PR176) : Remove 3 DLL compilation warnings
 *                 (calculateCheckusm, getEdidBlock,
 *                 bslpktSetVsInfoframe)
 *
 * * *****************  Version 25  *****************
 * User: Burnouf         Date: 18/04/07   Time: 13:25
 * Updated in $/Source/bsl/src
 * PR50 : Send event EV_SINKON for TDA9981
 *
 * *****************  Version 24  *****************
 * User: Burnouf      Date: 29/11/06   Time: 17:06
 * Updated in $/Source/bsl/Src
 * PNF79 and PR11 allow driver to go in state connected if EDID checksum
 * is bad
 *
 * *****************  Version 23  *****************
 * User: Mayhew       Date: 23/11/06   Time: 15:06
 * Updated in $/Source/bsl/Src
 * PNF78 Ensure that DDC is disconnected from I2C after bad EDID read
 *
 * *****************  Version 21  *****************
 * User: Mayhew       Date: 10/11/06   Time: 10:11
 * Updated in $/Source/bsl/Src
 * PNF68 RETIF_REG_FAIL macro replaces RETIF checks after register set/get
 * calls, and compiled out to save code space
 *
 * *****************  Version 19  *****************
 * User: Mayhew       Date: 2/11/06    Time: 16:54
 * Updated in $/Source/bsl/Src
 * Remove N3 support
 *
 * *****************  Version 17  *****************
 * User: Mayhew       Date: 21/09/06   Time: 15:49
 * Updated in $/Source/bsl/Src
 * Cut code size in demo by using RETIF_BADPARAM
 *
 * *****************  Version 15  *****************
 * User: Djw          Date: 22/08/06   Time: 9:56
 * Updated in $/Source/bsl/Src
 * Updated file configuration info.
 *
 * *****************  Version 14  *****************
 * User: Mayhew       Date: 10/07/06   Time: 13:09
 * Updated in $/Source/bsl/Src
 * Fix file header comment
 *
 * *****************  Version 12  *****************
 * User: Mayhew       Date: 30/06/06   Time: 13:26
 * Updated in $/Source/bsl/Src
 * EdidSinkType replaced by SinkType. Set full colourspace for VGA format
 * 1 as well as for PC formats.
 *
 * *****************  Version 10  *****************
 * User: Djw          Date: 16/06/06   Time: 12:04
 * Updated in $/Source/bsl/Src
 * Added use of alternate i2c address for EDID.  More conditional
 * compilation for demoboard build.
 *
 * *****************  Version 8  *****************
 * User: Mayhew       Date: 5/06/06    Time: 16:39
 * Updated in $/Source/bsl/Src
 * Save code space by replacing API unit checks with checkUnitSetDis.
 *
 * *****************  Version 7  *****************
 * User: Djw          Date: 24/05/06   Time: 15:30
 * Updated in $/Source/bsl/Src
 * Minor change to force EDID reads of block 0 or 1 to occur without
 * segment pointer - thus keeping compatibility with older EDIDs.
 *
 * *****************  Version 5  *****************
 * User: Djw          Date: 24/05/06   Time: 11:19
 * Updated in $/Source/bsl/Src
 * Added conditional compilation for demoboard build.  Added N4 EDID
 * capability with Seg Ptr writing.  Added all InfoFrames with N4 version
 * checking; InfoFrames use page 10h on N4.
 *
 * *****************  Version 4  *****************
 * User: Mayhew       Date: 10/05/06   Time: 17:08
 * Updated in $/Source/bsl/Src
 * Rename local E_ enums to public HDMITX_ enums
 *
 * *****************  Version 3  *****************
 * User: Djw          Date: 20/04/06   Time: 18:03
 * Updated in $/Source/bsl/Src
 * Fixed logic problem with EDID block parsing on block 1 upwards
 * (checksum test).  Added workaround for EDID ghost register problem on
 * n3 device.
 *
 * *****************  Version 2  *****************
 * User: Mayhew       Date: 11/04/06   Time: 14:09
 * Updated in $/Source/bsl/Src
 * Fixed Bad Parameter error in MatrixSetConversion and fixed wrong block
 * version in video infoframe.
 *
 * *****************  Version 1  *****************
 * User: Mayhew       Date: 4/04/06    Time: 16:28
 * Created in $/Source/bsl/Src
 * Driver phase 2
 *   \endverbatim
 *
 * */

/*============================================================================*/
/*                   FILE CONFIGURATION                                       */
/*============================================================================*/

/* Defining this symbol on the compiler command line excludes some API checks */
/* #define NO_RETIF_BADPARAM */

/* Defining this symbol on the compiler command line excludes some API checks */
/* #define NO_RETIF_REG_FAIL */

/* Defining this symbol on the compiler command line excludes unused code */
/* #define DEMO_BUILD */

/*============================================================================*/
/*                   STANDARD INCLUDE FILES                                   */
/*============================================================================*/

/*============================================================================*/
/*                   PROJECT INCLUDE FILES                                    */
/*============================================================================*/
#include "tmbslHdmiTx.h"
#include "tmbslHdmiTx_local.h"

/*============================================================================*/
/*                   MACRO DEFINITIONS                                        */
/*============================================================================*/
#define EDID_NUMBER_MAX_DTD_BLK_1           6
/** EDID block 0 parse start point */
#define EDID_BLK0_BASE_DTD                  0x36

#define EDID_BLK1_OFFSET_BASE_DTD           2

/** EDID block 0 extension block count */
#define EDID_BLK0_EXT_CNT                   0x7E

/** EDID extension block parse start point */
#define EDID_BLK_EXT_BASE                   0x04

/** CEA extension block type */
#define EDID_CEA_EXTENSION                  0x02

/** CEA Block Map */
#define EDID_BLOCK_MAP                      0xF0

/** NB Max of descriptor DTD or monitor in block 0 */
#define EDID_NB_MAX_DESCRIP_BLK_IN_BLK_0    4

#define EDID_MONITOR_NAME_DESC_DATA_TYPE    252

#define EDID_MONITOR_RANGE_DESC_DATA_TYPE   253

/*============================================================================*/
/*                   TYPE DEFINITIONS                                         */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC VARIABLE DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLE DECLARATIONS                             */
/*============================================================================*/

/**
 *  Lookup table for colour space conversion matrix register sets.
 *  Each array consists of 31 register values from MAT_CONTROL through
 *  to MAT_OO3_LSB
 * */
static CONST_DAT u8 k_matrix_preset[MATRIX_PRESET_QTY][MATRIX_PRESET_SIZE] = {
	{
		0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x6F, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x3, 0x6F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3,
		0x6F, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40
	},  /* RGB Full to RGB Limited */

	{
		0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x4, 0x1, 0x7, 0x0,
		0x64, 0x6, 0x88, 0x1, 0xC2, 0x7, 0xB7, 0x6, 0xD6, 0x7, 0x68, 0x1,
		0xC2, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	},  /* RGB Full to BT601 */

	{
		0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x75, 0x0, 0xBB, 0x0,
		0x3F, 0x6, 0x68, 0x1, 0xC2, 0x7, 0xD7, 0x6, 0xA6, 0x7, 0x99, 0x1,
		0xC2, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	},  /* RGB Full to BT709 */

	{
		0x1, 0x7, 0xC0, 0x7, 0xC0, 0x7, 0xC0, 0x2, 0x54, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x2, 0x54, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2,
		0x54, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	},  /* RGB Limited to RGB Full */

	{
		0x2, 0x7, 0xC0, 0x7, 0xC0, 0x7, 0xC0, 0x2, 0x59, 0x1, 0x32, 0x0,
		0x75, 0x6, 0x4A, 0x2, 0x0C, 0x7, 0xAB, 0x6, 0xA5, 0x7, 0x4F, 0x2,
		0x0C, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	},  /* RGB Limited to BT601 */

	{
		0x2, 0x7, 0xC0, 0x7, 0xC0, 0x7, 0xC0, 0x2, 0xDC, 0x0, 0xDA, 0x0,
		0x4A, 0x6, 0x24, 0x2, 0x0C, 0x7, 0xD0, 0x6, 0x6C, 0x7, 0x88, 0x2,
		0x0C, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	},  /* RGB Limited to BT709 */

	{
		0x0, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x1, 0x2A, 0x7, 0x30, 0x7,
		0x9C, 0x1, 0x2A, 0x1, 0x99, 0x0, 0x0, 0x1, 0x2A, 0x0, 0x0, 0x2,
		0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	},  /* BT601 to RGB Full */

	{
		0x1, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x2, 0x0, 0x6, 0x9A, 0x7,
		0x54, 0x2, 0x0, 0x2, 0xBE, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x3,
		0x77, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40
	},  /* BT601 to RGB Limited */

	{
		0x1, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x2, 0x0, 0x7, 0x96, 0x7,
		0xC5, 0x0, 0x0, 0x2, 0x0D, 0x0, 0x26, 0x0, 0x0, 0x0, 0x3B, 0x2,
		0x0A, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	},  /* BT601 to BT709 */

	{
		0x0, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x1, 0x2A, 0x7, 0x77, 0x7,
		0xC9, 0x1, 0x2A, 0x1, 0xCB, 0x0, 0x0, 0x1, 0x2A, 0x0, 0x0, 0x2,
		0x1D, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	},  /* BT709 to RGB Full */

	{
		0x1, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x2, 0x0, 0x7, 0x16, 0x7,
		0xA2, 0x2, 0x0, 0x3, 0x14, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x3,
		0xA1, 0x0, 0x40, 0x0, 0x40, 0x0, 0x40
	},  /* BT709 to RGB Limited */

	{
		0x1, 0x7, 0xC0, 0x6, 0x0, 0x6, 0x0, 0x2, 0x0, 0x0, 0x62, 0x0,
		0x33, 0x0, 0x0, 0x1, 0xF7, 0x7, 0xDB, 0x0, 0x0, 0x7, 0xC7, 0x1,
		0xFB, 0x0, 0x40, 0x2, 0x0, 0x2, 0x0
	}  /* BT709 to BT601 */
};

/**
 *  This table gives us the index into the kMatrixPreset array, based
 *  on the input and output colourspaces.
 *  The co-ordinates into this array are bslColourspace_t enums.
 *  The value of -1 is returned for matching input/output colourspaces.
 * */
static CONST_DAT int k_matrix_index[HDMITX_CS_NUM][HDMITX_CS_NUM] = {
	{ -1, E_MATRIX_RGBF_2_RGBL, E_MATRIX_RGBF_2_BT601, E_MATRIX_RGBF_2_BT709},
	{E_MATRIX_RGBL_2_RGBF, -1, E_MATRIX_RGBL_2_BT601, E_MATRIX_RGBL_2_BT709},
	{E_MATRIX_BT601_2_RGBF, E_MATRIX_BT601_2_RGBL, -1, E_MATRIX_BT601_2_BT709},
	{E_MATRIX_BT709_2_RGBF, E_MATRIX_BT709_2_RGBL, E_MATRIX_BT709_2_BT601, -1}
};

/*============================================================================*/
/*                   STATIC FUNCTION DECLARATIONS                             */
/*============================================================================*/
static u8            calculate_checksum(u8 *p_data, int num_bytes);
static error_code_t    get_edid_block(hdmi_txobject_t *p_dis,
                                      int block_number);
static error_code_t    parse_edid_block(hdmi_txobject_t *p_dis,
                                        int block_number);

static bool             store_dtd_block(hdmi_txobject_t    *p_dis,
                                        u8               block_ptr);

static bool             store_monitor_descriptor(hdmi_txobject_t    *p_dis,
        u8                block_ptr);

/*============================================================================*/
/*                   PUBLIC FUNCTION DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/* bslEdidGetAudioCapabilities                                        */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_edid_get_audio_capabilities
(
    unit_select_t          tx_unit,
    bsl_edid_sad_t    *p_edid_afmts,
    uint                    a_fmt_length,
    uint                    *p_afmts_avail,
    u8                   *p_audio_flags
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	uint                i;          /* Loop index */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_edid_afmts == NULL)
	RETIF_BADPARAM(a_fmt_length < 1)
	RETIF_BADPARAM(p_afmts_avail == NULL)
	RETIF_BADPARAM(p_audio_flags == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	/* Copy the Device Instance Structure EdidAFmts descriptors to
	 * pEdidAFmts until we run out or no more space in structure.
	 *	 */
	if(p_dis->edid_sad_cnt > 0) {
		for(i = 0; (i < (uint)p_dis->edid_sad_cnt) && (i < a_fmt_length); i++) {
			p_edid_afmts[i].mode_chans = p_dis->edid_afmts[i].mode_chans;
			p_edid_afmts[i].freqs     = p_dis->edid_afmts[i].freqs;
			p_edid_afmts[i].byte3     = p_dis->edid_afmts[i].byte3;
		}
	} else {
		/* No pEdidAFmts to copy so set a zero format to be safe */
		p_edid_afmts[0].mode_chans = 0;
		p_edid_afmts[0].freqs     = 0;
		p_edid_afmts[0].byte3     = 0;
	}

	/* Fill Audio Flags parameter */
	*p_audio_flags = ((p_dis->edid_cea_flags & 0x40) << 1);  /* Basic audio */
	if(p_dis->edid_sink_ai == true) {
		/* Mask in AI support */
		*p_audio_flags += 0x40;
	}

	/* Fill number of SADs available parameter */
	*p_afmts_avail = p_dis->edid_sad_cnt;

	return 0;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslEdidGetBlockCount                                               */
/*============================================================================*/
error_code_t
bsl_edid_get_block_count
(
    unit_select_t      tx_unit,
    u8               *pu_edid_block_count
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(pu_edid_block_count == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	*pu_edid_block_count = p_dis->edid_block_cnt;

	return 0;
}

/*============================================================================*/
/* bslEdidGetBlockData                                                */
/*============================================================================*/
error_code_t
bsl_edid_get_block_data
(
    unit_select_t  tx_unit,
    u8           *p_raw_edid,
    int             num_blocks,  /* Only relevant if pRawEdid valid */
    int             len_raw_edid, /* Only relevant if pRawEdid valid */
    u8           *p_edid_status
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	int                 i;          /* Loop index */
	u8               ext_block_cnt;   /* Count of extension blocks */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s)
	 * We do allow a null pRawEdid pointer, in which case buffer length is
	 * irrelevant. If pRawEdid pointer is valid, there is no point in
	 * continuing if insufficient space for at least one block.
	 *	 */
	RETIF_BADPARAM((p_raw_edid != NULL) && (len_raw_edid < EDID_BLOCK_SIZE))
	/* Sensible value of numBlocks? */
	RETIF((p_raw_edid != NULL) && ((num_blocks < 1) || (num_blocks > 255)),
	      ERR_HDMI_INCONSISTENT_PARAMS)
	/* Enough space for the data requested? */
	RETIF((p_raw_edid != NULL) && (len_raw_edid < (num_blocks * EDID_BLOCK_SIZE)),
	      ERR_HDMI_INCONSISTENT_PARAMS)
	RETIF_BADPARAM(p_edid_status == NULL)

	/* Reset the EdidStatus in the Device Instance Structure */
	p_dis->edid_status = HDMITX_EDID_NOT_READ;

	/* Reset stored parameters from EDID in the Device Instance Structure */
	p_dis->edid_sink_type = HDMITX_SINK_DVI;
	p_dis->edid_sink_ai = false;
	p_dis->edid_cea_flags = 0;
	p_dis->edid_svd_cnt = 0;
	p_dis->edid_sad_cnt = 0;
	p_dis->edid_source_address = 0;                /* 0.0.0.0 */
	p_dis->nb_dtdstored = 0;
	p_dis->edid_first_monitor_descriptor.b_desc_record = false;
	p_dis->edid_second_monitor_descriptor.b_desc_record = false;
	p_dis->edid_other_monitor_descriptor.b_desc_record = false;

	/* Read the HPD pin via the hpd_in flag in the first interrupt status
	 * register and return a ERR_HDMI_NULL_CONNECTION error if it is
	 * not set.
	 * We must use the flag in the Device Instance Structure to avoid
	 * clearing pending interrupt flags.
	 *	 */
	RETIF(p_dis->hot_plug_status != HDMITX_HOTPLUG_ACTIVE,
	      ERR_HDMI_NULL_CONNECTION)

	/* Get the first EDID block into Device Instance workspace */
	err = get_edid_block(p_dis, 0);
	/* PR11 : On i2c error or bad checksum in block 0 */
	/* allow driver to go in state CONNECTED */

	if(err != 0) {
		set_state(p_dis, EV_GETBLOCKDATA);
#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON
		if(p_dis->rx_sense_status == HDMITX_RX_SENSE_ACTIVE) {
			set_state(p_dis, EV_SINKON);
		}
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */
		return err;
	}

	/* Indicate that status OK so far */
	p_dis->edid_status = HDMITX_EDID_READ;

	/* If pointer present, copy block from workspace.  We know from the
	 * paramenter checking on entry that at least one block is required
	 * and we have the space for it.
	 *	 */
	if(p_raw_edid != NULL) {
		lmemcpy(p_raw_edid, p_dis->edid_block, EDID_BLOCK_SIZE);
	}

	/* Could check block 0 header (0x00,6 x 0xFF,0x00) here but not
	 * certain to be future proof [CEA861C A.2.3]
	 *	 */

	/* Read block count from penultimate byte of block and store in DIS */
	ext_block_cnt = p_dis->edid_block[EDID_BLK0_EXT_CNT];
	/* (For N3, used to have to limit extBlockCnt to one block) */
	/* Total = Block 0 + extensions */
	p_dis->edid_block_cnt = ext_block_cnt + 1;

	/* Parse block 0 */
	parse_edid_block(p_dis, 0);

	/* If extension blocks are present, process them */
	if(ext_block_cnt > 0) {
		for(i = 0; i < ext_block_cnt; i++) {
			/* read block */
			err = get_edid_block(p_dis, i + 1);
			/* On this occasion, we also accept INVALID_STATE which means
			 * there was a checksum error
			 *			 */
			if((err != 0) && (err != ERR_HDMI_INVALID_STATE)) {
				/* PR11 : allow driver to go in state CONNECTED */
				set_state(p_dis, EV_GETBLOCKDATA);
#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON
				if(p_dis->rx_sense_status == HDMITX_RX_SENSE_ACTIVE) {
					set_state(p_dis, EV_SINKON);
				}
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */
				return err;
			}

			/* If pointer was supplied, copy block from DIS to buffer */
			if(p_raw_edid != NULL) {
				/* Check if we've copied as many as requested yet? */
				if((i + 2) <= num_blocks) {
					lmemcpy(p_raw_edid + ((i + 1) * EDID_BLOCK_SIZE),
					        p_dis->edid_block,
					        EDID_BLOCK_SIZE);
					if(err == ERR_HDMI_INVALID_STATE) {
						/* Note checksum error in EdidStatus */
						p_dis->edid_status = HDMITX_EDID_ERROR;
					}
				} else { /* Fewer blocks requested than EDID contains, warn */
					if(p_dis->edid_status == HDMITX_EDID_ERROR) {
						p_dis->edid_status = HDMITX_EDID_ERROR_INCOMPLETE;
					}
				}
			}
			/* If the checksum was OK, we can parse the block */
			if(err == 0) {
				parse_edid_block(p_dis, i + 1);
			}
		}
	}

	/* Copy return value from EdidStatus */
	*p_edid_status = p_dis->edid_status;

	/* Filter out buffer status from the EdidStatus value in the
	 * Device Instance Structure
	 *	 */
	if((p_dis->edid_status == HDMITX_EDID_ERROR) ||
	   (p_dis->edid_status == HDMITX_EDID_ERROR_INCOMPLETE)) {
		p_dis->edid_status = HDMITX_EDID_ERROR;
	}

	set_state(p_dis, EV_GETBLOCKDATA);

#ifdef TMFL_TDA9981_SUPPORT
#ifdef TMFL_RX_SENSE_ON
	if(p_dis->rx_sense_status == HDMITX_RX_SENSE_ACTIVE) {
		set_state(p_dis, EV_SINKON);
	}
#endif /* TMFL_RX_SENSE_ON */
#endif /* TMFL_TDA9981_SUPPORT */

	return 0;
}

/*============================================================================*/
/* bslEdidGetSinkType                                                 */
/*============================================================================*/
error_code_t
bsl_edid_get_sink_type
(
    unit_select_t              tx_unit,
    bsl_sink_type_t      *p_sink_type
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_sink_type == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	*p_sink_type = p_dis->edid_sink_type;

	return 0;
}

/*============================================================================*/
/* bslEdidGetSourceAddress                                            */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_edid_get_source_address
(
    unit_select_t  tx_unit,
    u16          *p_source_address
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_source_address == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	*p_source_address = p_dis->edid_source_address;

	return 0;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslEdidGetVideoCapabilities                                        */
/*============================================================================*/
error_code_t
bsl_edid_get_video_capabilities
(
    unit_select_t          tx_unit,
    u8                   *p_edid_vfmts,
    uint                    v_fmt_length,
    uint                    *p_vfmts_avail,
    u8                   *p_vid_flags
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	uint                i;          /* Loop index */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_edid_vfmts == NULL)
	RETIF_BADPARAM(v_fmt_length < 1)
	RETIF_BADPARAM(p_vfmts_avail == NULL)
	RETIF_BADPARAM(p_vid_flags == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	/* Copy the Device Instance Structure EdidVFmts descriptors to
	 * pEdidVFmts until we run out or no more space in structure.
	 *	 */
	if(p_dis->edid_svd_cnt > 0) {
		for(i = 0; (i < (uint)p_dis->edid_svd_cnt) && (i < v_fmt_length); i++) {
			p_edid_vfmts[i] = p_dis->edid_vfmts[i];
		}
	} else {
		/* No pEdidVFmts to copy so set a zero format to be safe */
		p_edid_vfmts[0] = HDMITX_VFMT_NULL;
	}

	/* Fill Video Flags parameter */
	*p_vid_flags = ((p_dis->edid_cea_flags & 0x80) |         /* Underscan */
	                /* YUV444, YUV422 */
	                ((p_dis->edid_cea_flags & 0x30) << 1));

	/* Fill number of SVDs available parameter */
	*p_vfmts_avail = p_dis->edid_svd_cnt;

	return 0;
}

/*============================================================================*/
/* bslEdidGetVideoPreferred                                           */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_edid_get_video_preferred
(
    unit_select_t		   tx_unit,
    bsl_edid_dtd_t *p_edid_dtd
)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */
	u8               edid_result;

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_edid_dtd == NULL)

	/* IF the EdidStatus value is not valid in the Device Instance Structure
	 * THEN call bslEdidGetBlockData.
	 *	 */
	if(p_dis->edid_status == HDMITX_EDID_NOT_READ) {
		err = bsl_edid_get_block_data(tx_unit, NULL, 0, 0, &edid_result);
		/* IF the result is not 0 THEN return it. */
		RETIF(err != 0, err)
	}

	/* Populate the Detailed Timing Descriptor structure pEdidDTD from
	 * EdidDtd in the Device Instance Structure.
	 *	 */
	lmemcpy(p_edid_dtd, &p_dis->edid_dtd, sizeof(*p_edid_dtd));

	return 0;
}
#endif /* DEMO_BUILD */

#ifndef DEMO_BUILD
error_code_t
bsl_edid_get_detailed_timing_descriptors
(
    unit_select_t          tx_unit,
    bsl_edid_dtd_t    *p_edid_dtd,
    u8                   nb_size,
    u8                   *p_dtda_vail
)
{

	/* Pointer to Device Instance Structure */
	hdmi_txobject_t    *p_dis;
	error_code_t       err;        /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_edid_dtd == NULL)
	RETIF_BADPARAM(p_dtda_vail == NULL)
	RETIF_BADPARAM(nb_size == 0)

	if((p_dis->edid_status == HDMITX_EDID_READ)  ||
	   (p_dis->edid_status == HDMITX_EDID_ERROR) ||
	   (p_dis->edid_status == HDMITX_EDID_ERROR_INCOMPLETE)) {
		/* allow if edid are read or if there are a chk error on an other block than block 0 */
		if(nb_size > p_dis->nb_dtdstored) {
			*p_dtda_vail = p_dis->nb_dtdstored;
		} else {
			*p_dtda_vail = nb_size;
		}

		lmemcpy(p_edid_dtd, p_dis->edid_dtd, sizeof(bsl_edid_dtd_t) *(*p_dtda_vail));
	} else {
		/* Not allowed if EdidStatus value is not valid */
		err = ERR_HDMI_INVALID_STATE;
	}

	return 0;

}
#endif  /* DEMO_BUILD */

/*============================================================================*/
/* bslMatrixSetCoeffs                                                 */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_matrix_set_coeffs
(
    unit_select_t		   tx_unit,
    bsl_mat_coeff_t *p_mat_coeff
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	int                 i;      /* Loop index */
	u8               buf[HDMITX_MAT_COEFF_NUM * 2];     /* Temp buffer */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_mat_coeff == (bsl_mat_coeff_t *)0)
	for(i = 0; i < HDMITX_MAT_COEFF_NUM; i++) {
		RETIF_BADPARAM((p_mat_coeff->coeff[i] < HDMITX_MAT_OFFSET_MIN) ||
		               (p_mat_coeff->coeff[i] > HDMITX_MAT_OFFSET_MAX))
	}

	/* Convert signed 11 bit values from Coeff array to pairs of MSB-LSB
	 * register values, and write to register pairs
	 *	 */
	for(i = 0; i < HDMITX_MAT_COEFF_NUM; i++) {
		/* Mask & copy MSB */
		buf[i*2] = (u8)(((u16)p_mat_coeff->coeff[i] & 0x0700) >> 8);
		/* Copy LSB */
		buf[(i*2)+1] = (u8)((u16)p_mat_coeff->coeff[i] & 0x00FF);
	}
	err = set_hw_registers(p_dis,
	                       E_REG_P00_MAT_P11_MSB_W,
	                       &buf[0],
	                       HDMITX_MAT_COEFF_NUM * 2);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslMatrixSetConversion                                             */
/*============================================================================*/
error_code_t
bsl_matrix_set_conversion
(
    unit_select_t          tx_unit,
    bsl_vid_fmt_t     vin_fmt,
    bsl_vin_mode_t    vin_mode,
    bsl_vid_fmt_t     vout_fmt,
    bsl_vout_mode_t   vout_mode,
    bsl_vqr_t        dvi_vqr
)
{
	/* Ptr to Device Instance Structure */
	hdmi_txobject_t            *p_dis;
	error_code_t               err;        /* Error code */
	bsl_colourspace_t    cspace_in;  /* Input colourspaces */
	bsl_colourspace_t    cspace_out; /* Output colourspaces */
	/* Index into matrix preset array */
	int                         matrix_index;
	u8						buf[MATRIX_PRESET_SIZE]; /* Temp buffer */
	u8						i; /* Loop index */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(((vin_fmt < HDMITX_VFMT_TV_MIN) || (vin_fmt > HDMITX_VFMT_PC_MAX))
	               || ((vin_fmt > HDMITX_VFMT_TV_MAX) && (vin_fmt < HDMITX_VFMT_PC_MIN)))
	/* NB: NO_CHANGE is not valid for this function, so limit to actual values*/
	RETIF_BADPARAM(vin_mode >= HDMITX_VINMODE_NO_CHANGE)
	RETIF_BADPARAM(((vout_fmt < HDMITX_VFMT_TV_MIN) || (vout_fmt > HDMITX_VFMT_PC_MAX))
	               || ((vout_fmt > HDMITX_VFMT_TV_MAX) && (vout_fmt < HDMITX_VFMT_PC_MIN)))
	/* NB: NO_CHANGE is not valid for this function, so limit to actual values*/
	RETIF_BADPARAM(vout_mode >= HDMITX_VOUTMODE_NO_CHANGE)

	/* Since vinMode and voutMode are different types, we don't use a local
	 *	   function to do this and use inline code twice */

	/* Calculate input colour space */
	if((vin_fmt == HDMITX_VFMT_TV_MIN) || (vin_fmt >= HDMITX_VFMT_PC_MIN)) {
		/* Catch the VGA or PC formats */
		cspace_in = HDMITX_CS_RGB_FULL;
	} else {
		switch(vin_fmt) {
			/* Catch the HD modes */
		case hdmitx_vfmt_04_1280x720p_60hz:
		case hdmitx_vfmt_05_1920x1080i_60hz:
		case hdmitx_vfmt_16_1920x1080p_60hz:
		case hdmitx_vfmt_19_1280x720p_50hz:
		case hdmitx_vfmt_20_1920x1080i_50hz:
		case hdmitx_vfmt_31_1920x1080p_50hz:
		case hdmitx_vfmt_32_1920x1080p_24hz:
		case hdmitx_vfmt_33_1920x1080p_25hz:
		case hdmitx_vfmt_34_1920x1080p_30hz:
			if(vin_mode == HDMITX_VINMODE_RGB444) {  /* RGB */
				cspace_in = HDMITX_CS_RGB_LIMITED;
				/* CCIR656, YUV444, YU422 */
			} else {
				cspace_in = HDMITX_CS_YUV_ITU_BT709;
			}
			break;
		default:    /* Now all the SD modes */
			if(vin_mode == HDMITX_VINMODE_RGB444) {  /* we're RGB */
				cspace_in = HDMITX_CS_RGB_LIMITED;
				/* CCIR656, YUV444, YU422 */
			} else {
				cspace_in = HDMITX_CS_YUV_ITU_BT601;
			}
			break;
		}
	}

	/* Calculate output colour space */
	if((vout_fmt == HDMITX_VFMT_TV_MIN) || (vout_fmt >= HDMITX_VFMT_PC_MIN)) {
		/* Catch the VGA or PC formats */
		cspace_out = HDMITX_CS_RGB_FULL;
	} else {
		switch(vout_fmt) {
			/* Catch the HD modes */
		case hdmitx_vfmt_04_1280x720p_60hz:
		case hdmitx_vfmt_05_1920x1080i_60hz:
		case hdmitx_vfmt_16_1920x1080p_60hz:
		case hdmitx_vfmt_19_1280x720p_50hz:
		case hdmitx_vfmt_20_1920x1080i_50hz:
		case hdmitx_vfmt_31_1920x1080p_50hz:
		case hdmitx_vfmt_32_1920x1080p_24hz:
		case hdmitx_vfmt_33_1920x1080p_25hz:
		case hdmitx_vfmt_34_1920x1080p_30hz:
			if(vout_mode == HDMITX_VOUTMODE_RGB444) { /* RGB */
				cspace_out = HDMITX_CS_RGB_LIMITED;
			} else {                                /* YUV444 or YUV422 */
				cspace_out = HDMITX_CS_YUV_ITU_BT709;
			}
			break;
		default:    /* Now all the SD modes */
			if(vout_mode == HDMITX_VOUTMODE_RGB444) { /* RGB */
				cspace_out = HDMITX_CS_RGB_LIMITED;
			} else {                                /* YUV444 or YUV422 */
				cspace_out = HDMITX_CS_YUV_ITU_BT601;
			}
			break;
		}
	}

	if(p_dis->sink_type == HDMITX_SINK_DVI) {

		switch(dvi_vqr) {
		case HDMITX_VQR_DEFAULT :
			/* do nothing */
			break;

		case HDMITX_RGB_FULL :
			cspace_out = HDMITX_CS_RGB_FULL;
			break;

		case HDMITX_RGB_LIMITED :
			cspace_out = HDMITX_CS_RGB_LIMITED;
			break;
		}

	}

	if(cspace_in == cspace_out) {
		/* Switch off colour matrix by setting bypass flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_MAT_CONTRL_W,
		                            e_maskreg_p00_mat_contrl_mat_bp,
		                            1);
	} else {
		/* Load appropriate values into matrix  - we have preset blocks of
		 * 31 register vales in a table, just need to work out which set to use
		 *		 */
		matrix_index = k_matrix_index[cspace_in][cspace_out];

		/* Set the first block byte separately, as it is shadowed and can't
		 * be set by setHwRegisters */
		err = set_hw_register(p_dis,
		                      E_REG_P00_MAT_CONTRL_W,
		                      k_matrix_preset[matrix_index][0]);
		RETIF_REG_FAIL(err)

		for(i = 0; i < MATRIX_PRESET_SIZE; i++) {
			buf[i] = k_matrix_preset[matrix_index][i];
		}

		/* Set the rest of the block */
		err = set_hw_registers(p_dis,
		                       E_REG_P00_MAT_OI1_MSB_W,
		                       &buf[1],
		                       MATRIX_PRESET_SIZE - 1);
	}
	return err;
}

/*============================================================================*/
/* bslMatrixSetInputOffset                                            */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_matrix_set_input_offset
(
    unit_select_t		    tx_unit,
    bsl_mat_offset_t  *p_mat_offset
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	int                 i;      /* Loop index */
	u8               buf[HDMITX_MAT_OFFSET_NUM * 2];    /* Temp buffer */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_mat_offset == (bsl_mat_offset_t *)0)
	for(i = 0; i < HDMITX_MAT_OFFSET_NUM; i++) {
		RETIF_BADPARAM((p_mat_offset->offset[i] < HDMITX_MAT_OFFSET_MIN) ||
		               (p_mat_offset->offset[i] > HDMITX_MAT_OFFSET_MAX))
	}

	/* Convert signed 11 bit values from Offset array to pairs of MSB-LSB
	 * register values, and write to register pairs
	 *	 */
	for(i = 0; i < HDMITX_MAT_OFFSET_NUM; i++) {
		/* Mask & copy MSB */
		buf[i*2] = (u8)(((u16)p_mat_offset->offset[i] & 0x0700) >> 8);
		/* Copy LSB */
		buf[(i*2)+1] = (u8)((u16)p_mat_offset->offset[i] & 0x00FF);
	}
	err = set_hw_registers(p_dis,
	                       E_REG_P00_MAT_OI1_MSB_W,
	                       &buf[0],
	                       HDMITX_MAT_OFFSET_NUM * 2);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslMatrixSetMode                                                   */
/*============================================================================*/
error_code_t
bsl_matrix_set_mode
(
    unit_select_t		 tx_unit,
    bslm_cntrl_t m_control,
    bslm_scale_t m_scale
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM((m_control > HDMITX_MCNTRL_MAX) ||
	               (m_scale > HDMITX_MSCALE_MAX))

	/* For each value that is not NoChange, update the appropriate register */
	if(m_control != HDMITX_MCNTRL_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_MAT_CONTRL_W,
		                            e_maskreg_p00_mat_contrl_mat_bp,
		                            (u8)m_control);
		RETIF_REG_FAIL(err)
	}

	if(m_scale != HDMITX_MSCALE_NO_CHANGE) {
		err = set_hw_register_field(p_dis,
		                            E_REG_P00_MAT_CONTRL_W,
		                            e_maskreg_p00_mat_contrl_mat_sc,
		                            (u8)m_scale);
		RETIF_REG_FAIL(err)
	}

	return 0;
}

/*============================================================================*/
/* bslMatrixSetOutputOffset                                           */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_matrix_set_output_offset
(
    unit_select_t		    tx_unit,
    bsl_mat_offset_t  *p_mat_offset
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	int                 i;      /* Loop index */
	u8               buf[HDMITX_MAT_OFFSET_NUM * 2];   /* Temp buffer */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM(p_mat_offset == (bsl_mat_offset_t *)0)
	for(i = 0; i < HDMITX_MAT_OFFSET_NUM; i++) {
		RETIF_BADPARAM((p_mat_offset->offset[i] < HDMITX_MAT_OFFSET_MIN) ||
		               (p_mat_offset->offset[i] > HDMITX_MAT_OFFSET_MAX))
	}

	/* Convert signed 11 bit values from Offset array to pairs of MSB-LSB
	 * register values, and write to register pairs
	 *	 */
	for(i = 0; i < HDMITX_MAT_OFFSET_NUM; i++) {
		/* Mask & copy MSB */
		buf[i*2] = (u8)(((u16)p_mat_offset->offset[i] & 0x0700) >> 8);
		/* Copy LSB */
		buf[(i*2)+1] = (u8)((u16)p_mat_offset->offset[i] & 0x00FF);
	}
	err = set_hw_registers(p_dis,
	                       E_REG_P00_MAT_OO1_MSB_W,
	                       &buf[0],
	                       HDMITX_MAT_OFFSET_NUM * 2);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslPktSetAclkRecovery                                              */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_pkt_set_aclk_recovery
(
    unit_select_t  tx_unit,
    bool            b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	/* Write the ACR packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_acr,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetAcp                                                       */
/*============================================================================*/
error_code_t
bsl_pkt_set_acp
(
    unit_select_t		tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    u8               u_acp_type,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[3]; /* Temp buffer to hold header bytes */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM(byte_cnt > HDMITX_PKT_DATA_BYTE_CNT)
		RETIF(byte_cnt == 0, ERR_HDMI_INCONSISTENT_PARAMS)

		/* Data to change, start by clearing ACP packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_FLAGS_RW,
		                            e_maskreg_p11_dip_flags_acp,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare ACP header */
		buf[0] = 0x04;      /* ACP packet */
		buf[1] = u_acp_type;
		buf[2] = 0;         /* Reserved [HDMI 1.2] */

		/* Write 3 header bytes to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ACP_HB0_RW,
		                       &buf[0],
		                       3);
		RETIF_REG_FAIL(err)

		/* Write "byteCnt" bytes of data to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ACP_PB0_RW,
		                       &p_pkt->data_byte[0],
		                       (u16)byte_cnt);
		RETIF_REG_FAIL(err)
	}

	/* Write the ACP packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_acp,
	                            (u8)b_enable);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslPktSetAudioInfoframe                                            */
/*============================================================================*/
error_code_t
bsl_pkt_set_audio_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_aif_t *p_pkt,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[9]; /* Temp buffer to hold header/packet bytes */
	u16              buf_reg; /* Base register used for writing InfoFrame*/
	u16              flag_reg;/* Flag register to be used */
	u8               flag_mask;/* Mask used for writing flag register */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))
	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM(p_pkt->coding_type   > 0x0F)
		RETIF_BADPARAM(p_pkt->channel_count > 0x07)
		RETIF_BADPARAM(p_pkt->sample_freq   > 0x07)
		RETIF_BADPARAM(p_pkt->sample_size   > 0x03)
		/* No need to check ChannelAlloc - all values are allowed */
		RETIF_BADPARAM((p_pkt->down_mix_inhibit != true) &&
		               (p_pkt->down_mix_inhibit != false))
		RETIF_BADPARAM(p_pkt->level_shift   > 0x0F)
	}

	/* Only supported for device N4 or later */

	/* We're using n4 or later, use IF4 buffer for Audio InfoFrame */
	buf_reg = E_REG_P10_IF4_HB0_RW;
	flag_reg = E_REG_P11_DIP_IF_FLAGS_RW;
	flag_mask = e_maskreg_p11_dip_if_flags_if4;

	if(p_pkt != NULL) {
		/* Data to change, start by clearing AIF packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            flag_reg,
		                            flag_mask,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare AIF header */
		buf[0] = 0x84;     /* Audio InfoFrame */
		buf[1] = 0x01;     /* Version 1 [HDMI 1.2] */
		buf[2] = 0x0A;     /* Length [HDMI 1.2] */

		/* Prepare AIF packet (byte numbers offset by 3) */
		buf[0+3] = 0;     /* Preset checksum to zero so calculation works! */
		buf[1+3] = ((p_pkt->coding_type & 0x0F) << 4) |
		           (p_pkt->channel_count & 0x07);        /* CT3-0, CC2-0 */
		buf[2+3] = ((p_pkt->sample_freq & 0x07) << 2) |
		           (p_pkt->sample_size & 0x03);          /* SF2-0, SS1-0 */
		buf[3+3] = 0;                                      /* [HDMI 1.2] */
		buf[4+3] = p_pkt->channel_alloc;                  /* CA7-0 */
		buf[5+3] = ((p_pkt->level_shift & 0x0F) << 3);    /* LS3-0 */
		if(p_pkt->down_mix_inhibit == true) {
			buf[5+3] += 0x80;                              /* DMI bit */
		}

		/* Calculate checksum - this is worked out on "Length" bytes of the
		 * packet, the checksum (which we've preset to zero), and the three
		 * header bytes.  We exclude bytes PB6 to PB10 (which we
		 * are not writing) since they are zero.
		 *		 */
		buf[0+3] = calculate_checksum(&buf[0], 0x0A + 1 + 3 - 5);

		/* Write header and packet bytes in one operation */
		err = set_hw_registers(p_dis,
		                       buf_reg,
		                       &buf[0],
		                       9);
		RETIF_REG_FAIL(err)
	}

	/* Write AIF packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            flag_reg,
	                            flag_mask,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetGeneralCntrl                                              */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_pkt_set_general_cntrl
(
    unit_select_t      tx_unit,
    bsla_mute_t  *pa_mute,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(pa_mute != NULL) {
		RETIF_BADPARAM((*pa_mute != HDMITX_AMUTE_OFF) && (*pa_mute != HDMITX_AMUTE_ON))

		if(*pa_mute == HDMITX_AMUTE_ON) {
			err = set_hw_register(p_dis, E_REG_P11_GC_AVMUTE_RW, 0x02);
			RETIF_REG_FAIL(err)
		} else {
			err = set_hw_register(p_dis, E_REG_P11_GC_AVMUTE_RW, 0x01);
			RETIF_REG_FAIL(err)
		}
	}

	/* Set or clear GC packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_gc,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetIsrc1                                                     */
/*============================================================================*/
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
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[3]; /* Temp buffer to hold header bytes */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM((b_isrc_cont != true) && (b_isrc_cont != false))
		RETIF_BADPARAM((b_isrc_valid != true) && (b_isrc_valid != false))
		RETIF_BADPARAM(u_isrc_status > 7)    /* 3 bits */
		RETIF_BADPARAM(byte_cnt > HDMITX_PKT_DATA_BYTE_CNT)
		RETIF(byte_cnt == 0, ERR_HDMI_INCONSISTENT_PARAMS)

		/* Data to change, start by clearing ISRC1 packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_FLAGS_RW,
		                            e_maskreg_p11_dip_flags_isrc1,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare ISRC1 header */
		buf[0] = 0x05;      /* ISRC1 packet */
		buf[1] = (u_isrc_status & 0x07);
		if(b_isrc_valid == true) {
			buf[1] += 0x40;
		}
		if(b_isrc_cont == true) {
			buf[1] += 0x80;
		}
		buf[2] = 0;         /* Reserved [HDMI 1.2] */

		/* Write 3 header bytes to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ISRC1_HB0_RW,
		                       &buf[0],
		                       3);
		RETIF_REG_FAIL(err)

		/* Write "byteCnt" bytes of data to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ISRC1_PB0_RW,
		                       &p_pkt->data_byte[0],
		                       (u16)byte_cnt);
		RETIF_REG_FAIL(err)
	}

	/* Write the ISRC1 packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_isrc1,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetIsrc2                                                     */
/*============================================================================*/
error_code_t
bsl_pkt_set_isrc2
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[3]; /* Temp buffer to hold header bytes */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM(byte_cnt > HDMITX_PKT_DATA_BYTE_CNT)
		RETIF(byte_cnt == 0, ERR_HDMI_INCONSISTENT_PARAMS)

		/* Data to change, start by clearing ISRC2 packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_FLAGS_RW,
		                            e_maskreg_p11_dip_flags_isrc2,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare ISRC2 header */
		buf[0] = 0x06;      /* ISRC2 packet */
		buf[1] = 0;         /* Reserved [HDMI 1.2] */
		buf[2] = 0;         /* Reserved [HDMI 1.2] */

		/* Write 3 header bytes to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ISRC2_HB0_RW,
		                       &buf[0],
		                       3);
		RETIF_REG_FAIL(err)

		/* Write "byteCnt" bytes of data to registers */
		err = set_hw_registers(p_dis,
		                       E_REG_P11_ISRC2_PB0_RW,
		                       &p_pkt->data_byte[0],
		                       (u16)byte_cnt);
		RETIF_REG_FAIL(err)
	}

	/* Write the ISRC2 packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_isrc2,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetMpegInfoframe                                             */
/*============================================================================*/
error_code_t
bsl_pkt_set_mpeg_infoframe
(
    unit_select_t          tx_unit,
    bsl_pkt_mpeg_t    *p_pkt,
    bool                    b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[9]; /* Temp buffer to hold packet */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM((p_pkt->b_field_repeat != true) && (p_pkt->b_field_repeat != false))
		RETIF_BADPARAM(p_pkt->frame_type >= HDMITX_MPEG_FRAME_INVALID)

		/* Data to change, start by clearing MPEG packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_IF_FLAGS_RW,
		                            e_maskreg_p11_dip_if_flags_if5,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare MPEG header */
		buf[0] = 0x85;      /* MPEG Source InfoFrame */
		buf[1] = 0x01;      /* Version 1 [HDMI 1.2] */
		buf[2] = 0x0A;      /* Length [HDMI 1.2] */

		/* Prepare MPEG packet (byte numbers offset by 3) */
		buf[0+3] = 0;     /* Preset checksum to zero so calculation works! */
		buf[1+3] = (u8)(p_pkt->bit_rate & 0x000000FF);
		buf[2+3] = (u8)((p_pkt->bit_rate & 0x0000FF00) >> 8);
		buf[3+3] = (u8)((p_pkt->bit_rate & 0x00FF0000) >> 16);
		buf[4+3] = (u8)((p_pkt->bit_rate & 0xFF000000) >> 24);
		buf[5+3] = p_pkt->frame_type;                         /* MF1-0 */
		if(p_pkt->b_field_repeat == true) {
			buf[5+3] += 0x10;                               /* FR0 bit */
		}

		/* Calculate checksum - this is worked out on "Length" bytes of the
		 * packet, the checksum (which we've preset to zero), and the three
		 * header bytes.  We exclude bytes PB6 to PB10 (which we
		 * are not writing) since they are zero.
		 *		 */
		buf[0+3] = calculate_checksum(&buf[0], 0x0A + 1 + 3 - 5);

		/* Write header and packet bytes in one operation */
		err = set_hw_registers(p_dis,
		                       E_REG_P10_IF5_HB0_RW,
		                       &buf[0],
		                       9);
		RETIF_REG_FAIL(err)
	}

	/* Write the MPEG packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_IF_FLAGS_RW,
	                            e_maskreg_p11_dip_if_flags_if5,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetNullInsert                                                */
/*============================================================================*/
error_code_t
bsl_pkt_set_null_insert
(
    unit_select_t  tx_unit,
    bool            b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameter(s) */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	/* Set or clear FORCE_NULL packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_force_null,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetNullSingle                                                */
/*============================================================================*/
error_code_t
bsl_pkt_set_null_single
(
    unit_select_t  tx_unit
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Set NULL packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_FLAGS_RW,
	                            e_maskreg_p11_dip_flags_null,
	                            0x01);
	return err;
}

/*============================================================================*/
/* bslPktSetSpdInfoframe                                              */
/*============================================================================*/
error_code_t
bsl_pkt_set_spd_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_spd_t *p_pkt,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[29];/* Temp buffer to hold packet */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM(p_pkt->source_dev_info >= HDMITX_SPD_INFO_INVALID)

		/* Data to change, start by clearing SPD packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_IF_FLAGS_RW,
		                            e_maskreg_p11_dip_if_flags_if3,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare SPD header */
		buf[0] = 0x83;      /* Source. Product Descriptor InfoFrame */
		buf[1] = 0x01;      /* Version 1 [CEA 861B] */
		buf[2] = 0x19;      /* Length [HDMI 1.2] */

		/* Prepare SPD packet (byte numbers offset by 3) */
		buf[0+3] = 0;     /* Preset checksum to zero so calculation works! */
		lmemcpy(&buf[1+3], &p_pkt->vendor_name[0], HDMI_TX_SPD_VENDOR_SIZE);
		lmemcpy(&buf[1+3+HDMI_TX_SPD_VENDOR_SIZE], &p_pkt->prod_descr[0],
		        HDMI_TX_SPD_DESCR_SIZE);

		buf[HDMI_TX_SPD_LENGTH+3] = p_pkt->source_dev_info;

		/* Calculate checksum - this is worked out on "Length" bytes of the
		 * packet, the checksum (which we've preset to zero), and the three
		 * header bytes.
		 *		 */
		buf[0+3] = calculate_checksum(&buf[0], HDMI_TX_SPD_LENGTH + 1 + 3);

		/* Write header and packet bytes in one operation */
		err = set_hw_registers(p_dis,
		                       E_REG_P10_IF3_HB0_RW,
		                       &buf[0],
		                       29);
		RETIF_REG_FAIL(err)
	}

	/* Write the SPD packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_IF_FLAGS_RW,
	                            e_maskreg_p11_dip_if_flags_if3,
	                            (u8)b_enable);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslPktSetVideoInfoframe                                            */
/*============================================================================*/
error_code_t
bsl_pkt_set_video_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_vif_t *p_pkt,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[17];/* Temp buffer to hold header/packet bytes */
	u16              buf_reg; /* Base register used for writing InfoFrame*/
	u16              flag_reg;/* Flag register to be used */
	u8               flag_mask;/* Mask used for writing flag register */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))
	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		RETIF_BADPARAM(p_pkt->colour             > 0x03)
		RETIF_BADPARAM((p_pkt->active_info != true) && (p_pkt->active_info != false))
		RETIF_BADPARAM(p_pkt->bar_info            > 0x03)
		RETIF_BADPARAM(p_pkt->scan_info           > 0x03)
		RETIF_BADPARAM(p_pkt->colorimetry        > 0x03)
		RETIF_BADPARAM(p_pkt->picture_aspect_ratio > 0x03)
		RETIF_BADPARAM(p_pkt->active_format_ratio  > 0x0F)
		RETIF_BADPARAM(p_pkt->scaling            > 0x03)
		RETIF_BADPARAM(p_pkt->vid_format          > 0x7F)
		RETIF_BADPARAM(p_pkt->pixel_repeat        > 0x0F)
	}

	/* Only supported for device N4 or later */

	/* We're using n4 or later, use IF2 buffer for Video InfoFrame */
	buf_reg = E_REG_P10_IF2_HB0_RW;
	flag_reg = E_REG_P11_DIP_IF_FLAGS_RW;
	flag_mask = e_maskreg_p11_dip_if_flags_if2;

	if(p_pkt != NULL) {
		/* Data to change, start by clearing VIF packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            flag_reg,
		                            flag_mask,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare VIF header */
		buf[0] = 0x82;     /* Video InfoFrame */
		buf[1] = 0x02;     /* Version 2 [HDMI 1.2] */
		buf[2] = 0x0D;     /* Length [HDMI 1.2] */

		/* Prepare VIF packet (byte numbers offset by 3) */
		buf[0+3] = 0;     /* Preset checksum to zero so calculation works! */
		buf[1+3] = ((p_pkt->colour & 0x03) << 5) |       /* Y1-0, B1-0,S1-0 */
		           ((p_pkt->bar_info & 0x03) << 2) |
		           (p_pkt->scan_info & 0x03);
		if(p_pkt->active_info == true) {
			buf[1+3] += 0x10;                              /* AI bit */
		}
		buf[2+3] = ((p_pkt->colorimetry & 0x03) << 6) |  /* C1-0, M1-0, R3-0 */
		           ((p_pkt->picture_aspect_ratio & 0x03) << 4) |
		           (p_pkt->active_format_ratio & 0x0F);
		/* SC1-0  /  [HDMI 1.2] */
		buf[3+3] = (p_pkt->scaling & 0x03);
		buf[4+3] = (p_pkt->vid_format & 0x7F);            /* VIC6-0 */
		buf[5+3] = (p_pkt->pixel_repeat & 0x0F);          /* PR3-0 */
		buf[6+3] = (u8)(p_pkt->end_top_bar_line & 0x00FF);
		buf[7+3] = (u8)((p_pkt->end_top_bar_line & 0xFF00) >> 8);
		buf[8+3] = (u8)(p_pkt->start_bottom_bar_line & 0x00FF);
		buf[9+3] = (u8)((p_pkt->start_bottom_bar_line & 0xFF00) >> 8);
		buf[10+3] = (u8)(p_pkt->end_left_bar_pixel & 0x00FF);
		buf[11+3] = (u8)((p_pkt->end_left_bar_pixel & 0xFF00) >> 8);
		buf[12+3] = (u8)(p_pkt->start_right_bar_pixel & 0x00FF);
		buf[13+3] = (u8)((p_pkt->start_right_bar_pixel & 0xFF00) >> 8);

		/* Calculate checksum - this is worked out on "Length" bytes of the
		 * packet, the checksum (which we've preset to zero), and the three
		 * header bytes.
		 *		 */
		buf[0+3] = calculate_checksum(&buf[0], 0x0D + 1 + 3);

		/* Write header and packet bytes in one operation */
		err = set_hw_registers(p_dis,
		                       buf_reg,
		                       &buf[0],
		                       17);
		RETIF_REG_FAIL(err)
	}

	/* Write VIF packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            flag_reg,
	                            flag_mask,
	                            (u8)b_enable);
	return err;
}

/*============================================================================*/
/* bslPktSetVsInfoframe                                               */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t
bsl_pkt_set_vs_infoframe
(
    unit_select_t      tx_unit,
    bsl_pkt_t    *p_pkt,
    uint                byte_cnt,
    u8               u_version,
    bool                b_enable
)
{
	hdmi_txobject_t    *p_dis;  /* Pointer to Device Instance Structure */
	error_code_t       err;    /* Error code */
	u8               buf[31];/* Temp buffer to hold packet */

	/* Check unit parameter and point to TX unit object */
	err = check_unit_set_dis(tx_unit, &p_dis);
	RETIF(err != 0, err)

	/* Return ERR_HDMI_OPERATION_NOT_PERMITTED error if the
	 * sinkType is not HDMI
	 *	 */
	RETIF(p_dis->sink_type != HDMITX_SINK_HDMI,
	      ERR_HDMI_OPERATION_NOT_PERMITTED)

	/* Only supported for device N4 or later */

	/* Check remaining parameter(s) - NULL pointer allowed */
	RETIF_BADPARAM((b_enable != true) && (b_enable != false))

	if(p_pkt != NULL) {
		/* Pointer to structure provided so check parameters */
		/* InfoFrame needs a checksum, so 1 usable byte less than full pkt */
		RETIF_BADPARAM(byte_cnt > (HDMITX_PKT_DATA_BYTE_CNT - 1))
		RETIF(byte_cnt == 0, ERR_HDMI_INCONSISTENT_PARAMS)

		/* Data to change, start by clearing VS_IF packet insertion flag */
		err = set_hw_register_field(p_dis,
		                            E_REG_P11_DIP_IF_FLAGS_RW,
		                            e_maskreg_p11_dip_if_flags_if1,
		                            0x00);
		RETIF_REG_FAIL(err)

		/* Prepare VS_IF header */
		/* Clear buffer as user may vary length used */
		lmemset(&buf[0], 0, 31);
		buf[0] = 0x81;          /* Vendor Specific InfoFrame */
		buf[1] = u_version;      /* Vendor defined version */
		buf[2] = (u8)byte_cnt;       /* Length [HDMI 1.2] */

		/* Prepare VS_IF packet (byte numbers offset by 3) */
		buf[0+3] = 0;     /* Preset checksum to zero so calculation works! */
		lmemcpy(&buf[1+3], &p_pkt->data_byte[0], byte_cnt);

		/* Calculate checksum - this is worked out on "Length" bytes of the
		 * packet, the checksum (which we've preset to zero), and the three
		 * header bytes.
		 *		 */
		buf[0+3] = calculate_checksum(&buf[0], byte_cnt + 1 + 3);

		/* Write header and packet bytes in one operation  - write entire
		 * buffer even though we may not be using it all so that zeros
		 * are placed in the unused registers. */
		err = set_hw_registers(p_dis,
		                       E_REG_P10_IF1_HB0_RW,
		                       &buf[0],
		                       31);
		RETIF_REG_FAIL(err)
	}

	/* Write the VS_IF packet insertion flag */
	err = set_hw_register_field(p_dis,
	                            E_REG_P11_DIP_IF_FLAGS_RW,
	                            e_maskreg_p11_dip_if_flags_if1,
	                            (u8)b_enable);
	return err;
}
#endif /* DEMO_BUILD */

/*============================================================================*/
/*                   STATIC FUNCTION DEFINTIONS                               */
/*============================================================================*/

/*============================================================================*/
/* calculateChecksum - returns the byte needed to yield a checksum of zero    */
/*============================================================================*/
static u8
calculate_checksum
(
    u8       *p_data,     /* Pointer to checksum data */
    int         num_bytes    /* Number of bytes over which to calculate */
)
{
	u8       checksum = 0;   /* Working checksum calculation */
	u8       result = 0;     /* Value to be returned */
	int         i;

	if((p_data != NULL) && (num_bytes > 0)) {
		for(i = 0; i < num_bytes; i++) {
			checksum = checksum + (*(p_data + i));
		}
		result = (255 - checksum) + 1;
	}
	return result;          /* returns 0 in the case of null ptr or 0 bytes */
}

/*============================================================================*/
/* getEdidBlock - reads an entire edid block                                  */
/*============================================================================*/
static error_code_t
get_edid_block
(
    hdmi_txobject_t    *p_dis,      /* Device instance strucure to use */
    int                 block_number /* Block number to read */
)
{
	error_code_t   err;        /* Error code */
	int             i;          /* Loop index */
	u8           chksum;     /* Checksum value */
	u8           segptr;     /* Segment ptr value */
	u8           offset;     /* Word offset value */
	bsl_sys_args_edid_t sys_args; /* Arguments passed to system function */

	/* Check block number is valid [CEA861C A.2.1] */
	RETIF_BADPARAM(block_number >= 255)

	/* Enable the EDID ghost device to allow I2C write-through */
	/* Write the segment pointer address into the ghost regsiter */
	/* (For N3, we did't use the segptr and did't support alternate addr) */
	/* Load both Ghost registers  - we load whole register so no shadowing
	 *	   to give control of lsbs */

	if(p_dis->b_edid_alternate_addr == true) {
		/* Use alternate address and set  a0_zero bit (lsb) in GHOST_XADDR */
		err = set_hw_register(p_dis, E_REG_P00_GHOST_XADDR_W,
		                      DDC_SGMT_PTR_ADDRESS + 1);
		RETIF_REG_FAIL(err)
		err = set_hw_register(p_dis, E_REG_P00_GHOST_ADDR_W,
		                      DDC_EDID_ADDRESS_ALT);
		RETIF_REG_FAIL(err)
	} else {
		/* Use normal address and don't set a0_zero bit */
		err = set_hw_register(p_dis, E_REG_P00_GHOST_XADDR_W,
		                      DDC_SGMT_PTR_ADDRESS);
		RETIF_REG_FAIL(err)
		err = set_hw_register(p_dis, E_REG_P00_GHOST_ADDR_W,
		                      DDC_EDID_ADDRESS);
		RETIF_REG_FAIL(err)
	}

	/* Calculate which segment of the EDID we need (2 blocks per segment) */
	segptr = (u8)block_number / 2;
	/* For even blocks we need an offset of 0, odd blocks we need 128 */
	offset = (((u8)block_number & 1) == 1) ? 128 : 0;

	/* If we're reading blocks 0 or 1, we don't use segptr, as sink may not
	 *	   support it.  We also never use for n3 or earlier, as it is not
	 *	   supported.
	 *	 */
	if(block_number < 2) {
		/* NULL SegPtrAddress used to indicate seg ptr should be skipped */
		sys_args.seg_ptr_addr  = 0;
	} else {
		sys_args.seg_ptr_addr  = DDC_SGMT_PTR_ADDRESS;
	}

	if(p_dis->b_edid_alternate_addr == true) {
		/* Use alternate address */
		sys_args.data_reg_addr = DDC_EDID_ADDRESS_ALT;
	} else {
		/* Use default address */
		sys_args.data_reg_addr = DDC_EDID_ADDRESS;
	}

	/* Read EDID block: THIS CAN FAIL IF DDC CONNECTION IS BROKEN */
	sys_args.seg_ptr      = segptr;
	sys_args.word_offset  = offset;
	sys_args.len_data     = EDID_BLOCK_SIZE;
	sys_args.p_data       = p_dis->edid_block;
	err = p_dis->sys_func_edid_read(&sys_args);

	/* Do not quit yet on I2C error: must always set GHOST_ADDR here */
	set_hw_register(p_dis, E_REG_P00_GHOST_ADDR_W, 0x01);

	/* Ignore last error, but return the more important EDID Read error */
	RETIF(err != 0, ERR_HDMI_I2C_READ)

	/* Add up all the values of the EDID block bytes, including the
	 * checksum byte
	 *	 */
	chksum = 0;
	for(i = 0; i < EDID_BLOCK_SIZE; i++) {
		chksum = chksum + p_dis->edid_block[i];
	}

	/* IF the EDID block does not yield a checksum of zero
	 * THEN return a ERR_HDMI_INVALID_STATE error.
	 *	 */
	return (chksum == 0) ? 0 : ERR_HDMI_INVALID_STATE;
}

/*============================================================================*/
/* parseEdidBlock                                                             */
/*============================================================================*/
static error_code_t
parse_edid_block
(
    /* Device instance strucure holding block */
    hdmi_txobject_t    *p_dis,
    int                 block_number /* Block number */
)
{
	u8           i;                  /* Loop index */
	u8           block_ptr, end_ptr;   /* Parsing pointers */
	u8           block_type, block_length;
	bool            dtd_found;
	u8           nb_blk_read;

	/* Check block number is valid [CEA861C A.2.1] */
	RETIF_BADPARAM(block_number >= 255)

	nb_blk_read = 0;
	dtd_found = true;
	block_ptr = 0;

	if(block_number == 0) {
		p_dis->edidbasic_display_param.u_video_input_def = p_dis->edid_block[0x14];
		p_dis->edidbasic_display_param.u_max_horizontal_size = p_dis->edid_block[0x15];
		p_dis->edidbasic_display_param.u_max_vertical_size = p_dis->edid_block[0x16];
		p_dis->edidbasic_display_param.u_gamma = p_dis->edid_block[0x17];
		p_dis->edidbasic_display_param.u_feature_support = p_dis->edid_block[0x18];

		/* Block 0 - contains DTDs but no video data block (SVDs) */
		/* search 2 possible DTD blocks in block 0 */
		for(i = 0; (i < 2) && (dtd_found); i++) {
			block_ptr = (u8)(EDID_BLK0_BASE_DTD + (i * EDID_DTD_BLK_SIZE));
			if((block_ptr + EDID_DTD_BLK_SIZE - 1) < EDID_BLOCK_SIZE) {
				dtd_found = store_dtd_block(p_dis, block_ptr);
				if(dtd_found) {
					nb_blk_read++;
				}
			}
		}

		dtd_found = true;

		/* Parse monitor descriptor */
		for(i = nb_blk_read; (i < EDID_NB_MAX_DESCRIP_BLK_IN_BLK_0) && (dtd_found); i++) {
			block_ptr = (u8)(EDID_BLK0_BASE_DTD + (i * EDID_DTD_BLK_SIZE));
			if((block_ptr + EDID_DTD_BLK_SIZE - 1) < EDID_BLOCK_SIZE) {
#ifndef DEMO_BUILD
				dtd_found = store_monitor_descriptor(p_dis, block_ptr);
#endif /* DEMO_BUILD */
			}
		}
	} else if(block_number >= 1) {
		switch(p_dis->edid_block[0]) {
			/* CEA EXTENSION */
		case EDID_CEA_EXTENSION:
			/* Read CEA flag bits here - lockout when read once??? */
			p_dis->edid_cea_flags = p_dis->edid_block[3];

			/* data block start always fixed */
			block_ptr = EDID_BLK_EXT_BASE;
			/* byte after end of data blocks */
			end_ptr = p_dis->edid_block[2];
			if(end_ptr >= (EDID_BLK_EXT_BASE + 2))
				/* Only try reading if data blocks take up 2 bytes or more, since
				 * a video data block must be at least 2 bytes
				 *				 */
			{
				while(block_ptr < end_ptr) {
					block_type   = (u8)((p_dis->edid_block[block_ptr] & 0xE0) >> 5);
					block_length = (p_dis->edid_block[block_ptr] & 0x1F);

					switch((int)block_type) {
					case E_CEA_VIDEO_BLOCK:  /* We have a video data block */
						for(i = 1; i <= block_length; i++) {
							/* If space, store non-zero SVDs */
							if((p_dis->edid_block[block_ptr + i] != 0) &&
							   (p_dis->edid_svd_cnt < HDMI_TX_SVD_MAX_CNT)) {
								p_dis->edid_vfmts[p_dis->edid_svd_cnt] =
								    p_dis->edid_block[block_ptr + i];
								p_dis->edid_svd_cnt++;
							}
						}
						break;
					case E_CEA_AUDIO_BLOCK:  /* We have an audio data block */
						for(i = 1; (i + 2) <= block_length; i += 3) {
							/* Must loop in steps of 3 (SAD size) */
							/* If space, store non-zero SADs */
							if(((p_dis->edid_block[block_ptr + i] & 0x78) != 0) &&
							   (p_dis->edid_sad_cnt < HDMI_TX_SAD_MAX_CNT)) {
								p_dis->edid_afmts[p_dis->edid_sad_cnt].mode_chans =
								    p_dis->edid_block[block_ptr + i];
								p_dis->edid_afmts[p_dis->edid_sad_cnt].freqs =
								    p_dis->edid_block[block_ptr + i + 1];
								p_dis->edid_afmts[p_dis->edid_sad_cnt].byte3 =
								    p_dis->edid_block[block_ptr + i + 2];
								p_dis->edid_sad_cnt++;
							}
						}
						break;
					case E_CEA_VSDB:         /* We have a VSDB */
						/* 5 bytes expected, but this is EDID land so double check*/
						if(block_length >= 5) {
							if((p_dis->edid_block[block_ptr + 1] == 0x03) &&
							   (p_dis->edid_block[block_ptr + 2] == 0x0C) &&
							   (p_dis->edid_block[block_ptr + 3] == 0x00)) {
								p_dis->edid_sink_type = HDMITX_SINK_HDMI;
								p_dis->edid_source_address =
								    ((u16)p_dis->edid_block[block_ptr + 4] << 8) +
								    p_dis->edid_block[block_ptr + 5];
							} else {
								p_dis->edid_sink_type = HDMITX_SINK_DVI;
							}
						}
						/* Space for byte with AI flag */
						if(block_length >= 6) {
							/* Mask AI bit */
							if((p_dis->edid_block[block_ptr + 6] & 0x80) == 0x80) {
								p_dis->edid_sink_ai = true;
							}
						}
						break;
					default:
						break;
					}
					block_ptr += (block_length + 1);  /* Point to next block */
				}
			}
			dtd_found = true;

			/* search possible DTD blocks in block 1 */
			for(i = 0; (i < EDID_NUMBER_MAX_DTD_BLK_1) && (dtd_found); i++) {
				block_ptr = ((u8)p_dis->edid_block[EDID_BLK1_OFFSET_BASE_DTD]) + ((u8)(i * EDID_DTD_BLK_SIZE));
				if((block_ptr + EDID_DTD_BLK_SIZE - 1) < EDID_BLOCK_SIZE) {
					dtd_found = store_dtd_block(p_dis, block_ptr);

				}
			}

			break;

		case EDID_BLOCK_MAP:
			/* BLOCK MAP */
			/* Nothing special to do */
			break;

		}

	}

	return 0;
}

/*============================================================================*/
/* storeDtdBlock                                                              */
/*============================================================================*/

static bool
store_dtd_block
(
    /* Device instance strucure holding block */
    hdmi_txobject_t    *p_dis,
    u8               block_ptr
)
{

	bool    dtd_found = false;

#ifndef DEMO_BUILD

	/* First, select blocks that are DTDs [CEA861C A.2.10] */
	if(((p_dis->edid_block[block_ptr+0] != 0) ||
	    (p_dis->edid_block[block_ptr+1] != 0) ||
	    (p_dis->edid_block[block_ptr+2] != 0) ||
	    (p_dis->edid_block[block_ptr+4] != 0))
	   &&
	   (p_dis->nb_dtdstored < NUMBER_DTD_STORED)) {
		/* Store the first DTD we find, others will be skipped */
		p_dis->edid_dtd[p_dis->nb_dtdstored].u_pixel_clock =
		    ((u16)p_dis->edid_block[block_ptr+1] << 8) |
		    (u16)p_dis->edid_block[block_ptr+0];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_hactive_pixels =
		    (((u16)p_dis->edid_block[block_ptr+4] & 0x00F0) << 4) |
		    (u16)p_dis->edid_block[block_ptr+2];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_hblank_pixels =
		    (((u16)p_dis->edid_block[block_ptr+4] & 0x000F) << 8) |
		    (u16)p_dis->edid_block[block_ptr+3];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vactive_lines =
		    (((u16)p_dis->edid_block[block_ptr+7] & 0x00F0) << 4) |
		    (u16)p_dis->edid_block[block_ptr+5];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vblank_lines =
		    (((u16)p_dis->edid_block[block_ptr+7] & 0x000F) << 8) |
		    (u16)p_dis->edid_block[block_ptr+6];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_hsync_offset =
		    (((u16)p_dis->edid_block[block_ptr+11] & 0x00C0) << 2) |
		    (u16)p_dis->edid_block[block_ptr+8];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_hsync_width =
		    (((u16)p_dis->edid_block[block_ptr+11] & 0x0030) << 4) |
		    (u16)p_dis->edid_block[block_ptr+9];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vsync_offset =
		    (((u16)p_dis->edid_block[block_ptr+11] & 0x000C) << 2) |
		    (((u16)p_dis->edid_block[block_ptr+10] & 0x00F0) >> 4);

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vsync_width =
		    (((u16)p_dis->edid_block[block_ptr+11] & 0x0003) << 4) |
		    ((u16)p_dis->edid_block[block_ptr+10] & 0x000F);

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_himage_size =
		    (((u16)p_dis->edid_block[block_ptr+14] & 0x00F0) << 4) |
		    (u16)p_dis->edid_block[block_ptr+12];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vimage_size =
		    (((u16)p_dis->edid_block[block_ptr+14] & 0x000F) << 8) |
		    (u16)p_dis->edid_block[block_ptr+13];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_hborder_pixels =
		    (u16)p_dis->edid_block[block_ptr+15];

		p_dis->edid_dtd[p_dis->nb_dtdstored].u_vborder_pixels =
		    (u16)p_dis->edid_block[block_ptr+16];

		p_dis->edid_dtd[p_dis->nb_dtdstored].flags = p_dis->edid_block[block_ptr+17];

		p_dis->nb_dtdstored++;

		dtd_found = true;    /* Stop any more DTDs being parsed */
	}
#endif /* DEMO_BUILD */

	return (dtd_found);
}

/*============================================================================*/
/* storeMonitorDescriptor                                                     */
/*============================================================================*/
#ifndef DEMO_BUILD
static bool
store_monitor_descriptor
(
    /* Device instance strucure holding block */
    hdmi_txobject_t    *p_dis,
    u8               block_ptr
)
{

	bool    dtd_found = false;

	/* First, select blocks that are DTDs [CEA861C A.2.10] */
	if((p_dis->edid_block[block_ptr+0] == 0) &&
	   (p_dis->edid_block[block_ptr+1] == 0) &&
	   (p_dis->edid_block[block_ptr+2] == 0)
	  ) {
		if(p_dis->edid_block[block_ptr+3] == EDID_MONITOR_NAME_DESC_DATA_TYPE) {
			if(p_dis->edid_first_monitor_descriptor.b_desc_record == false) {
				p_dis->edid_first_monitor_descriptor.b_desc_record = true;
				lmemcpy(&(p_dis->edid_first_monitor_descriptor.u_monitor_name) ,
				        &(p_dis->edid_block[block_ptr+5]), EDID_MONITOR_DESCRIPTOR_SIZE);
				dtd_found = true;
			} else if((p_dis->edid_other_monitor_descriptor.b_desc_record == false)) {
				p_dis->edid_other_monitor_descriptor.b_desc_record = true;
				lmemcpy(&(p_dis->edid_other_monitor_descriptor.u_other_descriptor) ,
				        &(p_dis->edid_block[block_ptr+5]), EDID_MONITOR_DESCRIPTOR_SIZE);
				dtd_found = true;
			}
		} else if(p_dis->edid_block[block_ptr+3] == EDID_MONITOR_RANGE_DESC_DATA_TYPE) {
			if(p_dis->edid_second_monitor_descriptor.b_desc_record == false) {
				p_dis->edid_second_monitor_descriptor.b_desc_record = true;
				p_dis->edid_second_monitor_descriptor.u_min_vertical_rate = p_dis->edid_block[block_ptr+5];
				p_dis->edid_second_monitor_descriptor.u_max_vertical_rate = p_dis->edid_block[block_ptr+6];
				p_dis->edid_second_monitor_descriptor.u_min_horizontal_rate = p_dis->edid_block[block_ptr+7];
				p_dis->edid_second_monitor_descriptor.u_max_horizontal_rate = p_dis->edid_block[block_ptr+8];
				p_dis->edid_second_monitor_descriptor.u_max_supported_pixel_clk = p_dis->edid_block[block_ptr+9];
				dtd_found = true;
			}
		}
	}

	return (dtd_found);

}
#endif /* DEMO_BUILD */

/*============================================================================*/
/* bslDebugWriteFakeRegPage                                                 */
/*============================================================================*/
#ifndef DEMO_BUILD
error_code_t bsl_debug_write_fake_reg_page(unit_select_t tx_unit)
{
	/* Pointer to Device Instance Structure */
	hdmi_txobject_t *p_dis;
	error_code_t     err;            /* Error code */

	err = check_unit_set_dis(tx_unit, &p_dis);

	p_dis->cur_reg_page = 0x20;

	return err;
}
#endif  /* DEMO_BUILD */

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/
