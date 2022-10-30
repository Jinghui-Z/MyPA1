#include <proc.h>
#include <device.h>
#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb()
{
  current = &pcb_boot;
}

void hello_fun(void *arg)
{
  int j = 1;
  while (1)
  {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j++;
    yield();
  }
}
void naive_uload(PCB *pcb, const char *filename);
// void context_kload(PCB *pcb, void (*entry)(void *), void *arg);
// void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]);

#define PROG_PATH1 "/bin/nterm"
#define PROG_PATH2 "/bin/nslider"
#define PROG_PATH3 "/bin/bird"
void init_proc()
{

  printf("111\n");
  naive_uload(NULL, "/bin/dummy");
  printf("222\n");
}

Context *schedule(Context *prev)
{
  static int prio_count = 0;
  current->cp = prev;
  assert(1 <= fg_pcb);
  assert(fg_pcb <= 3);
  if (prio_count < 100)
  {
    prio_count++;
    current = &pcb[fg_pcb];
  }
  else
  {
    prio_count = 0;
    current = &pcb[0];
  }
  // Log("schedule %p(updir %p) -> %p(updir %p)", prev, prev->pdir, current->cp, current->cp->pdir);
  return current->cp;
}
