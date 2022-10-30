#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <proc.h>
// #define CONFIG_STRACE
#ifdef CONFIG_STRACE
static char *syscall_names[] = {
    "SYS_exit", "SYS_yield", "SYS_open", "SYS_read", "SYS_write", "SYS_kill", "SYS_getpid", "SYS_close",
    "SYS_lseek", "SYS_brk", "SYS_fstat", "SYS_time", "SYS_signal", "SYS_execve", "SYS_fork", "SYS_link",
    "SYS_unlink", "SYS_wait", "SYS_times", "SYS_gettimeofday"};
#endif

int mm_brk(int);
void do_syscall(Context *c)
{
 
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
#ifdef CONFIG_STRACE
  printf("STRACE: [#%3ld]( %ld, %ld, %ld )\n", a[0], a[1], a[2], a[3]);
#endif
  switch (a[0])
  {
  case SYS_yield:
    yield();
    c->GPRx = 0;
    break;
  case SYS_exit:
    printf("1234\n");
    halt(a[1]);
    // naive_uload(NULL, "/bin/menu");
    c->GPRx = a[1];
    break;
  case SYS_write:
    c->GPRx = fs_write(a[1], (void *)a[2], a[3]);
    break;
  case SYS_brk:
    c->GPRx = mm_brk(a[1]);
    break;
  default:
    panic("Unhandled syscall ID = %d", a[0]);
  }
#ifdef CONFIG_STRACE
  printf("        [%s] -> %ld\n", syscall_names[a[0]], c->GPRx);
#endif
}
