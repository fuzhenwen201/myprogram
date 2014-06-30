#include <linux/init.h>
#include <linux/modul.h>
#include <irq.h>
#include <interrupt.h>
//定义按键硬件数据结构
struct key_resource{
	int irq;
	char* name;
};
//初始化按键信息
static struct key_resource key_info[] = {
	[0] = {
		.irq = IRQ_EINT(0);
		.name = "key_up";
	},
	[1] = {
		.irq = IRQ_EINT(1);
		.name = "key_down";
	},
};
//分配工作和延时工作
static struct work_struct work;
static struct delayed_work dwork;
//工作处理函数
static void key_work_func(struct work_struct *work)
{
	printk("%s\n",__func__);
}
//延时工作的处理函数
static void key_dwork_func(struct  work_struct *work)
{
	printk("%s\n",__func__);
}

//中断处理函数---顶半部
static irqreturn_t key_isr(int irq, void *dev_id)
{
	//1.登记工作
	schedule_work(&work);
	//2.登记延时工作
	//schedule_delayed_work(&dwork, 5*HZ);//5s后内核执行延时工作处理函数
	prink("%s\n",__func__);
	return IRQ_HANDLED;
}

static int key_init(void)
{
	int i;
	//1.注册中断
	for(i = 0; i < ARRAY_SIZE(key_info); i++)
		request_irq(key_info[i].irq,key_isr,IRQ_TRIGGER_RISING|IRQ_TRIGGER_FALLING,
				key_info[i].name,&key_info[i]);
	//2.初始化工作和延时工作
	INIT_WORK(&work, key_work_func);
	INIT_DELAYED_WORK(&dwork, key_dwork_func);

	return 0;
}
static void key_exit(void)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(key_info); i++)
		free_irq(key_info[i].irq, &key_info[i]);
}
module_init(key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");
