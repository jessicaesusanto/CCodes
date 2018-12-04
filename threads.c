#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#define NO_OF_THREADS 5

sem_t semaphore;


void *printHello(void *tid)
{
    printf("Hello from thread %d\n", *(int *)tid + 1);
    pthread_exit(NULL);
}

int main()
{
    sem_init(&semaphore, 0, 1);
    pthread_t threads[NO_OF_THREADS];
    int status, i, j;
  
    for (i = 0; i < NO_OF_THREADS; i++)
    {
      sem_wait(&semaphore);
      for (j = 0; j < i + 1; j++)
      {
          status = pthread_create(&threads[i], NULL, printHello, &(i));
        
            if (status != 0)
            {
              printf("Oops, pthread_create returned error code %d", status);
            }
      }
      pthread_join(threads[i], NULL);
      sem_post(&semaphore);
    }

    sem_destroy(&semaphore);

    return 0;
}
  