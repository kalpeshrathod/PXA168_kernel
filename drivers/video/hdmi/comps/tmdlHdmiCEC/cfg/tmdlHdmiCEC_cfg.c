/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          dlHdmiCEC_cfg.c
 *
 * \version       Revision: 1
 *
 * \date          Date:
 *
 * \brief         devlib driver component API for the CEC message
 *
 * \section refs  Reference Documents
 *
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   History:       dlHdmiCEC_cfg.c
 *
 *
 *   \endverbatim
 *
 * */

/*============================================================================*/
/*                             INCLUDE FILES                                  */
/*============================================================================*/
#include "tmdlHdmiCEC_IW.h"
#include "tmNxTypes.h"
#include "tmdlHdmiCEC.h"
#include "tmdlHdmiCEC_cfg.h"

#ifdef   TMFL_OS_WINDOWS    /* OS Windows */
#include "infra_i2c.h"
#else                       /* OS ARM7 */
#include "I2C.h"
#include <LPC21xx.H>
#endif                      /* endif TMFL_OS_WINDOWS */

/******************************************************************************
 ******************************************************************************
 *                 THIS PART CAN BE MODIFIED BY CUSTOMER                      *
 ******************************************************************************
 *****************************************************************************/
/*============================================================================*/
/*                          INTERNAL PROTOTYPE                                */
/*============================================================================*/

#ifdef TMFL_OS_WINDOWS      /* OS Windows */
error_code_t windows_i2c_read_function(bsl_sys_args_t *p_sys_args);
error_code_t windows_i2c_write_function(bsl_sys_args_t *p_sys_args);
#else                       /* OS ARM7 */
error_code_t rtx_i2c_read_function(dl_hdmi_cec_sys_args_t *p_sys_args);
error_code_t rtx_i2c_write_function(dl_hdmi_cec_sys_args_t *p_sys_args);
#endif                      /* endif TMFL_OS_WINDOWS */

/*============================================================================*/
/*                                MACRO                                       */
/*============================================================================*/

/* macro for quick error handling */
#define RETIF(cond, rslt) if ((cond)){return (rslt);}

/*============================================================================*/
/*                          TYPES DECLARATIONS                                */
/*============================================================================*/

/*============================================================================*/
/*                       CONSTANTS DECLARATIONS                               */
/*============================================================================*/

/* Configuration for unit 0 *************************** */
#ifdef TMFL_OS_WINDOWS         /* OS Windows */
#define COMMAND_TASK_PRIORITY_0  THREAD_PRIORITY_HIGHEST
#else                          /* OS ARM7 */
#define COMMAND_TASK_PRIORITY_0  250
#endif                        /* endif TMFL_OS_WINDOWS */
/* stack size of the command tasks */
#define COMMAND_TASK_STACKSIZE_0 128
/* size of the message queues for command tasks */
#define COMMAND_TASK_QUEUESIZE_0 8
/* I2C adress of the unit */
#ifdef TMFL_TDA9996
#define UNIT_I2C_ADDRESS_0 0x60 /* I2C Address of TDA9950 */
#else
#define UNIT_I2C_ADDRESS_0 0x34 /* I2C Address of TDA9950 */
#endif

/* Configuration for unit 1 *************************** */
/* priority of the command tasks */
#ifdef TMFL_OS_WINDOWS         /* OS Windows */
#define COMMAND_TASK_PRIORITY_1  THREAD_PRIORITY_HIGHEST
#else                          /* OS ARM7 */
#define COMMAND_TASK_PRIORITY_1  250
#endif                        /* endif TMFL_OS_WINDOWS */
/* stack size of the command tasks */
#define COMMAND_TASK_STACKSIZE_1 128
/* size of the message queues for command tasks */
#define COMMAND_TASK_QUEUESIZE_1 8
/* I2C adress of the unit */
#ifdef TMFL_TDA9996
#define UNIT_I2C_ADDRESS_1 0x60 /* I2C Address of TDA9950 */
#else
#define UNIT_I2C_ADDRESS_1 0x34 /* I2C Address of TDA9950 */
#endif

/*============================================================================*/
/*                        DEFINES DECLARATIONS                                */
/*============================================================================*/

/*============================================================================*/
/*                       VARIABLES DECLARATIONS                               */
/*============================================================================*/

/**
 * \brief List of the capabilities to be enabled by the device library
 * */
dl_hdmi_cec_capabilities_t ceccapabilities_list = {DL_HDMICEC_DEVICE_UNKNOWN, cec_version_1_3a};

/**
 * \brief Configuration Tables. This table can be modified by the customer
 *            to choose its prefered configuration
 * */

#ifdef TMFL_OS_WINDOWS
dl_hdmi_cec_driver_config_table_t cecdriver_config_table[MAX_UNITS] = {
	{
		COMMAND_TASK_PRIORITY_0,
		COMMAND_TASK_STACKSIZE_0,
		COMMAND_TASK_QUEUESIZE_0,
		UNIT_I2C_ADDRESS_0,
		windows_i2c_read_function,
		windows_i2c_write_function,
		&ceccapabilities_list
	}
};
#else
dl_hdmi_cec_driver_config_table_t cecdriver_config_table[MAX_UNITS] = {
	{
		COMMAND_TASK_PRIORITY_0,
		COMMAND_TASK_STACKSIZE_0,
		COMMAND_TASK_QUEUESIZE_0,
		UNIT_I2C_ADDRESS_0,
		rtx_i2c_read_function,
		rtx_i2c_write_function,
		&ceccapabilities_list
	}
};
#endif

#ifdef TMFL_OS_WINDOWS
/*============================================================================*/
/*                              FUNCTIONS                                     */
/*============================================================================*/

/**
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C read structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
windows_i2c_read_function
(
    dl_hdmi_cec_sys_args_t        *p_sys_args
)
{
	error_code_t  err;

#ifdef TMFL_TDA9996
	err = i2c_read(reg_tda_9996, (bsl_hdmi_sys_args_t *) p_sys_args);
#else
	err = i2c_read(reg_tda_9950, (bsl_hdmi_sys_args_t *) p_sys_args);
#endif

	return err;
}
/**
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C write structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
windows_i2c_write_function
(
    dl_hdmi_cec_sys_args_t *p_sys_args
)
{
	error_code_t   err;

#ifdef TMFL_TDA9996
	err = i2c_write(reg_tda_9996, (bsl_hdmi_sys_args_t *) p_sys_args);
#else
	err = i2c_write(reg_tda_9950, (bsl_hdmi_sys_args_t *) p_sys_args);
#endif

	return err;
}
#else

/*============================================================================*/

/**
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C read structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
rtx_i2c_read_function
(
    dl_hdmi_cec_sys_args_t        *p_sys_args
)
{
	error_code_t               err;

	err = i2c_read(reg_tda_9950, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}
/**
 *    \brief Write to BSL driver through I2C bus
 *
 *    \param pSysArgs Pointer to the I2C write structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
error_code_t
rtx_i2c_write_function
(
    dl_hdmi_cec_sys_args_t *p_sys_args
)
{
	error_code_t               err;

	err = i2c_write(reg_tda_9950, (bsl_hdmi_sys_args_t *) p_sys_args);

	return err;
}

#endif

/******************************************************************************
 ******************************************************************************
 *                THIS PART MUST NOT BE MODIFIED BY CUSTOMER                  *
 ******************************************************************************
 *****************************************************************************/

/**
 *    \brief This function allows to the main driver to retrieve its
 *           configuration parameters.
 *
 *    \param pConfig Pointer to the config structure
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_UNIT_NUMBER: the unit number is wrong or
 *              the receiver instance is not initialised
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t dl_hdmi_cec_cfg_get_config
(
    unit_select_t                 unit,
    dl_hdmi_cec_driver_config_table_t *p_config
)
{
	/* check if unit number is in range */
	RETIF((unit < 0) || (unit >= MAX_UNITS), ERR_DLHDMICEC_BAD_UNIT_NUMBER)

	/* check if pointer is NULL */
	RETIF(p_config == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	*p_config = cecdriver_config_table[unit];

	return(0);
};
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
