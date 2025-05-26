#include "simulate.h"
#include "pqueue.h"
#include <stddef.h>

void simulate_sjf() {
    int t = 0;          // Current time
    int completed = 0;  // Count of completed processes
    pqueue ready = init_pqueue(n);  // Initialize priority queue with capacity n

    while (completed < n) {
        // Add newly arrived processes to the priority queue based on arrival time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == t) {
                processes[i].active = 1;
                enqueue_sjf(&ready, processes[i]);  // Enqueue based on total_cpu_time for SJF
            }
        }

        // If no process is ready, increment time and continue
        if (ready.size == 0) {
            t++;
            continue;
        }

        // Dequeue the process with the shortest total CPU time and run it to completion
        process shortest = dequeue_sjf(&ready);
        int cur = shortest.id;

        // Run the selected job to completion
        t += processes[cur].remaining_time;           // Advance time by the process's remaining time
        processes[cur].remaining_time = 0;            // Set remaining time to 0
        processes[cur].active = 0;                    // Mark as inactive
        processes[cur].turnaround_time = t - processes[cur].arrival_time;  // Calculate turnaround time

        completed++;                                  // Increment completed processes count
    }

    pq_free(&ready);  // Free the priority queue memory
}
