

struct reader_writer
{
    unsigned int readers_active;
    unsigned int readers_waiting;
    bool writer_active;
    unsigned int writers_waiting;
    pthread_mutex_t mut;
    pthread_cond_t con;
};

void reader_writer_init(struct reader_writer *rw);

void reader_enterR(struct reader_writer *rw);
void reader_exitR(struct reader_writer *rw);
void reader_enterW(struct reader_writer *rw);
void reader_exitW(struct reader_writer *rw);
void writer_enterR(struct reader_writer *rw);
void writer_exitR(struct reader_writer *rw);
void writer_enterW(struct reader_writer *rw);
void writer_exitW(struct reader_writer *rw);