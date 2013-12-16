/* -------------------------------------------------------------------------- */
/* (C) Copyright 2000-2005              Koninklijke Philips Electronics N.V., */
/*     All rights reserved                                                    */
/*                                                                            */
/* This source code and any compilation or derivative thereof is the          */
/* proprietary information of Konlinklijke Philips Electronics N.V. and is    */
/* Confidential in nature.                                                    */
/* Under no circumstances is this software to be exposed to or placed under an*/
/* Open Source License of any type without the expressed written permission of*/
/* Koninklijke Philips Electronics N.V.                                       */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* MoReUse - 2005-10-24   Version 118                                         */
/*                                                                            */
/* Added:                                                                     */
/*    CID_AACPENC                                                             */
/*                                                                            */
/*                                                                            */
/* Changed:                                                                   */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* Removed:                                                                   */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* General Error Codes Added                                                  */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/* FILE NAME:    tmNxCompId.h                                                 */
/*                                                                            */
/* DESCRIPTION:  This header file identifies the standard component           */
/*               identifiers (CIDs) and interface identifiers (IID) for       */
/*               Nexperia platforms.                                          */
/*               The objective of these identifiers is to enable unique       */
/*               identification of software components and interfaces.        */
/*               In addition, standard status values are also defined to make */
/*               determination of typical error cases much easier.            */
/*                                                                            */
/*               Functional errors are not real errors in the sense of        */
/*               unexpected behaviour but are part of the normal communication*/
/*               between a client an a server component. They are linked to   */
/*               an interface, rather than to a component. All implementations*/
/*               of an interface must have the same behaviour with respect to */
/*               functional errors. Functional erros are all positive         */
/*               One global functional error is defined:  0 0x00000000    */
/*                                                                            */
/*               Non-functional errors (all negative numbers) indicate        */
/*               unexpected behaviour. They are linked to concrete component  */
/*               implementations                                              */
/*                                                                            */
/*               NOTE: The current implementation is different from the prev. */
/*                     component identifier implementation, based on classes, */
/*                     types and layers. However, the new system is backward  */
/*                     compatitible with the old implementation.              */
/*                                                                            */
/*               tmNxCompId.h defines a number of general error codes that can*/
/*               be used by all components. These error codes are concatenated*/
/*               to the CID or IID value in the local component headerfile of */
/*               the component that wants to (re-)use this general error code */
/*               General error codes can be used for both functional and      */
/*               non-functional errors. They should only be used if they      */
/*               semantically fully match (if not, defined a new component or */
/*               interface specific error code.                               */
/*                                                                            */
/* General Rules:                                                             */
/*               A return value has a length of 32 bits. At the binary level, */
/*               1 bit indicates the component or interface flag; 16 bits are */
/*               used for the actual component id (CID) or interface id (IID) */
/*               and 12 bits for the return status.                           */
/*                     The component/interface flag is bit 31.                */
/*                     Bits 30--28 are all 0.                                 */
/*                     The component/interface id occupies bits 27--12.       */
/*                     The return status occupies bits 11--0.                 */
/*                                                                            */
/*                     +--------+-----+-------+-----------+                   */
/*                     | flag:1 | 0:3 | id:16 | status:12 |                   */
/*                     +--------+-----+-------+-----------+                   */
/*                                                                            */
/*                     Format of interface ids:                               */
/*                                                                            */
/*                     +-----+-----+--------+-----------+                     */
/*                     | 0:1 | 0:3 | iid:16 | status:12 |                     */
/*                     +-----+-----+--------+-----------+                     */
/*                                                                            */
/*                     Format of component ids:                               */
/*                                                                            */
/*                     +-----+-----+--------+-----------+                     */
/*                     | 1:1 | 0:3 | cid:16 | status:12 |                     */
/*                     +-----+-----+--------+-----------+                     */
/*                                                                            */
/*               At the macro level, we use the prefix "CID_" for component   */
/*               ids (previous version "CID_COMP_") and "IID_" for interface  */
/*               ids.                                                         */
/*                                                                            */
/*               Each component id will be used by only one component; each   */
/*               component will have its own component id.                    */
/*               Each interface id will be used by only one interface; each   */
/*               interface will have its own interface id.                    */
/*                                                                            */
/*               In order to avoid problems when promoting a UNIQUE interface */
/*               to a SEPARATE interface, the ranges for CIDs and IIDS must   */
/*               not overlap.                                                 */
/*                                                                            */
/*               Component names and component ids have to be registered      */
/*               together; the same applies for interface names and ids.      */
/*                                                                            */
/*           NOTE about Compatibility                                         */
/*               In the previous implementation the first four bits were      */
/*               reserved for class, and there were separate fields for       */
/*               type and tag, like this:                                     */
/*                                                                            */
/*                     +---------+--------+-------+---------+-----------+     */
/*                     | class:4 | type:4 | tag:8 | layer:4 | status:12 |     */
/*                     +---------+--------+-------+---------+-----------+     */
/*                                                                            */
/*               The values 0 or 8 are not valid classes, and this fact       */
/*               can be used to distinguish a new-style IID (class == 0),     */
/*               a new-style CID (class == 8), and an old-style CID           */
/*               (otherwise).                                                 */
/*                                                                            */
/*           NOTE about error codes                                           */
/*               The general error codes use the range 0x001 to 0x7FF.        */
/*               The component specific error codes are defined in the        */
/*               local component header file and can use 0x800 to 0xFFF.      */
/*               0 has the value 0x00000000.                              */
/*               The proposed error code ranges (general and specific) are    */
/*               the same for functional and non-functional errors.           */
/*                                                                            */
/*               The previously defined ranges for external customers,        */
/*               assert errors and fatal errors have been dropped.            */
/*               The previously defined range for general errors started      */
/*               at 0x000 instead of 0x001                                    */
/*                                                                            */
/* DOCUMENT REF: Nexperia/MoReUse Naming Conventions                          */
/*                                                                            */
/* -------------------------------------------------------------------------- */

#ifndef TMNXCOMPID_H
#define TMNXCOMPID_H

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   Standard include files:                                                  */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#include "tmNxTypes.h"

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*    Types and defines:                                                      */
/*                                                                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*   0 is the 32 bit global status value used by all Nexperia components  */
/*   to indicate successful function/operation status.  If a non-zero value is*/
/*   returned as status, it should use the component ID formats defined.      */
/*                                                                            */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*     General Defines                                                        */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define CID_IID_FLAG_BITSHIFT    31
#define CID_ID_BITSHIFT          12
#define IID_ID_BITSHIFT          12

#define CID_FLAG                 (0x1U << CID_IID_FLAG_BITSHIFT)
#define IID_FLAG                 (0x0U << CID_IID_FLAG_BITSHIFT)

#define CID_ID(number)           ((number) << CID_ID_BITSHIFT)
#define CID_ID_BITMASK           (0x7FFFFU  << CID_ID_BITSHIFT)

#define IID_ID(number)           ((number) << IID_ID_BITSHIFT)
#define IID_ID_BITMASK           (0x7FFFFU  << IID_ID_BITSHIFT)

#define CID_ERR_BITMASK                 0xFFFU
#define CID_ERR_BITSHIFT                0
#define CID_GET_ERROR(compId)   ((compId & CID_ERR_BITMASK) >> CID_ERR_BITSHIFT)

#define CID_BSL_HDMITX              (CID_ID(0x8240U) | CID_FLAG)

#define ERR_COMPATIBILITY            0x001U /* SW Interface compatibility   */
#define ERR_MAJOR_VERSION            0x002U /* SW Major Version error       */
#define ERR_COMP_VERSION             0x003U /* SW component version error   */
#define ERR_BAD_MODULE_ID            0x004U /* SW - HW module ID error      */
#define ERR_BAD_UNIT_NUMBER          0x005U /* Invalid device unit number   */
#define ERR_BAD_INSTANCE             0x006U /* Bad input instance value     */
#define ERR_BAD_HANDLE               0x007U /* Bad input handle             */
#define ERR_BAD_INDEX                0x008U /* Bad input index              */
#define ERR_BAD_PARAMETER            0x009U /* Invalid input parameter      */
#define ERR_NO_INSTANCES             0x00AU /* No instances available       */
#define ERR_NO_COMPONENT             0x00BU /* Component is not present     */
#define ERR_NO_RESOURCES             0x00CU /* Resource is not available    */
#define ERR_INSTANCE_IN_USE          0x00DU /* Instance is already in use   */
#define ERR_RESOURCE_OWNED           0x00EU /* Resource is already in use   */
#define ERR_RESOURCE_NOT_OWNED       0x00FU /* Caller does not own resource */
#define ERR_INCONSISTENT_PARAMS      0x010U /* Inconsistent input params    */
#define ERR_NOT_INITIALIZED          0x011U /* Component is not initialized */
#define ERR_NOT_ENABLED              0x012U /* Component is not enabled     */
#define ERR_NOT_SUPPORTED            0x013U /* Function is not supported    */
#define ERR_INIT_FAILED              0x014U /* Initialization failed        */
#define ERR_BUSY                     0x015U /* Component is busy            */
#define ERR_NOT_BUSY                 0x016U /* Component is not busy        */
#define ERR_READ                     0x017U /* Read error                   */
#define ERR_WRITE                    0x018U /* Write error                  */
#define ERR_ERASE                    0x019U /* Erase error                  */
#define ERR_LOCK                     0x01AU /* Lock error                   */
#define ERR_UNLOCK                   0x01BU /* Unlock error                 */
#define ERR_OUT_OF_MEMORY            0x01CU /* Memory allocation failed     */
#define ERR_BAD_VIRT_ADDRESS         0x01DU /* Bad virtual address          */
#define ERR_BAD_PHYS_ADDRESS         0x01EU /* Bad physical address         */
#define ERR_TIMEOUT                  0x01FU /* Timeout error                */
#define ERR_OVERFLOW                 0x020U /* Data overflow/overrun error  */
#define ERR_FULL                     0x021U /* Queue (etc.) is full         */
#define ERR_EMPTY                    0x022U /* Queue (etc.) is empty        */
#define ERR_NOT_STARTED              0x023U /* Streaming function failed    */
#define ERR_ALREADY_STARTED          0x024U /* Start function failed        */
#define ERR_NOT_STOPPED              0x025U /* Non-streaming function failed*/
#define ERR_ALREADY_STOPPED          0x026U /* Stop function failed         */
#define ERR_ALREADY_SETUP            0x027U /* Setup function failed        */
#define ERR_NULL_PARAMETER           0x028U /* NULL input parameter         */
#define ERR_NULL_DATAINFUNC          0x029U /* NULL data input function     */
#define ERR_NULL_DATAOUTFUNC         0x02AU /* NULL data output function    */
#define ERR_NULL_CONTROLFUNC         0x02BU /* NULL control function        */
#define ERR_NULL_COMPLETIONFUNC      0x02CU /* NULL completion function     */
#define ERR_NULL_PROGRESSFUNC        0x02DU /* NULL progress function       */
#define ERR_NULL_ERRORFUNC           0x02EU /* NULL error handler function  */
#define ERR_NULL_MEMALLOCFUNC        0x02FU /* NULL memory alloc function   */
#define ERR_NULL_MEMFREEFUNC         0x030U /* NULL memory free  function   */
#define ERR_NULL_CONFIGFUNC          0x031U /* NULL configuration function  */
#define ERR_NULL_PARENT              0x032U /* NULL parent data             */
#define ERR_NULL_IODESC              0x033U /* NULL in/out descriptor       */
#define ERR_NULL_CTRLDESC            0x034U /* NULL control descriptor      */
#define ERR_UNSUPPORTED_DATACLASS    0x035U /* Unsupported data class       */
#define ERR_UNSUPPORTED_DATATYPE     0x036U /* Unsupported data type        */
#define ERR_UNSUPPORTED_DATASUBTYPE  0x037U /* Unsupported data subtype     */
#define ERR_FORMAT                   0x038U /* Invalid/unsupported format   */
#define ERR_INPUT_DESC_FLAGS         0x039U /* Bad input  descriptor flags  */
#define ERR_OUTPUT_DESC_FLAGS        0x03AU /* Bad output descriptor flags  */
#define ERR_CAP_REQUIRED             0x03BU /* Capabilities required ???    */
#define ERR_BAD_TMALFUNC_TABLE       0x03CU /* Bad TMAL function table      */
#define ERR_INVALID_CHANNEL_ID       0x03DU /* Invalid channel identifier   */
#define ERR_INVALID_COMMAND          0x03EU /* Invalid command/request      */
#define ERR_STREAM_MODE_CONFUSION    0x03FU /* Stream mode config conflict  */
#define ERR_UNDERRUN                 0x040U /* Data underflow/underrun      */
#define ERR_EMPTY_PACKET_RECVD       0x041U /* Empty data packet received   */
#define ERR_OTHER_DATAINOUT_ERR      0x042U /* Other data input/output err  */
#define ERR_STOP_REQUESTED           0x043U /* Stop in progress             */
#define ERR_ASSERTION                0x049U /* Assertion failure            */
#define ERR_HIGHWAY_BANDWIDTH        0x04AU /* Highway bandwidth bus error  */
#define ERR_HW_RESET_FAILED          0x04BU /* Hardware reset failed        */
#define ERR_BAD_FLAGS                0x04DU /* Bad flags                    */
#define ERR_BAD_PRIORITY             0x04EU /* Bad priority                 */
#define ERR_BAD_REFERENCE_COUNT      0x04FU /* Bad reference count          */
#define ERR_BAD_SETUP                0x050U /* Bad setup                    */
#define ERR_BAD_STACK_SIZE           0x051U /* Bad stack size               */
#define ERR_BAD_TEE                  0x052U /* Bad tee                      */
#define ERR_IN_PLACE                 0x053U /* In place                     */
#define ERR_NOT_CACHE_ALIGNED        0x054U /* Not cache aligned            */
#define ERR_NO_ROOT_TEE              0x055U /* No root tee                  */
#define ERR_NO_TEE_ALLOWED           0x056U /* No tee allowed               */
#define ERR_NO_TEE_EMPTY_PACKET      0x057U /* No tee empty packet          */
#define ERR_NULL_PACKET              0x059U /* NULL packet                  */
#define ERR_FORMAT_FREED             0x05AU /* Format freed                 */
#define ERR_FORMAT_INTERNAL          0x05BU /* Format internal              */
#define ERR_BAD_FORMAT               0x05CU /* Bad format                   */
#define ERR_FORMAT_NEGOTIATE_DATACLASS 0x05DU /* Format negotiate class     */
#define ERR_FORMAT_NEGOTIATE_DATATYPE 0x05EU /* Format negotiate type       */
#define ERR_FORMAT_NEGOTIATE_DATASUBTYPE 0x05FU /* Format negotiate subtype */
#define ERR_FORMAT_NEGOTIATE_DESCRIPTION 0x060U /* Format negotiate desc    */
#define ERR_NULL_FORMAT              0x061U /* NULL format                  */
#define ERR_FORMAT_REFERENCE_COUNT   0x062U /* Format reference count       */
#define ERR_FORMAT_NOT_UNIQUE        0x063U /* Format not unique            */
#define NEW_FORMAT                   0x064U /* New format (not an error)    */
#define ERR_FORMAT_NEGOTIATE_EXTENSION 0x065U /* Format negotiate extension */
#define ERR_INVALID_STATE            0x066U /* Invalid state for function   */
#define ERR_NULL_CONNECTION          0x067U /* No connection to this pin    */
#define ERR_OPERATION_NOT_PERMITTED  0x068U /* corresponds to posix EPERM   */
#define ERR_NOT_CLOCKED              0x069U /* Power down - clocked off     */


#endif /* TMNXCOMPID_H ----------------- */
