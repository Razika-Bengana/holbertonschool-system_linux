#ifndef LAPS_H
#define LAPS_H

#include <stdlib.h>
#include <stdio.h>

/**
 * struct car - defines a new data type 'Car'
 * @id: the id of the car
 * @laps: the number of laps that the car has completed
 * @next: a pointer to the next car in the linked list
 */

typedef struct car
{
    int id;
    size_t laps;
    struct car *next;
} Car;

void add_car(Car **head, int id);
void race_state(int *id, size_t size);

#endif /* LAPS_H */
