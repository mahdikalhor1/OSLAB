#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday {
    int year;
    int month;
    int day;
    struct list_head list;
};


static LIST_HEAD(birthday_list);

struct  birthday* create_node(int year , int month , int day)
{
    struct  birthday *person = kmalloc(sizeof(struct birthday),GFP_KERNEL);
    person -> year = year;
    person -> month = month;
    person -> day = day;
    return person;
};


int mymo_init(void)
{
    printk(KERN_INFO "Loading second module\n");
    INIT_LIST_HEAD(&birthday_list);
    struct birthday* n;
    
    n = create_node(1900,11,11);
    list_add_tail(&n->list,&birthday_list);
    n = create_node(2006,12,01);
    list_add_tail(&n->list,&birthday_list);
    n = create_node(2007,04,07);
    list_add_tail(&n->list,&birthday_list);
    n = create_node(1996,05,05);
    list_add_tail(&n->list,&birthday_list);
    n = create_node(2011,05,05);
    list_add_tail(&n->list,&birthday_list);
    
    
    struct birthday *ptr;
    list_for_each_entry(ptr,&birthday_list,list){ 
        printk(KERN_INFO "birthday: %d %d %d\n",ptr->year,ptr->month,ptr->day); 
    }
    return 0;
}

void mymo_exit(void)
{
    struct birthday *node;
    struct list_head *ptr,*next;
    
    list_for_each_safe(ptr,next,&birthday_list){
        node = list_entry(ptr,struct birthday,list); 
        printk(KERN_INFO "remove: %d %d %d\n",node->year,node->month,node->day); 
        list_del(ptr);
        kfree(node);
    }
    
    printk(KERN_INFO "Removing second module\n");
}

module_init(mymo_init);
module_exit(mymo_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("second module");
MODULE_AUTHOR("SGG");