//------------------------------------------------------------------------------
//
// Copyright (c) 2008-2015 IAR Systems
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// $Revision: 38952 $
//
//------------------------------------------------------------------------------

#include "flash_config.h"
#include <stdint.h>

#define RESULT_OK                   0
#define RESULT_ERROR                1
#define RESULT_OVERRIDE_DEVICE      2
#define RESULT_ERASE_DONE           3
#define RESULT_CUSTOM_FIRST         100
#define RESULT_CUSTOM_LAST          200

#define FLAG_ERASE_ONLY             0x1

#ifndef CODE_ADDR_AS_VOID_PTR
#define CODE_ADDR_AS_VOID_PTR 1
#endif

// These are functions you MUST implement -------------------------------
#if CODE_ADDR_AS_VOID_PTR

#if USE_ARGC_ARGV
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[]);
#else
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags);
#endif

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer);

uint32_t FlashErase(void *block_start,
                    uint32_t block_size);

#else  // !CODE_ADDR_AS_VOID_PTR

#if USE_ARGC_ARGV
uint32_t FlashInit(uint32_t base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[]);
#else
uint32_t FlashInit(uint32_t base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags);
#endif

uint32_t FlashWrite(uint32_t block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer);

uint32_t FlashErase(uint32_t block_start,
                    uint32_t block_size);

#endif // CODE_ADDR_AS_VOID_PTR

// These are functions you MAY implement --------------------------------

#if CODE_ADDR_AS_VOID_PTR
uint32_t FlashChecksum(void const *begin, uint32_t count);
#else
uint32_t FlashChecksum(uint32_t begin, uint32_t count);
#endif

uint32_t FlashSignoff(void);

#define OPTIONAL_CHECKSUM _Pragma("required=FlashChecksumEntry") __root
#define OPTIONAL_SIGNOFF  _Pragma("required=FlashSignoffEntry") __root
void FlashChecksumEntry();
void FlashSignoffEntry();

// These are functions you may call -------------------------------------

// If your code cannot be accessed using data pointers, you will have to
// write your own Crc16 function.
uint16_t Crc16(uint8_t const *p, uint32_t len);
