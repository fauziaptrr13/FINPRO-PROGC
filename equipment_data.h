#ifndef EQUIPMENT_DATA_H
#define EQUIPMENT_DATA_H

#include <stdio.h> // Untuk size_t jika diperlukan di sini, tapi lebih baik di .c
#include <string.h> // Untuk strcmp, strcpy

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

extern EquipmentCategoryID idGenerators[100];
extern int categoryCount;

int getCategoryPrefix(const char *categoryName);

#endif