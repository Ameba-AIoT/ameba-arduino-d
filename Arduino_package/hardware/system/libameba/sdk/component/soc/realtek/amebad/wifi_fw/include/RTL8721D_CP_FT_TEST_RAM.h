#ifndef __RTL8721D_CP_FT_TEST_RAM_H__
#define __RTL8721D_CP_FT_TEST_RAM_H__

extern void
load_phy_reg(void);

extern void
load_radioa(void);

extern BOOLEAN DoTestItem(u32 TestItem);

extern VOID CP_GPIOInit(u32 GPIO_Pin, u8 Direction, u8 PuPd);

extern void CP_FT_TEST(void);
#endif