/**
 * Copyright (C) 2006 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          dlHdmiCEC_local.c
 *
 * \version       $Revision:  $
 *
 * \date          $Date: $
 *
 * \brief         dev lib driver component for the CEC messages
 *
 * \section refs  Reference Documents
 * \section info  Change Information
 *
 * \verbatim
 *   $History: dlHdmiCEC_local.c $
 *
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
#ifdef __LINUX_ARM_ARCH__
#include <linux/kernel.h>
#else
#include <string.h>
#endif
#include "tmdlHdmiCEC.h"
#include "tmdlHdmiCEC_local.h"

/*============================================================================*/
/*                   MACRO DEFINITIONS                                        */
/*============================================================================*/

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
/* getCecHwRegisters                                                             */
/*============================================================================*/
error_code_t
get_cec_hw_registers
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8  reg_addr,
    u8  *p_data,
    u16 len_data
)
{
	error_code_t        err;     /* Error code */
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Get I2C register range - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = (u8)len_data;
	sys_args.p_data           = p_data;
	err = p_dis->i2c_read_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_READ;
}

/*============================================================================*/
/* getCecHwRegister                                                              */
/*============================================================================*/
error_code_t
get_cec_hw_register
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8 reg_addr,
    u8 *p_reg_value
)
{
	error_code_t        err;     /* Error code */
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Get I2C register - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = p_reg_value;
	err = p_dis->i2c_read_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_READ;

}

/*============================================================================*/
/* setCecHwRegisters                                                             */
/*============================================================================*/
error_code_t
set_cec_hw_registers
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8  reg_addr,
    u8  *p_data,
    u16 len_data
)
{
	error_code_t        err;     /* Error code */
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Write to I2C register range - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = (u8)len_data;
	sys_args.p_data           = p_data;
	err = p_dis->i2c_write_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_WRITE;
}

/*============================================================================*/
/* setCecHwRegisterMsbLsb                                                        */
/*============================================================================*/
error_code_t
set_cec_hw_register_msb_lsb
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8  reg_addr,
    u16 reg_word
)
{
	error_code_t        err;        /* Error code */
	/* The bytes from regWord                  */
	u8                msb_lsb[2];
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Unpack regWord bytes, MSB first */
	msb_lsb[0] = (u8)(reg_word >> 8);
	msb_lsb[1] = (u8)(reg_word & 0xFF);

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 2;
	sys_args.p_data           = &msb_lsb[0];
	err = p_dis->i2c_write_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_WRITE;
}

/*============================================================================*/
/* setCecHwRegister                                                              */
/*============================================================================*/
error_code_t
set_cec_hw_register
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8 reg_addr,
    u8 reg_value
)
{
	error_code_t        err;     /* Error code */
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = &reg_value;
	err = p_dis->i2c_write_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_WRITE;
}

/*============================================================================*/
/*  setCecHwRegisterField                                                        */
/*============================================================================*/
error_code_t
set_cec_hw_register_field
(
    dl_hdmi_cec_driver_config_table_t *p_dis,
    u8 reg_addr,
    u8 field_mask,
    u8 field_value
)
{
	error_code_t        err;        /* Error code */
	/* The register's current value            */
	u8                reg_value;
	/* Arguments passed to system function   */
	dl_hdmi_cec_sys_args_t sys_args;

	/* Read I2C register value.
	 * All bitfield registers are either shadowed or can be read.
	 *	 */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = &reg_value;
	err = p_dis->i2c_read_function(&sys_args);
	RETIF(err != 0, ERR_DLHDMICEC_I2C_READ)

	/* Reset register bits that are set in the mask */
	reg_value = reg_value & (u8)(~field_mask);

	/* Shift the field value left to align its bits with the mask */
	field_value <<= k_mask_to_shift[field_mask];

	/* Reset shifted field bits that are not set in the mask */
	field_value &= field_mask;

	/* Set the shifted bitfield */
	reg_value |= field_value;

	/* Write to I2C - all non-OK results are errors */
	sys_args.slave_addr       = p_dis->i2c_address;
	sys_args.first_register   = reg_addr;
	sys_args.len_data         = 1;
	sys_args.p_data           = &reg_value;
	err = p_dis->i2c_write_function(&sys_args);
	return (err == 0) ? 0 : ERR_DLHDMICEC_I2C_WRITE;
}

/*============================================================================*/
/*                   STATIC FUNCTION DEFINTIONS                               */
/*============================================================================*/

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/
