#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - program that prints information about a Python list
 *
 * this function prints various pieces of information about the Python list,
 * including its size, the number of allocated slots for elements,
 * and the type names of each element in the list
 *
 * @p: a pointer to the PyObject representing a Python list
 *
 * Return: nothing (void)
 *
 */

void print_python_list(PyObject *p)
{
    int i;

/* Print header information */
    printf("[*] Python list info\n");

/* Print the size of the Python list */
    printf("[*] Size of the Python List = %lu\n", Py_SIZE(p));

/*
 * Print the allocated size of the Python list.
 * Allocated represents the number of slots allocated for elements in the
 * list, which can be greater than the actual number of elements in the list
 */
    printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);

/* Iterate through the list and print information about each element */
    for (i = 0; i < Py_SIZE(p); i++)
    {
    /*
     * Get the type name of the current element
     * Note: PyList_GetItem(p, i) retrieves the item at the given index 'i'
     * and Py_TYPE() returns a pointer to the type object of the item
     */
        printf("Element %d: %s\n", i, Py_TYPE(PyList_GetItem(p, i))->tp_name);
    }
}