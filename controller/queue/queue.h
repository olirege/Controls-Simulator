#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdlib.h>
#include "../sockets/sockets.h"
typedef struct {
    int socket_fd;
    ConnectionType type;
} Task;

typedef struct NodeTask {
    Task task;
    struct NodeTask *next;
} NodeTask;

typedef struct {
    NodeTask *head, *tail;
    pthread_mutex_t lock;
} Queue;

void queue_init(Queue *queue);
void queue_push(Queue *queue, Task task);
int queue_pop(Queue *queue, Task *task);
void worker_thread(void *arg);

#endif