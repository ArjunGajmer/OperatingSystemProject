#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#define TRUE 1
sem_t full,empty,mutex;
int BUFFER_SIZE=10;
int front,rear;
long randomise;
void * consumer();
void * producer();
int insert_item(int);
void remove_item();
pthread_t tid[20];
int *buffer;
int main()
{
    int main_sleep_time,no_of_thread;
    printf("Enter the number of main_sleep_time\t");
    scanf("%d",&main_sleep_time);
    printf("Enter the number of thread\t");
    scanf("%d",&no_of_thread);
    if(no_of_thread>20)
    {
        printf("Please,limit the number of thread created");
    }
    else{
    buffer=(int*)calloc(BUFFER_SIZE,sizeof(int));
    sem_init(&mutex,0,1);
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    front=-1;
    rear=-1;
    randomise=100000000000;
    int i;
    for(i=0;i<no_of_thread;i++)
    {
         pthread_create(&tid[i],NULL,producer,NULL);
         i++;
         pthread_create(&tid[i],NULL,consumer,NULL);
        
    }
    for(i=0;i<no_of_thread;i++)
    {
        pthread_join(tid[i],NULL);
    }
    sleep(main_sleep_time);
    printf("Exit the program\n");
    exit(0);
    }
}

void *producer()    
{  
	    do{
		int item;
        int random_sleep_time= rand()/randomise;    //to generate random sleep time
        sleep(random_sleep_time);            
        item=rand()%100;                  //generates random item
        sem_wait(&empty);
        sem_wait(&mutex);
        int x=insert_item(item);
        if(x==0)
        {
            printf("\n%d PRODUCED SUCCESSFULLY IN BUFFER",item);
        }
        else
        {
            printf("\nPRODUCTION FAILED");
        }
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
int insert_item(int item)
{
        if(rear == -1)
            {
                rear=0;
                front=0;
            }
        else if(rear == BUFFER_SIZE -1)    
            rear=0;
        else
            rear=rear+1;
            buffer[rear]=item;
        return 0;
}  
void  remove_item()
{
       printf("\n%d IS CONSUMNED FROM THE BUFFER",buffer[front]);
        if(front == rear)
            {
                front=-1;
                rear=-1;
            }
        else if(front ==BUFFER_SIZE-1)
        {
            front=0;
        }
        else
        {
            front++;
        }
    
}

