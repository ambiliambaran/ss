#include <stdio.h>

struct Process 
{
    int pid;
    int at;     
    int bt;     
    int pr;     
    int wt;     
    int tat;    
    int ct;     
    int done;   
};

int main() {
    int n, i, time = 0, completed = 0;
    struct Process p[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time and Priority for Process %d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
        p[i].done = 0;
    }

    while(completed < n) {
        int idx = -1;
        int min_pr = 9999;
        for(i = 0; i < n; i++)
        {
            if(p[i].at <= time && p[i].done == 0) 
            {
                if(p[i].pr < min_pr)
                {
                    min_pr = p[i].pr;
                    idx = i;
                } else if(p[i].pr == min_pr) 
                {
  
                    if(p[i].at < p[idx].at) 
                    {
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1) 
        {
            p[idx].wt = time - p[idx].at;
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].done = 1;
            completed++;
        } else
        {
            time++;
        }
    }
    printf("\nProcess\tAT\tBT\tPR\tCT\tWT\tTAT\n");
    for(i = 0; i < n; i++) 
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].wt, p[i].tat);
    }
    float avg_wt = 0, avg_tat = 0;
    for(i = 0; i < n; i++) 
    {
        avg_wt += p[i].wt;
        avg_tat += p[i].tat;
    }
    avg_wt /= n;
    avg_tat /= n;

    printf("\nAverage Waiting Time: %.2f", avg_wt);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat);

    return 0;
}

