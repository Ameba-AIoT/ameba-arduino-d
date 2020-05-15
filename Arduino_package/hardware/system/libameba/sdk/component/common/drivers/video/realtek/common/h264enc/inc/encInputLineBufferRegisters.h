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
--  Description :  Only for test purpose. Input line buffer registers.
--
------------------------------------------------------------------------------*/

#ifndef ENC_INPUTLINEBUFREGISTER_H
#define ENC_INPUTLINEBUFREGISTER_H

#include "basetype.h"
#include "encswhwregisters.h"

#if 0
slice_hw_mode_en
a)	[8:0] : slice_wr_cntr // +slice_depth when one slice is filled into slice_fifo
b)	[17:9]: slice_depth // unit is MB line
c)	[18]: slice_hw_mode_en // active high, enable bit of slice_fifo hardware mode, should be disabled before the start of next frame.
d)	[27:19] : pic_height // same value of swreg14[18:10] in H1
e)	[36:28] : slice_rd_cntr // read only
#endif

/* HW Register field names */
typedef enum {
    InputlineBufWrCntr,
    InputlineBufDepth,
    InputlineBufHwHandshake,
    InputlineBufPicHeight,
    InputlineBufRdCntr,
} lineBufRegName;

#define LINE_BUF_SWREG_AMOUNT   4 /*4x 32-bit*/

static const regField_s lineBufRegisterDesc[] = {
/* HW ID register, read-only */
    {InputlineBufWrCntr       , 0x000, 0x000001ff,  0, 0, RW, "slice_wr_cntr. +slice_depth when one slice is filled into slice_fifo"},
    {InputlineBufDepth        , 0x000, 0x0003fe00,  9, 0, RW, "slice_depth. unit is MB line"},
    {InputlineBufHwHandshake  , 0x000, 0x00040000, 18, 0, RW, "slice_hw_mode_en. active high. enable bit of slice_fifo hardware mode. should be disabled before the start of next frame."},
    {InputlineBufPicHeight    , 0x000, 0x0ff80000, 19, 0, RW, "pic_height. same value of swreg14[18:10] in H1."},
    {InputlineBufRdCntr       , 0x008, 0x000001ff,  0, 0, RO, "slice_rd_cntr. read only"},
};

#define GetInputLineBufReg(reg,name,value) \
    value=(reg[lineBufRegisterDesc[name].base/4]&lineBufRegisterDesc[name].mask)>>lineBufRegisterDesc[name].lsb

#define SetInputLineBufReg(reg,name,value) \
    reg[lineBufRegisterDesc[name].base/4]= \
        (reg[lineBufRegisterDesc[name].base/4] & ~(lineBufRegisterDesc[name].mask)) | \
        ((value << lineBufRegisterDesc[name].lsb) & lineBufRegisterDesc[name].mask)

#endif

