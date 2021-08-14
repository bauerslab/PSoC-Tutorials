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
#include <math.h>
#include <stdlib.h>

#define min(a,b) (a < b) ? a : b

const int STEP_CLOCK_FREQUENCY = 1000000;
uint32 maxStepPeriod = 1000000;//Cycles / step

double stepSize = 360.0 / 200.0;//Converts steps to units that we want to use (in this case: 360 degrees of rotation per 200 steps)
double accel = 360;//degrees / s^2
double maxVelocity = 240*360/60;//degrees / s
double fromPosition = 0;
double toPosition = 0;

uint8 moving = 0;
double newPosition = 320000*360;

double getCurrentPosition()
{
    return (int)Step_Counter_ReadCounter() * stepSize;
}

//Update the variables during the move to change the speed when accelerating
void UpdateMove()
{
    double currentPosition = getCurrentPosition();
    //Use whichever is closer start or end to determine speed target (this assumes acceleration == deceleration)
    double distanceFromStart = fabs(currentPosition - fromPosition) + stepSize;//add stepSize to use next step for speed target (don't want 0 speed at step 0)
    double distanceFromEnd = fabs(toPosition - currentPosition);
    double accelPosition = min(distanceFromStart,distanceFromEnd);
    
    double targetVelocity = min(accel * sqrt((2.0 * accelPosition) / accel), maxVelocity);
    
    //convert to count of clock cycles used by Step_Timer
    uint32 step_period = maxStepPeriod;
    if (targetVelocity > 0)
        step_period = min(round(STEP_CLOCK_FREQUENCY / (targetVelocity / stepSize)), maxStepPeriod);
        
    //Must have at least 2 timer cycles for a step cycle (one high cycle and one low cycle)
    if (step_period < 2)
        step_period = 2;
    
    //Set step period to desired clock cycles per step
    Step_Timer_WritePeriod(step_period);
    Step_Timer_WriteCompare(step_period / 2);//compare is step pulse duration. halfway for 50% duty cycle
}

void StartMove(double targetPosition)
{
    //The move starts at the current position
    fromPosition = getCurrentPosition();
    toPosition = targetPosition;
    
    //Set step timer to execute first step immediately and wait for the period to be updated
    Step_Timer_WriteCounter(0);
    Step_Timer_WritePeriod(maxStepPeriod);
    Step_Timer_WriteCompare(maxStepPeriod);
    
    int targetStep = round(targetPosition / stepSize);
    uint32 compare = targetStep;
    //Set target position in step counter
    Step_Counter_WriteCompare(compare);
    
    Direction_Write(toPosition >= fromPosition);
    Enable_Write(1);
    UpdateMove();
}

CY_ISR(On_Button)
{
    if (!moving)
    {
        StartMove(newPosition);
        moving = 1;
    }
    Button_Interrupt_ClearPending();
}

CY_ISR(On_Step)
{
    //Every time step is sent update for next step
    UpdateMove();
    Step_Interrupt_ClearPending();
}

CY_ISR(On_Done)
{
    Enable_Write(0);
    moving = 0;
    //Change the target position so the next move will move somewhere else
    newPosition -= 360;
    Done_Interrupt_ClearPending();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Step_Interrupt_StartEx(On_Step);
    Done_Interrupt_StartEx(On_Done);
    Button_Interrupt_StartEx(On_Button);
    Step_Timer_Start();
    Step_Counter_Start();

    for(;;)
    {
    }
}

/* [] END OF FILE */
