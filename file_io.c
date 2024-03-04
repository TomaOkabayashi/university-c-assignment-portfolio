#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"

int readMapFile(const char* filename, int* map_rows, int* map_columns, char*** arrayBox) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    if (fscanf(file, "%d %d", map_rows, map_columns) != 2) {
        perror("Error reading map size from the file");
        fclose(file);
        return 1;
    }

    /* Add a border by extending the dimensions */
    int rows_with_border = *map_rows + 2;
    int columns_with_border = *map_columns + 2;

    /* Allocate memory for arrayBox with the border */
    *arrayBox = (char**)malloc(rows_with_border * sizeof(char*));
    if (*arrayBox == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < rows_with_border; i++) {
        (*arrayBox)[i] = (char*)malloc(columns_with_border * sizeof(char));
        if ((*arrayBox)[i] == NULL) {
            perror("Memory allocation failed");
            fclose(file);
            return 1;
        }
    }

    /* Initialise the border with "*" */
    for (int i = 0; i < rows_with_border; i++) {
        for (int j = 0; j < columns_with_border; j++) {
            if (i == 0 || i == rows_with_border - 1 || j == 0 || j == columns_with_border - 1) {
                (*arrayBox)[i][j] = '*'; /* Border */
            } else {
                (*arrayBox)[i][j] = ' '; /* Empty space within the border */
            }
        }
    }

    /* Read the map data and update the inner area */
    for (int i = 1; i < rows_with_border - 1; i++) {
        for (int j = 1; j < columns_with_border - 1; j++) {
            int value;
            if (fscanf(file, "%d", &value) != 1) {
                perror("Error reading data from the file");
                fclose(file);
                return 1;
            }
            switch (value) {
                case 0:
                    (*arrayBox)[i][j] = ' '; /* Empty space */
                    break;
                case 1:
                    (*arrayBox)[i][j] = '.'; /* Traffic path */
                    break;
                case 2:
                    (*arrayBox)[i][j] = '>'; /* Initial position of the car */
                    break;
                case 3:
                    (*arrayBox)[i][j] = 'P'; /* Position of the player */
                    break;
                case 4:
                    (*arrayBox)[i][j] = 'G'; /* Position of the goal */
                    break;
                default:
                    perror("Invalid value in the map file");
                    fclose(file);
                    return 1;
            }
        }
    }

    fclose(file);
    return 0;
}
