#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TIME 20
#define MAX_PROC 10

typedef struct {
    int id;
    int exec_time;
    int period;
    int remaining;
    int next_release;
} Process;

int compare_period(const void *a, const void *b) {
    return ((Process*)a)->period - ((Process*)b)->period;
}

void rms_schedule(int n, Process procs[]) {
    int time = 0;
    char gantt[MAX_TIME];
    memset(gantt, '-', sizeof(gantt));

    qsort(procs, n, sizeof(Process), compare_period);

    printf("\nProcesses (Priority: shortest period first):\n");
    for(int i = 0; i < n; i++) {
        printf("P%d: exec=%d, period=%d\n", procs[i].id, procs[i].exec_time, procs[i].period);
        procs[i].remaining = 0;
        procs[i].next_release = 0;
    }


    printf("\nR represents P0\n");
    printf("\nP represents P1\n");
    printf("\nQ represents P2\n");
    printf("\nGantt Chart:\nProc: ");

    while(time < MAX_TIME) {
        for(int i = 0; i < n; i++) {
            if(time == procs[i].next_release) {
                procs[i].remaining = procs[i].exec_time;
                procs[i].next_release += procs[i].period;
            }
        }

        int selected = -1;
        for(int i = 0; i < n; i++) {
            if(procs[i].remaining > 0) {
                selected = i;
                break;
            }
        }

        if(selected != -1) {
            gantt[time] = 'P' + selected;
            procs[selected].remaining--;
        }

        time++;
    }

    for(int t = 0; t < MAX_TIME && t < 50; t++) {
        printf("%c ", gantt[t]);
    }
    printf("\n");

    float utilization = 0;
    for(int i = 0; i < n; i++) {
        utilization += (float)procs[i].exec_time / procs[i].period;
    }
    float bound = n * (pow(2, 1.0/n) - 1);
    printf("\nUtilization = %.3f, Bound = %.3f\n", utilization, bound);
    if(utilization <= bound) {
        printf("SCHEDULABLE\n");
    } else {
        printf("MAY MISS DEADLINES\n");
    }
}

int main() {
    int n;
    Process procs[MAX_PROC];

    printf("Enter number of processes (max %d): ", MAX_PROC);
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        procs[i].id = i;
        printf("P%d execution time: ", i);
        scanf("%d", &procs[i].exec_time);
        printf("P%d period: ", i);
        scanf("%d", &procs[i].period);
    }

    rms_schedule(n, procs);
    return 0;
}
