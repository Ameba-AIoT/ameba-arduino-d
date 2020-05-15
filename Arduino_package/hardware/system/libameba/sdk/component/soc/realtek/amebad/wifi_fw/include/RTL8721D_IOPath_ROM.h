#ifndef __RTL8721D_IOPATH_H__
#define __RTL8721D_IOPATH_H__

#ifndef __ASSEMBLY__

/*--------------------Define --------------------------------------------*/

#define IOPATH_CMD_READ             0
#define IOPATH_CMD_WRITE            1
#define POLL_U3PHY_CNT              5000



//DBI Cmd
#define DBI_WRITE_MASK              3
#define PCIE_DBI_WRITE_EN           0x0001F000
#define PCIE_DBI_READ_EN            0x00020000

//PCIE MDIO
#define PCIE_MDIO_WRITE_EN          0x20
#define PCIE_MDIO_READ_EN           0x40    

//PCIE MIO INTF
#define PCIE_W4B_EN                 0x0F
#define PCIE_WRITE_MASK             0x0F



//USB SIE INTF
#define USB_TYPE_2_0                0
#define USB_TYPE_3_0                1

#define U2SIE_PHY_W_EN              0x81

#define U2SIEADDR_PHY_DATA          0x41
#define U2SIEADDR_PHY_ADDR          0x40
#define U2SIEADDR_PHY_CTRL          0x42
#define U2SIEADDR_PHY_RDATA         0x43


#define U2_PHY_ADDR_WRITE_EN        BIT5


#define U3SIEADDR_PHY_CTRL          0x55
#define U3SIEADDR_PHY_DATA          0x56

#define U3_PHY_MDIO_READ            BIT6
#define U3_PHY_MDIO_WRITE           BIT7

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


extern BOOLEAN
DBI_PCIE_CONF_SPACE(
    IN u8   rwcmd,
    IN u8   *u8addr,
    IN u8   *u8data         
);

extern BOOLEAN
MDIO_PCIE_PHY(
    IN  u8  rwcmd,
    IN  u8  addr,
    IN  u8  *u8data  
);

BOOLEAN
PCIE_MIO_INTF(
    IN  u8  rwcmd,
    IN  u8  addr,
    IN  u8  wren,
    IN  u8  *u8data
);


extern BOOLEAN
USB_SIE_INTF_WRITE(
    IN  u8  uphy_type,
    IN  u8  addr,
    IN  u8  wdata   
);

extern BOOLEAN
USB_SIE_INTF_READ(
    IN  u8  uphy_type,
    IN  u8  addr,
    OUT u8  *rdata 
);

extern BOOLEAN
USB_PHY_CMD_READ(
    IN  u8  uphy_type,
    IN  u8  addr,
    OUT u8  *rdata 
);

extern BOOLEAN
USB_PHY_CMD_WRITE(
    IN  u8  uphy_type,
    IN  u8  addr,
    IN  u8  *wdata  
);

#endif  //#ifndef __ASSEMBLY__

#endif  //__RTL8721D_IOPATH_H__

