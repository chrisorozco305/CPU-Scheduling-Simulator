#include "simulate.h"
#include "queue.h"
#include "pqueue.h"

void simulate_priority_rr() {
    int t = 0;  // current time
    int completed = 0;
    int time_slice = 0;
    queue ready = init_queue();

    // Main simulation loop
    while (completed < n) {
        // Add newly arrived processes to the ready queue
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time == t) {
                processes[i].active = 1;
                enqueue(&ready, processes[i]);
            }
        }

        // If no process is ready, increment time and continue
        if (!ready.size) {
            t++;
            continue;
        }

        // Select the highest priority process in the ready queue
        process *highest_priority_process = NULL;
        node *temp = ready.head;
        while (temp != NULL) {
            if (highest_priority_process == NULL || 
                temp->value.priority < highest_priority_process->priority) {
                highest_priority_process = &temp->value;
            }
            temp = temp->next;
        }

        // Check if highest_priority_process is valid
        if (highest_priority_process == NULL) {
            t++;
            continue;
        }

        int cur = highest_priority_process->id;
        
        // Run the selected process for one time unit
        processes[cur].remaining_time--;
        time_slice++;
        t++;

        // Check if the process has completed
        if (processes[cur].remaining_time == 0) {
            processes[cur].active = 0;
            processes[cur].turnaround_time = t - processes[cur].arrival_time;
            completed++;
            dequeue(&ready);  // Remove the completed process from the queue
            time_slice = 0;   // Reset time slice for the next process
        } else if (time_slice == q) {
            // Time quantum expired, rotate the process
            process moved_process = dequeue(&ready);
            enqueue(&ready, moved_process);  // Move to the end of the queue
            time_slice = 0;                  // Reset time slice for the next process
        }
    }
}
