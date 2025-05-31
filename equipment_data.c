#include "equipment_data.h"
#include <string.h> // Included via equipment_data.h
#include <stdio.h>  // For fprintf, stderr if errors occur

EquipmentCategoryID idGenerators[100];
int categoryCount = 0;

int getCategoryPrefix(const char *pureCategoryName) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(idGenerators[i].category, pureCategoryName) == 0) {
            idGenerators[i].currentCount++;
            return idGenerators[i].baseID + idGenerators[i].currentCount;
        }
    }

    if (categoryCount < 100) { 
        strcpy(idGenerators[categoryCount].category, pureCategoryName);
        
        idGenerators[categoryCount].baseID = (categoryCount + 1) * 10000;
        
        idGenerators[categoryCount].currentCount = 0; 
        
        categoryCount++; 

        idGenerators[categoryCount - 1].currentCount++; 
        return idGenerators[categoryCount - 1].baseID + idGenerators[categoryCount - 1].currentCount;
    } else {
        fprintf(stderr, "Error: Maximum number of categories reached. Cannot add new category %s.\n", pureCategoryName);
        return -1;
    }
}