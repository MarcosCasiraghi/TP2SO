
#include <test_sync.h>

#define SEM_ID 55
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  // yield(); //This makes the race condition highly probable
  yield();
  aux += inc;
  *p = aux;
}

void my_process_inc(uint64_t argc, char *argv[]){
  uint64_t pid = sys_getPID();
  if(!semOpen(pid, 0)){
    my_printf("test_sync: ERROR opening semaphore arriba de todo\n");
    exit();
  }
  // wait(pid); no hay que hacer wait ya que semaforo arranca en 0

  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3){
    my_printf("Esta funcion recibe 3 parametros\nEl primero la cantidad de procesos\nEl segundo si es incremental o decremental\nEl tercero si se quiere usar semaforos o no\n");
    exit();
  }

  if ((n = atoi(argv[0])) <= 0){
    my_printf("La cantidad de procesos debe ser mayor a 0\n");
    exit();
  }
  if ((inc = atoi(argv[1])) != -1 && inc != 1){
    my_printf("El segundo parametro debe ser 1 o -1\n1 para hacerlo incremental\n-1 para hacerlo decremental\n");
    exit();
  }
  if ((use_sem = atoi(argv[2])) != 0 && use_sem != 1){
    my_printf("El tercer parametro debe ser 1 o 0 indicando si se quiere usar semaforos o no\n");
    exit();
  }

  if (use_sem)
    if (!semOpen(SEM_ID, 1)){
      my_printf("test_sync: ERROR opening semaphore\n");
      exit();
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) 
      wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem) 
      post(SEM_ID);
  }

  if (use_sem) 
    semClose(SEM_ID);
  
  post(pid);    //si esto no se corre, tira invalid opcode exception, error del programador
  semClose(pid);
  exit();
}

//recibe en argv[1] N (cantidad de ciclos), en argv[2] 0 o 1 si quiere usar semaforos o no
void test_sync(uint64_t argc, char *argv[]){ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  if (argc != 3){
    my_printf("Esta funcion debe recibir 2 parametros\n");
    exit();
  }
  int N = atoi(argv[1]);
  if( N <= 0){
    my_printf("El primer parametro es la cantidad de ciclos, debe ser mayor a 0\n");
    exit();
  }
  int semaphore = atoi(argv[2]);
  if(semaphore != 0 && semaphore != 1 ){
    my_printf("El segundo parametro indica si se quieren usar semaforos o no\nDebe ser 1 o 0\n");
    exit();
  }

  char * argvDec[] = {argv[1], "-1", argv[2]};
  char * argvInc[] = {argv[1], "1", argv[2]};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[i] = sys_scheduler(&my_process_inc,FOREGROUND,MEDIUM,3, argvInc);//"my_process_inc", 3, argvDec
    pids[i + TOTAL_PAIR_PROCESSES] = sys_scheduler(&my_process_inc,FOREGROUND,MEDIUM,3, argvDec);

    if (!semOpen(pids[i], 0)){
      my_printf("test_sync: ERROR opening semaphore\n");
      exit();
    }
    if (!semOpen(pids[i + TOTAL_PAIR_PROCESSES], 0)){
      my_printf("test_sync: ERROR opening semaphore\n");
      exit();
    }
  }

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    wait(pids[i]);
    // for( int j = 0 ; j < 1000000000 ; j++);
    wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }
  
  my_printf("Expected value: 0\n");
  if( global < 0){
    my_printf("Final value: -%d\n", -global);
  }else{
    my_printf("Final value: %d\n", global);
  }


  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    semClose(pids[i]);
    semClose(pids[i + TOTAL_PAIR_PROCESSES]);
  }
  exit();
}
