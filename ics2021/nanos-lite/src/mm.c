#include <memory.h>
#include <proc.h>
// #include <assert.h>
#ifndef PGSIZE
#define PGSIZE 4096
#endif
static void *pf = NULL;

void *new_page(size_t nr_page)
{
  void *old_pf = pf;
  memset(pf, 0, nr_page * PGSIZE);
  pf += nr_page * PGSIZE;
  assert(pf < (void *)heap.end);
  // Log("new page allocated in %p", old);
  return old_pf;
}

#ifdef HAS_VME
static void *pg_alloc(int n)
{
  return NULL;
}
#endif

void free_page(void *p)
{
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk)
{
  current->max_brk = ROUNDUP(current->max_brk, PGSIZE);
  if (brk > current->max_brk)
  {
    int page_count = ROUNDUP(brk - current->max_brk, PGSIZE) >> 12;
    uintptr_t pages_start = (uintptr_t)new_page(page_count);
    for (int i = 0; i < page_count; ++i)
    {
      map(&current->as,
          (void *)(current->max_brk + i * PGSIZE),
          (void *)(pages_start + i * PGSIZE),
          MMAP_READ | MMAP_WRITE);
    }
    current->max_brk += page_count * PGSIZE;
  }
  return 0;
}

void init_mm()
{
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
