#include <Python.h>
#include <stdio.h>

/**
 * print_python_bytes - program that prints information about Python bytes
 * object
 *
 * line “first X bytes”: we have to print a maximum of 10 bytes;
 * if p is not a valid PyBytesObject, we have to print an error message
 *
 * @p: PyObject pointer representing the Python bytes object
 *
 * Return: nothing (void)
 */

void print_python_bytes(PyObject *p)
{
    int i;
    Py_ssize_t size, printed_bytes;
    char *array_as_string = NULL;

    printf("[.] bytes object info\n");
/* Check if the given object is a valid Python bytes object */
    if (!PyBytes_Check(p))
    {
        printf("  [ERROR] Invalid Bytes Object\n");
        return;
    }
/* Get the size of the bytes object */
    size = ((PyVarObject *)(p))->ob_size;
    printf("  size: %li\n", size);
/* Convert the bytes object to a string */
    array_as_string = ((PyBytesObject *)(p))->ob_sval;
    printf("  trying string: %s\n", array_as_string);
/* Determine how many bytes to print, up to 10 bytes */
    printed_bytes = (size + 1 >= 10) ? 10 : size + 1;
    printf("  first %li bytes:", printed_bytes);

    for (i = 0; i < printed_bytes; i++)
        printf(" %02x", (unsigned char)(array_as_string[i]));
    putchar('\n');
}


/**
 * print_python_list - program that prints information about Python list object
 *
 * @p: PyObject pointer representing the Python list object
 *
 * Return: nothing (void)
 */

void print_python_list(PyObject *p)
{
    int i;
    Py_ssize_t size;
    PyObject *list_member;

    printf("[*] Python list info\n");
/* Check if the given object is a valid Python list object */
    if (!PyList_Check(p))
    {
        printf("  [ERROR] Invalid List Object\n");
        return;
    }
/* Get the size of the list */
    size = ((PyVarObject *)(p))->ob_size;
    printf("[*] Size of the Python List = %li\n", size);
/* Print the number of allocated elements in the list */
    printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);
/* Iterate through the list elements and print their types */
    for (i = 0; i < size; i++)
    {
        list_member = ((PyListObject *)p)->ob_item[i];
        printf("Element %d: %s\n", i,
               list_member->ob_type->tp_name);
/* If the list element is a bytes object, print its details */
        if (PyBytes_Check(list_member))
            print_python_bytes(list_member);
    }
}
