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
    uint8 lightState = 0;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    YourNameForUSBHere_Start(0, YourNameForUSBHere_5V_OPERATION);

    for(;;)
    {
        /* Place your application code here. */
        if (LightItUp_Read() != lightState)
        {
            lightState = LightItUp_Read();
            if (lightState)
                YourNameForUSBHere_PutString("Light On\r\n");
            else
                YourNameForUSBHere_PutString("Light Off\r\n");
        }
    }
}

/* [] END OF FILE */
