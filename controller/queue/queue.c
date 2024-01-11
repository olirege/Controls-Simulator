#include "queue.h"
#include "../sockets/sockets.h"
void queue_init(Queue *queue){
    // Initialize the queue
    queue->head = queue->tail = NULL;
    pthread_mutex_init(&queue->lock, NULL);
};
void queue_push(Queue *queue, Task task){
    // Add task to queue
    NodeTask *new_node = malloc(sizeof(NodeTask));
    new_node->task = task;
    new_node->next = NULL;

    pthread_mutex_lock(&queue->lock);
    if (queue->tail) {
        queue->tail->next = new_node;
    } else {
        queue->head = new_node;
    }
    queue->tail = new_node;
    pthread_mutex_unlock(&queue->lock);
};
int queue_pop(Queue *queue, Task *task){
    // Remove task from queue
    pthread_mutex_lock(&queue->lock);
    if (queue->head == NULL) {
        pthread_mutex_unlock(&queue->lock);
        return 0; // queue is empty
    }
    *task = queue->head->task;
    NodeTask *tmp = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(tmp);
    pthread_mutex_unlock(&queue->lock);
    return 1; // Success
};
void worker_thread(void *arg){
    Queue *queue = (Queue *)arg;
    Task task;
    while (1) {
        // Get task from queue
        if (queue_pop(queue, &task)) {
            // Process the task
            handle_client(task.socket_fd);
        }
    }
    return NULL;
};
void free_queue(Queue *queue){
    // Free memory
    NodeTask *tmp;
    while (queue->head != NULL) {
        tmp = queue->head;
        queue->head = queue->head->next;
        free(tmp);
    }
    pthread_mutex_destroy(&queue->lock);
};