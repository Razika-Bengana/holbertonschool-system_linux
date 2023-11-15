#include <Python.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>


/**
 * print_python_string - program that prints Python strings
 *
 * if p is not a valid string, we have to print an error message
 *
 * @p: PyObject pointer representing the Python string object
 *
 * Return: nothing (void)
 */

void print_python_string(PyObject *p)
{
    wchar_t *wchar_buf = NULL;

    printf("[.] string object info\n");
/* Check if the given object is a valid Python string object and is ready */
    if (!PyUnicode_Check(p) || !((PyASCIIObject *)p)->state.ready)
    {
        printf("  [ERROR] Invalid String Object\n");
        return;
    }
/* Print whether the string is compact and ASCII or Unicode */
    printf("  type: %s%s\n",
           ((PyASCIIObject *)p)->state.compact ? "compact " : "",
           ((PyASCIIObject *)p)->state.ascii ? "ascii" : "unicode object");
/* Print the length of the string */
    printf("  length: %li\n",
           ((PyASCIIObject *)(p))->length);
/*
 * Convert the Python Unicode string to a wide character string (wchar_t *)
 * and print its value
 * Note: The wide character string is used to handle non-ASCII characters
 * correctly
 */
    wchar_buf = PyUnicode_AsWideCharString(p, NULL);
    if (wchar_buf != NULL)
    {
        printf("  value: %ls\n", wchar_buf);
/* Free the memory allocated for the wide character string */
        PyMem_Free(wchar_buf);
    }
}