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
#include "mesh_data_uart.h"
#include "mesh_user_cmd_parse.h"
#include "mesh_api.h"
#include "blob_transfer.h"
#include "firmware_update.h"
#include "firmware_distribution.h"

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
    },\
    {\
     "glgg",\
     "glgg [dst] [app key index]\n\r",\
     "generic location global get\n\r",\
     user_cmd_generic_location_global_get\
    },\
    {\
     "glgs",\
     "glgs [dst] [latitude] [longitude] [altitude] [ack] [app key index]\n\r",\
     "generic location global set\n\r",\
     user_cmd_generic_location_global_set\
    },\
    {\
     "gllg",\
     "gllg [dst] [app key index]\n\r",\
     "generic location local get\n\r",\
     user_cmd_generic_location_local_get\
    },\
    {\
     "glls",\
     "glls [dst] [north] [east] [altitude] [floor] [sationary] [update time] [precision] [ack] [app key index]\n\r",\
     "generic location local set\n\r",\
     user_cmd_generic_location_local_set\
    },\
    {\
     "gupsg",\
     "gupsg [dst] [app key index]\n\r",\
     "generic user properties get\n\r",\
     user_cmd_generic_user_properties_get\
    },\
    {\
     "gupg",\
     "gupg [dst] [property id] [app key index]\n\r",\
     "generic user property get\n\r",\
     user_cmd_generic_user_property_get\
    },\
    {\
     "gups",\
     "gups [dst] [app key index] [ack] [property id] [property value...] \n\r",\
     "generic user property set\n\r",\
     user_cmd_generic_user_property_set\
    },\
    {\
     "gapsg",\
     "gapsg [dst] [app key index]\n\r",\
     "generic admin properties get\n\r",\
     user_cmd_generic_admin_properties_get\
    },\
    {\
     "gapg",\
     "gapg [dst] [property id] [app key index]\n\r",\
     "generic admin property get\n\r",\
     user_cmd_generic_admin_property_get\
    },\
    {\
     "gaps",\
     "gaps [dst] [app key index] [ack] [property id] [property access] [property value...]\n\r",\
     "generic admin property set\n\r",\
     user_cmd_generic_admin_property_set\
    },\
    {\
     "gmpsg",\
     "gmpsg [dst] [app key index]\n\r",\
     "generic manufacturer properties get\n\r",\
     user_cmd_generic_manufacturer_properties_get\
    },\
    {\
     "gmpg",\
     "gmpg [dst] [property id] [app key index]\n\r",\
     "generic manufacturer property get\n\r",\
     user_cmd_generic_manufacturer_property_get\
    },\
    {\
     "gmps",\
     "gmps [dst] [app key index] [ack] [property id] [property access]\n\r",\
     "generic manufacturer property set\n\r",\
     user_cmd_generic_manufacturer_property_set\
    },\
    {\
     "gcpsg",\
     "gcpsg [dst] [app key index] [property id]\n\r",\
     "generic client properties get\n\r",\
     user_cmd_generic_client_properties_get\
    },\
    {\
     "lxg",\
     "lxg [dst] [app_key_index]\n\r",\
     "light xyl get\n\r",\
     user_cmd_light_xyl_get\
    },\
    {\
     "lxs",\
     "lxs [dst] [lightness] [xyl_x] [xyl_y] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light xyl set\n\r",\
     user_cmd_light_xyl_set\
    },\
    {\
     "lxtg",\
     "lxtg [dst] [app_key_index]\n\r",\
     "light xyl target get\n\r",\
     user_cmd_light_xyl_target_get\
    },\
    {\
     "lxdg",\
     "lxdg [dst] [app_key_index]\n\r",\
     "light xyl default get\n\r",\
     user_cmd_light_xyl_default_get\
    },\
    {\
     "lxds",\
     "lxds [dst] [lightness] [xyl_x] [xyl_y] [ack] [app_key_index]\n\r",\
     "light xyl default set\n\r",\
     user_cmd_light_xyl_default_set\
    },\
    {\
     "lxrg",\
     "lxrg [dst] [app_key_index]\n\r",\
     "light xyl range get\n\r",\
     user_cmd_light_xyl_range_get\
    },\
    {\
     "lxrs",\
     "lxrs [dst] [xyl_x min] [xyl_x max] [xyl_y min] [xyl_y max] [ack] [app_key_index]\n\r",\
     "light xyl range set\n\r",\
     user_cmd_light_xyl_range_set\
    },\
    {\
     "llcmg",\
     "llcmg [dst] [app_key_index]\n\r",\
     "light lc mode get\n\r",\
     user_cmd_light_lc_mode_get\
    },\
    {\
     "llcms",\
     "llcms [dst] [mode] [app_key_index] [ack]\n\r",\
     "light lc mode set\n\r",\
     user_cmd_light_lc_mode_set\
    },\
    {\
     "llcomg",\
     "llcomg [dst] [app_key_index]\n\r",\
     "light lc om get\n\r",\
     user_cmd_light_lc_om_get\
    },\
    {\
     "llcoms",\
     "llcoms [dst] [mode] [app_key_index] [ack]\n\r",\
     "light lc om set\n\r",\
     user_cmd_light_lc_om_set\
    },\
    {\
     "llcloog",\
     "llcloog [dst] [app_key_index]\n\r",\
     "light lc light on off get\n\r",\
     user_cmd_light_lc_light_on_off_get\
    },\
    {\
     "llcloos",\
     "llcloos [dst] [on off] [ack] [app_key_index] [steps] [resolution] [delay]\n\r",\
     "light lc light on off set\n\r",\
     user_cmd_light_lc_light_on_off_set\
    },\
    {\
     "llcpg",\
     "llcpg [dst] [property id] [app_key_index]\n\r",\
     "light lc property get\n\r",\
     user_cmd_light_lc_property_get\
    },\
    {\
     "llcps",\
     "llcps [dst] [app_key_index] [ack] [property id] [property value...] \n\r",\
     "light lc property set\n\r",\
     user_cmd_light_lc_property_set\
    },\
    {\
     "prbg",\
     "prbg [dst]\n\r",\
     "Private beacon get\n\r",\
     user_cmd_private_beacon_get\
    },\
    {\
     "prbs",\
     "prbs [dst] [enable/disable] [random_update_interval]\n\r",\
     "Private beacon set\n\r",\
     user_cmd_private_beacon_set\
    },\
    {\
     "pgpg",\
     "pgpg [dst]\n\r",\
     "Private gatt proxy get\n\r",\
     user_cmd_private_gatt_proxy_get\
    },\
    {\
     "pgps",\
     "pgps [dst] [state]\n\r",\
     "Private gatt proxy set\n\r",\
     user_cmd_private_gatt_proxy_set\
    },\
    {\
     "pnig",\
     "pnig [dst] [net_key_index]\n\r",\
     "Private node identity get\n\r",\
     user_cmd_private_node_identity_get\
    },\
    {\
     "pnis",\
     "pnis [dst] [net_key_index] [state]\n\r",\
     "Private node identity set\n\r",\
     user_cmd_private_node_identity_set\
    },\
    {\
     "btg",\
     "btg [dst] [app_key_index]\n\r",\
     "blob transfer get\n\r",\
     user_cmd_blob_transfer_get\
    },\
    {\
     "bts",\
     "bts [dst] [app_key_index] [mode] [object id] [object size] [block size log] [client mtu size]\n\r",\
     "blob transfer start\n\r",\
     user_cmd_blob_transfer_start\
    },\
    {\
     "btc",\
     "btc [dst] [app_key_index] [object id]\n\r",\
     "blob transfer cancel\n\r",\
     user_cmd_blob_transfer_cancel\
    },\
    {\
     "bbg",\
     "bbg [dst] [app_key_index]\n\r",\
     "blob block get\n\r",\
     user_cmd_blob_block_get\
    },\
    {\
     "bbs",\
     "bbs [dst] [app_key_index] [block num] [chunk size]\n\r",\
     "blob block start\n\r",\
     user_cmd_blob_block_start\
    },\
    {\
     "bct",\
     "bct [dst] [app_key_index] [chunk num] [chunk size]\n\r",\
     "blob chunk transfer\n\r",\
     user_cmd_blob_chunk_transfer\
    },\
    {\
     "big",\
     "big [dst] [app_key_index]\n\r",\
     "blob information get\n\r",\
     user_cmd_blob_info_get\
    },\
    {\
     "fuig",\
     "fuig [dst] [app_key_index] [first_index] [entries_limit]\n\r",\
     "firmware update information get\n\r",\
     user_cmd_fw_update_info_get\
    },\
    {\
     "fumc",\
     "fumc [dst] [app_key_index] [fw_image_idx] [fw_metadata] [metadata len]\n\r",\
     "firmware update metadata check\n\r",\
     user_cmd_fw_update_metadata_check\
    },\
    {\
     "fug",\
     "fug [dst] [app_key_index]\n\r",\
     "firmware update get\n\r",\
     user_cmd_fw_update_get\
    },\
    {\
     "fus",\
     "fus [dst] [app_key_index] [update ttl] [update timeout base] [blob id] [fw image idx] [fw metadata] [metadata len]\n\r",\
     "firmware update start\n\r",\
     user_cmd_fw_update_start\
    },\
    {\
     "fuc",\
     "fuc [dst] [app_key_index]\n\r",\
     "firmware update cancel\n\r",\
     user_cmd_fw_update_cancel\
    },\
    {\
     "fua",\
     "fua [dst] [app_key_index]\n\r",\
     "firmware update apply\n\r",\
     user_cmd_fw_update_apply\
    },\
    {\
     "fdra",\
     "fdra [dst] [app_key_index] [address] [update fw index]\n\r",\
     "firmware distribution receivers add\n\r",\
     user_cmd_fw_dist_recvs_add\
    },\
    {\
     "fdrda",\
     "fdrda [dst] [app_key_index]\n\r",\
     "firmware distribution receivers delete all\n\r",\
     user_cmd_fw_dist_recvs_delete_all\
    },\
    {\
     "fdrg",\
     "fdrg [dst] [app_key_index] [first_index] [entries_limit]\n\r",\
     "firmware distribution receivers get\n\r",\
     user_cmd_fw_dist_recvs_get\
    },\
    {\
     "fdcg",\
     "fdcg [dst] [app_key_index]\n\r",\
     "firmware distribution capabilites get\n\r",\
     user_cmd_fw_dist_caps_get\
    },\
    {\
     "fdg",\
     "fdg [dst] [app_key_index]\n\r",\
     "firmware distribution get\n\r",\
     user_cmd_fw_dist_get\
    },\
    {\
     "fds",\
     "fds [dst] [app_key_index] [dist_app_key_index] [dist_ttl] [dist_timeout_base] [dist_transfer_mode] [update_policy] [dist_fw_image_idx] [dist_multicast_addr] [dist_dst_len]\n\r",\
     "firmware distribution start\n\r",\
     user_cmd_fw_dist_start\
    },\
    {\
     "fdc",\
     "fdc [dst] [app_key_index]\n\r",\
     "firmware distribution cancel\n\r",\
     user_cmd_fw_dist_cancel\
    },\
    {\
     "fda",\
     "fua [dst] [app_key_index]\n\r",\
     "firmware distribution apply\n\r",\
     user_cmd_fw_dist_apply\
    },\
    {\
     "fdug",\
     "fdug [dst] [app_key_index]\n\r",\
     "firmware distribution upload get\n\r",\
     user_cmd_fw_dist_upload_get\
    },\
    {\
     "fdus",\
     "fdus [dst] [app_key_index] [upload_ttl] [upload_timeout_base] [blob_id] [upload_fw_size] [metadata] [metadata_len] [fw_id] [fw_id_len]\n\r",\
     "firmware distribution upload start\n\r",\
     user_cmd_user_cmd_fw_dist_upload_start\
    },\
    {\
     "fduos",\
     "fduos [dst] [app_key_index] [upload_uri] [upload_uri_len] [fw_id] [fw_id_len]\n\r",\
     "firmware distribution upload oob start\n\r",\
     user_cmd_fw_dist_upload_oob_start\
    },\
    {\
     "fduc",\
     "fduc [dst] [app_key_index]\n\r",\
     "firmware distribution upload cancel\n\r",\
     user_cmd_fw_dist_upload_cancel\
    },\
    {\
     "fdfg",\
     "fdfg [dst] [app_key_index] [fw_id] [fw_id_len]\n\r",\
     "firmware distribution firmware get\n\r",\
     user_cmd_fw_dist_fw_get\
    },\
    {\
     "fdgbi",\
     "fdgbi [dst] [app_key_index] [dist_fw_image_index]\n\r",\
     "firmware distribution get by index\n\r",\
     user_cmd_fw_dist_fw_get_by_index\
    },\
    {\
     "fdfd",\
     "fdfd [dst] [app_key_index] [fw_id] [fw_id_len]\n\r",\
     "firmware distribution firmware delete\n\r",\
     user_cmd_fw_dist_fw_delete\
    },\
    {\
     "fdfda",\
     "fdfda [dst] [app_key_index]\n\r",\
     "firmware distribution firmware delete all\n\r",\
     user_cmd_fw_dist_fw_delete_all\
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

user_cmd_parse_result_t user_cmd_generic_location_global_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_location_global_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_location_local_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_location_local_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_generic_user_properties_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_user_property_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_user_property_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_admin_properties_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_admin_property_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_admin_property_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_generic_manufacturer_properties_get(user_cmd_parse_value_t
                                                                     *pparse_value);
user_cmd_parse_result_t user_cmd_generic_manufacturer_property_get(user_cmd_parse_value_t
                                                                   *pparse_value);
user_cmd_parse_result_t user_cmd_generic_manufacturer_property_set(user_cmd_parse_value_t
                                                                   *pparse_value);
user_cmd_parse_result_t user_cmd_generic_client_properties_get(user_cmd_parse_value_t
                                                               *pparse_value);

user_cmd_parse_result_t user_cmd_light_xyl_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_target_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_default_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_default_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_range_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_xyl_range_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_light_lc_mode_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_mode_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_om_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_om_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_light_on_off_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_light_on_off_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_property_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_light_lc_property_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_private_beacon_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_private_beacon_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_private_gatt_proxy_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_private_gatt_proxy_set(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_private_node_identity_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_private_node_identity_set(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_blob_transfer_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_transfer_start(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_transfer_cancel(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_block_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_block_start(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_chunk_transfer(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_blob_info_get(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_fw_update_info_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_update_metadata_check(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_update_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_update_start(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_update_cancel(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_update_apply(user_cmd_parse_value_t *pparse_value);

user_cmd_parse_result_t user_cmd_fw_dist_recvs_add(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_recvs_delete_all(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_recvs_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_caps_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_start(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_cancel(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_apply(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_upload_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_user_cmd_fw_dist_upload_start(user_cmd_parse_value_t
                                                               *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_upload_oob_start(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_upload_cancel(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_fw_get(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_fw_get_by_index(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_fw_delete(user_cmd_parse_value_t *pparse_value);
user_cmd_parse_result_t user_cmd_fw_dist_fw_delete_all(user_cmd_parse_value_t *pparse_value);


/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _MESH_CMD_H */

