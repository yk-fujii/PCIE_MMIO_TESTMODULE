#include <asm/current.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/sched.h>

#define MMIO_BAR0 0
#define MMIO_BAR1 1	

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("testiorw");
MODULE_AUTHOR("yusuke fujii");


struct gpu_pci_list{
	struct gpu_pci_list *next;
	struct gpu_pci_list *prev;
	void *mappings;
};


static int gpu_pci_init(struct gpu_pci_list *head){

	struct pci_dev *__dev = NULL;
	struct irqaction *__act = NULL;


	struct gpu_pci_list *list = head;

	while(__dev = pci_get_class(PCI_CLASS_DISPLAY_VGA << 8, __dev))
	{
		if(!list || list->mappings){
			list->next = (struct gpu_pci_list *)kmalloc(sizeof(struct gpu_pci_list), GFP_DMA);
			list->prev = list;
			list = list->next;
			list->mappings = NULL;

		}
		list->mappings = ioremap(pci_resource_start(__dev, MMIO_BAR0), pci_resource_len(__dev, MMIO_BAR0));
		printk("mapped\n");
	}

}

static int __init  testiorw_init(void){

	unsigned long long t0, t1;
	struct gpu_pci_list head;
	printk("%s: loaded!", __func__);
	int i = 0;
	int val;

	gpu_pci_init(&head);

	if(!head.mappings){
		printk("mapped area is NULL\n");
	}else{

		while(++i<3){

			t0 = sched_clock();
			//ioread32(head.mappings + 0x80);
			//val = ioread32(head.mappings + 0x80);
			iowrite32(0xff, head.mappings + 0x80);
			t1 = sched_clock();
			printk("read 0x%x time:0x%lu ns\n", val,t1-t0);
#define TIMEOUT 500
			schedule_timeout(TIMEOUT);

		}}

}


static void __exit testiorw_exit(void){
	printk("unloaded!!!\n");

}

module_init(testiorw_init);
module_exit(testiorw_exit);


static int testiorw_open(struct inode *inode, struct file *filp){
	return 1;
}

static int testiorw_release(struct inode *inode, struct file *filp){
	return 1;
}

static int testiorw_write(struct inode *inode, struct file *filp){

	return 1;
}

static int testiorw_ioctl(struct inode *inode, struct file *filp){
	return 1;
}

static struct file_operations resch_fops = {
	.owner = THIS_MODULE,
	.open = testiorw_open, /* do nothing but must exist. */
	.release = testiorw_release, /* do nothing but must exist. */
	//	.read = NULL,
	//	.write = testiorw_write,
	//	.unlocked_ioctl = testiorw_ioctl
};
