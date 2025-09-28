#include <stdio.h>

int main() 
{
    int n, i, timeQuantum;
    int bt[20], rem_bt[20], wt[20], tat[20];
    int t = 0; 
    int done;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter burst time for each process:\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rem_bt[i] = bt[i]; 
    }
    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);
    for (i = 0; i < n; i++) 
    {
        wt[i] = 0;
    }
    while (1)
    {
        done = 1; 
        for (i = 0; i < n; i++)
        {
            if (rem_bt[i] > 0) 
            {
                done = 0; 
                if (rem_bt[i] > timeQuantum) 
                {
                    t += timeQuantum;
                    rem_bt[i] -= timeQuantum;
                } else 
                {
                    t += rem_bt[i];
                    wt[i] = t - bt[i]; 
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1) break; 
    }
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    int total_wt = 0, total_tat = 0;
    for (i = 0; i < n; i++) 
    {
        tat[i] = bt[i] + wt[i];
        total_wt += wt[i];
        total_tat += tat[i];
        printf("P%d\t%d\t\t%d\t\t%d\n", i + 1, bt[i], wt[i], tat[i]);
    }
    printf("\nAverage Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
    return 0;
}

