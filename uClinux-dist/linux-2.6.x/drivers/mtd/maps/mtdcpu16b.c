/*
 *
 * Normal mappings of chips in physical memory
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/reboot.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nftl.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/cfi.h>

#include <linux/fs.h>
#include <linux/major.h>
#include <linux/root_dev.h>

#include <linux/init.h>
#include <asm/io.h>
#include <asm/delay.h>

#define FLASH_BASE   0xffe00000
#define FLASH_SIZE   0x00200000
#define BUS_WIDTH 2

#define NB_OF(x)   (sizeof(x)/sizeof(x[0]))

//extern char* ppcboot_getenv(char* v);


static struct map_info cpu16b_flash_map = {
	.name      = "SNEHA CPU16B",
	.size      = FLASH_SIZE,
	.bankwidth = BUS_WIDTH,
	.phys      = FLASH_BASE
};

static struct mtd_info *flash_mtdinfo;

static struct mtd_partition cpu16b_partitions[] = {
	{.name = "boot (128K)",   .offset = 0x00000,  .size = 0x020000},
	{.name = "kernel (896K)", .offset = 0x20000,  .size = 0x0E0000},
	{.name = "jffs2 (1024K)", .offset = 0x100000, .size = 0x100000}
};

/* Find the MTD device with the given name */
static struct mtd_info *get_mtd_named(char *name)
{
	int i;
	struct mtd_info *mtd;

	for (i=0;i<MAX_MTD_DEVICES;i++)
	{
		mtd=get_mtd_device(NULL,i);
		if (mtd)
		{
			if (strcmp(mtd->name, name)==0)
				return(mtd);
			put_mtd_device(mtd);
		}
	}
	return(NULL);
}




/****************************************************************************/

static int
cpu16b_point(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, u_char **mtdbuf)
{
	struct map_info *map = (struct map_info *) mtd->priv;
	*mtdbuf = (u_char *) (map->map_priv_1 + (int)from);
	*retlen = len;
	return(0);
}

/****************************************************************************/

static int __init
cpu16b_probe(void)
{
	struct mtd_info *mymtd;
	struct map_info *map_ptr;

	map_ptr = &cpu16b_flash_map;

	//map_ptr->buswidth = buswidth;
	//map_ptr->map_priv_2 = addr;
	//map_ptr->phys = addr;
        map_ptr->map_priv_2 = FLASH_BASE;
	//map_ptr->size = size;

	map_ptr->virt = (unsigned long)
			ioremap_nocache(map_ptr->phys, map_ptr->size);

	if (!map_ptr->virt) {
		printk("Failed to ioremap_nocache\n");
		return -EIO;
	}
	simple_map_init(map_ptr);	
	mymtd = do_map_probe("cfi_probe", map_ptr);
	if (!mymtd)
		mymtd = do_map_probe("jedec_probe", map_ptr);

	if (!mymtd) {
		iounmap((void *)map_ptr->map_priv_1);
		return -ENXIO;
	}
		
	mymtd->owner = THIS_MODULE;	mymtd->point = cpu16b_point;
	mymtd->priv = map_ptr;


	flash_mtdinfo = mymtd;
	add_mtd_partitions(mymtd, cpu16b_partitions, NB_OF(cpu16b_partitions));

	return 0;
}

/****************************************************************************/

int __init cpu16b_mtd_init(void)
{
	int rc = -1;
	struct mtd_info *mtd;

    	if (rc != 0)
        	rc = cpu16b_probe();


	
//	mtd = get_mtd_named("Romfs");
//	if (mtd) {
//		ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
//		put_mtd_device(mtd);
//	} else
//		printk("%s: Failed to find & make root filesystem\n", __FUNCTION__);

//	mtd = get_mtd_named("boot");
//    	if (mtd) {
//        	ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
//        	put_mtd_device(mtd);
//    	}
//	mtd = get_mtd_named("kernel");
//    	if (mtd) {
//        	ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
//        	put_mtd_device(mtd);
//    	}
//	mtd = get_mtd_named("jffs2");
//    	if (mtd) {
//        	ROOT_DEV = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
//        	put_mtd_device(mtd);
//    	}

	return(rc);
}

/****************************************************************************/

static void __exit cpu16b_mtd_cleanup(void)
{
	if (flash_mtdinfo) {
		del_mtd_partitions(flash_mtdinfo);
		map_destroy(flash_mtdinfo);
		flash_mtdinfo = NULL;
	}
	if (cpu16b_flash_map.map_priv_1) {
		iounmap((void *)cpu16b_flash_map.map_priv_1);
		cpu16b_flash_map.map_priv_1 = 0;
	}
}

/****************************************************************************/

module_init(cpu16b_mtd_init);
module_exit(cpu16b_mtd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jorge Borrego <jbs@sneha-tech.com>");
MODULE_DESCRIPTION("SNEHA Sarasvati FLASH support for uClinux");

/****************************************************************************/
