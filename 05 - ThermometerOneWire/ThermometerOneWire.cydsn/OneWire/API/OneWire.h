#ifndef `$INSTANCE_NAME`_ONEWIRE_H
    #define `$INSTANCE_NAME`_ONEWIRE_H
    
    #include "`$INSTANCE_NAME`_Pin.h"
    
    #include "CyLib.h"

    unsigned int `$INSTANCE_NAME`_ResetTime = 1000;
    unsigned int `$INSTANCE_NAME`_ResetResponseTime = 64;
    unsigned int `$INSTANCE_NAME`_DelimitTime = 10;
    unsigned int `$INSTANCE_NAME`_WriteTime = 60;
    unsigned int `$INSTANCE_NAME`_ReadTime = 5;
    unsigned int `$INSTANCE_NAME`_BitTime = 90;

    uint8 `$INSTANCE_NAME`_Connected = 0;

    void `$INSTANCE_NAME`_Reset()
    {
        `$INSTANCE_NAME`_Pin_Write(0);
        CyDelayUs(`$INSTANCE_NAME`_ResetTime);
        `$INSTANCE_NAME`_Pin_Write(1);
        CyDelayUs(`$INSTANCE_NAME`_ResetResponseTime);
        `$INSTANCE_NAME`_Connected = !`$INSTANCE_NAME`_Pin_Read();
        CyDelayUs(`$INSTANCE_NAME`_ResetTime - `$INSTANCE_NAME`_ResetResponseTime);
    }
    void `$INSTANCE_NAME`_WriteByte(uint8 byte)
    {
        int i;
        for (i=0; i<8; i++)
        {
            `$INSTANCE_NAME`_Pin_Write(0);
            CyDelayUs(`$INSTANCE_NAME`_DelimitTime);
            `$INSTANCE_NAME`_Pin_Write((byte >> i) & 1);
            CyDelayUs(`$INSTANCE_NAME`_WriteTime);
            `$INSTANCE_NAME`_Pin_Write(1);
            CyDelayUs(`$INSTANCE_NAME`_BitTime - `$INSTANCE_NAME`_WriteTime - `$INSTANCE_NAME`_DelimitTime);
        }
    }
    uint8 `$INSTANCE_NAME`_ReadByte()
    {
        uint8 byte = 0;
        int i;
        for (i=0; i<8; i++)
        {
            `$INSTANCE_NAME`_Pin_Write(0);
            CyDelayUs(`$INSTANCE_NAME`_ReadTime);
            `$INSTANCE_NAME`_Pin_Write(1);
            CyDelayUs(`$INSTANCE_NAME`_ReadTime);
            byte += `$INSTANCE_NAME`_Pin_Read() << i;
            CyDelayUs(`$INSTANCE_NAME`_BitTime - `$INSTANCE_NAME`_ReadTime - `$INSTANCE_NAME`_ReadTime);
        }
        return byte;
    }
#endif
