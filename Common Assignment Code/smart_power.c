#include <stdio.h>

#define MAX_CONSUMERS 50
#define MAX_HOURS 24
#define FILENAME "consumption_data.txt"

typedef struct {
    int hour;
    float consumption;
    float allocated;
} HourlyData;

typedef struct {
    int consumerId;
    int priority;
    HourlyData hourly[MAX_HOURS];
    int recordCount;
} Consumer;

Consumer consumers[MAX_CONSUMERS];
int consumerCount = 0;

const char *priorityName(int p) {
    if (p == 4) return "Critical-Hospital";
    if (p == 3) return "Emergency-Service";
    if (p == 2) return "Commercial";
    return "Household";
}

int addConsumer(Consumer arr[], int *count, int priority) {
    static int idGenerator = 1000;

    if (priority < 1 || priority > 4) {
        printf("Invalid priority value. Consumer not added.\n");
        return -1;
    }

    if (*count >= MAX_CONSUMERS) {
        printf("Consumer limit reached.\n");
        return -1;
    }

    idGenerator = idGenerator + 1;
    arr[*count].consumerId = idGenerator;
    arr[*count].priority = priority;
    arr[*count].recordCount = 0;
    *count = *count + 1;

    return idGenerator;
}

void updateConsumption(Consumer *c, int hour, float value) {
    int i;
    int found = 0;

    for (i = 0; i < c->recordCount; i++) {
        if (c->hourly[i].hour == hour) {
            c->hourly[i].consumption = value;
            c->hourly[i].allocated = 0;
            found = 1;
            break;
        }
    }

    if (found == 0 && c->recordCount < MAX_HOURS) {
        c->hourly[c->recordCount].hour = hour;
        c->hourly[c->recordCount].consumption = value;
        c->hourly[c->recordCount].allocated = 0;
        c->recordCount = c->recordCount + 1;
    }
}

int searchConsumerById(Consumer arr[], int count, int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (arr[i].consumerId == id) {
            return i;
        }
    }
    return -1;
}

void sortByPriorityDescending(Consumer arr[], int count) {
    int i, j;
    Consumer temp;

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - 1 - i; j++) {
            if (arr[j].priority < arr[j + 1].priority) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

float getConsumptionForHour(Consumer *c, int hour) {
    int i;
    for (i = 0; i < c->recordCount; i++) {
        if (c->hourly[i].hour == hour) {
            return c->hourly[i].consumption;
        }
    }
    return 0;
}

void setAllocatedForHour(Consumer *c, int hour, float value) {
    int i;
    for (i = 0; i < c->recordCount; i++) {
        if (c->hourly[i].hour == hour) {
            c->hourly[i].allocated = value;
            return;
        }
    }
}

void allocatePower(Consumer arr[], int count, int hour, float capacity) {
    int i;
    float remaining = capacity;
    float demand, alloc;

    sortByPriorityDescending(arr, count);

    printf("\n=== Priority-Based Power Allocation (Hour %d, Capacity %.2f kW) ===\n", hour, capacity);

    for (i = 0; i < count; i++) {
        demand = getConsumptionForHour(&arr[i], hour);

        if (demand <= 0) {
            continue;
        }

        if (remaining <= 0) {
            setAllocatedForHour(&arr[i], hour, 0);
            printf("Consumer %d [%s]: Demand %.2f kW -> Allocated 0.00 kW (SHORTAGE)\n",
                   arr[i].consumerId, priorityName(arr[i].priority), demand);
            continue;
        }

        if (demand <= remaining) {
            alloc = demand;
        } else {
            alloc = remaining;
        }

        remaining = remaining - alloc;
        setAllocatedForHour(&arr[i], hour, alloc);

        if (alloc < demand) {
            printf("Consumer %d [%s]: Demand %.2f kW -> Allocated %.2f kW (SHORTAGE)\n",
                   arr[i].consumerId, priorityName(arr[i].priority), demand, alloc);
        } else {
            printf("Consumer %d [%s]: Demand %.2f kW -> Allocated %.2f kW (FULL)\n",
                   arr[i].consumerId, priorityName(arr[i].priority), demand, alloc);
        }
    }

    printf("Remaining Unused Capacity: %.2f kW\n", remaining);
}

void displayAll(Consumer arr[], int count) {
    int i, j;
    printf("\n=== All Consumers ===\n");

    for (i = 0; i < count; i++) {
        printf("Consumer ID: %d | Priority: %s\n", arr[i].consumerId, priorityName(arr[i].priority));
        for (j = 0; j < arr[i].recordCount; j++) {
            printf("  Hour %2d -> Consumption: %6.2f kW | Allocated: %6.2f kW\n",
                   arr[i].hourly[j].hour, arr[i].hourly[j].consumption, arr[i].hourly[j].allocated);
        }
    }
}

void saveToFile(Consumer arr[], int count) {
    FILE *fp;
    int i, j;

    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(fp, "%d\n", count);
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %d %d\n", arr[i].consumerId, arr[i].priority, arr[i].recordCount);
        for (j = 0; j < arr[i].recordCount; j++) {
            fprintf(fp, "%d %.2f %.2f\n", arr[i].hourly[j].hour, arr[i].hourly[j].consumption, arr[i].hourly[j].allocated);
        }
    }

    fclose(fp);
    printf("Data saved to %s successfully.\n", FILENAME);
}

int loadFromFile(Consumer arr[]) {
    FILE *fp;
    int count, i, j, rec;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No existing data file found. Starting fresh.\n");
        return 0;
    }

    if (fscanf(fp, "%d", &count) != 1) {
        fclose(fp);
        return 0;
    }

    for (i = 0; i < count; i++) {
        fscanf(fp, "%d %d %d", &arr[i].consumerId, &arr[i].priority, &rec);
        arr[i].recordCount = rec;
        for (j = 0; j < rec; j++) {
            fscanf(fp, "%d %f %f", &arr[i].hourly[j].hour, &arr[i].hourly[j].consumption, &arr[i].hourly[j].allocated);
        }
    }

    fclose(fp);
    printf("Data loaded from %s successfully. %d consumers loaded.\n", FILENAME, count);
    return count;
}

int main() {
    int choice, id, hour, priority, idx;
    float value, capacity;

    consumerCount = loadFromFile(consumers);

    do {
        printf("\n===== Smart Power Distribution System =====\n");
        printf("1. Add Consumer\n");
        printf("2. Update Hourly Consumption\n");
        printf("3. Display All Consumers\n");
        printf("4. Search Consumer by ID\n");
        printf("5. Sort Consumers by Priority (Descending)\n");
        printf("6. Allocate Power for an Hour (Priority-Based)\n");
        printf("7. Save Data to File\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter Priority (1=Household, 2=Commercial, 3=Emergency, 4=Critical-Hospital): ");
            scanf("%d", &priority);
            id = addConsumer(consumers, &consumerCount, priority);
            if (id != -1) {
                printf("Consumer added successfully. Assigned ID: %d\n", id);
            }
        } else if (choice == 2) {
            printf("Enter Consumer ID: ");
            scanf("%d", &id);
            idx = searchConsumerById(consumers, consumerCount, id);
            if (idx == -1) {
                printf("Consumer not found.\n");
            } else {
                printf("Enter Hour (0-23): ");
                scanf("%d", &hour);
                if (hour < 0 || hour > 23) {
                    printf("Invalid hour.\n");
                } else {
                    printf("Enter Consumption (kW): ");
                    scanf("%f", &value);
                    updateConsumption(&consumers[idx], hour, value);
                    printf("Consumption updated successfully.\n");
                }
            }
        } else if (choice == 3) {
            displayAll(consumers, consumerCount);
        } else if (choice == 4) {
            printf("Enter Consumer ID to search: ");
            scanf("%d", &id);
            idx = searchConsumerById(consumers, consumerCount, id);
            if (idx == -1) {
                printf("Consumer not found.\n");
            } else {
                printf("Found -> Consumer ID: %d | Priority: %s\n", consumers[idx].consumerId, priorityName(consumers[idx].priority));
            }
        } else if (choice == 5) {
            sortByPriorityDescending(consumers, consumerCount);
            printf("Consumers sorted by priority (descending).\n");
            displayAll(consumers, consumerCount);
        } else if (choice == 6) {
            printf("Enter Hour (0-23): ");
            scanf("%d", &hour);
            printf("Enter Available Capacity (kW): ");
            scanf("%f", &capacity);
            if (capacity < 0) {
                printf("Invalid capacity.\n");
            } else {
                allocatePower(consumers, consumerCount, hour, capacity);
            }
        } else if (choice == 7) {
            saveToFile(consumers, consumerCount);
        } else if (choice == 8) {
            printf("Exiting. Saving data...\n");
            saveToFile(consumers, consumerCount);
        } else {
            printf("Invalid choice. Try again.\n");
        }

    } while (choice != 8);

    return 0;
}
