#include "project.h"

enum State
{
    Done,
    WaitingForConversion,
    ConversionComplete
};
enum State state = Done;

float temp = -1000.0f;

void Timer(uint16 us)
{
    Timeout_WriteCounter(us);
}

CY_ISR(OnTimeout)
{
    RawThermo_Disable();
    //after timeout, just assume it's done
    state = ConversionComplete;
    RawThermo_ClearPending();
}
CY_ISR(OnConversion)
{
    RawThermo_Disable();
    state = ConversionComplete;
    RawThermo_ClearPending();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    Thermo_Start();
    Timeout_Start();
    TimeoutInt_StartEx(OnTimeout);
    
    for(;;)
    {
        switch(state)
        {
            case Done:
                Timer(65535);
                Thermo_StartConversion();
                state = WaitingForConversion;
                RawThermo_StartEx(OnConversion);
                break;
            case WaitingForConversion:
                //Wait for interrupt
                break;
            case ConversionComplete:
                if (Thermo_ReadTemp())
                    temp = Thermo_Temperature;
                state = Done;
                break;
            default:
                break;
        }
        
        LED_Write(temp > 25);
    }
}
