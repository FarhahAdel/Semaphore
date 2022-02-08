#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
int N,bcount=0, l=-1, h=-1,counter=0;
int const b=10;
int*buffer;
sem_t s,n,e,se;

typedef struct {
	int indx;
}args;

int generateRandNum(){
	return (random()%5)+1;
}

void insert(int x){
		if(h==-1)
			h=0;
		l++;
		if(l>=b)
			l=l%b;
		buffer[l]=x;
}

int delete(){
		int x=buffer[h];
		h++;
		if(h>=b)
			h=h%b;
		return x;
}
void append(int x){
	insert(x);
	printf("Monitor thread : Writing to buffer at position  %d  \n",l);
		
}
void take(){
	printf("Collector thread : reading from the buffer at position  %d \n",h);
	int x=delete();
	
	
}
void * monitorFunc (){
	while(1){
		sem_wait(&se);
		printf("Monitor thread : waiting to read counter \n");
		int x=counter;
		printf("Monitor thread : reading a count value of  %d  \n",x);
		counter=0;
		sem_post(&se);
		int val;
		sem_getvalue(&n,&val);
		if(val==b)
			printf("Monitor thread : Buffer full!!\n");
		else{
		
		sem_wait(&e);
		sem_wait(&s);
		append(x);
		sem_post(&s);
		sem_post(&n);
		}
		int r= generateRandNum();
		sleep(r*2);
	}
}
void * collectorFunc (){
	while(1){
		sem_wait(&n);
		sem_wait(&s);
		int val;
		sem_getvalue(&e,&val);
		if(val==b)
			printf("Collectore thread : nothing is on the buffer!! \n ");
		else{
		take();
		sem_post(&s);
		sem_post(&e);
		}
		int r= generateRandNum();
		sleep(r*2);
	}
}
void * counterFunc (void* arg){
	
	while(1){
		args *a=(args*)arg;
		int p = a->indx;
		printf("Counter thread %d : recieved a message \n",p);
		printf("Counter thread %d : waiting to write \n",p);
		sem_wait(&se);
		counter++;
		printf("Counter thread %d : now adding to counter, counter value= %d \n",p,counter);
		sem_post(&se);

		int r= generateRandNum();
		sleep(r);
	}
}	

void  main(){
srandom(time(NULL));
printf("Enter N: ");
scanf("%d",&N);
buffer=malloc(sizeof(int)*b);
args arg[N];
sem_init(&se,0,1);
sem_init(&s,0,1);
sem_init(&n,0,0);
sem_init(&e,0,b);
pthread_t mMonitor,mCollector,mCounter[N];
pthread_create(&mMonitor,NULL,monitorFunc,NULL);
pthread_create(&mCollector,NULL,collectorFunc,NULL);
for(int i=0;i<N;i++){
	arg[i].indx=i;
   pthread_create(&mCounter[i],NULL,counterFunc,&arg[i]);
}
pthread_join(mMonitor, NULL);
pthread_join(mCollector, NULL);
for(int i=0;i<N;i++)
   pthread_join(mCounter[i],NULL);
}

