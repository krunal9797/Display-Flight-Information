#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Include ctype.h for isspace function

#define MAX_STRING_LENGTH 100
#define MAX_FLIGHTS 100
#define INF 999999

struct Flight {
    char source[MAX_STRING_LENGTH];
    char destination[MAX_STRING_LENGTH];
    float fare;
    char airline[MAX_STRING_LENGTH];
};

// Function prototype
int parseLine(const char *line, char *source, char *destination, float *fare, char *airline);
void readFlights(const char *flightsFile, struct Flight flights[], int *numFlights);
float findLeastFare(struct Flight flights[], int numFlights, const char *source, const char *destination);
void displayLeastFareDetails(struct Flight flights[], int numFlights, const char *source, const char *destination);
int strcasecmp(const char *s1, const char *s2);

int main() {
    struct Flight flights[MAX_FLIGHTS];
    int numFlights;

    readFlights("flights.txt", flights, &numFlights);

    char source[MAX_STRING_LENGTH], destination[MAX_STRING_LENGTH];
    printf("Enter source: ");
    fgets(source, sizeof(source), stdin);
    printf("Enter destination: ");
    fgets(destination, sizeof(destination), stdin);

    // Remove trailing newline character
    source[strcspn(source, "\n")] = '\0';
    destination[strcspn(destination, "\n")] = '\0';

    float leastFare = findLeastFare(flights, numFlights, source, destination);

    if (leastFare == INF) {
        printf("No flights found for the given source-destination pair.\n");
    } else {
        printf("Least fare found for %s-%s pair: $%.2f\n", source, destination, leastFare);
    }

    displayLeastFareDetails(flights, numFlights, source, destination);

    return 0;
}

int parseLine(const char *line, char *source, char *destination, float *fare, char *airline) {
    char tempAirline[MAX_STRING_LENGTH];
    char tempSource[MAX_STRING_LENGTH];
    char tempDestination[MAX_STRING_LENGTH];
    if (sscanf(line, "%[^:]: %[^-] - %[^,], $%f", tempAirline, tempSource, tempDestination, fare) != 4) {
        printf("Error: Invalid format in flight data line: %s\n", line);
        return 0; // Failure
    }

    // Remove leading and trailing spaces from source and destination
    int i = 0, j = strlen(tempSource) - 1;
    while (isspace((unsigned char)tempSource[i])) {
        i++;
    }
    while ((j >= i) && isspace((unsigned char)tempSource[j])) {
        j--;
    }
    tempSource[j + 1] = '\0';
    strcpy(source, tempSource);

    i = 0, j = strlen(tempDestination) - 1;
    while (isspace((unsigned char)tempDestination[i])) {
        i++;
    }
    while ((j >= i) && isspace((unsigned char)tempDestination[j])) {
        j--;
    }
    tempDestination[j + 1] = '\0';
    strcpy(destination, tempDestination);

    strcpy(airline, tempAirline);
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
        //printf("Read line: %s\n", line); // Debug message
        char source[MAX_STRING_LENGTH], destination[MAX_STRING_LENGTH], airline[MAX_STRING_LENGTH];
        float fare;
        if (parseLine(line, source, destination, &fare, airline)) {
            //printf("Parsed: Airline: %s, Source: %s, Destination: %s, Fare: %.2f\n", airline, source, destination, fare); // Debug message
            strcpy(flights[*numFlights].source, source);
            strcpy(flights[*numFlights].destination, destination);
            flights[*numFlights].fare = fare;
            strcpy(flights[*numFlights].airline, airline);
            (*numFlights)++;
        } else {
            printf("Failed to parse line: %s\n", line); // Debug message
        }
    }

    fclose(file);
}

float findLeastFare(struct Flight flights[], int numFlights, const char *source, const char *destination) {
    float leastFare = INF;
    for (int i = 0; i < numFlights; i++) {
        if (strcasecmp(flights[i].source, source) == 0 && strcasecmp(flights[i].destination, destination) == 0) {
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
        if (strcasecmp(flights[i].source, source) == 0 && strcasecmp(flights[i].destination, destination) == 0) {
            if (flights[i].fare < minFare) {
                minFare = flights[i].fare;
                found = 1;
            }
        }
    }

    if (found && minFare != INF) {
        for (int i = 0; i < numFlights; i++) {
            if (strcasecmp(flights[i].source, source) == 0 && strcasecmp(flights[i].destination, destination) == 0 && flights[i].fare == minFare) {
                printf("%s: %s - %s, Fare: $%.2f\n", flights[i].airline, flights[i].source, flights[i].destination, flights[i].fare);
            }
        }
    } else {
        printf("No flights found for the given source-destination pair.\n");
    }
}

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (toupper(*s1) != toupper(*s2)) {
            return (toupper(*s1) - toupper(*s2));
        }
        s1++;
        s2++;
    }
    return (toupper(*s1) - toupper(*s2));
}
