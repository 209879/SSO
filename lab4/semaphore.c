#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define	SEM_A	0200	// alter 
#define	SEM_R	0400	// read
#define	SHM_W	0200	// write

#define PHILOSOPHERS_AMOUNT 5

#define EATING      3
#define THINKING    3
#define LEFT        2              
#define RIGHT       2 

short forks[PHILOSOPHERS_AMOUNT+1];
short output;	
short mutex;	

int pid;
int semgroup;

struct shared_memory {
	int forks_pos[PHILOSOPHERS_AMOUNT];
} *shared;


void put_down(int semid) {
	struct sembuf semopr;

	semopr.sem_num = semid;
	semopr.sem_op = -1;
	semopr.sem_flg = 0;
	semop(semgroup, &semopr, 1);

	if (semid <= PHILOSOPHERS_AMOUNT)
		shared->forks_pos[semid] = pid;
}

void take_up(int semid) {
	struct sembuf semopr;

	if (semid <= PHILOSOPHERS_AMOUNT)
		shared->forks_pos[semid] = -1;

	semopr.sem_num = semid;
	semopr.sem_op = 1;
	semopr.sem_flg = 0;
	semop(semgroup, &semopr, 1);
}

void msg(char *s)
{
	put_down(output);
	printf("Nr %d %s ", pid, s);
	take_up(output);
}


void eat(int n) {
	msg("hungry.");
	put_down(forks[n+1]);
	msg("takes right fork.");
	sleep(rand() % RIGHT );
  	put_down(forks[n]);
	msg("takes left fork, begins eating.");
	sleep(rand() % EATING);
	take_up(forks[n+1]);
	take_up(forks[n]);     
	msg("returns forks.");
}

void think() {
	msg("thinking.");
	sleep(rand() % THINKING);
}

void philosopher(int n) {
	pid = n;
	for(;;) {
		think();
		eat(pid);
	}
}


int main() {
	int i;
	int shmid;
	int shmret;

	shmid = shmget(IPC_PRIVATE, sizeof(*shared),
			IPC_CREAT|SHM_W|SHM_R);

	shared = (struct shared_memory *)  shmat(shmid, (char *)0, 0);
	semgroup = semget(IPC_PRIVATE, PHILOSOPHERS_AMOUNT+3, IPC_CREAT|SEM_A|SEM_R);
	
	for (i=0; i<=PHILOSOPHERS_AMOUNT; i++) {
	    take_up(i);
		forks[i]=i;
		shared->forks_pos[i] = -1;
	}

	forks[PHILOSOPHERS_AMOUNT] = forks[0];
	
	take_up(PHILOSOPHERS_AMOUNT+1);
	take_up(PHILOSOPHERS_AMOUNT+2);

	mutex =  PHILOSOPHERS_AMOUNT+2;
	for (i=1; i<PHILOSOPHERS_AMOUNT; i++) {
		if (!fork())
			philosopher(i);
		srand(rand());
	}
	philosopher(0);

	return 0;
}