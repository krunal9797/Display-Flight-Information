#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100
#define MAX_FLIGHTS 100
#define INF 999999

struct Flight {
    char source[MAX_STRING_LENGTH];
    char destination[MAX_STRING_LENGTH];
    float fare;
    char airline[MAX_STRING_LENGTH];
};


int parseLine(const char *line, char *source, char *destination, float *fare, char *airline) {
    char tempSource[MAX_STRING_LENGTH], tempDestination[MAX_STRING_LENGTH];
    if (sscanf(line, "%[^:]: %[^-] - %[^,], $%f\n", airline, tempSource, tempDestination, fare) != 4) {
        printf("Error: Invalid format in flight data line: %s\n", line);
        return 0; // Failure
    }

    strcpy(source, tempSource);
    strcpy(destination, tempDestination);
    return 1; // Success
}



void readFlights(const char *flightsFile, struct Flight flights[], int *numFlights) {
    FILE *file = fopen(flightsFile, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", flightsFile);
        exit(EXIT_FAILURE);
    }

    char line[MAX_STRING_LENGTH];
    *numFlights = 0;
    while ((*numFlights) < MAX_FLIGHTS && fgets(line, sizeof(line), file) != NULL) {
        char source[MAX_STRING_LENGTH], destination[MAX_STRING_LENGTH], airline[MAX_STRING_LENGTH];
        float fare;
        if (parseLine(line, source, destination, &fare, airline)) {
            strcpy(flights[*numFlights].source, source);
            strcpy(flights[*numFlights].destination, destination);
            flights[*numFlights].fare = fare;
            strcpy(flights[*numFlights].airline, airline);
            (*numFlights)++;
        }
    }

    fclose(file);
}


float findLeastFare(struct Flight flights[], int numFlights, const char *source, const char *destination) {
    float leastFare = INF;
    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].source, source) == 0 && strcmp(flights[i].destination, destination) == 0) {
            if (flights[i].fare < leastFare) {
                leastFare = flights[i].fare;
            }
        }
    }
    return leastFare;
}

void displayLeastFareDetails(struct Flight flights[], int numFlights, const char *source, const char *destination) {
    printf("Least fare details for %s-%s pair:\n", source, destination);
    float minFare = INF;
    int found = 0;

    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].source, source) == 0 && strcmp(flights[i].destination, destination) == 0) {
            if (flights[i].fare < minFare) {
                minFare = flights[i].fare;
                found = 1;
            }
        }
    }

    if (found && minFare != INF) {
        for (int i = 0; i < numFlights; i++) {
            if (strcmp(flights[i].source, source) == 0 && strcmp(flights[i].destination, destination) == 0 && flights[i].fare == minFare) {
                printf("%s: %s to %s, Fare $%.2f\n", flights[i].airline, flights[i].source, flights[i].destination, flights[i].fare);
            }
        }
    } else {
        printf("No flights found for the given source-destination pair.\n");
    }
}

int main() {
    struct Flight flights[MAX_FLIGHTS];
    int numFlights;

    readFlights("flights.txt", flights, &numFlights);

    char source[MAX_STRING_LENGTH], destination[MAX_STRING_LENGTH];
    printf("Enter source: ");
    fgets(source, sizeof(source), stdin);
    printf("Enter destination: ");
    fgets(destination, sizeof(destination), stdin);
    source[strcspn(source, "\n")] = '\0'; // Remove trailing newline character
    destination[strcspn(destination, "\n")] = '\0'; // Remove trailing newline character

    float leastFare = findLeastFare(flights, numFlights, source, destination);

    if (leastFare == INF) {
        printf("No flights found for the given source-destination pair.\n");
    } else {
        printf("Least fare found for %s-%s pair: $%.2f\n", source, destination, leastFare);
    }

    displayLeastFareDetails(flights, numFlights, source, destination);

    return 0;
}
