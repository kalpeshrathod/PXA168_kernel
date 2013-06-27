/*============================================================================= */
/* Copyright (C) 2007 NXP N.V., All Rights Reserved. */
/* This source code and any compilation or derivative thereof is the proprietary */
/* information of NXP N.V. and is confidential in nature. Under no circumstances */
/* is this software to be  exposed to or placed under an Open Source License of */
/* any type without the expressed written permission of NXP N.V. */
/*============================================================================= */
/*!
 *    \file    dlHdmiCEC.c
 *
 *    \version 1.0
 *
 *    \date    24/07/2007
 *
 *    \brief   devlib driver component API for the CEC features.
 *
 *    \section refs  Reference Documents
 *             TDA998X Driver - tx - SCS.doc
 *    \note    None.
 *
 *    HISTORY :
 *    \verbatim
 *     Date          Modified by CRPRNr  TASKNr  Maintenance description
 *     -------------|-----------|-------|-------|-----------------------------------
 *     24/07/2007   |  F.G      |       |       | Creation.
 *     -------------|-----------|-------|-------|-----------------------------------
 *    \endverbatim
 * */
/*========================================================================== */

/*============================================================================*/
/*                   FILE CONFIGURATION                                       */
/*============================================================================*/

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
#include <stdio.h>
#endif
#include "tmdlHdmiCEC_IW.h"
#include "tmdlHdmiCEC_cfg.h"
#include "tmdlHdmiCEC.h"
#include "tmdlHdmiCEC_local.h"

/*============================================================================*/
/*                   MACRO DEFINITIONS                                        */
/*============================================================================*/
#ifdef __LINUX_ARM_ARCH__
#define DV_DBG_PRINT printk
#else
#define DV_DBG_PRINT printf
#endif

/*============================================================================*/
/*                   TYPE DEFINITIONS                                         */
/*============================================================================*/

/*============================================================================*/
/*                   PUBLIC VARIABLE DEFINITIONS                              */
/*============================================================================*/
dl_hdmi_cec_unit_config_t unit_table[MAX_UNITS] = {
	{0, CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST, false, DL_HDMICEC_DEVICE_TDA9950, CEC_STATE_NOT_INITIALIZED, 0}
};

dl_hdmi_cec_driver_config_table_t gtmdl_hdmi_cec_driver_config_table[MAX_UNITS];

dl_hdmi_cec_save_message_t gtmdl_hdmi_cec_driver_save_message;

/*============================================================================*/
/*                   STATIC CONSTANT DECLARATIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC VARIABLE DECLARATIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                   STATIC FUNCTION DECLARATIONS                             */
/*============================================================================*/

/*============================================================================*/
/*                              FUNCTIONS                                     */
/*============================================================================*/

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetSWVersion( )
 *    \brief Get the software version of the driver.
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
)
{
	/* check that input pointer is not NULL */
	RETIF(p_swversion == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	/* copy SW version */
	p_swversion->compatibility_nr = VERSION_COMPATIBILITY;
	p_swversion->major_version_nr  = VERSION_MAJOR;
	p_swversion->minor_version_nr  = VERSION_MINOR;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetNumberOfUnits( )
 *    \brief Get the number of available CEC devices in the system.
 *           A unit directly represents a physical device.
 *
 *    \param pUnitCount Pointer to the number of available units.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_number_of_units
(
    u32  *p_unit_count
)
{
	/* check that input pointer is not NULL */
	RETIF(p_unit_count == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	/* copy the maximum number of units */
	*p_unit_count = MAX_UNITS;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetInstanceSetup( )
 *    \brief Get instance setup parameters.
 *
 *    \param instance   Instance identifier.
 *    \param pSetupInfo Pointer to the structure that will receive setup
 *                      parameters
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_get_instance_setup
(
    instance_t                instance,
    ptmdl_hdmi_cec_instance_setup_t p_setup_info
)
{
	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check that input pointer is not NULL */
	RETIF(p_setup_info == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_setup_info->device_logical_address  = unit_table[instance].device_logical_address;

	return(0);
}

/*=========================	================================================= */
/*!
 *    \fn tmErrorCode_t dlHdmiCecHandleInterrupt( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_read_buffer[19] ;   /* I2C Read data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	/* Pointer to Cec Object */
	dl_hdmi_cec_unit_config_t        *p_cec_object;
	dl_hdmi_cec_frame_format_t  read_frame;
	dl_hdmi_cec_save_message_t  last_send_message;
	int i;

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];
	p_cec_object = &unit_table[instance];

#ifdef TMFL_TDA9989
	/*Check if pending CEC interruption */
	err = get_cec_hw_registers(p_dis, E_REG_CEC_INT, i2c_read_buffer, 1);
	RETIF(err != 0, err)

	if((i2c_read_buffer[0] & CEC_INT_MASK) == 0x00) {
		/*No CEC interruption pending. */
		return 0;
	}
#endif

	err = get_cec_hw_registers(p_dis, E_REG_CDR0, i2c_read_buffer, 19);
	RETIF(err != 0, err)

	/*Fill Frame structure with read data*/

	/* Case of Receiving CECData.cnf*/
	/*Inform Success or reason of failure of CEC message sending*/
	if(i2c_read_buffer[1] == 0x01) {
		/* Get Infos of last message send */
		get_cec_last_message(&last_send_message);

		if(last_send_message.message_type_polling) {
			read_frame.frame_byte_count = i2c_read_buffer[0];
			read_frame.address_byte = last_send_message.address_byte;
			read_frame.data_bytes[0] = i2c_read_buffer[2];
		} else {
			read_frame.frame_byte_count = i2c_read_buffer[0] + 1;
			read_frame.address_byte = last_send_message.address_byte;
			read_frame.data_bytes[0] = i2c_read_buffer[2];
			read_frame.data_bytes[1] = last_send_message.opcode;
		}

		p_cec_object->message_callback(DL_HDMICEC_CALLBACK_STATUS
		                               , (void *) &read_frame, read_frame.frame_byte_count);
	}

	/* Case of Receiving CECData.ind*/
	/*Give receive data from CEC bus*/
	if(i2c_read_buffer[1] == 0x81) {
		read_frame.frame_byte_count = i2c_read_buffer[0];
		read_frame.address_byte = i2c_read_buffer[2];
		for(i = 0; i < 15; i++) {
			read_frame.data_bytes[i] = i2c_read_buffer[i+3];
		}

		p_cec_object->message_callback(DL_HDMICEC_CALLBACK_MESSAGE_AVAILABLE
		                               , (void *) &read_frame, read_frame.frame_byte_count);
	}

	return(0);

}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecOpen( )
 *    \brief Open unit 0 of CEC and provides the instance number to
 *           the caller. Note that one unit of CEC represents one physical
 *           CEC device and that only one instance per unit can be opened.
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
 *            - ERR_DLHDMICEC_INVALID_STATE: the state is invalid for
 *              the function
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_open
(
    instance_t   *p_instance
)
{
	/* directly call OpenM function for unit 0 and return the result */
	return(dl_hdmi_cec_open_m(p_instance, (unit_select_t)0));
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecOpenM( )
 *    \brief Open a specific unit of CEC driver and provides the instance
 *           number to the caller. Note that one unit of CEC represents one
 *           physical CEC device and that only one instance per unit can be
 *           opened. This function switches driver's state machine to
 *           "initialized" state.
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
 *            - ERR_DLHDMICEC_INVALID_STATE: the state is invalid for
 *              the function
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_open_m
(
    instance_t   *p_instance,
    unit_select_t  unit
)
{

	/* check if unit number is in range */
	RETIF((unit < 0) || (unit >= MAX_UNITS), ERR_DLHDMICEC_BAD_UNIT_NUMBER)

	/* check if Instance pointer is NULL */
	RETIF(p_instance == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	/* check if unit is already instanciated */
	RETIF(unit_table[unit].opened == true, ERR_DLHDMICEC_RESOURCE_OWNED)

	/* Ckeck the state */
	RETIF(unit_table[unit].state != CEC_STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE)

	/* instanciate unit and return corresponding instance number */
	/* Since HW unit are only instanciable once, instance = unit */
	unit_table[unit].opened           = true;
	unit_table[unit].message_callback = NULL;
	/* Give a logical Address to Device */
	unit_table[unit].device_logical_address = CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;

	/* Recover the configuration of the device library */
	RETIF(dl_hdmi_cec_cfg_get_config(unit, &gtmdl_hdmi_cec_driver_config_table[unit]) != 0, ERR_DLHDMICEC_INIT_FAILED)

	*p_instance = (instance_t)unit;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecClose( )
 *    \brief Close an instance of CEC driver.
 *
 *    \param instance Instance identifier.
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
error_code_t dl_hdmi_cec_close
(
    instance_t   instance
)
{
	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* close instance */
	unit_table[instance].opened = false;
	unit_table[instance].state = CEC_STATE_NOT_INITIALIZED;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecInstanceConfig( )
 *    \brief Set the configuration of instance attributes. This function is
 *           required by DVP architecture rules but actually does nothing in this
 *           driver
 *
 *    \param instance    Instance identifier.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_BAD_PARAMETER: a parameter is invalid or out
 *              of range
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_instance_config
(
    instance_t    instance
)
{
	if(instance);

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecInstanceSetup( )
 *    \brief Setup the instance with its configuration parameters. This function
 *           allows basic instance configuration for CEC Stack Processor.
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
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_instance_setup
(
    instance_t               instance,
    dl_hdmi_cec_instance_setup_t *p_setup_info
)
{
	error_code_t                  err;
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
#ifdef TMFL_TDA9989
	unsigned char               i2c_read_buffer[1];
#endif

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check that input pointer is not NULL */
	RETIF(p_setup_info == NULL, ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* Ckeck the state */
	RETIF(unit_table[instance].state != CEC_STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Wait for 250 ms */
	RETIF((err = tx_iwwait(500)) != 0, err)

#ifdef TMFL_TDA9989
	/* Enable CEC Stack Processor */
	err = get_cec_hw_registers(p_dis, E_REG_CDR0, i2c_read_buffer, 1);
	RETIF(err != 0, err)

	err = get_cec_hw_registers(p_dis, E_REG_ENAMODS, i2c_read_buffer, 1);
	RETIF(err != 0, err)

	i2c_read_buffer[0] |= DEFAULT_ENAMODS;

	err = set_cec_hw_register(p_dis, E_REG_ENAMODS, i2c_read_buffer[0]);
	if(err != 0) {
		/*TODO WA still needed? */
		err = set_cec_hw_register(p_dis, E_REG_ENAMODS, i2c_read_buffer[0]);
		RETIF(err != 0, err)
	}

	RETIF((err = tx_iwwait(TDA9950_RESET_DELAY_MS)) != 0, err)

	/* Select CEC clock source and divider value */

	if(p_setup_info->cec_clock_source == DL_HDMICEC_CLOCK_XTAL) {
		err = get_cec_hw_registers(p_dis, E_REG_CEC_CLK, i2c_read_buffer, 1);
		RETIF(err != 0, err)

		i2c_read_buffer[0] &= CEC_CLK_SEL;
		err = set_cec_hw_register(p_dis, E_REG_CEC_CLK, i2c_read_buffer[0]);
	}

	RETIF((err = tx_iwwait(TDA9950_RESET_DELAY_MS)) != 0, err)

	/*TODO WA to avoid spurious interrupts */
	err = get_cec_hw_registers(p_dis, E_REG_CDR0, i2c_read_buffer, 1);
	RETIF(err != 0, err)
#endif

	/* Reset CEC Stack Processor */
	err = set_cec_hw_register(p_dis, E_REG_CCR, 0x80);
	RETIF(err != 0, err)

	/* Wait for 250 ms */
	RETIF((err = tx_iwwait(TDA9950_RESET_DELAY_MS)) != 0, err)

	/* Configure Stack Processor (Retry = 5)*/
	err = set_cec_hw_register(p_dis, E_REG_CCONR, 0x05);
	RETIF(err != 0, err)

	unit_table[instance].device_logical_address = p_setup_info->device_logical_address;

	/* CEC Control register */
	err = set_cec_hw_register_msb_lsb(p_dis, E_REG_ACKH, 0x1 << (unit_table[instance].device_logical_address));
	RETIF(err != 0, err)

	/* CEC Stack Processor enable*/
	err = set_cec_hw_register(p_dis, E_REG_CCR, 0x40);
	RETIF(err != 0, err)

	/* switch instance to its new state */
	unit_table[instance].state = CEC_STATE_CONFIGURED;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRegisterCallback( )
 *    \brief Register event callbacks. Three types of callbacks can be
 *           registered : input activity related callback, data related
 *           callback (infoframes, packets, etc.) and general information
 *           callback. A null pointer means that no callback are registered.
 *
 *    \param instance                 Instance identifier.
 *    \param MessageCallback         Pointer to the callback function that will
 *                                    handle message related events.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_INSTANCE: the instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMICEC_INVALID_STATE: the state is invalid for
 *              the function
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_register_callbacks
(
    instance_t               instance,
    ptmdl_hdmi_cec_callback_func_t message_callback
)
{
	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	/* store callback pointers */
	unit_table[instance].message_callback  = message_callback;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetLogicalAddress( )
 *    \brief Set Device Logical Address
 *
 *    \param instance      Instance identifier.
 *    \param LogicalAddress Logical address value.
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
)
{
	error_code_t                  err;
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* Ckeck the state */
	RETIF(unit_table[instance].state != CEC_STATE_CONFIGURED, ERR_DLHDMICEC_INVALID_STATE)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	unit_table[instance].device_logical_address = logical_address;

	err = set_cec_hw_register_msb_lsb(p_dis, E_REG_ACKH, 0x1 << (unit_table[instance].device_logical_address));
	RETIF(err != 0, err)

	return(0);
}

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
)
{
	error_code_t                  err;
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* Ckeck the state */
	RETIF(unit_table[instance].state != CEC_STATE_CONFIGURED, ERR_DLHDMICEC_INVALID_STATE)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Configure Retry register */
	err = set_cec_hw_register(p_dis, E_REG_CCONR, nb_retry);
	RETIF(err != 0, err)

	return(0);
}

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
)
{
	/* copy Last CEC message datas */
	p_save_message->address_byte  = gtmdl_hdmi_cec_driver_save_message.address_byte;
	p_save_message->message_type_polling = gtmdl_hdmi_cec_driver_save_message.message_type_polling;
	p_save_message->opcode  = gtmdl_hdmi_cec_driver_save_message.opcode;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecImageViewOn( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C Write data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	/*RETIF(UnitTable[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED) */

	/* check if instance state is correct */
	/*RETIF(UnitTable[instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Image View On command */

	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_IMAGE_VIEW_ON ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecInactiveSource( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];
	/*======To do : make a prepare message function with parameter */
	/* Inactive source command */
	i2c_buffer[0] = 0x06;

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_INACTIVE_SOURCE ;  /* Inactive Source*/
	/* MsByte of Physical Address */
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecActiveSource()
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Active Source command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	i2c_buffer[3] = CEC_OPCODE_ACTIVE_SOURCE ;        /* Active source */
	/* MsByte of Physical Address */
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecVersion()
 *    \brief This message is used to indicate the supported CEC version in response
 *           to a <Get CEC Version>
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress\n
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* CEC Version command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_CEC_VERSION ;  /* CECVersion*/
	i2c_buffer[4] = cecversion;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecClearAnalogueTimer( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[15] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Clear Analogue Timer command */
	i2c_buffer[0] = 0x0f;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_CLEAR_ANALOGUE_TIMER ;
	i2c_buffer[4] = day_of_month;                       /*Day of Month*/
	i2c_buffer[5] = month_of_year;                      /*Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);  /*Start Time*/
	i2c_buffer[7] = (u8)start_time;
	i2c_buffer[8] = p_duration -> hours;               /*Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;              /*Duration minute*/
	i2c_buffer[10] = recording_sequence;               /*Recording Sequence*/
	/*Analogue Broadcast Type*/
	i2c_buffer[11] = analogue_broadcast_type;
	/*Analogue Frequency*/
	i2c_buffer[12] = (unsigned char)(analogue_frequency >> 8);
	i2c_buffer[13] = (unsigned char)analogue_frequency;
	i2c_buffer[14] = broadcast_system;                 /*BroadcastSystem*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 15);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecClearDigitalTimer( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[18] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cecarib_data_t *p_arib_pointer;
	dl_hdmi_cecatsc_data_t *p_atsc_pointer;
	dl_hdmi_cecdvb_data_t  *p_dvb_pointer;

	unsigned char regval;                 /* Local variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Clear Digital Timer command */
	i2c_buffer[0] = 0x12;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_CLEAR_DIGITAL_TIMER ;
	i2c_buffer[4] = day_of_month;                       /*Day of Month*/
	i2c_buffer[5] = month_of_year;                      /*Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);  /*Start Time*/
	i2c_buffer[7] = (u8)start_time;
	i2c_buffer[8] = p_duration -> hours;               /*Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;              /*Durantion Minute*/
	i2c_buffer[10] = recording_sequence;               /*Recording Sequence*/

	/* Digital service Identification*/
	/*Merge Service Method and Digital Broadcast System in the same Byte*/
	/*bit 7 is Service Method*/
	regval = (unsigned char)(p_service_identification->service_identification_method & 0x01);
	regval = regval << 7;
	/*bits 6 to 0 are Digital Broadcast*/
	regval |= (unsigned char)(p_service_identification->digital_broadcast_system & 0x7F);
	i2c_buffer[11] = regval;

	/*Case of a ARIB Generic*/
	if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC) {
		p_arib_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_arib_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_arib_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_arib_pointer->service_id >> 8);
		i2c_buffer[15] = (unsigned char)p_arib_pointer->service_id;
		i2c_buffer[16] = (unsigned char)(p_arib_pointer->original_network_id >> 8);
		i2c_buffer[17] = (unsigned char)p_arib_pointer->original_network_id;

	}
	/*Case of a ATSC Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC) {
		p_atsc_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_atsc_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_atsc_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_atsc_pointer->program_number >> 8);
		i2c_buffer[15] = (unsigned char)p_atsc_pointer->program_number;
		i2c_buffer[16] = (unsigned char)(p_atsc_pointer->reserved >> 8);
		i2c_buffer[17] = (unsigned char)p_atsc_pointer->reserved;
	}
	/*Case of a DVB Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC) {
		p_dvb_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_dvb_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_dvb_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_dvb_pointer->service_id >> 8);
		i2c_buffer[15] = (unsigned char)p_dvb_pointer->service_id;
		i2c_buffer[16] = (unsigned char)(p_dvb_pointer->original_network_id >> 8);
		i2c_buffer[17] = (unsigned char)p_dvb_pointer->original_network_id;
	}
	/*other cases, Buffer are empty*/
	else {
		i2c_buffer[12] = 0xFF;
		i2c_buffer[13] = 0xFF;
		i2c_buffer[14] = 0xFF;
		i2c_buffer[15] = 0xFF;
		i2c_buffer[16] = 0xFF;
		i2c_buffer[17] = 0xFF;
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 18);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecClearExternalTimerWithExternalPlug( )
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
    dl_hdmi_cecexternal_plug_t   external_plug
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[13] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/* RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Clear External Timer with External Plug Command*/
	i2c_buffer[0] = 0x0D;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_CLEAR_EXTERNAL_TIMER ;
	i2c_buffer[4] = day_of_month;                       /*Day of Month*/
	i2c_buffer[5] = month_of_year;                      /*Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);  /*Start Time*/
	i2c_buffer[7] = (unsigned char)start_time;
	i2c_buffer[8] = p_duration -> hours;               /*Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;              /*Duration minute*/
	i2c_buffer[10] = recording_sequence;               /*Recording Sequence*/
	/*External Source Specifier = External Plug */
	i2c_buffer[11] = CEC_EXTERNAL_PLUG;
	i2c_buffer[12] = external_plug;                    /*External Plug*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 13);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecClearExternalTimerWithPhysicalAddress( )
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
    dl_hdmi_cecexternal_physical_address_t   external_physical_address
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[14] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Clear External Timer with Physical Address Command */
	i2c_buffer[0] = 0x0E;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/*Clear External Timer*/
	i2c_buffer[3] = CEC_OPCODE_CLEAR_EXTERNAL_TIMER ;
	/*Day of Month*/
	i2c_buffer[4] = day_of_month;
	/*Month of Year*/
	i2c_buffer[5] = month_of_year;
	/*Start Time*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);
	i2c_buffer[7] = (unsigned char)start_time;
	/*Duration Hours*/
	i2c_buffer[8] = p_duration -> hours;
	/*Duration Minute*/
	i2c_buffer[9] = p_duration -> minute;
	/*Recording Sequence*/
	i2c_buffer[10] = recording_sequence;
	/*External Source Specifier = External Address*/
	i2c_buffer[11] = CEC_EXTERNAL_PHYSICAL_ADDRESS;
	/*External Address*/
	i2c_buffer[12] = (unsigned char)(external_physical_address >> 8);
	i2c_buffer[13] = (unsigned char)external_physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 14);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTextViewOn( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to Instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if Instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Text View On command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_TEXT_VIEW_ON ;     /* Text View On */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTimerClearedStatus( )
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
)
{

	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Timer Clear Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_TIMER_CLEARED_STATUS;  /* System Audio Status*/
	/* Timer Cleared Status*/
	i2c_buffer[4] = timer_cleared_status_data;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTimerStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cectimer_programmed_info_t *p_timer_prog_info;
	unsigned char regval;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Timer Status command */
	i2c_buffer[0] = 0x07;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_TIMER_CLEARED_STATUS;  /* System Audio Status*/
	/* First Byte Building */
	/* bit 7 for Timer Overlap Warning */
	regval = ((unsigned char)(p_timer_status_data->timer_overlap_warning) & 0x01) << 7 ;
	/* bit 6 to 5 for Media Info */
	regval |= ((unsigned char)(p_timer_status_data->media_info) & 0x03) << 5;

	p_timer_prog_info = &(p_timer_status_data->timer_programmed_info);
	/* bit 4 for Timer Programed Indicator */
	regval |= ((unsigned char)(p_timer_prog_info->select_program_info) & 0x01) << 4;
	/* bit 3 to 0 for Program Information */
	regval |= (unsigned char)(p_timer_prog_info->program_info) & 0x0F;
	i2c_buffer[4] = regval;

	/* 2 Duration Available Bytes Building */
	/* Duration Available is only filled in the the both following conditions*/
	if((p_timer_prog_info->select_program_info == CEC_PROGRAM_INDICATOR_NOT_PROGRAMMED) && (p_timer_prog_info->program_info == CEC_PROGRAM_ERROR_INFO_DUPLICATE_ALREADY_PROGRAMMED)) {
		i2c_buffer[5] = (unsigned char)(p_timer_prog_info->duration_available >> 8);
		i2c_buffer[6] = (unsigned char)p_timer_prog_info->duration_available;
	} else if((p_timer_prog_info->select_program_info == CEC_PROGRAM_INDICATOR_PROGRAMMED) && (p_timer_prog_info->program_info == CEC_PROGRAM_INFO_NOT_ENOUGHT_SPACE_AVAILABLE_FOR_RECORDING)) {
		i2c_buffer[5] = (unsigned char)(p_timer_prog_info->duration_available >> 8);
		i2c_buffer[6] = (unsigned char)p_timer_prog_info->duration_available;
	}
	/*Else, 2 bytes of Duration Available are filled with 0xFF*/
	else {
		i2c_buffer[5] = 0xFF;
		i2c_buffer[6] = 0xFF;
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTunerDeviceStatusAnalogue( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[9] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	unsigned char regval;                 /*Local Variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Select Aanalogue Service command */
	i2c_buffer[0] = 0x09;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Tuner Device Status*/
	i2c_buffer[3] = CEC_OPCODE_TUNER_DEVICE_STATUS ;
	/* Build First Byte*/
	/*bit 7 is Recording Flag */
	regval = ((unsigned char)recording_flag & 0X01) << 7;
	/*bit 6 to 0 are Tuner display Info*/
	regval |= (unsigned char)tuner_display_info & 0X7F;
	i2c_buffer[4] = regval;

	/*Analogue Broadcast System type*/
	i2c_buffer[5] = analogue_broadcast_type;
	/*Analogue Frequency*/
	i2c_buffer[6] = (unsigned char)(analogue_frequency >> 8);
	i2c_buffer[7] = (unsigned char)analogue_frequency;
	/*Broadcast System*/
	i2c_buffer[8] = broadcast_system;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 9);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTunerDeviceStatusDigital( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[12] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cecarib_data_t *p_arib_pointer;
	dl_hdmi_cecatsc_data_t *p_atsc_pointer;
	dl_hdmi_cecdvb_data_t  *p_dvb_pointer;

	unsigned char regval;                 /* Local variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record On Digital Service command */
	i2c_buffer[0] = 0x0C;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Tuner Device Status*/
	i2c_buffer[3] = CEC_OPCODE_TUNER_DEVICE_STATUS ;

	/* Merge Recording Flag With Tuner Display Info*/
	/* bit 7 is Recording Flag*/
	regval = ((unsigned char)recording_flag & 0X01) << 7;
	/* bit 6 to 0 are Tuner display Info*/
	regval |= (unsigned char)tuner_display_info & 0X7F;
	i2c_buffer[4] = regval;

	/* Digital service Identification*/
	/*Merge Service Method and Digital Broadcast System in the same Byte*/
	/* bit 7 is Service Method*/
	regval = (unsigned char)(p_service_identification->service_identification_method & 0x01) << 7;
	/* bits 6 to 0 are Digital Broadcast*/
	regval |= (unsigned char)(p_service_identification->digital_broadcast_system & 0x7F);
	i2c_buffer[5] = regval;

	/*Case of a ARIB Generic*/
	if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC) {
		p_arib_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_arib_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_arib_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_arib_pointer->service_id >> 8);
		i2c_buffer[9] = (unsigned char)p_arib_pointer->service_id;
		i2c_buffer[10] = (unsigned char)(p_arib_pointer->original_network_id >> 8);
		i2c_buffer[11] = (unsigned char)p_arib_pointer->original_network_id;
	}
	/*Case of a ATSC Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC) {
		p_atsc_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_atsc_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_atsc_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_atsc_pointer->program_number >> 8);
		i2c_buffer[9] = (unsigned char)p_atsc_pointer->program_number;
		i2c_buffer[10] = (unsigned char)(p_atsc_pointer->reserved >> 8);
		i2c_buffer[11] = (unsigned char)p_atsc_pointer->reserved;
	}
	/*Case of a DVB Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC) {
		p_dvb_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_dvb_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_dvb_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_dvb_pointer->service_id >> 8);
		i2c_buffer[9] = (unsigned char)p_dvb_pointer->service_id;
		i2c_buffer[10] = (unsigned char)(p_dvb_pointer->original_network_id >> 8);
		i2c_buffer[11] = (unsigned char)p_dvb_pointer->original_network_id;
	}
	/*other cases, Buffer are empty*/
	else {
		i2c_buffer[6] = 0xFF;
		i2c_buffer[7] = 0xFF;
		i2c_buffer[8] = 0xFF;
		i2c_buffer[9] = 0xFF;
		i2c_buffer[10] = 0xFF;
		i2c_buffer[11] = 0xFF;
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 12);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRequestActiveSource( )
 *    \brief This message is used by a new device to discover the status of
 *           the system.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMICEC_BAD_Instance: the Instance number is wrong or
 *              out of range
 *            - ERR_DLHDMICEC_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_request_active_source
(
    instance_t  instance
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* IRequest Active Source command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Request Active Source */
	i2c_buffer[3] = CEC_OPCODE_REQUEST_ACTIVE_SOURCE ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRoutingChange( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[8] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Routing Change command */
	i2c_buffer[0] = 0x08;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	i2c_buffer[3] = CEC_OPCODE_ROUTING_CHANGE ;            /* Routing Change */
	/* MsByte of Original Address*/
	i2c_buffer[4] = (unsigned char)(original_address >> 8);
	/* LsByte of Original Address */
	i2c_buffer[5] = (unsigned char)original_address;
	/* MsByte of New Address */
	i2c_buffer[6] = (unsigned char)(new_address >> 8);
	/* LsByte of New Address */
	i2c_buffer[7] = (unsigned char)new_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 8);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRoutingInformation( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Routing Information command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Routing Information */
	i2c_buffer[3] = CEC_OPCODE_ROUTING_INFORMATION ;
	/* MsByte of Physical Address*/
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSelectAnalogueService( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[8] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Select Aanalogue Service command */
	i2c_buffer[0] = 0x08;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Select Analogue Service*/
	i2c_buffer[3] = CEC_OPCODE_SET_ANALOGUE_SERVICE ;
	i2c_buffer[4] = analogue_broadcast_type;
	i2c_buffer[5] = (unsigned char)(analogue_frequency >> 8);
	i2c_buffer[6] = (unsigned char)analogue_frequency;
	i2c_buffer[7] = broadcast_system;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 8);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetStreamPath( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set Stream Path command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Set Stream Path */
	i2c_buffer[3] = CEC_OPCODE_SET_STREAM_PATH ;
	/* MsByte of Physical Address*/
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}
/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetSystemAudioMode( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set System Audio Mode Command */
	i2c_buffer[0] = 0x05;    /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Set System Audio Mode*/
	i2c_buffer[3] = CEC_OPCODE_SET_SYSTEM_AUDIO_MODE ;
	/*System Audio Status*/
	i2c_buffer[4] = system_audio_status;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetTimerProgramTitle( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Set Timer Program Title */
	/* Calculate Message length*/
	mess_length = program_title_length + 4;

	i2c_buffer[0] = (unsigned char)mess_length;

	/* Request CEC data */
	i2c_buffer[1] = 0x00;

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Set Timer Program Title*/
	i2c_buffer[3] = CEC_OPCODE_SET_TIMER_PROGRAM_TITLE ;

	for(loci = 0; loci <= program_title_length ; loci++) {
		/* Fill Table with Program Title characters*/
		i2c_buffer[(loci+4)] = p_program_title_string[loci];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, (mess_length));
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecStandby( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Standby command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_STANDBY ;      /* Standby */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSystemAudioModeRequest( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* System Audio Mode Request command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* System Audio Mode Request*/
	i2c_buffer[3] = CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST ;
	/* MsByte of Physical Address */
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSystemAudioModeStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* System Audio Mode Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* System Audio Mode Status*/
	i2c_buffer[3] = CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS ;
	/* System Audio Status*/
	i2c_buffer[4] = system_audio_status;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveTunerDeviceStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* System Audio Mode Request command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Give Tuner Device Status*/
	i2c_buffer[3] = CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS ;
	i2c_buffer[4] = (unsigned char)status_request; /* Status Request */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordTvScreen( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record TV Sreen command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_TV_SCREEN ;     /* Record TV screen */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportAudioStatus( )
 *    \brief This message report an amplifier's volume and mute.
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8    ReceiverLogicalAddress \n
 *           Address of message receiver. \n
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char regval;                 /*Local Variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Report Audio Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_REPORT_AUDIO_STATUS ;  /* Report Audio Statust*/
	/* bit 7 Mute Status*/
	regval = (((unsigned char)p_audio_status -> audio_mute_status) & 0x01) << 7;
	/* bit 6 to 0 Volum Status*/
	regval |= ((unsigned char)p_audio_status -> audio_volume_status) & 0x7F;
	i2c_buffer[4] = regval;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x07;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Report Audio Capability*/
	i2c_buffer[3] = CEC_OPCODE_REPORT_SHORT_AUDIO_DESCRIPTOR ;
	/* MSByte of ShortAudioDecriptor*/
	i2c_buffer[4] = (unsigned char)(short_audio_decriptor >> 16);
	i2c_buffer[5] = (unsigned char)(short_audio_decriptor >> 8);
	/* LSByte of ShortAudioDecriptor*/
	i2c_buffer[6] = (unsigned char)short_audio_decriptor;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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

)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char regval;                 /*Local Variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Request Audio Capability*/
	i2c_buffer[3] = CEC_OPCODE_REQUEST_SHORT_AUDIO_DESCRIPTOR ;
	/* bit 3 to 7  AudioFormatCode*/
	regval = (((unsigned char)audio_format_code) & 0x3F) << 2;
	/* bit 1 to 0 AudioFormatID*/
	regval |= ((unsigned char)audio_format_id) & 0x03;
	i2c_buffer[4] = regval;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_INITATE_ARC ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_REPORT_ARC_INITIATED ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_REPORT_ARC_TERMINATED ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_REPORT_ARC_INITIATION ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_REPORT_ARC_TERMINATION ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Report Short Audio Decriptor */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_TERMINATE_ARC ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGivePhysicalAddress( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give physical Address command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Give Physical Address */
	i2c_buffer[3] = CEC_OPCODE_GIVE_PHYSICAL_ADDRESS ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveSystemAudioModeStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give System Audio Mode Status command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Give System Audio Mode Status*/
	i2c_buffer[3] = CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetMenuLanguage( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Get Menu Language command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Get Menu Address */
	i2c_buffer[3] = CEC_OPCODE_GET_MENU_LANGUAGE ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}
/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveAudioStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give Audio Mode Status command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_GIVE_AUDIO_STATUS ;    /* Message Abort*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecPollingMessage( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[3] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Polling Message command */
	i2c_buffer[0] = 0x03;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 3);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 1;
	gtmdl_hdmi_cec_driver_save_message.opcode = 0;

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_STATUS ;    /* Record Status */
	i2c_buffer[4] = record_status_info; /* Record Status */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOff( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record Off command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_OFF ;   /* Record Off */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOnAnalogueService( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[9] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Record On Analogue Device command */
	i2c_buffer[0] = 0x09;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_ON ;                     /*Record On*/

	/*RecordSourceType = CEC_RECORD_SOURCE_ANALOGUE_SERVICE*/
	i2c_buffer[4] = CEC_RECORD_SOURCE_ANALOGUE_SERVICE;
	/*Analogue Brodcast Type*/
	i2c_buffer[5] = analogue_broadcast_type;
	/*Analogue Frequency*/
	i2c_buffer[6] = (unsigned char)(analogue_frequency >> 8);
	i2c_buffer[7] = (unsigned char)analogue_frequency;
	/*Brodcast System*/
	i2c_buffer[8] = broadcast_system;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 9);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOnDigitalService( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[12] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cecarib_data_t *p_arib_pointer;
	dl_hdmi_cecatsc_data_t *p_atsc_pointer;
	dl_hdmi_cecdvb_data_t  *p_dvb_pointer;

	unsigned char regval;                 /* Local variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record On Digital Service command */
	i2c_buffer[0] = 0x0C;     /* Param number = 10*/

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Record On Digital Service*/
	i2c_buffer[3] = CEC_OPCODE_RECORD_ON ;

	/* RecordSourceType = CEC_RECORD_SOURCE_DIGITAL_SERVICE */
	i2c_buffer[4] = CEC_RECORD_SOURCE_DIGITAL_SERVICE;

	/* Digital Service Identification*/
	/*Merge Service Method and Digital Broadcast System in the same Byte*/
	/*bit 7 is Service Method*/
	regval = (unsigned char)(p_service_identification->service_identification_method & 0x01);
	regval = regval << 7;
	/*bits 6 to 0 are Digital Broadcast*/
	regval |= (unsigned char)(p_service_identification->digital_broadcast_system & 0x7F);
	i2c_buffer[5] = regval;

	/*Case of a ARIB Generic*/
	if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC) {
		p_arib_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_arib_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_arib_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_arib_pointer->service_id >> 8);
		i2c_buffer[9] = (unsigned char)p_arib_pointer->service_id;
		i2c_buffer[10] = (unsigned char)(p_arib_pointer->original_network_id >> 8);
		i2c_buffer[11] = (unsigned char)p_arib_pointer->original_network_id;
	}
	/*Case of a ATSC Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC) {
		p_atsc_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_atsc_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_atsc_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_atsc_pointer->program_number >> 8);
		i2c_buffer[9] = (unsigned char)p_atsc_pointer->program_number;
		i2c_buffer[10] = (unsigned char)(p_atsc_pointer->reserved >> 8);
		i2c_buffer[11] = (unsigned char)p_atsc_pointer->reserved;
	}
	/*Case of a DVB Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC) {
		p_dvb_pointer = p_service_identification->p_service_identification;

		i2c_buffer[6] = (unsigned char)(p_dvb_pointer->transport_stream_id >> 8);
		i2c_buffer[7] = (unsigned char)p_dvb_pointer->transport_stream_id;
		i2c_buffer[8] = (unsigned char)(p_dvb_pointer->service_id >> 8);
		i2c_buffer[9] = (unsigned char)p_dvb_pointer->service_id;
		i2c_buffer[10] = (unsigned char)(p_dvb_pointer->original_network_id >> 8);
		i2c_buffer[11] = (unsigned char)p_dvb_pointer->original_network_id;
	}
	/*other cases, Buffer are empty*/
	else {
		i2c_buffer[6] = 0xFF;
		i2c_buffer[7] = 0xFF;
		i2c_buffer[8] = 0xFF;
		i2c_buffer[9] = 0xFF;
		i2c_buffer[10] = 0xFF;
		i2c_buffer[11] = 0xFF;
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 12);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOnExternalPhysicalAddress( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record On External Physial Address command */
	i2c_buffer[0] = 0x07;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/*Record On*/
	i2c_buffer[3] = CEC_OPCODE_RECORD_ON ;
	/*RecordSourceType = CEC_RECORD_SOURCE_EXTERNAL_PHYSICAL_ADDRESS*/
	i2c_buffer[4] = CEC_RECORD_SOURCE_EXTERNAL_PHYSICAL_ADDRESS;
	/*External Physical Address*/
	i2c_buffer[5] = (unsigned char)(external_physical_address >> 8);
	i2c_buffer[6] = (unsigned char)external_physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOnExternalPlug( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record On External Plug command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_ON ;                   /* Record On*/

	/* RecordSourceType = CEC_RECORD_SOURCE_EXTERNAL_PLUG*/
	i2c_buffer[4] = CEC_RECORD_SOURCE_EXTERNAL_PLUG;
	i2c_buffer[5] = external_plug;                            /*External Plug*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecRecordOnOwnSource( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Record On Own Source command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_RECORD_ON ;                /* Record On*/

	/* RecordSourceType = CEC_RECORD_SOURCE_OWN_SOURCE*/
	i2c_buffer[4] = CEC_RECORD_SOURCE_OWN_SOURCE;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportPhysicalAddress( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Report Physical Address command */
	i2c_buffer[0] = 0x07;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Report Physical Address */
	i2c_buffer[3] = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS ;
	/* MsByte of Physical Address*/
	i2c_buffer[4] = (unsigned char)(physical_address >> 8);
	/* LsByte of Physical Address */
	i2c_buffer[5] = (unsigned char)physical_address;

	i2c_buffer[6] = device_type ;                              /* Device Type*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetMenuLanguage( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/* RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set Menu Language command */
	i2c_buffer[0] = 0x07;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Set Menu Language*/
	i2c_buffer[3] = CEC_OPCODE_SET_MENU_LANGUAGE ;
	i2c_buffer[4] = p_language[0];     /* First Tocken*/
	i2c_buffer[5] = p_language[1];
	i2c_buffer[6] = p_language[2];     /* Last Tocken*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecDeckControl()
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if Instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to Instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if Instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Deck Control command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_DESCK_CONTROL;     /* Deck Control Mode*/
	i2c_buffer[4] = deck_control_mode;      /* Deck Control Value*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecDeckStatus()
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Deck Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_DECK_STATUS;       /* Deck Status*/
	i2c_buffer[4] = deck_info;     /* Deck Status Mode Information*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveDeckStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if Instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give Deck Status command */
	i2c_buffer[0] = 0x05;    /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_GIVE_DECK_STATUS;      /* Give Deck Status*/
	i2c_buffer[4] = status_request;        /* Deck Status Request Information*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecPlay( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Play command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_PLAY;  /* Play*/
	/* Play Mode Information Information*/
	i2c_buffer[4] = (unsigned char)play_mode;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSelectDigitalService( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[11] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cecarib_data_t *p_arib_pointer;
	dl_hdmi_cecatsc_data_t *p_atsc_pointer;
	dl_hdmi_cecdvb_data_t  *p_dvb_pointer;

	unsigned char regval;                 /*Local Variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Select Digital Service command */
	i2c_buffer[0] = 0x0B;     /* Param number = 10*/

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Select Digital Service*/
	i2c_buffer[3] = CEC_OPCODE_SET_DIGITAL_SERVICE ;

	/*Merge Service Method and Digital Broadcast System in the same Byte*/
	/* Load the 1 bit of Service Method*/
	regval = (unsigned char)(p_service_identification->service_identification_method & 0x01);
	regval = regval << 7;
	/*Merge with the 7 bits of Digital Broadcast*/
	regval |= (unsigned char)(p_service_identification->digital_broadcast_system & 0x7F);
	i2c_buffer[4] = regval;

	/*Case of a ARIB Generic*/
	if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC) {
		p_arib_pointer = p_service_identification->p_service_identification;

		/* Service Identification */
		i2c_buffer[5] = (unsigned char)(p_arib_pointer->transport_stream_id >> 8);
		i2c_buffer[6] = (unsigned char)p_arib_pointer->transport_stream_id;
		i2c_buffer[7] = (unsigned char)(p_arib_pointer->service_id >> 8);
		i2c_buffer[8] = (unsigned char)p_arib_pointer->service_id;
		i2c_buffer[9] = (unsigned char)(p_arib_pointer->original_network_id >> 8);
		i2c_buffer[10] = (unsigned char)p_arib_pointer->original_network_id;
	}
	/*Case of a ATSC Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC) {
		p_atsc_pointer = p_service_identification->p_service_identification;

		/* Service Identification */
		i2c_buffer[5] = (unsigned char)(p_atsc_pointer->transport_stream_id >> 8);
		i2c_buffer[6] = (unsigned char)p_atsc_pointer->transport_stream_id;
		i2c_buffer[7] = (unsigned char)(p_atsc_pointer->program_number >> 8);
		i2c_buffer[8] = (unsigned char)p_atsc_pointer->program_number;
		i2c_buffer[9] = (unsigned char)(p_atsc_pointer->reserved >> 8);
		i2c_buffer[10] = (unsigned char)p_atsc_pointer->reserved;
	}
	/*Case of a DVB Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC) {
		p_dvb_pointer = p_service_identification->p_service_identification;

		/* Service Identification */
		i2c_buffer[5] = (unsigned char)(p_dvb_pointer->transport_stream_id >> 8);
		i2c_buffer[6] = (unsigned char)p_dvb_pointer->transport_stream_id;
		i2c_buffer[7] = (unsigned char)(p_dvb_pointer->service_id >> 8);
		i2c_buffer[8] = (unsigned char)p_dvb_pointer->service_id;
		i2c_buffer[9] = (unsigned char)(p_dvb_pointer->original_network_id >> 8);
		i2c_buffer[10] = (unsigned char)p_dvb_pointer->original_network_id;
	}
	/*other cases, Buffer are empty*/
	else {
		i2c_buffer[5] = 0xFF;       /* Service Identification */
		i2c_buffer[6] = 0xFF;
		i2c_buffer[7] = 0xFF;
		i2c_buffer[8] = 0xFF;
		i2c_buffer[9] = 0xFF;
		i2c_buffer[10] = 0xFF;
	}
	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 11);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetAnalogueTimer( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[15] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set Analogue Timer command */
	i2c_buffer[0] = 0x0F;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_ANALOGUE_TIMER ;   /* Message Abort*/
	i2c_buffer[4] = day_of_month;
	i2c_buffer[5] = month_of_year;
	i2c_buffer[6] = (unsigned char)(start_time >> 8);
	i2c_buffer[7] = (unsigned char)start_time;
	i2c_buffer[8] = p_duration -> hours;
	i2c_buffer[9] = p_duration -> minute;
	i2c_buffer[10] = recording_sequence;
	i2c_buffer[11] = analogue_broadcast_type;
	i2c_buffer[12] = (unsigned char)(analogue_frequency >> 8);
	i2c_buffer[13] = (unsigned char)analogue_frequency;
	i2c_buffer[14] = broadcast_system;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 15);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetAudioRate( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set Audio Rate command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_AUDIO_RATE ;   /* Set Audio Rate */
	i2c_buffer[4] = audio_rate;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetDigitalTimer( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[18] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	dl_hdmi_cecarib_data_t *p_arib_pointer;
	dl_hdmi_cecatsc_data_t *p_atsc_pointer;
	dl_hdmi_cecdvb_data_t  *p_dvb_pointer;

	unsigned char regval;                 /* Local variable*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set Digital Timer command */
	i2c_buffer[0] = 0x12;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Set Digital Timer*/
	i2c_buffer[3] = CEC_OPCODE_SET_DIGITAL_TIMER ;
	i2c_buffer[4] = day_of_month;                           /* Day of Month*/
	i2c_buffer[5] = month_of_year;                          /* Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);      /* Start Time*/
	i2c_buffer[7] = (unsigned char)start_time;
	i2c_buffer[8] = p_duration -> hours;                   /* Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;                  /* Duration Minute*/
	/*Recording Sequence*/
	i2c_buffer[10] = recording_sequence;

	/* Digital service Identification*/
	/*Merge Service Method and Digital Broadcast System in the same Byte*/
	/*bit 7 is Service Method*/
	regval = (unsigned char)(p_service_identification->service_identification_method & 0x01);
	regval = regval << 7;
	/*bits 6 to 0 are Digital Broadcast*/
	regval |= (unsigned char)(p_service_identification->digital_broadcast_system & 0x7F);
	i2c_buffer[11] = regval;

	/*Case of a ARIB Generic*/
	if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ARIB_GENERIC) {
		p_arib_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_arib_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_arib_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_arib_pointer->service_id >> 8);
		i2c_buffer[15] = (unsigned char)p_arib_pointer->service_id;
		i2c_buffer[16] = (unsigned char)(p_arib_pointer->original_network_id >> 8);
		i2c_buffer[17] = (unsigned char)p_arib_pointer->original_network_id;
	}
	/*Case of a ATSC Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_ATSC_GENERIC) {
		p_atsc_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_atsc_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_atsc_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_atsc_pointer->program_number >> 8);
		i2c_buffer[15] = (unsigned char)p_atsc_pointer->program_number;
		i2c_buffer[16] = (unsigned char)(p_atsc_pointer->reserved >> 8);
		i2c_buffer[17] = (unsigned char)p_atsc_pointer->reserved;
	}
	/*Case of a DVB Generic*/
	else if(p_service_identification->digital_broadcast_system == CEC_DIGITAL_BROADCAST_SYSTEM_DVB_GENERIC) {
		p_dvb_pointer = p_service_identification->p_service_identification;

		i2c_buffer[12] = (unsigned char)(p_dvb_pointer->transport_stream_id >> 8);
		i2c_buffer[13] = (unsigned char)p_dvb_pointer->transport_stream_id;
		i2c_buffer[14] = (unsigned char)(p_dvb_pointer->service_id >> 8);
		i2c_buffer[15] = (unsigned char)p_dvb_pointer->service_id;
		i2c_buffer[16] = (unsigned char)(p_dvb_pointer->original_network_id >> 8);
		i2c_buffer[17] = (unsigned char)p_dvb_pointer->original_network_id;
	}
	/*other cases, Buffer are empty*/
	else {
		i2c_buffer[12] = 0xFF;
		i2c_buffer[13] = 0xFF;
		i2c_buffer[14] = 0xFF;
		i2c_buffer[15] = 0xFF;
		i2c_buffer[16] = 0xFF;
		i2c_buffer[17] = 0xFF;
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 18);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetExternalTimerWithExternalPlug( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[13] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set External Timer With External Plug command */
	i2c_buffer[0] = 0x0D;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_EXTERNAL_TIMER ;   /*SetDigital Timer*/

	i2c_buffer[4] = day_of_month;                       /*Day of Month*/
	i2c_buffer[5] = month_of_year;                      /*Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);  /*Start Time*/
	i2c_buffer[7] = (unsigned char)start_time;
	i2c_buffer[8] = p_duration -> hours;               /*Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;              /*Duration Minute*/
	i2c_buffer[10] = recording_sequence;               /*Recording Sequence*/
	/*External Source Specifier = External Plug */
	i2c_buffer[11] = CEC_EXTERNAL_PLUG;
	i2c_buffer[12] = external_plug;                    /*External Plug*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 13);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetExternalTimerWithPhysicalAddress( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[14] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set external Timer With External Physical Address command */
	i2c_buffer[0] = 0x0E;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_EXTERNAL_TIMER ;   /* SetDigital Timer*/

	i2c_buffer[4] = day_of_month;                       /* Day of Month*/
	i2c_buffer[5] = month_of_year;                      /* Month of Year*/
	i2c_buffer[6] = (unsigned char)(start_time >> 8);  /* Start Time*/
	i2c_buffer[7] = (unsigned char)start_time;
	i2c_buffer[8] = p_duration -> hours;               /* Duration Hours*/
	i2c_buffer[9] = p_duration -> minute;              /* Duration Minute*/
	i2c_buffer[10] = recording_sequence;               /* Recording Sequence*/
	/*External Source Specifier = External Address*/
	i2c_buffer[11] = CEC_EXTERNAL_PHYSICAL_ADDRESS;
	/*External Address*/
	i2c_buffer[12] = (unsigned char)(external_physical_address >> 8);
	i2c_buffer[13] = (unsigned char) external_physical_address;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 14);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTunerStepDecrement( )
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
)
{

	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Tuner Step Decrement command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Tuner Step Decrement*/
	i2c_buffer[3] = CEC_OPCODE_TUNER_STEP_DECREMENT ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecTunerStepIncrement( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Tuner Step Increment command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Tuner Step Increment*/
	i2c_buffer[3] = CEC_OPCODE_TUNER_STEP_INCREMENT ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecDeviceVendorID()
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[7] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Device Vendor ID command */
	/* Param number in case of Vendor ID is 32 Bytes*/
	i2c_buffer[0] = 0x07;

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Broadcast*/
	i2c_buffer[2] |= 0x0F;

	/* Device Vendor ID opcode = 0x87*/
	i2c_buffer[3] = CEC_OPCODE_DEVICE_VENDOR_ID ;
	i2c_buffer[4] = (unsigned char)(vendor_id >> 16);  /* MSByte of Vendor ID*/
	i2c_buffer[5] = (unsigned char)(vendor_id >> 8);
	i2c_buffer[6] = (unsigned char)vendor_id;          /* LSByte of Vendor ID*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 7);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveDeviceVendorID( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give Device Vendor ID command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Give Device Vendor*/
	i2c_buffer[3] = CEC_OPCODE_GIVE_DEVICE_VENDOR_ID ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecVendorCommand( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Vendor Command command */
	/* Calculate Message length*/
	mess_length = vendor_specific_data_length + 4;

	i2c_buffer[0] = mess_length;                    /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_VENDOR_COMMAND ;       /* Vendor Command*/

	for(loci = 0; loci <= vendor_specific_data_length ; loci++) {
		/* Fill Table with vendorSpecific Data characters*/
		i2c_buffer[(loci+7)] = p_vendor_specific_data[loci];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, mess_length);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecVendorCommandWithID( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Vendor Command With ID command */
	/* Calculate Message length*/
	mess_length = vendor_specific_data_length + 7;

	i2c_buffer[0] = mess_length;                      /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Vendor Command*/
	i2c_buffer[3] = CEC_OPCODE_VENDOR_COMMAND_WITH_ID ;
	i2c_buffer[4] = (unsigned char)(vendor_id >> 16);  /* MSByte of Vendor ID*/
	i2c_buffer[5] = (unsigned char)(vendor_id >> 8);
	i2c_buffer[6] = (unsigned char)vendor_id;          /* LSByte of Vendor ID*/

	for(loci = 0; loci <= vendor_specific_data_length ; loci++) {
		/* Fill Table with vendorSpecific Data characters*/
		i2c_buffer[(loci+7)] = p_vendor_specific_data[loci];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, mess_length);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecVendorRemoteButtonDown( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Vendor Remote Button Down command */
	/* Calculate Message length*/
	mess_length = vendor_specific_rc_code_length + 4;

	i2c_buffer[0] = mess_length;                       /* Message Length */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Vendor Remote Button Down Opcode*/
	i2c_buffer[3] = CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN ;
	/*Vendor Specific RC code Parameter*/
	for(loci = 0; loci <= vendor_specific_rc_code_length ; loci++) {
		/* Fill Table with Vendor Specific RC Code data*/
		i2c_buffer[(loci+4)] = p_vendor_specific_rc_code[loci];
	}
	/*Send message Via I2C*/

	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, mess_length);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecVendorRemoteButtonUp( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Vendor Remote Button Up command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Vendor Remote Button Up*/
	i2c_buffer[3] = CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetOsdString( )
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
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_osd_string
(
    instance_t                 instance,
    u8                        receiver_logical_address,
    dl_hdmi_cecdisplay_control_t  display_control,
    const char                   *p_osd_string,
    u8                        osd_string_length
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set OSD String command */

	/* Calculate Message length*/
	mess_length = osd_string_length + 5;

	i2c_buffer[0] = (unsigned char)mess_length;

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_OSD_STRING ;       /* Set Osd String*/
	i2c_buffer[4] = display_control;                   /*Display Control*/
	for(loci = 0; loci <= osd_string_length ; loci++) {
		/* Fill Table with OSD Name characters*/
		i2c_buffer[(loci+5)] = p_osd_string[loci];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, (mess_length));
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveOsdName( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give OSD Name command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_GIVE_OSD_NAME ;    /* Give OSD Name*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetOsdName( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[19] ;       /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char loci;                   /* Local increment variable*/
	unsigned char mess_length;             /* Local Message length*/

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Set OSD Name command */

	/* Calculate Message length*/
	mess_length = osd_name_length + 4;

	i2c_buffer[0] = (unsigned char)mess_length;

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_SET_OSD_NAME ;     /* Set Osd Name*/
	for(loci = 0; loci <= osd_name_length ; loci++) {
		/* Fill Table with OSD Name characters*/
		i2c_buffer[(loci+4)] = p_osd_name[loci];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, (mess_length));
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecMenuRequest( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Menu Request command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_MENU_REQUEST ;     /* Menu Request*/
	i2c_buffer[4] = menu_request_type;              /*Menu Request Type */

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecMenuStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Menu Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_MENU_STATUS;       /* Menu Status*/
	i2c_buffer[4] = menu_state;                    /* Menu State*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressed( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Pressed command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control pressed*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;
	i2c_buffer[4] = uicommand;                            /* UI Command*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressedPlay( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Presses Play command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Pressed*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;

	/* UI Command = CEC_REMOTE_BUTTON_PLAY_FUNCTION */
	i2c_buffer[4] = CEC_REMOTE_BUTTON_PLAY_FUNCTION;
	i2c_buffer[5] = play_mode;                             /* Play Mode*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressedSelectAudioInput( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Pressed Select Audio Input command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Pressed*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;

	/* UI Command = CEC_REMOTE_BUTTON_SELECT_AUDIO_INPUT_FUNCTION*/
	i2c_buffer[4] = CEC_REMOTE_BUTTON_SELECT_AUDIO_INPUT_FUNCTION;
	/* UI Function Select Audio mode*/
	i2c_buffer[5] = select_audio_input;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressedSelectAVInput( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Pressed Select AV Input command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Pressed*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;

	/* UI Command = CEC_REMOTE_BUTTON_SELECT_AV_INPUT_FUNCTION */
	i2c_buffer[4] = CEC_REMOTE_BUTTON_SELECT_AV_INPUT_FUNCTION;
	/* UI Function Select A/V Input*/
	i2c_buffer[5] = select_avinput;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressedSelectMedia( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Pressed Select Media command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Pressed*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;

	/* UI Command = CEC_REMOTE_BUTTON_SELECT_MEDIA_FUNCTION*/
	i2c_buffer[4] = CEC_REMOTE_BUTTON_SELECT_MEDIA_FUNCTION;
	/* UI Function Media*/
	i2c_buffer[5] = select_media;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlPressedTune( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[10] ;       /* I2C data buffer */
	u16 regval16 ;                      /* Local variable used for conversion*/
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Pressed Tune command */
	i2c_buffer[0] = 0x0A;   /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Pressed Opcode*/
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_PRESSED;

	/* UI Command = CEC_REMOTE_BUTTON_TUNE_FUNCTION*/
	i2c_buffer[4] = CEC_REMOTE_BUTTON_TUNE_FUNCTION;

	/* Merge 6 bits of ChanNum with 10 bits of Major channel*/
	/* Save the 6 lsbits */
	regval16 = (u16)(p_channel_identifier->chan_num_format & 0x003F);
	regval16 = regval16 << 10;
	regval16 |= (u16)(p_channel_identifier->major_chan_number & 0x03FF);

	/* Load the 4 information bytes of Channel ID*/
	i2c_buffer[5] = (unsigned char)(regval16 >> 8);
	i2c_buffer[6] = (unsigned char)regval16;
	i2c_buffer[7] = (unsigned char)(p_channel_identifier->minor_chan_number >> 8);
	i2c_buffer[8] = (unsigned char)p_channel_identifier->minor_chan_number;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 9);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}
/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecUserControlReleased( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* User Control Released command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* User Control Released */
	i2c_buffer[3] = CEC_OPCODE_USER_CONTROL_RELEASED ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGiveDevicePowerStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Give Device power Status Power Status command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Give Device Power Status */
	i2c_buffer[3] = CEC_OPCODE_GIVE_DEVICE_POWER_STATUS ;

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecReportPowerStatus( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[5] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Report Power Status command */
	i2c_buffer[0] = 0x05;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	/* Report Power Status*/
	i2c_buffer[3] = CEC_OPCODE_REPORT_POWER_STATUS ;
	i2c_buffer[4] = power_status;                          /* Power Status*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 5);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecFeatureAbort()
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[6] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Feature Abort command */
	i2c_buffer[0] = 0x06;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_FEATURE_ABORT ;    /* Feature Abort*/
	i2c_buffer[4] = feature_opcode;    /* Feature Opcode*/
	i2c_buffer[5] = abort_reason;      /* Abort Reason*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 6);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetCecVersion( )
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* Get CEC Version command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_GET_CEC_VERSION ;  /* Get CEC Version*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecAbortMessage( )
 *    \brief This message is reserved for testing purposes
 *
 *    \param tmInstance_t Instance \n
 *           Instance identifier. \n
 *
 *    \param u8 ReceiverLogicalAddress\n
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
)
{
	error_code_t  err;
	unsigned char  i2c_buffer[4] ;        /* I2C data buffer */
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if instance state is correct */
	/*RETIF(UnitTable[Instance].state != STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/*======To do : make a prepare message function with parameter */
	/* CEC Abort Message command */
	i2c_buffer[0] = 0x04;     /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	/* Receiver logical Address*/
	i2c_buffer[2] |= receiver_logical_address & 0x0F;

	i2c_buffer[3] = CEC_OPCODE_ABORT_MESSAGE ;    /* Message Abort*/

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, 4);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = i2c_buffer[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = i2c_buffer[3];

	return(0);
}

/*Non Functional function used to provide easy way to access register */

/*========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecGetRegister( )
 *    \brief Setup the instance with its configuration parameters. This function
 *           allows basic instance configuration for CEC Stack Processor.
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
 *
 * */
/*========================================================================== */
unsigned char dl_hdmi_cec_get_register
(
    instance_t               instance,
    u32                     offset
)
{
	error_code_t                  err;
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;
	unsigned char               i2c_read_buffer[1];

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), 0xFF)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, 0xFF)

	/* Ckeck the state */
	/*RETIF(UnitTable[instance].state != CEC_STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	err = get_cec_hw_registers(p_dis, (u8) offset, i2c_read_buffer, 1);
	RETIF(err != 0, 0xff)

	return(i2c_read_buffer[0]);
}

/*QB 10 Jan ========================================================================== */
/*!
 *    \fn tmErrorCode_t dlHdmiCecSetRegister( )
 *    \brief Setup the instance with its configuration parameters. This function
 *           allows basic instance configuration for CEC Stack Processor.
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
 *
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_set_register
(
    instance_t                instance,
    u32                      offset,
    u32                      value
)
{
	error_code_t                  err;
	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* Ckeck the state */
	/*RETIF(UnitTable[instance].state != CEC_STATE_NOT_INITIALIZED, ERR_DLHDMICEC_INVALID_STATE) */

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	err = set_cec_hw_register(p_dis, (u8) offset, (u8)value);
	RETIF(err != 0, err)

	return(0);
}

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
)
{
	error_code_t err = 0;

#ifdef TMFL_TDA9989
	unsigned char i2c_read_buffer[1];

	dl_hdmi_cec_driver_config_table_t *p_dis;

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	if(power_state == power_on) {

		err = get_cec_hw_registers(p_dis, E_REG_ENAMODS, i2c_read_buffer, 1);
		RETIF(err != 0, err)

		i2c_read_buffer[0] |= DEFAULT_ENAMODS;

		err = set_cec_hw_register(p_dis, E_REG_ENAMODS, i2c_read_buffer[0]);
		RETIF(err != 0, err)

	} else if(power_state == power_standby) {

		err = get_cec_hw_registers(p_dis, E_REG_ENAMODS, i2c_read_buffer, 1);
		RETIF(err != 0, err)

		i2c_read_buffer[0] &=  ~(DEFAULT_ENAMODS);

		err = set_cec_hw_register(p_dis, E_REG_ENAMODS, i2c_read_buffer[0]);
		RETIF(err != 0, err)

		unit_table[instance].state = CEC_STATE_NOT_INITIALIZED;

	} else {
		return ERR_DLHDMICEC_BAD_PARAMETER;
	}

#endif  /*  TMFL_TDA9989 */

	return err;
}

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
)
{
	error_code_t err = 0;

#ifdef TMFL_TDA9989

	unsigned char i2c_read_buffer[1];

	dl_hdmi_cec_driver_config_table_t *p_dis;

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	if(p_power_state == NULL) return ERR_DLHDMICEC_BAD_PARAMETER;

	err = get_cec_hw_registers(p_dis, E_REG_ENAMODS, i2c_read_buffer, 1);
	RETIF(err != 0, err)

	if((i2c_read_buffer[0] & DEFAULT_ENAMODS) == DEFAULT_ENAMODS) {
		*p_power_state = power_on;
	} else {
		*p_power_state = power_standby;
	}

#endif  /*  TMFL_TDA9989 */

	return err;

}

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
 *	    - ERR_DLHDMICEC_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 * */
/*========================================================================== */
error_code_t dl_hdmi_cec_send_message(

    instance_t instance,
    u8        *p_data,
    u16       len_data
)
{

	error_code_t  err = 0;

#ifdef TMFL_TDA9989

	unsigned char  i2c_buffer[19] ;        /* I2C data buffer */
	unsigned char  loci;                   /* Local increment variable*/
	unsigned char  mess_length;             /* Local Message length*/

	/* Pointer to Device Instance Structure */
	dl_hdmi_cec_driver_config_table_t *p_dis;

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* check if unit corresponding to instance is opened */
	RETIF(unit_table[instance].opened == false, ERR_DLHDMICEC_RESOURCE_NOT_OWNED)

	/* check if CEC message is not too long */
	RETIF((len_data > 16), ERR_DLHDMICEC_INCONSISTENT_PARAMS)

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* Calculate Internal Message length*/
	/* real data is less ReceiverLogical address */
	mess_length = (len_data - 1) + 3;

	i2c_buffer[0] = mess_length;  /* Param number */

	i2c_buffer[1] = 0x00;     /* Request CEC data */

	/*Build Initiator and Reciever Logical Address Byte*/
	/*Initiator logical Address*/
	i2c_buffer[2] = (unsigned char)(unit_table[instance].device_logical_address) & 0x0F;
	i2c_buffer[2] = i2c_buffer[2] << 4;
	i2c_buffer[2] |= p_data[0] & 0x0F;

	for(loci = 0; loci <= len_data ; loci++) {
		/* Fill Table with Data from middleware, Data begin at position 1*/
		i2c_buffer[(loci+3)] = p_data[(loci+1)];
	}

	/* CEC Data register */
	err = set_cec_hw_registers(p_dis, E_REG_CDR0, i2c_buffer, mess_length);
	RETIF(err != 0, err)

	/* Save Datas of the CEC message send */
	gtmdl_hdmi_cec_driver_save_message.address_byte = p_data[2];
	gtmdl_hdmi_cec_driver_save_message.message_type_polling = 0;
	gtmdl_hdmi_cec_driver_save_message.opcode = p_data[3];

#endif  /*  TMFL_TDA9989 */

	return err;

}

/*========================================================================== */
/*========================================================================== */
error_code_t dl_hdmi_cec_enable_calibration(instance_t instance, dl_hdmi_cec_clock_source_t cec_clock_source)
{
	error_code_t err = 0;

#ifdef TMFL_TDA9989

	unsigned char                   i2c_read_buffer[1];
	dl_hdmi_cec_driver_config_table_t *p_dis;

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* CLOCK SOURCE is FRO */
	if(cec_clock_source == DL_HDMICEC_CLOCK_FRO) {

		/* cf PR1795 set desired frequency to 12 Mhz*/

		dl_hdmi_cec_set_register(instance, 0xF3, 0xC0);

		RETIF(err != 0, err)

		dl_hdmi_cec_set_register(instance, 0xF4, 0xD4);

		RETIF(err != 0, err)

		/* set calibration in automatic mode */
		err = get_cec_hw_registers(p_dis, E_REG_CEC_DES_FREQ2, i2c_read_buffer, 1);
		RETIF(err != 0, err)
		i2c_read_buffer[0] &= CEC_AUTOMATIC_CALIBRATION_MSK;
		err = set_cec_hw_register(p_dis, E_REG_CEC_DES_FREQ2, i2c_read_buffer[0]);
		RETIF(err != 0, err)

		/* select FRO clock mode, osc_freq shall be also set to one */
		i2c_read_buffer[0] = CEC_SELECT_FRO_CLOCK_SOURCE;
		err = set_cec_hw_register(p_dis, E_REG_CEC_CLK, i2c_read_buffer[0]);
		RETIF(err != 0, err)

		/* Enable cec_clk AND FRO */
		err = get_cec_hw_registers(p_dis, E_REG_ENAMODS, i2c_read_buffer, 1);
		RETIF(err != 0, err)
		i2c_read_buffer[0] |= CEC_ENABLE_CEC_CLK_MSK;
		i2c_read_buffer[0] &= CEC_ENABLE_FRO_MSK;
		err = set_cec_hw_register(p_dis, E_REG_ENAMODS, i2c_read_buffer[0]);
		RETIF(err != 0, err)

		/* Enable calibration */
		i2c_read_buffer[0] = CEC_ENABLE_CALIBRATION;
		err = set_cec_hw_register(p_dis, E_REG_CEC_CAL_XOSC_CTRL1, i2c_read_buffer[0]);
		RETIF(err != 0, err)

	} /* CLOCK SOURCE is FRO */

#endif  /*  TMFL_TDA9989 */

	return err;
}

/*========================================================================== */
/*========================================================================== */
error_code_t dl_hdmi_cec_disable_calibration(instance_t instance)
{
	error_code_t err = 0;

#ifdef TMFL_TDA9989

	unsigned char                   i2c_read_buffer[1];
	dl_hdmi_cec_driver_config_table_t *p_dis;

	p_dis = &gtmdl_hdmi_cec_driver_config_table[instance];

	/* check if instance number is in range */
	RETIF((instance < 0) || (instance >= MAX_UNITS), ERR_DLHDMICEC_BAD_INSTANCE)

	/* Disable calibration */
	i2c_read_buffer[0] = CEC_DISABLE_CALIBRATION;
	err = set_cec_hw_register(p_dis, E_REG_CEC_CAL_XOSC_CTRL1, i2c_read_buffer[0]);
	RETIF(err != 0, err)

#endif  /*  TMFL_TDA9989 */

	return err;
}

/*============================================================================*/
/*                     END OF FILE                                            */
/*============================================================================*/
