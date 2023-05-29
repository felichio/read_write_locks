#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "reader_writer.h"

struct wrapper
{
    struct reader_writer *rw;
    long rank;
};

void *reader(void *wrapper)
{
    struct wrapper *wrap = (struct wrapper *) wrapper;
    struct reader_writer *rw = wrap->rw;
    long rank = wrap->rank;
    printf("Attempting reading from thread: %ld\n", rank);
    reader_enterR(rw);
    printf("Reading!!! thread id: %ld\n", rank);
    reader_exitR(rw);
    printf("Finished reader thread: %ld\n", rank);
}

void *writer(void *wrapper)
{
    struct wrapper *wrap = (struct wrapper *) wrapper;
    struct reader_writer *rw = wrap->rw;
    long rank = wrap->rank;
    printf("Attempting writing from thread: %ld\n", rank);
    writer_enterR(rw);
    printf("Writing!!! thread id: %ld\n", rank);
    sleep(1);
    writer_exitR(rw);
    printf("Finished writer thread: %ld\n", rank);
}


int main()
{
    struct reader_writer rw;

    reader_writer_init(&rw);
    int size = 10;
    pthread_t readers[size];
    struct wrapper readers_wraps[size];
    pthread_t writers[size];
    struct wrapper writers_wraps[size];


    for (int i = 0; i < size; i++)
    {
        writers_wraps[i].rw = &rw;
        writers_wraps[i].rank = i;
        pthread_create(&writers[i], NULL, writer, &writers_wraps[i]);
    }


    for (int i = 0; i < size; i++)
    {
        readers_wraps[i].rw = &rw;
        readers_wraps[i].rank = i;
        pthread_create(&readers[i], NULL, reader, &readers_wraps[i]);
    }

    for (int i = 0; i < size; i++)
    {
        pthread_join(writers[i], NULL);
        pthread_join(readers[i], NULL);
    }
    


    return 0;
}
