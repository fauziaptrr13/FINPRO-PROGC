#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipment_data.h"
#include "nakes_functions.h"
#include "engineer_functions.h"

void extractBaseCategory(const char *fullVendorString, char *baseCategory, size_t maxLen) {
    const char *firstParen = strchr(fullVendorString, '(');
    if (firstParen != NULL) {
        size_t length = firstParen - fullVendorString;
        if (length > 0 && fullVendorString[length - 1] == ' ') {
            length--;
        }
        if (length >= maxLen) {
            length = maxLen - 1;
        }
        strncpy(baseCategory, fullVendorString, length);
        baseCategory[length] = '\0';
    } else {
        strncpy(baseCategory, fullVendorString, maxLen - 1);
        baseCategory[maxLen - 1] = '\0';
    }
}

void loadEquipmentDatabase(Equipment ***equipmentList, int *equipmentCount) {
    FILE *file = fopen("equipment.txt", "r");
    if (!file) {
        printf("Could not open equipment.txt. Starting with empty list.\n");
        return;
    }

    Equipment *temp_eq_ptr;
    char id_str[20], vendor_str[50], room_str[50], status_str[30], patientID_str[20];
    int lastServiced_val, yearBought_val;

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d\n",
                  id_str, vendor_str, room_str, status_str, patientID_str, &lastServiced_val, &yearBought_val) == 7) {
        temp_eq_ptr = (Equipment *)malloc(sizeof(Equipment));
        if (!temp_eq_ptr) {
            printf("Memory allocation failed during DB load.\n");
            break;
        }

        strcpy(temp_eq_ptr->id, id_str);
        strcpy(temp_eq_ptr->vendor, vendor_str);
        strcpy(temp_eq_ptr->lastRoom, room_str);
        strcpy(temp_eq_ptr->status, status_str);
        strcpy(temp_eq_ptr->patientID, patientID_str);
        temp_eq_ptr->lastServiced = lastServiced_val;
        temp_eq_ptr->yearBought = yearBought_val;
        temp_eq_ptr->nextService = lastServiced_val + 1;

        strcpy(temp_eq_ptr->patientHistory, "-");
        strcpy(temp_eq_ptr->usageTimestamp, "-");
        strcpy(temp_eq_ptr->notes, "-");

        Equipment **tempList = realloc(*equipmentList, (*equipmentCount + 1) * sizeof(Equipment *));
        if (tempList == NULL) {
            printf("Failed to reallocate memory for equipment list.\n");
            free(temp_eq_ptr); 
            break; 
        }
        *equipmentList = tempList;
        (*equipmentList)[*equipmentCount] = temp_eq_ptr;
        (*equipmentCount)++;

        char baseCat[50];
        extractBaseCategory(temp_eq_ptr->vendor, baseCat, sizeof(baseCat)); 

        int numericIdFromFile = atoi(temp_eq_ptr->id); 
        int loadedBaseIdValue = (numericIdFromFile / 10000) * 10000; 
        int loadedCurrentCountValue = numericIdFromFile % 10000;   

        int foundCategoryIdx = -1;
        for (int i = 0; i < categoryCount; i++) {
            if (strcmp(idGenerators[i].category, baseCat) == 0) {
                foundCategoryIdx = i;
                break;
            }
        }

        if (foundCategoryIdx != -1) {
             if (idGenerators[foundCategoryIdx].baseID != loadedBaseIdValue) {
             }
            if (loadedCurrentCountValue > idGenerators[foundCategoryIdx].currentCount) {
                idGenerators[foundCategoryIdx].currentCount = loadedCurrentCountValue;
            }
        } else { 
            if (categoryCount < 100) {
                strcpy(idGenerators[categoryCount].category, baseCat);
                idGenerators[categoryCount].baseID = loadedBaseIdValue;
                idGenerators[categoryCount].currentCount = loadedCurrentCountValue;
                categoryCount++;
            } else {
                fprintf(stderr, "Warning: Max categories reached during DB load for ID priming (%s).\n", baseCat);
            }
        }
    }

    fclose(file);
    printf("%d equipment(s) loaded from equipment.txt.\n", *equipmentCount);
}

int main() {
    Equipment **equipmentList = NULL;
    int equipmentCount = 0;
    int role;

    char useDatabase;
    printf("Do you want to use existing equipment database? (y/n): ");
    scanf(" %c", &useDatabase);
    getchar();

    if (useDatabase == 'y' || useDatabase == 'Y') {
        loadEquipmentDatabase(&equipmentList, &equipmentCount);
    } else {
        printf("Starting with empty equipment list. ID generation will start from defaults (10001, 20001, etc.).\n");
    }

    while (1) {
        printf("\n=== MEDICAL EQUIPMENT SYSTEM ===\n");
        printf("Choose your role:\n");
        printf("1. Engineer\n");
        printf("2. Nakes (Medical Practitioner)\n");
        printf("3. Exit\n");
        printf("Your role: ");
        if (scanf("%d", &role) != 1) {
            while(getchar() != '\n');
            printf("Invalid input, please enter a number.\n");
            continue;
        }
        getchar();

        if (role == 1) {
            engineerMenu(&equipmentList, &equipmentCount);
        } else if (role == 2) {
            nakesMenu(equipmentList, equipmentCount);
        } else if (role == 3) {
            printf("Exiting program.\n");
            for (int i = 0; i < equipmentCount; i++) {
                free(equipmentList[i]);
            }
            free(equipmentList);
            break;
        } else {
            printf("Invalid role.\n");
        }
    }
    return 0;
}