#include <stdio.h>

#define MAX 10

int main() {
    int n, m;
    int i, j, k;

    int alloc[MAX][MAX];
    int max[MAX][MAX];
    int need[MAX][MAX];
    int avail[MAX];

    int request[MAX];

    int finish[MAX], safeSeq[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resource types: ");
    scanf("%d", &m);

    // Allocation Matrix
    printf("\nEnter Allocation Matrix:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Max Matrix
    printf("\nEnter Max Matrix:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Available Resources
    printf("\nEnter Available Resources:\n");
    for(i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }

    // Calculate Need Matrix
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // ---------------- SAFETY ALGORITHM ----------------

    for(i = 0; i < n; i++)
        finish[i] = 0;

    int count = 0;

    while(count < n) {

        int found = 0;

        for(i = 0; i < n; i++) {

            if(finish[i] == 0) {

                for(j = 0; j < m; j++) {

                    if(need[i][j] > avail[j])
                        break;
                }

                // Process can execute
                if(j == m) {

                    for(k = 0; k < m; k++)
                        avail[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if(found == 0)
            break;
    }

    // Safe State Check
    if(count == n) {

        printf("\nSYSTEM IS IN SAFE STATE\n");

        printf("Safe Sequence: ");

        for(i = 0; i < n; i++) {

            printf("P%d", safeSeq[i]);

            if(i != n - 1)
                printf(" -> ");
        }

        printf("\n");
    }
    else {

        printf("\nSYSTEM IS NOT IN SAFE STATE\n");
        return 0;
    }

    // ---------------- RESOURCE REQUEST ALGORITHM ----------------

    int p;

    printf("\nEnter process number making request: ");
    scanf("%d", &p);

    printf("Enter request vector:\n");

    for(i = 0; i < m; i++)
        scanf("%d", &request[i]);

    // Check Request <= Need
    for(i = 0; i < m; i++) {

        if(request[i] > need[p][i]) {

            printf("\nERROR: Process exceeded maximum claim.\n");
            return 0;
        }
    }

    // Check Request <= Available
    for(i = 0; i < m; i++) {

        if(request[i] > avail[i]) {

            printf("\nResources not available. Process must wait.\n");
            return 0;
        }
    }

    // Pretend Allocation
    for(i = 0; i < m; i++) {

        avail[i] -= request[i];
        alloc[p][i] += request[i];
        need[p][i] -= request[i];
    }

    // Run Safety Algorithm Again
    for(i = 0; i < n; i++)
        finish[i] = 0;

    count = 0;

    while(count < n) {

        int found = 0;

        for(i = 0; i < n; i++) {

            if(finish[i] == 0) {

                for(j = 0; j < m; j++) {

                    if(need[i][j] > avail[j])
                        break;
                }

                if(j == m) {

                    for(k = 0; k < m; k++)
                        avail[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if(found == 0)
            break;
    }

    // Final Result
    if(count == n) {

        printf("\nREQUEST CAN BE GRANTED.\n");

        printf("New Safe Sequence: ");

        for(i = 0; i < n; i++) {

            printf("P%d", safeSeq[i]);

            if(i != n - 1)
                printf(" -> ");
        }

        printf("\n");
    }
    else {

        printf("\nREQUEST CANNOT BE GRANTED (Unsafe State)\n");
    }

    return 0;
}
