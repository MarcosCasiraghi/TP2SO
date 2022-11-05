#include <priorityTest.h>

#define TOTAL_PROCESSES 3


void priorityTest(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

my_printf("Creating Processes\n\n");
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    char *argv[] = {"Idle Process"};
    pids[i] = sys_scheduler( &idleProcess, BACKGROUND, MEDIUM, 1, argv);
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  my_printf("Changing Priorites...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    switch (i % 3) {
      case 0:
        sys_nice(pids[i], LOWEST );  // lowest priority
        break;
      case 1:
        sys_nice(pids[i], MEDIUM);  // medium priority
        break;
      case 2:
        sys_nice(pids[i], HIGHEST);  // highest priority
        break;
    }
  }

   busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  my_printf("Blocking...\n\n");
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_block(pids[i]);
  }

  my_printf("Changing priorities while blocked...\n\n");
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_nice(pids[i], MEDIUM);
  }

  my_printf("Unblocking...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_unblock(pids[i]);
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);
  my_printf("Killing...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_kill(pids[i]);
  }
  exit();
}