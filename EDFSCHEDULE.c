#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int exec_time;
    int deadline;
    int remaining;
    int completed;
} Process;


double compute_utilization(Process p[], int n) {
    double U = 0.0;
    for (int i = 0; i < n; i++) {
        U += (double)p[i].exec_time / p[i].deadline;
    }
    return U;
}


int earliest_deadline(Process p[], int n, int t) {
    int min_deadline = 1e9;
    int chosen = -1;
    for (int i = 0; i < n; i++) {
        if (!p[i].completed && p[i].arrival <= t && p[i].remaining > 0) {
            if (p[i].deadline < min_deadline) {
                min_deadline = p[i].deadline;
                chosen = i;
            }
        }
    }
    return chosen;
}

void EDF_Scheduler(Process p[], int n) {
    double U = compute_utilization(p, n);
    if (U > 1.0) {
        printf("Scheduling not feasible (deadlines may be missed)\n");
        return;
    } else {
        printf("Scheduling feasible\n");
    }

    int t = 0;
    int finished = 0;
    int timeline[100];

    while (finished < n) {
        int idx = earliest_deadline(p, n, t);

        if (idx == -1) {
            printf("Time %d: Idle\n", t);
            timeline[t] = 0;
            t++;
            continue;
        }

        printf("Time %d: Executing Process %d\n", t, p[idx].id);
        timeline[t] = p[idx].id;
        p[idx].remaining--;
        t++;

        if (p[idx].remaining == 0) {
            p[idx].completed = 1;
            finished++;
            printf("Process %d completed at time %d\n", p[idx].id, t);
        }
    }


    printf("\n--- Gantt Chart ---\n");
    printf("Time:   ");
    for (int i = 0; i < t; i++) {
        printf("%2d ", i);
    }
    printf("\nProcess:");
    for (int i = 0; i < t; i++) {
        if (timeline[i] == 0)
            printf(" - ");
        else
            printf(" P%d", timeline[i]);
    }

    for (int i = 0; i < n; i++) {
     p[i].id = i+1;
    p[i].remaining = p[i].exec_time;
    p[i].completed = 0;


    double Ui = (double)p[i].exec_time / p[i].deadline;
    printf("/n 8Utilization of Process %d = %.2f\n", p[i].id, Ui);
}

    printf("\n");
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time, execution time, deadline for Process %d: ", i+1);
        scanf("%d %d %d", &p[i].arrival, &p[i].exec_time, &p[i].deadline);
        p[i].id = i+1;
        p[i].remaining = p[i].exec_time;
        p[i].completed = 0;
    }

    EDF_Scheduler(p, n);

    return 0;
}
