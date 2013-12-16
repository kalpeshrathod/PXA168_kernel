/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          bsl_local.c
 *
 * \version       $Revision: 15 $
 *
 * \date          $Date: 29/10/07 14:11 $
 *
 * \brief         BSL driver component for the TDA998x HDMI Transmitter
 *
 * \section refs  Reference Documents
 * HDMI Driver - Outline Architecture.doc
 * HDMI Driver - bsl - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *   $History: bsl_local.c $
 *
 * *****************  Version 15  *****************
 * User: B.Vereecke     Date: 29/10/07   Time: 14:11
 * Updated in $/Source/bsl/src
 * PR852 : remove external library dependancy
 *
 * *****************  Version 14  *****************
 * User: B.Vereecke    Date: 08/08/07   Time: 15:40
 * Updated in $/Source/bsl/Src
 * PR 563 : Unused shadow value for register
 *          E_REG_P00_INT_FLAGS_2_RW
 *
 * *****************  Version 13  *****************
 * User: Burnouf       Date: 18/04/07   Time: 15:50
 * Updated in $/Source/bsl/Src
 * PR 50 : Manage state ST_AWAIT_RX_SENSE for TDA9981
 *
 * *****************  Version 12  *****************
 * User: Mayhew       Date: 21/09/06   Time: 15:50
 * Updated in $/Source/bsl/Src
 * Cut code size in demo by using RETIF_BADPARAM
 *
 * *****************  Version 10  *****************
 * User: Mayhew       Date: 15/09/06   Time: 16:08
 * Updated in $/Source/bsl/Src
 * Reduce code size in setState by using local copies of DIS
 *
 * *****************  Version 8  *****************
 * User: Mayhew       Date: 30/06/06   Time: 13:23
 * Updated in $/Source/bsl/Src
 * EV_HDCP_BKSV_SECURE renamed to EV_HDCP_BKSV_REPEAT
 *
 * *****************  Version 6  *****************
 * User: Mayhew       Date: 6/06/06    Time: 13:40
 * Updated in $/Source/bsl/Src
 * Add checkUnitSetDis to reduce code size in most APIs
 *
 * *****************  Version 4  *****************
 * User: Djw          Date: 24/05/06   Time: 11:20
 * Updated in $/Source/bsl/Src
 * Added E_PAGE_10 for new Infoframe registers in N4.
 *
 * *****************  Version 3  *****************
 * User: Mayhew       Date: 10/05/06   Time: 17:09
 * Updated in $/Source/bsl/Src
 * Add HDCP state handling
 *
 * *****************  Version 2  *****************
 * User: Djw          Date: 20/04/06   Time: 17:32
 * Updated in $/Source/bsl/Src
 * Modification to prevent reading of shadow copies of interrupt flag
 * registers.
 *
 * *****************  Version 1  *****************
 * User: Mayhew       Date: 4/04/06    Time: 16:30
 * Created in $/Source/bsl/Src
 * Driver local code & data phase 2
 *   \endverbatim
 *
 * */

/*============================================================================*/
/*                   FILE CONFIGURATION                                       */
/*============================================================================*/

/* Defining this symbol on the compiler command line excludes some API checks */
/* #define NO_RETIF_BADPARAM */

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

/*============================================================================*/
/*                   TYPE DEFINITIONS                                         */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC VARIABLE DEFINITIONS                              */
/*============================================================================*/

/** The array of object instances for all concurrently supported HDMI
 *  Transmitter units
 * */
RAM_DAT hdmi_txobject_t g_hdmi_tx_instance[HDMITX_UNITS_MAX];

/**
 * Lookup table to map register page index to actual page number
 * */
CONST_DAT u8 k_page_index_to_page[E_PAGE_NUM] = {
	0x00,   /* E_PAGE_00 */
	0x01,   /* E_PAGE_01 */
	0x02,   /* E_PAGE_02 */
	0x10,   /* E_PAGE_10 */
	0x11,   /* E_PAGE_11 */
	0x12    /* E_PAGE_12 */
};

/*============================================================================*/
/*                   STATIC VARIABLE DECLARATIONS                             */
/*============================================================================*/

/**
 * Lookup table to map an 8-bit mask to a number of left shifts
 * needed to shift a value starting at bit 0 onto the mask.
 * Indexed by mask 0-255. For example, mask 0x00 and 0x01 need
 * no shift, mask 0x02 needs one shift, mask 0x03 needs no shift,
 * mask 0x04 needs 2 shifts, etc.
 * Rows were formatted by "HDMI Driver - Register List.xls" and pasted here
 * */
static CONST_DAT u8 k_mask_to_shift[256] = {
	/* Mask index: */
	/*x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF        */
	0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 0x */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 1x */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 2x */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 3x */
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 4x */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 5x */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 6x */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 7x */
	7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 8x */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 9x */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* Ax */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* Bx */
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* Cx */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* Dx */
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* Ex */
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0  /* Fx */
};

/*============================================================================*/
/*                   STATIC FUNCTION DECLARATIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC FUNCTION DEFINITIONS                              */
/*============================================================================*/

/*============================================================================*/
/* checkUnitSetDis                                                            */
/*============================================================================*/
error_code_t
check_unit_set_dis
(
    unit_select_t	 tx_unit,
    hdmi_txobject_t **pp_dis
)
{
	/* Return error if unit numbr is out of range */
	RETIF(tx_unit < unit0, ERR_HDMI_BAD_UNIT_NUMBER)
	RETIF(tx_unit >= HDMITX_UNITS_MAX, ERR_HDMI_BAD_UNIT_NUMBER)

	/* Point to unit's Device Instance Structure */
	*pp_dis = &g_hdmi_tx_instance[tx_unit];

	/* Return if this device instance is not initialised */
	RETIF(!(*pp_dis)->b_initialized, ERR_HDMI_NOT_INITIALIZED)

	return 0;
}

/*============================================================================*/
/* getHwRegisters                                                             */
/*============================================================================*/
error_code_t
get_hw_registers
(
    hdmi_txobject_t *p_dis,
    u16 reg_shad_page_addr,
    u8 *p_data,
    u16 len_data
)
{
	error_code_t   err;        /* Error code */
	u8           reg_shad;    /* The index to the register's shadow copy */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack 1st register's shadow index, page index and address */
	reg_shad = SPA2SHAD(reg_shad_page_addr);
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Check length does not overflow page */
	RETIF_BADPARAM((reg_addr + len_data) > E_REG_CURPAGE_ADR_W)

	/* Check 1st reg does not have a shadow - whole range assumed likewise */
	RETIF_BADPARAM(reg_shad != E_SNONE)

	/* Set page register if required */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* Get I2C register range - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->u_hw_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = (u8)len_data;
	sys_args.p_data           = p_data;
	err = p_dis->sys_func_read(&sys_args);
	return (err == 0) ? 0 : ERR_HDMI_I2C_READ;
}

/*============================================================================*/
/* getHwRegister                                                              */
/*============================================================================*/
error_code_t
get_hw_register
(
    hdmi_txobject_t *p_dis,
    u16  reg_shad_page_addr,
    u8   *p_reg_value
)
{
	error_code_t   err;        /* Error code */
	u8           reg_shad;    /* The index to the register's shadow copy */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack register shadow index, page index and address */
	reg_shad = SPA2SHAD(reg_shad_page_addr);
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Set page register if required */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	if((reg_shad != E_SNONE)
	   && (reg_shad_page_addr != E_REG_P00_INT_FLAGS_0_RW)
	   && (reg_shad_page_addr != E_REG_P00_INT_FLAGS_1_RW)
#ifdef TMFL_TDA9981_SUPPORT
	   && (reg_shad_page_addr != E_REG_P00_INT_FLAGS_2_RW)
#endif /* TMFL_TDA9981_SUPPORT */
	  ) {
		/* Get shadow copy - shadowed registers can't be read */
		/* Don't read shadow copy of interrupt status flags! */
		*p_reg_value = p_dis->shadow_reg[reg_shad];
		return 0;
	} else {
		/* Get I2C register - all non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = reg_addr;
		sys_args.len_data         = 1;
		sys_args.p_data           = p_reg_value;
		err = p_dis->sys_func_read(&sys_args);
		return (err == 0) ? 0 : ERR_HDMI_I2C_READ;
	}
}

/*============================================================================*/
/* setHwRegisters                                                             */
/*============================================================================*/
error_code_t
set_hw_registers
(
    hdmi_txobject_t *p_dis,
    u16 reg_shad_page_addr,
    u8 *p_data,
    u16 len_data
)
{
	error_code_t   err;        /* Error code */
	u8           reg_shad;    /* The index to the register's shadow copy */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack 1st register's shadow index, page index and address */
	reg_shad = SPA2SHAD(reg_shad_page_addr);
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Check length does not overflow page */
	RETIF_BADPARAM((reg_addr + len_data) > E_REG_CURPAGE_ADR_W)

	/* Check 1st reg does not have a shadow - whole range assumed likewise */
	RETIF_BADPARAM(reg_shad != E_SNONE)

	/* Set page register if required - whole range is on same page */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* Write to I2C register range - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->u_hw_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = (u8)len_data;
	sys_args.p_data           = p_data;
	err = p_dis->sys_func_write(&sys_args);
	return (err == 0) ? 0 : ERR_HDMI_I2C_WRITE;
}

/*============================================================================*/
/* setHwRegisterMsbLsb                                                        */
/*============================================================================*/
error_code_t
set_hw_register_msb_lsb
(
    hdmi_txobject_t *p_dis,
    u16  reg_shad_page_addr,
    u16  reg_word
)
{
	error_code_t   err;        /* Error code */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	u8           msb_lsb[2];  /* The bytes from regWord                  */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack register shadow index, page index and address */
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Unpack regWord bytes, MSB first */
	msb_lsb[0] = (u8)(reg_word >> 8);
	msb_lsb[1] = (u8)(reg_word & 0xFF);

	/* Set page register if required */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* No word registers are shadowed */

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->u_hw_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 2;
	sys_args.p_data           = &msb_lsb[0];
	err = p_dis->sys_func_write(&sys_args);
	return (err == 0) ? 0 : ERR_HDMI_I2C_WRITE;
}

/*============================================================================*/
/* setHwRegister                                                              */
/*============================================================================*/
error_code_t
set_hw_register
(
    hdmi_txobject_t *p_dis,
    u16  reg_shad_page_addr,
    u8   reg_value
)
{
	error_code_t   err;        /* Error code */
	u8           reg_shad;    /* The index to the register's shadow copy */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack register shadow index, page index and address */
	reg_shad = SPA2SHAD(reg_shad_page_addr);
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Set page register if required */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	/* Set shadow copy */
	if(reg_shad != E_SNONE) {
		p_dis->shadow_reg[reg_shad] = reg_value;
	}

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->u_hw_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = &reg_value;
	err = p_dis->sys_func_write(&sys_args);
	return (err == 0) ? 0 : ERR_HDMI_I2C_WRITE;
}

/*============================================================================*/
/*  setHwRegisterField                                                        */
/*============================================================================*/
error_code_t
set_hw_register_field
(
    hdmi_txobject_t *p_dis,
    u16  reg_shad_page_addr,
    u8   field_mask,
    u8   field_value
)
{
	error_code_t   err;        /* Error code */
	u8           reg_shad;    /* The index to the register's shadow copy */
	u8           reg_page;    /* The index to the register's page        */
	u8           reg_addr;    /* The register's address on the page      */
	u8           new_reg_page; /* The register's new page number          */
	u8           reg_value;   /* The register's current value            */
	bsl_sys_args_t sys_args; /* Arguments passed to system function   */

	/* Unpack register shadow index, page index and address */
	reg_shad = SPA2SHAD(reg_shad_page_addr);
	reg_page = SPA2PAGE(reg_shad_page_addr);
	reg_addr = SPA2ADDR(reg_shad_page_addr);
	new_reg_page = k_page_index_to_page[reg_page];

	/* Set page register if required */
	if(p_dis->cur_reg_page != new_reg_page) {
		/* All non-OK results are errors */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = E_REG_CURPAGE_ADR_W;
		sys_args.len_data         = 1;
		sys_args.p_data           = &new_reg_page;
		err = p_dis->sys_func_write(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_WRITE)
		p_dis->cur_reg_page = new_reg_page;
	}

	if(reg_shad != E_SNONE) {
		/* Get shadow copy */
		reg_value = p_dis->shadow_reg[reg_shad];
	} else {
		/* Read I2C register value.
		 * All bitfield registers are either shadowed or can be read.
		 *		 */
		sys_args.slave_addr       = p_dis->u_hw_address;
		sys_args.first_register   = reg_addr;
		sys_args.len_data         = 1;
		sys_args.p_data           = &reg_value;
		err = p_dis->sys_func_read(&sys_args);
		RETIF(err != 0, ERR_HDMI_I2C_READ)
	}

	/* Reset register bits that are set in the mask */
	reg_value = reg_value & (u8)(~field_mask);

	/* Shift the field value left to align its bits with the mask */
	field_value <<= k_mask_to_shift[field_mask];

	/* Reset shifted field bits that are not set in the mask */
	field_value &= field_mask;

	/* Set the shifted bitfield */
	reg_value |= field_value;

	/* Set shadow copy */
	if(reg_shad != E_SNONE) {
		p_dis->shadow_reg[reg_shad] = reg_value;
	}

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->u_hw_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = &reg_value;
	err = p_dis->sys_func_write(&sys_args);
	return (err == 0) ? 0 : ERR_HDMI_I2C_WRITE;
}

/*============================================================================*/
/* setHwRegisterFieldTable                                                    */
/*============================================================================*/
error_code_t
set_hw_register_field_table
(
    hdmi_txobject_t *p_dis,
    const hdmi_tx_reg_mask_val_t *p_table
)
{
	error_code_t err; /* Error code */
	int           i;   /* Table index */

	/* Set register, mask and value from table until terminator is reached */
	for(i = 0; p_table[i].reg > 0; i++) {
		err = set_hw_register_field(p_dis, p_table[i].reg, p_table[i].mask, p_table[i].val);
		RETIF(err != 0, err)
	}
	return 0;
}

/*============================================================================*/
/* setState                                                                   */
/*============================================================================*/
error_code_t
set_state
(
    hdmi_txobject_t   *p_dis,
    bsl_event_t  event
)
{
	bsl_state_t state = p_dis->state;
	u8 n_ignored_events = p_dis->n_ignored_events;

	switch(state) {
	case ST_UNINITIALIZED:
		switch(event) {
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_PLUGGEDIN:
			state = ST_AWAIT_EDID;
			break;
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_DISCONNECTED:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_PLUGGEDIN:
			state = ST_AWAIT_EDID;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_AWAIT_EDID:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_GETBLOCKDATA:
			state = ST_AWAIT_RX_SENSE;
			break;
#else /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		case EV_GETBLOCKDATA:
			state = ST_SINK_CONNECTED;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
	case ST_AWAIT_RX_SENSE:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_SINKON:
			state = ST_SINK_CONNECTED;
			break;
		default:
			n_ignored_events++;
			break;
		}
		break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
	case ST_SINK_CONNECTED:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_SETINOUT:
			state = ST_VIDEO_NO_HDCP;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_VIDEO_NO_HDCP:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_OUTDISABLE:
			state = ST_SINK_CONNECTED;
			break;
		case EV_HDCP_RUN:
			state = ST_HDCP_WAIT_RX;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_HDCP_WAIT_RX:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_HDCP_BKSV_NREPEAT:
			state = ST_HDCP_AUTHENTICATED;
			break;
		case EV_HDCP_BKSV_REPEAT:
			state = ST_HDCP_WAIT_BSTATUS;
			break;
		case EV_HDCP_BKSV_NSECURE:
			state = ST_HDCP_WAIT_RX;
			break;
		case EV_HDCP_T0:
			state = ST_HDCP_WAIT_RX;
			break;
		case EV_HDCP_STOP:
			state = ST_VIDEO_NO_HDCP;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_HDCP_WAIT_BSTATUS:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_HDCP_BSTATUS_GOOD:
			state = ST_HDCP_WAIT_SHA_1;
			break;
		case EV_HDCP_T0:
			state = ST_HDCP_WAIT_RX;
			break;
		case EV_HDCP_STOP:
			state = ST_VIDEO_NO_HDCP;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_HDCP_WAIT_SHA_1:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_HDCP_KSV_SECURE:
			state = ST_HDCP_AUTHENTICATED;
			break;
		case EV_HDCP_T0:
			state = ST_HDCP_WAIT_RX;
			break;
		case EV_HDCP_STOP:
			state = ST_VIDEO_NO_HDCP;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_HDCP_AUTHENTICATED:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_STANDBY:
			state = ST_STANDBY;
			break;
		case EV_HDCP_T0:
			state = ST_HDCP_WAIT_RX;
			break;
		case EV_HDCP_STOP:
			state = ST_VIDEO_NO_HDCP;
			break;
#if defined (TMFL_TDA9981_SUPPORT) && defined(TMFL_RX_SENSE_ON)
		case EV_SINKOFF:
			state = ST_AWAIT_RX_SENSE;
			break;
#endif /* TMFL_TDA9981_SUPPORT & TMFL_RX_SENSE_ON */
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_STANDBY:
		switch(event) {
		case EV_DEINIT:
			state = ST_UNINITIALIZED;
			break;
		case EV_RESUME_UNPLUGGED:
			state = ST_DISCONNECTED;
			break;
		case EV_RESUME_PLUGGEDIN:
			state = ST_AWAIT_EDID;
			break;
		default:
			n_ignored_events++;
			break;
		}
		break;
	case ST_INVALID:
		n_ignored_events++;
		break;
	}
	p_dis->state = state;
	p_dis->n_ignored_events = n_ignored_events;
	return 0;
}

/*============================================================================*/
/* lmemcpy                                                                     */
/*============================================================================*/
error_code_t
lmemcpy
(
    void *p_table1,
    const void *p_table2,
    uint size
)
{
	char *ptr_source = (char *) p_table1;
	char *end_source = (char *)p_table1 + size;
	char *ptr_dest = (char *)p_table2;

	RETIF_BADPARAM(p_table1 == NULL)
	RETIF_BADPARAM(p_table2 == NULL)

	while(end_source > ptr_source) {
		*(ptr_source++) = *(ptr_dest++);
	}
	return 0;
}

/*============================================================================*/
/* lmemset                                                                     */
/*============================================================================*/
error_code_t
lmemset
(
    void *p_table1,
    const u8 value,
    uint size
)
{
	char *ptr_source = (char *) p_table1;
	char *end_source = (char *)p_table1 + size;

	RETIF_BADPARAM(p_table1 == NULL)

	while(end_source > ptr_source) {
		*(ptr_source++) = value;
	}
	return 0;
}

/*============================================================================*/
/*                   STATIC FUNCTION DEFINTIONS                               */
/*============================================================================*/

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/
