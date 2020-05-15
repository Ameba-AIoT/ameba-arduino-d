/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Hantro Products Oy.                             --
--                                                                            --
--                   (C) COPYRIGHT 2006 HANTRO PRODUCTS OY                    --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
--------------------------------------------------------------------------------
--
--  Description : Locking semaphore for hardware sharing
--
------------------------------------------------------------------------------*/

#ifndef __EWL_LINUX_LOCK_H__
#define __EWL_LINUX_LOCK_H__

int binary_semaphore_allocation(void *key, int nsem, int sem_flags);
int binary_semaphore_deallocate(int semid);
int binary_semaphore_wait(int semid, int sem_num);
int binary_semaphore_post(int semid, int sem_num);
int binary_semaphore_initialize(int semid, int sem_num);

#endif /* __EWL_LINUX_LOCK_H__ */
