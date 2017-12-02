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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_SAR_1_Start();
    PWM_1_Start();

    ADC_SAR_1_StartConvert();
    for(;;)
    {
        /* Place your application code here. */
        PWM_1_WriteCompare(ADC_SAR_1_GetResult8());
    }
}

/* [] END OF FILE */
