/*
 * modified to keep order (syncronization)
 */
#include <pthread.h> // pthread functions. need '-lpthread' option during compile
                     // https://bitsoul.tistory.com/156
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int coin_flip;          // a shared global variable to hold the outcome
                        // of the coin flip, 0 or 1.
volatile bool coin_flip_done;    // --> new variable is introduced

static void *
thread1(void *_)
{
    coin_flip = rand() % 2;
    coin_flip_done = true;  // --> set variable
    asm volatile("" ::: "memory"); // compiler barrier
    printf("thread 1: flipped coin %d\n", coin_flip);
    return NULL;
}

static void *
thread2(void *_)
{
    while (!coin_flip_done) // --> check variable
    continue;
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
    pthread_create(&t[1], NULL, thread2, NULL);
    pthread_create(&t[0], NULL, thread1, NULL);

    for (int i=0; i<N; i++)
    pthread_join(t[i], NULL);
    return 0;
}
