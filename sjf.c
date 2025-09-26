#include <stdio.h>

int main() 
{
    int n, i, j;
    int bt[20], p[20], wt[20], tat[20];
    int total_wt = 0, total_tat = 0, temp;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the burst time for each process:\n");
    for (i = 0; i < n; i++)
    {
        printf("Process %d: ", i + 1);
        scanf("%d", &bt[i]);
        p[i] = i + 1;
    }
    for (i = 0; i < n - 1; i++) 
    {
        for (j = i + 1; j < n; j++) 
        {
            if (bt[i] > bt[j]) 
            {
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    wt[0] = 0; 
    for (i = 1; i < n; i++)
    {
        wt[i] = wt[i - 1] + bt[i - 1];
    }
    for (i = 0; i < n; i++) 
    {
        tat[i] = wt[i] + bt[i];
    }
    printf("\nProcess\tBurstTime\tWaitingTime\tTurnAroundTime\n");
    for (i = 0; i < n; i++) 
    {
        printf("P%d\t%3d\t\t%3d\t\t%3d\n", p[i], bt[i], wt[i], tat[i]);
        total_wt += wt[i];
        total_tat += tat[i];
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    return 0;
}

