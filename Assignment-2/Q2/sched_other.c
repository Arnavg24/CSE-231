#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
  struct sched_param param;
  param.sched_priority = 0;
  sched_setscheduler(0, SCHED_OTHER, &param);
  nice(0);
  execl("./count", "count", NULL);
  perror("execl failed for child");
  exit(1);
}
