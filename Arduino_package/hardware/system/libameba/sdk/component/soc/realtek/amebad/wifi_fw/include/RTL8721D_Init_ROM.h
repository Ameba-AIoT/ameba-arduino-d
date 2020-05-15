#ifndef __RTL8721D_INIT_H__
#define __RTL8721D_INIT_H__


/*--------------------------Define -------------------------------------------*/

#ifndef __ASSEMBLY__

extern MEMTYPE_BDATA   u8  SharedINTCtrl;
//Interrupt Jump Decision sbit

#else

#endif  //#ifndef __ASSEMBLY__

#define WAIT_FD_LOCK_BREAK_CNT  (5000)
#define WAIT_FD_LOCK_CNT        (50)
#define WAIT_FD_LOCK_ONCE_TIME  (20)





/*------------------------------Define Enum-----------------------------------*/

/*--------------------------Define MACRO--------------------------------------*/

/*------------------------------Define Struct---------------------------------*/

/*------------------------Export global variable------------------------------*/

/*------------------------------Function declaration--------------------------*/

#ifndef __ASSEMBLY__

extern void
ClrAllFWUsedIMR_8721D_ROM(
    void
);

extern void
ClrAllFWUsedISR_8721D_ROM(
    void
);

#endif  //#ifndef __ASSEMBLY__

#endif  //__RTL8721D_INIT_H__

