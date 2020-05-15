#ifndef __HALCOM_IQK_REGTABLE_RAM_H__
#define __HALCOM_IQK_REGTABLE_RAM_H__

#if (CONFIG_IQK_OFFLOAD || (CONFIG_MCC & IS_CHIP_RF_AC(CONFIG_CHIP_SEL)))

MEMTYPE_CODE u16    AFE_REG1[2][AFE1_REG_NUM] = {
                                            {0xc00, 0xc94, 0xc88, 0xc8c, 0xce8, 0xc10, 0xd00, 0xc90, 0xcc4, 0xcc8, 0xccc, 0xcd4, 0xc80, 0xc84, 0xcb8},
                                            {0xe00, 0xe94, 0xe88, 0xe8c, 0xee8, 0xe10, 0xd40, 0xe90, 0xec4, 0xec8, 0xecc, 0xed4, 0xe80, 0xe84, 0xeb8}};
#endif  //#if (CONFIG_IQK_OFFLOAD || (CONFIG_MCC & IS_CHIP_RF_AC(CONFIG_CHIP_SEL))

#if CONFIG_IQK_OFFLOAD

MEMTYPE_CODE u16    BK_MAC_REG[MAC_REG_NUM]       =   {0x522, 0x550, 0x551};
MEMTYPE_CODE u16    BK_BB_REG[BB_REG_NUM]        =   {0x808, 0xa04, 0x90c, 0xc00, 0xe00, 0x838, 0x82c}; 
MEMTYPE_CODE u16    BK_AFE_REG[2][AFE_REG_NUM]   =   {
                                            {0xc5c, 0xc60, 0xc64, 0xc68, 0xcb0, 0xcb4},
                                            {0xe5c, 0xe60, 0xe64, 0xe68, 0xeb0, 0xeb4}}; 
//MEMTYPE_XDATA u8    BK_RF_REG[RF_REG_NUM] = {0x65, 0x8f, 0x0}; 


MEMTYPE_XDATA u8    CHANNEL_ALL[TARGET_CHNL_NUM_2G_5G] = 
            {1,2,3,4,5,6,7,8,9,10,11,12,13,14,
             36,40,44,48,   //Band1
             52,56,60,64,   //Band2
             100,104,108,112,116,120,124,128,132,136,140,   //Band3
             149,153,157,161,165};  //Band4

#endif  //#if CONFIG_IQK_OFFLOAD
#endif  //#ifndef   __HALCOM_IQK_REGTABLE_RAM_H__





