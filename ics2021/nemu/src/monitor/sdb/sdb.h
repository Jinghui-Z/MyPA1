#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

word_t expr(const char *e, bool *success);

typedef struct watchpoint
{
  int NO;
  char condition[100];
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;
bool check_watchpoint(WP **point);

WP *new_wp(char *condation, bool *success);

void free_wp(int NO);

void watchpoint_display();
#endif
