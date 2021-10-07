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
	{ 0x373ee92c, "param_ops_charp" },
	{ 0xd83e24a, "param_ops_int" },
	{ 0xfc14e42e, "unregister_netdev" },
	{ 0xb123b985, "proc_remove" },
	{ 0xdfb1f5f3, "netdev_rx_handler_unregister" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x1daadb36, "netdev_rx_handler_register" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xc707482a, "register_netdev" },
	{ 0xd5814a46, "dev_alloc_name" },
	{ 0x19c67a6e, "free_netdev" },
	{ 0x86abdd96, "__dev_get_by_name" },
	{ 0x465d6010, "init_net" },
	{ 0x7af95552, "alloc_netdev_mqs" },
	{ 0x4e80d775, "proc_create" },
	{ 0x30ae3fdf, "dev_queue_xmit" },
	{ 0x69acdf38, "memcpy" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x37a0cba, "kfree" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xb6e1e296, "kmem_cache_alloc_trace" },
	{ 0xb7349dcd, "kmalloc_caches" },
	{ 0x1f180fa0, "ether_setup" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E7C3001A5C4A1F81FB56645");
