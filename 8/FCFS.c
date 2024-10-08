#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int pid;
    int bt;
    int wt;
    int tt;
};

struct Process *p;

int main()
{
    int n;
    printf("Enter number of process: ");
    scanf("%d", &n);
    p = (struct Process *)malloc((size_t)n * sizeof(struct Process));
    printf("Enter Burst Time:\n");
    for (int i = 1; i <= n; i++)
    {
        p[i - 1].pid = (int)i;
        printf("P%d: ", i);
        scanf("%d", &p[i - 1].bt);
        p[i - 1].wt = 0;
        p[i - 1].tt = 0;
    }

    p[0].wt = 0;

    // calculating waiting time
    for (long int i = 1; i < n; i++)
        p[i].wt = p[i - 1].wt + p[i - 1].bt;

    for (long int i = 0; i < n; i++)
        p[i].tt = p[i].bt + p[i].wt;

    printf("Processes \t Burst Time \t Waiting Time \t Turn-Around Time\n");
    int total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++)
    {
        total_wt = total_wt + p[i].wt;
        total_tat = total_tat + p[i].tt;
        printf("%d\t\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].bt, p[i].wt, p[i].tt);
    }

    printf("Average waiting time = %.2lf", (double)total_wt / (double)n);
    printf("\nAverage turn around time = %.2lf\n", (double)total_tat / (double)n);

    return 0;
}
