#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm-generic/barrier.h>

/*  seqlock
 * doc: https://www.kernel.org/doc/html/latest/locking/seqlock.html
 *  */
#include <linux/seqlock.h>
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Hcamal");

/* dynamic */
seqlock_t foo_seqlock;

/* static */
static DEFINE_SEQLOCK(foo_seqlock_s);

/* C99 struct init */
/*
struct {
        .seql   = __SEQLOCK_UNLOCKED(foo.seql)
} foo;
*/

void lock_init(void) {
  /* seqlock  dynamic init */
    smp_mb();
  seqlock_init(&foo_seqlock);
}
void read_seq_lock(void) {
  unsigned seq = 0;
  do {
    seq = read_seqbegin(&foo_seqlock);
    /* ... [[read-side critical section]] ... */
  } while (read_seqretry(&foo_seqlock, seq));
}
void write_seq_lock(void) {
  write_seqlock(&foo_seqlock);
  /* ... [[write-side critical section]] ... */
  write_sequnlock(&foo_seqlock);
}
int hello_init(void) {
  int size = sizeof(struct task_struct);
  printk(KERN_INFO "Hello World\n");
  printk(KERN_INFO "task_struct size:%d\n", size);
  lock_init();
  read_seq_lock();
  write_seq_lock();
  return 0;
}

void hello_exit(void) { printk(KERN_INFO "Goodbye World\n"); }

module_init(hello_init);
module_exit(hello_exit);
