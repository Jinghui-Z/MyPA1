#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <math.h>
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <regex.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args)
{
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args)
{
  uint64_t n = 1;
  if (args != NULL)
  {
    for (int i = strlen(args) - 1; i >= 0; --i)
    {
      n += (uint64_t)(args[i] - '0') * pow(10, i);
    }
  }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args)
{
  if (args == NULL)
  {
    printf("info指令 缺少参数\n");
  }
  else if (strcmp(args, "r") == 0)
  {
    isa_reg_display();
  }
  else if (strcmp(args, "w") == 0)
  {
    watchpoint_display();
  }
  else
  {
    printf("未知的参数 [%s] \n", args);
  }

  return 0;
}
static int cmd_x(char *args)
{
  char *N = strtok(NULL, " ");
  char *addr = strtok(NULL, " ");
  paddr_t n_addr;
  int num;
  sscanf(N, "%d", &num);
  sscanf(addr, "%x", &n_addr);
  // printf("%d", num);
  // printf("%x", n_addr);
  for (int i = 0; i < num; ++i)
  {
    printf("0x%x: ", n_addr);
    printf("0x%08lx\n", paddr_read(n_addr, 4));
    n_addr += 4;
  }
  return 0;
}
static int cmd_p(char *args)
{
  bool success;
  word_t res = expr(args, &success);
  if (success)
    printf("%s = \e[1;36m%lu\e[0m\n", args, res);
  return 0;
}

static int cmd_w(char *args)
{
  bool success = true;
  // printf("test w1");
  WP *point = new_wp(args, &success);
  // printf("test w2");
  if (!success)
  {
    printf("Some thing wrong happend.\n");
  }
  else
  {
    printf("Created a \e[1;36mWatchPoint(NO.%d)\e[0m: %s \n", point->NO, point->condition);
  }
  return 0;
}

static int cmd_d(char *args)
{
  int NO;
  sscanf(args, "%d", &NO);
  free_wp(NO);
  return 0;
}

// static int cmd_s(char *args)
// {
//   print_stack_trace();
//   return 0;
// }

static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Let the program single step through N instructions and pause execution", cmd_si},
    {"info", "info r prints register status, info w prints watchpoint information", cmd_info},
    {"x", "Scan memory", cmd_x},
    {"p", "Evaluate the value of an expression", cmd_p},
    {"w", "w EXPR, Pause program execution when the value of expression EXPR changes", cmd_w},
    {"d", "d N, Delete the watchpoint with sequence number N", cmd_d},
    // {"s", "s, print the current function call stack", cmd_s}
    /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode()
{
  is_batch_mode = true;
}

void sdb_mainloop()
{
  if (is_batch_mode)
  {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;)
  {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}

void init_sdb()
{
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
