/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     model_property.h
* @brief    Head file for model property.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-9-3
* @version  v1.0
* *************************************************************************************
*/

#ifndef _MODEL_PROPERTY_H_
#define _MODEL_PROPERTY_H_
/**
 * @addtogroup MODEL_PROPERTY
 * @{
 */

/**
 * @defgroup MODEL_PROPERYTY_MACROS Model Property Macros
 * @brief
 * @{
 */
#define MODEL_PROPERTY_INVALID                                    0x00

#define MODEL_PROPERTY_LIGHT_CONTORL_AMBIENT_LUXLEVEL_ON          0x002B
#define MODEL_PROPERTY_LIGHT_CONTORL_AMBIENT_LUXLEVEL_PROLONG     0x002C
#define MODEL_PROPERTY_LIGHT_CONTORL_AMBIENT_LUXLEVEL_STANDBY     0x002D
#define MODEL_PROPERTY_LIGHT_CONTORL_LIGHTNESS_ON                 0x002E
#define MODEL_PROPERTY_LIGHT_CONTORL_LIGHTNESS_PROLONG            0x002F
#define MODEL_PROPERTY_LIGHT_CONTORL_LIGHTNESS_STANDBY            0x0030
#define MODEL_PROPERTY_LIGHT_CONTORL_REGULATOR_ACCURACY           0x0031
#define MODEL_PROPERTY_LIGHT_CONTORL_REGULATOR_KID                0x0032
#define MODEL_PROPERTY_LIGHT_CONTORL_REGULATOR_KIU                0x0033
#define MODEL_PROPERTY_LIGHT_CONTORL_REGULATOR_KPD                0x0034
#define MODEL_PROPERTY_LIGHT_CONTORL_REGULATOR_KPU                0x0035
#define MODEL_PROPERTY_LIGHT_CONTORL_TIME_FADE                    0x0036
#define MODEL_PROPERTY_LIGHT_CONTROL_TIME_FADE_ON                 0x0037
#define MODEL_PROPERTY_LIGHT_CONTORL_TIME_FADE_STANDBY_AUDIO      0x0038
#define MODEL_PROPERTY_LIGHT_CONTORL_TIME_FADE_STANDBY_MANUAL     0x0039
#define MODEL_PROPERTY_LIGHT_CONTROL_TIME_OCCUPANCY_DELAY         0x003A
#define MODEL_PROPERTY_LIGHT_CONTORL_TIME_PROLONG                 0x003B
#define MODEL_PROPERTY_LIGHT_CONTROL_TIME_RUN_ON                  0x003C
#define MODEL_PROPERTY_MOTION_SENSED                              0x0042
#define MODEL_PROPERTY_PEOPLE_COUNT                               0x004C
#define MODEL_PROPERTY_PRESENCE_DETECTED                          0x004D
#define MODEL_PROPERTY_PRESENT_AMBIENT_LIGHT_LEVEL                0x004E
#define MODEL_PROPERTY_TIME_SINCE_MOTION_SENSED                   0x0068
/** @} */
/** @} */

#endif /* _MODEL_PROPERTY_H_ */


