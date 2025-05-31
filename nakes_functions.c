#include "nakes_functions.h"
#include "shared_functions.h" // Untuk viewEquipment
#include "utils.h"            // Untuk getCurrentDateTime
#include <stdio.h>
#include <stdlib.h> // Untuk time_t, localtime, tm
#include <string.h> // Untuk strcmp, strcpy, strtok
#include <time.h>   // Untuk time, localtime

void nakesMenu(Equipment **list, int count) {
    int choice;
    while (1) {
        printf("\n[NAKES MENU]\n");
        printf("1. Input Patient Information\n");
        printf("2. Equipment Status\n");
        printf("3. Back to Main Menu\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input, please enter a number.\n");
            continue;
        }
        getchar();

        if (choice == 1) {
            inputPatientInformation(list, count);
        } else if (choice == 2) {
            viewEquipment(list, count, 0);
            checkEquipmentStatus(list, count, 0);
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid input.\n");
        }
    }
}

void inputPatientInformation(Equipment **list, int count) {
    if (count == 0) {
        printf("No equipment available to assign patient information.\n");
        return;
    }
    char equipmentID[20];
    printf("Enter equipment ID used: ");
    fgets(equipmentID, sizeof(equipmentID), stdin); strtok(equipmentID, "\n");

    Equipment *found = NULL;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i]->id, equipmentID) == 0) {
            found = list[i];
            break;
        }
    }
    if (!found) {
        printf("Equipment not found.\n");
        return;
    }

    printf("Patient ID: ");
    fgets(found->patientID, sizeof(found->patientID), stdin); strtok(found->patientID, "\n");

    printf("Patient History (brief): ");
    fgets(found->patientHistory, sizeof(found->patientHistory), stdin); strtok(found->patientHistory, "\n");

    getCurrentDateTime(found->usageTimestamp, sizeof(found->usageTimestamp)); // Dari utils.c
    printf("Timestamp automatically recorded: %s\n", found->usageTimestamp);

    printf("Room Used: ");
    fgets(found->lastRoom, sizeof(found->lastRoom), stdin); strtok(found->lastRoom, "\n");
    
    printf("Patient information updated for equipment ID %s.\n", found->id);
}

void checkEquipmentStatus(Equipment **list, int count, int isEngineer) {
    if (count == 0) {
        printf("No equipment available to check status.\n");
        return;
    }
    char equipmentID[20];
    printf("Enter equipment ID to check/update status: ");
    fgets(equipmentID, sizeof(equipmentID), stdin); strtok(equipmentID, "\n");

    for (int i = 0; i < count; i++) {
        Equipment *found = list[i];
        if (strcmp(equipmentID, found->id) == 0) {
            printf("\nCurrent status for ID %s: %s\n", found->id, found->status);
            char broken[10];
            printf("Is the equipment malfunctioning? (yes/no): ");
            fgets(broken, sizeof(broken), stdin); strtok(broken, "\n");

            time_t t = time(NULL);
            struct tm tm_info = *localtime(&t);
            int currentYear = tm_info.tm_year + 1900;

            if (strcmp(broken, "yes") == 0) {
                printf("Enter damage notes: ");
                fgets(found->notes, sizeof(found->notes), stdin); strtok(found->notes, "\n");
                strcpy(found->status, "Out of Service");

                found->nextService = found->lastServiced + 1; 

                printf("Equipment marked as out of service and noted.\n");
            } else if (strcmp(broken, "no") == 0) { 
                if (strcmp(found->status, "Out of Service") == 0 || strcmp(found->status, "Under Maintenance") == 0) {
                     printf("Equipment status is '%s'. Please contact an engineer for status update if it's now working.\n", found->status);
                } else {
                    strcpy(found->status, "Available"); // Hanya set Available jika memang sebelumnya tidak rusak parah
                    strcpy(found->notes, "-");
                    printf("Equipment status confirmed as Available.\n");
                }
                
                if (currentYear >= found->nextService) {
                     printf("Reminder: This equipment is due for or overdue for service (Next Service Year: %d).\n", found->nextService);
                } else {
                     printf("This equipment is scheduled for next service in %d.\n", found->nextService);
                }

            } else {
                printf("Invalid input for malfunctioning status. No changes made.\n");
            }
            
            printf("\nUpdated Details for Equipment ID %s:\n", found->id);
            printf("Status: %s\n", found->status);
            printf("Notes: %s\n", found->notes);
            printf("Next Service Year: %d\n", found->nextService);
            return;
        }
    }
    printf("Equipment not found.\n");
}
