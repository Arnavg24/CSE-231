#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
  // sleep(1);
  struct sched_param param;
  param.sched_priority = 99;
  sched_setscheduler(99, SCHED_FIFO, &param);
  execl("./count", "count", NULL);
  perror("execl failed for child");

  exit(1);
}
