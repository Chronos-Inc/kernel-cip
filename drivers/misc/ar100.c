#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/arm-smccc.h>
#include <uapi/linux/stat.h>
#include <uapi/linux/psci.h>

enum { AR100_SMC_CALL_ID = 0x8400A100 };

static ssize_t show(struct device *dev, struct device_attribute *attr, char *buff)
{
	struct arm_smccc_res res;
        arm_smccc_smc(AR100_SMC_CALL_ID, -1, 0, 0, 0, 0, 0, 0, &res);
        if((s32)res.a0 < 0) return -EIO;
	buff[0] = (res.a0 & 0x1) ? '0' : '1';
	buff[1] = '\n';
	return 2;
}

static ssize_t store(struct device *dev, struct device_attribute *attr,
		const char *buff, size_t count)
{
	struct arm_smccc_res res;
	u32 assert_reset;
	if(kstrtou32(buff, 10, &assert_reset) != 0 || assert_reset > 1) return -EINVAL;
	arm_smccc_smc(AR100_SMC_CALL_ID, !assert_reset, 0, 0, 0, 0, 0, 0, &res);
	return (s32)res.a0 < 0 ? -EIO : count;
}

const static struct device_attribute reset_attribute = __ATTR(reset, S_IRUGO | S_IWUSR, show, store);

static struct device *dev = NULL;

static int mod_init(void)
{
	int ret;
	dev = root_device_register("ar100");
	if(IS_ERR(dev)) return PTR_ERR(dev);
	ret = sysfs_create_file(&dev->kobj, &reset_attribute.attr);
	if(!ret) return 0;
	root_device_unregister(dev);
	return ret;
}

static void mod_exit(void)
{
	sysfs_remove_file(&dev->kobj, &reset_attribute.attr);
	root_device_unregister(dev);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David Beinder");
