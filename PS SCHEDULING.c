#include <stdio.h>

#define MAX 10
#define TIME_QUANTUM 10

typedef struct process {
    int id;
    int burst;
    int remaining;
    int weight;
    int completed;
} process;

int main() {
    process p[MAX];
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    int total_weight = 0;
    int total_burst = 0;


    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        p[i].id = i + 1;

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);

        printf("Weight: ");
        scanf("%d", &p[i].weight);

        p[i].remaining = p[i].burst;
        p[i].completed = 0;

        total_weight += p[i].weight;
        total_burst += p[i].burst;
    }


    if (total_weight == 0) {
        printf("\nScheduling not feasible (total weight is zero)\n");
        return 0;
    } else {
        printf("\nScheduling feasible\n");
    }

    int done = 0;

    printf("\nExecution:\n");

    while (done < n) {

        for (int i = 0; i < n; i++) {

            if (p[i].completed == 0) {

                float slice = ((float)p[i].weight / total_weight) * TIME_QUANTUM;
                int time_slice = (int)slice;

                if (time_slice <= 0)
                    time_slice = 1;

                if (time_slice > p[i].remaining)
                    time_slice = p[i].remaining;

                printf("P%d runs for %d units\n", p[i].id, time_slice);

                p[i].remaining -= time_slice;

                if (p[i].remaining <= 0) {
                    p[i].completed = 1;
                    done++;
                    total_weight -= p[i].weight;
                }
            }
        }
    }

    return 0;
}
