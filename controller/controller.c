#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./sockets/sockets.h"
#include "queue/queue.h"
#include "utils/utils.h"
#define SERVER_PORT 6543
#define NUM_THREADS 5

int main()
{
    // Create server
    int server_fd, new_socket;
    setbuf(stdout, NULL);
    sleep(5);
    log(__func__, "Controller on");
    server_fd = start_server(SERVER_PORT);
    log(__func__, "Server started on port %d", SERVER_PORT);

    // Create queue
    log(__func__, "Creating queue");
    pthread_t threads[NUM_THREADS];
    Queue queue;
    queue_init(&queue);

    // Create worker threads
    log(__func__, "Creating worker threads");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker_thread, (void *)&queue);
    }

    // Accept connections
    log(__func__, "Accepting connections");
    while (1)
    {
        log(__func__, "Waiting for connections...");
        new_socket = accept_connection(server_fd);
        if (new_socket >= 0)
        {
            // Add task to queue
            Task task = {.socket_fd = new_socket};
            queue_push(&queue, task);
        }
    }

    // Join threads when server shuts down
    log(__func__, "Shutting down server");
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    // Free memory
    log(__func__, "Freeing memory");
    free_queue(&queue);
    return 0;
}