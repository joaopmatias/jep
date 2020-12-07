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
void *on_thread_3(void *vargp){
    PyEval_AcquireThread(main_state);
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    // PyRun_SimpleString("import subprocess; subprocess.run(['python', '-c', 'import run']);");
    PyRun_SimpleString("import t");
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void *on_thread_shared(void *vargp){
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    PyRun_SimpleString("print('In a shared interpreter')\n");
    PyRun_SimpleString("import t");
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);
    return NULL;
}

int main() {
    Py_Initialize();
    PyEval_InitThreads();
    main_state = PyThreadState_Get();
    PyRun_SimpleString("import python.t");
    Py_BEGIN_ALLOW_THREADS

    pthread_t thread_id;
    // pthread_create(&thread_id, NULL, on_thread_1, NULL);
    // pthread_create(&thread_id, NULL, on_thread_2, NULL);
    // pthread_create(&thread_id, NULL, on_thread_3, NULL);
    // pthread_create(&thread_id, NULL, on_thread_shared, NULL);
    pthread_join(thread_id, NULL);

    Py_END_ALLOW_THREADS
    // PyRun_SimpleString("import run");
    Py_Finalize();

    exit(0);
}
