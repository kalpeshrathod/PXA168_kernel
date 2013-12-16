/*==========================================================================*/
/*     (Copyright (C) 2003 Koninklijke Philips Electronics N.V.             */
/*     All rights reserved.                                                 */
/*     This source code and any compilation or derivative thereof is the    */
/*     proprietary information of Koninklijke Philips Electronics N.V.      */
/*     and is confidential in nature.                                       */
/*     Under no circumstances is this software to be exposed to or placed   */
/*     under an Open Source License of any type without the expressed       */
/*     written permission of Koninklijke Philips Electronics N.V.           */
/*==========================================================================*/
/*
 * Copyright (C) 2000,2001
 *               Koninklijke Philips Electronics N.V.
 *               All Rights Reserved.
 *
 * Copyright (C) 2000,2001 TriMedia Technologies, Inc.
 *               All Rights Reserved.
 *
 *############################################################
 *
 * Module name  : tmNxTypes.h  %version: 7 %
 *
 * Last Update  : %date_modified: Tue Jul  8 18:08:00 2003 %
 *
 * Description: TriMedia/MIPS global type definitions.
 *
 * Document Ref: DVP Software Coding Guidelines Specification
 * DVP/MoReUse Naming Conventions specification
 * DVP Software Versioning Specification
 * DVP Device Library Architecture Specification
 * DVP Board Support Library Architecture Specification
 * DVP Hardware API Architecture Specification
 *
 *
 *############################################################
 * */

#ifndef TMNXTYPES_H
#define TMNXTYPES_H

#include <linux/kernel.h>

#ifndef _TMtypes_h
#define _TMtypes_h

#define false           0
#define true            1

/*----------------------------------------------------------------------------- */
/* Legacy TM Types/Structures (Not necessarily DVP Coding Guideline compliant) */
/* NOTE: For DVP Coding Gudeline compliant code, do not use these types. */

typedef char          *address;        /* Ready for address-arithmetic */
typedef char const    *const_address;
typedef unsigned char  byte;           /* Raw byte */
typedef float          float32;        /* Single-precision float */
typedef double         float64;        /* Double-precision float */
typedef void          *pointer;        /* Pointer to anonymous object */
typedef void const    *const_pointer;
typedef char const    *const_string;

typedef int             endian;
#define BigEndian       0
#define LittleEndian    1

typedef struct version {
	u8 major_version;
	u8 minor_version;
	u16 build_version;
}   version_t, *p_version_t;
#endif /*ndef _TMtypes_h*/

#define TMFL_ENDIAN_BIG               1
#define TMFL_ENDIAN_LITTLE            0
#define TMFL_ENDIAN                   (TMFL_ENDIAN_LITTLE)

/*Define DVP types that are not TCS types.*/
/*
 ** ===== updated from SDE2/2.3_beta/sde_template/inc/nx_types.h =====
 **
 ** NOTE: ibits32/ubits32 types are defined for use with 32 bit bitfields.
 **       this is done because ANSI/ISO compliant compilers require bitfields
 **       to be of type "int" else a large number of compiler warnings will
 **       result.  to avoid the risks associated with redefining int32/u32
 **       to type "int" instead of type "long" (which are the same size on 32
 **       bit cpus) separate 32bit signed/unsigned bitfield types are defined.
 * */
/* FIXME */
typedef signed   int    ibits32;    /* 32 bit   signed integer bitfields */
typedef unsigned int    ubits32;    /* 32 bit unsigned integer bitfields */
typedef ibits32 *p_ibits32;          /* 32 bit   signed integer bitfield ptr */
typedef ubits32 *p_ubits32;          /* 32 bit unsigned integer bitfield ptr */

typedef char   *p_int8;            /*  8 bit   signed integer */
typedef s16  *p_int16;           /* 16 bit   signed integer */
typedef s32  *p_int32;           /* 32 bit   signed integer */
typedef u8  *p_uint8;           /*  8 bit unsigned integer */
typedef u16 *p_uint16;          /* 16 bit unsigned integer */
typedef u32 *p_uint32;          /* 32 bit unsigned integer */
typedef void *p_void;           /* void (typeless) */
typedef float  *p_float;           /* 32 bit floating point */
typedef double  *p_double; /* 32/64 bit floating point */
typedef bool   *p_bool;            /* Boolean (true/false) */
typedef char   *p_char;            /* character, character array ptr */
typedef int    *p_int;             /* machine-natural integer */
typedef uint   *p_uint;            /* machine-natural unsigned integer */

/* Maximum length of device name in all BSP and capability structures */
#define HAL_DEVICE_NAME_LENGTH 16

typedef u32 error_code_t;
typedef u32 progress_code_t;


//-----------------------------------------------------------------------------
// Hardware device power states
//
typedef enum tmPowerState
{
    power_on,                          // Device powered on      (D0 state)
    power_standby,                     // Device power standby   (D1 state)
    power_suspend,                     // Device power suspended (D2 state)
    power_off                          // Device powered off     (D3 state)

}   power_state_t, *p_power_state_t;

/*----------------------------------------------------------------------------- */
/* Software Version Structure */

typedef struct swversion {
	u32      compatibility_nr;        /* Interface compatibility number */
	u32      major_version_nr;         /* Interface major version number */
	u32      minor_version_nr;         /* Interface minor version number */

}   swversion_t, *p_swversion_t;

/*Under the TCS, <tm1/tmBoardDef.h> may have been included by our client. In
 *    order to avoid errors, we take account of this possibility, but in order to
 *    support environments where the TCS is not available, we do not include the
 *    file by name.*/
#ifndef _TMBOARDDEF_H_
#define _TMBOARDDEF_H_

/*----------------------------------------------------------------------------- */
/* HW Unit Selection */

typedef int unit_select_t, *p_unit_select_t;

#define tmUnitNone (-1)
#define tmUnit0    0
#define tmUnit1    1
#define tmUnit2    2
#define tmUnit3    3
#define tmUnit4    4

/*+compatibility*/
#define unitSelect_t       tmUnitSelect_t
#define unit0              tmUnit0
#define unit1              tmUnit1
#define unit2              tmUnit2
#define unit3              tmUnit3
#define unit4              tmUnit4
#define DEVICE_NAME_LENGTH HAL_DEVICE_NAME_LENGTH
/*-compatibility*/

#endif /*ndef _TMBOARDDEF_H_ */

/*----------------------------------------------------------------------------- */
/* Instance handle */

typedef int instance_t, *p_instance_t;

/* Callback function declaration */
typedef void(*p_callback_t)(u32 events, void *p_data, u32 user_data);
#define tmCallback_t ptmCallback_t /*compatibility*/

#endif /*ndef TMNXTYPES_H */
