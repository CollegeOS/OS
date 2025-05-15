#include<stdio.h>
#include<stdbool.h>

#define MAX 10

int n, m;
int alloc[MAX][MAX], need[MAX][MAX], max[MAX][MAX], avail[MAX];

void calculateneed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

bool isSafe() {
    int work[MAX], finish[MAX] = {0}, safeSeq[MAX], count = 0;
    for (int i = 0; i < m; i++) work[i] = avail[i];
    
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canrun = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canrun = false;
                        break;
                    }
                }
                if (canrun) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    
    printf("System Is In Safe State.\nSafe Sequence: ");
    for (int i = 0; i < n; i++) printf("P%d ", safeSeq[i]);
    printf("\n");
    return true;
}

void req_resources() {
    int p, req[MAX];
    printf("Enter Process Number (0-%d): ", n-1);
    scanf("%d", &p);
    
    printf("Enter Requested Resources: ");
    for (int i = 0; i < m; i++) scanf("%d", &req[i]);
    
    for (int i = 0; i < m; i++) {
        if (req[i] > need[p][i]) {
            printf("Error: Resource exceeds need\n");
            return;
        }
        if (req[i] > avail[i]) {
            printf("Error: Resources Not Available\n");
            return;
        }
    }
    
    // Allocate resources temporarily
    for (int i = 0; i < m; i++) {
        avail[i] -= req[i];
        need[p][i] -= req[i];
        alloc[p][i] += req[i];
    }
    
    if (isSafe()) {
        printf("Request Granted\n");
    } else {
        // Rollback allocation if the request was unsafe
        for (int i = 0; i < m; i++) {
            avail[i] += req[i];
            need[p][i] += req[i];
            alloc[p][i] -= req[i];
        }
        printf("Request Denied\n");
    }
}

int main() {
    printf("Enter Number Of Processes: ");
    scanf("%d", &n);
    
    printf("Enter Number Of Resources: ");
    scanf("%d", &m);

    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }
    
    printf("Enter Maximum Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("Enter Available Resources:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }
    
    calculateneed();
    
    if (!isSafe()) {
        printf("System is not in safe state\n");
        return 0;
    }
    
    char choice;
    do {
        req_resources();
        printf("Want To Make Another Request [y/n]: ");
        scanf(" %c", &choice);  // Added a space before %c to consume any leftover newline character
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}
