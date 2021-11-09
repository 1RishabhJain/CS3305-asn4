#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * CS3305A - Assignment 4
 * November  8th, 2021
 * Rishabh Jain
 */

struct ProcessDetails {
    // Process ID for use with the queue
    int id;
    // Boolean to store value indicating if the process has been added to the queue
    bool notAdded;
    char processName[10];
    int arrivalTime;
    int burstTime;
    int timeLeft;
    int waitingTime;
    int tat;
};

int main() {
    // Array for output using sprintf
    char output[256];
    char fileLine[256];
    char delim[] = " ";

    // File "rr_input.txt" opened in read mode
    FILE *file = fopen("rr_input.txt", "r");

    // Check if the file pointer is NULL
    if (file == NULL) {
        sprintf(output, "Unable to open the file");
        puts(output);
        exit(1);
    }

    // Continue to read lines until no more lines are left
    while (fgets(fileLine, sizeof(fileLine), file)) {
        // Array to hold individual line values
        char lineValues[100][10];

        // Number of values in the line
        int count = 0;

        // Continue splitting line around " " until NULL
        char *ptr = strtok(fileLine, delim);
        while (ptr != NULL) {
            ptr[strcspn(ptr, "\r\n")] = 0;
            strcpy(&lineValues[count][0], ptr);
            ptr = strtok(NULL, delim);
            count = count + 1;
        }

        // Number of processes
        int numOfProcesses = (count - 1) / 3;

        // Creating the right number of processes
        struct ProcessDetails process[numOfProcesses];

        // Value that points to lineValue array
        int index = 0;
        // Integer to store total time needed to complete all processes
        int totalTime = 0;

        //  Iterate through the struct processes and set the variables
        for (int i = 0; i < numOfProcesses; i++) {
            process[i].id = i;
            process[i].notAdded = true;
            strcpy(process[i].processName, lineValues[index]);
            process[i].arrivalTime = atoi(lineValues[index + 1]);
            process[i].burstTime = atoi(lineValues[index + 2]);
            process[i].timeLeft = process[i].burstTime;
            totalTime = totalTime + process[i].burstTime;
            // Index incremented by 3 since each process uses 3 array values (name, arrival time, burst time)
            index = index + 3;
        }

        int quantumTime = atoi(lineValues[count - 1]);
        int readyQueue[200];
        int queueInsert = 0;
        int current = 0;
        int currentTime = 0;
        int processID;
        float totalWaitingTime = 0;
        float totalTAT = 0;

        // Insert first process into ready queue
        readyQueue[queueInsert] = process[0].id;
        process[0].notAdded = false;
        queueInsert++;

        // Loop to continue until all processes complete
        while (totalTime > 0) {
            processID = readyQueue[current];

            // When process is on its last run
            if (process[processID].timeLeft <= quantumTime) {
                // Decrease the total time counter by the time remaining for the process
                totalTime = totalTime - process[processID].timeLeft;
                // Increase current time by the time remaining for the process
                currentTime = currentTime + process[processID].timeLeft;
                // Set time left to 0
                process[processID].timeLeft = 0;
                // Calculate turnaround time
                process[processID].tat = currentTime - process[processID].arrivalTime;
                process[processID].waitingTime = process[processID].tat - process[processID].burstTime;
                totalWaitingTime = totalWaitingTime + process[processID].waitingTime;
                totalTAT = totalTAT + process[processID].tat;
            } else {
                // Decrease the total time counter by the quantum time
                totalTime = totalTime - quantumTime;
                // Increase current time by quantum time
                currentTime = currentTime + quantumTime;
                // Decrease the process time that is remaining
                process[processID].timeLeft = process[processID].timeLeft - quantumTime;
            }

            // Indicate that current process has been run at least one
            process[processID].notAdded = false;

            // Iterate through processes to see if any new arrived
            for (int i = 0; i < numOfProcesses; i++) {
                if (process[i].arrivalTime <= currentTime && process[i].notAdded) {
                    readyQueue[queueInsert] = process[i].id;
                    process[i].notAdded = false;
                    queueInsert++;
                }
            }

            // Will re-queue the process if not complete
            if (process[processID].timeLeft > 0) {
                readyQueue[queueInsert] = process[processID].id;
                queueInsert++;
            }
            current++;
        }

        printf("\n");
        for (int i = 0; i < numOfProcesses; i++) {
            sprintf(output, "Process: %s Arrival Time: %d Burst Time: %d Waiting Time: %d Turnaround Time: %d",
            process[i].processName, process[i].arrivalTime, process[i].burstTime, process[i].waitingTime, process[i].tat);
            puts(output);
        }
        printf("\n");
        sprintf(output, "Total Turnaround Time: %.0f", totalTAT);
        puts(output);
        sprintf(output, "Average waiting time = %.2f", totalWaitingTime / numOfProcesses);
        puts(output);
        sprintf(output, "Average turnaround time = %.2f", totalTAT / numOfProcesses);
        puts(output);
    }
    return 0;
}
