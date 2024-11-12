#include <stdio.h>
#include <string.h>

typedef struct {
    char sign[15];
    char lane[15];
} Activity;

typedef struct {
    Activity activity;
    int seconds;
    int priority;
} Lane;

typedef struct {
    Lane arr[10];
    int count;
} PriorityQueue;

void initPQ(PriorityQueue* pq);
Lane createActivity(char* sign, char* lane, int seconds, int priority);
void maxHeapify(PriorityQueue* pq, int i);
void buildMaxHeap(PriorityQueue* pq);
void maxHeapSort(PriorityQueue* pq);
int getTime(PriorityQueue pq, char* end_sign, char* end_lane);
void saveTrafficResults(PriorityQueue pq, const char* filename, int scenario);

int main() {
    PriorityQueue pq;
    
    // Scenario 1
    initPQ(&pq);
    pq.arr[0] = createActivity("straight", "main", 50, 1);
    pq.arr[1] = createActivity("left", "diversion", 20, 4);
    pq.arr[2] = createActivity("right", "main", 25, 5);
    pq.arr[3] = createActivity("pedestrian", "diversion", 20, 8);
    pq.arr[4] = createActivity("straight", "diversion", 30, 2);
    pq.arr[5] = createActivity("left", "main", 25, 3);
    pq.arr[6] = createActivity("right", "diversion", 20, 6);
    pq.arr[7] = createActivity("pedestrian", "main", 15, 7);
    pq.count = 8;
    buildMaxHeap(&pq);
    maxHeapSort(&pq);
    saveTrafficResults(pq, "traffic_result.dat", 1);

    // Scenario 2
    initPQ(&pq);
    pq.arr[0] = createActivity("left", "main", 45, 1);
    pq.arr[1] = createActivity("straight", "diversion", 25, 3);
    pq.arr[2] = createActivity("right", "main", 20, 5);
    pq.arr[3] = createActivity("pedestrian", "main", 15, 2);
    pq.arr[4] = createActivity("left", "diversion", 30, 4);
    pq.count = 5;
    buildMaxHeap(&pq);
    maxHeapSort(&pq);
    saveTrafficResults(pq, "traffic_result.dat", 2);

    // Scenario 3
    initPQ(&pq);
    pq.arr[0] = createActivity("right", "main", 30, 2);
    pq.arr[1] = createActivity("straight", "diversion", 40, 4);
    pq.arr[2] = createActivity("pedestrian", "main", 10, 1);
    pq.arr[3] = createActivity("left", "main", 20, 3);
    pq.count = 4;
    buildMaxHeap(&pq);
    maxHeapSort(&pq);
    saveTrafficResults(pq, "traffic_result.dat", 3);

    return 0;
}

void initPQ(PriorityQueue* pq) {
    pq->count = 0;
}

Lane createActivity(char* sign, char* lane, int seconds, int priority) {
    Lane l;
    Activity a;
    strcpy(a.sign, sign);
    strcpy(a.lane, lane);
    l.activity = a;
    l.seconds = seconds;
    l.priority = priority;
    return l;
}

void maxHeapify(PriorityQueue* pq, int i) {
    int left = (i * 2) + 1;
    int right = (i * 2) + 2;
    int largest = i;

    if (left < pq->count && pq->arr[left].priority > pq->arr[largest].priority) {
        largest = left;
    }
    if (right < pq->count && pq->arr[right].priority > pq->arr[largest].priority) {
        largest = right;
    }
    if (largest != i) {
        Lane temp = pq->arr[i];
        pq->arr[i] = pq->arr[largest];
        pq->arr[largest] = temp;
        maxHeapify(pq, largest);
    }
}

void buildMaxHeap(PriorityQueue* pq) {
    for (int i = (pq->count / 2) - 1; i >= 0; i--) {
        maxHeapify(pq, i);
    }
}

void maxHeapSort(PriorityQueue* pq) {
    int original_count = pq->count;
    for (int i = pq->count - 1; i > 0; i--) {
        Lane temp = pq->arr[0];
        pq->arr[0] = pq->arr[i];
        pq->arr[i] = temp;
        pq->count--;
        maxHeapify(pq, 0);
    }
    pq->count = original_count;
}

int getTime(PriorityQueue pq, char* end_sign, char* end_lane) {
    int total_time = 0;
    for (int i = 0; i < pq.count; i++) {
        if (strcmp(end_sign, pq.arr[i].activity.sign) == 0 && strcmp(end_lane, pq.arr[i].activity.lane) == 0) {
            break;
        }
        total_time += pq.arr[i].seconds;
    }
    return total_time;
}

void saveTrafficResults(PriorityQueue pq, const char* filename, int scenario) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening result file");
        return;
    }

    fprintf(file, "\nScenario %d\n", scenario);
    fprintf(file, "Priority,Sign,Lane,Seconds\n");
    for (int i = 0; i < pq.count; i++) {
        fprintf(file, "Priority %d: %-10s %-10s %d seconds\n",
                pq.arr[i].priority, pq.arr[i].activity.sign, pq.arr[i].activity.lane, pq.arr[i].seconds);
    }

    int total_time = getTime(pq, "pedestrian", "main");
    fprintf(file, "\nTotal time before 'pedestrian main' can cross: %d seconds\n", total_time);

    fclose(file);
}
