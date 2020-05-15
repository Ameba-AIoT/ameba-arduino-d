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


#define OVERRIDE_LAYOUT   0x010000
#define OVERRIDE_BUFSIZE  0x020000
#define OVERRIDE_PAGESIZE 0x040000

#define LAYOUT_OVERRIDE_BUFFER ((char*)theFlashParams.buffer)
#define SET_BUFSIZE_OVERRIDE(new_size) theFlashParams.block_size = (new_size)
#define SET_PAGESIZE_OVERRIDE(new_size) theFlashParams.offset_into_block = (new_size)

// parameter passing structure
typedef struct {
  uint32_t base_ptr;
  uint32_t count;
  uint32_t offset_into_block;
  void *buffer;
  uint32_t block_size;
} FlashParamsHolder;

typedef struct {
  uint32_t start;
  uint32_t length;
} FlashEraseData;

extern FlashParamsHolder theFlashParams;
extern char FlashBufferStart;
extern char FlashBufferEnd;
