#include <priorityTest.h>

#define TOTAL_PROCESSES 3


void priorityTest(){
    uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    char *argv[] = {"Idle Process"};
    pids[i] = sys_scheduler(*argv, &idleProcess, FOREGROUND, MEDIUM, '\0');
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  my_printf(
      "\nChanging Priorites...\n\n");

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

//   busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  my_printf("\nBLOCKING...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_block(pids[i]);
  }

  my_printf(
      "\nCHANGING PRIORITIES WHILE BLOCKED...\n\n");
  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_nice(pids[i], MEDIUM);
  }

  my_printf("\nUNBLOCKING...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_block(pids[i]);
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);
  my_printf("\nKILLING...\n\n");

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    sys_kill(pids[i]);
  }
}