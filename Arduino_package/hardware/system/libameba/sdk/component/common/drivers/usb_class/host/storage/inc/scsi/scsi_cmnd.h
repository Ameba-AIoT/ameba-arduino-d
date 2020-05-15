#ifndef __SCSI_CMND_H_
#define __SCSI_CMND_H_

#include "us_usb.h"
#include "us_os_wrap_via_osdep_api.h"
#include <scsi/scsi.h>
#include <scsi/dma_direction.h>
#include <scatterlist/scatterlist.h>
/**
 * define flash block size
 */
#define BLOCK_SIZE					512

struct scsi_data_buffer {
	struct sg_table table;
	unsigned char *data_buffer; /* Data buffer to store read data */
	unsigned length;
	int resid;
};

struct scsi_cmnd{
	int result;		/* Status code from lower level driver */

	unsigned int channel,id,lun;
	enum dma_data_direction sc_data_direction;
	unsigned short cmd_len;
	unsigned length;
	_Sema cmnd_done;

	int eh_eflags;		/* Used by error handlr */

	struct scsi_data_buffer sdb;

	unsigned long sector;/* Sector address in LBA */
	unsigned int count;/* Number of sectors to read */
	void *request_buffer;

	 /* These elements define the operation we are about to perform */
#define MAX_COMMAND_SIZE	16
	unsigned char cmnd[MAX_COMMAND_SIZE];

#define SCSI_SENSE_BUFFERSIZE 	96
	unsigned char *sense_buffer;		/* obtained by REQUEST SENSE
						 * when CHECK CONDITION is
						 * received on original command
						 * (auto-sense) */

	/* Low-level done function - can be used by low-level driver to point
		 *        to completion function.  Not used by mid/upper level code. */
	void (*scsi_done) (struct scsi_cmnd *);

	unsigned underflow;	/* Return error if less than
						   this amount is transferred */
};

static inline unsigned scsi_sg_count(struct scsi_cmnd *cmd)
{
	return cmd->sdb.table.nents;
}

static inline struct scatterlist *scsi_sglist(struct scsi_cmnd *cmd)
{
	return cmd->sdb.table.sgl;
}

static inline void scsi_set_resid(struct scsi_cmnd *cmd, int resid)
{
	cmd->sdb.resid = resid;
}
//
static inline int scsi_get_resid(struct scsi_cmnd *cmd)
{
	return cmd->sdb.resid;
}
static inline unsigned scsi_bufflen(struct scsi_cmnd *cmd)
{
	return cmd->sdb.length;
}

extern int scsi_cmnd_execute(char cmnd, unsigned char * _buff,
		unsigned long _sector, unsigned int _count);

#endif

