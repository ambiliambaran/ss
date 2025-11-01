#include <stdio.h>

struct Process {
    int pid;   // Process ID
    int at;    // Arrival Time
    int bt;    // Burst Time
    int pr;    // Priority (lower number = higher priority)
    int wt;    // Waiting Time
    int tat;   // Turnaround Time
    int ct;    // Completion Time
    int done;  // Flag to mark if process is completed
};

int main() {
    int n, i, time = 0, completed = 0;
    struct Process p[10];

    // Step 1: Input number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Step 2: Input process details
    for(i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time, and Priority for Process %d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
        p[i].done = 0;  // initially all processes are incomplete
    }

    // Step 3: Scheduling logic (non-preemptive)
    while(completed < n) {
        int idx = -1;        // to store index of process to be executed next
        int min_pr = 9999;   // assume very large number as minimum priority initially

        // Step 4: Find process with highest priority (lowest pr value)
        for(i = 0; i < n; i++) {
            if(p[i].at <= time && p[i].done == 0) {
                if(p[i].pr < min_pr) {
                    min_pr = p[i].pr;
                    idx = i;
                } 
                // If same priority, choose process that arrived earlier
                else if(p[i].pr == min_pr) {
                    if(p[i].at < p[idx].at) {
                        idx = i;
                    }
                }
            }
        }

        // Step 5: Execute the selected process
        if(idx != -1) {
            p[idx].wt = time - p[idx].at;      // Waiting Time = Current time - Arrival Time
            time += p[idx].bt;                 // Update current time after execution
            p[idx].ct = time;                  // Completion Time
            p[idx].tat = p[idx].ct - p[idx].at;// Turnaround Time = CT - AT
            p[idx].done = 1;                   // Mark process as done
            completed++;                       // Increment completed count
        } 
        else {
            // If no process has arrived yet, increase time
            time++;
        }
    }

    // Step 6: Display all process details
    printf("\nProcess\tAT\tBT\tPR\tCT\tWT\tTAT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }

    // Step 7: Calculate and display averages
    float avg_wt = 0, avg_tat = 0;
    for(i = 0; i < n; i++) {
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    avg_wt /= n;
    avg_tat /= n;

    printf("\nAverage Waiting Time: %.2f", avg_wt);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat);

    return 0;
}
