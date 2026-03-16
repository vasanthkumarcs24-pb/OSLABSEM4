#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
} Process;

void copyProcesses(Process dest[], Process src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}


void display(Process p[], int n, char *algo, int quantum) {
    printf("\n%s (Quantum=%d) Results:\n", algo, quantum);
    printf("PID\tArrival\tBurst\tComplete\tWaiting\tTurnaround\n");
    float avg_wait = 0, avg_ta = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst,
               p[i].completion, p[i].waiting, p[i].turnaround);
        avg_wait += p[i].waiting;
        avg_ta += p[i].turnaround;
    }
    printf("Avg Waiting: %.2f\tAvg Turnaround: %.2f\n", avg_wait/n, avg_ta/n);
}

void rr(Process p[], int n, int quantum) {
    Process procs[MAX];
    copyProcesses(procs, p, n);
    int time = 0, completed = 0;
    printf("\nGantt: ");
    while (completed < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (procs[i].arrival <= time && procs[i].remaining > 0) {
                found = 1;
                int exec = (procs[i].remaining > quantum) ? quantum : procs[i].remaining;
                printf("P%d(%d) ", procs[i].pid, exec);
                procs[i].remaining -= exec;
                time += exec;
                if (procs[i].remaining == 0) {
                    procs[i].completion = time;
                    procs[i].turnaround = time - procs[i].arrival;
                    procs[i].waiting = procs[i].turnaround - procs[i].burst;
                    completed++;
                }
                break;
            }
        }
        if (!found) {
            printf("IDLE ");
            time++;
        }
    }
    display(procs, n, "Round Robin", quantum);
}

void mlq(Process p[], int n) {
    Process sys[MAX/2], user[MAX/2], temp[MAX];
    int ns = 0, nu = 0;
    copyProcesses(temp, p, n);

    // Separate system (prio 0) and user (prio 1) processes
    printf("\nEnter priority for each process (0=system, 1=user):\n");
    for (int i = 0; i < n; i++) {
        int prio;
        printf("P%d: ", temp[i].pid);
        scanf("%d", &prio);
        if (prio == 0) {
            sys[ns++] = temp[i];
        } else {
            user[nu++] = temp[i];
        }
    }

    // FCFS for system queue (higher priority)
    int time = 0;
    printf("\nMLQ Gantt: ");
    for (int i = 0; i < ns; i++) {
        if (sys[i].arrival > time) time = sys[i].arrival;
        printf("P%d(%d) ", sys[i].pid, sys[i].burst);
        time += sys[i].burst;
        sys[i].completion = time;
        sys[i].turnaround = time - sys[i].arrival;
        sys[i].waiting = sys[i].turnaround - sys[i].burst;
    }

    // Then FCFS for user queue
    for (int i = 0; i < nu; i++) {
        if (user[i].arrival > time) time = user[i].arrival;
        printf("P%d(%d) ", user[i].pid, user[i].burst);
        time += user[i].burst;
        user[i].completion = time;
        user[i].turnaround = time - user[i].arrival;
        user[i].waiting = user[i].turnaround - user[i].burst;
    }

    // Display combined results
    printf("\nMulti-Level Queue Results:\n");
    printf("PID\tPrio\tArrival\tBurst\tComplete\tWaiting\tTurnaround\n");
    float avg_wait = 0, avg_ta = 0;
    for (int i = 0; i < ns; i++) {
        printf("%d\t0\t%d\t%d\t%d\t\t%d\t%d\n", sys[i].pid, sys[i].arrival, sys[i].burst,
               sys[i].completion, sys[i].waiting, sys[i].turnaround);
        avg_wait += sys[i].waiting;
        avg_ta += sys[i].turnaround;
    }
    for (int i = 0; i < nu; i++) {
        printf("%d\t1\t%d\t%d\t%d\t\t%d\t%d\n", user[i].pid, user[i].arrival, user[i].burst,
               user[i].completion, user[i].waiting, user[i].turnaround);
        avg_wait += user[i].waiting;
        avg_ta += user[i].turnaround;
    }
    printf("Avg Waiting: %.2f\tAvg Turnaround: %.2f\n", avg_wait/n, avg_ta/n);
}

int main() {
    int n, i;
    Process p[MAX];
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        p[i].remaining = 0;
        printf("P%d arrival time: ", p[i].pid);
        scanf("%d", &p[i].arrival);
        printf("P%d burst time: ", p[i].pid);
        scanf("%d", &p[i].burst);
        p[i].remaining = p[i].burst;
    }

    // Experiment with RR quantum sizes
    rr(p, n, 2);
    rr(p, n, 4);
    rr(p, n, 8);
      // Medium quantum


    // MLQ
    mlq(p, n);

    return 0;
}
