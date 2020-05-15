/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     client_cmd.h
  * @brief    Head file for mesh client model cmd.
  * @details  User command interfaces.
  * @author   bill
  * @date     2017-3-31
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _CLIENT_CMD_H
#define _CLIENT_CMD_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "data_uart.h"
#include "user_cmd_parse.h"
#include "mesh_api.h"

/****************************************************************************************************************
* exported variables other .c files may use all defined here.
****************************************************************************************************************/

/**
 * @addtogroup CLIENT_CMD
 * @{
 */

/**
 * @defgroup Client_Cmd_Exported_Macros Client Command Exported Macros
 * @brief
 * @{
 */
#define CLIENT_CMD \
    {\
        "cmg",\
        "cmg [dst] [model]\n\r",\
        "configuration model get parameters\n\r",\
        user_cmd_cmg\
    },\
    {\
     "cms",\
     "cms [dst] [model] [parameters]\n\r",\
     "configuration model set parameters\n\r",\
     user_cmd_cms\
    },\
    {\
     "gdttg",\
     "gdttg [dst address] [app key index]\n\r",\
     "generic default transition time get\n\r",\
     user_cmd_gdtt_get\
    },\
    {\
     "gdtts",\
     "gdtts [dst address] [steps] [resolution] [ack] [app key index]\n\r",\
     "generic default transition time set\n\r",\
     user_cmd_gdtt_set\
    },\
    {\
     "hfg",\
     "hfg [dst address] [app key index] [company id]\n\r",\
     "health fault get\n\r",\
     user_cmd_health_fault_get\
    },\
    {\
     "hfc",\
     "hfc [dst address] [app key index] [company_id] [ack]\n\r",\
     "health fault clear\n\r",\
     user_cmd_health_fault_clear\
    },\
    {\
     "hft",\
     "hft [dst address] [app key index] [test id] [company id] [ack]\n\r",\
     "health fault test\n\r",\
     user_cmd_health_fault_test\
    },\
    {\
     "hpg",\
     "hps [dst address] [app key index]\n\r",\
     "health period get\n\r",\
     user_cmd_health_period_get\
    },\
    {\
     "hps",\
     "hps [dst address] [app key index] [period] [ack]\n\r",\
     "health period set\n\r",\
     user_cmd_health_period_set\
    },\
    {\
     "hag",\
     "hag [dst address] [app key index]\n\r",\
     "health attention timer get\n\r",\
     user_cmd_health_attn_get\
    },\
    {\
     "has",\
     "has [dst address] [app key index] [time] [ack]\n\r",\
     "health attention timer set\n\r",\
     user_cmd_health_attn_set\
    },\
    {\
     "goog",\
     "goog [dst] [app_key_index]\n\r",\
     "generic on off get\n\r",\
     user_cmd_generic_on_off_get\
    },\
    {\
     "goos",\
     "goos [dst] [on/off] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "generic on off set\n\r",\
     user_cmd_generic_on_off_set\
    },\
    {\
     "glg",\
     "glg [dst] [app_key_index]\n\r",\
     "generic level get\n\r",\
     user_cmd_generic_level_get\
    },\
    {\
     "gls",\
     "gls [dst] [level] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "generic level set\n\r",\
     user_cmd_generic_level_set\
    },\
    {\
     "gds",\
     "gds [dst] [delta] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "generic delta set\n\r",\
     user_cmd_generic_delta_set\
    },\
    {\
     "gms",\
     "gms [dst] [move] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "generic move set\n\r",\
     user_cmd_generic_move_set\
    },\
    {\
     "gopug",\
     "gopug [dst] [app_key_index]\n\r",\
     "generic on power up get\n\r",\
     user_cmd_generic_on_powerup_get\
    },\
    {\
     "gopus",\
     "gopus [dst] [power] [ack] [app_key_index]\n\r",\
     "generic on power up set\n\r",\
     user_cmd_generic_on_powerup_set\
    },\
    {\
     "gplg",\
     "gplg [dst] [app_key_index]\n\r",\
     "generic power level get\n\r",\
     user_cmd_generic_power_level_get\
    },\
    {\
     "gpls",\
     "gpls [dst] [level] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "generic power level set\n\r",\
     user_cmd_generic_power_level_set\
    },\
    {\
     "gpllg",\
     "gpllg [dst] [app_key_index]\n\r",\
     "generic power last get\n\r",\
     user_cmd_generic_power_last_get\
    },\
    {\
     "gpdg",\
     "gpdg [dst] [app_key_index]\n\r",\
     "generic power default get\n\r",\
     user_cmd_generic_power_default_get\
    },\
    {\
     "gpds",\
     "gpds [dst] [power] [ack] [app_key_index]\n\r",\
     "generic power default set\n\r",\
     user_cmd_generic_power_default_set\
    },\
    {\
     "gprg",\
     "gprg [dst] [app_key_index]\n\r",\
     "generic power range get\n\r",\
     user_cmd_generic_power_range_get\
    },\
    {\
     "gprs",\
     "gprs [dst] [min] [max] [ack] [app_key_index]\n\r",\
     "generic power range set\n\r",\
     user_cmd_generic_power_range_set\
    },\
    {\
     "gbg",\
     "gbg [dst] [app_key_index]\n\r",\
     "generic battery get\n\r",\
     user_cmd_generic_battery_get\
    },\
    {\
     "llg",\
     "llg [dst] [app_key_index]\n\r",\
     "light lightness get\n\r",\
     user_cmd_light_lightness_get\
    },\
    {\
     "lls",\
     "lls [dst] [lightness] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light lightness set\n\r",\
     user_cmd_light_lightness_set\
    },\
    {\
     "lllg",\
     "lllg [dst] [app_key_index]\n\r",\
     "light lightness linear get\n\r",\
     user_cmd_light_lightness_linear_get\
    },\
    {\
     "llls",\
     "llls [dst] [lightness] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light lightness linear set\n\r",\
     user_cmd_light_lightness_linear_set\
    },\
    {\
     "lllag",\
     "lllag [dst] [app_key_index]\n\r",\
     "light lightness last get\n\r",\
     user_cmd_light_lightness_last_get\
    },\
    {\
     "lldg",\
     "lldg [dst] [app_key_index]\n\r",\
     "light lightness default get\n\r",\
     user_cmd_light_lightness_default_get\
    },\
    {\
     "llds",\
     "llds [dst] [lightness] [ack] [app_key_index]\n\r",\
     "light lightness default set\n\r",\
     user_cmd_light_lightness_default_set\
    },\
    {\
     "llrg",\
     "llrg [dst] [app_key_index]\n\r",\
     "light lightness range get\n\r",\
     user_cmd_light_lightness_range_get\
    },\
    {\
     "llrs",\
     "llrs [dst] [min] [max] [ack] [app_key_index]\n\r",\
     "light lightness range set\n\r",\
     user_cmd_light_lightness_range_set\
    },\
    {\
     "lcg",\
     "lcg [dst] [app_key_index]\n\r",\
     "light ctl set\n\r",\
     user_cmd_light_ctl_get\
    },\
    {\
     "lcs",\
     "lcs [dst] [lightness] [temperature] [delta uv] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light ctl set\n\r",\
     user_cmd_light_ctl_set\
    },\
    {\
     "lctg",\
     "lctg [dst] [app_key_index]\n\r",\
     "light ctl temperature get\n\r",\
     user_cmd_light_ctl_temperature_get\
    },\
    {\
     "lcts",\
     "lcts [dst] [temperature] [delta uv] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light ctl temperature set\n\r",\
     user_cmd_light_ctl_temperature_set\
    },\
    {\
     "lctrg",\
     "lctrg [dst] [app_key_index]\n\r",\
     "light ctl temperature range get\n\r",\
     user_cmd_light_ctl_temperature_range_get\
    },\
    {\
     "lctrs",\
     "lctrs [dst] [min] [max] [ack] [app_key_index]\n\r",\
     "light ctl temperature range set\n\r",\
     user_cmd_light_ctl_temperature_range_set\
    },\
    {\
     "lcdg",\
     "lcdg [dst] [app_key_index]\n\r",\
     "light ctl default get\n\r",\
     user_cmd_light_ctl_default_get\
    },\
    {\
     "lcds",\
     "lcds [dst] [lightness] [temperature] [delta uv] [ack] [app_key_index]\n\r",\
     "light ctl default set\n\r",\
     user_cmd_light_ctl_default_set\
    },\
    {\
     "lhg",\
     "lhg [dst] [app_key_index]\n\r",\
     "light hsl get\n\r",\
     user_cmd_light_hsl_get\
    },\
    {\
     "lhs",\
     "lhs [dst] [hue] [saturation] [lightness] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light hsl set\n\r",\
     user_cmd_light_hsl_set\
    },\
    {\
     "lhtg",\
     "lhtg [dst] [app_key_index]\n\r",\
     "light hsl target get\n\r",\
     user_cmd_light_hsl_target_get\
    },\
    {\
     "lhhg",\
     "lhhg [dst] [app_key_index]\n\r",\
     "light hsl hue get\n\r",\
     user_cmd_light_hsl_hue_get\
    },\
    {\
     "lhhs",\
     "lhhs [dst] [hue] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light hsl hue set\n\r",\
     user_cmd_light_hsl_hue_set\
    },\
    {\
     "lhsg",\
     "lhsg [dst] [app_key_index]\n\r",\
     "light hsl saturation get\n\r",\
     user_cmd_light_hsl_saturation_get\
    },\
    {\
     "lhss",\
     "lhss [dst] [saturation] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light hsl saturation set\n\r",\
     user_cmd_light_hsl_saturation_set\
    },\
    {\
     "lhdg",\
     "lhdg [dst] [app_key_index]\n\r",\
     "light hsl default get\n\r",\
     user_cmd_light_hsl_default_get\
    },\
    {\
     "lhds",\
     "lhds [dst] [lightness] [hue] [saturation] [ack] [app_key_index]\n\r",\
     "light hsl default set\n\r",\
     user_cmd_light_hsl_default_set\
    },\
    {\
     "lhrg",\
     "lhrg [dst] [app_key_index]\n\r",\
     "light hsl range get\n\r",\
     user_cmd_light_hsl_range_get\
    },\
    {\
     "lhrs",\
     "lhrs [dst] [hue min] [hue max] [saturation min] [saturation max] [ack] [app_key_index]\n\r",\
     "light hsl range set\n\r",\
     user_cmd_light_hsl_range_set\
    },\
    {\
     "ts",\
     "ts [dst] [tai second low] [tai second high] [subsecond] [uncertainty] [time authority] [tai utc delta] [time zone offset] [app_key_index]\n\r",\
     "time set\n\r",\
     user_cmd_time_set\
    },\
    {\
     "tg",\
     "tg [dst] [app_key_index]\n\r",\
     "time get\n\r",\
     user_cmd_time_get\
    },\
    {\
     "tzs",\
     "tzs [dst] [time zone offset new] [tai of zone change low] [tai of zone change high] [app_key_index]\n\r",\
     "time zone set\n\r",\
     user_cmd_time_zone_set\
    },\
    {\
     "tzg",\
     "tzg [dst] [app_key_index]\n\r",\
     "time zone get\n\r",\
     user_cmd_time_zone_get\
    },\
    {\
     "ttuds",\
     "ttuds [dst] [tai utc delta new] [tai of delta change low] [tai of delta change high] [app_key_index]\n\r",\
     "time tai utc delta set\n\r",\
     user_cmd_time_tai_utc_delta_set\
    },\
    {\
     "ttudg",\
     "ttudg [dst] [app_key_index]\n\r",\
     "time tai utc delta get\n\r",\
     user_cmd_time_tai_utc_delta_get\
    },\
    {\
     "trs",\
     "trs [dst] [role] [app_key_index]\n\r",\
     "time role set\n\r",\
     user_cmd_time_role_set\
    },\
    {\
     "trg",\
     "trg [dst] [app_key_index]\n\r",\
     "time role get\n\r",\
     user_cmd_time_role_get\
    },\
    {\
     "ss",\
     "ss [dst] [scene number] [ack] [app_key_index]\n\r",\
     "scene store\n\r",\
     user_cmd_scene_store\
    },\
    {\
     "sr",\
     "sr [dst] [scene number] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "scene recall\n\r",\
     user_cmd_scene_recall\
    },\
    {\
     "sg",\
     "sg [dst] [app_key_index]\n\r",\
     "scene get\n\r",\
     user_cmd_scene_get\
    },\
    {\
     "srg",\
     "srg [dst] [app_key_index]\n\r",\
     "scene register get\n\r",\
     user_cmd_scene_register_get\
    },\
    {\
     "sd",\
     "sd [dst] [scene number] [ack] [app_key_index]\n\r",\
     "scene delete\n\r",\
     user_cmd_scene_delete\
    },\
    {\
     "sdg",\
     "sdg [dst] [property id] [app_key_index]\n\r",\
     "sensor descriptor get\n\r",\
     user_cmd_sensor_descriptor_get\
    },\
    {\
     "scg",\
     "scg [dst] [property id] [app_key_index]\n\r",\
     "sensor cadence get\n\r",\
     user_cmd_sensor_cadence_get\
    },\
    {\
     "scs",\
     "scs [dst] [property id] [divisor] [tirgger type] [trigger delta] [min interval] [fast cadence] [ack] [app_key_index]\n\r",\
     "sensor cadence set\n\r",\
     user_cmd_sensor_cadence_set\
    },\
    {\
     "sssg",\
     "sssg [dst] [property id] [app_key_index]\n\r",\
     "sensor settings get\n\r",\
     user_cmd_sensor_settings_get\
    },\
    {\
     "sss",\
     "sss [dst] [property id] [setting property id] [raw data] [ack] [app_key_index]\n\r",\
     "sensor setting set\n\r",\
     user_cmd_sensor_setting_set\
    },\
    {\
     "ssg",\
     "ssg [dst] [property id] [setting property id] [app_key_index]\n\r",\
     "sensor setting get\n\r",\
     user_cmd_sensor_setting_get\
    },\
    {\
     "seg",\
     "seg [dst] [property id] [app_key_index]\n\r",\
     "sensor get\n\r",\
     user_cmd_sensor_get\
    },\
    {\
     "scog",\
     "scog [dst] [property id] [raw value x] [app_key_index]\n\r",\
     "sensor column get\n\r",\
     user_cmd_sensor_column_get\
    },\
    {\
     "sseg",\
     "sseg [dst] [property id] [raw value xn] [app_key_index]\n\r",\
     "sensor series get\n\r",\
     user_cmd_sensor_series_get\
    },\
    {\
     "scheg",\
     "scheg [dst] [app_key_index]\n\r",\
     "scheduler get\n\r",\
     user_cmd_scheduler_get\
    },\
    {\
     "scheag",\
     "scheag [dst] [index] [app_key_index]\n\r",\
     "scheduler action get\n\r",\
     user_cmd_scheduler_action_get\
    },\
    {\
     "scheas",\
     "scheas [dst] [index] [year] [month] [day] [hour] [minute] [second] [day_of_week] [action] [num_steps] [step_resolution] [scene_number] [ack] [app_key_index]\n\r",\
     "scheduler action set\n\r",\
     user_cmd_scheduler_action_set\
    }
/** @} */


/**
 * @defgroup Client_Cmd_Exported_Functions Client Command Exported Functions
 * @brief
 * @{
 */
user_cmd_parse_result_t user_cmd_cmg(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_cms(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_gdtt_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_gdtt_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_health_fault_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_fault_clear(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_fault_test(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_period_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_period_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_attn_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_health_attn_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_on_off_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_on_off_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_level_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_level_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_delta_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_move_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_on_powerup_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_on_powerup_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_power_level_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_level_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_last_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_default_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_default_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_range_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_power_range_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_battery_get(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_light_lightness_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_linear_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_linear_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_last_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_default_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_default_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_range_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lightness_range_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_light_ctl_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_temperature_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_temperature_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_temperature_range_get(user_cmd_parse_value_t
                                                                 *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_temperature_range_set(user_cmd_parse_value_t
                                                                 *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_default_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_ctl_default_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_light_hsl_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_target_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_hue_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_hue_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_saturation_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_saturation_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_default_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_default_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_range_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_hsl_range_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_time_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_zone_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_zone_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_tai_utc_delta_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_tai_utc_delta_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_role_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_time_role_get(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_scene_store(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scene_recall(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scene_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scene_register_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scene_delete(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_sensor_descriptor_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_cadence_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_cadence_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_settings_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_setting_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_setting_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_column_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_sensor_series_get(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_scheduler_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scheduler_action_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_scheduler_action_set(user_cmd_parse_value_t *pparse_value);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _MESH_CMD_H */

