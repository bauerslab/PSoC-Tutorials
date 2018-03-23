/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#define PRESSED 0
#define RELEASED 1
#define ON 1
#define OFF 0

CY_ISR(SwitchInterrupt)
{
    CyDelay(50);
    Pin_Led_Write(OFF);
    while(Pin_Sw_Read() == PRESSED);
    CyDelay(50);
    Pin_Led_Write(ON);
    Pin_Sw_ClearInterrupt();
    Sw_Isr_ClearPending();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Sw_Isr_StartEx(SwitchInterrupt);

    for(;;)
    {
        /* Place your application code here. */
        CySysPmDeepSleep();
    }
}

/* [] END OF FILE */
