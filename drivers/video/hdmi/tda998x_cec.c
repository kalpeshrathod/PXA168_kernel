/*****************************************************************************/
/* Copyright (c) 2009 NXP Semiconductors BV                                  */
/*                                                                           */
/* This program is free software; you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by      */
/* the Free Software Foundation, using version 2 of the License.             */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307       */
/* USA.                                                                      */
/*                                                                           */
/*****************************************************************************/

#define _cec_c_

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <asm/uaccess.h>
#include <mach/gpio.h>

/* HDMI DevLib */
#include "tmNxCompId.h"
#include "tmdlHdmiCEC.h"
#include "tmdlHdmiCEC_local.h"

/* local */
#include "tda998x_version.h"
#include "tda998x_cec.h"
#include "tda998x_ioctl.h"

/*
 *
 * DEFINITION
 * ----------
 * LEVEL 0
 *
 * */

/*
 *  Global
 * */

MODULE_DEVICE_TABLE(i2c, this_i2c_id);
static const struct i2c_device_id this_i2c_id[] = {
	{ CEC_NAME, 0 },
	{ },
};
cec_instance our_instance;
static struct cdev our_cdev, *this_cdev = &our_cdev;

#ifdef TWL4030_HACK
/* AL : hack to bypass keypad */
struct input_dev *gkp_input;
extern struct input_dev *get_twm4030_input(void);
#endif

/*
 * Dependancies to HdmiTx module
 * */

extern void register_cec_interrupt(cec_callback_t fct);
extern void unregister_cec_interrupt(void);
extern short edid_phy_addr(void);
extern int hdmi_enable(void);
extern int hdmi_disable(int event_tracking);
extern cec_power get_hdmi_status(void);
extern cec_power get_hpd_status(void);
extern int edid_received(void);

/*
 *  Module params
 * */

static int param_verbose = 0, param_major = 0, param_minor = 0, param_device = 4, param_addr = 0xFFFF;
module_param_named(verbose, param_verbose, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(verbose, "make the driver verbose");
module_param_named(major, param_major, int, S_IRUGO);
MODULE_PARM_DESC(major, "the major number of the device mapper");
module_param_named(device, param_device, int, S_IRUGO);
MODULE_PARM_DESC(device, "device type can be 0:tv, 1:rec 3:tuner 4:mediaplayer, 5:audio");
module_param_named(addr, param_addr, int, S_IRUGO);
MODULE_PARM_DESC(addr, "physical address (until EDID received)");

/*
 *
 * TOOLBOX
 * -------
 * LEVEL 1
 *
 * - i2c read/write
 * - chip Id check
 * - i2c client info
 *
 * */

/*
 *  Get main and unique I2C Client driver handle
 * */
struct i2c_client *get_this_i2c_client(void) {
	cec_instance *this = &our_instance;
	return this->driver.i2c_client;
}

/*
 * error handling
 * */
static char *hdmi_cec_err_string(int err)
{
	switch(err) {
	case ERR_DLHDMICEC_COMPATIBILITY: {
		return "SW interface compatibility";
		break;
	}
	case ERR_DLHDMICEC_MAJOR_VERSION: {
		return "SW major version error";
		break;
	}
	case ERR_DLHDMICEC_COMP_VERSION: {
		return "SW component version error";
		break;
	}
	case ERR_DLHDMICEC_BAD_UNIT_NUMBER: {
		return "invalid device unit number";
		break;
	}
	case ERR_DLHDMICEC_BAD_INSTANCE: {
		return "bad input instance value  ";
		break;
	}
	case ERR_DLHDMICEC_BAD_HANDLE: {
		return "bad input handle";
		break;
	}
	case ERR_DLHDMICEC_BAD_PARAMETER: {
		return "invalid input parameter";
		break;
	}
	case ERR_DLHDMICEC_NO_RESOURCES: {
		return "resource is not available ";
		break;
	}
	case ERR_DLHDMICEC_RESOURCE_OWNED: {
		return "resource is already in use";
		break;
	}
	case ERR_DLHDMICEC_RESOURCE_NOT_OWNED: {
		return "caller does not own resource";
		break;
	}
	case ERR_DLHDMICEC_INCONSISTENT_PARAMS: {
		return "inconsistent input params";
		break;
	}
	case ERR_DLHDMICEC_NOT_INITIALIZED: {
		return "component is not initializ";
		break;
	}
	case ERR_DLHDMICEC_NOT_SUPPORTED: {
		return "function is not supported";
		break;
	}
	case ERR_DLHDMICEC_INIT_FAILED: {
		return "initialization failed";
		break;
	}
	case ERR_DLHDMICEC_BUSY: {
		return "component is busy";
		break;
	}
	case ERR_DLHDMICEC_I2C_READ: {
		return "read error";
		break;
	}
	case ERR_DLHDMICEC_I2C_WRITE: {
		return "write error";
		break;
	}
	case ERR_DLHDMICEC_FULL: {
		return "queue is full";
		break;
	}
	case ERR_DLHDMICEC_NOT_STARTED: {
		return "function is not started";
		break;
	}
	case ERR_DLHDMICEC_ALREADY_STARTED: {
		return "function is already starte";
		break;
	}
	case ERR_DLHDMICEC_ASSERTION: {
		return "assertion failure";
		break;
	}
	case ERR_DLHDMICEC_INVALID_STATE: {
		return "invalid state for function";
		break;
	}
	case ERR_DLHDMICEC_OPERATION_NOT_PERMITTED: {
		return "corresponds to posix EPERM";
		break;
	}
	default : {
		return "unexpected error";
		break;
	}
	}
}

char *cec_opcode(int op)
{
	switch(op) {
	case CEC_OPCODE_FEATURE_ABORT: {
		return "CEC_OPCODE_FEATURE_ABORT";
		break;
	}
	case CEC_OPCODE_IMAGE_VIEW_ON: {
		return "CEC_OPCODE_IMAGE_VIEW_ON";
		break;
	}
	case CEC_OPCODE_TUNER_STEP_INCREMENT: {
		return "CEC_OPCODE_TUNER_STEP_INCREMENT";
		break;
	}
	case CEC_OPCODE_TUNER_STEP_DECREMENT: {
		return "CEC_OPCODE_TUNER_STEP_DECREMENT";
		break;
	}
	case CEC_OPCODE_TUNER_DEVICE_STATUS: {
		return "CEC_OPCODE_TUNER_DEVICE_STATUS";
		break;
	}
	case CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS: {
		return "CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS";
		break;
	}
	case CEC_OPCODE_RECORD_ON: {
		return "CEC_OPCODE_RECORD_ON";
		break;
	}
	case CEC_OPCODE_RECORD_STATUS: {
		return "CEC_OPCODE_RECORD_STATUS";
		break;
	}
	case CEC_OPCODE_RECORD_OFF: {
		return "CEC_OPCODE_RECORD_OFF";
		break;
	}
	case CEC_OPCODE_TEXT_VIEW_ON: {
		return "CEC_OPCODE_TEXT_VIEW_ON";
		break;
	}
	case CEC_OPCODE_RECORD_TV_SCREEN: {
		return "CEC_OPCODE_RECORD_TV_SCREEN";
		break;
	}
	case CEC_OPCODE_GIVE_DECK_STATUS: {
		return "CEC_OPCODE_GIVE_DECK_STATUS";
		break;
	}
	case CEC_OPCODE_DECK_STATUS: {
		return "CEC_OPCODE_DECK_STATUS";
		break;
	}
	case CEC_OPCODE_SET_MENU_LANGUAGE: {
		return "CEC_OPCODE_SET_MENU_LANGUAGE";
		break;
	}
	case CEC_OPCODE_CLEAR_ANALOGUE_TIMER: {
		return "CEC_OPCODE_CLEAR_ANALOGUE_TIMER";
		break;
	}
	case CEC_OPCODE_SET_ANALOGUE_TIMER: {
		return "CEC_OPCODE_SET_ANALOGUE_TIMER";
		break;
	}
	case CEC_OPCODE_TIMER_STATUS: {
		return "CEC_OPCODE_TIMER_STATUS";
		break;
	}
	case CEC_OPCODE_STANDBY: {
		return "CEC_OPCODE_STANDBY";
		break;
	}
	case CEC_OPCODE_PLAY: {
		return "CEC_OPCODE_PLAY";
		break;
	}
	/*       case CEC_OPCODE_DECK_CONTROL: {return "CEC_OPCODE_DECK_CONTROL";break;} */
	case CEC_OPCODE_TIMER_CLEARED_STATUS: {
		return "CEC_OPCODE_TIMER_CLEARED_STATUS";
		break;
	}
	case CEC_OPCODE_USER_CONTROL_PRESSED: {
		return "CEC_OPCODE_USER_CONTROL_PRESSED";
		break;
	}
	case CEC_OPCODE_USER_CONTROL_RELEASED: {
		return "CEC_OPCODE_USER_CONTROL_RELEASED";
		break;
	}
	case CEC_OPCODE_GIVE_OSD_NAME: {
		return "CEC_OPCODE_GIVE_OSD_NAME";
		break;
	}
	case CEC_OPCODE_SET_OSD_NAME: {
		return "CEC_OPCODE_SET_OSD_NAME";
		break;
	}
	case CEC_OPCODE_SET_OSD_STRING: {
		return "CEC_OPCODE_SET_OSD_STRING";
		break;
	}
	case CEC_OPCODE_SET_TIMER_PROGRAM_TITLE: {
		return "CEC_OPCODE_SET_TIMER_PROGRAM_TITLE";
		break;
	}
	case CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST: {
		return "CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST";
		break;
	}
	case CEC_OPCODE_GIVE_AUDIO_STATUS: {
		return "CEC_OPCODE_GIVE_AUDIO_STATUS";
		break;
	}
	case CEC_OPCODE_SET_SYSTEM_AUDIO_MODE: {
		return "CEC_OPCODE_SET_SYSTEM_AUDIO_MODE";
		break;
	}
	case CEC_OPCODE_REPORT_AUDIO_STATUS: {
		return "CEC_OPCODE_REPORT_AUDIO_STATUS";
		break;
	}
	case CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS: {
		return "CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS";
		break;
	}
	case CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS: {
		return "CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS";
		break;
	}
	case CEC_OPCODE_ROUTING_CHANGE: {
		return "CEC_OPCODE_ROUTING_CHANGE";
		break;
	}
	case CEC_OPCODE_ROUTING_INFORMATION: {
		return "CEC_OPCODE_ROUTING_INFORMATION";
		break;
	}
	case CEC_OPCODE_ACTIVE_SOURCE: {
		return "CEC_OPCODE_ACTIVE_SOURCE";
		break;
	}
	case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS: {
		return "CEC_OPCODE_GIVE_PHYSICAL_ADDRESS";
		break;
	}
	case CEC_OPCODE_REPORT_PHYSICAL_ADDRESS: {
		return "CEC_OPCODE_REPORT_PHYSICAL_ADDRESS";
		break;
	}
	case CEC_OPCODE_REQUEST_ACTIVE_SOURCE: {
		return "CEC_OPCODE_REQUEST_ACTIVE_SOURCE";
		break;
	}
	case CEC_OPCODE_SET_STREAM_PATH: {
		return "CEC_OPCODE_SET_STREAM_PATH";
		break;
	}
	case CEC_OPCODE_DEVICE_VENDOR_ID: {
		return "CEC_OPCODE_DEVICE_VENDOR_ID";
		break;
	}
	case CEC_OPCODE_VENDOR_COMMAND: {
		return "CEC_OPCODE_VENDOR_COMMAND";
		break;
	}
	case CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN: {
		return "CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN";
		break;
	}
	case CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP: {
		return "CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP";
		break;
	}
	case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID: {
		return "CEC_OPCODE_GIVE_DEVICE_VENDOR_ID";
		break;
	}
	case CEC_OPCODE_MENU_REQUEST: {
		return "CEC_OPCODE_MENU_REQUEST";
		break;
	}
	case CEC_OPCODE_MENU_STATUS: {
		return "CEC_OPCODE_MENU_STATUS";
		break;
	}
	case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS: {
		return "CEC_OPCODE_GIVE_DEVICE_POWER_STATUS";
		break;
	}
	case CEC_OPCODE_REPORT_POWER_STATUS: {
		return "CEC_OPCODE_REPORT_POWER_STATUS";
		break;
	}
	case CEC_OPCODE_GET_MENU_LANGUAGE: {
		return "CEC_OPCODE_GET_MENU_LANGUAGE";
		break;
	}
	case CEC_OPCODE_SET_ANALOGUE_SERVICE: {
		return "CEC_OPCODE_SET_ANALOGUE_SERVICE";
		break;
	}
	case CEC_OPCODE_SET_DIGITAL_SERVICE: {
		return "CEC_OPCODE_SET_DIGITAL_SERVICE";
		break;
	}
	case CEC_OPCODE_SET_DIGITAL_TIMER: {
		return "CEC_OPCODE_SET_DIGITAL_TIMER";
		break;
	}
	case CEC_OPCODE_CLEAR_DIGITAL_TIMER: {
		return "CEC_OPCODE_CLEAR_DIGITAL_TIMER";
		break;
	}
	case CEC_OPCODE_SET_AUDIO_RATE: {
		return "CEC_OPCODE_SET_AUDIO_RATE";
		break;
	}
	case CEC_OPCODE_INACTIVE_SOURCE: {
		return "CEC_OPCODE_INACTIVE_SOURCE";
		break;
	}
	case CEC_OPCODE_CEC_VERSION: {
		return "CEC_OPCODE_CEC_VERSION";
		break;
	}
	case CEC_OPCODE_GET_CEC_VERSION: {
		return "CEC_OPCODE_GET_CEC_VERSION";
		break;
	}
	case CEC_OPCODE_VENDOR_COMMAND_WITH_ID: {
		return "CEC_OPCODE_VENDOR_COMMAND_WITH_ID";
		break;
	}
	case CEC_OPCODE_CLEAR_EXTERNAL_TIMER: {
		return "CEC_OPCODE_CLEAR_EXTERNAL_TIMER";
		break;
	}
	case CEC_OPCODE_SET_EXTERNAL_TIMER: {
		return "CEC_OPCODE_SET_EXTERNAL_TIMER";
		break;
	}
	case CEC_OPCODE_ABORT_MESSAGE: {
		return "CEC_OPCODE_ABORT_MESSAGE";
		break;
	}
	default : {
		return "unknown";
		break;
	}
	}
}

static char *cec_ioctl(int io)
{
	switch(io) {
	case CEC_VERBOSE_ON_CMD: {
		return "CEC_VERBOSE_ON_CMD";
		break;
	}
	case CEC_VERBOSE_OFF_CMD: {
		return "CEC_VERBOSE_OFF_CMD";
		break;
	}
	case CEC_BYEBYE_CMD: {
		return "CEC_BYEBYE_CMD";
		break;
	}
	case CEC_IOCTL_RX_ADDR_CMD: {
		return "CEC_IOCTL_RX_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_PHY_ADDR_CMD: {
		return "CEC_IOCTL_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_WAIT_FRAME_CMD: {
		return "CEC_IOCTL_WAIT_FRAME_CMD";
		break;
	}
	case CEC_IOCTL_ABORT_MSG_CMD: {
		return "CEC_IOCTL_ABORT_MSG_CMD";
		break;
	}
	case CEC_IOCTL_ACTIVE_SRC_CMD: {
		return "CEC_IOCTL_ACTIVE_SRC_CMD";
		break;
	}
	case CEC_IOCTL_VERSION_CMD: {
		return "CEC_IOCTL_VERSION_CMD";
		break;
	}
	case CEC_IOCTL_CLEAR_ANALOGUE_TIMER_CMD: {
		return "CEC_IOCTL_CLEAR_ANALOGUE_TIMER_CMD";
		break;
	}
	case CEC_IOCTL_CLEAR_DIGITAL_TIMER_CMD: {
		return "CEC_IOCTL_CLEAR_DIGITAL_TIMER_CMD";
		break;
	}
	case CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG_CMD: {
		return "CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG_CMD";
		break;
	}
	case CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR_CMD: {
		return "CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_DECK_CTRL_CMD: {
		return "CEC_IOCTL_DECK_CTRL_CMD";
		break;
	}
	case CEC_IOCTL_DECK_STATUS_CMD: {
		return "CEC_IOCTL_DECK_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_DEVICE_VENDOR_ID_CMD: {
		return "CEC_IOCTL_DEVICE_VENDOR_ID_CMD";
		break;
	}
	case CEC_IOCTL_FEATURE_ABORT_CMD: {
		return "CEC_IOCTL_FEATURE_ABORT_CMD";
		break;
	}
	case CEC_IOCTL_GET_CEC_VERSION_CMD: {
		return "CEC_IOCTL_GET_CEC_VERSION_CMD";
		break;
	}
	case CEC_IOCTL_GET_MENU_LANGUAGE_CMD: {
		return "CEC_IOCTL_GET_MENU_LANGUAGE_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_AUDIO_STATUS_CMD: {
		return "CEC_IOCTL_GIVE_AUDIO_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_DECK_STATUS_CMD: {
		return "CEC_IOCTL_GIVE_DECK_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_DEVICE_POWER_STATUS_CMD: {
		return "CEC_IOCTL_GIVE_DEVICE_POWER_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_DEVICE_VENDOR_ID_CMD: {
		return "CEC_IOCTL_GIVE_DEVICE_VENDOR_ID_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_OSD_NAME_CMD: {
		return "CEC_IOCTL_GIVE_OSD_NAME_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_PHY_ADDR_CMD: {
		return "CEC_IOCTL_GIVE_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS_CMD: {
		return "CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS_CMD: {
		return "CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_IMAGE_VIEW_ON_CMD: {
		return "CEC_IOCTL_IMAGE_VIEW_ON_CMD";
		break;
	}
	case CEC_IOCTL_INACTIVE_SRC_CMD: {
		return "CEC_IOCTL_INACTIVE_SRC_CMD";
		break;
	}
	case CEC_IOCTL_MENU_REQUEST_CMD: {
		return "CEC_IOCTL_MENU_REQUEST_CMD";
		break;
	}
	case CEC_IOCTL_MENU_STATUS_CMD: {
		return "CEC_IOCTL_MENU_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_PLAY_CMD: {
		return "CEC_IOCTL_PLAY_CMD";
		break;
	}
	case CEC_IOCTL_POLLING_MSG_CMD: {
		return "CEC_IOCTL_POLLING_MSG_CMD";
		break;
	}
	case CEC_IOCTL_REC_OFF_CMD: {
		return "CEC_IOCTL_REC_OFF_CMD";
		break;
	}
	case CEC_IOCTL_REC_ON_ANALOGUE_SERVICE_CMD: {
		return "CEC_IOCTL_REC_ON_ANALOGUE_SERVICE_CMD";
		break;
	}
	case CEC_IOCTL_REC_ON_DIGITAL_SERVICE_CMD: {
		return "CEC_IOCTL_REC_ON_DIGITAL_SERVICE_CMD";
		break;
	}
	case CEC_IOCTL_REC_ON_EXT_PHY_ADDR_CMD: {
		return "CEC_IOCTL_REC_ON_EXT_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_REC_ON_EXT_PLUG_CMD: {
		return "CEC_IOCTL_REC_ON_EXT_PLUG_CMD";
		break;
	}
	case CEC_IOCTL_REC_ON_OWN_SRC_CMD: {
		return "CEC_IOCTL_REC_ON_OWN_SRC_CMD";
		break;
	}
	case CEC_IOCTL_REC_STATUS_CMD: {
		return "CEC_IOCTL_REC_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_REC_TV_SCREEN_CMD: {
		return "CEC_IOCTL_REC_TV_SCREEN_CMD";
		break;
	}
	case CEC_IOCTL_REPORT_AUDIO_STATUS_CMD: {
		return "CEC_IOCTL_REPORT_AUDIO_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_REPORT_PHY_ADDR_CMD: {
		return "CEC_IOCTL_REPORT_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_REPORT_POWER_STATUS_CMD: {
		return "CEC_IOCTL_REPORT_POWER_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_REQUEST_ACTIVE_SRC_CMD: {
		return "CEC_IOCTL_REQUEST_ACTIVE_SRC_CMD";
		break;
	}
	case CEC_IOCTL_ROUTING_CHANGE_CMD: {
		return "CEC_IOCTL_ROUTING_CHANGE_CMD";
		break;
	}
	case CEC_IOCTL_ROUTING_INFORMATION_CMD: {
		return "CEC_IOCTL_ROUTING_INFORMATION_CMD";
		break;
	}
	case CEC_IOCTL_SELECT_ANALOGUE_SERVICE_CMD: {
		return "CEC_IOCTL_SELECT_ANALOGUE_SERVICE_CMD";
		break;
	}
	case CEC_IOCTL_SELECT_DIGITAL_SERVICE_CMD: {
		return "CEC_IOCTL_SELECT_DIGITAL_SERVICE_CMD";
		break;
	}
	case CEC_IOCTL_SET_ANALOGUE_TIMER_CMD: {
		return "CEC_IOCTL_SET_ANALOGUE_TIMER_CMD";
		break;
	}
	case CEC_IOCTL_SET_AUDIO_RATE_CMD: {
		return "CEC_IOCTL_SET_AUDIO_RATE_CMD";
		break;
	}
	case CEC_IOCTL_SET_DIGITAL_TIMER_CMD: {
		return "CEC_IOCTL_SET_DIGITAL_TIMER_CMD";
		break;
	}
	case CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG_CMD: {
		return "CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG_CMD";
		break;
	}
	case CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR_CMD: {
		return "CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR_CMD";
		break;
	}
	case CEC_IOCTL_SET_MENU_LANGUAGE_CMD: {
		return "CEC_IOCTL_SET_MENU_LANGUAGE_CMD";
		break;
	}
	case CEC_IOCTL_SET_OSD_NAME_CMD: {
		return "CEC_IOCTL_SET_OSD_NAME_CMD";
		break;
	}
	case CEC_IOCTL_SET_OSD_STRING_CMD: {
		return "CEC_IOCTL_SET_OSD_STRING_CMD";
		break;
	}
	case CEC_IOCTL_SET_STREAM_PATH_CMD: {
		return "CEC_IOCTL_SET_STREAM_PATH_CMD";
		break;
	}
	case CEC_IOCTL_SET_SYS_AUDIO_MODE_CMD: {
		return "CEC_IOCTL_SET_SYS_AUDIO_MODE_CMD";
		break;
	}
	case CEC_IOCTL_SET_TIMER_PROGRAM_TITLE_CMD: {
		return "CEC_IOCTL_SET_TIMER_PROGRAM_TITLE_CMD";
		break;
	}
	case CEC_IOCTL_STANDBY_CMD: {
		return "CEC_IOCTL_STANDBY_CMD";
		break;
	}
	case CEC_IOCTL_SYS_AUDIO_MODE_REQUEST_CMD: {
		return "CEC_IOCTL_SYS_AUDIO_MODE_REQUEST_CMD";
		break;
	}
	case CEC_IOCTL_SYS_AUDIO_MODE_STATUS_CMD: {
		return "CEC_IOCTL_SYS_AUDIO_MODE_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_TEXT_VIEW_ON_CMD: {
		return "CEC_IOCTL_TEXT_VIEW_ON_CMD";
		break;
	}
	case CEC_IOCTL_TIMER_CLEARED_STATUS_CMD: {
		return "CEC_IOCTL_TIMER_CLEARED_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_TIMER_STATUS_CMD: {
		return "CEC_IOCTL_TIMER_STATUS_CMD";
		break;
	}
	case CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE_CMD: {
		return "CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE_CMD";
		break;
	}
	case CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL_CMD: {
		return "CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL_CMD";
		break;
	}
	case CEC_IOCTL_TUNER_STEP_DECREMENT_CMD: {
		return "CEC_IOCTL_TUNER_STEP_DECREMENT_CMD";
		break;
	}
	case CEC_IOCTL_TUNER_STEP_INCREMENT_CMD: {
		return "CEC_IOCTL_TUNER_STEP_INCREMENT_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_CMD: {
		return "CEC_IOCTL_USER_CTRL_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_PLAY_CMD: {
		return "CEC_IOCTL_USER_CTRL_PLAY_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT_CMD: {
		return "CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_SELECT_AVINPUT_CMD: {
		return "CEC_IOCTL_USER_CTRL_SELECT_AVINPUT_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_SELECT_MEDIA_CMD: {
		return "CEC_IOCTL_USER_CTRL_SELECT_MEDIA_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_TUNE_CMD: {
		return "CEC_IOCTL_USER_CTRL_TUNE_CMD";
		break;
	}
	case CEC_IOCTL_USER_CTRL_RELEASED_CMD: {
		return "CEC_IOCTL_USER_CTRL_RELEASED_CMD";
		break;
	}
	case CEC_IOCTL_VENDOR_COMMAND_CMD: {
		return "CEC_IOCTL_VENDOR_COMMAND_CMD";
		break;
	}
	case CEC_IOCTL_VENDOR_COMMAND_WITH_ID_CMD: {
		return "CEC_IOCTL_VENDOR_COMMAND_WITH_ID_CMD";
		break;
	}
	case CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN_CMD: {
		return "CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN_CMD";
		break;
	}
	case CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP_CMD: {
		return "CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP_CMD";
		break;
	}
	case CEC_IOCTL_GET_SW_VERSION_CMD: {
		return "CEC_IOCTL_GET_SW_VERSION_CMD";
		break;
	}
	case CEC_IOCTL_SET_POWER_STATE_CMD: {
		return "CEC_IOCTL_SET_POWER_STATE_CMD";
		break;
	}
	case CEC_IOCTL_GET_POWER_STATE_CMD: {
		return "CEC_IOCTL_GET_POWER_STATE_CMD";
		break;
	}
	case CEC_IOCTL_INSTANCE_CONFIG_CMD: {
		return "CEC_IOCTL_INSTANCE_CONFIG_CMD";
		break;
	}
	case CEC_IOCTL_INSTANCE_SETUP_CMD: {
		return "CEC_IOCTL_INSTANCE_SETUP_CMD";
		break;
	}
	case CEC_IOCTL_GET_INSTANCE_SETUP_CMD: {
		return "CEC_IOCTL_GET_INSTANCE_SETUP_CMD";
		break;
	}
	case CEC_IOCTL_ENABLE_EVENT_CMD: {
		return "CEC_IOCTL_ENABLE_EVENT_CMD";
		break;
	}
	case CEC_IOCTL_DISABLE_EVENT_CMD: {
		return "CEC_IOCTL_DISABLE_EVENT_CMD";
		break;
	}
	case CEC_IOCTL_ENABLE_CALIBRATION_CMD: {
		return "CEC_IOCTL_ENABLE_CALIBRATION_CMD";
		break;
	}
	case CEC_IOCTL_DISABLE_CALIBRATION_CMD: {
		return "CEC_IOCTL_DISABLE_CALIBRATION_CMD";
		break;
	}
	case CEC_IOCTL_SEND_MSG_CMD: {
		return "CEC_IOCTL_SEND_MSG_CMD";
		break;
	}
	case CEC_IOCTL_SET_REGISTER_CMD: {
		return "CEC_IOCTL_SET_REGISTER_CMD";
		break;
	}
	default : {
		return "unknown";
		break;
	}
	}
}

static char *cec_rxstatus(int s)
{
	switch(s) {
	case CEC_MSG_SUCCESS : {
		return "success";
		break;
	}
	case CEC_MSG_FAIL_DATA_NOT_ACK : {
		return "data not ack";
		break;
	}
	case CEC_CSP_OFF_STATE : {
		return "CSP off";
		break;
	}
	case CEC_BAD_REQ_SERVICE : {
		return "bad req";
		break;
	}
	case CEC_MSG_FAIL_UNABLE_TO_ACCESS : {
		return "CEC line error";
		break;
	}
	case CEC_MSG_FAIL_ARBITRATION_ERROR : {
		return "arb error";
		break;
	}
	case CEC_MSG_FAIL_BIT_TIMMING_ERROR : {
		return "bit error";
		break;
	}
	case CEC_MSG_FAIL_DEST_NOT_ACK : {
		return "destination not ack";
		break;
	}
	default : {
		return "unknown";
		break;
	}
	}
}

static unsigned char get_next_logical_addr(cec_device_type device, unsigned char la)
{
	switch(device) {
	case CEC_DEVICE_TYPE_TV:
		switch(la) {
		case CEC_LOGICAL_ADDRESS_TV:
			return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		default:
			return CEC_LOGICAL_ADDRESS_TV;
		}
	case CEC_DEVICE_TYPE_REC_DEVICE:
		switch(la) {
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1:
			return CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2;
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2:
			return CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3;
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3:
			return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		default:
			return CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1;
		}
	case CEC_DEVICE_TYPE_TUNER:
		switch(la) {
		case CEC_LOGICAL_ADDRESS_TUNER_1:
			return CEC_LOGICAL_ADDRESS_TUNER_2;
		case CEC_LOGICAL_ADDRESS_TUNER_2:
			return CEC_LOGICAL_ADDRESS_TUNER_3;
		case CEC_LOGICAL_ADDRESS_TUNER_3:
			return CEC_LOGICAL_ADDRESS_TUNER_4;
		case CEC_LOGICAL_ADDRESS_TUNER_4:
			return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		default:
			return CEC_LOGICAL_ADDRESS_TUNER_1;
		}
	case CEC_DEVICE_TYPE_PLAYBACK_DEVICE:
		switch(la) {
		case CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1:
			return CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2;
		case CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_2:
			return CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3;
		case CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_3:
			return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		default:
			return CEC_LOGICAL_ADDRESS_PLAYBACK_DEVICE_1;
		}
	case CEC_DEVICE_TYPE_AUDIO_DEVICE:
		switch(la) {
		case CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM:
			return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		default:
			return CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM;
		}
	default:
		return CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
	}
}

static int device_type(int type)
{
	printk(KERN_INFO "hdmicec declared as a ");
	switch(type) {
	case CEC_DEVICE_TYPE_TV:
		printk("TV");
		break;
	case CEC_DEVICE_TYPE_REC_DEVICE:
		printk("record");
		break;
	case CEC_DEVICE_TYPE_TUNER:
		printk("tuner");
		break;
	case CEC_DEVICE_TYPE_PLAYBACK_DEVICE:
		printk("playback");
		break;
	case CEC_DEVICE_TYPE_AUDIO_DEVICE:
		printk("audio");
		break;
	default:
		printk("default (playback)");
		type = CEC_DEVICE_TYPE_PLAYBACK_DEVICE;
		break;
	}
	printk(" device type\n");
	return type;
}

/*
 *
 * PROCESSING
 * ----------
 * LEVEL 2
 *
 * */

/*
 *  CEC Power On
 * */
static void cec_on(cec_instance *this)
{
	int err;
	struct task_struct *tsk = current;

	disable_irq(gpio_to_irq(TDA_IRQ_CALIB));

	this->cec.power = power_on;
	TRY(dl_hdmi_cec_set_power_state(this->cec.inst, this->cec.power));

	/* turn GPIO into calib pulse generator */
	/* output (1 means try-state or high) */
	gpio_direction_output(TDA_IRQ_CALIB, 0);
	__gpio_set_value(TDA_IRQ_CALIB, 1);
	this->cec.clock = DL_HDMICEC_CLOCK_FRO;
	TRY(dl_hdmi_cec_enable_calibration(this->cec.inst, this->cec.clock));
	msleep(10);
	set_current_state(TASK_UNINTERRUPTIBLE);

	/* CAUTION : TDA needs a real 10ms pulse */
	cpu_relax();
	spin_lock_irq(&tsk->sighand->siglock);
	__gpio_set_value(TDA_IRQ_CALIB, 0);
	__udelay(10000);
	__gpio_set_value(TDA_IRQ_CALIB, 1);
	spin_unlock_irq(&tsk->sighand->siglock);

	msleep(10);
	TRY(dl_hdmi_cec_disable_calibration(this->cec.inst));

	/* setup */
	TRY(dl_hdmi_cec_get_instance_setup(this->cec.inst, &this->cec.setup));
	this->cec.setup.device_logical_address = this->cec.rx_addr;
	this->cec.clock = DL_HDMICEC_CLOCK_FRO;
	this->cec.setup.cec_clock_source = this->cec.clock;
	TRY(dl_hdmi_cec_instance_setup(this->cec.inst, &this->cec.setup));

	/* turn GPIO into IRQ */
	gpio_direction_input(TDA_IRQ_CALIB);
	enable_irq(gpio_to_irq(TDA_IRQ_CALIB));

	LOG(KERN_INFO, "standby --> on\n");

TRY_DONE:
	(void)0;
}

/*
 *  CEC Power Off
 * */
static void cec_standby(cec_instance *this)
{
	int err;

	this->cec.power = power_standby;
	TRY(dl_hdmi_cec_set_power_state(this->cec.inst, this->cec.power));

	LOG(KERN_INFO, "on --> standby\n");

TRY_DONE:
	(void)0;
}

/*
 *  CEC interrupt polling
 * */
static void cec_interrupt(struct work_struct *dummy)
{
	cec_instance *this = &our_instance;
	unsigned short new_phy_addr = edid_phy_addr();
	int err = 0;

	LOG(KERN_INFO, "%s called\n", __func__);

	/* switch on/off CEC */
	if(!get_hpd_status() &&                     \
	   (this->cec.power == power_on)) {
		this->cec.source_status = CEC_POWER_STATUS_STANDBY;
		/*       TRY(dlHdmiCecInactiveSource(this->cec.inst,             \ */
		/*                                        this->cec.initiator,     \ */
		/*                                        this->cec.phy_addr)); */
		cec_standby(this);
	} else if(get_hpd_status() &&                         \
	          (this->cec.power == power_standby)) {
		/* send active msg when hdmi has been abled */
		cec_on(this);
	}
	/* new phy addr means new EDID, mean HPD ! */
	else if((this->cec.phy_addr != new_phy_addr) &&        \
	        (this->cec.source_status == CEC_POWER_STATUS_ON)) {
		LOG(KERN_INFO, "new physical address %02x\n", new_phy_addr);
		this->cec.phy_addr = new_phy_addr;
		if(this->cec.phy_addr != 0xFFFF) {
			this->cec.rx_addr = get_next_logical_addr(this->cec.device_type, CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST);
			TRY(dl_hdmi_cec_polling_message(this->cec.inst, this->cec.rx_addr));
		} else {
			this->cec.rx_addr = CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;
		}
	}
#ifdef GUI_OVER_HDMI
	else if(edid_received()) {  /* Check me */
		if(this->cec.source_status == CEC_POWER_STATUS_STANDBY) {
			/* only for GFX on HDMI, do not use if only video playback on HDMI */
			TRY(dl_hdmi_cec_image_view_on(this->cec.inst, this->cec.initiator));
			TRY(dl_hdmi_cec_handle_interrupt(this->cec.inst));
			msleep(200);
			TRY(dl_hdmi_cec_active_source(this->cec.inst, this->cec.phy_addr));
			this->cec.source_status = CEC_POWER_STATUS_ON;
		}
	}
#endif

#if 0
	if(this->cec.phy_addr != 0xFFFF) {

		/* claim source status */
		if((get_hdmi_status() ==  power_standby) &&             \
		   (this->cec.source_status == CEC_POWER_STATUS_ON)) {
			/* send inactive msg when hdmi has been disabled */
			this->cec.source_status = CEC_POWER_STATUS_STANDBY;
			TRY(dl_hdmi_cec_inactive_source(this->cec.inst,          \
			                                this->cec.initiator,     \
			                                this->cec.phy_addr));
		} else if((get_hdmi_status() ==  power_on) &&                     \
		          (this->cec.source_status == CEC_POWER_STATUS_STANDBY)) {
			/* send active msg when hdmi has been abled */
			this->cec.source_status = CEC_POWER_STATUS_ON;
			TRY(dl_hdmi_cec_active_source(this->cec.inst,        \
			                              this->cec.phy_addr));
		}
		/*       printk(KERN_INFO "DBG phd_status:%s cec.power:%s\n", \ */
		/*              get_hpd_status()?"Active":"Inactive",                                      \ */
		/*              (this->cec.power==tmPowerOn)?"On":"Standby"); */
	}
#endif

	/* internal handeling */
	TRY(dl_hdmi_cec_handle_interrupt(this->cec.inst));

TRY_DONE:

	/* setup next tick */
	if(!this->driver.deinit_req) {
		/* setup next polling */
#ifndef IRQ
		/*       this->driver.timer.expires = jiffies + ( CHECK_EVERY_XX_MS * HZ / 1000 ); */
		/*       add_timer(&this->driver.timer); */
		mod_timer(&this->driver.timer, jiffies + (CHECK_EVERY_XX_MS * HZ / 1000));
#endif
	} else {
		this->driver.deinit_req++;
		wake_up_interruptible(&this->driver.wait);
	}
}

#ifndef IRQ
static DECLARE_WORK(wq_name, cec_interrupt);

void polling_timeout(unsigned long arg)
{

#if 0
	/* fake frame for equipement-less testing */

	cec_instance *this = &our_instance;

	if(this->driver.timer.data++ > 1000) {
		printk(KERN_INFO "fake rx message\n");
		this->driver.timer.data = 0;

		this->cec.frame.count = 4;
		this->cec.frame.addr = 4; /* 0-->4 (TV-->MediaPlayer1) */
		this->cec.frame.data[0] = 0x46; /* opcode: "GiveOsd" */
		this->cec.frame.service = CEC_RX_DONE;

		this->driver.poll_done = true;
		wake_up_interruptible(&this->driver.wait);
	}
#endif

	/* derefered because ATOMIC context of timer does not support I2C_transfert */
	schedule_work(&wq_name);

}
#endif

#ifndef IRQ
/*
 *  TDA irq
 * */
static irqreturn_t tda_irq(int irq, void *_udc)
{
	cec_instance *this = &our_instance;
	/*    printk(KERN_INFO "DBG caught irq:%d\n",irq); */

	/* do it now */
	mod_timer(&this->driver.timer, jiffies);

	return IRQ_HANDLED;
}
#endif

#ifdef TWL4030_HACK
/*
 *  User Control
 * */
static void user_control(int key, int press)
{
	input_report_key(gkp_input, key, press);
	input_sync(gkp_input);
	msleep(20);
	input_report_key(gkp_input, key, 0);
	input_sync(gkp_input);
}
#endif

/*
 *  CEC callback
 * */
static void event_callback_cec(dl_hdmi_cec_event_t event, unsigned char *data, unsigned char length)
{
	int err = 0;
	cec_instance *this = &our_instance;
	int opcode;
	int initiator, receiver;

	if(event == DL_HDMICEC_CALLBACK_MESSAGE_AVAILABLE) {

		this->cec.frame.count = length;
		this->cec.frame.addr = data[1]; /* .AddressByte */
		initiator = (this->cec.frame.addr >> 4) & 0x0F;
		this->cec.initiator = initiator;
		receiver = this->cec.frame.addr & 0x0F;
		/* .DataBytes[], length - siezof(length,addr,ack) */
		memcpy(&this->cec.frame.data, &data[2], length - 2);
		opcode = this->cec.frame.data[0];
		printk(KERN_INFO "hdmicec:rx:[%x--->%x] %s length:%d addr:%d %02x%02x%02x%02x\n", initiator, receiver, cec_opcode(opcode), \
		       length, data[1],
		       this->cec.frame.data[0],                                      \
		       this->cec.frame.data[1],                                      \
		       this->cec.frame.data[2],                                      \
		       this->cec.frame.data[3]);
		this->cec.frame.service = CEC_RX_DONE;

		msleep(20);

		/* automatic answering */
		switch(opcode) {
		case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
			TRY(dl_hdmi_cec_report_physical_address(this->cec.inst,           \
			                                        this->cec.phy_addr,       \
			                                        this->cec.device_type));
			break;
		case CEC_OPCODE_GET_CEC_VERSION:
			TRY(dl_hdmi_cec_version(this->cec.inst,                         \
			                        this->cec.initiator,                    \
			                        this->cec.version));
			break;
		case CEC_OPCODE_GIVE_OSD_NAME:
			TRY(dl_hdmi_cec_set_osd_name(this->cec.inst,         \
			                             this->cec.initiator,        \
			                             this->cec.osd_name.data,         \
			                             this->cec.osd_name.length));
			break;
		case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:
			TRY(dl_hdmi_cec_device_vendor_id(this->cec.inst,              \
			                                 this->cec.vendor_id));
			break;
		case CEC_OPCODE_REQUEST_ACTIVE_SOURCE:
			if(this->cec.source_status == CEC_POWER_STATUS_ON) {
				if(this->cec.initiator != 0x0F) {
					if(receiver == 0x0F) {
						TRY(dl_hdmi_cec_active_source(this->cec.inst, this->cec.phy_addr));
					}
				}
			}
			break;
		case CEC_OPCODE_ACTIVE_SOURCE:
			if(this->cec.source_status == CEC_POWER_STATUS_ON) {
				this->cec.source_status = CEC_POWER_STATUS_STANDBY;
				hdmi_disable(1);
				this->cec.power = power_on;
				/* keeps CEC alive */
				TRY(dl_hdmi_cec_set_power_state(this->cec.inst, this->cec.power));
			}
			break;
		case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:
			TRY(dl_hdmi_cec_report_power_status(this->cec.inst,               \
			                                    this->cec.initiator,          \
			                                    this->cec.source_status));
			break;
		case CEC_OPCODE_STANDBY:
			/* mind recording device can only be stopped by appli */
			if(this->cec.device_type != CEC_DEVICE_TYPE_REC_DEVICE) {
				this->cec.source_status = CEC_POWER_STATUS_STANDBY;
				hdmi_disable(1);
				this->cec.power = power_on;
				/* keeps CEC alive */
				TRY(dl_hdmi_cec_set_power_state(this->cec.inst, this->cec.power));
			}
			break;
		case CEC_OPCODE_ROUTING_INFORMATION:
		case CEC_OPCODE_SET_STREAM_PATH:
			/* wake-up if called */
			if(this->cec.phy_addr == (((int)this->cec.frame.data[1] << 8) + this->cec.frame.data[2])) {
				if(this->cec.source_status != CEC_POWER_STATUS_ON) {
					this->cec.source_status = CEC_POWER_STATUS_ON;
					hdmi_enable();
				}
				TRY(dl_hdmi_cec_active_source(this->cec.inst, this->cec.phy_addr));
			}
			break;
			/*       case /\* NEW DECK ??? *\/ */
		case CEC_OPCODE_ROUTING_CHANGE:
			/* wake-up if called */
			if(this->cec.phy_addr == (((int)this->cec.frame.data[3] << 8) + this->cec.frame.data[4])) {
				if(this->cec.source_status != CEC_POWER_STATUS_ON) {
					this->cec.source_status = CEC_POWER_STATUS_ON;
					hdmi_enable();
				}
				TRY(dl_hdmi_cec_active_source(this->cec.inst, this->cec.phy_addr));
			}
			break;
		case CEC_OPCODE_ABORT_MESSAGE:
			if(this->cec.phy_addr == (((int)this->cec.frame.data[3] << 8) + this->cec.frame.data[4])) {
				TRY(dl_hdmi_cec_feature_abort(this->cec.inst, \
				                              this->cec.initiator, \
				                              this->cec.feature_abort.feature_opcode, \
				                              this->cec.feature_abort.abort_reason));
			}
			break;
		case CEC_OPCODE_MENU_REQUEST:
#ifdef TWL4030_HACK
			this->cec.menu_status = CEC_MENU_STATE_ACTIVATE;
			TRY(dl_hdmi_cec_menu_status(this->cec.inst, \
			                            this->cec.initiator, \
			                            this->cec.menu_status));
			break;
#endif
		case CEC_OPCODE_USER_CONTROL_PRESSED:
			switch(this->cec.frame.data[1]) {
#ifdef TWL4030_HACK /* AL : hack to bypass keypad */
			case CEC_REMOTE_BUTTON_SELECT:
				user_control(353, 64);
				break;
			case CEC_REMOTE_BUTTON_UP:
				user_control(103, 128);
				break;
			case CEC_REMOTE_BUTTON_DOWN:
				user_control(108, 128);
				break;
			case CEC_REMOTE_BUTTON_LEFT:
				user_control(105, 128);
				break;
			case CEC_REMOTE_BUTTON_RIGHT:
				user_control(106, 128);
				break;
			case CEC_REMOTE_BUTTON_EXIT:
				user_control(14, 8);
				break;
#endif
			case CEC_REMOTE_BUTTON_POWER:
				this->cec.source_status = CEC_POWER_STATUS_ON;
				hdmi_enable();
				break;
			default:
				this->cec.feature_abort.feature_opcode = opcode;
				this->cec.feature_abort.abort_reason = CEC_ABORT_INVALID_OPERAND;
				TRY(dl_hdmi_cec_feature_abort(this->cec.inst, \
				                              this->cec.initiator, \
				                              this->cec.feature_abort.feature_opcode, \
				                              this->cec.feature_abort.abort_reason));
				break;
			}
			break;
#ifdef TWL4030_HACK
		case CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
			user_control(59, 8);
#endif
			break;
		case CEC_OPCODE_FEATURE_ABORT:
			/* stop any state machine transition */
			break;
		case CEC_OPCODE_VENDOR_COMMAND:
		case CEC_OPCODE_DEVICE_VENDOR_ID:
			/* hopefully will be handle in userspace */
			break;
		default:
			if(receiver != 0x0F) {
				this->cec.feature_abort.feature_opcode = opcode;
				this->cec.feature_abort.abort_reason = CEC_ABORT_UNKNOWN_OPCODE;
				TRY(dl_hdmi_cec_feature_abort(this->cec.inst,                 \
				                              this->cec.initiator,            \
				                              this->cec.feature_abort.feature_opcode, \
				                              this->cec.feature_abort.abort_reason));
			}
			break;
		}
		this->driver.poll_done = true;
		wake_up_interruptible(&this->driver.wait);
	} else if(event == DL_HDMICEC_CALLBACK_STATUS) {

		this->cec.frame.count = length;
		this->cec.frame.addr = data[1]; /* .AddressByte */
		initiator = (this->cec.frame.addr >> 4) & 0x0F;
		receiver = this->cec.frame.addr & 0x0F;
		/* .DataBytes[], length - siezof(length,addr)  */
		memcpy(&this->cec.frame.data, &data[2], length - 2);
		opcode = this->cec.frame.data[0];
		this->cec.frame.service = CEC_TX_DONE;

		if(length == POLLING_LENGTH) {
			if(opcode == CEC_MSG_FAIL_DEST_NOT_ACK) {
				/* no echo means it's mine ! */
				TRY(dl_hdmi_cec_set_logical_address(this->cec.inst, this->cec.rx_addr));
				TRY(dl_hdmi_cec_report_physical_address(this->cec.inst,        \
				                                        this->cec.phy_addr,    \
				                                        this->cec.device_type));
				/* DEVICE VENDOR ID sending after logicial address allocation according to spec 1.4 */
				TRY(dl_hdmi_cec_device_vendor_id(this->cec.inst, this->cec.vendor_id));
			} else if(opcode == CEC_MSG_SUCCESS) {
				/* try next one */
				this->cec.rx_addr = get_next_logical_addr(this->cec.device_type, this->cec.rx_addr);
				if(this->cec.rx_addr != CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST) {
					TRY(dl_hdmi_cec_polling_message(this->cec.inst, this->cec.rx_addr));
				} else {
					/* no more room, keep and claim unregistred */
					TRY(dl_hdmi_cec_set_logical_address(this->cec.inst, this->cec.rx_addr));
					TRY(dl_hdmi_cec_report_physical_address(this->cec.inst,     \
					                                        this->cec.phy_addr, \
					                                        this->cec.device_type));
				}
			} else {
				printk(KERN_INFO "ACK [%x--->%x] %s\n", initiator, receiver, cec_rxstatus(opcode));
			}
		} else {
			if(CEC_MSG_SUCCESS != opcode) {
				printk(KERN_INFO "ACK [%x--->%x] %s\n", initiator, receiver, cec_rxstatus(opcode));
			}
		}

		this->driver.poll_done = true;
		wake_up_interruptible(&this->driver.wait);

	} else {
		LOG(KERN_ERR, "oups ! callback got invalid event %d !\n", event);
	}

TRY_DONE:
	(void)err;
}

/*
 *  DevLib CEC opening
 * */
static int hdmi_cec_init(cec_instance *this)
{
	int err = 0;

	/* Real opening */
	TRY(dl_hdmi_cec_open(&this->cec.inst));

	/*    this->cec.vendor_id = 0x006037;   /\* NXP (IEEE OUI) *\/ */
	/*    this->cec.vendor_id = 0x0000f0;   /\* Samsung *\/ */
	this->cec.vendor_id = 0x00e091;   /* LGE */

	/*    this->cec.version = CEC_VERSION_1_4; */
	this->cec.version = cec_version_1_3a;
	this->cec.osd_name.data[0] = 0x54; /* TDA19989 by default */
	this->cec.osd_name.data[1] = 0x44;
	this->cec.osd_name.data[2] = 0x41;
	this->cec.osd_name.data[3] = 0x31;
	this->cec.osd_name.data[4] = 0x39;
	this->cec.osd_name.data[5] = 0x39;
	this->cec.osd_name.data[6] = 0x38;
	this->cec.osd_name.data[7] = 0x39;
	this->cec.osd_name.length = 8;

	TRY(dl_hdmi_cec_register_callbacks(this->cec.inst, event_callback_cec));

	this->cec.phy_addr = param_addr;
	this->cec.device_type = device_type(param_device);

TRY_DONE:
	return err;
}

/*
 *
 * ENTRY POINTS
 * ------------
 * LEVEL 3
 *
 * -
 *
 * */

/*
 *  ioctl driver :: opening
 * */

static int this_cdev_open(struct inode *p_inode, struct file *p_file)
{
	cec_instance *this;
	int minor = iminor(p_inode);

	if(minor >= MAX_MINOR) {
		printk(KERN_ERR "hdmicec:%s:only one cec opening please\n", __func__);
		return -EINVAL;
	}

	if((p_file->private_data != NULL) && (p_file->private_data != &our_instance)) {
		printk(KERN_ERR "hdmicec:%s:p_file missmatch\n", __func__);
	}
	this = p_file->private_data = &our_instance;
	down(&this->driver.sem);

	LOG(KERN_INFO, "major:%d minor:%d user:%d\n", imajor(p_inode), iminor(p_inode), this->driver.user_counter);

	if((this->driver.user_counter++) && (this->driver.minor == minor)) {
		/* init already done */
		up(&this->driver.sem);
		return 0;
	}
	this->driver.minor = minor;

	up(&this->driver.sem);
	return 0;
}

/*
 *  ioctl driver :: ioctl
 * */
static int this_cdev_ioctl(struct inode *p_inode, struct file *p_file, unsigned int cmd, unsigned long arg)
{
	cec_instance *this = p_file->private_data;
	int err = 0;

	LOG(KERN_INFO, ":%s\n", cec_ioctl(_IOC_NR(cmd)));

	BUG_ON(this->driver.minor != iminor(p_inode));
	if(_IOC_TYPE(cmd) != CEC_IOCTL_BASE) {
		printk(KERN_INFO "hdmicec:%s:unknown ioctl type: %x\n", __func__, _IOC_TYPE(cmd));
		return -ENOIOCTLCMD;
	}

	if(_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd)) || !arg;
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd)) || !arg;
	if(err) {
		printk(KERN_ERR "hdmicec:%s:argument access denied (check address vs value)\n", __func__);
		printk(KERN_ERR "_IOC_DIR:%d arg:%lx\n", _IOC_DIR(cmd), arg);
		return -EFAULT;
	}

	down(&this->driver.sem);

	/* Check DevLib consistancy here */

	switch(_IOC_NR(cmd)) {
	case CEC_VERBOSE_ON_CMD: {
		printk(KERN_INFO "verbose on\n");
		this->param.verbose = 1;
		break;
	}

	case CEC_VERBOSE_OFF_CMD: {
		printk(KERN_INFO "verbose off\n");
		this->param.verbose = 0;
		break;
	}

	case CEC_BYEBYE_CMD: {
		LOG(KERN_INFO, "callback release request\n");
		this->cec.frame.service = CEC_RELEASE;
		this->driver.poll_done = true;
		wake_up_interruptible(&this->driver.wait);
		break;
	}

	/*
	 *	  no param
	 *	*/

	case CEC_IOCTL_DISABLE_CALIBRATION_CMD: {
		TRY(dl_hdmi_cec_disable_calibration(this->cec.inst));
		break;
	}

	case CEC_IOCTL_INSTANCE_CONFIG_CMD: {
		TRY(dl_hdmi_cec_instance_config(this->cec.inst));
		break;
	}

	case CEC_IOCTL_REQUEST_ACTIVE_SRC_CMD: {
		TRY(dl_hdmi_cec_request_active_source(this->cec.inst));
		break;
	}

	case CEC_IOCTL_ABORT_MSG_CMD: {
		TRY(dl_hdmi_cec_abort_message(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GET_MENU_LANGUAGE_CMD: {
		TRY(dl_hdmi_cec_get_menu_language(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_AUDIO_STATUS_CMD: {
		TRY(dl_hdmi_cec_give_audio_status(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_DEVICE_POWER_STATUS_CMD: {
		TRY(dl_hdmi_cec_give_device_power_status(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_DEVICE_VENDOR_ID_CMD: {
		TRY(dl_hdmi_cec_give_device_vendor_id(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_OSD_NAME_CMD: {
		TRY(dl_hdmi_cec_give_osd_name(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_PHY_ADDR_CMD: {
		TRY(dl_hdmi_cec_give_physical_address(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_GIVE_SYS_AUDIO_MODE_STATUS_CMD: {
		TRY(dl_hdmi_cec_give_system_audio_mode_status(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_IMAGE_VIEW_ON_CMD: {
		TRY(dl_hdmi_cec_image_view_on(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_POLLING_MSG_CMD: {
		TRY(dl_hdmi_cec_polling_message(this->cec.inst, this->cec.rx_addr));
		break;
	}

	case CEC_IOCTL_REC_OFF_CMD: {
		TRY(dl_hdmi_cec_record_off(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_REC_ON_OWN_SRC_CMD: {
		TRY(dl_hdmi_cec_record_on_own_source(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_REC_TV_SCREEN_CMD: {
		TRY(dl_hdmi_cec_record_tv_screen(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_STANDBY_CMD: {
		TRY(dl_hdmi_cec_standby(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_TEXT_VIEW_ON_CMD: {
		TRY(dl_hdmi_cec_text_view_on(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_TUNER_STEP_DECREMENT_CMD: {
		TRY(dl_hdmi_cec_tuner_step_decrement(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_TUNER_STEP_INCREMENT_CMD: {
		TRY(dl_hdmi_cec_tuner_step_increment(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_USER_CTRL_RELEASED_CMD: {
		TRY(dl_hdmi_cec_user_control_released(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_VENDOR_REMOTE_BUTTON_UP_CMD: {
		TRY(dl_hdmi_cec_vendor_remote_button_up(this->cec.inst, this->cec.initiator));
		break;
	}

	case CEC_IOCTL_ROUTING_INFORMATION_CMD: {
		TRY(dl_hdmi_cec_routing_information(this->cec.inst, this->cec.phy_addr));
		break;
	}

	case CEC_IOCTL_SET_STREAM_PATH_CMD: {
		TRY(dl_hdmi_cec_set_stream_path(this->cec.inst, this->cec.phy_addr));
		break;
	}

	case CEC_IOCTL_ACTIVE_SRC_CMD: {
		/* NEW first do a <image view on> */
		/* NEW when switch by DSS and was inactive */
		TRY(dl_hdmi_cec_active_source(this->cec.inst, this->cec.phy_addr));
		break;
	}

	case CEC_IOCTL_SYS_AUDIO_MODE_REQUEST_CMD: {
		TRY(dl_hdmi_cec_system_audio_mode_request(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.phy_addr));
		break;
	}

	/*
	 *	  1 param
	 *	*/

	case CEC_IOCTL_RX_ADDR_CMD: {
		/* 	    BUG_ON(copy_from_user(&this->cec.rx_addr,(unsigned char*)arg,sizeof(unsigned char)) != 0); */
		this->cec.rx_addr = arg;
		TRY(dl_hdmi_cec_set_logical_address(this->cec.inst, this->cec.rx_addr));
		break;
	}

	case CEC_IOCTL_PHY_ADDR_CMD: {
		BUG_ON(copy_from_user(&this->cec.phy_addr, (unsigned short *)arg, sizeof(unsigned short)) != 0);
		break;
	}

	case CEC_IOCTL_GET_CEC_VERSION_CMD: {
		BUG_ON(copy_from_user(&this->cec.version, (cec_version *)arg, sizeof(cec_version)) != 0);
		TRY(dl_hdmi_cec_get_cec_version(this->cec.inst, this->cec.version));
		break;
	}

	case CEC_IOCTL_GET_SW_VERSION_CMD: {
		TRY(dl_hdmi_cec_get_swversion(&this->cec.sw_version));
		BUG_ON(copy_to_user((cec_sw_version *)arg, &this->cec.sw_version, sizeof(cec_sw_version)) != 0);
		break;
	}

	case CEC_IOCTL_SET_POWER_STATE_CMD: {
		/* NEW : log : please use DSS */
		BUG_ON(copy_from_user(&this->cec.power, (cec_power *)arg, sizeof(cec_power)) != 0);
		TRY(dl_hdmi_cec_set_power_state(this->cec.inst, this->cec.power));
		break;
	}

	case CEC_IOCTL_GET_POWER_STATE_CMD: {
		TRY(dl_hdmi_cec_get_power_state(this->cec.inst, &this->cec.power));
		BUG_ON(copy_to_user((cec_power *)arg, &this->cec.power, sizeof(cec_power)) != 0);
		break;
	}

	case CEC_IOCTL_INSTANCE_SETUP_CMD: {
		BUG_ON(copy_from_user(&this->cec.setup, (cec_setup *)arg, sizeof(cec_setup)) != 0);
		TRY(dl_hdmi_cec_instance_setup(this->cec.inst, &this->cec.setup));
		break;
	}

	case CEC_IOCTL_GET_INSTANCE_SETUP_CMD: {
		TRY(dl_hdmi_cec_get_instance_setup(this->cec.inst, &this->cec.setup));
		BUG_ON(copy_to_user((cec_setup *)arg, &this->cec.setup, sizeof(cec_setup)) != 0);
		break;
	}

	/*
	 *	  case CEC_IOCTL_ENABLE_EVENT_CMD:
	 *	  {
	 *	  BUG_ON(copy_from_user(&this->cec.an_event,(cec_event*)arg,sizeof(cec_event)) != 0);
	 *	  TRY(dlHdmiCecEnableEvent(this->cec.inst,this->cec.an_event));
	 *	  break;
	 *	  }
	 *
	 *	  case CEC_IOCTL_DISABLE_EVENT_CMD:
	 *	  {
	 *	  BUG_ON(copy_from_user(&this->cec.an_event,(cec_event*)arg,sizeof(cec_event)) != 0);
	 *	  TRY(dlHdmiCecDisableEvent(this->cec.inst,this->cec.an_event));
	 *	  break;
	 *	  }
	 *	*/

	case CEC_IOCTL_SET_MENU_LANGUAGE_CMD: {
		BUG_ON(copy_from_user(&this->cec.clock, (cec_string *)arg, sizeof(cec_string)) != 0);
		TRY(dl_hdmi_cec_set_menu_language(this->cec.inst, this->cec.string.data));
		break;
	}

	case CEC_IOCTL_ENABLE_CALIBRATION_CMD: {
		BUG_ON(copy_from_user(&this->cec.clock, (cec_clock *)arg, sizeof(cec_clock)) != 0);
		TRY(dl_hdmi_cec_enable_calibration(this->cec.inst, this->cec.clock));
		break;
	}

	/*
	 *	  >1 param
	 *	*/

	case CEC_IOCTL_WAIT_FRAME_CMD: {
		this->cec.frame.service = CEC_WAITING;
		this->driver.poll_done = false;
		up(&this->driver.sem);
		if(wait_event_interruptible(this->driver.wait, this->driver.poll_done)) return -ERESTARTSYS;
		down(&this->driver.sem);
		BUG_ON(copy_to_user((cec_frame *)arg, &this->cec.frame, sizeof(cec_frame)) != 0);
		break;
	}

	case CEC_IOCTL_VERSION_CMD: {
		BUG_ON(copy_from_user(&this->cec.version, (cec_version *)arg, sizeof(cec_version)) != 0);
		TRY(dl_hdmi_cec_version(this->cec.inst, \
		                        this->cec.initiator, \
		                        this->cec.version));
		break;
	}

	case CEC_IOCTL_CLEAR_ANALOGUE_TIMER_CMD: {
		BUG_ON(copy_from_user(&this->cec.analog_timer, (cec_analogue_timer *)arg, sizeof(cec_analogue_timer)) != 0);
		TRY(dl_hdmi_cec_clear_analogue_timer(this->cec.inst, \
		                                     this->cec.initiator, \
		                                     this->cec.analog_timer.day_of_month, \
		                                     this->cec.analog_timer.month_of_year, \
		                                     this->cec.analog_timer.start_time, \
		                                     &this->cec.analog_timer.duration, \
		                                     this->cec.analog_timer.recording_sequence, \
		                                     this->cec.analog_timer.analogue_broadcast_type, \
		                                     this->cec.analog_timer.analogue_frequency, \
		                                     this->cec.analog_timer.broadcast_system));
		break;
	}

	case CEC_IOCTL_CLEAR_DIGITAL_TIMER_CMD: {
		BUG_ON(copy_from_user(&this->cec.digital_timer, (cec_digital_timer *)arg, sizeof(cec_digital_timer)) != 0);
		TRY(dl_hdmi_cec_clear_digital_timer(this->cec.inst, \
		                                    this->cec.initiator, \
		                                    this->cec.digital_timer.day_of_month, \
		                                    this->cec.digital_timer.month_of_year, \
		                                    this->cec.digital_timer.start_time, \
		                                    &this->cec.digital_timer.duration, \
		                                    this->cec.digital_timer.recording_sequence, \
		                                    &this->cec.digital_timer.service_identification));
		break;
	}

	case CEC_IOCTL_CLEAR_EXT_TIMER_WITH_EXT_PLUG_CMD: {
		BUG_ON(copy_from_user(&this->cec.etwep, (cec_ext_timer_with_ext_plug *)arg, sizeof(cec_ext_timer_with_ext_plug)) != 0);
		TRY(dl_hdmi_cec_clear_external_timer_with_external_plug(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.etwep.day_of_month, \
		        this->cec.etwep.month_of_year, \
		        this->cec.etwep.start_time, \
		        &this->cec.etwep.duration, \
		        this->cec.etwep.recording_sequence, \
		        this->cec.etwep.external_plug));
		break;
	}

	case CEC_IOCTL_CLEAR_EXT_TIMER_WITH_PHY_ADDR_CMD: {
		BUG_ON(copy_from_user(&this->cec.etwpa, (cec_ext_timer_with_phy_addr *)arg, sizeof(cec_ext_timer_with_phy_addr)) != 0);
		TRY(dl_hdmi_cec_clear_external_timer_with_physical_address(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.etwpa.day_of_month, \
		        this->cec.etwpa.month_of_year, \
		        this->cec.etwpa.start_time, \
		        &this->cec.etwpa.duration, \
		        this->cec.etwpa.recording_sequence, \
		        this->cec.etwpa.external_physical_address));
		break;
	}

	case CEC_IOCTL_DECK_CTRL_CMD: {
		BUG_ON(copy_from_user(&this->cec.deck_ctrl, (cec_deck_ctrl *)arg, sizeof(cec_deck_ctrl)) != 0);
		TRY(dl_hdmi_cec_deck_control(this->cec.inst, \
		                             this->cec.initiator, \
		                             this->cec.deck_ctrl));
		break;
	}

	case CEC_IOCTL_DECK_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.deck_status, (cec_deck_status *)arg, sizeof(cec_deck_status)) != 0);
		TRY(dl_hdmi_cec_deck_status(this->cec.inst, \
		                            this->cec.initiator, \
		                            this->cec.deck_status));
		break;
	}

	case CEC_IOCTL_DEVICE_VENDOR_ID_CMD: {
		BUG_ON(copy_from_user(&this->cec.vendor_id, (unsigned long *)arg, sizeof(unsigned long)) != 0);
		TRY(dl_hdmi_cec_device_vendor_id(this->cec.inst, \
		                                 this->cec.vendor_id));
		break;
	}

	case CEC_IOCTL_FEATURE_ABORT_CMD: {
		BUG_ON(copy_from_user(&this->cec.feature_abort, (cec_feature_abort *)arg, sizeof(cec_feature_abort)) != 0);
		TRY(dl_hdmi_cec_feature_abort(this->cec.inst, \
		                              this->cec.initiator, \
		                              this->cec.feature_abort.feature_opcode, \
		                              this->cec.feature_abort.abort_reason));
		break;
	}

	case CEC_IOCTL_GIVE_DECK_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.satus_request, (cec_status_request *)arg, sizeof(cec_status_request)) != 0);
		TRY(dl_hdmi_cec_give_deck_status(this->cec.inst, \
		                                 this->cec.initiator, \
		                                 this->cec.satus_request));
		break;
	}

	case CEC_IOCTL_GIVE_TUNER_DEVICE_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.satus_request, (cec_status_request *)arg, sizeof(cec_status_request *)) != 0);
		TRY(dl_hdmi_cec_give_tuner_device_status(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.satus_request));
		break;
	}

	case CEC_IOCTL_INACTIVE_SRC_CMD: {
		/* NEW first stand by video */
		/* NEW when hdmi_disable and was active */
		TRY(dl_hdmi_cec_inactive_source(this->cec.inst, \
		                                this->cec.initiator, \
		                                this->cec.phy_addr));
		break;
	}

	case CEC_IOCTL_MENU_REQUEST_CMD: {
		BUG_ON(copy_from_user(&this->cec.menu_request, (cec_menu_request *)arg, sizeof(cec_menu_request)) != 0);
		TRY(dl_hdmi_cec_menu_request(this->cec.inst, \
		                             this->cec.initiator, \
		                             this->cec.menu_request));
		break;
	}

	case CEC_IOCTL_MENU_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.menu_status, (cec_menu_status *)arg, sizeof(cec_menu_status)) != 0);
		TRY(dl_hdmi_cec_menu_status(this->cec.inst, \
		                            this->cec.initiator, \
		                            this->cec.menu_status));
		break;
	}

	case CEC_IOCTL_PLAY_CMD: {
		BUG_ON(copy_from_user(&this->cec.play, (cec_play *)arg, sizeof(cec_play)) != 0);
		TRY(dl_hdmi_cec_play(this->cec.inst, \
		                     this->cec.initiator, \
		                     this->cec.play));
		break;
	}

	case CEC_IOCTL_REC_ON_ANALOGUE_SERVICE_CMD: {
		BUG_ON(copy_from_user(&this->cec.analog_service, (cec_analogue_service *)arg, sizeof(cec_analogue_service)) != 0);
		TRY(dl_hdmi_cec_record_on_analogue_service(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.analog_service.analogue_broadcast_type, \
		        this->cec.analog_service.analogue_frequency, \
		        this->cec.analog_service.broadcast_system));
		break;
	}

	case CEC_IOCTL_REC_ON_DIGITAL_SERVICE_CMD: {
		BUG_ON(copy_from_user(&this->cec.digital_service, (cec_digital_service *)arg, sizeof(cec_digital_service)) != 0);
		TRY(dl_hdmi_cec_record_on_digital_service(this->cec.inst, \
		        this->cec.initiator, \
		        &this->cec.digital_service));
		break;
	}

	case CEC_IOCTL_REC_ON_EXT_PHY_ADDR_CMD: {
		TRY(dl_hdmi_cec_record_on_external_physical_address(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.phy_addr));
		break;
	}

	case CEC_IOCTL_REC_ON_EXT_PLUG_CMD: {
		BUG_ON(copy_from_user(&this->cec.ext_plug, (cec_ext_plug *)arg, sizeof(cec_ext_plug)) != 0);
		TRY(dl_hdmi_cec_record_on_external_plug(this->cec.inst, \
		                                        this->cec.initiator, \
		                                        this->cec.ext_plug));
		break;
	}

	case CEC_IOCTL_REC_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.rec_status, (cec_rec_status *)arg, sizeof(cec_rec_status)) != 0);
		TRY(dl_hdmi_cec_record_status(this->cec.inst, \
		                              this->cec.initiator, \
		                              this->cec.rec_status));
		break;
	}

	case CEC_IOCTL_REPORT_AUDIO_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.audio_status, (cec_audio_status *)arg, sizeof(cec_audio_status)) != 0);
		TRY(dl_hdmi_cec_report_audio_status(this->cec.inst, \
		                                    this->cec.initiator, \
		                                    &this->cec.audio_status));
		break;
	}

	case CEC_IOCTL_REPORT_PHY_ADDR_CMD: {
		BUG_ON(copy_from_user(&this->cec.device_type, (cec_device_type *)arg, sizeof(cec_device_type)) != 0);
		TRY(dl_hdmi_cec_report_physical_address(this->cec.inst, \
		                                        this->cec.phy_addr, \
		                                        this->cec.device_type));
		break;
	}

	case CEC_IOCTL_REPORT_POWER_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.source_status, (cec_power_status *)arg, sizeof(cec_power_status)) != 0);
		TRY(dl_hdmi_cec_report_power_status(this->cec.inst, \
		                                    this->cec.initiator, \
		                                    this->cec.source_status));
		break;
	}

	case CEC_IOCTL_SELECT_ANALOGUE_SERVICE_CMD: {
		BUG_ON(copy_from_user(&this->cec.analog_service, (cec_analogue_service *)arg, sizeof(cec_analogue_service)) != 0);
		TRY(dl_hdmi_cec_select_analogue_service(this->cec.inst, \
		                                        this->cec.initiator, \
		                                        this->cec.analog_service.analogue_broadcast_type, \
		                                        this->cec.analog_service.analogue_frequency, \
		                                        this->cec.analog_service.broadcast_system));
		break;
	}

	case CEC_IOCTL_SELECT_DIGITAL_SERVICE_CMD: {
		BUG_ON(copy_from_user(&this->cec.digital_service, (cec_digital_service *)arg, sizeof(cec_digital_service)) != 0);
		TRY(dl_hdmi_cec_select_digital_service(this->cec.inst, \
		                                       this->cec.initiator, \
		                                       &this->cec.digital_service));
		break;
	}

	case CEC_IOCTL_SET_ANALOGUE_TIMER_CMD: {
		BUG_ON(copy_from_user(&this->cec.analog_timer, (cec_analogue_timer *)arg, sizeof(cec_analogue_timer)) != 0);
		TRY(dl_hdmi_cec_set_analogue_timer(this->cec.inst, \
		                                   this->cec.initiator, \
		                                   this->cec.analog_timer.day_of_month, \
		                                   this->cec.analog_timer.month_of_year, \
		                                   this->cec.analog_timer.start_time, \
		                                   &this->cec.analog_timer.duration, \
		                                   this->cec.analog_timer.recording_sequence, \
		                                   this->cec.analog_timer.analogue_broadcast_type, \
		                                   this->cec.analog_timer.analogue_frequency, \
		                                   this->cec.analog_timer.broadcast_system));
		break;
	}

	case CEC_IOCTL_SET_AUDIO_RATE_CMD: {
		BUG_ON(copy_from_user(&this->cec.audio_rate, (cec_audio_rate *)arg, sizeof(cec_audio_rate)) != 0);
		TRY(dl_hdmi_cec_set_audio_rate(this->cec.inst, \
		                               this->cec.initiator, \
		                               this->cec.audio_rate));
		break;
	}

	case CEC_IOCTL_SET_DIGITAL_TIMER_CMD: {
		BUG_ON(copy_from_user(&this->cec.digital_timer, (cec_digital_timer *)arg, sizeof(cec_digital_timer)) != 0);
		TRY(dl_hdmi_cec_set_digital_timer(this->cec.inst, \
		                                  this->cec.initiator, \
		                                  this->cec.digital_timer.day_of_month, \
		                                  this->cec.digital_timer.month_of_year, \
		                                  this->cec.digital_timer.start_time, \
		                                  &this->cec.digital_timer.duration, \
		                                  this->cec.digital_timer.recording_sequence, \
		                                  &this->cec.digital_timer.service_identification));
		break;
	}

	case CEC_IOCTL_SET_EXT_TIMER_WITH_EXT_PLUG_CMD: {
		BUG_ON(copy_from_user(&this->cec.etwep, (cec_ext_timer_with_ext_plug *)arg, sizeof(cec_ext_timer_with_ext_plug)) != 0);
		TRY(dl_hdmi_cec_set_external_timer_with_external_plug(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.etwep.day_of_month, \
		        this->cec.etwep.month_of_year, \
		        this->cec.etwep.start_time, \
		        &this->cec.etwep.duration, \
		        this->cec.etwep.recording_sequence, \
		        this->cec.etwep.external_plug));
		break;
	}

	case CEC_IOCTL_SET_EXT_TIMER_WITH_PHY_ADDR_CMD: {
		BUG_ON(copy_from_user(&this->cec.etwpa, (cec_ext_timer_with_phy_addr *)arg, sizeof(cec_ext_timer_with_phy_addr)) != 0);
		TRY(dl_hdmi_cec_set_external_timer_with_physical_address(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.etwpa.day_of_month, \
		        this->cec.etwpa.month_of_year, \
		        this->cec.etwpa.start_time, \
		        &this->cec.etwpa.duration, \
		        this->cec.etwpa.recording_sequence, \
		        this->cec.etwpa.external_physical_address));
		break;
	}

	case CEC_IOCTL_SET_SYS_AUDIO_MODE_CMD: {
		BUG_ON(copy_from_user(&this->cec.sys_audio_status, (cec_sys_audio_status *)arg, sizeof(cec_sys_audio_status)) != 0);
		TRY(dl_hdmi_cec_set_system_audio_mode(this->cec.inst, \
		                                      this->cec.initiator, \
		                                      this->cec.sys_audio_status));
		break;
	}

	case CEC_IOCTL_SYS_AUDIO_MODE_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.sys_audio_status, (cec_sys_audio_status *)arg, sizeof(cec_sys_audio_status)) != 0);
		TRY(dl_hdmi_cec_system_audio_mode_status(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.sys_audio_status));
		break;
	}

	case CEC_IOCTL_TIMER_CLEARED_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.timer_cleared_status, (cec_timer_cleared_status *)arg, sizeof(cec_timer_cleared_status)) != 0);
		TRY(dl_hdmi_cec_timer_cleared_status(this->cec.inst, \
		                                     this->cec.initiator, \
		                                     this->cec.timer_cleared_status));
		break;
	}

	case CEC_IOCTL_TIMER_STATUS_CMD: {
		BUG_ON(copy_from_user(&this->cec.timer_status, (cec_timer_status *)arg, sizeof(cec_timer_status)) != 0);
		TRY(dl_hdmi_cec_timer_status(this->cec.inst, \
		                             this->cec.initiator, \
		                             &this->cec.timer_status));
		break;
	}

	case CEC_IOCTL_TUNER_DEVICE_STATUS_ANALOGUE_CMD: {
		BUG_ON(copy_from_user(&this->cec.tdsa, (cec_tuner_device_status_analogue *)arg, sizeof(cec_tuner_device_status_analogue)) != 0);
		TRY(dl_hdmi_cec_tuner_device_status_analogue(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.tdsa.recording_flag, \
		        this->cec.tdsa.tuner_display_info, \
		        this->cec.tdsa.analogue_broadcast_type, \
		        this->cec.tdsa.analogue_frequency, \
		        this->cec.tdsa.broadcast_system));
		break;
	}

	case CEC_IOCTL_TUNER_DEVICE_STATUS_DIGITAL_CMD: {
		BUG_ON(copy_from_user(&this->cec.tdsd, (cec_tuner_device_status_digital *)arg, sizeof(cec_tuner_device_status_digital)) != 0);
		TRY(dl_hdmi_cec_tuner_device_status_digital(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.tdsd.recording_flag, \
		        this->cec.tdsd.tuner_display_info, \
		        &this->cec.tdsd.service_identification));
		break;
	}

	case CEC_IOCTL_USER_CTRL_CMD: {
		BUG_ON(copy_from_user(&this->cec.user_ctrl, (cec_user_ctrl *)arg, sizeof(cec_user_ctrl)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed(this->cec.inst, \
		                                     this->cec.initiator, \
		                                     this->cec.user_ctrl));
		break;
	}

	case CEC_IOCTL_USER_CTRL_PLAY_CMD: {
		BUG_ON(copy_from_user(&this->cec.play, (cec_play *)arg, sizeof(cec_play)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed_play(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.play));
		break;
	}

	case CEC_IOCTL_USER_CTRL_SELECT_AUDIOINPUT_CMD: {
		BUG_ON(copy_from_user(&this->cec.select, (unsigned char *)arg, sizeof(unsigned char)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed_select_audio_input(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.select));
		break;
	}

	case CEC_IOCTL_USER_CTRL_SELECT_AVINPUT_CMD: {
		BUG_ON(copy_from_user(&this->cec.select, (unsigned char *)arg, sizeof(unsigned char)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed_select_avinput(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.select));
		break;
	}

	case CEC_IOCTL_USER_CTRL_SELECT_MEDIA_CMD: {
		BUG_ON(copy_from_user(&this->cec.select, (unsigned char *)arg, sizeof(unsigned char)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed_select_media(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.select));
		break;
	}

	case CEC_IOCTL_USER_CTRL_TUNE_CMD: {
		BUG_ON(copy_from_user(&this->cec.user_ctrl_tune, (cec_user_ctrl_tune *)arg, sizeof(cec_user_ctrl_tune)) != 0);
		TRY(dl_hdmi_cec_user_control_pressed_tune(this->cec.inst, \
		        this->cec.initiator, \
		        &this->cec.user_ctrl_tune));
		break;
	}

	case CEC_IOCTL_SET_OSD_NAME_CMD: {
		BUG_ON(copy_from_user(&this->cec.osd_name, (cec_string *)arg, sizeof(cec_string)) != 0);
		TRY(dl_hdmi_cec_set_osd_name(this->cec.inst,      \
		                             this->cec.initiator, \
		                             this->cec.osd_name.data, \
		                             this->cec.osd_name.length));
		break;
	}

	case CEC_IOCTL_SET_OSD_STRING_CMD: {
		BUG_ON(copy_from_user(&this->cec.osd_string, (cec_osd_string *)arg, sizeof(cec_osd_string)) != 0);
		TRY(dl_hdmi_cec_set_osd_string(this->cec.inst, \
		                               this->cec.initiator, \
		                               this->cec.osd_string.display_control, \
		                               this->cec.osd_string.data, \
		                               this->cec.osd_string.length));
		break;
	}

	case CEC_IOCTL_SET_TIMER_PROGRAM_TITLE_CMD: {
		BUG_ON(copy_from_user(&this->cec.string, (cec_string *)arg, sizeof(cec_string)) != 0);
		TRY(dl_hdmi_cec_set_timer_program_title(this->cec.inst, \
		                                        this->cec.initiator, \
		                                        this->cec.string.data, \
		                                        this->cec.string.length));
		break;
	}

	case CEC_IOCTL_VENDOR_COMMAND_CMD: {
		BUG_ON(copy_from_user(&this->cec.string, (cec_string *)arg, sizeof(cec_string)) != 0);
		TRY(dl_hdmi_cec_vendor_command(this->cec.inst, \
		                               this->cec.initiator, \
		                               this->cec.string.data, \
		                               this->cec.string.length));
		break;
	}

	case CEC_IOCTL_VENDOR_REMOTE_BUTTON_DOWN_CMD: {
		BUG_ON(copy_from_user(&this->cec.string, (cec_string *)arg, sizeof(cec_string)) != 0);
		TRY(dl_hdmi_cec_vendor_remote_button_down(this->cec.inst, \
		        this->cec.initiator, \
		        this->cec.string.data, \
		        this->cec.string.length));
		break;
	}

	case CEC_IOCTL_VENDOR_COMMAND_WITH_ID_CMD: {
		BUG_ON(copy_from_user(&this->cec.vcwi, (cec_vendor_command_with_id *)arg, sizeof(cec_vendor_command_with_id)) != 0);
		TRY(dl_hdmi_cec_vendor_command_with_id(this->cec.inst,	\
		                                       this->cec.initiator,	\
		                                       this->cec.vcwi.vendor_id, \
		                                       this->cec.vcwi.cmd.data, \
		                                       this->cec.vcwi.cmd.length));
		break;
	}

	/*       case : */
	/* 	 { */
	/* 	    BUG_ON(copy_from_user(&this->cec.,(*)arg,sizeof()) != 0); */
	/* 	    TRY((this->cec.inst, \ */
	/* 		 this->cec.,	 \ */
	/* 		 &this->cec.)); */
	/* 	    break; */
	/* 	 } */

	default: {
		/* unrecognized ioctl */
		printk(KERN_INFO " unknown ioctl %x\n", cmd);
		up(&this->driver.sem);
		return -ENOIOCTLCMD;
	}
	}

TRY_DONE:
	up(&this->driver.sem);
	return err;
}

/*
 *  ioctl driver :: releasing
 * */
static int this_cdev_release(struct inode *p_inode, struct file *p_file)
{
	cec_instance *this = p_file->private_data;
	int minor = iminor(p_inode);

	LOG(KERN_INFO, "called\n");

	if(minor >= MAX_MINOR) {
		return -EINVAL;
	}

	BUG_ON(this->driver.minor != iminor(p_inode));
	down(&this->driver.sem);

	this->driver.user_counter--;
	if(this->driver.user_counter == 0) {
		p_file->private_data = NULL;
	} else {
		LOG(KERN_INFO, "still %d user pending\n", this->driver.user_counter);
	}

	up(&this->driver.sem);
	return 0;
}

/*
 *  I2C client :: creation
 * */
static int __devinit this_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	cec_instance *this = &our_instance;
	int err = 0;

	LOG(KERN_INFO, "called\n");

	/*
	 *	  I2C setup
	 *	*/
	if(this->driver.i2c_client) {
		dev_err(&this->driver.i2c_client->dev, "<%s> CEC device already created \n",
		        __func__);
		return -ENODEV;
	}

	this->driver.i2c_client = kmalloc(sizeof(struct i2c_client), GFP_KERNEL);
	if(!this->driver.i2c_client) {
		return -ENOMEM;
	}
	memset(this->driver.i2c_client, 0, sizeof(struct i2c_client));

	strncpy(this->driver.i2c_client->name, CEC_NAME, I2C_NAME_SIZE);
	this->driver.i2c_client->addr = TDA99XCEC_I2C_SLAVEADDRESS;
	this->driver.i2c_client->adapter = client->adapter;

	i2c_set_clientdata(client, this->driver.i2c_client);

	dl_hdmi_cec_get_swversion(&this->cec.sw_version);
	LOG(KERN_INFO, "HDMI CEC SW version:%ul.%ul compatibility:%ul\n", \
	    this->cec.sw_version.major_version_nr, \
	    this->cec.sw_version.minor_version_nr, \
	    this->cec.sw_version.compatibility_nr);

	/* I2C ok, then let's startup CEC */

	/* prepare event */
	this->driver.poll_done = true; /* currently idle */
	init_waitqueue_head(&this->driver.wait);
#ifndef IRQ
	init_timer(&this->driver.timer); /* do it before request_irq */
	this->driver.timer.function = polling_timeout;
	this->driver.timer.data = 0;
	this->driver.timer.expires = jiffies + HZ; /* start polling in one sec */
	add_timer(&this->driver.timer);
#else
	register_cec_interrupt((cec_callback_t)cec_interrupt);
#endif

#ifndef IRQ
	/* FRO calibration */
	err = gpio_request(TDA_IRQ_CALIB, "tda19989 calibration");
	if(err < 0) {
		printk(KERN_ERR "hdmicec:%s:cannot use GPIO 107\n", __func__);
		goto i2c_out;
	}
	/* turn GPIO into IRQ */
	gpio_direction_input(TDA_IRQ_CALIB);
	msleep(1);
	if(request_irq(gpio_to_irq(TDA_IRQ_CALIB), \
	               tda_irq, IRQF_TRIGGER_FALLING | IRQF_DISABLED, "TDA IRQ", NULL)) {
		printk(KERN_ERR "hdmicec:%s:cannot request irq, err:%d\n", __func__, err);
		gpio_free(TDA_IRQ_CALIB);
		goto i2c_out;
	}
#endif

	err = hdmi_cec_init(this);
	if(err) goto i2c_out;
	this->cec.rx_addr = CEC_LOGICAL_ADDRESS_UNREGISTRED_BROADCAST;

	if(get_hpd_status()) {
		cec_on(this);
		disable_irq(gpio_to_irq(TDA_IRQ_CALIB));
		cec_interrupt(NULL); /* initiate polling */
		enable_irq(gpio_to_irq(TDA_IRQ_CALIB));
	} else {
		cec_standby(this);
	}

	return 0;

i2c_out:
	LOG(KERN_INFO, "HDMICEC eject: this->driver.i2c_client removed\n");
	dl_hdmi_cec_close(this->cec.inst);
	kfree(this->driver.i2c_client);
	this->driver.i2c_client = NULL;

	return err;
}

/*
 *  I2C client :: destroy
 * */
static int this_i2c_remove(struct i2c_client *client)
{
	cec_instance *this = &our_instance;
	int err = 0;

	LOG(KERN_INFO, "called\n");

	err = dl_hdmi_cec_close(this->cec.inst);

	if(!client->adapter) {
		dev_err(&this->driver.i2c_client->dev, "<%s> no CEC device \n",
		        __func__);
		return -ENODEV;
	}
	kfree(this->driver.i2c_client);
	this->driver.i2c_client = NULL;

	return err;
}

/*
 *  I2C client driver (backend)
 *  -----------------
 * */
static struct i2c_driver this_i2c_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = CEC_NAME,
	},
	.probe = this_i2c_probe,
	 .remove = this_i2c_remove,
	  .id_table = this_i2c_id,
   };

/*
 *  ioctl driver (userland frontend)
 *  ------------
 * */
static struct file_operations this_cdev_fops = {
owner:
	THIS_MODULE,
open:
	this_cdev_open,
release:
	this_cdev_release,
ioctl:
	this_cdev_ioctl,
};

/*
 *  Module :: start up
 * */
static int __init cec_init(void)
{
	cec_instance *this = &our_instance;
	dev_t dev = 0;
	int err = 0;

	/*
	 *	   general device context
	 *	*/
	memset(this, 0, sizeof(cec_instance));
	this->param.verbose = param_verbose;
	this->param.major = param_major;
	this->param.minor = param_minor;

	/* Hello word */
	printk(KERN_INFO "%s(%s) %d.%d.%d compiled: %s %s %s\n", HDMICEC_NAME, TDA_NAME, TDA_VERSION_MAJOR,
	       TDA_VERSION_MINOR, TDA_VERSION_PATCHLEVEL, __DATE__, __TIME__, TDA_VERSION_EXTRA);
	if(this->param.verbose) LOG(KERN_INFO, ".verbose mode\n");

	/*
	 *	  plug I2C (backend : Hw interfacing)
	 *	*/
	err = i2c_add_driver(&this_i2c_driver);
	if(err < 0) {
		printk(KERN_ERR "driver registration failed\n");
		return -ENODEV;
	}

	if(this->driver.i2c_client == NULL) {
		printk(KERN_ERR "this->driver.i2c_client not allocated\n");
		err = -ENODEV;
		goto init_out;
	}

	/*
	 *	  cdev init (userland frontend)
	 *	*/

	/* arbitray range of device numbers */
	if(this->param.major) {
		/* user force major number @ insmod */
		dev = MKDEV(this->param.major, this->param.minor);
		err = register_chrdev_region(dev, MAX_MINOR, HDMICEC_NAME);
		if(err) {
			printk(KERN_ERR "unable to register %s, dev=%d %s\n", HDMICEC_NAME, dev, ERR_TO_STR(err));
			goto init_out;
		}
	} else {
		/* fully dynamic major number */
		err = alloc_chrdev_region(&dev, this->param.minor, MAX_MINOR, HDMICEC_NAME);
		if(err) {
			printk(KERN_ERR "unable to alloc chrdev region for %s, dev=%d %s\n", HDMICEC_NAME, dev, ERR_TO_STR(err));
			goto init_out;
		}
		this->param.major = MAJOR(dev);
	}

	cdev_init(this_cdev, &this_cdev_fops);
	this_cdev->owner = THIS_MODULE;

	this->driver.class = class_create(THIS_MODULE, HDMICEC_NAME);
	if(IS_ERR(this->driver.class)) {
		printk(KERN_INFO "error creating mmap device class.\n");
		err = -EIO;
		goto init_out;
	}
	this->driver.dev = device_create(this->driver.class, NULL, dev, NULL, HDMICEC_NAME);

	this->driver.devno = dev;
	err = cdev_add(this_cdev, this->driver.devno, MAX_MINOR);
	if(err) {
		printk(KERN_INFO "unable to add device for %s, ipp_driver.devno=%d %s\n", HDMICEC_NAME, this->driver.devno, ERR_TO_STR(err));
		device_destroy(this->driver.class, this->driver.devno);
		class_destroy(this->driver.class);
		unregister_chrdev_region(this->driver.devno, MAX_MINOR);
		goto init_out;
	}

#ifdef TWL4030_HACK
	/* AL : hack to bypass keypad */
	gkp_input = get_twm4030_input();
#endif

	/*
	 *	   general device context
	 *	*/
	init_MUTEX(&this->driver.sem);
	this->driver.deinit_req = 0;

#if 0
	/* TEST START */
	{
		struct device *dev, *devn;
		struct kset *dev_kset;
		int cpt = 0;

		dev_kset = this->driver.dev->kobj.kset;
		list_for_each_entry_safe_reverse(dev, devn, &dev_kset->list, kobj.entry) {
			cpt++;
			if((dev->bus_id[0] == 'h') \
			   && (dev->bus_id[1] == 'd') \
			   && (dev->bus_id[2] == 'm') \
			   && (dev->bus_id[3] == 'i')) {
				printk("DBG -%s-\n", dev->bus_id);
			}
		}
		printk("DBG get %d devices\n", cpt);
	}
	/* TEST STOP */
#endif

	return 0;

init_out:
	i2c_del_driver(&this_i2c_driver);
	return err;
}

/*
 *  Module :: shut down
 * */
static void __exit cec_exit(void)
{
	cec_instance *this = &our_instance;

	LOG(KERN_INFO, "called\n");

#ifndef IRQ
	free_irq(gpio_to_irq(TDA_IRQ_CALIB), NULL);
#endif

	unregister_cec_interrupt();
	this->driver.deinit_req = 1;
#ifndef IRQ
	if(wait_event_interruptible(this->driver.wait, this->driver.deinit_req > 1)) {
		/* oups... just wait... */
		msleep(CHECK_EVERY_XX_MS * 20);
	}
#endif

#ifndef IRQ
	/* release GPIO */
	gpio_free(TDA_IRQ_CALIB);
#endif

	/* unregister cdevice */
	cdev_del(this_cdev);
	unregister_chrdev_region(this->driver.devno, MAX_MINOR);

	/* unregister device */
	device_destroy(this->driver.class, this->driver.devno);
	class_destroy(this->driver.class);

	/* unregister i2c */
	i2c_del_driver(&this_i2c_driver);

}

/*
 *  Module
 *  ------
 * */
/* late_initcall(cec_init); */
module_init(cec_init);
module_exit(cec_exit);

/*
 *  Disclamer
 *  ---------
 * */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("andre lepine <andre.lepine@nxp.com>");
MODULE_DESCRIPTION(HDMICEC_NAME " driver");
