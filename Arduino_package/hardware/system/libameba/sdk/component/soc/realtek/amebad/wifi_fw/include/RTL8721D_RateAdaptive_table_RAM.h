#ifndef __RTL8721D_RATE_ADAPTIVE_TABLE_RAM_H__
#define __RTL8721D_RATE_ADAPTIVE_TABLE_RAM_H__

#if CONFIG_RATE_ADAPTIVE



// =====[RA_PARAMETERS========================================================================//
        MEMTYPE_CODE u8     ARFB_table[Rate_id_NUM+2][ARFB_table_NUM]={
                            {0x1f,0xF0,0xFF,0x0F,0x00,0x00,0x00},   // 0x0ffff015 for BGN 40 2SS
                            {0x0f,0xF0,0x0F,0x00,0x00,0x00,0x00},   // 0x000ff015 for BGN 40 1SS
                            {0x0f,0xF0,0xFF,0x0F,0x00,0x00,0x00},   // 0x0ffff005 for BGN 20 2SS
                            {0x0f,0xF0,0x0F,0x00,0x00,0x00,0x00},   // 0x000ff005 for BGN 20 1SS
                            {0x10,0xF0,0xFF,0x0F,0x00,0x00,0x00},   // 0x0ff ff 01 0 for GN 2SS
                            {0x10,0xF0,0x0F,0x00,0x00,0x00,0x00},   // 0x000ff 01 0 for GN 1SS
                            {0xF5,0x0F,0x00,0x00,0x00,0x00,0x00},   // 0x000ff010 for BG
                            {0xF0,0x0F,0x00,0x00,0x00,0x00,0x00},   // 0x000ff010 for G or A
                            {0x0D,0x00,0x00,0x00,0x00,0x00,0x00},   // 0x000ff010 for B
                            {0x10,0xF0,0xFF,0xFF,0x00,0x00,0x00},   // VHT 2SS 0xFFFFF010 // ARFB VHT 2SS 0xFF00F010
                            {0x10,0xF0,0x3F,0x00,0x00,0x00,0x00}}; // VHT 1SS 0x003FF010

        MEMTYPE_CODE u8      RATE_UP_FAIL_WAITING[RATESIZE] = {
                             2,4,8,16,
                             16,20,24,28,28,32,32,32,  
                             16,20,24,28,28,32,32,32,  
                            #if 0//(CONFIG_2SS == 1)
                            16,20,24,28,28,32,32,32,         // MCS 8~15
                            #endif
                            
                            #if 0//(CONFIG_11AC == 1)
                            16,20,24,28,28,32,32,32,36,40,    // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            16,20,24,28,28,32,32,32,36,40     // 2SS MCS 0~9
                            #endif
                            #endif
                            };


        MEMTYPE_CODE u8         DIFF2VAR_TABLE[PCR_VAR_SIZE] = {
                            6,7,8,9,10,11,12,13,14,14 //diff to var : >10 +,  <10 -
                            };

        MEMTYPE_CODE u8         RATE_DOWN_PROB[RATESIZE] = {  /* ex: M7-LGI  =>  M6-LGI */
                            50, 50, 36, 50,                                   //CCK
                            50, 67, 75, 67, 75, 67, 75, 89,           // OFDM 6~54
                            50, 50, 67, 75, 67, 75, 89, 90,           // MCS 0~7
                            #if 0//(CONFIG_2SS == 1)
                            50, 50, 67, 75, 67, 75, 89, 90,            // MCS 8~15
                            #endif
                            
                            #if 0//(CONFIG_11AC == 1)
                            50, 50, 67, 75, 67, 75, 89, 90, 83, 90, // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            50, 50, 67, 75, 67, 75, 89, 90, 83, 90  // 2SS MCS 0~9
                            #endif
                            
                            #endif
                            };
       #if CONFIG_1SS_SL_GI_MULTI_TRY    
        MEMTYPE_CODE u8         RATE_DOWN_PROB_SGI2LGI[RATESIZE] = {  /* ex: M7-LGI  =>  M6-SGI */
                            50, 50, 36, 50,                                   //CCK
                            50, 67, 75, 67, 75, 67, 75, 89,           // OFDM 6~54
                            88, 88, 88, 88, 88, 88, 88, 88,           // MCS 0~7
                            #if 0//(CONFIG_2SS == 1)
                            88, 88, 88, 88, 88, 88, 88, 88,            // MCS 8~15
                            #endif
                            
                            #if 0//(CONFIG_11AC == 1)
                            88, 88, 88, 83, 88, 88, 88, 99, 90, 92, // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            88, 88, 88, 83, 88, 88, 88, 99, 90, 92  // 2SS MCS 0~9
                            #endif
                            
                            #endif
                            };
        #endif
        
        #if 0//CONFIG_1SS_SL_GI_MULTI_TRY    
        MEMTYPE_CODE u8         RATE_DOWN_PROB_LGI2SGI[RATESIZE] = {  /* ex: M7-LGI  =>  M6-SGI */
                            50, 50, 36, 50,                                   //CCK
                            50, 67, 75, 67, 75, 67, 75, 89,           // OFDM 6~54
                            50, 56, 75, 83, 74, 83, 98, 99,           // MCS 0~7
                            #if (CONFIG_2SS == 1)
                            50, 56, 75, 83, 74, 83, 98, 99,            // MCS 8~15
                            #endif
                            
                            #if (CONFIG_11AC == 1)
                            50, 56, 75, 83, 74, 83, 98, 99, 92, 99, // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            50, 56, 75, 83, 74, 83, 98, 99, 92, 99  // 2SS MCS 0~9
                            #endif
                            
                            #endif
                            };
        #endif

        MEMTYPE_CODE u8         RATE_UP_PROB[RATESIZE] = {
                            82, 83, 84, 85,                                   //CCK
                            83, 84, 85, 86, 87, 88, 89, 90,           // OFDM 6~54
                            86, 87, 88, 89, 90, 91, 92, 93,           // MCS 0~7
                            #if 0//(CONFIG_2SS == 1)
                            86, 87, 88, 89, 90, 91, 92, 93,            // MCS 8~15
                            #endif

                            #if 0//(CONFIG_11AC == 1)
                            86, 87, 88, 89, 90, 91, 92, 93, 93, 93, // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            86, 87, 88, 89, 90, 91, 92, 93, 93, 93 // 2SS MCS 0~9
                            #endif
                            #endif
                            };
        //[ reminder for trying index ]         TRY_NESS_CNT_TABLE[12]     = {2,4,6,8,10,12,16,24,32,48,64,80};
        MEMTYPE_CODE u8         TRYING_NECESSARY_idx[RATESIZE] = {
                            0,0,0,1,                  // CCK
                            0,0,1,1,1,2,2,3,      // OFDM
                            0,1,1,1,1,2,2,3,      // MCS 0~7
                            #if 0//(CONFIG_2SS == 1)
                            0,1,1,2,2,3,3,4    // MCS 8~15
                            #endif
                            #if 0//(CONFIG_11AC == 1)
                            0,1,1,2,2,3,3,4,5,6,      // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            0,1,1,2,2,3,3,4,5,6      // 2SS MCS 0~9
                            #endif
                            #endif
                            };

        MEMTYPE_CODE u8      DROPING_NECESSARY[RATESIZE] = {
                            1,1,1,1,
                            1,2,3,4,5,6,7,8,
                            1,2,3,4,5,6,7,8,
                            #if 0//(CONFIG_2SS == 1)
                            5,6,6,7,8,10,11,12,         // MCS 8~15
                            #endif

                            #if 0//(CONFIG_11AC == 1)
                            1,2,2,3,3,4,4,4,5,5,        // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            1,2,2,3,3,4,4,4,5,5,       // 2SS MCS 0~9
                            #endif
                            #endif
                            };
        

        
// =====[Multi-Try Table]===========================================================================//  
        MEMTYPE_CODE u8      RateUp_LEGACY_Multi_Table[5][2] = {
                              {RATE_CCK_2M,  RATE_OFDM_6M},       // 1M
                              {RATE_CCK_5M,  RATE_OFDM_6M},       // 2M
                              {RATE_OFDM_6M,  RATE_CCK_11M},    // 5M
                              {RATE_HT_MCS0,  RATE_OFDM_6M}, //11M
                              {RATE_HT_MCS0,  RATE_CCK_11M}}; //6M
                              

        MEMTYPE_CODE u8      PHY_RATE_20M[RATESIZE] = { // round( index*6.5 ) = phyrate
                             0,0,1,2,
                             1,1,2,3,4,6,8,9,  
                             1,2,3,4,6,8,9,10,  
                            #if 0//(CONFIG_2SS == 1)
                            2,4,6,8,12,16,18,20,         // MCS 8~15
                            #endif
                            
                            #if 0//(CONFIG_11AC == 1)
                            1,2,3,4,6,8,9,10,12,14,    // 1SS MCS 0~9
                            #if (CONFIG_2SS == 1)
                            2,4,6,8,12,16,18,20,24,27    // 2SS MCS 0~9
                            #endif
                            #endif
                            };
#if 0//(CONFIG_2SS) 
        MEMTYPE_CODE u8      PHY_RATE_HT_20M[28] = { //MCS0~MCS15,  round( index*6.5 ) = phyrate
                             0,0,1,2,
                             1,1,2,3,4,6,8,9,  
                             1,2,3,4,6,8,9,10,         // MCS 0~7
                             2,4,6,8,12,16,18,20,   // MCS 8~15
                            };

        MEMTYPE_CODE u8      PHY_RATE_VHT_20M[32] = { //MCS0~MCS15,  round( index*6.5 ) = phyrate
                             0,0,1,2,
                             1,1,2,3,4,6,8,9,  
                             1,2,3,4,6,8,9,10,12,14,    // 1SS MCS 0~9
                             2,4,6,8,12,16,18,20,24,27    // 2SS MCS 0~9
                            };
#endif

#if (CONFIG_1SS)

  #if 0//(CONFIG_11AC == 1)
            MEMTYPE_CODE u8      RateUp_Multi_Table_SGI_VHT[3][2] = {
            		      {RATE_VHT1SS_MCS7 | TXRPT_SGI, RATE_VHT1SS_MCS7},   //  MCS6-SGI	      
            		      {RATE_VHT1SS_MCS8, RATE_VHT1SS_MCS8 | TXRPT_SGI},   //  MCS7-SGI
            		      {RATE_VHT1SS_MCS9 | TXRPT_SGI, RATE_VHT1SS_MCS9},   //  MCS8-SGI
                            };
  
            MEMTYPE_CODE u8      RateDown_Multi_Table_SGI_VHT[3][2] = {
            		      {RATE_VHT1SS_MCS6 | TXRPT_SGI, RATE_VHT1SS_MCS7},   //  MCS7-SGI	      
            		      {RATE_VHT1SS_MCS8, RATE_VHT1SS_MCS7 | TXRPT_SGI},   //  MCS8-SGI
            		      {RATE_VHT1SS_MCS8 | TXRPT_SGI, RATE_VHT1SS_MCS9},   //  MCS9-SGI
                            };
  #endif


#elif 0//(CONFIG_2SS) 


        MEMTYPE_CODE u8      RateUp_Multi_Table[MULTI_TRY_TABLE_SIZE][2] = {
            		      {RATE_HT_MCS8,    RATE_HT_MCS1},   // idx=0
            		      {RATE_HT_MCS9,    RATE_HT_MCS2},   // idx=1
            		      {RATE_HT_MCS9,    RATE_HT_MCS3},   // idx=2
            		      {RATE_HT_MCS10,   RATE_HT_MCS4}, // idx=3
            		      {RATE_HT_MCS11,   RATE_HT_MCS5}, // idx=4
            		      {RATE_HT_MCS12,   RATE_HT_MCS6}, // idx=5
            		      {RATE_HT_MCS12,   RATE_HT_MCS7}, // idx=6
            		      {RATE_HT_MCS12,   RATE_UNUSED},   // idx=7
            		      {RATE_HT_MCS9,     RATE_HT_MCS2},  // idx=8
            		      {RATE_HT_MCS10,   RATE_HT_MCS4}, // idx=9
            		      {RATE_HT_MCS11,   RATE_HT_MCS5}, // idx=10
            		      {RATE_HT_MCS12,   RATE_HT_MCS6}, // idx=11
            		      {RATE_HT_MCS13,   RATE_UNUSED},   // idx=12
            		      {RATE_HT_MCS14,   RATE_UNUSED},   // idx=13
            		      {RATE_HT_MCS15,   RATE_UNUSED}}; // idx=14

        MEMTYPE_CODE u8      RateDown_Multi_Table[MULTI_TRY_TABLE_SIZE][2] = {
            		      {RATE_HT_MCS8,    RATE_HT_MCS0},   // idx=0
            		      {RATE_HT_MCS8,    RATE_HT_MCS1},   // idx=1
            		      {RATE_HT_MCS2,    RATE_HT_MCS8},   // idx=2
            		      {RATE_HT_MCS9,    RATE_HT_MCS3}, // idx=3
            		      {RATE_HT_MCS10,  RATE_HT_MCS4}, // idx=4
            		      {RATE_HT_MCS11,  RATE_HT_MCS5}, // idx=5
            		      {RATE_HT_MCS6,    RATE_HT_MCS11}, // idx=6
            		      {RATE_HT_MCS0,    RATE_UNUSED},   // idx=7
            		      {RATE_HT_MCS2,    RATE_HT_MCS8},  // idx=8
            		      {RATE_HT_MCS9,    RATE_HT_MCS3}, // idx=9
            		      {RATE_HT_MCS10,  RATE_HT_MCS4}, // idx=10
            		      {RATE_HT_MCS7,    RATE_HT_MCS11}, // idx=11
            		      {RATE_HT_MCS12,   RATE_HT_MCS7}, // idx=12
            		      {RATE_HT_MCS13,   RATE_HT_MCS12}, // idx=13
            		      {RATE_HT_MCS14,   RATE_HT_MCS13}}; // idx=14
#endif

        MEMTYPE_CODE u8      PT_PENALTY[RETRYSIZE+1] = {25,6,4,2,0,24};    //{32,30,28,24,16,24};{25,6,4,2,0,24}


#endif  //#if CONFIG_RATE_ADAPTIVE
#endif  //#ifndef  __RTL8721D_RATE_ADAPTIVE_TABLE_RAM_H__






