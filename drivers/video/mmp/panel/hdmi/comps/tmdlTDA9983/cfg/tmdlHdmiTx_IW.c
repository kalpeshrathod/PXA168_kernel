/*
 * Copyright (C) 2007 NXP N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of NXP N.V. and is confidential in nature. Under no circumstances
 * is this software to be  exposed to or placed under an Open Source License of
 * any type without the expressed written permission of NXP N.V.
 *
 * \file          tx_IW.c
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
 *   $History: tx_IW.c $
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

/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#define _WIN32_WINNT 0x0500

#ifndef WINDOWS_QMORE
#include "windows.h"
#endif

#include "tmNxTypes.h"
#include "tmNxCompId.h"
#include "tmdlHdmiTx_Types.h"
#include "tmdlHdmiTx_cfg.h"
#include "tmdlHdmiTx_IW.h"

/*============================================================================*/
/*                       DEFINES                                              */
/*============================================================================*/

/* maximum number of tasks that can be handled by the wrapper */
#define MAX_TASKS  5
/* maximum number of message queues that can be handled by the wrapper */
#define MAX_QUEUES 5

/*============================================================================*/
/*                                MACRO                                       */
/*============================================================================*/

/* macro for quick error handling */
#define RETIF(cond, rslt) if ((cond)){return (rslt);}

/*============================================================================*/
/*                       TYPE DEFINITIONS                                     */
/*============================================================================*/

/* structure describing each task handled by the wrapper */
typedef struct {
	bool                    created;
	bool                    started;
	u8                   priority;
	u16                  stack_size;
	DWORD                   thread_id;
	HANDLE                  thread_handle;
	LPTHREAD_START_ROUTINE  associated_thread;
	tx_iwfunc_ptr_t   associated_task;
} iw_tcb_t;

/* structure describing each message queue handled by the wrapper */
typedef struct {
	bool    created;
	HANDLE  access_semaphore;
	HANDLE  count_semaphore;
	u16  queue_fullness;
	u16  queue_size;
	u16  write_pointer;
	u16  read_pointer;
	u8  *queue;
} iw_queue_t;

/*============================================================================*/
/*                       FUNCTION PROTOTYPES                                  */
/*============================================================================*/

DWORD WINAPI thread_proc0(LPVOID lp_parameter);
DWORD WINAPI thread_proc1(LPVOID lp_parameter);
DWORD WINAPI thread_proc2(LPVOID lp_parameter);
DWORD WINAPI thread_proc3(LPVOID lp_parameter);
DWORD WINAPI thread_proc4(LPVOID lp_parameter);

/*============================================================================*/
/*                       VARIABLES                                            */
/*============================================================================*/

/* table storing all tasks descriptions */
iw_tcb_t task_table[MAX_TASKS] = {
	{false, false, 0, 0, 0, NULL, thread_proc0},
	{false, false, 0, 0, 0, NULL, thread_proc1},
	{false, false, 0, 0, 0, NULL, thread_proc2},
	{false, false, 0, 0, 0, NULL, thread_proc3},
	{false, false, 0, 0, 0, NULL, thread_proc4}
};

/* table storing all message queues descriptions */
iw_queue_t queue_table[MAX_QUEUES] = {
	{false, 0, 0, 0, 0, 0, 0, NULL},
	{false, 0, 0, 0, 0, 0, 0, NULL},
	{false, 0, 0, 0, 0, 0, 0, NULL},
	{false, 0, 0, 0, 0, 0, 0, NULL},
	{false, 0, 0, 0, 0, 0, 0, NULL}
};

/*============================================================================*/
/*                                  FUNCTION                                  */
/*============================================================================*/

/******************************************************************************
 *    \brief  This function creates a task and allocates all the necessary resources.
 *            Note that creating a task do not start it automatically,
 *            an explicit call to txIWTaskStart must be made.
 *
 *    \param  pFunc        Pointer to the function that will be executed in the task context.
 *    \param  Priority     Priority of the task. The minimum priority is 0, the maximum is 255.
 *    \param  StackSize    Size of the stack to allocate for this task.
 *    \param  pHandle      Pointer to the handle buffer.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *            - ERR_DLHDMITX_INCONSISTENT_PARAMS: an input parameter is
 *              inconsistent
 *
 ******************************************************************************/
error_code_t tx_iwtask_create
(
    tx_iwfunc_ptr_t       p_func,
    u8                       priority,
    u16                      stack_size,
    tx_iwtask_handle_t    *p_handle
)
{
	u32 i;

	/* check that input pointer is not NULL */
	RETIF(p_func == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* check that input pointer is not NULL */
	RETIF(p_handle == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* search for available task slot */
	for(i = 0; i < MAX_TASKS; i++) {
		if(task_table[i].created == false)
			break;

	}
	RETIF(i >= MAX_TASKS, ERR_DLHDMITX_NO_RESOURCES)

	/* store task parameters into the dedicated structure */
	task_table[i].priority = priority;
	task_table[i].stack_size = stack_size;
	task_table[i].associated_task = p_func;
	task_table[i].created = true;

	*p_handle = (tx_iwtask_handle_t)i;

	return(0);
}

/******************************************************************************
 *    \brief  This function destroys an existing task and frees resources used by it.
 *
 *    \param  Handle  Handle of the task to be destroyed, as returned by
 *                    txIWTaskCreate.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_RESOURCE_NOT_OWNED: the caller does not own
 *              the resource
 *            - ERR_DLHDMITX_BAD_HANDLE: the handle number is wrong
 *
 ******************************************************************************/
error_code_t tx_iwtask_destroy
(
    tx_iwtask_handle_t handle
)
{
	/* check if handle number is in range */
	RETIF((handle < 0) || (handle >= MAX_TASKS), ERR_DLHDMITX_BAD_HANDLE)

	/* check if handle corresponding to task is created */
	RETIF(task_table[handle].created == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	if(task_table[handle].started == true) {
		terminate_thread(task_table[handle].thread_handle, 0);
		task_table[handle].started = false;
	}
	task_table[handle].created = false;
	close_handle(task_table[handle].thread_handle);

	return(0);
}

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
error_code_t tx_iwtask_start
(
    tx_iwtask_handle_t handle
)
{
	HANDLE thread_handle;

	/* check if handle number is in range */
	RETIF((handle < 0) || (handle >= MAX_TASKS), ERR_DLHDMITX_BAD_HANDLE)

	/* check if task is already started */
	RETIF(task_table[handle].started == true, ERR_DLHDMITX_ALREADY_STARTED)

	/* start thread associated to the task */
	thread_handle = create_thread(NULL,
	                              (SIZE_T)task_table[handle].stack_size,
	                              task_table[handle].associated_thread,
	                              NULL,
	                              0,
	                              &(task_table[handle].thread_id));

	/* check return code for errors */
	RETIF(!thread_handle, ERR_DLHDMITX_NOT_STARTED)

	/* set the priority task */
	set_thread_priority(thread_handle, (int)task_table[handle].priority);

	/* update task status */
	task_table[handle].thread_handle = thread_handle;
	task_table[handle].started = true;

	return(0);
}

/******************************************************************************
 *    \brief  This function blocks the current task for the specified amount time.
 *            This is a passive wait.
 *
 *    \param  Duration    Duration of the task blocking in milliseconds.
 *
 *    \return The call result:
 *            - 0: the call was successful
 *            - ERR_DLHDMITX_NO_RESOURCES: the resource is not available
 *
 ******************************************************************************/
error_code_t tx_iwwait
(
    u16 duration
)
{
	HANDLE        timer_handle;
	LARGE_INTEGER time;

	timer_handle = create_waitable_timer(NULL, true, NULL);
	RETIF(timer_handle == NULL, ERR_DLHDMITX_NO_RESOURCES)

	time.quad_part = -10000 * (long)duration;
	set_waitable_timer(timer_handle, &time, 0, NULL, NULL, false);
	wait_for_single_object(timer_handle, INFINITE);

	close_handle(timer_handle);

	return(0);
}

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
error_code_t tx_iwqueue_create
(
    u8                       queue_size,
    tx_iwqueue_handle_t   *p_handle
)
{
	u32 i;

	/* check that input pointer is not NULL */
	RETIF(p_handle == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* search for available queue slot */
	for(i = 0; i < MAX_QUEUES; i++) {
		if(queue_table[i].created == false)
			break;

	}

	RETIF(i >= MAX_QUEUES, ERR_DLHDMITX_NO_RESOURCES)

	/* allocate memory for the queue */
	queue_table[i].queue = (u8 *)global_alloc(GMEM_FIXED, queue_size);
	RETIF(queue_table[i].queue == NULL, ERR_DLHDMITX_NO_RESOURCES)

	/* allocate semaphores for the queue */
	queue_table[i].count_semaphore = create_semaphore(NULL, 0, queue_size, NULL);
	RETIF(queue_table[i].count_semaphore == NULL, ERR_DLHDMITX_NO_RESOURCES)

	queue_table[i].access_semaphore = create_semaphore(NULL, 1, 1, NULL);
	RETIF(queue_table[i].access_semaphore == NULL, ERR_DLHDMITX_NO_RESOURCES)

	/* update status of the queue table */
	queue_table[i].created = true;
	queue_table[i].queue_size = queue_size;
	*p_handle = (tx_iwqueue_handle_t)i;

	return(0);
}

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
error_code_t tx_iwqueue_destroy
(
    tx_iwqueue_handle_t handle
)
{
	RETIF(handle > MAX_QUEUES, ERR_DLHDMITX_BAD_HANDLE)

	RETIF(queue_table[handle].created == false, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	global_free((LPVOID)queue_table[handle].queue);
	close_handle(queue_table[handle].count_semaphore);
	close_handle(queue_table[handle].access_semaphore);
	queue_table[handle].created = false;

	return(0);
}

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
error_code_t tx_iwqueue_send
(
    tx_iwqueue_handle_t   handle,
    u8                       message
)
{
	error_code_t error_code = 0;

	/* check that handle is correct */
	RETIF(handle > MAX_QUEUES, ERR_DLHDMITX_BAD_HANDLE)

	RETIF(queue_table[handle].created != true, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	/* ask for exclusive access to this queue */
	wait_for_single_object(queue_table[handle].access_semaphore, INFINITE);

	if(queue_table[handle].queue_fullness < (queue_table[handle].queue_size - 1)) {
		queue_table[handle].queue[queue_table[handle].write_pointer] = message;
		queue_table[handle].queue_fullness++;
		queue_table[handle].write_pointer++;
		if(queue_table[handle].write_pointer == queue_table[handle].queue_size) {
			queue_table[handle].write_pointer = 0;
		}
		release_semaphore(queue_table[handle].count_semaphore, 1, NULL);
	} else {
		error_code = ERR_DLHDMITX_FULL;
	}

	/* release access to this queue */
	release_semaphore(queue_table[handle].access_semaphore, 1, NULL);

	return(0);
}

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
error_code_t tx_iwqueue_receive
(
    tx_iwqueue_handle_t handle, u8 *p_message
)
{
	/* check that handle is correct */
	RETIF(handle > MAX_QUEUES, ERR_DLHDMITX_BAD_HANDLE)

	RETIF(queue_table[handle].created != true, ERR_DLHDMITX_RESOURCE_NOT_OWNED)

	/* check that input pointer is not NULL */
	RETIF(p_message == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	/* ask for a new message by acquiring the counting semaphore */
	wait_for_single_object(queue_table[handle].count_semaphore, INFINITE);

	/* if we reach this point, this means that we got a message */
	/* ask for exclusive access to this queue */
	wait_for_single_object(queue_table[handle].access_semaphore, INFINITE);

	*p_message = queue_table[handle].queue[queue_table[handle].read_pointer];
	queue_table[handle].queue_fullness--;
	queue_table[handle].read_pointer++;
	if(queue_table[handle].read_pointer == queue_table[handle].queue_size) {
		queue_table[handle].read_pointer = 0;
	}

	/* release access to this queue */
	release_semaphore(queue_table[handle].access_semaphore, 1, NULL);

	return(0);
}

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
error_code_t tx_iwsemaphore_create
(
    tx_iwsem_handle_t *p_handle
)
{
	/* check that input pointer is not NULL */
	RETIF(p_handle == NULL, ERR_DLHDMITX_INCONSISTENT_PARAMS)

	*p_handle = (tx_iwsem_handle_t)create_semaphore(NULL, 1, 1, NULL);

	RETIF((*p_handle) == NULL, ERR_DLHDMITX_NO_RESOURCES)

	return(0);
}

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
error_code_t tx_iwsemaphore_destroy
(
    tx_iwsem_handle_t handle
)
{
	RETIF(close_handle(handle) == false, ERR_DLHDMITX_BAD_HANDLE)

	return(0);
}

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
error_code_t tx_iwsemaphore_p
(
    tx_iwsem_handle_t handle
)
{
	RETIF(wait_for_single_object(handle, INFINITE) != WAIT_OBJECT_0, ERR_DLHDMITX_BAD_HANDLE)

	return(0);
}

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
error_code_t tx_iwsemaphore_v
(
    tx_iwsem_handle_t handle
)
{
	RETIF(release_semaphore(handle, 1, NULL) == 0, ERR_DLHDMITX_BAD_HANDLE)

	return(0);
}

/******************************************************************************
 *    \brief  This function disables the interrupts for a specific device.
 *
 *    \param
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
void tx_iwdisable_interrupts(dl_hdmi_iwdevice_interrupt_t device)
{
	device;
}

/******************************************************************************
 *    \brief  This function enables the interrupts for a specific device.
 *
 *    \param
 *
 *    \return The call result:
 *            - 0: the call was successful
 *
 ******************************************************************************/
void tx_iwenable_interrupts(dl_hdmi_iwdevice_interrupt_t device)
{
	device;
}

/******************************************************************************/
DWORD WINAPI thread_proc0(LPVOID lp_parameter)
{
	/* dummy reference to avoid compilation warning C4100 */
	lp_parameter;

	/* call the registered task */
	task_table[0].associated_task();
	/* if we reach this point, the task is terminated, so update its status */
	task_table[0].started = false;

	return(0);
}

/******************************************************************************/
DWORD WINAPI thread_proc1(LPVOID lp_parameter)
{
	/* dummy reference to avoid compilation warning C4100 */
	lp_parameter;

	/* call the registered task */
	task_table[1].associated_task();
	/* if we reach this point, the task is terminated, so update its status */
	task_table[1].started = false;

	return(0);
}

/******************************************************************************/
DWORD WINAPI thread_proc2(LPVOID lp_parameter)
{
	/* dummy reference to avoid compilation warning C4100 */
	lp_parameter;

	/* call the registered task */
	task_table[2].associated_task();
	/* if we reach this point, the task is terminated, so update its status */
	task_table[2].started = false;

	return(0);
}

/******************************************************************************/
DWORD WINAPI thread_proc3(LPVOID lp_parameter)
{
	/* dummy reference to avoid compilation warning C4100 */
	lp_parameter;

	/* call the registered task */
	task_table[3].associated_task();
	/* if we reach this point, the task is terminated, so update its status */
	task_table[3].started = false;

	return(0);
}

DWORD WINAPI thread_proc4(LPVOID lp_parameter)
{
	/* dummy reference to avoid compilation warning C4100 */
	lp_parameter;

	/* call the registered task */
	task_table[4].associated_task();
	/* if we reach this point, the task is terminated, so update its status */
	task_table[4].started = false;

	return(0);
}

/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

