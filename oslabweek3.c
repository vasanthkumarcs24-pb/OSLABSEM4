#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 10

// Structure to hold process information
typedef struct {
    int pid;          // Process ID
    int arrival;      // Arrival time
    int burst;        // Burst time
    int priority;     // Priority (smaller = higher priority)
    int waiting;      // Waiting time
    int turnaround;   // Turnaround time
    int completion;   // Completion time
} Process;

// Function prototypes
void inputProcesses(Process p[], int n);
void sjfNonPreemptive(Process p[], int n);
void sjfPreemptive(Process p[], int n);
void priorityNonPreemptive(Process p[], int n);
void priorityPreemptive(Process p[], int n);
void displayResults(Process p[], int n);
void copyProcesses(Process dest[], Process src[], int n);

// 1. SJF Non-Preemptive
void sjfNonPreemptive(Process p[], int n) {
    Process temp[MAX_PROCESSES];
    copyProcesses(temp, p, n);

    // Sort by arrival time first
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(temp[j].arrival > temp[j+1].arrival) {
                Process swap = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = swap;
            }
        }
    }

    int current_time = 0;
    int complete = 0;

    printf("\n=== SJF NON-PREEMPTIVE ===\n");
    printf("Gantt Chart: ");

    while(complete != n) {
        // Find process with shortest burst time among ready processes
        int shortest = -1;
        int min_burst = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(temp[i].arrival <= current_time && temp[i].burst < min_burst &&
               temp[i].completion == 0) {
                min_burst = temp[i].burst;
                shortest = i;
            }
        }

        if(shortest == -1) {
            printf("IDLE ");
            current_time++;
            continue;
        }

        // Execute the process
        printf("P%d ", temp[shortest].pid);
        current_time += temp[shortest].burst;
        temp[shortest].completion = current_time;
        temp[shortest].turnaround = temp[shortest].completion - temp[shortest].arrival;
        temp[shortest].waiting = temp[shortest].turnaround - temp[shortest].burst;
        complete++;
    }
    printf("\n");
    displayResults(temp, n);
}

// 2. SJF Preemptive (Shortest Remaining Time First)
void sjfPreemptive(Process p[], int n) {
    Process temp[MAX_PROCESSES];
    copyProcesses(temp, p, n);

    int current_time = 0;
    int complete = 0;
    int prev_pid = -1;

    printf("\n=== SJF PREEMPTIVE (SRTF) ===\n");
    printf("Gantt Chart: ");

    while(complete != n) {
        int shortest = -1;
        int min_burst = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(temp[i].arrival <= current_time && temp[i].burst > 0 &&
               temp[i].burst < min_burst) {
                min_burst = temp[i].burst;
                shortest = i;
            }
        }

        if(shortest == -1) {
            printf("IDLE ");
            current_time++;
            continue;
        }

        // Execute for 1 time unit
        printf("P%d ", temp[shortest].pid);
        temp[shortest].burst--;
        current_time++;

        if(temp[shortest].burst == 0) {
            temp[shortest].completion = current_time;
            temp[shortest].turnaround = temp[shortest].completion - temp[shortest].arrival;
            temp[shortest].waiting = temp[shortest].turnaround - p[shortest].burst;
            complete++;
        }

        prev_pid = temp[shortest].pid;
    }
    printf("\n");
    displayResults(temp, n);
}

// 3. Priority Non-Preemptive
void priorityNonPreemptive(Process p[], int n) {
    Process temp[MAX_PROCESSES];
    copyProcesses(temp, p, n);

    // Sort by arrival time
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(temp[j].arrival > temp[j+1].arrival) {
                Process swap = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = swap;
            }
        }
    }

    int current_time = 0;
    int complete = 0;

    printf("\n=== PRIORITY NON-PREEMPTIVE ===\n");
    printf("Gantt Chart: ");

    while(complete != n) {
        int highest = -1;
        int min_priority = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(temp[i].arrival <= current_time && temp[i].priority < min_priority &&
               temp[i].completion == 0) {
                min_priority = temp[i].priority;
                highest = i;
            }
        }

        if(highest == -1) {
            printf("IDLE ");
            current_time++;
            continue;
        }

        printf("P%d ", temp[highest].pid);
        current_time += temp[highest].burst;
        temp[highest].completion = current_time;
        temp[highest].turnaround = temp[highest].completion - temp[highest].arrival;
        temp[highest].waiting = temp[highest].turnaround - temp[highest].burst;
        complete++;
    }
    printf("\n");
    displayResults(temp, n);
}

// 4. Priority Preemptive
void priorityPreemptive(Process p[], int n) {
    Process temp[MAX_PROCESSES];
    copyProcesses(temp, p, n);

    int current_time = 0;
    int complete = 0;

    printf("\n=== PRIORITY PREEMPTIVE ===\n");
    printf("Gantt Chart: ");

    while(complete != n) {
        int highest = -1;
        int min_priority = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(temp[i].arrival <= current_time && temp[i].burst > 0 &&
               temp[i].priority < min_priority) {
                min_priority = temp[i].priority;
                highest = i;
            }
        }

        if(highest == -1) {
            printf("IDLE ");
            current_time++;
            continue;
        }

        printf("P%d ", temp[highest].pid);
        temp[highest].burst--;
        current_time++;

        if(temp[highest].burst == 0) {
            temp[highest].completion = current_time;
            temp[highest].turnaround = temp[highest].completion - temp[highest].arrival;
            temp[highest].waiting = temp[highest].turnaround - p[highest].burst;
            complete++;
        }
    }
    printf("\n");
    displayResults(temp, n);
}

// Helper function to input processes
void inputProcesses(Process p[], int n) {
    printf("Enter details for %d processes:\n", n);
    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process P%d:\n", p[i].pid);
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        printf("Priority (smaller = higher): ");
        scanf("%d", &p[i].priority);
        p[i].completion = 0;
        p[i].waiting = 0;
        p[i].turnaround = 0;
    }
}

// Helper function to copy processes
void copyProcesses(Process dest[], Process src[], int n) {
    for(int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Display results
void displayResults(Process p[], int n) {
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    float total_wait = 0, total_turn = 0;

    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst + (p[i].burst == 0 ? 0 : 0),
               p[i].priority, p[i].waiting, p[i].turnaround);
        total_wait += p[i].waiting;
        total_turn += p[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wait/n);
    printf("Average Turnaround Time: %.2f\n", total_turn/n);
    printf("------------------------------------------------\n");
}

int main() {
    int n;

    printf("CPU Scheduling Simulator\n");
    printf("=======================\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    if(n > MAX_PROCESSES || n <= 0) {
        printf("Invalid number of processes!\n");
        return 1;
    }

    Process processes[MAX_PROCESSES];
    inputProcesses(processes, n);

    // Run all algorithms
    sjfNonPreemptive(processes, n);
    sjfPreemptive(processes, n);
    priorityNonPreemptive(processes, n);
    priorityPreemptive(processes, n);

    printf("\nProgram completed successfully!\n");
    return 0;
}
