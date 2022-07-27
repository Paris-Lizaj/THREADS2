#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
//GLOBAL
int max,flag=1;
int N;
int A[4][4]={{13,2,5,3},{1,3,4,6},{1,2,35,4},{1,21,3,7}};//Wrong table
int B[4][4]; 
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t bar;
void * thread_check(void * arg)
{
int id=(int)arg;
int j,sum=0;
for(j=0;j<N;j++)
{
	if (id==j) continue;
	sum=sum+abs(A[id][j]);
}

pthread_mutex_lock(&mut);
	if(sum>abs(A[id][id]))	//an den einai
		flag=0;
pthread_mutex_unlock(&mut);
return NULL;
}


void * thread_max(void *arg)//vriskontas to maximum diagwnio stoixeio
{
int i,local_max=abs(A[0][0]);
for(i=1;i<N;i++)
{
	if (abs(A[i][i])>local_max)
		local_max=abs(A[i][i]);
}
max=local_max;
return NULL;
}


void * thread_function_createB(void *arg)
{
int id=(int)arg;//type casting 
int i;
int T[N];
for(i=0;i<N;i++)
{
	if (id==i)
		T[id]=max;
	else
		T[i]=max-abs(A[id][i]);
}
pthread_barrier_wait(&bar);
for(i=0;i<N;i++)
	B[id][i]=T[i];
return NULL;
}

int main(int argc,char *argv[])
{
if (argc !=2) // WRONG NUMBER OF ARGUMENTS
    {
        printf("I NEED EXACTLY 2 ARGUMENTS");
	exit(2);
    }
N=atoi(argv[1]);
max=A[0][0];
pthread_t pid[N];
int i,j;
for(i=0;i<N;i++)
	pthread_create(&pid[i],NULL,thread_check,(void *)i);
for(i=0;i<N;i++)
	pthread_join(pid[i],NULL);
if(!flag)
printf("O pinakas den einai aystira diagvnia despozwn");
else
{
pthread_t pid2;
pthread_create(&pid2,NULL,thread_max,NULL);
pthread_join(pid2,NULL);
printf("MAX: %d \n", max);
pthread_t pidb[N];
pthread_barrier_init(&bar,NULL,N);
for(i=0;i<N;i++)
	pthread_create(&pidb[i],NULL,thread_function_createB,(void *)i);
for(i=0;i<N;i++)
	pthread_join(pidb[i],NULL);
for(i=0;i<N;i++){
for(j=0;j<N;j++)
	printf("%d ",B[i][j]);
printf("\n");}
}
return 0;
}
