#ifndef `$INSTANCE_NAME`_DALLASTHERMOMETER_H
    #define `$INSTANCE_NAME`_DALLASTHERMOMETER_H
    
    #include "`$INSTANCE_NAME`_OneWire_OneWire.h"

    #define `$INSTANCE_NAME`_ROM_SearchRom 0xF0
    #define `$INSTANCE_NAME`_ROM_ReadRom 0x33
    #define `$INSTANCE_NAME`_ROM_MatchRom 0x55
    #define `$INSTANCE_NAME`_ROM_SkipRom 0xCC //Address all devices on wire
    #define `$INSTANCE_NAME`_ROM_AlarmSearch 0xEC

    #define `$INSTANCE_NAME`_CMD_StartConversion 0x44
    #define `$INSTANCE_NAME`_CMD_WriteScratchpad 0x4E
    #define `$INSTANCE_NAME`_CMD_ReadScratchpad 0xBE
    #define `$INSTANCE_NAME`_CMD_ReadPowerSupply 0xB4

    #define `$INSTANCE_NAME`_CFG_RESOLUTION_09Bits 0x1F
    #define `$INSTANCE_NAME`_CFG_RESOLUTION_10Bits 0x3F
    #define `$INSTANCE_NAME`_CFG_RESOLUTION_11Bits 0x5F
    #define `$INSTANCE_NAME`_CFG_RESOLUTION_12Bits 0x7F


    const float `$INSTANCE_NAME`_LSB_TO_C = 1 / 16.0f;
    uint16 `$INSTANCE_NAME`_AlarmTemp = 100.0 * 16.0f;
    float `$INSTANCE_NAME`_Temperature = 0.0f;

    void `$INSTANCE_NAME`_Start()
    {
        CyGlobalIntDisable;
        `$INSTANCE_NAME`_OneWire_Reset();
        `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_ROM_SkipRom);
        `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_CMD_WriteScratchpad);
        `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_AlarmTemp >> 8);
        `$INSTANCE_NAME`_OneWire_WriteByte((uint8)`$INSTANCE_NAME`_AlarmTemp);
        `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_CFG_RESOLUTION_12Bits);
        CyGlobalIntEnable;
    }
    void `$INSTANCE_NAME`_StartConversion()
    {
        CyGlobalIntDisable;
        `$INSTANCE_NAME`_OneWire_Reset();
        if (`$INSTANCE_NAME`_OneWire_Connected)
        {
            `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_ROM_SkipRom);
            `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_CMD_StartConversion);
        }
        CyGlobalIntEnable;
    }
    float `$INSTANCE_NAME`_ReadTemp()
    {
        CyGlobalIntDisable;
        `$INSTANCE_NAME`_OneWire_Reset();
        if (`$INSTANCE_NAME`_OneWire_Connected)
        {
            `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_ROM_SkipRom);
            `$INSTANCE_NAME`_OneWire_WriteByte(`$INSTANCE_NAME`_CMD_ReadScratchpad);
            uint16 rawTemp = `$INSTANCE_NAME`_OneWire_ReadByte();
            rawTemp += `$INSTANCE_NAME`_OneWire_ReadByte() << 8;
            CyGlobalIntEnable;
            if (rawTemp > 0xFFF)
                return -1000.0f;
            if (rawTemp & 0x800)
                rawTemp |= 0xF000;
            `$INSTANCE_NAME`_Temperature = ((int16)rawTemp) * `$INSTANCE_NAME`_LSB_TO_C;
        }
        return `$INSTANCE_NAME`_Temperature;
    }
#endif
