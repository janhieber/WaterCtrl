/** @file stuff.c
 *  @brief some usefull stuff
 *
 *  @author Jan Hieber <mail@janhieber.net>
 */

/** @addtogroup Stuff
  * @{
  */

#include "stm32f1xx_hal.h"

#include <mxconstants.h>

#include <usart.h>

/**
 * @brief System alive indicator for LED
 */
void AliveTicker(void) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/**
 * @brief Reset watchdog
 */
void WatchdogReset(IWDG_HandleTypeDef *hiwdg) {
#ifdef USEWATCHDOG
  HAL_IWDG_Refresh(hiwdg);
#endif
}

/**
 * @brief Start watchdog
 */
void WatchdogStart(IWDG_HandleTypeDef *hiwdg) {
#ifdef USEWATCHDOG
  IWDG_ENABLE_WRITE_ACCESS(hiwdg);
  __HAL_IWDG_START(hiwdg);
#endif
}



/*****************************************
 * The above code are some fault handlers.
 * The code is ugly copy/paste. Need rewrite!
 ****************************************/


struct fault_saved{
  unsigned int arm_r4;
  unsigned int arm_r5;
  unsigned int arm_r6;
  unsigned int arm_r7;
  unsigned int arm_r8;
  unsigned int arm_r9;
  unsigned int arm_r10;
  unsigned int arm_fp;
  // saved by hardware
  unsigned int arm_r0;
  unsigned int arm_r1;
  unsigned int arm_r2;
  unsigned int arm_r3;
  unsigned int arm_ip;
  unsigned int arm_lr;
  unsigned int arm_pc;
  unsigned int arm_cpsr;
};


void default_handler (void)
{
  asm(" add r1, sp, #32\n"    // old stack pointer
         " sub sp, sp, #32\n"    // allocate additional storage
         " stmia sp, {r4-r11}\n" // save registers
         " mov r0, sp\n"
         " b default_handler_c\n");
}


void  default_handler_c(struct fault_saved *saved, unsigned int sp)
{


  if (CoreDebug->DHCSR & 1)  {     // check C_DEBUGEN == 1 -> Debugger Connected
    __asm volatile("bkpt 0\n");
  }
  while (1);                       // enter endless loop otherwise
}


/* The prototype shows it is a naked function - in effect this is just an
assembly function. */
static void HardFault_Handler( void ) __attribute__( ( naked ) );

/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
static void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}


void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
  /* These are volatile to try and prevent the compiler/linker optimising them
  away as the variables never actually get used.  If the debugger won't show the
  values of the variables, make them global my moving their declaration outside
  of this function. */
  volatile uint32_t r0;
  volatile uint32_t r1;
  volatile uint32_t r2;
  volatile uint32_t r3;
  volatile uint32_t r12;
  volatile uint32_t lr; /* Link register. */
  volatile uint32_t pc; /* Program counter. */
  volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    /* When the following line is hit, the variables contain the register values. */
    if (CoreDebug->DHCSR & 1)  {     // check C_DEBUGEN == 1 -> Debugger Connected
      __asm volatile("bkpt 0\n");
    }
    while(1);
}



/**
  * @}
  */
