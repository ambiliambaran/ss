#include <stdio.h>
int main() 
{
    int n, m, i, j, k;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);
    int alloc[n][m], max[n][m], need[n][m], avail[m];
    int finish[n], safeSeq[n];
    printf("\nEnter Allocation Matrix :\n");
    for(i = 0; i < n; i++) 
    {
        printf("Process %d: ", i);
        for(j = 0; j < m; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
    }
    printf("\nEnter Maximun Matrix :\n");
    for(i = 0; i < n; i++) 
    {
        printf("Process %d: ", i);
        for(j = 0; j < m; j++)
        {
            scanf("%d", &max[i][j]);
        }
    }
    printf("\nEnter Available Resources:\n");
    for(i = 0; i < m; i++) 
    {
        scanf("%d", &avail[i]);
    }
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    for(i = 0; i < n; i++) finish[i] = 0;

    int count = 0;
    while(count < n) 
    {
        int found = 0;
        for(i = 0; i < n; i++)
        {
            if(finish[i] == 0) 
            {
                int canAllocate = 1;
                for(j = 0; j < m; j++) 
                {
                    if(need[i][j] > avail[j]) 
                    {
                        canAllocate = 0;
                        break;
                    }
                }
                if(canAllocate) 
                {
                    for(k = 0; k < m; k++) 
                    {
                        avail[k] += alloc[i][k];
                    }
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if(found == 0) 
        {
            printf("\nSystem is NOT in a safe state. Deadlock may occur.\n");
            return 0;
        }
    }
    printf("\nSystem is in a safe state.\nSafe sequence is: ");
    for(i = 0; i < n; i++)
    {
        printf("P%d ", safeSeq[i]);
        if(i != n-1) printf("-> ");
    }

    printf("\n\nAllocation Matrix:\n");
    printf("P#\t");
    for(j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(i = 0; i < n; i++) 
    {
        printf("P%d\t", i);
        for(j = 0; j < m; j++) 
        {
            printf("%d\t", alloc[i][j]);
        }
        printf("\n");
    }

    printf("\nMaximum Matrix:\n");
    printf("P#\t");
    for(j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(i = 0; i < n; i++) 
    {
        printf("P%d\t", i);
        for(j = 0; j < m; j++) 
        {
            printf("%d\t", max[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed Matrix:\n");
    printf("P#\t");
    for(j = 0; j < m; j++) printf("R%d\t", j);
    printf("\n");
    for(i = 0; i < n; i++) 
    {
        printf("P%d\t", i);
        for(j = 0; j < m; j++) 
        {
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }

    printf("\nAvailable Resources:\n");
    for(j = 0; j < m; j++) 
    {
        printf("R%d ", j);
    }
    printf("\n");
    for(j = 0; j < m; j++) 
    {
        printf("%d\t", avail[j]);
    }
    printf("\n");

    return 0;
}

