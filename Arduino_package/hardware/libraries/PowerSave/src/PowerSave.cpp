
#ifdef __cplusplus
extern "C" {
#include "ameba_soc.h"

}
#endif

#include "PowerSave.h"

KM4SLEEP_ParamDef DSLP_Para;

PMUClass::PMUClass(void) {
}

void PMUClass::begin(uint32_t sleep_mode) {
    if (sleep_mode == 11) {
        // deepsleep
        InterruptRegister(IPC_INTHandler, IPC_IRQ, (u32)IPCM0_DEV, 5);
        InterruptEn(IPC_IRQ, 5);
        ipc_table_init();
    } else if (sleep_mode == 22) {
        // tickless
        InterruptRegister(IPC_INTHandler, IPC_IRQ, (u32)IPCM0_DEV, 10);
        InterruptEn(IPC_IRQ, 10);
        ipc_table_init();
        InterruptRegister((IRQ_FUN) shell_uart_irq_rom, UART_LOG_IRQ, (u32)NULL, 10);
        InterruptEn(UART_LOG_IRQ, 10);
        /*set KM4 sleep type*/
        pmu_set_sleep_type(SLEEP_PG);
    } else {
        printf("ERROR: sleep mode selection fail.    \r\n");
    }
}

void PMUClass::AONTimerDuration(uint32_t duration_ms) {
    DSLP_Para.sleep_time = duration_ms;
}

void PMUClass::AONTimerCmd(void) {
    SOCPS_AONTimerCmd(DISABLE);
}

void PMUClass::RTCWakeSetup(uint32_t duration_d, uint32_t duration_h, uint32_t duration_m, uint32_t duration_s) {
    RTC_InitTypeDef RTC_InitStruct_temp;
    RTC_AlarmTypeDef RTC_AlarmStruct_temp;
    RTC_TimeTypeDef RTC_TimeStruct;

    RTC_StructInit(&RTC_InitStruct_temp);
    RTC_Init(&RTC_InitStruct_temp);

    RTC_TimeStructInit(&RTC_TimeStruct);
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
    RTC_AlarmStructInit(&RTC_AlarmStruct_temp);

    RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Days = duration_d;
    RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Hours = duration_h;
    RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Minutes = duration_m;
    RTC_AlarmStruct_temp.RTC_AlarmTime.RTC_Seconds = duration_s;

#if 0
    if (duration_d > 0) {
        RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_None;
        RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_None;
    } else {
        if (duration_h > 0) {
            RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_None;
            RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_Days;
        } else {
            if (duration_m > 0) {
                RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_Hours;
                RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_Days;
            } else {
                if (duration_s > 0) {
                    RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
                    RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_Days;
                } else {
                    RTC_AlarmStruct_temp.RTC_AlarmMask = RTC_AlarmMask_All;
                    RTC_AlarmStruct_temp.RTC_Alarm2Mask = RTC_Alarm2Mask_Days;
                }
            }
        }
    }

#else
    // RTC_AlarmMask_None    RTC_AlarmMask_Hours    RTC_AlarmMask_Minutes    RTC_AlarmMask_Seconds    RTC_AlarmMask_All 
    // RTC_Alarm2Mask_None    RTC_Alarm2Mask_Days
    uint32_t temp_mask1 = 0;
    uint32_t temp_mask2 = 0;
    if (duration_d <= 0) {
        temp_mask2 = temp_mask2 | RTC_Alarm2Mask_Days;
    }
    if (duration_h <= 0) {
        temp_mask1 = temp_mask1 | RTC_AlarmMask_Hours;
    }
    if (duration_m <= 0) {
        temp_mask1 = temp_mask1 | RTC_AlarmMask_Minutes;
    }
    if (duration_s <= 0) {
        temp_mask1 = temp_mask1 | RTC_AlarmMask_Seconds;
    }

    RTC_AlarmStruct_temp.RTC_AlarmMask = temp_mask1;
    RTC_AlarmStruct_temp.RTC_Alarm2Mask = temp_mask2;
#endif

    RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStruct_temp);
    RTC_AlarmCmd(ENABLE);
}

void PMUClass::enable(void) {
    DSLP_Para.dlps_enable = TRUE;
    asm volatile ("cpsid i" : : : "memory");
    ipc_send_message(IPC_INT_KM4_TICKLESS_INDICATION, (uint32_t)&DSLP_Para);
    asm volatile ("wfe");
    asm volatile ("wfe");
    asm volatile ("cpsie i" : : : "memory");
}

uint32_t PMUClass::AONWakeReason(void) {
    uint32_t aon_wake_event = SOCPS_AONWakeReason();
    if (BIT_GPIO_WAKE_STS & aon_wake_event) {
        return AONWakeReason_AON_GPIO;
    }
    if (BIT_AON_WAKE_TIM0_STS & aon_wake_event) {
        return AONWakeReason_AON_TIMER;
    }
    if (BIT_RTC_WAKE_STS & aon_wake_event) {
        return AONWakeReason_RTC;
    }
    return 0;
}

//BIT(0): wakepin0, BIT(1): wakepin1, BIT(2): wakepin2, BIT(3): wakepin3
//wakepin 0 :
//  PA_12,  PINMUX_S0
//  PA_16,  PINMUX_S1
//  PA_20,  PINMUX_S2
//  
//wakepin 1 :
//  PA_13, PINMUX_S0
//  PA_17, PINMUX_S1
//  PA_21, PINMUX_S2
//wakepin 2 :
//  PA_14, PINMUX_S0
//  PA_18, PINMUX_S1
//  PA_25, PINMUX_S2
//  wakepin 3 :
//  PA_15, PINMUX_S0
//  PA_19, PINMUX_S1
//  PA_26  PINMUX_S2
int PMUClass::WakePinCheck(void) {
    int checkpin_number = SOCPS_WakePinCheck();
    //printf("checkpin_number %d  \r\n", checkpin_number);
    if (checkpin_number == 1) {
        return 1; //BIT(0): wakepin0
    } else if (checkpin_number == 2) {
        return 2; //BIT(1): wakepin1
    } else if (checkpin_number == 4) {
        return 4; //BIT(2): wakepin2
    } else if (checkpin_number == 8) {
        return 8; //BIT(3): wakepin3
    }
    return 0;
}

void PMUClass::AONWakeClear(void) {
    SOCPS_AONWakeClear(BIT_ALL_WAKE_STS);
}

bool PMUClass::DsleepWakeStatusGet(void) {
    return SOCPS_DsleepWakeStatusGet();
}

void PMUClass::TL_sysactive_time(uint32_t duration_ms) {
    pmu_set_sysactive_time(duration_ms);
}

void PMUClass::TL_wakelock(uint32_t select_lock) {
    if (select_lock == 1) {
        /*acquire wakelock to avoid KM4 enter sleep mode*/
        pmu_acquire_wakelock(PMU_OS);
    } else if (select_lock == 0) {
        /*release wakelock to allow KM4 enter sleep mode*/
        pmu_release_wakelock(PMU_OS);
    } else {
        printf("ERROR: TL_wakelock wrong selection.    \r\n");
    }
}

void PMUClass::TL_sleep_callback(uint32_t suspend(void), uint32_t resume(void)) {
    TL_wakelock(1);
    pmu_register_sleep_callback(PMU_LOGUART_DEVICE, (PSM_HOOK_FUN)suspend, NULL, (PSM_HOOK_FUN)resume, NULL);
    TL_wakelock(0);
}


void PMUClass::DS_AON_TIMER_WAKEUP(void) {
    printf("Set Deepsleep wakeup AON timer.    \r\n");
}
void PMUClass::DS_RTC_WAKEUP(void) {
    printf("Set Deepsleep wakeup RTC.    \r\n");
}

void PMUClass::TL_UART_WAKEUP(void) {
    printf("Set Tickless wakeup LOGUART.    \r\n");
}
void PMUClass::TL_RTC_WAKEUP(void) {
    printf("Set Tickless wakeup RTC.    \r\n");
}

//For board AMB21/AMB22
    //AON_WAKEPIN_WAKEUP_PA25    // pin 16
    //AON_WAKEPIN_WAKEUP_PA26    // pin 17
    //AON_WAKEPIN_WAKEUP_PA21    // pin 26
    //AON_WAKEPIN_WAKEUP_PA20    // pin 27
    //AON_WAKEPIN_WAKEUP_PA19    // pin 28
//For board AMB23
    //AON_WAKEPIN_WAKEUP_PA12    // pin 9
    //AON_WAKEPIN_WAKEUP_PA13    // pin 10
    //AON_WAKEPIN_WAKEUP_PA14    // pin 11
    //AON_WAKEPIN_WAKEUP_PA15    // pin 12
    //AON_WAKEPIN_WAKEUP_PA16    // pin 13
    //AON_WAKEPIN_WAKEUP_PA18    // pin 15
    //AON_WAKEPIN_WAKEUP_PA19    // pin 16
    //AON_WAKEPIN_WAKEUP_PA21    // pin 18
//For board BW16/BW16-TypeC
    //AON_WAKEPIN_WAKEUP_PA25    // pin 7
    //AON_WAKEPIN_WAKEUP_PA26    // pin 8
    //AON_WAKEPIN_WAKEUP_PA15    // pin 9
    //AON_WAKEPIN_WAKEUP_PA14    // pin 10
    //AON_WAKEPIN_WAKEUP_PA13    // pin 11
    //AON_WAKEPIN_WAKEUP_PA12    // pin 12
//For board AW-CU488_ThingPlus
    //AON_WAKEPIN_WAKEUP_PA18    // pin 0
    //AON_WAKEPIN_WAKEUP_PA16    // pin 1
    //AON_WAKEPIN_WAKEUP_PA17    // pin 2
    //AON_WAKEPIN_WAKEUP_PA13    // pin 3
    //AON_WAKEPIN_WAKEUP_PA12    // pin 4
    //AON_WAKEPIN_WAKEUP_PA26    // pin 5
    //AON_WAKEPIN_WAKEUP_PA25    // pin 6
    //AON_WAKEPIN_WAKEUP_PA19    // pin 8
    //AON_WAKEPIN_WAKEUP_PA15    // pin 28
    //AON_WAKEPIN_WAKEUP_PA14    // pin 29

#if defined(BOARD_RTL8722DM)
void PMUClass::AON_WAKEPIN_WAKEUP_PA12(void) {
    printf("Set Deepsleep wakeup AON pin PA12.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA13(void) {
    printf("Set Deepsleep wakeup AON pin PA13.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA14(void) {
    printf("Set Deepsleep wakeup AON pin PA14.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA15(void) {
    printf("Set Deepsleep wakeup AON pin PA15.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA16(void) {
    printf("Set Deepsleep wakeup AON pin PA16.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA17(void) {
    printf("Set Deepsleep wakeup AON pin PA17.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA18(void) {
    printf("Set Deepsleep wakeup AON pin PA18.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA19(void) {
    printf("Set Deepsleep wakeup AON pin PA19 / pin 28.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA20(void) {
    printf("Set Deepsleep wakeup AON pin PA20 / pin 27.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA21(void) {
    printf("Set Deepsleep wakeup AON pin PA21 / pin 26.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA25(void) {
    printf("Set Deepsleep wakeup AON pin PA25 / pin 16.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA26(void) {
    printf("Set Deepsleep wakeup AON pin PA26 / pin 17.    \r\n");
}

#elif defined(BOARD_RTL8722DM_MINI)
void PMUClass::AON_WAKEPIN_WAKEUP_PA12(void) {
    printf("Set Deepsleep wakeup AON pin PA12 / pin 9.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA13(void) {
    printf("Set Deepsleep wakeup AON pin PA13 / pin 10.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA14(void) {
    printf("Set Deepsleep wakeup AON pin PA14 / pin 11.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA15(void) {
    printf("Set Deepsleep wakeup AON pin PA15 / pin 12.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA16(void) {
    printf("Set Deepsleep wakeup AON pin PA16 / pin 13.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA17(void) {
    printf("RTL8722DM_MINI does not support Deepsleep wakeup AON pin PA17.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA18(void) {
    printf("Set Deepsleep wakeup AON pin PA18 / pin 15.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA19(void) {
    printf("Set Deepsleep wakeup AON pin PA19 / pin 16.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA20(void) {
    printf("RTL8722DM_MINI does not support Deepsleep wakeup AON pin PA20.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA21(void) {
    printf("Set Deepsleep wakeup AON pin PA21 / pin 18.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA25(void) {
    printf("RTL8722DM_MINI does not support Deepsleep wakeup AON pin PA25.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA26(void) {
    printf("RTL8722DM_MINI does not support Deepsleep wakeup AON pin PA26.    \r\n");
}

#elif defined(BOARD_RTL8720DN_BW16)
void PMUClass::AON_WAKEPIN_WAKEUP_PA12(void) {
    printf("Set Deepsleep wakeup AON pin PA12 / pin 12.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA13(void) {
    printf("Set Deepsleep wakeup AON pin PA13 / pin 11.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA14(void) {
    printf("Set Deepsleep wakeup AON pin PA14 / pin 10.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA15(void) {
    printf("Set Deepsleep wakeup AON pin PA15 / pin 9.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA16(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA16.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA17(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA17.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA18(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA18.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA19(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA19.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA20(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA20.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA21(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA21.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA25(void) {
    printf("Set Deepsleep wakeup AON pin PA25 / pin 7.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA26(void) {
    printf("Set Deepsleep wakeup AON pin PA26 / pin 8.    \r\n");
}

#elif defined(BOARD_RTL8721DM)
void PMUClass::AON_WAKEPIN_WAKEUP_PA12(void) {
    printf("Set Deepsleep wakeup AON pin PA12 / pin 4.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA13(void) {
    printf("Set Deepsleep wakeup AON pin PA13 / pin 3.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA14(void) {
    printf("Set Deepsleep wakeup AON pin PA14 / pin 29.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA15(void) {
    printf("Set Deepsleep wakeup AON pin PA15 / pin 28.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA16(void) {
    printf("Set Deepsleep wakeup AON pin PA16 / pin 1.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA17(void) {
    printf("Set Deepsleep wakeup AON pin PA17 / pin 2.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA18(void) {
    printf("Set Deepsleep wakeup AON pin PA18 / pin 0.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA19(void) {
    printf("Set Deepsleep wakeup AON pin PA19 / pin 8.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA20(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA20.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA21(void) {
    printf("RTL8720DN_BW16 does not support Deepsleep wakeup AON pin PA21.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA25(void) {
    printf("Set Deepsleep wakeup AON pin PA25 / pin 6.    \r\n");
}
void PMUClass::AON_WAKEPIN_WAKEUP_PA26(void) {
    printf("Set Deepsleep wakeup AON pin PA26 / pin 5.    \r\n");
}
#endif

PMUClass PowerSave;
