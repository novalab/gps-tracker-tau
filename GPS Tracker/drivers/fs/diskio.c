/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "msp430f2618.h"

#include "diskio.h"
#include "MMC.h"
#include "hw_conf.h"

void sd_hw_init(void)
{
  // Chip Select / Slave select
  // - not using HW STE since it's timing does not fit sd uses
  SD_CS_DIR |= SD_CS_BIT; // set as output
  sd_cs_high();

  sd_spi_init();
}

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define ATA		0
#define MMC		1
#define USB		2



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
  sd_hw_init();
  
  if (mmcInit() == MMC_SUCCESS)
    return RES_OK;
    
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
  // TODO: implement
  return RES_OK;
  /*
  result = MMC_disk_status();
  
  return stat;
  
	return STA_NOINIT;
  */
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	int result;

  // if more than one sector is required, call read multiple times.  
  while (count-- > 0)
  {
    result = mmcReadSector(sector++, buff);
    if (result != MMC_SUCCESS)
      return RES_ERROR;
    buff += SECTOR_SIZE;
  }
  return RES_OK;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	int result;
  
  if (count == 0)
    return RES_OK;
  
  // if more than one sector is required, call write multiple times.  
  while (count-- > 0)
  {
    result = mmcWriteSector(sector++, (unsigned char*)buff);
    if (result != MMC_SUCCESS)
      return RES_ERROR;
    buff += SECTOR_SIZE;
  }
  return RES_OK;

}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  return RES_OK;
}


/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/

DWORD get_fattime (void)
{
	return	  ((DWORD)(2012 - 1980) << 25)	/* Year = 2012 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
}
