/*
 * POSIX thread test
 */
#include <pthread.h> // pthread functions. need '-lpthread' option during compile
                     // https://bitsoul.tistory.com/156
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

struct thread_info {
    const char * msg;
};

static void *
thread_function(void *_arg)
{
    struct thread_info *info = _arg;
    printf("Thread 1 runs, msg was `%s'\n", info->msg);
    return (void *) 42;
}

int
main()
{
    struct thread_info info = { .msg = "Hello, Thread" };

    pthread_t t;
    pthread_create(&t, NULL, thread_function, &info); // pass function & pointer (argument of new function)

    uintptr_t status;
    pthread_join(t, (void **) &status); // similar function with "waitpid" ... block 
    printf("Thread returned status %lu\n", status);
    return 0;
}