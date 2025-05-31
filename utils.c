#include "utils.h"
#include <string.h> // Untuk strftime

// Get Current DateTime String
char* getCurrentDateTime(char *buffer, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}
