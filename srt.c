#include "simulate.h"
#include "pqueue.h"
#include <stddef.h>

void simulate_srt() {
    int t = 0;          // Current time
    int completed = 0;  // Count of completed processes
    pqueue ready = init_pqueue(n);  // Initialize priority queue with capacity n

    while (completed < n) {
        // Add newly arrived processes to the priority queue
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == t) {
                processes[i].active = 1;
                enqueue_srt(&ready, processes[i]);
            }
        }

        // If no process is ready, increment time and continue
        if (ready.size == 0) {
            t++;
            continue;
        }

        // Peek the process with the shortest remaining time
        process shortest = pq_peek(ready);
        int cur = shortest.id;

        // Run the selected process for one unit of time
        processes[cur].remaining_time--;
        t++;

        // If the process has completed, remove it from the queue
        if (processes[cur].remaining_time == 0) {
            processes[cur].active = 0;
            processes[cur].turnaround_time = t - processes[cur].arrival_time;
            completed++;
            dequeue_srt(&ready);  // Remove the completed process
        } else {
            // Re-insert the process with updated remaining time into the queue
            dequeue_srt(&ready);
            enqueue_srt(&ready, processes[cur]);
        }
    }

    pq_free(&ready);  // Free the priority queue memory
}
