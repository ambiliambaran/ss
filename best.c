#include <stdio.h>

int main() {
    int n, m;
    printf("Enter number of memory blocks: ");
    scanf("%d", &n);
    int block[n];
    printf("Enter block sizes: ");
    for(int i=0;i<n;i++) scanf("%d", &block[i]);

    printf("Enter number of processes: ");
    scanf("%d", &m);
    int process[m];
    printf("Enter process sizes: ");
    for(int i=0;i<m;i++) scanf("%d", &process[i]);

    int allocation[m];
    for(int i=0;i<m;i++) allocation[i] = -1;

    for(int i=0;i<m;i++) {
        int best = -1;
        for(int j=0;j<n;j++) {
            if(block[j] >= process[i]) {
                if(best == -1 || block[j] < block[best]) best = j;
            }
        }
        if(best != -1) {
            allocation[i] = best;
            block[best] -= process[i];
        }
    }

    printf("\nProcess\tProcess Size\tBlock Allocated\n");
    for(int i=0;i<m;i++) {
        printf("%d\t%d\t\t", i+1, process[i]);
        if(allocation[i] != -1) printf("%d\n", allocation[i]+1);
        else printf("Not Allocated\n");
    }
    return 0;
}