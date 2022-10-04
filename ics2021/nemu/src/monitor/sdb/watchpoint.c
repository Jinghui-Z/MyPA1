#include "sdb.h"
#include <string.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool()
{
  int i;
  for (i = 0; i < NR_WP; i++)
  {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

static int number = 1;

bool check_watchpoint(WP **point)
{
  WP *cur = head;
  bool success = true;
  while (cur)
  {
    if (expr(cur->condition, &success))
    {
      *point = cur;
      // IFDEF(CONFIG_DEBUG, Log("Break"));
      return true;
    }
    cur = cur->next;
  }
  return false;
}

WP *new_wp(char *condition, bool *success)
{
  if (free_->next == NULL)
  {
    assert(0);
  }
  WP *result = free_->next;
  result->NO = number++;
  free_->next = result->next;
  result->next = NULL;
  expr(condition, success);
  strcpy(result->condition, condition);
  // printf("test w2: %s\n", result->condition);

  if (head == NULL)
  {
    head = result;
  }
  else
  {
    result->next = head->next;
    head->next = result;
  }
  return result;
}

static void insert_free(WP *wp)
{
  wp->next = free_->next;
  free_->next = wp;
}

void free_wp(int NO)
{
  if (head->NO == NO)
  {
    WP *buffer = head->next;
    insert_free(head);
    head = buffer;
    return;
  }

  WP *prev = head;
  while (prev->next)
  {
    if (prev->next->NO == NO)
    {
      WP *buffer = prev->next->next;
      insert_free(prev->next);
      prev->next = buffer;
      return;
    }
    prev = prev->next;
  }

  printf("未找到 \e[1;36mWatchPoint(NO.%d)\e[0m\n", NO);
}

void watchpoint_display()
{
  printf("NO.\tCondation\n");
  WP *cur = head;
  while (cur)
  {
    printf("\e[1;36m%d\e[0m\t\e[0;32m%s\e[0m\n", cur->NO, cur->condition);
    cur = cur->next;
  }
}