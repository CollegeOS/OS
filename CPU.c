#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int pid, at, ct, bt, rt, wt, tat, priority;
    bool done;
} Process;

int compare_arrival_time(const void *a, const void *b) {
    return ((Process *)a)->at - ((Process *)b)->at;
}

void input(Process p[], int *n, int with_priority) {
    printf("Enter Number Of Processes: ");
    scanf("%d", n);
    for (int i = 0; i < *n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        if (with_priority) {
            printf("Enter Priority for P%d: ", i + 1);
            scanf("%d", &p[i].priority);
        }
        p[i].rt = p[i].bt;
        p[i].done = false;
    }
    qsort(p, *n, sizeof(Process), compare_arrival_time);
}

void display(Process p[], int n) {
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
}

void fcfs(Process p[], int n) {
    Process temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = p[i];
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < temp[i].at)
            time = temp[i].at;
        time += temp[i].bt;
        temp[i].ct = time;
    }
    display(temp, n);
}

void sjf(Process p[], int n) {
    Process temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = p[i];
    int complete = 0, time = 0;
    while (complete < n) {
        int min_bt = INT_MAX, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!temp[i].done && temp[i].rt < min_bt && temp[i].rt > 0 && temp[i].at <= time) {
                min_bt = temp[i].rt;
                idx = i;
            }
        }
        if (idx != -1) {
            temp[idx].rt--;
            if (temp[idx].rt == 0) {
                temp[idx].ct = time + 1;
                temp[idx].done = true;
                complete++;
            }
        }
        time++;
    }
    display(temp, n);
}

void priority_scheduling(Process p[], int n) {
    Process temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = p[i];
    int time = 0, done = 0;
    while (done < n) {
        int highest = INT_MAX, idx = -1;
        for (int i = 0; i < n; i++) {
            if (!temp[i].done && temp[i].priority < highest && temp[i].at <= time) {
                highest = temp[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            time += temp[idx].bt;
            temp[idx].ct = time;
            temp[idx].done = true;
            done++;
        } else {
            time++;
        }
    }
    display(temp, n);
}

void roundrobin(Process p[], int n) {
    Process temp[MAX];
    int quantum;
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);
    for (int i = 0; i < n; i++) temp[i] = p[i];

    int done = 0, time = 0;
    while (done < n) {
        int idle = 1;
        for (int i = 0; i < n; i++) {
            if (temp[i].rt > 0 && temp[i].at <= time) {
                idle = 0;
                if (temp[i].rt > quantum) {
                    time += quantum;
                    temp[i].rt -= quantum;
                } else {
                    time += temp[i].rt;
                    temp[i].rt = 0;
                    temp[i].ct = time;
                    done++;
                }
            }
        }
        if (idle) time++;
    }
    display(temp, n);
}

int main() {
    Process p[MAX];
    int n, choice, use_priority = 0;
    printf("Select Scheduling Algorithm:\n");
    printf("1. FCFS\n2. SJF (Preemptive)\n3. Priority\n4. Round Robin\n");
    scanf("%d", &choice);
    if (choice == 3) use_priority = 1;
    input(p, &n, use_priority);
    switch (choice) {
        case 1: fcfs(p, n); break;
        case 2: sjf(p, n); break;
        case 3: priority_scheduling(p, n); break;
        case 4: roundrobin(p, n); break;
        default: printf("Invalid Choice\n");
    }
    return 0;
}
