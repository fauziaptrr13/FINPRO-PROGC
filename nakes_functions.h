#ifndef NAKES_FUNCTIONS_H
#define NAKES_FUNCTIONS_H

#include "equipment_data.h" // Butuh definisi Equipment

// Prototipe Fungsi
void nakesMenu(Equipment **list, int count);
void inputPatientInformation(Equipment **list, int count);
void checkEquipmentStatus(Equipment **list, int count, int isEngineer);
// viewEquipment sudah di shared_functions.h

#endif // NAKES_FUNCTIONS_H
