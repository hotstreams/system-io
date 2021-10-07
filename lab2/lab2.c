#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/blk_types.h>
#include <linux/bio.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("eddie");
MODULE_DESCRIPTION("lab2");

int c = 0; //Variable for Major Number
#define SECTOR_SIZE 512
#define MEMSIZE 50 * 1024 * 1024 / SECTOR_SIZE // Size of Ram disk in sectors
#define MBR_SIZE SECTOR_SIZE
#define MBR_DISK_SIGNATURE_OFFSET 440
#define MBR_DISK_SIGNATURE_SIZE 4
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_ENTRY_SIZE 16
#define PARTITION_TABLE_SIZE 64
#define MBR_SIGNATURE_OFFSET 510
#define MBR_SIGNATURE_SIZE 2
#define MBR_SIGNATURE 0xAA55
#define BR_SIZE SECTOR_SIZE
#define BR_SIGNATURE_OFFSET 510
#define BR_SIGNATURE_SIZE 2
#define BR_SIGNATURE 0xAA55

typedef struct {
    unsigned char boot_type; // 0x00 - Inactive; 0x80 - Active (Bootable)
    unsigned char start_head;
    unsigned char start_sec: 6;
    unsigned char start_cyl_hi: 2;
    unsigned char start_cyl;
    unsigned char part_type;
    unsigned char end_head;
    unsigned char end_sec: 6;
    unsigned char end_cyl_hi: 2;
    unsigned char end_cyl;
    unsigned int abs_start_sec;
    unsigned int sec_in_part;
} PartEntry;

typedef struct sblkdev_cmd_s {
    //
} sblkdev_cmd_t;

typedef PartEntry PartTable[4];

#define SEC_PER_HEAD 63
#define HEAD_PER_CYL 255
#define HEAD_SIZE (SEC_PER_HEAD * SECTOR_SIZE)
#define CYL_SIZE (SEC_PER_HEAD * HEAD_PER_CYL * SECTOR_SIZE)

#define sec4size(s) ((((s) % CYL_SIZE) % HEAD_SIZE) / SECTOR_SIZE)
#define head4size(s) (((s) % CYL_SIZE) / HEAD_SIZE)
#define cyl4size(s) ((s) / CYL_SIZE)

static PartTable def_part_table =
{
    {
            .boot_type =  0x00,
            .start_sec =  0x02,
            .start_head =  0x0,
            .start_cyl_hi =  0,
            .start_cyl =  0x0,
            .part_type =  0x83,
            .end_head =  0xD2,
            .end_sec =  0x10,
            .end_cyl_hi =  0x00,
            .end_cyl =  0x03,
            .abs_start_sec =  0x01,
            .sec_in_part =  0xF000
    },
    {
            .boot_type =  0x00,
            .start_head =  0xD2,
            .start_sec =  0x11,
            .start_cyl_hi =  0,
            .start_cyl =  0x0,
            .part_type =  0x0F, // extended partition type
            .end_sec =  0x19,
            .end_head =  0x5F,
            .end_cyl_hi =  0x00,
            .end_cyl =  0x9F,
            .abs_start_sec =  0xF001,
            .sec_in_part =  0x9FFF
    }
};
static unsigned int def_log_part_br_abs_start_sector[] = {0xF001, 0xF001 + 0x5001};
static const PartTable def_log_part_table[] =
        {
                {
                        {
                                .boot_type =  0x00,
                                .start_head =  0xD2,
                                .start_sec =  0x12,
                                .start_cyl_hi =  0,
                                .start_cyl =  0x03,
                                .part_type =  0x83,
                                .end_head =  0x19,
                                .end_sec =  0x16,
                                .end_cyl_hi =  0x00,
                                .end_cyl =  0x05,
                                .abs_start_sec =  0x1,
                                .sec_in_part =  0x5000
                        },
                        {
                                .boot_type =  0x00,
                                .start_head =  0x19,
                                .start_sec =  0x17,
                                .start_cyl_hi =  0,
                                .start_cyl =  0x05,
                                .part_type =  0x05,
                                .end_head =  0x5F,
                                .end_sec =  0x19,
                                .end_cyl_hi =  0x00,
                                .end_cyl =  0x6,
                                .abs_start_sec =  0x5001,
                                .sec_in_part =  0x4FFE
                        }
                },
                {
                        {
                                .boot_type =  0x00,
                                .start_head =  0x19,
                                .start_sec =  0x18,
                                .start_cyl_hi =  0,
                                .start_cyl =  0x05,
                                .part_type =  0x83,
                                .end_head =  0x5F,
                                .end_sec =  0x20,
                                .end_cyl_hi =  0x00,
                                .end_cyl =  0x6,
                                .abs_start_sec =  0x1,
                                .sec_in_part =  0x4FFC
                        }
                }
        };

static void copy_mbr(u8 *disk) {
    memset(disk, 0x0, MBR_SIZE);
    *(unsigned long *)(disk + MBR_DISK_SIGNATURE_OFFSET) = 0x36E5756D;
    memcpy(disk + PARTITION_TABLE_OFFSET, &def_part_table, PARTITION_TABLE_SIZE);
    *(unsigned short *)(disk + MBR_SIGNATURE_OFFSET) = MBR_SIGNATURE;
}

static void copy_br(u8 *disk, int abs_start_sector, const PartTable *part_table) {
    disk += (abs_start_sector * SECTOR_SIZE);
    memset(disk, 0x0, BR_SIZE);
    memcpy(disk + PARTITION_TABLE_OFFSET, part_table, PARTITION_TABLE_SIZE);
    *(unsigned short *)(disk + BR_SIGNATURE_OFFSET) = BR_SIGNATURE;
}

void copy_mbr_n_br(u8 *disk) {
    int i;

    copy_mbr(disk);
    for (i = 0; i < ARRAY_SIZE(def_log_part_table); i++)
    {
        copy_br(disk, def_log_part_br_abs_start_sector[i], &def_log_part_table[i]);
    }
}

struct mydiskdrive_dev
{
    int size;
    u8 *data;
    spinlock_t lock;
    struct request_queue *queue;
    struct gendisk *gd;

    struct blk_mq_tag_set tag_set;
} device;

struct mydiskdrive_dev *x;

static int my_open(struct block_device *x, fmode_t mode)
{
    printk(KERN_INFO "mydiskdrive : open \n");
    return 0;

}

static void my_release(struct gendisk *disk, fmode_t mode)
{
    printk(KERN_INFO "mydiskdrive : closed \n");
}

static struct block_device_operations fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
};

int mydisk_init(void)
{
    (device.data) = vmalloc(MEMSIZE * SECTOR_SIZE);
    copy_mbr_n_br(device.data);

    return MEMSIZE;
}

static int rb_transfer(struct request *req)
{
    int dir = rq_data_dir(req);
    int ret = 0;

    sector_t start_sector = blk_rq_pos(req);
    unsigned int sector_cnt = blk_rq_sectors(req);
    struct bio_vec bv;

    #define BV_PAGE(bv) ((bv).bv_page)
    #define BV_OFFSET(bv) ((bv).bv_offset)
    #define BV_LEN(bv) ((bv).bv_len)

    struct req_iterator iter;
    sector_t sector_offset;
    unsigned int sectors;
    u8 *buffer;
    sector_offset = 0;
    rq_for_each_segment(bv, req, iter)
    {
        buffer = page_address(BV_PAGE(bv)) + BV_OFFSET(bv);
        if (BV_LEN(bv) % (SECTOR_SIZE) != 0)
        {
            printk(KERN_ERR"bio size is not a multiple ofsector size\n");
            ret = -EIO;
        }
        sectors = BV_LEN(bv) / SECTOR_SIZE;
        printk(KERN_DEBUG "my disk: Start Sector: %llu, Sector Offset: %llu; \
		Buffer: %p; Length: %u sectors\n", \
		       (unsigned long long)(start_sector), (unsigned long long) \
		       (sector_offset), buffer, sectors);

        if (dir == WRITE) /* Write to the device */
        {
            memcpy((device.data) + ((start_sector + sector_offset)*SECTOR_SIZE)\
			       , buffer, sectors * SECTOR_SIZE);
        }
        else /* Read from the device */
        {
            memcpy(buffer, (device.data) + ((start_sector + sector_offset)\
			                                *SECTOR_SIZE), sectors * SECTOR_SIZE);
        }
        sector_offset += sectors;
    }

    if (sector_offset != sector_cnt)
    {
        printk(KERN_ERR "mydisk: bio info doesn't match with the request info");
        ret = -EIO;
    }
    return ret;
}

static blk_status_t my_block_request(struct blk_mq_hw_ctx* hctx, const struct blk_mq_queue_data* bd) {
    struct request* rq = bd->rq;
    struct block_dev* dev = device.queue->queuedata;

    blk_mq_start_request(rq);

    if (blk_rq_is_passthrough(rq)) {
        printk(KERN_NOTICE "skip non-fs request\n");
        blk_mq_end_request(rq, BLK_STS_IOERR);
        return BLK_STS_OK;
    }

    rb_transfer(rq);

    blk_mq_end_request(rq, BLK_STS_OK);

    return 0;
}

static struct blk_mq_ops my_queue_ops = {
    .queue_rq = my_block_request,
};

void init_queue(void) {
    device.tag_set.ops = &my_queue_ops;
    device.tag_set.nr_hw_queues = 1;
    device.tag_set.queue_depth = 128;
    device.tag_set.numa_node = NUMA_NO_NODE;
    device.tag_set.cmd_size = 0;
    device.tag_set.flags = BLK_MQ_F_SHOULD_MERGE;

    blk_mq_alloc_tag_set(&device.tag_set);

    device.queue = blk_mq_init_queue(&device.tag_set);

    blk_queue_logical_block_size(device.queue, SECTOR_SIZE);
}

int device_setup(void) {
    mydisk_init();
    if ((c = register_blkdev(c, "mydisk")) < 0) {
        return -1;
    };

    printk(KERN_ALERT "Major Number is : %d", c);
    spin_lock_init(&device.lock); // lock for queue

    init_queue();
    device.gd = alloc_disk(8); // gendisk allocation

    (device.gd)->major = c; // major no to gendisk
    device.gd->first_minor = 0; // first minor of gendisk

    device.gd->fops = &fops;
    device.gd->private_data = &device;
    device.gd->queue = device.queue;
    device.size = mydisk_init();
    printk(KERN_INFO"THIS IS DEVICE SIZE %d", device.size);
    sprintf(((device.gd)->disk_name), "mydisk");
    set_capacity(device.gd, device.size);
    add_disk(device.gd);

    return 0;
}
static int __init mydiskdrive_init(void)
{
    device_setup();

    return 0;
}
void mydisk_cleanup(void)
{
    vfree(device.data);
}

void __exit mydiskdrive_exit(void)
{
    del_gendisk(device.gd);
    put_disk(device.gd);
    blk_cleanup_queue(device.queue);
    unregister_blkdev(c, "mydisk");
    mydisk_cleanup();
}
module_init(mydiskdrive_init);
module_exit(mydiskdrive_exit);