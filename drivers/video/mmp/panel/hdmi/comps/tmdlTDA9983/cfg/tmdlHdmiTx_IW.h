/**
 * Copyright (C) 2007 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_IW.h
 *
 * \version       $Revision: 1 $
 *
 * \date          $Date: 07/08/07 16:00 $
 *
 * \brief         devlib driver component API for the TDA998x HDMI Transmitters
 *
 * \section refs  Reference Documents
 * TDA998x Driver - FRS.doc,
 * TDA998x Driver - tx - SCS.doc
 *
 * \section info  Change Information
 *
 * \verbatim
 *
 *   $History: tx_IW.h $
 *
 * *****************  Version 1  *****************
 * User: J. Lamotte Date: 07/08/07   Time: 16:00
 * Updated in $/Source/tx/inc
 * initial version
 *
 *
 *   \endverbatim
 *
 * */

#ifndef DLHDMITX_IW_H
#define DLHDMITX_IW_H

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#ifdef TMFL_OS_WINDOWS
#define _WIN32_WINNT 0x0500
#include "windows.h"
#endif

#include "tmNxTypes.h"
#include "tmdlHdmiTx_Types.h"

/*============================================================================*/
/*                       TYPE DEFINITIONS                                     */
/*============================================================================*/
typedef void (*tx_iwfunc_ptr_t)(void);
typedef u8 tx_iwtask_handle_t;
typedef u8 tx_iwqueue_handle_t;
#ifdef __LINUX_ARM_ARCH__ /* AL */
typedef unsigned long tx_iwsem_handle_t;
#else
#ifdef TMFL_OS_WINDOWS
typedef HANDLE tx_iwsem_handle_t;
#else
typedef u8 tx_iwsem_handle_t;
#endif
#endif

/**
 * \brief Enum listing all available devices for enable/disable interrupts
 * */
typedef enum {
	DL_HDMI_IW_RX_1,
	DL_HDMI_IW_RX_2,
	DL_HDMI_IW_TX_1,
	DL_HDMI_IW_TX_2,
	DL_HDMI_IW_CEC_1,
	DL_HDMI_IW_CEC_2
} dl_hdmi_iwdevice_interrupt_t;

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/******************************************************************************
 *    \brief  This function creates a task and allocates all the necessary
 *            resources. Note that creating a task do not start it automatically,
 *            an explicit call to txIWTaskStart must be made.
 *
 *    \param  pFunc       Pointer to the function that will be executed in the task context.
 *    \param  Priority    Priority of the task. The minimum priority is 0, the maximum is 255.
 *    \param  StackSize   Size of the stack to allocate for this task.
 *    \param  pHandle     Pointer to the handle buffer.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_iwtask_create(tx_iwfunc_ptr_t p_func, u8 priority, u16 stack_size, tx_iwtask_handle_t *p_handle);

/******************************************************************************
 *    \brief  This function destroys an existing task and frees resources used by it.
 *
 *    \param  Handle  Handle of the task to be destroyed, as returned by txIWTaskCreate.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwtask_destroy(tx_iwtask_handle_t handle);

/******************************************************************************
 *    \brief  This function start an existing task.
 *
 *    \param  Handle  Handle of the task to be started.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_ALREADY_STARTED: the function is already started
 *            - ERR_DLHDMITX_NOT_STARTED: the function is not started
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwtask_start(tx_iwtask_handle_t handle);

/******************************************************************************
 *    \brief  This function blocks the current task for the specified amount time. This is a passive wait.
 *
 *    \param  Duration    Duration of the task blocking in milliseconds.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *
 ******************************************************************************/
error_code_t tx_iwwait(u16 duration);

/******************************************************************************
 *    \brief  This function creates a message queue.
 *
 *    \param  QueueSize   Maximum number of messages in the message queue.
 *    \param  pHandle     Pointer to the handle buffer.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *
 ******************************************************************************/
error_code_t tx_iwqueue_create(u8 queue_size, tx_iwqueue_handle_t *p_handle);

/******************************************************************************
 *    \brief  This function destroys an existing message queue.
 *
 *    \param  Handle  Handle of the queue to be destroyed.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *
 ******************************************************************************/
error_code_t tx_iwqueue_destroy(tx_iwqueue_handle_t handle);

/******************************************************************************
 *    \brief  This function sends a message into the specified message queue.
 *
 *    \param  Handle  Handle of the queue that will receive the message.
 *    \param  Message Message to be sent.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_FULL: the queue is full
 *
 ******************************************************************************/
error_code_t tx_iwqueue_send(tx_iwqueue_handle_t handle, u8 message);

/******************************************************************************
 *    \brief  This function reads a message from the specified message queue.
 *
 *    \param  Handle      Handle of the queue from which to read the message.
 *    \param  pMessage    Pointer to the message buffer.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_iwqueue_receive(tx_iwqueue_handle_t handle, u8 *p_message);

/******************************************************************************
 *    \brief  This function creates a semaphore.
 *
 *    \param  pHandle Pointer to the handle buffer.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_iwsemaphore_create(tx_iwsem_handle_t *p_handle);

/******************************************************************************
 *    \brief  This function destroys an existing semaphore.
 *
 *    \param  Handle  Handle of the semaphore to be destroyed.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwsemaphore_destroy(tx_iwsem_handle_t handle);

/******************************************************************************
 *    \brief  This function acquires the specified semaphore.
 *
 *    \param  Handle  Handle of the semaphore to be acquired.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwsemaphore_p(tx_iwsem_handle_t handle);

/******************************************************************************
 *    \brief  This function releases the specified semaphore.
 *
 *    \param  Handle  Handle of the semaphore to be released.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwsemaphore_v(tx_iwsem_handle_t handle);

/******************************************************************************
 *    \brief  This function disables the interrupts for a specific device.
 *
 *    \param
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
void tx_iwdisable_interrupts(dl_hdmi_iwdevice_interrupt_t device);

/******************************************************************************
 *    \brief  This function enables the interrupts for a specific device.
 *
 *    \param
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
void tx_iwenable_interrupts(dl_hdmi_iwdevice_interrupt_t device);

#endif /* DLHDMITX_IW_H */

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

