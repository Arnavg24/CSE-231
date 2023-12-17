#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnav Gupta");
MODULE_DESCRIPTION("Count the number of running processes");

static void count_interruptible_processes(void)
{
    struct task_struct *task;
    int existing_processes = 0;

    for_each_process(task) {
        // Check if the task state is TASK_RUNNING or TASK_INTERRUPTIBLE
        if (task->state == TASK_RUNNING || task->state == TASK_INTERRUPTIBLE) {
            existing_processes++;
        }
    }

    printk(KERN_INFO "Number of interruptible processes: %d\n", existing_processes);
}

static void count_running_processes(void)
{
    struct task_struct *task;
    int running_processes = 0;

    for_each_process(task) {
        if (task->state == TASK_RUNNING) {
            running_processes++;
        }
    }

    printk(KERN_INFO "Number of running processes: %d\n", running_processes);
}

static void count_all_processes(void)
{
    struct task_struct *task;
    int all_processes = 0;

    // Iterate through all processes and count them
    for_each_process(task) {
        all_processes++;
    }

    printk(KERN_INFO "Number of all processes: %d\n", all_processes);
}


static int __init count_processes_init(void)
{   printk("\n");
    printk(KERN_INFO "Initializing Count Running Processes module\n");
    count_interruptible_processes();  // Call the function to count running processes
    count_running_processes();
    count_all_processes();
    return 0;
}

static void __exit count_processes_exit(void)
{
    printk(KERN_INFO "Exiting Count Running Processes module\n");
}

module_init(count_processes_init);
module_exit(count_processes_exit);
