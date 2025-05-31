#include "shared_functions.h"
#include <stdio.h>
#include <stdlib.h> // Untuk time_t, localtime, tm
#include <string.h> // Untuk strcmp, strlen
#include <time.h>   // Untuk time, localtime


// View Equipment with Detail and Optional Status Update
void viewEquipment(Equipment **list, int count, int isEngineer) {
    if (count == 0) {
        printf("No equipment records available.\n");
        return;
    }

    printf("\n--- EQUIPMENT TABLE ---\n");
    printf("| %-6s | %-20s | %-15s | %-20s | %-12s | %-10s |\n",
           "ID", "Name", "Room Used", "Status", "Patient ID", "Calibrated");
    printf("---------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        Equipment *e = list[i];
        const char *room = (strcmp(e->lastRoom, "-") == 0 || strlen(e->lastRoom) == 0) ? "-" : e->lastRoom;
        const char *patient = (strcmp(e->patientID, "-") == 0) ? "-" : e->patientID;
        printf("| %-6s | %-20s | %-15s | %-20s | %-12s | %-10d |\n",
               e->id, e->vendor, room, e->status, patient, e->lastServiced);
    }
    printf("---------------------------------------------------------------------------------------------\n");

    printf("View full details of a specific equipment? (1 = Yes, 2 = No): ");
    int opt;
    if (scanf("%d", &opt) != 1) {
        while(getchar() != '\n');
        return;
    }
    getchar();
    if (opt != 1) return;

    char searchID[20];
    printf("Enter equipment ID: ");
    fgets(searchID, sizeof(searchID), stdin); strtok(searchID, "\n");

    for (int i = 0; i < count; i++) {
        Equipment *e = list[i];
        if (strcmp(e->id, searchID) == 0) {
            printf("\n--- EQUIPMENT DETAILS ---\n");
            printf("ID: %s\nVendor: %s\n", e->id, e->vendor);
            printf("Bought: %d, Last Serviced: %d, Next Service: %d\n", e->yearBought, e->lastServiced, e->nextService);
            printf("Status: %s\n", e->status);
            printf("Last Room Used: %s\n", e->lastRoom);
            printf("Last Patient ID: %s\n", e->patientID);
            printf("Patient History: %s\n", e->patientHistory);
            printf("Last Used: %s\n", e->usageTimestamp);
            printf("Notes: %s\n", e->notes);

            if (isEngineer && 
                (strcmp(e->status, "Out of Service") == 0 || strcmp(e->status, "Under Maintenance") == 0)) {
                int choice;
                printf("\nEquipment status requires maintenance.\n");
                printf("Choose status update:\n");
                printf("1. The equipment has been fixed (Available)\n");
                printf("2. The equipment is being fixed (Under Maintenance)\n");
                printf("Choice: ");
                if (scanf("%d", &choice) != 1) {
                     while(getchar() != '\n');
                     return;
                }
                getchar();

                time_t t = time(NULL);
                struct tm tm_info = *localtime(&t);
                int currentYear = tm_info.tm_year + 1900;

                if (choice == 1) {
                    strcpy(e->status, "Available");
                    strcpy(e->notes, "-");
                    e->lastServiced = currentYear;
                    e->nextService = currentYear + 1;
                } else if (choice == 2) {
                    strcpy(e->status, "Under Maintenance");
                } else {
                    printf("Invalid choice.\n");
                    return;
                }

                char updatedTime[30];
                getCurrentDateTime(updatedTime, sizeof(updatedTime)); // Dari utils.h/.c
                printf("Equipment status updated.\nLast update: %s\n", updatedTime);
            }
            return;
        }
    }
    printf("Equipment not found.\n");
}
