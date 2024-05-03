/**
 * @file   bsp.c
 *
 * @brief  	Brief description of the content of bsp.c
 * @author 	mootseeker
 * @date   	May 3, 2024 - first implementation
 * @version 1.0.0
 * 
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright (c) 2024 mootseeker
 */
 
 // ********************************************************************************
 /**
 * @brief  Includes
 *
 * Section for module-specific include files
 * If all include files are inserted in main.h, only the file main.h must be included here
 */

#include "sst.h"
#include "bsp.h"
#include "blinky.h"

#include "main.h"  /* CMSIS-compliant header file for the MCU used */


/* test pins on GPIO PA */
#define LED_PIN   3U /* LED LD2-Green */
 

/* Assertion handler ======================================================*/
DBC_NORETURN
void DBC_fault_handler(char const * const module, int const label)
{
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)label;

    /* set PRIMASK to disable interrupts and stop SST right here */
    __asm volatile ("cpsid i");

#ifndef NDEBUG
    for (;;) { /* keep blinking LED2 */
        BSP_ledOn();  /* turn LED2 on */
        uint32_t volatile ctr;
        for (ctr = 10000U; ctr > 0U; --ctr) {
        }
        BSP_ledOff(); /* turn LED2 off */
        for (ctr = 10000U; ctr > 0U; --ctr) {
        }
    }
#endif
    NVIC_SystemReset();
}
/*..........................................................................*/
void assert_failed(char const * const module, int const label);/* prototype */
void assert_failed(char const * const module, int const label) {
    DBC_fault_handler(module, label);
}

/* SST task activations ====================================================*/
/* preprocessor switch to choose between regular and reserved IRQs */
#define REGULAR_IRQS

#ifdef REGULAR_IRQS
/* repurpose regular IRQs for SST Tasks */
/* prototypes */

#else /* use reserved IRQs for SST Tasks */
/* prototypes */
void Reserved14_IRQHandler(void);

void Reserved14_IRQHandler(void) { SST_Task_activate(AO_Blinky);  }
#endif

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* Configure the MPU to prevent NULL-pointer dereferencing
    * see: www.state-machine.com/null-pointer-protection-with-arm-cortex-m-mpu
    */
    MPU->RBAR = 0x0U                          /* base address (NULL) */
                | MPU_RBAR_VALID_Msk          /* valid region */
                | (MPU_RBAR_REGION_Msk & 7U); /* region #7 */
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     /* 2^(7+1) region */
                | (0x0U << MPU_RASR_AP_Pos)   /* no-access region */
                | MPU_RASR_ENABLE_Msk;        /* region enable */

    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       /* enable background region */
                | MPU_CTRL_ENABLE_Msk;        /* enable the MPU */
    __ISB();
    __DSB();

    /* assign IRQs to tasks. NOTE: critical for SST... */
#ifdef REGULAR_IRQS
    /* repurpose regular IRQs for SST Tasks */
    SST_Task_setIRQ(AO_Blinky,  PVD_PVM_IRQn);
#else
    /* use reserved IRQs for SST Tasks */
    SST_Task_setIRQ(AO_Blinky,  14U);
#endif

}

/*..........................................................................*/
void BSP_ledOn( void )
{
	HAL_GPIO_WritePin( LED_BLU_GPIO_Port, LED_BLU_Pin, GPIO_PIN_SET );
}

/* LED2 */
void BSP_ledOff( void )
{
	HAL_GPIO_WritePin( LED_BLU_GPIO_Port, LED_BLU_Pin, GPIO_PIN_RESET );
}

/* SST callbacks ===========================================================*/
void SST_onStart( void )
{
	SystemCoreClockUpdate( );

	/* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
	SysTick_Config( ( SystemCoreClock / BSP_TICKS_PER_SEC ) + 1U );

	/* set priorities of ISRs used in the system */
	NVIC_SetPriority( SysTick_IRQn, 0U );
	/* ... */
}
/*..........................................................................*/
void SST_onIdle(void)
{
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
#endif
}

