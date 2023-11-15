#include <Python.h>
#include <stdio.h>
#include <limits.h>

/**
 * print_python_int - program that prints the integer value of a Python object
 *
 * this function checks if the provided PyObject is an instance of PyLongObject,
 * which represents Python's integer objects;
 * if the check passes, the function retrieves the integer value and prints it
 * to standard output;
 * if the PyObject is not a PyLongObject or if an error occurs during value
 * retrieval, it prints an appropriate error message to standard error
 *
 * @p: a PyObject pointer, expected to point to a PyLongObject
 *
 * Return: nothing (void)
 */

void print_python_int(PyObject *p)
{
    if (!PyLong_Check(p))
    {
        fprintf(stderr, "Invalid PyObject\n");
        return;
    }

    /* Retrieve the integer value from the PyLongObject;
     * PyLong_AsLongLong is used here because we want to handle long integers
     * which may not fit into a regular long on some systems
     */
    long long value = PyLong_AsLongLong(p);
    if (value == -1 && PyErr_Occurred())
    {
        PyErr_Print();
        return;
    }

    printf("%lld\n", value);
}
