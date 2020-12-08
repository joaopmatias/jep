#include "Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static PyThreadState* main_state;

/*
 * This is how it is done in jep 3.3
 * Works in python 2.7
 * Broke in python 3.x, before python 3.5
 * ndjensen changed jep in 5d4b51ec
 */
void *on_thread_1(void *vargp){
    PyEval_AcquireLock();
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    Py_EndInterpreter(state);
    PyEval_ReleaseLock();
    return NULL; 
} 
   
/*
 * This is how it is done in jep 3.4 - 3.9.0
 * Works in every python version through 3.9a6
 * Broke in python 3.9b1 after 0b1e3307
 */
void *on_thread_2(void *vargp){
    PyEval_AcquireThread(main_state);
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    Py_EndInterpreter(state);
    PyEval_ReleaseLock();
    return NULL; 
} 

/*
 * Is this how we must do it in python 3.9?
 */
void *on_thread_4(void *vargp){
    PyEval_AcquireThread(main_state);
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    // PyRun_SimpleString("import t");
    PyObject* t;
    t = PyImport_ImportModule("sys");
    PyRun_SimpleString("import sys");
    // PyRun_SimpleString("print(sys.path)");
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void *on_thread_3(void *vargp){
    PyEval_AcquireThread(main_state);
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    // PyRun_SimpleString("import t");
    PyObject* t;
    t = PyImport_ImportModule("sys");
    PyRun_SimpleString("import sys");
    // PyRun_SimpleString("print(sys.path)");
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void *on_thread_shared(void *vargp){
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    PyRun_SimpleString("print('In a shared interpreter')\n");
    PyObject* t;
    t = PyImport_ImportModule("sys");
    PyRun_SimpleString("import examples");
    // PyRun_SimpleString("print(sys.path)");
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);
    return NULL;
}

void *on_thread_shared2(void *vargp){
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    PyRun_SimpleString("print('In a shared interpreter')\n");
    PyObject* t;
    t = PyImport_ImportModule("sys");
    PyRun_SimpleString("import t");
    PyRun_SimpleString("print(sys.modules)");
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);
    return NULL;
}

int main() {
    return 0;
}
