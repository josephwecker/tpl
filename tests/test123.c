#include <setjmp.h>
#include <stdio.h>
#include <stdarg.h>
#include "tpl.h"

jmp_buf env;
extern tpl_hook_t tpl_hook;

void catch_fatal(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  longjmp(env,-1);                /* return to setjmp point */
}

int main() {
  int err;
  tpl_node *tn;
  tpl_hook.fatal = catch_fatal;    /* install fatal handler */

  err = setjmp(env); /* on error, control will return here  */
  if (err) {
    printf("caught error!\n");
    return -1;
  }

  tn = tpl_map("@");              /* generate a fatal error */
  printf("program ending, without error\n");
  return 0;
}
