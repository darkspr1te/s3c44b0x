/*
 *  linux/arch/armnommu/mm/consistent.c
 *
 *  Copyright (C) 2004 Hyok S. Choi <hyok.choi@samsung.com>
 *
 *  Dynamic DMA mapping support.
 *
 *  We never have any address translations to worry about, so this
 *  is just alloc/free.
 */
                                                                                                                                           
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>

#include <asm/cacheflush.h>
#include <asm/io.h>
                                                                                                                                           
void *dma_alloc_coherent(struct device *dev, size_t size,
                           dma_addr_t *dma_handle, int gfp)
{
        void *ret;
        /* ignore region specifiers */
        gfp &= ~(__GFP_DMA | __GFP_HIGHMEM);
                                                                                                                                           
        if (dev == NULL || (*dev->dma_mask < 0xffffffff))
                gfp |= GFP_DMA;
        ret = (void *)__get_free_pages(gfp, get_order(size));
                                                                                                                                           
        if (ret != NULL) {
                memset(ret, 0, size);
                *dma_handle = virt_to_phys(ret);
        }
        return ret;
}
EXPORT_SYMBOL(dma_alloc_coherent);
                                                                                                                                           
void dma_free_coherent(struct device *dev, size_t size,
                         void *vaddr, dma_addr_t dma_handle)
{
        free_pages((unsigned long)vaddr, get_order(size));
}

EXPORT_SYMBOL(dma_free_coherent);

/*
 * Initialise the consistent memory allocation.
 */
static int __init consistent_init(void)
{
	return 0;
}

core_initcall(consistent_init);

/*
 * Make an area consistent for devices.
 */
void consistent_sync(void *vaddr, size_t size, int direction)
{
	unsigned long start = (unsigned long)vaddr;
	unsigned long end   = start + size;

	switch (direction) {
	case DMA_FROM_DEVICE:		/* invalidate only */
		dmac_inv_range(start, end);
		break;
	case DMA_TO_DEVICE:		/* writeback only */
		dmac_clean_range(start, end);
		break;
	case DMA_BIDIRECTIONAL:		/* writeback and invalidate */
		dmac_flush_range(start, end);
		break;
	default:
		BUG();
	}
}
EXPORT_SYMBOL(consistent_sync);
