/*
 * modified to keep order (syncronization)
 */
#include <pthread.h> // pthread functions. need '-lpthread' option during compile
                     // https://bitsoul.tistory.com/156
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>  // control shared resources access
                        // avoid the waste of resources
                        // https://www.crocus.co.kr/487

// PRODUCER ///////////////////////////////////////////////////////////

int coin_flip;          // a shared global variable to hold the outcome
                        // of the coin flip, 0 or 1.
sem_t coin_flip_done;    // --> variable is semaphored

static void *
thread1(void *_)
{
    coin_flip = rand() % 2;
    sem_post(&coin_flip_done);  // raise semaphore, increment, 'up'
    printf("thread 1: flipped coin %d\n", coin_flip);
    return NULL;
}

// CONSUMER ///////////////////////////////////////////////////////////

static void *
thread2(void *_)
{
    // wait until semaphore is raised, then decrement, 'down'
    sem_wait(&coin_flip_done); // --> check variable
    printf("thread 2: flipped coin %d\n", coin_flip);
    return NULL;
}

/* The main function starts thread 1 and 2 and then joins
 * them, i.e., waits for them to finish.
 */
int
main()
{
    const int N = 2;
    pthread_t t[N];
    srand(getpid());
    sem_init(&coin_flip_done, 0, /* initial value */ 0); // initialize semaphore
        // initial value 0 --> ordering, 1 --> mutex
    pthread_create(&t[1], NULL, thread2, NULL);
    pthread_create(&t[0], NULL, thread1, NULL);

    for (int i=0; i<N; i++)
    pthread_join(t[i], NULL);
    return 0;
}