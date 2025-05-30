#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct dan Konstanta
typedef struct {
    char id[20];
    char vendor[50];
    int yearBought;
    int lastServiced;
    char status[30];
    int nextService;

    char patientID[20];
    char patientHistory[100];
    char usageTimestamp[30];
    char lastRoom[50];
    char notes[100];
} Equipment;

typedef struct {
    char category[50];
    int baseID;
    int currentCount;
} EquipmentCategoryID;

EquipmentCategoryID idGenerators[100];
int categoryCount = 0;

// Prototypes
int getCategoryPrefix(const char *categoryName);
char* getCurrentDateTime(char *buffer, size_t size);
void engineerMenu(Equipment ***list, int *count);
void nakesMenu(Equipment **list, int count);
void addEquipment(Equipment ***list, int *count);
void viewEquipment(Equipment **list, int count, int isEngineer);
void inputPatientInformation(Equipment **list, int count);
void checkEquipmentStatus(Equipment **list, int count, int isEngineer);

// ID Generator
int getCategoryPrefix(const char *categoryName) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(idGenerators[i].category, categoryName) == 0) {
            idGenerators[i].currentCount++;
            return idGenerators[i].baseID + idGenerators[i].currentCount;
        }
    }
    strcpy(idGenerators[categoryCount].category, categoryName);
    idGenerators[categoryCount].baseID = (categoryCount + 1) * 10000;
    idGenerators[categoryCount].currentCount = 1;
    int id = idGenerators[categoryCount].baseID + idGenerators[categoryCount].currentCount;
    categoryCount++;
    return id;
}

// Main
int main() {
    Equipment **equipmentList = NULL;
    int equipmentCount = 0;
    int role;

    while (1) {
        printf("\n=== MEDICAL EQUIPMENT SYSTEM ===\n");
        printf("Choose your role:\n");
        printf("1. Engineer\n");
        printf("2. Nakes (Medical Practitioner)\n");
        printf("3. Exit\n");
        printf("Your role: ");
        if (scanf("%d", &role) != 1) break;
        getchar();

        if (role == 1) {
            engineerMenu(&equipmentList, &equipmentCount);
        } else if (role == 2) {
            nakesMenu(equipmentList, equipmentCount);
        } else if (role == 3) {
            printf("Exiting program.\n");
            for (int i = 0; i < equipmentCount; i++) free(equipmentList[i]);
            free(equipmentList);
            break;
        } else {
            printf("Invalid role.\n");
        }
    }
    return 0;
}

// Engineer Menu
void engineerMenu(Equipment ***list, int *count) {
    int choice;
    while (1) {
        printf("\n[ENGINEER MENU]\n");
        printf("1. Add New Equipment\n");
        printf("2. View Equipment Details\n");
        printf("3. Back to Choosing Role\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) break;
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
    char category[50], vendor[50];
    int quantity, yearBought, lastServiced;

    printf("\n[ADD NEW EQUIPMENT]\n");
    printf("Category: ");
    fgets(category, sizeof(category), stdin); strtok(category, "\n");

    printf("Vendor/Type: ");
    fgets(vendor, sizeof(vendor), stdin); strtok(vendor, "\n");

    printf("Quantity: ");
    scanf("%d", &quantity); getchar();

    printf("Year Bought: ");
    scanf("%d", &yearBought); getchar();

    printf("Last Maintenance Year: ");
    scanf("%d", &lastServiced); getchar();

    for (int i = 0; i < quantity; i++) {
        Equipment *e = (Equipment*)malloc(sizeof(Equipment));
        if (!e) {
            printf("Memory allocation failed.\n");
            return;
        }
        int id = getCategoryPrefix(category);
        sprintf(e->id, "%05d", id);
        snprintf(e->vendor, sizeof(e->vendor), "%s (%s)", category, vendor);
        e->yearBought = yearBought;
        e->lastServiced = lastServiced;
        e->nextService = lastServiced + 1;
        strcpy(e->status, "Available");

        strcpy(e->lastRoom, "-");
        strcpy(e->patientID, "-");
        strcpy(e->patientHistory, "-");
        strcpy(e->usageTimestamp, "-");
        strcpy(e->notes, "-");

        *list = realloc(*list, (*count + 1) * sizeof(Equipment*));
        (*list)[*count] = e;
        (*count)++;
    }
    printf("Equipment %s (%s) added successfully (%d units).\n", category, vendor, quantity);
}

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
    scanf("%d", &opt); getchar();
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
                printf("\nStatus alat saat ini menunjukkan perlu perbaikan.\n");
                printf("Choose status update:\n");
                printf("1. Sudah diperbaiki (Available)\n");
                printf("2. Sedang diperbaiki (Under Maintenance)\n");
                printf("Choice: ");
                scanf("%d", &choice); getchar();

                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                int currentYear = tm.tm_year + 1900;

                if (choice == 1) {
                    strcpy(e->status, "Available");
                    strcpy(e->notes, "-");
                    e->lastServiced = currentYear;
                    e->nextService = currentYear + 1;
                } else if (choice == 2) {
                    strcpy(e->status, "Under Maintenance");
                    // lastServiced tidak diperbarui
                } else {
                    printf("Invalid choice.\n");
                    return;
                }

                char updatedTime[30];
                getCurrentDateTime(updatedTime, sizeof(updatedTime));
                printf("Equipment status updated.\nLast update: %s\n", updatedTime);
            }
            return;
        }
    }
    printf("Equipment not found.\n");
}

// Nakes Menu
void nakesMenu(Equipment **list, int count) {
    int choice;
    while (1) {
        printf("\n[NAKES MENU]\n");
        printf("1. Input Patient Information\n");
        printf("2. Status Kerja Alat\n");
        printf("3. Back to Main Menu\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) break;
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

// Status Kerja Alat dari Nakes (dengan input catatan kerusakan)
void checkEquipmentStatus(Equipment **list, int count, int isEngineer) {
    char equipmentID[20];
    printf("Enter equipment ID to check status: ");
    fgets(equipmentID, sizeof(equipmentID), stdin); strtok(equipmentID, "\n");

    for (int i = 0; i < count; i++) {
        Equipment *found = list[i];
        if (strcmp(equipmentID, found->id) == 0) {
            char broken[10];
            printf("Is the equipment malfunctioning? (yes/no): ");
            fgets(broken, sizeof(broken), stdin); strtok(broken, "\n");

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            int currentYear = tm.tm_year + 1900;

            if (strcmp(broken, "yes") == 0) {
                printf("Enter damage notes: ");
                fgets(found->notes, sizeof(found->notes), stdin); strtok(found->notes, "\n");
                strcpy(found->status, "Out of Service");

                // Jangan ubah tahun lastServiced saat hanya pelaporan kerusakan
                found->nextService = found->lastServiced + 1;

                printf("Equipment marked as out of service and noted.\n");
            } else {
                strcpy(found->status, "Available");
                strcpy(found->notes, "-");
                int yearsPassed = currentYear - found->lastServiced;
                if (yearsPassed >= 1) {
                    printf("Alat baik-baik saja, servis selanjutnya pada tahun %d.\n", found->lastServiced + 1);
                } else {
                    printf("Alat sudah didata dan akan segera diproses.\n");
                }
            }

            viewEquipment(list, count, 0);
            return;
        }
    }
    printf("Equipment not found.\n");
}

// Input Data Pasien
void inputPatientInformation(Equipment **list, int count) {
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

    getCurrentDateTime(found->usageTimestamp, sizeof(found->usageTimestamp));
    printf("Timestamp automatically recorded: %s\n", found->usageTimestamp);

    printf("Room Used: ");
    fgets(found->lastRoom, sizeof(found->lastRoom), stdin); strtok(found->lastRoom, "\n");
}

// Get Current DateTime String
char* getCurrentDateTime(char *buffer, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}
