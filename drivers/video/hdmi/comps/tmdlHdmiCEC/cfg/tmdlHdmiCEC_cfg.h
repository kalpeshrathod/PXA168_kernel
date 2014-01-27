/**
 * Copyright (C) 2006 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          dlHdmiCEC_cfg.h
 *
 * \version       $Revision: 1 $
 *
 * \date          $Date:  $
 *
 * \brief         devlib driver component API for the CEC messages
 *
 * \section refs  Reference Documents
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History:  dlHdmiCEC_cfg.h
 *
 *
 *   \endverbatim
 *
 * */
/******************************************************************************
 ******************************************************************************
 *                THIS FILE MUST NOT BE MODIFIED BY CUSTOMER                  *
 ******************************************************************************
 *****************************************************************************/

#ifndef DLHDMICEC_CFG_H
#define DLHDMICEC_CFG_H

#include "tmNxTypes.h"
#include "tmdlHdmiCEC_Types.h"
#include "tmdlHdmiCEC_Functions.h"

/* Number of HW units supported by SW driver */
#define MAX_UNITS 1

#ifndef TMFL_CEC_AVAILABLE
typedef struct _bsl_sys_args_t {
	u8 slave_addr;
	u8 first_register;
	u8 len_data;
	u8 *p_data;
} bsl_sys_args_t;
#endif

/*============================================================================*/
/*                          TYPES DECLARATIONS                                */
/*============================================================================*/
typedef struct {
	u8                         command_task_priority;
	u8                         command_task_stack_size;
	u8                         command_task_queue_size;
	u8                         i2c_address;
	ptmdl_hdmi_cec_sys_func_t         i2c_read_function;
	ptmdl_hdmi_cec_sys_func_t         i2c_write_function;
	dl_hdmi_cec_capabilities_t     *p_capabilities_list;
} dl_hdmi_cec_driver_config_table_t;

/*============================================================================*/
/*                       FUNCTIONS DECLARATIONS                               */
/*============================================================================*/

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
);

#endif /* DLHDMICEC_CFG_H */

/*============================================================================*/
/*                               END OF FILE                                  */
/*============================================================================*/
