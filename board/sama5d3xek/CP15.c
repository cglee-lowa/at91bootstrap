/*
 * @file CP15.c
 * @brief This file contains the CP15/MMU helpers functions definitions.
 *
 *  Created on: 19 juin 2013
 *      Author: eric from ATMEL source code (see below).
 */
/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//-----------------------------------------------------------------------------
// Reg Reads                    Writes
//----------------------------------------------------------------------------
// 0   ID code                  Unpredictable
// 0   cache type               Unpredictable
// 0   TCM status               Unpredictable
// 1   Control                  Control
// 2   Translation table base   Translation table base
// 3   Domain access control    Domain access control
// 4                                                       (Reserved)
// 5   Data fault status        Data fault status
// 5   Instruction fault status Instruction fault status
// 6   Fault address            Fault address
// 7   cache operations         cache operations
// 8   Unpredictable            TLB operations
// 9   cache lockdown           cache lockdown
// 9   TCM region               TCM region
// 10  TLB lockdown             TLB lockdown
// 11                                                      (Reserved)
// 12                                                      (Reserved)
// 13  FCSE PID                 FCSE PID
// 13  Context ID               Context ID
// 14                                                      (Reserved)
// 15  Test configuration       Test configuration
//-----------------------------------------------------------------------------


/** \page cp15_f CP15 Functions.
 *
 * \section CP15 function Usage
 *
 * Methods to manage the Coprocessor 15. Coprocessor 15, or System Control
 * Coprocessor CP15, is used to configure and control all the items in the
 * list below:
 * <ul>
 * <li> ARM core
 * <li> caches (Icache, Dcache and write buffer)
 * <li> TCM
 * <li> MMU
 * <li> Other system options
 * </ul>
 * \section Usage
 *
 * -# Enable or disable D cache with Enable_D_cache and Disable_D_cache
 * -# Enable or disable I cache with Enable_I_cache and Disable_I_cache
 *
 * Related files:\n
 * \ref cp15.h\n
 * \ref cp15.c.\n
 */

#include "CP15.h"
#include "debug.h"

// these functions are defined in cp15_asm_gcc.S
extern unsigned int CP15_ReadControl(void);
extern void         CP15_WriteControl(unsigned int value);


//**************************************************
unsigned int CP15_IsMMUEnabled(void)
{
    unsigned int control;

    control = CP15_ReadControl();
    return ((control & (1 << CP15_M_BIT)) != 0);
}
//**************************************************
void CP15_EnableMMU(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    // Check if MMU is disabled
    if ((control & (1 << CP15_M_BIT)) == 0) {

        control |= (1 << CP15_M_BIT);
        CP15_WriteControl(control);
        dbg_log(DEBUG_LOUD,"MMU enabled.\n");
    }
    else {

    	dbg_log(DEBUG_LOUD,"MMU is already enabled.\n");
    }
}
//**************************************************
void CP15_DisableMMU(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    // Check if MMU is enabled
    if ((control & (1 << CP15_M_BIT)) != 0) {

        control &= ~(1ul << CP15_M_BIT);
        control &= ~(1ul << CP15_C_BIT);
        CP15_WriteControl(control);
        dbg_log(DEBUG_LOUD,"MMU disabled.\n");
    }
    else {

    	dbg_log(DEBUG_LOUD,"MMU is already disabled.\n");
    }
}
//**************************************************
unsigned int CP15_IsIcacheEnabled(void)
{
    unsigned int control;

    control = CP15_ReadControl();
    return ((control & (1 << CP15_I_BIT)) != 0);
}

//********************************************************
void CP15_EnableIcache(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    // Check if cache is disabled
    if ((control & (1 << CP15_I_BIT)) == 0) {

        control |= (1 << CP15_I_BIT);
        CP15_WriteControl(control);
        dbg_log(DEBUG_LOUD,"I cache enabled.\n");
    }
#if !defined(OP_BOOTSTRAP_on)
    else {

    	dbg_log(DEBUG_LOUD,"I cache is already enabled.\n");
    }
#endif
}
//*********************************************************
void CP15_DisableIcache(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    // Check if cache is enabled
    if ((control & (1 << CP15_I_BIT)) != 0) {

        control &= ~(1ul << CP15_I_BIT);
        CP15_WriteControl(control);
        dbg_log(DEBUG_LOUD,"I cache disabled.\n");
    }
    else {

    	dbg_log(DEBUG_LOUD,"I cache is already disabled.\n");
    }
}
//**********************************************************
unsigned int CP15_IsDcacheEnabled(void)
{
    unsigned int control;

    control = CP15_ReadControl();
    return ((control & ((1 << CP15_C_BIT)||(1 << CP15_M_BIT))) != 0);
}
//*********************************************************
void CP15_EnableDcache(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    if( !CP15_IsMMUEnabled() ) {
    	dbg_log(DEBUG_LOUD, "Do nothing: MMU not enabled\n");
    }
    else {
        // Check if cache is disabled
        if ((control & (1 << CP15_C_BIT)) == 0) {

            control |= (1 << CP15_C_BIT);
            CP15_WriteControl(control);
            dbg_log(DEBUG_LOUD,"D cache enabled.\n");
        }
        else {

        	dbg_log(DEBUG_LOUD,"D cache is already enabled.\n");
        }
    }
}
//*********************************************************
void CP15_DisableDcache(void)
{
    unsigned int control;

    control = CP15_ReadControl();

    // Check if cache is enabled
    if ((control & (1 << CP15_C_BIT)) != 0) {

        control &= ~(1ul << CP15_C_BIT);
        CP15_WriteControl(control);
        dbg_log(DEBUG_LOUD,"D cache disabled.\n");
    }
    else {

    	dbg_log(DEBUG_LOUD,"D cache is already disabled.\n");
    }
}
//*********************************************************