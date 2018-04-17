#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#define TRUE 1
sem_t full,empty,mutex;
int BUFFER_SIZE=6;
int front,rear;
long randomise;
void * consumer();
void * producer();
void insert_item(int);
void remove_item();
pthread_t tidp[20],tidc[20];
int *buffer;
int main()
{
    int main_sleep_time,no_of_thread_producer,no_of_thread_consumer;
    printf("Enter the number of main_sleep_time\t");
    scanf("%d",&main_sleep_time);
    printf("Enter the number of producer thread\t");
    scanf("%d",&no_of_thread_producer);
	printf("Enter the number of thread consumer thread\t");
	scanf("%d",&no_of_thread_consumer);
    main_sleep_time/=100;
	 
    if(no_of_thread_producer>20||no_of_thread_consumer>20)
    {
        printf("Please,limit the number of thread created");
    }
   
    else

    {	if(no_of_thread_consumer>no_of_thread_producer)
		no_of_thread_consumer=no_of_thread_producer;
        buffer=(int*)calloc(BUFFER_SIZE,sizeof(int));
        sem_init(&mutex,0,1);
        sem_init(&empty,0,BUFFER_SIZE);
        sem_init(&full,0,0);
        front=-1;
        rear=-1;
        randomise=100000000000;
        int i;
        for(i=0;i<no_of_thread_producer;i++)
		 pthread_create(&tidp[i],NULL,producer,NULL);
        for(i=0;i<no_of_thread_consumer;i++)
		 pthread_create(&tidc[i],NULL,consumer,NULL);
        for(i=0;i<no_of_thread_producer;i++)
           pthread_join(tidp[i],NULL);
        for(i=0;i<no_of_thread_consumer;i++)
		 pthread_join(tidc[i],NULL);
    }
    sleep(main_sleep_time);
    printf("\nExit the program\n");
    exit(0);
}
void *producer()    
{
    do{
	int item;
        int random_sleep_time= rand()/randomise;    //to generate random sleep time
        sleep(random_sleep_time);            
        item=rand()%100;  //generates random item
        sem_wait(&empty);
        sem_wait(&mutex);
        insert_item(item);
        sem_post(&mutex);
        sem_post(&full);
    	pthread_exit(0);
    }while(TRUE);
}
void *consumer()

{     do{
        int random_sleep_time= rand()/randomise;
        sleep(random_sleep_time);
        sem_wait(&full);
        sem_wait(&mutex);
        remove_item();
        sem_post(&mutex);
        sem_post(&empty);
        pthread_exit(0);
    }while(TRUE);
}
void  insert_item(int item)
{
             if(rear == BUFFER_SIZE -1 && front!=0)
                 rear=-1;
            rear=rear+1;
            buffer[rear]=item;
            if(front==-1)
            front=0;
		printf("\n%d IS PRODUCED IN THE BUFFER",item);
		display();
           
}
void  remove_item()
{
       printf("\n%d IS CONSUMNED FROM THE BUFFER",buffer[front++]);
         if(front ==BUFFER_SIZE)
        {
            front=0;
        }
        if(front-1==rear)
        {
            front=-1;
            rear=-1;
        }
	display();
}
void display()
{   printf("\t\t");
	if(front == -1 || rear==-1)
      printf("Circular Queue is Empty!!!");
     else{
      int i = front;
      if(front <= rear)
      {
	    while(i <= rear)
	    printf("%d  ",buffer[i++]);
      }
      else
      {
	    while(i <= BUFFER_SIZE - 1)
	    printf("%d ", buffer[i++]);
	    i = 0;
	    while(i <= rear)
	    printf("%d ",buffer[i++]);
      }
   }
}






