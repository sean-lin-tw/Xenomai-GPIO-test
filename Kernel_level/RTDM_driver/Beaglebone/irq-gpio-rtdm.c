irq-gpio-rtdm.c
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/module.h>

#include <rtdm/rtdm_driver.h>

// GPIO IN 20 -> Pin 41
#define GPIO_IN  20
// GPIO OUT 7 -> Pin 42
#define GPIO_OUT 7

static rtdm_irq_t irq_rtdm;

static int handler_interruption(rtdm_irq_t * irq)
{
    static int value = 0;
    gpio_set_value(GPIO_OUT, value);
    value = 1 - value;
    return RTDM_IRQ_HANDLED;
}

static int __init exemple_init (void)
{
    int err;

    int numero_interruption = gpio_to_irq(GPIO_IN);

    if ((err = gpio_request(GPIO_IN, THIS_MODULE->name)) != 0) {
        return err;
    }
    if ((err = gpio_direction_input(GPIO_IN)) != 0) {
        gpio_free(GPIO_IN);
        return err;
    }
    if ((err = gpio_request(GPIO_OUT, THIS_MODULE->name)) != 0) {
        gpio_free(GPIO_IN);
        return err;
    }
    if ((err = gpio_direction_output(GPIO_OUT, 1)) != 0) {
        gpio_free(GPIO_OUT);
        gpio_free(GPIO_IN);
        return err;
    }

    irq_set_irq_type(numero_interruption,  IRQF_TRIGGER_RISING);

    if ((err = rtdm_irq_request(& irq_rtdm, 
                     numero_interruption, handler_interruption, 
                     RTDM_IRQTYPE_EDGE,
                     THIS_MODULE->name, NULL)) != 0) {
        gpio_free(GPIO_OUT);
        gpio_free(GPIO_IN);
        return err;
    }
    return 0; 
}

static void __exit exemple_exit (void)
{
    rtdm_irq_free(& irq_rtdm);
    gpio_free(GPIO_OUT);
    gpio_free(GPIO_IN);
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");
