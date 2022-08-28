/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * w1-gpio interface to platform code
 *
 * Copyright (C) 2007 Ville Syrjala <syrjala@sci.fi>
 */
#ifndef _LINUX_W1_GPIO_H
#define _LINUX_W1_GPIO_H

struct gpio_desc;

/**
 * struct w1_gpio_platform_data - Platform-dependent data for w1-gpio
 */
struct w1_gpio_platform_data {
	struct gpio_desc *gpiod; /* data input; also used to pull the line down if pulldown_gpiod is not set */
	struct gpio_desc *strong_pullup_gpiod; /* strong (high current) pull up */
	struct gpio_desc *pulldown_gpiod; /* optional, data output using pull down */
	unsigned int strong_pullup_duration;
};

#endif /* _LINUX_W1_GPIO_H */
