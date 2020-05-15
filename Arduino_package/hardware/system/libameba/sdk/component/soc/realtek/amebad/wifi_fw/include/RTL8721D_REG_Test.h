#ifndef __RTL8192F_REG_TEST_H__
#define __RTL8192F_REG_TEST_H__

extern void RegTest(
    const char *argv[]
)MEMMDL_LARGE;

extern void
RegDump(
    u16 stAddr,
    u16 endAddr,
    u8 type
)MEMMDL_LARGE;

#endif  //#ifndef __RTL8192F_REG_TEST_H__
