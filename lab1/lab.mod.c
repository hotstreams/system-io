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
	{ 0xb123b985, "proc_remove" },
	{ 0xf8c5592b, "cdev_del" },
	{ 0x4e80d775, "proc_create" },
	{ 0xa64c7bb5, "device_destroy" },
	{ 0xeae5956f, "cdev_add" },
	{ 0x46baef61, "cdev_init" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x8cf87f41, "class_destroy" },
	{ 0x2256e61, "device_create" },
	{ 0xf0e0cd26, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x37a0cba, "kfree" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xb6e1e296, "kmem_cache_alloc_trace" },
	{ 0xb7349dcd, "kmalloc_caches" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4F8A018D5DFE8AAC171A5BB");
