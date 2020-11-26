#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

int main()
{
    PyObject* pGlobal;

    Py_Initialize();
    pGlobal = PyModule_GetDict(PyImport_AddModule("__main__"));
    PyRun_String("from py_meh import meh; meh(9); meh(10)", Py_file_input, pGlobal, pGlobal);

    PyRun_SimpleString("from py_meh import meh; meh(9); meh(10)");
    Py_Finalize();

    return 0;
}