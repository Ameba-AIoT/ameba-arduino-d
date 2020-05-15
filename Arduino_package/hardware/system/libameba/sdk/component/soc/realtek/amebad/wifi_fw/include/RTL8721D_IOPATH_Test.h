#ifndef __RTL8192F_IOPATH_TEST_H__
#define __RTL8192F_IOPATH_TEST_H__

#if CONFIG_IOPATH_TEST
/*--------------------------Define -------------------------------------------*/
#define IOPATH_BB_DBG_REG           0x0928
#define IOPATH_BB_DBG_REG1           0x092A
#define IOPATH_RF_DBGREG_A           0x18 //RF test REG
#define IOPATH_DIRECT_RF_DBGREG_A           0x2860 //RF A test REG
#define IOPATH_RF_DBGREG_B           0x18 //RF test REG
#define IOPATH_DIRECT_RF_DBGREG_B           0x2C60 //RF B test REG
#define IOPATH_CPU_DBGREG           0xFD9F
#define IOPATH_SIE_INDIRECT_DBGREG           0x38//0xFE38
#define IOPATH_SIE_DIRECT_DBGREG           0xFE38
#define IOPATH_UPHY_INDIRECT_DBGREG     0xF4
#define IOPATH_PCIEMIOINTF_DBGREG           0x98 //IOREG 0x398
#define IOPATH_PCIE_PHY_DBGREG           0x10
#define IOPATH_PCIE_CONFIG_DBGREG          0x2C
//#define IOPATH_SPIC_DBGREG           0x0014
#define IOPATH_SPIC_DBGREG           0x011C
#define SPIC_REG_EN           0x0008
#define IOPATH_TXBUF_ADDR_H     0xFE
#define IOPATH_TXBUF_ADDR_L     0x00
#define IOPATH_RXBUF_ADDR_H     0x3E
#define IOPATH_RXBUF_ADDR_L     0x00
#define IOPATH_TXRPT_ENTRY      0x10
#define IOPATH_TXRPT_OFFSET     0x04
#define SECCAM_ENTRY_NUM        32    //8721D entry num is 32
#define SECCAM_INDIRECT_ENTRY_SIZE                32
#define SECCAM_DIRECT_ENTRY_SIZE                  32
#define SECCAM_START_ADDR        0x1A00  //END AT 0X1FFF
#define BACAM_ENTRY_NUM       32
#define BACAM_ENTRY_SIZE                  16
#define BACAM_START_ADDR        0x1800  //END AT 0X19FF
#define VDRCTL              0xFE40
#define VSTAIN              0xFE41
#define VLOADM              0xFE42
#define VSTAOUT             0xFE43

/*------------------------------Define Enum-----------------------------------*/

/*--------------------------Define MACRO--------------------------------------*/


/*------------------------------Define Struct---------------------------------*/


/*------------------------Export global variable------------------------------*/


/*------------------------------Function declaration--------------------------*/

extern void 
IOPATHTest(
    IN  u8    *argv[]
)MEMMDL_LARGE;
#endif
#endif  //__RTL8192F_INT_TEST_H__