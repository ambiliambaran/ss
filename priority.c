#include<stdio.h>
int main()
{
  int n,i,j,temp;
  int bt[20],wt[20],tat[20],p[20],pri[20];
  int total_wt=0,total_tat=0;
  
  printf("Enter the number of processes : ");
  scanf("%d",&n);
   for(i=0;i<n;i++){
  printf("Enter Burst time and Priority for P%d : ",i+1);
 
    scanf("%d %d",&bt[i],&pri[i]);
    p[i]=i+1;
  }
  for(i=0;i<n-1;i++){
     for(j=i+1;j<n;j++){
       if(pri[i]>pri[j]){ 
          temp=pri[i];
          pri[i]=pri[j];
          pri[j]=temp;
          
          temp=bt[i];
          bt[i]=bt[j];
          bt[j]=temp;
          
          temp=p[i];
          p[i]=p[j];
          p[j]=temp;
       }
    }
  }
  wt[0]=0;
  for(i=1;i<n;i++){
    wt[i]=wt[i-1]+bt[i-1];
  }
  for(i=0;i<n;i++){
    tat[i]=wt[i]+bt[i];
  }
  printf("\nProcess\tPriority\tBurst_time\tWaiting_time\tTurnAround_time\n");
  for(i=0;i<n;i++){
    printf("\nP%d\t\t%3d\t\t%3d\t\t%3d\t\t%3d\n",p[i],pri[i],bt[i],wt[i],tat[i]);
       total_wt+=wt[i];
       total_tat+=tat[i];
  }
  printf("\nAverage waiting time : %.2f ",(float)total_wt/n);
  printf("\nAverage TurnAround time : %.2f  ",(float)total_tat/n);
  return 0;
}
    
       
        
