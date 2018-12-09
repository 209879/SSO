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

int forks[PHILOSOPHERS_AMOUNT+1];
int output;				
int mutex;				

int pid;	
int semgroup;	

struct shared_memory {
	int forks_pos[PHILOSOPHERS_AMOUNT];
} *shared;


void eat() 
{}

void think() 
{}


void philosopher(int n) {
	pid = n;
	for(;;) {
		think();
		eat();
	}
}

int main() {
	int i;
	int shmid;
	int shmret;

	shmid = shmget(IPC_PRIVATE, sizeof(*shared),
			IPC_CREAT|SHM_W|SHM_R);

	printf("shmid: %i \n", shmid);
	
	shared = (struct shared_memory *)  shmat(shmid, (char *)0, 0);
	semgroup = semget(IPC_PRIVATE, PHILOSOPHERS_AMOUNT, IPC_CREAT|SEM_A|SEM_R);
	
	printf("semgroup: %i \n", semgroup);
	

	forks[PHILOSOPHERS_AMOUNT] = forks[0];

	for (i=1; i<PHILOSOPHERS_AMOUNT; i++) {
		if (!fork())
			philosopher(i);
		srand(rand());
	}
	philosopher(0);

	return 0;
}
