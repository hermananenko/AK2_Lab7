// SPDX-License-Identifier: GPL-2.0+

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/bug.h>

#include "hello1.h"

struct list_head *iter, *iter_safe;
struct data *item;
LIST_HEAD(list);

int print_hello(uint num)
{
	int i;

	if (num >= 0 && num <= 10) {
		if (num == 0)
			pr_warn("Small value!\n");
		else if (num > 5 && num <= 10)
			pr_warn("Big value!\n");

		for (i = 0; i < num; i++) {
			if (i == 4)
				item = 0;
			else
				item = kmalloc(sizeof(*item), GFP_KERNEL);
			item->time_before = ktime_get_ns();
			pr_alert("Hello, world!\n");
			item->time_after = ktime_get_ns();
			list_add_tail(&(item->list), &list);
		}
	} else {
		BUG_ON(1);
	}
	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_alert("hello1 started!\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	ktime_t sub;

	list_for_each_safe(iter, iter_safe, &list) {
		item = list_entry(iter, struct data, list);
		sub = ktime_sub(item->time_after, item->time_before);
		pr_info("%lu\n", (unsigned long) sub);
		list_del(iter);
		kfree(item);
	}
}

module_init(hello1_init);
module_exit(hello1_exit);

MODULE_AUTHOR("Herman Ananenko <example@exmpl.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training. Part 1.");
MODULE_LICENSE("Dual BSD/GPL");
