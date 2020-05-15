#ifndef _DMA_DIRECTION_H
#define _DMA_DIRECTION_H

#include <platform_opts.h>

#ifdef CONFIG_USBH_MSC

enum dma_data_direction {
         DMA_BIDIRECTIONAL = 0,
         DMA_TO_DEVICE = 1,
         DMA_FROM_DEVICE = 2,
         DMA_NONE = 3,
};

#endif // CONFIG_USBH_MSC

#endif // _DMA_DIRECTION_H

