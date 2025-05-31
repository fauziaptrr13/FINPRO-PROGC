#include "engineer_functions.h"
#include "shared_functions.h" // Untuk viewEquipment
#include <stdio.h>
#include <stdlib.h> // Untuk malloc, realloc
#include <string.h> // Untuk strtok, strcpy, snprintf

// Engineer Menu
void engineerMenu(Equipment ***list, int *count) {
    int choice;
    while (1) {
        printf("\n[ENGINEER MENU]\n");
        printf("1. Add New Equipment\n");
        printf("2. View Equipment Details\n");
        printf("3. Back to Choosing Role\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input, please enter a number.\n");
            continue;
        }
        getchar();

        if (choice == 1) {
            addEquipment(list, count);
        } else if (choice == 2) {
            viewEquipment(*list, *count, 1);
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid input.\n");
        }
    }
}

// Add Equipment
void addEquipment(Equipment ***list, int *count) {
    char pure_category[50], vendor_details[50];
    int quantity, yearBought, lastServiced;

    printf("\n[ADD NEW EQUIPMENT]\n");
    printf("Category : ");
    fgets(pure_category, sizeof(pure_category), stdin); strtok(pure_category, "\n");

    printf("Vendor/Type : ");
    fgets(vendor_details, sizeof(vendor_details), stdin); strtok(vendor_details, "\n");

    printf("Quantity: ");
    if (scanf("%d", &quantity) != 1) {
        while(getchar() != '\n');
        printf("Invalid quantity.\n");
        return;
    }
    getchar(); 

    printf("Year Bought: ");
     if (scanf("%d", &yearBought) != 1) {
        while(getchar() != '\n');
        printf("Invalid year.\n");
        return;
    }
    getchar();

    printf("Last Maintenance Year: ");
    if (scanf("%d", &lastServiced) != 1) {
        while(getchar() != '\n');
        printf("Invalid year.\n");
        return;
    }
    getchar();
    for (int i = 0; i < quantity; i++) {
        Equipment *e = (Equipment*)malloc(sizeof(Equipment));
        if (!e) {
            printf("Memory allocation failed.\n");
            return;
        }
        int full_id_value = getCategoryPrefix(pure_category);
        if (full_id_value == -1) {
            printf("Failed to generate equipment ID. Aborting add equipment.\n");
            free(e);
            return;
        }
        sprintf(e->id, "%d", full_id_value); 
        snprintf(e->vendor, sizeof(e->vendor), "%s (%s)", pure_category, vendor_details);
        
        e->yearBought = yearBought;
        e->lastServiced = lastServiced;
        e->nextService = lastServiced + 1;
        strcpy(e->status, "Available");

        strcpy(e->lastRoom, "-");
        strcpy(e->patientID, "-");
        strcpy(e->patientHistory, "-");
        strcpy(e->usageTimestamp, "-");
        strcpy(e->notes, "-");

        // Add to list
        Equipment **temp_list = realloc(*list, (*count + 1) * sizeof(Equipment*));
        if (!temp_list) {
            printf("Memory reallocation failed.\n");
            free(e);
            return;
        }
        *list = temp_list;
        (*list)[*count] = e;
        (*count)++;
    }
    printf("Equipment %s (%s) added successfully (%d units).\n", pure_category, vendor_details, quantity);
}