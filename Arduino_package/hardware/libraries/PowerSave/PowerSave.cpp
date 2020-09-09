
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
    ipc_send_message(IPC_INT_KM4_TICKLESS_INDICATION, (uint32_t)&DSLP_Para);
}

uint32_t PMUClass::AONWakeReason(void) {
    uint32_t aon_wake_event = SOCPS_AONWakeReason();
    if (BIT_GPIO_WAKE_STS & aon_wake_event) {
        return 11;
    }
    if (BIT_AON_WAKE_TIM0_STS & aon_wake_event) {
        return 22;
    }
    if (BIT_RTC_WAKE_STS & aon_wake_event) {
        return 33;
    }
    return 0;
}

int PMUClass::WakePinCheck(void) {
    int checkpin_number = SOCPS_WakePinCheck();
    //printf("checkpin_number %d  \r\n", checkpin_number);
// PA20; D27
    if (checkpin_number == 1) {
        return 27;
    }
// PA21; D26
    if (checkpin_number == 2) {
        return 26;
    }
//PA_25; D16
    if (checkpin_number == 4) {
        return 16;
    }
//PA_26; D17
    if (checkpin_number == 8) {
        return 17;
    }
    return 111;
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

//void TL_sleep_callback(uint32_t *suspend, uint32_t *resume) {
    //pmu_register_sleep_callback(PMU_LOGUART_DEVICE, (PSM_HOOK_FUN)suspend, NULL, (PSM_HOOK_FUN)resume, NULL);
//}

PMUClass PowerSave;
