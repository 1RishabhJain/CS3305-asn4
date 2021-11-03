#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CS3305A - Assignment 4
 * November  8th, 2021
 * Rishabh Jain
 */

int main() {
    // Creating array to be used by sprintf, the line being read, the individual values, and delimmeter
    char output[256];
    char fileLine[256];
    char lineValues[256];
    char delim[] = " ";
    int arrayPointer;
    int quantumTime = 0;

    // File "rr_input.txt" opened in read mode
    FILE* inputFile = fopen("rr_input.txt", "r");

    // Check if the file pointer is NULL
    if (inputFile == NULL){
        sprintf(output, "Unable to open the file");
        puts(output);
        exit(1);
    }

    // Continue to read lines until no more lines are left
    while (fgets(fileLine, sizeof(fileLine), inputFile)) {
        sprintf(output, "%s", fileLine);
        puts(output);

        // Continue splitting line around " " until NULL
        char *ptr = strtok(fileLine, delim);
        arrayPointer = 0;
        while(ptr != NULL) {
            printf("%s\n", ptr);
            lineValues[arrayPointer] = *ptr;
            ptr = strtok(NULL, delim);
            arrayPointer = arrayPointer + 1;
        }

        int n = strlen(lineValues);
        printf("Size of %d\n", n);        

        for (int i = 0; i < n; i++) {
            printf("Current value %c\n", lineValues[i]);
        }
        quantumTime = lineValues[n - 1];






    }

    fclose(inputFile);
    return 0;
}
