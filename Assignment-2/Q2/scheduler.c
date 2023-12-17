#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int findPidIndex(pid_t pidArray[], int arraySize, pid_t targetPid) {
  for (int i = 0; i < arraySize; i++) {
    if (pidArray[i] == targetPid) {
      return i; // Return the index of the matching pid
    }
  }
  return -1; // Return -1 if not found
}

int main() {

  struct timespec start_time, end_time;
  int status; // store exit status of counters
  // start and execution times for each child process
  double process_start_time[3];
  double process_execution_time[3];
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  // Create three processes in parallel
  const char *process_names[3] = {"SCHED_FIFO", "SCHED_RR", "SCHED_OTHER"};
  pid_t childrenpids[3];
  for (int i = 0; i < 3; i++) {
    pid_t child_pid = fork();
    childrenpids[i] = child_pid;
    if (child_pid == 0) {
      // child
      struct timespec child_start_time, child_end_time;
      // stamp start
      clock_gettime(CLOCK_MONOTONIC, &child_start_time);
      if (i == 0) {
        // Set scheduling policy and priority for SCHED_OTHER
        // struct sched_param param;
        // param.sched_priority = 0;
        // sched_setscheduler(0, SCHED_OTHER, &param);
        // nice(0);
        execl("./sched_fifo", "sched_fifo", NULL);
      } else if (i == 1) {
        // Set scheduling policy and priority for SCHED_RR
        //      struct sched_param param;
        //      param.sched_priority = 0;
        //      sched_setscheduler(0, SCHED_RR, &param);
        execl("./sched_rr", "sched_rr", NULL);
      } else if (i == 2) {
        // Set scheduling policy and priority for SCHED_FIFO
        // struct sched_param param;
        // param.sched_priority = 0;
        // sched_setscheduler(0, SCHED_FIFO, &param);
        execl("./sched_other", "sched_other", NULL);
      }
      // execl("./count", "count", NULL);
      // perror("execl failed for child");
      // exit(1);
    } else if (child_pid < 0) {
      perror("fork failed");
      exit(1);
    }
    // Record the start time for the next child process
    process_start_time[i] =
        start_time.tv_sec + (double)start_time.tv_nsec / 1e9;
  }
  // Wait for each child process to complete and record their times as well as
  // catch exit status for potential error handling if needed using pointer to
  // status
  for (int i = 0; i < 3; i++) {
    pid_t childfinishingwhenever = wait(&status);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    // Calculate execution time for the current child process
    // process_execution_time[findPidIndex(childrenpids, 3,
    //                                    childfinishingwhenever)] =
    //    (end_time.tv_sec - start_time.tv_sec) +
    //    (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    process_execution_time[i] = (end_time.tv_sec - start_time.tv_sec) +
                                (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    // printf("%d\n", findPidIndex(childrenpids, 3, childfinishingwhenever));
  }
  for (int i = 0; i < 3; i++) {
    printf("Process %s execution time: %lf seconds\n", process_names[i],
           process_execution_time[i]);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  double total_execution_time = (end_time.tv_sec - start_time.tv_sec) +
                                (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
  printf("Total execution time: %lf seconds\n", total_execution_time);
  char str1[100];
  char str2[100];
  char str3[100];

  sprintf(str1, "%f", process_execution_time[0]);
  sprintf(str2, "%f", process_execution_time[1]);
  sprintf(str3, "%f", process_execution_time[2]);
  execl("/usr/bin/python3", "/usr/bin/python3", "seggs.py", str1, str2, str3,
        NULL);
  return 0;
}
