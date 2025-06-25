#ifndef _SCATTERLIST_H
#define _SCATTERLIST_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

struct scatterlist {
	unsigned long   sg_magic;
	unsigned long   page_link;
	unsigned int    offset;

	unsigned int    length;

	dma_addr_t      dma_address;
	__u32           dma_length;
};

struct sg_table {
	struct scatterlist *sgl;	/* the list */
	unsigned int nents;		/* number of mapped entries */
	unsigned int orig_nents;	/* original size of list */
};

#endif // CONFIG_USBH_MSC

#endif // _SCATTERLIST_H

