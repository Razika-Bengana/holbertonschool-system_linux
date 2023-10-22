#include "laps.h"

/**
 * add_car - program that adds a car to the linked list of cars or updates its lap count
 *
 * @head: a pointer to the head of the linked list of cars
 * @id: the ID of the car to add or update
 *
 * this function checks if a car with the given ID exists in the list;
 * if it does, it increments the lap count of the car;
 * if it doesn't, a new car node is created and added in a sorted position in the list
 * according to the car ID;
 *
 * Return: nothing (void)
 */

void add_car(Car **head, int id)
{
    Car *node, *temp;

    if (!*head || id < (*head)->id)
    {
        node = malloc(sizeof(Car));
        if (!node)
            exit(EXIT_FAILURE);
        node->id = id;
        node->laps = 0;
        node->next = *head;
        *head = node;
        printf("Car %d joined the race\n", id);
        return;
    }
    for (node = *head; node->next && node->next->id <= id; node = node->next)
        ;
    if (node->id == id)
    {
        node->laps++;
        return;
    }
    temp = malloc(sizeof(Car));
    if (!temp)
        exit(EXIT_FAILURE);
    temp->id = id;
    temp->laps = 0;
    temp->next = node->next;
    node->next = temp;
    printf("Car %d joined the race\n", id);
}

/**
 * race_state - program that maintains the state of the race, adding cars or freeing all cars
 *
 * @id: an rray of car IDs that participated in the current lap
 * @size: the size of the ID array
 *
 * this function iterates over the array of car IDs and adds each one
 * to the linked list of cars, if it's not already there, or updates
 * its lap count if it is;
 * if the size parameter is zero, it frees the entire list of cars;
 *
 * Return: nothing (void)
 */

void race_state(int *id, size_t size)
{
    static Car *head;
    Car *node;
    size_t i;

    if (!size)
    {
        while (head)
        {
            node = head;
            head = head->next;
            free(node);
        }
        return;
    }
    for (i = 0; i < size; i++)
    {
        add_car(&head, id[i]);
    }

    printf("Race state:\n");

    for (node = head; node; node = node->next)
    {
        printf("Car %d [%lu laps]\n", node->id, node->laps);
    }
}