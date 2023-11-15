#include <Python.h>
#include <stdio.h>
#include <stdbool.h>

#define _ABS(x) ((x) < 0 ? -(x) : (x))

/**
 * print_python_int - program that prints Python integers
 *
 *
 * if p is not a valid string, we have to print an error message;
 * the function should first translate the Python int to a C unsigned long int
 * and then use printf to print the number;
 * if the absolute value of the number is too big to be stored in
 * a C unsigned long int, print C unsigned long int overflow,
 * followed by a new line
 *
 * @p: PyObject pointer representing the Python int object
 *
 * Return: nothing (void)
 */

void print_python_int(PyObject *p)
{
    Py_ssize_t i, size;
    bool negative;
    unsigned long base10 = 0, pow_base_pylong;
    unsigned int base_pylong = 1 << PyLong_SHIFT;
/* Check if the given object is a valid Python long (int) object */
    if (!PyLong_Check(p))
    {
        puts("Invalid Int Object");
        return;
    }
/* Get the size of the Python long object and check if it's negative */
    size = _ABS(((PyVarObject *)p)->ob_size);
    negative = ((PyVarObject *)p)->ob_size < 0;
/*
 * If the Python long object can be represented as a C unsigned long int,
 * calculate its decimal value and print it
 */
    if (size < 3 ||
        (size == 3 && ((PyLongObject *)p)->ob_digit[2] < 16))
    {
        pow_base_pylong = 1;
        for (i = 0; i < size; i++)
        {
            base10 += pow_base_pylong *
                      ((PyLongObject *)p)->ob_digit[i];
            pow_base_pylong *= base_pylong;
        }
        /* If the Python long object is negative, print the '-' sign */
        if (negative)
            putchar('-');
        printf("%lu\n", base10);
    }
    else
        puts("C unsigned long int overflow");
}