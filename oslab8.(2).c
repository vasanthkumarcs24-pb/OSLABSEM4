#include <stdio.h>
#include <stdbool.h>

#define MAX 10

int main() {
    int n, m;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resource types: ");
    scanf("%d", &m);

    int allocation[MAX][MAX], request[MAX][MAX], available[MAX];
    bool finish[MAX] = {false};
    int work[MAX];

    // Input Allocation matrix
    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);

    // Input Request matrix
    printf("\nEnter Request Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &request[i][j]);

    // Input Available resources
    printf("\nEnter Available Resources:\n");
    for (int j = 0; j < m; j++) {
        scanf("%d", &available[j]);
        work[j] = available[j];
    }

    // Deadlock detection
    int count = 0;
    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (request[i][j] > work[j])
                        break;
                }

                if (j == m) {
                    // Process can finish
                    for (int k = 0; k < m; k++)
                        work[k] += allocation[i][k];

                    finish[i] = true;
                    found = true;
                    count++;

                    printf("Process P%d is completed\n", i);
                }
            }
        }

        if (!found)
            break;
    }

    // Check deadlock
    bool deadlock = false;
    printf("\nDeadlocked processes: ");
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            printf("P%d ", i);
            deadlock = true;
        }
    }

    if (!deadlock)
        printf("None");

    printf("\n");

    return 0;
}
