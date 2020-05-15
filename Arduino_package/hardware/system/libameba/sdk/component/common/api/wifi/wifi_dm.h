/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/ 
#ifndef _WIFI_DM_H
#define _WIFI_DM_H

#ifdef	__cplusplus
extern "C" {
#endif

enum{
	RATE_BITMAP_CCK_1M = 0,
	RATE_BITMAP_CCK_2M,
	RATE_BITMAP_CCK_5_5M,
	RATE_BITMAP_CCK_11M,
	RATE_BITMAP_OFDM_6M,
	RATE_BITMAP_OFDM_9M,
	RATE_BITMAP_OFDM_12M,
	RATE_BITMAP_OFDM_18M,
	RATE_BITMAP_OFDM_24M,
	RATE_BITMAP_OFDM_36M,
	RATE_BITMAP_OFDM_48M,
	RATE_BITMAP_OFDM_54M,
	RATE_BITMAP_HT_MCS0,
	RATE_BITMAP_HT_MCS1,
	RATE_BITMAP_HT_MCS2,
	RATE_BITMAP_HT_MCS3,
	RATE_BITMAP_HT_MCS4,
	RATE_BITMAP_HT_MCS5,
	RATE_BITMAP_HT_MCS6,
	RATE_BITMAP_HT_MCS7,
};

// rate_bitmap :
// bit0: CCK 1M , bit1: CCK 2M , bit2: CCK 5.5M , bit3: CCK 11M
// bit4: OFDM 6M , bit5: OFDM 9M , bit6: OFDM 12M , bit7: OFDM 18M , bit8: OFDM 24M , bit9: OFDM 36M , bit10: OFDM 48M , bit11: OFDM 54M
// bit12: MCS0 , bit13: MCS1 , bit14: MCS2 , bit15: MCS3 , bit16: MCS4 , bit17: MCS5 , bit18: MCS6 , bit19: MCS7
int wext_dm_set_rate_bitmap(unsigned int rate_bitmap);

// rate_fallback_1_4 : 
// byte0: degrade rate on 1st  retry , byte1: degrade rate on 2nd retry , byte2: degrade rate on  3rd retry , byte3: degrade rate on 4th retry
// rate_fallback_5_8 : 
// byte0: degrade rate on 5th retry , byte1: degrade rate on 6th retry , byte2: degrade rate on 7th retry , byte3: degrade rate on 8th retry
int wext_dm_set_auto_rate_fallback(unsigned int rate_fallback_1_4, unsigned int rate_fallback_5_8);

// retry_limit : 
// packet retry limit count
int wext_dm_set_retry_limit(unsigned char retry_limit);

#ifdef	__cplusplus
}
#endif

#endif /* _WIFI_DM_H */
