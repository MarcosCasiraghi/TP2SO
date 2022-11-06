
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
static void pipePhyloHeader(int pipeId);
static void pipeTable(int argc, char **argv);

void philoProblem(int argc, char ** argv) {
    philosopherCount = 0;
    tableOpen = 1;
    int tablePID;

    if (argc == 3) {
        int pipeId = pipeOpen(atoi(argv[1]));

        if (semOpen(MUTEX_SEM_ID, 1) == -1) {
            pipeWrite(pipeId,"error abriendo semaforo\n");
            exit();
        }

        pipePhyloHeader(pipeId);

        int i = 0;
        while (i < INITIAL_PHILOS) {
            addPhilo();
            i++;
        }

        pipeWrite(pipeId,"\nVamos a dejar comer a los filosofos iniciales por 2 segundos.\n\n");

        char buffer[10];
        itoa(pipeId, buffer, 10);

        char *args[] = {"Phylo Table", buffer};
        tablePID = sys_scheduler(&pipeTable, BACKGROUND, HIGHEST, 2, args);

        sleep(FRONTEND_WAIT_SECONDS);

        pipeWrite(pipeId,"\nYa puede agregar o retirar comensales y terminar la cena.\n\n");

        while (tableOpen) {
            char key = getChar();
            switch (key) {
                case 'a':
                    if (addPhilo() == -1) {
                        pipeWrite(pipeId,"\nNo hay mas lugar en la mesa.\n\n");
                    } else {
                        pipeWrite(pipeId,"\nSe agrego un comensal.\n\n");
                    }
                    break;
                case 'r':
                    if (removePhilo() == -1) {
                        pipeWrite(pipeId,"\nPor favor no se vaya, ya llega el postre.\n\n");
                    } else {
                        pipeWrite(pipeId,"\nSe retiro un comensal.\n\n");
                    }
                    break;
                case 'q':
                    pipeWrite(pipeId,
                            "\nMesa cerrada, cuenta: $999999, esperamos hayan disrutado "
                            "su\ncomida.\n\n");
                    tableOpen = 0;
                    break;
                default:
                    break;
            }
        }


    }
    else{
        if (semOpen(MUTEX_SEM_ID, 1) == -1) {
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
        tablePID = sys_scheduler(&printTable, FOREGROUND, HIGHEST, 1, args);

        sleep(FRONTEND_WAIT_SECONDS);

        my_printf("\nYa puede agregar o retirar comensales y terminar la cena.\n\n");

        while (tableOpen) {
            char key = getChar();
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

    wait(MUTEX_SEM_ID);
    t_philosofer *philosopher = philosophers[--philosopherCount];
    post(MUTEX_SEM_ID);
    post(MUTEX_SEM_ID);
    semClose(philosopher->sem);
    sys_kill(philosopher->pid);
    sys_free(philosopher);

  return 0;
}

static void philoMain(int argc, char **argv) {
  int i = atoi(argv[1]);
  while (1) {
    takeForks(i);
    thinkOrEat();
    putForks(i);
    thinkOrEat();
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
  }
}

static void pipeTable(int argc, char **argv){
    int pipeId = atoi(argv[1]);
    while (tableOpen) {
        wait(MUTEX_SEM_ID);
        int i;
        for (i = 0; i < philosopherCount; i++) {
            if (philosophers[i]->state == EATING) {
                pipeWrite(pipeId,"E");
            } else{
                pipeWrite(pipeId,"-");
            }
            pipeWrite(pipeId," ");
        }
        pipeWrite(pipeId,"\n");
        post(MUTEX_SEM_ID);
        yield();
    }
}

static void printPhyloHeader() {
  my_printf("Bienvenido al problema de los filosofos comensales.\n");
  my_printf( "Use A para agregar un filosofo\n");
  my_printf( "Use R para remover un filosofo\n");
  my_printf( "Use Q para finalizar\n");
  sleep(FRONTEND_WAIT_SECONDS);
}

static void pipePhyloHeader(int pipeId) {
    pipeWrite(pipeId,"Bienvenido al problema de los filosofos comensales.\n");
    pipeWrite(pipeId, "Use A para agregar un filosofo\n");
    pipeWrite(pipeId, "Use R para remover un filosofo\n");
    pipeWrite(pipeId, "Use Q para finalizar\n");
    sleep(FRONTEND_WAIT_SECONDS);
}



