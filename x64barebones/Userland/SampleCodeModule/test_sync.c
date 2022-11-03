
#include <test_sync.h>

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]){
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3){
    my_printf("malos parametros brooo");
    exit();
  }

  if ((n = atoi(argv[0])) <= 0){
    my_printf("malos parametros brooo");
    exit();
  }
  if ((inc = atoi(argv[1])) == 0){
    my_printf("malos parametros brooo");
    exit();
  }
  if ((use_sem = atoi(argv[2])) < 0){
    my_printf("malos parametros brooo");
    exit();
  }

  if (use_sem)
    if (!semOpen(SEM_ID, 1)){
      my_printf("test_sync: ERROR opening semaphore\n");
      exit();
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem) post(SEM_ID);
  }

  if (use_sem) semClose(SEM_ID);
  
  exit();
}

uint64_t test_sync(uint64_t argc, char *argv[]){ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  if (argc != 3){
    my_printf("malos parametros brooo");
    exit();
  }

  char * argvDec[] = {argv[1], "-1", argv[2]};
  char * argvInc[] = {argv[1], "1", argv[2]};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[i] = sys_scheduler(&my_process_inc,1,1,3, argvDec);//"my_process_inc", 3, argvDec
    pids[i + TOTAL_PAIR_PROCESSES] = sys_scheduler(&my_process_inc,1,1,3, argvInc);
  }


  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    wait(pids[i]);
    wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  my_printf("Final value: %d\n", global);

  exit();
  return 1;
}
