#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe2a06d27, "module_layout" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x10c1ed0a, "blk_cleanup_queue" },
	{ 0x2df89dc5, "put_disk" },
	{ 0x9af7064a, "del_gendisk" },
	{ 0x999e8297, "vfree" },
	{ 0x73ecfb27, "device_add_disk" },
	{ 0xfd653c9f, "set_capacity" },
	{ 0x414988c2, "__alloc_disk_node" },
	{ 0x720a27a7, "__register_blkdev" },
	{ 0x4caca289, "blk_queue_logical_block_size" },
	{ 0xbabc04d8, "blk_mq_init_queue" },
	{ 0x92f5b702, "blk_mq_alloc_tag_set" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x5a67f2da, "blk_mq_end_request" },
	{ 0x7c72dd9f, "blk_mq_start_request" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "583D4F133C9371D5F4C4084");
