#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_PAGES 50

bool isInMemory(int frames[], int page, int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

void fifo(int pages[], int capacity, int n) {
    int frames[MAX_FRAMES], front = 0, page_faults = 0;
    for (int i = 0; i < capacity; i++) frames[i] = -1;

    printf("\nFIFO Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        if (!isInMemory(frames, pages[i], capacity)) {
            frames[front] = pages[i];
            front = (front + 1) % capacity;
            page_faults++;
        }

        printf("Page: %d -> [ ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf("]\n");
    }

    printf("Total Page Faults (FIFO): %d\n", page_faults);
}

void lru(int pages[], int capacity, int n) {
    int frames[MAX_FRAMES], time[MAX_FRAMES], page_faults = 0;
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    printf("\nLRU Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == pages[i]) {
                time[j] = i;
                found = true;
                break;
            }
        }

        if (!found) {
            int lru = 0;
            for (int j = 1; j < capacity; j++) {
                if (time[j] < time[lru]) lru = j;
            }
            frames[lru] = pages[i];
            time[lru] = i;
            page_faults++;
        }

        printf("Page: %d -> [ ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf("]\n");
    }
    printf("Total Page Faults (LRU): %d\n", page_faults);
}

void optimal(int pages[], int n, int capacity) {
    int frames[MAX_FRAMES], page_faults = 0;
    for (int i = 0; i < capacity; i++) frames[i] = -1;

    printf("\nOptimal Page Replacement:\n");
    for (int i = 0; i < n; i++) {
        if (!isInMemory(frames, pages[i], capacity)) {
            int replace = -1, farthest = -1;

            for (int j = 0; j < capacity; j++) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (frames[j] == pages[k]) break;
                }

                if (k == n) {
                    replace = j;
                    break;
                }

                if (k > farthest) {
                    farthest = k;
                    replace = j;
                }
            }

            if (replace == -1) replace = 0;
            frames[replace] = pages[i];
            page_faults++;
        }

        printf("Page: %d -> [ ", pages[i]);
        for (int j = 0; j < capacity; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf("]\n");
    }
    printf("Total Page Faults (Optimal): %d\n", page_faults);
}

int main() {
    int choice, n, capacity, pages[MAX_PAGES];

    printf("Enter number of pages: ");
    scanf("%d", &n);
    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d", &capacity);

    do {
        printf("\n--- Page Replacement Menu ---\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fifo(pages, capacity, n);
                break;
            case 2:
                lru(pages, capacity, n);
                break;
            case 3:
                optimal(pages, n, capacity);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}
