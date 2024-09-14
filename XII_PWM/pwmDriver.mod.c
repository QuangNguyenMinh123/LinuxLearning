#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0x6fbd61d8, "module_layout" },
	{ 0x8e061c72, "platform_driver_unregister" },
	{ 0xe80c7f3b, "sysfs_remove_group" },
	{ 0x42899e9a, "__platform_driver_register" },
	{ 0xedbfbd3f, "kobject_put" },
	{ 0xc5850110, "printk" },
	{ 0x8b3faaa4, "sysfs_create_group" },
	{ 0x14798c46, "kobject_create_and_add" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*CP9_14_USERPWM");
MODULE_ALIAS("of:N*T*CP9_14_USERPWMC*");
