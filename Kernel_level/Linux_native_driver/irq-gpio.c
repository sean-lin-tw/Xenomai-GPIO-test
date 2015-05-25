#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
 
#include <linux/interrupt.h>
#include <linux/gpio.h>
 
 
#define DRIVER_AUTHOR "Jacky Lin"
#define DRIVER_DESC   "Tnterrupt Latency Test"

#define GPIO_INPUT	23
#define GPIO_OUTPUT	22
 
// text below will be seen in 'cat /proc/interrupt' command
#define GPIO_ANY_GPIO_DESC	"R-pi"
 
// below is optional, used in more complex code, in our case, this could be
// NULL
#define DEVICE_DESCRIPTION	"Raspberry pi I model B"
 
 
/****************************************************************************/
/* Interrupts variables block                                               */
/****************************************************************************/
short int irq_any_gpio = 0;
 
 
/****************************************************************************/
/* IRQ handler - fired on interrupt                                         */
/****************************************************************************/
static irqreturn_t r_irq_handler(int irq, void *dev_id, struct pt_regs *regs) {
 
   unsigned long flags;
   
	//disable hard interrupts (remember them in flag 'flags')
	//local_irq_save(flags);

	static int value;
	value = gpio_get_value(GPIO_INPUT);
	gpio_set_value(GPIO_OUTPUT, value);
	 
	//restore hard interrupts
	//local_irq_restore(flags);
 
   return IRQ_HANDLED;
}
 
 
/****************************************************************************/
/* This function configures interrupts.                                     */
/****************************************************************************/
void r_int_config(void) {
 
   if (gpio_request(GPIO_INPUT, GPIO_ANY_GPIO_DESC)) {
      printk("GPIO request faiure: %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
 
   if ( (irq_any_gpio = gpio_to_irq(GPIO_INPUT)) < 0 ) {
      printk("GPIO to IRQ mapping faiure %s\n", GPIO_ANY_GPIO_DESC);
      return;
   }
 
   printk(KERN_NOTICE "Mapped int %d\n", irq_any_gpio);
 
   if (request_irq(irq_any_gpio,
                   (irq_handler_t ) r_irq_handler,
                   IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                   GPIO_ANY_GPIO_DESC,
                   DEVICE_DESCRIPTION)) {
      printk("Irq Request failure\n");
      return;
   }
 
   return;
}
 
 
/****************************************************************************/
/* This function releases interrupts.                                       */
/****************************************************************************/
void r_int_release(void) {
 
   free_irq(irq_any_gpio, DEVICE_DESCRIPTION);
   gpio_free(GPIO_INPUT);
 
   return;
}
 
 
/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
int r_init(void) {
 
   printk(KERN_NOTICE "Driver initiated !\n");
   r_int_config();
 
   return 0;
}
 
void r_cleanup(void) {
   printk(KERN_NOTICE "Driver deactivated\n");
   r_int_release();
 
   return;
}
 

module_init(r_init);
module_exit(r_cleanup);
 
 
/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
