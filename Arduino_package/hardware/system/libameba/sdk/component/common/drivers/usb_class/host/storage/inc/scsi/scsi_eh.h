#ifndef _SCSI_EH_H_
#define _SCSI_EH_H_

#include "scsi/scsi_cmnd.h"
#include "dma_direction.h"

#define BLK_MAX_CDB	16
/*
 * This is a slightly modified SCSI sense "descriptor" format header.
 * The addition is to allow the 0x70 and 0x71 response codes. The idea
 * is to place the salient data from either "fixed" or "descriptor" sense
 * format into one structure to ease application processing.
 *
 * The original sense buffer should be kept around for those cases
 * in which more information is required (e.g. the LBA of a MEDIUM ERROR).
 */
struct scsi_sense_hdr {		/* See SPC-3 section 4.5 */
	u8 response_code;	/* permit: 0x0, 0x70, 0x71, 0x72, 0x73 */
	u8 sense_key;
	u8 asc;
	u8 ascq;
	u8 byte4;
	u8 byte5;
	u8 byte6;
	u8 additional_length;	/* always 0 for fixed sense format */
};

static inline int scsi_sense_valid(struct scsi_sense_hdr *sshdr)
{
	if (!sshdr)
		return 0;
	return (sshdr->response_code & 0x70) == 0x70;
}

struct scsi_eh_save {
	/* saved state */
	int result;
	enum dma_data_direction data_direction;
	unsigned underflow;
	unsigned char cmd_len;
//	unsigned char prot_op;
	unsigned char cmnd[BLK_MAX_CDB];
	struct scsi_data_buffer sdb;
//	struct request *next_rq;
	/* new command support */
	unsigned char eh_cmnd[BLK_MAX_CDB];
//	struct scatterlist sense_sgl;
};
const u8 * scsi_sense_desc_find(const u8 * sense_buffer, int sb_len,
				int desc_type);
#endif

