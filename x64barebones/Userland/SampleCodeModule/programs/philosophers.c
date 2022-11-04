
#include <philosophers.h>


typedef enum { THINKING, HUNGRY, EATING } t_philo_state;

typedef struct t_philosofer {
  int pid;
  int sem;
  int ID;
  t_philo_state state;
} t_philosofer;

t_philosofer *philosophers[MAX_PHILOS];
static int philosopherCount = 0;
static int mutex;
static int tableOpen;

#define LEFT(i) (((i) + philosopherCount - 1) % philosopherCount)
#define RIGHT(i) (((i) + 1) % philosopherCount)

static void thinkOrEat();
static void philoMain(int argc, char **argv);
static void takeForks(int i);
static void putForks(int i);
static void test(int i);
static int addPhilo();
static int removePhilo();
static void printTable(int argc, char **argv);
static void printPhyloHeader();

void philoProblem() {
  philosopherCount = 0;
  tableOpen = 1;
  if(semOpen(MUTEX_SEM_ID, 1) == -1){
    my_printf("error abriendo semaforo\n");
    exit();
  }


  printPhyloHeader();

  int i = 0;
  while (i < INITIAL_PHILOS) {
    addPhilo();
    i++;
  }

  my_printf("\nVamos a dejar comer a los filosofos iniciales por %d segundos.\n\n",
         FRONTEND_WAIT_SECONDS);

  char *args[] = {"Phylo Table"};
  int tablePID = sys_scheduler(&printTable, FOREGROUND, HIGHEST, 1, args);

  sleep(FRONTEND_WAIT_SECONDS);

  my_printf("\nYa puede agregar o retirar comensales y terminar la cena.\n\n");

  while (tableOpen) {
    // my_printf("getchar antes\n");
    char key = getChar();
    // my_printf("getchar despues\n");
    switch (key) {
      case 'a':
        if (addPhilo() == -1) {
          my_printf("\nNo hay mas lugar en la mesa.\n\n");
        } else {
          my_printf("\nSe agrego un comensal.\n\n");
        }
        break;
      case 'r':
        if (removePhilo() == -1) {
          my_printf("\nPor favor no se vaya, ya llega el postre.\n\n");
        } else {
          my_printf("\nSe retiro un comensal.\n\n");
        }
        break;
      case 'q':
        my_printf(
            "\nMesa cerrada, cuenta: $999999, esperamos hayan disrutado "
            "su\ncomida.\n\n");
        tableOpen = 0;
        break;
      default:
        break;
    }
  }

  for (int i = 0; i < philosopherCount; i++) {
    semClose(philosophers[i]->sem);
    sys_kill(philosophers[i]->pid);
    sys_free(philosophers[i]);
  }
  sys_kill(tablePID);
  semClose(MUTEX_SEM_ID);
  exit();
}

static int addPhilo() {
  if (philosopherCount == MAX_PHILOS) {
    return -1;
  }

  wait(MUTEX_SEM_ID);
  t_philosofer *philosopher = sys_malloc(sizeof(t_philosofer));
  if (philosopher == NULL) {
    return -1;
  }
  philosopher->sem = semOpen(FILO_SEM_ID + philosopherCount, 1);
  if(philosopher->sem == -1){
    my_printf("error abriendo semaforo\n");
    exit();
  }
  philosopher->state = THINKING;
  philosopher->ID = philosopherCount;

  char index[3] = {0};
  itoa(philosopherCount,index,10);

  // char *argv[] = {"philosohper", NULL};
  char ** argv = sys_malloc(2*sizeof(char *));
  argv[0] = "philosopher";
  argv[1] = sys_malloc(3*sizeof(char));
  argv[1][0] = index[0];
  argv[1][1] = index[1];
  argv[1][2] = index[2];

  philosopher->pid = sys_scheduler(&philoMain, FOREGROUND,MEDIUM,2, argv);

  sys_free(argv[1]);
  sys_free(argv);

  philosophers[philosopherCount++] = philosopher;

  post(MUTEX_SEM_ID);
  return 0;
}

static int removePhilo() {
  if (philosopherCount == INITIAL_PHILOS) {
    return -1;
  }
  
  // my_printf("remove antes\n");
  wait(MUTEX_SEM_ID);

  t_philosofer *philosopher = philosophers[--philosopherCount];
  semClose(philosopher->sem);
  sys_kill(philosopher->pid);
  sys_free(philosopher);

  post(MUTEX_SEM_ID);
  // my_printf("remove despues\n");

  return 0;
}

static void philoMain(int argc, char **argv) {
  // my_printf("soy%d recibi%s\n", sys_getPID()-2,argv[1]);
  int i = atoi(argv[1]);
  // my_printf("chau%d\n", i);
  while (1) {
    // if(i == 4)
    //   my_printf("start\n");
    takeForks(i);
    // if(i == 4)
    //   my_printf("after Take Forks\n");
    thinkOrEat();
    // if(i == 4)
    //   my_printf("after Think or Eat\n");
    putForks(i);
    // if(i == 4)
    //   my_printf("after putforks\n");
    thinkOrEat();
    // if(i == 4)
    //   my_printf("after Think or Eat 2\n");
  }
}

static void takeForks(int i) {
  wait(MUTEX_SEM_ID);
  philosophers[i]->state = HUNGRY;
  test(i);
  post(MUTEX_SEM_ID);
  wait(philosophers[i]->sem);
}

static void putForks(int i) {
  wait(MUTEX_SEM_ID);
  philosophers[i]->state = THINKING;
  test(LEFT(i));
  test(RIGHT(i));
  post(MUTEX_SEM_ID);
}

static void test(int i) {
  if (philosophers[i]->state == HUNGRY &&
      philosophers[LEFT(i)]->state != EATING &&
      philosophers[RIGHT(i)]->state != EATING) {
    philosophers[i]->state = EATING;
    post(philosophers[i]->sem);
  }
}

static void thinkOrEat() { sleep(THINK_EAT_WAIT_SECONDS); }

static void printTable(int argc, char **argv) {
  while (tableOpen) {
    // my_printf("print antes\n");
    wait(MUTEX_SEM_ID);
    int i;
    for (i = 0; i < philosopherCount; i++) {
      if (philosophers[i]->state == EATING) {
        put_char('E');
      } else{
        put_char('-');
      }
      put_char(' ');
    }
    put_char('\n');
    post(MUTEX_SEM_ID);
    // my_printf("print despues\n");
    // sys_exec();
    // sys_yield();
    yield();
  }
  // my_printf("no deberia estar aca\n");
}

static void printPhyloHeader() {
  my_printf("Bienvenido al problema de los filosofos comensales.\n");
  my_printf( "Use A para agregar un filosofo\n");
  my_printf( "Use R para remover un filosofo\n");
  my_printf( "Use Q para finalizar\n");
  sleep(FRONTEND_WAIT_SECONDS);
}



