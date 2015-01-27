/**************************************************************************
 *
 * Copyright (c) 2009, 2010 Marvell International Ltd.
 *
 * This file is part of GNU program.
 *
 * GNU program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 2 of the License, or (at your
 * option) any later version.
 *
 * GNU program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 * If not, see http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 *************************************************************************/

/*
 * linux/arch/arm/mach-mmp/include/mach/regs-mpmu.h
 *
 *   Main Power Management Unit
 */

#ifndef __ASM_MACH_REGS_MPMU_H
#define __ASM_MACH_REGS_MPMU_H

#include <mach/addr-map.h>

#define MPMU_CPCR	MPMU_REG(0x0000)
#define MPMU_FCCR	MPMU_REG(0x0008)
#define MPMU_POCR	MPMU_REG(0x000c)
#define MPMU_POSR	MPMU_REG(0x0010)
#define MPMU_SUCCR	MPMU_REG(0x0014)
#define MPMU_VRCR	MPMU_REG(0x0018)
#define MPMU_OHCR	MPMU_REG(0x001c)
#define MPMU_GPCR	MPMU_REG(0x0030)
#define MPMU_PLL2CR	MPMU_REG(0x0034)
#define MPMU_SCCR	MPMU_REG(0x0038)
#define MPMU_CWUCRM	MPMU_REG(0x004c)
#define MPMU_PLL1_REG1	MPMU_REG(0x0050)
#define MPMU_PLL1_REG2	MPMU_REG(0x0054)
#define MPMU_PLL1_SSC	MPMU_REG(0x0058)
#define MPMU_PLL2_REG1	MPMU_REG(0x0060)
#define MPMU_PLL2_REG2	MPMU_REG(0x0064)
#define MPMU_PLL2_SSC	MPMU_REG(0x0068)
#define MPMU_TS		MPMU_REG(0x0080)
#define MPMU_WDTPCR	MPMU_REG(0x0200)
#define MPMU_APCR	MPMU_REG(0x1000)
#define MPMU_APSR	MPMU_REG(0x1004)
#define MPMU_APRR	MPMU_REG(0x1020)
#define MPMU_ACGR	MPMU_REG(0x1024)
#define MPMU_ARSR	MPMU_REG(0x1028)
#define MPMU_AWUCRS	MPMU_REG(0x1048)
#define MPMU_AWUCRM	MPMU_REG(0x104c)

#endif /* __ASM_MACH_REGS_APMU_H */
