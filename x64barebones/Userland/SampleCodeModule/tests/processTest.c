#include <processTest.h>

#define MAX_PROCESSES 10

#define READY 0
#define BLOCKED 1
#define KILLED 2

typedef struct P_rq {
  uint32_t pid;
  int state;
} p_rq;

void processTest() {
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  int counter = 0;

  while (1) {
    my_printf("Cycle Number: %d\n", counter++);
    for (rq = 0; rq < MAX_PROCESSES; rq++) {
      char *argv[] = {"Idle Process"};
      p_rqs[rq].pid = sys_scheduler(&idleProcess, BACKGROUND, MEDIUM, 1, argv );
      if (p_rqs[rq].pid == -1) {
        my_printf("Error creating process\n");
        exit();
      } else {
        p_rqs[rq].state = READY;
        alive++;
      }
    }

    while (alive > 0) {
      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        action = GetUniform(2) % 2;
        switch (action) {
          case 0:
            if (p_rqs[rq].state == READY || p_rqs[rq].state == BLOCKED) {
              if (sys_kill(p_rqs[rq].pid) == -1) {
                my_printf("Error killing process\n");
                exit();
              }
              p_rqs[rq].state = KILLED;
              alive--;
              busyWait(MAJOR_WAIT);
            }
            break;

          case 1:
            if (p_rqs[rq].state == READY) {
              if (sys_block(p_rqs[rq].pid) == -1) {
                my_printf("Error blocking process\n");
                exit();
              }
              p_rqs[rq].state = BLOCKED;
            }
            break;
        }
      }
      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2) {
          if (sys_unblock(p_rqs[rq].pid) == -1) {
            my_printf("Error unblocking process\n");
            exit();
          }
          p_rqs[rq].state = READY;
        }
      }
    }
  }
}