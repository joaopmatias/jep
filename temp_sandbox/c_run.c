#include "Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static PyThreadState* main_state;
static PyThread_type_lock thread_done = NULL;
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
    // PyObject* t;
    // t = PyImport_ImportModule("sys");
    // PyRun_SimpleString("import sys");
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void on_thread_3(void *vargp){
    PyEval_AcquireThread(main_state);
    PyRun_SimpleString("print(threading.current_thread())");
    PyThreadState* state = Py_NewInterpreter();
    PyRun_SimpleString("print('In a sub interpreter')\n");
    PyRun_SimpleString("import threading;print(threading.current_thread())");
    // PyObject* t;
    // t = PyImport_ImportModule("sys");
    // PyRun_SimpleString("import sys");
    PyObject* t, *v, *u;
    //t = PyImport_ImportModule("run");
    t = PyImport_ImportModule("run");
    v = PyObject_GetAttrString(t, "run");
    // PyRun_SimpleString("import threading; threading.current_thread().join()");
    u = PyObject_CallObject(v, NULL);
    PyRun_SimpleString("print('lalalala', flush=True)");
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    // return NULL;
}

void *on_thread_shared(void *vargp){
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    PyRun_SimpleString("print('In a shared interpreter')\n");
    // PyObject* t;
    // t = PyImport_ImportModule("sys");
    // PyRun_SimpleString("import sys");
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);
    return NULL;
}

void on_thread_shared2(void *vargp){
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    PyRun_SimpleString("print('In a shared interpreter')\n");
    PyObject* t, *v, *u;
    //t = PyImport_ImportModule("run");
    // PyRun_SimpleString("import threading; threading.current_thread().join()");
    PyThreadState_Swap(NULL);
    PyThreadState* sstate = Py_NewInterpreter();
    thread_done = PyThread_allocate_lock();
    t = PyImport_ImportModule("run");
    v = PyObject_GetAttrString(t, "run");
    u = PyObject_CallObject(v, (void*)NULL);
    PyThread_release_lock(thread_done);
    Py_EndInterpreter(sstate);
    PyThreadState_Swap(state);
    // PyThread_start_new_thread(v, NULL);
    // PyRun_SimpleString("import run");
    // PyRun_SimpleString("print(sys.modules)");
    PyThreadState_Clear(state);
    // PyThreadState_DeleteCurrent();
    PyThreadState_Delete(state);
    PyEval_ReleaseThread(state);
    //return NULL;
}

typedef struct {
    void (*func) (void *);
    void *arg;
} pythread_callback;

static void *
pythread_wrapper(void *arg)
{
    /* copy func and func_arg and free the temporary structure */
    pythread_callback *callback = arg;
    void (*func)(void *) = callback->func;
    void *func_arg = callback->arg;
    PyMem_RawFree(arg);

    func(func_arg);
    return NULL;
}

int main() {
    Py_Initialize();
    PyObject *a, *b, *c, *d, *e, *f, *g;
    PyObject* threadingModule = NULL;
    PyObject* lockCreator     = NULL;
    PyObject* r     = NULL;
    threadingModule = PyImport_ImportModule("threading");
    lockCreator = PyObject_GetAttrString(threadingModule, "Lock");
    Py_DECREF(threadingModule);
    Py_DECREF(lockCreator);

    // r = PyImport_ImportModule("run");
    main_state = PyThreadState_Get();
    pythread_callback *callback = PyMem_RawMalloc(sizeof(pythread_callback));
    callback->func = &on_thread_shared2;
    callback->arg = (void*)NULL;

    // thread_done = PyThread_allocate_lock();
    PyRun_SimpleString("import threading");
    PyRun_SimpleString("print(threading.current_thread())");

    Py_BEGIN_ALLOW_THREADS

    pthread_t thread_id, thread_id2, thread_id3, thread_id4;
    // pthread_create(&thread_id, NULL, on_thread_1, NULL);
    // pthread_create(&thread_id, NULL, on_thread_2, NULL);
    // pthread_create(&thread_id3, (pthread_attr_t*)NULL, on_thread_3, callback);
    // a = PyImport_ImportModule("examples");
    // b = PyObject_GetAttrString(a, "eg1");
    // PyThread_start_new_thread(b, NULL);
    // PyThread_acquire_lock(thread_done, 1);
    // pthread_join(thread_id3, NULL);
    // on_thread_3((void *)NULL);
    // pthread_create(&thread_id4, (pthread_attr_t*)NULL, on_thread_4, callback);
    // pthread_join(thread_id4, NULL);
    // pthread_create(&thread_id, (pthread_attr_t*)NULL, on_thread_shared, callback);
    // pthread_join(thread_id, NULL);
    pthread_create(&thread_id2, (pthread_attr_t*)NULL, pythread_wrapper, callback);
    // pthread_detach(thread_id2);
    pthread_join(thread_id2, NULL);
    // PyThread_acquire_lock(thread_done, 1);

    Py_END_ALLOW_THREADS

    // PyThread_release_lock(thread_done);

    // PyThread_free_lock(thread_done);

    Py_Finalize();

    exit(0);
}
