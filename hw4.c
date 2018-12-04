#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef sem_t semaphore;
#define true 1
#define down sem_wait
#define up sem_post
semaphore mutex;                   
semaphore db;                       
semaphore printer;                  
int rc = 0;                         

void read_data_base(int num)
{
    down(&printer);
    printf("Reader %d is reading data\n", num);
    up(&printer);
}

void use_data_read(int num)
{
    down(&printer);
    printf("Reader %d is using data\n", num);
    up(&printer);
    sleep(rand() % 3);
}
 
void think_up_data(int num)
{
    down(&printer);
    printf("Writer %d is making up data\n", num);
    up(&printer);
    sleep(rand() % 3);
}

void write_data_base(int num)
{
    down(&printer);
    printf("Writer %d is writing data\n", num);
    up(&printer);
}

void *reader(void *arg)
{
  int f = (int) (arg);
  while (true)
  {
    down(&mutex);
    rc = rc + 1;
    if(rc == 1)
      down(&db);
    up(&mutex);
    read_data_base(f);
    down(&mutex);
    rc = rc - 1;
    if(rc == 0)
      up(&db);
    up(&mutex);
    use_data_read(f);
  }
}

void *writer(void *arg)
{
  int f = (int) (arg);
  while (true)
  {
    think_up_data(f);
    down(&db);
    write_data_base(f);
    up(&db);
  }
}

int main()
{
  int i; 
  pthread_t rid[5], wid[5];
  
  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);
  sem_init(&printer, 0, 1);
  
  for (i = 0; i < 2; i++)
  {
    pthread_create(&wid[i], NULL, writer, (void*)(i+1));
    down(&printer);
    printf("Writer %d thread created\n", i+1);
    up(&printer);
  }

  for (i = 0; i < 5; i++)
  {
    pthread_create(&rid[i], NULL, reader, (void*)(i+1));
    down(&printer);
    printf("Reader %d thread created\n", i+1);
    up(&printer);
  }

  for (i = 0; i < 5; i++)
  {
    pthread_join(rid[i], NULL);
  }

  for (i = 0; i < 2; i++)
  {
    pthread_join(wid[i], NULL);
  }

  
  sem_destroy(&mutex);
  sem_destroy(&db);
  sem_destroy(&printer);
  
  return 0;
}