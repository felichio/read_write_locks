#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "reader_writer.h"

void reader_writer_init(struct reader_writer *rw)
{
    rw->readers_active = 0;
    rw->readers_waiting = 0;
    rw->writer_active = false;
    rw->writers_waiting = 0;
    pthread_mutex_init(&rw->mut, NULL);
    pthread_cond_init(&rw->con, NULL);
}

void reader_enterR(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->readers_waiting++;
    while (rw->writer_active)
        pthread_cond_wait(&rw->con, &rw->mut);
    rw->readers_waiting--;
    rw->readers_active++;
    pthread_mutex_unlock(&rw->mut);
}

void reader_exitR(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->readers_active--;
    if (rw->readers_active == 0)
        pthread_cond_signal(&rw->con);
    pthread_mutex_unlock(&rw->mut);
}

void writer_enterR(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    while (rw->readers_active > 0 || rw->readers_waiting > 0 || rw->writer_active)
        pthread_cond_wait(&rw->con, &rw->mut);
    rw->writer_active = true;
    pthread_mutex_unlock(&rw->mut);

}

void writer_exitR(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->writer_active = false;
    pthread_cond_broadcast(&rw->con);
    pthread_mutex_unlock(&rw->mut);
}


void reader_enterW(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    while (rw->writers_waiting > 0 || rw->writer_active)
        pthread_cond_wait(&rw->con, &rw->mut);
    rw->readers_active++;
    pthread_mutex_unlock(&rw->mut);
}

void reader_exitW(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->readers_active--;
    if (rw->readers_active == 0)
        pthread_cond_signal(&rw->con);
    pthread_mutex_unlock(&rw->mut);
}


void writer_enterW(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->writers_waiting++;
    while (rw->readers_active > 0 || rw->writer_active)
        pthread_cond_wait(&rw->con, &rw->mut);
    rw->writers_waiting--;
    rw->writer_active = true;
    pthread_mutex_unlock(&rw->mut);
}

void writer_exitW(struct reader_writer *rw)
{
    pthread_mutex_lock(&rw->mut);
    rw->writer_active = false;
    pthread_cond_broadcast(&rw->con);
    pthread_mutex_unlock(&rw->mut);
}
