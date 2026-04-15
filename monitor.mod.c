#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x97235f8d, "cdev_init" },
	{ 0xae7140fb, "cdev_add" },
	{ 0x02f9bbf0, "init_timer_key" },
	{ 0x058c185a, "jiffies" },
	{ 0x32feeafc, "mod_timer" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x89258034, "device_destroy" },
	{ 0x7c77f2d5, "class_destroy" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xf46d5bf3, "mutex_lock" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0xa62b1cc9, "kmalloc_caches" },
	{ 0xd1f07d8f, "__kmalloc_cache_noprof" },
	{ 0xc609ff70, "strncpy" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x2352b148, "timer_delete_sync" },
	{ 0x91d81025, "cdev_del" },
	{ 0x16e6fb27, "mmput" },
	{ 0xd272d446, "__rcu_read_lock" },
	{ 0x756f475d, "find_vpid" },
	{ 0x871b2e7b, "pid_task" },
	{ 0xd272d446, "__rcu_read_unlock" },
	{ 0x5146d978, "get_task_mm" },
	{ 0x99b61475, "__put_task_struct" },
	{ 0x2520ea93, "refcount_warn_saturate" },
	{ 0x871e3873, "send_sig" },
	{ 0xd272d446, "__fentry__" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xfb3de43c, "class_create" },
	{ 0x773c4019, "device_create" },
	{ 0xab006604, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x97235f8d,
	0xae7140fb,
	0x02f9bbf0,
	0x058c185a,
	0x32feeafc,
	0xe8213e80,
	0xd272d446,
	0x0bc5fb0d,
	0x89258034,
	0x7c77f2d5,
	0x092a35a2,
	0xf46d5bf3,
	0xcb8b6ec6,
	0xf46d5bf3,
	0xbd03ed67,
	0xa62b1cc9,
	0xd1f07d8f,
	0xc609ff70,
	0xd272d446,
	0x2352b148,
	0x91d81025,
	0x16e6fb27,
	0xd272d446,
	0x756f475d,
	0x871b2e7b,
	0xd272d446,
	0x5146d978,
	0x99b61475,
	0x2520ea93,
	0x871e3873,
	0xd272d446,
	0x9f222e1e,
	0xfb3de43c,
	0x773c4019,
	0xab006604,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"cdev_init\0"
	"cdev_add\0"
	"init_timer_key\0"
	"jiffies\0"
	"mod_timer\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"unregister_chrdev_region\0"
	"device_destroy\0"
	"class_destroy\0"
	"_copy_from_user\0"
	"mutex_lock\0"
	"kfree\0"
	"mutex_unlock\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"strncpy\0"
	"__stack_chk_fail\0"
	"timer_delete_sync\0"
	"cdev_del\0"
	"mmput\0"
	"__rcu_read_lock\0"
	"find_vpid\0"
	"pid_task\0"
	"__rcu_read_unlock\0"
	"get_task_mm\0"
	"__put_task_struct\0"
	"refcount_warn_saturate\0"
	"send_sig\0"
	"__fentry__\0"
	"alloc_chrdev_region\0"
	"class_create\0"
	"device_create\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A5274BAF4344BF7562DC0BF");
