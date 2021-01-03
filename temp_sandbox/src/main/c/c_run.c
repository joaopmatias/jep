#include "Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;
static PyThreadState* main_state;
static PyThread_type_lock thread_done = NULL;

void *start_py(void* argv)
{
    Py_Initialize();
    PyObject *a, *b, *c, *d, *e, *f, *g;
    PyObject* threadingModule = NULL;
    PyObject* lockCreator     = NULL;
    PyObject* r     = NULL;
    threadingModule = PyImport_ImportModule("threading");
    lockCreator = PyObject_GetAttrString(threadingModule, "Lock");
    Py_DECREF(threadingModule);
    Py_DECREF(lockCreator);
    main_state = PyThreadState_Get();
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void *end_py(void* argv)
{
    PyEval_AcquireThread(main_state);
    Py_Finalize();
    return NULL;
}

typedef struct {
    PyThread_type_lock start_event;
    PyThread_type_lock exit_event;
    PyObject *callback;
} test_c_thread_t;

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
    Py_EndInterpreter(state);
    PyThreadState_Swap(main_state);
    PyEval_ReleaseThread(main_state);
    return NULL;
}

void *on_thread_3(void *vargp){
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
    return NULL;
}

/*
 * SharedInterpreter
 */

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

void *on_thread_shared2(void *vargp){
    start_py((void*)NULL);
    // PyEval_AcquireLock();
    // PyThreadState_Swap(main_state);
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    // PyGILState_STATE gstate;
    // gstate = PyGILState_Ensure();
    PyRun_SimpleString("print('In a shared interpreter')\n");
    PyObject* t, *v, *u;
    //t = PyImport_ImportModule("run");
    // PyRun_SimpleString("import threading; threading.current_thread().join()");
    // PyThreadState_Swap(NULL);
    // PyThreadState* sstate = Py_NewInterpreter();
    // thread_done = PyThread_allocate_lock();
    t = PyImport_ImportModule("run");
    v = PyObject_GetAttrString(t, "run");
    // u = PyObject_CallObject(v, NULL);
    // PyThread_release_lock(thread_done);
    // Py_EndInterpreter(sstate);
    // PyThreadState_Swap(state);
    // PyThread_start_new_thread(v, NULL);
    // PyRun_SimpleString("import run");
    // PyRun_SimpleString("print(sys.modules)");
    // PyGILState_Release(gstate);
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);
    // PyThreadState_DeleteCurrent();
    // PyThreadState_Swap(NULL);
    // PyEval_ReleaseLock();
    end_py((void*)NULL);
    return NULL;
}


void *on_thread_shared3(void *vargp){
    pthread_mutex_lock( &count_mutex );
    printf("adasdas");
    // PyThread_type_lock lock = *((PyThread_type_lock*)vargp);
fflush(stdout);
    PyThreadState* state = PyThreadState_New(main_state->interp);
    PyEval_AcquireThread(state);
    // PyGILState_STATE state = PyGILState_Ensure();
    // Py_Initialize();
    // PyObject *a, *b, *c, *d, *e, *f, *g;
    // PyObject* threadingModule = NULL;
    // PyObject* lockCreator     = NULL;
    // PyObject* r     = NULL;
    // threadingModule = PyImport_ImportModule("threading");
    // lockCreator = PyObject_GetAttrString(threadingModule, "Lock");
    // Py_DECREF(threadingModule);
    // Py_DECREF(lockCreator);
    PyRun_SimpleString("print('In a shared interpreter', flush=True)\n");
    PyObject* t, *v, *u;
    fflush(stdout);
    //t = PyImport_ImportModule("run");
    // PyRun_SimpleString("import threading; threading.current_thread().join()");
    // PyThreadState_Swap(NULL);
    // PyThreadState* sstate = Py_NewInterpreter();
    // thread_done = PyThread_allocate_lock();
    t = PyImport_ImportModule("run");
    v = PyObject_GetAttrString(t, "run");
    u = PyObject_CallObject(v, NULL);
    printf("adasdas");
    // Py_Finalize();
    // return NULL;
    pthread_cond_signal( &condition_var );


    // PyGILState_Release(state);
    PyThreadState_Clear(state);
    PyEval_ReleaseThread(state);
    PyThreadState_Delete(state);

    // PyThread_release_lock(lock);

 pthread_mutex_unlock( &count_mutex );
    // PyThread_exit_thread();
    return NULL;
}


static void on_thread_shared4(void *vargp){
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, on_thread_shared3, NULL);
    pthread_join(thread_id, NULL);
    // on_thread_shared3((void*)NULL);
    // return NULL;
}

static void
temporary_c_thread(void *data)
{
    test_c_thread_t *test_c_thread = data;
    PyGILState_STATE state;
    PyObject *res;

    PyThread_release_lock(test_c_thread->start_event);

    /* Allocate a Python thread state for this thread */
    state = PyGILState_Ensure();

    res = _PyObject_CallNoArg(test_c_thread->callback);
    Py_CLEAR(test_c_thread->callback);

    if (res == NULL) {
        PyErr_Print();
    }
    else {
        Py_DECREF(res);
    }

    /* Destroy the Python thread state for this thread */
    PyGILState_Release(state);

    PyThread_release_lock(test_c_thread->exit_event);

    PyThread_exit_thread();
}

static PyObject *
call_in_temporary_c_thread(PyObject *self, PyObject *callback)
{
    PyObject *res = NULL;
    test_c_thread_t test_c_thread;
    long thread;

    test_c_thread.start_event = PyThread_allocate_lock();
    test_c_thread.exit_event = PyThread_allocate_lock();
    test_c_thread.callback = NULL;


    PyThread_acquire_lock(test_c_thread.start_event, 1);
    PyThread_acquire_lock(test_c_thread.exit_event, 1);

    thread = PyThread_start_new_thread(temporary_c_thread, &test_c_thread);


    PyThread_acquire_lock(test_c_thread.start_event, 1);
    PyThread_release_lock(test_c_thread.start_event);

    Py_BEGIN_ALLOW_THREADS
        PyThread_acquire_lock(test_c_thread.exit_event, 1);
        PyThread_release_lock(test_c_thread.exit_event);
    Py_END_ALLOW_THREADS




    Py_CLEAR(test_c_thread.callback);
    if (test_c_thread.start_event)
        PyThread_free_lock(test_c_thread.start_event);
    if (test_c_thread.exit_event)
        PyThread_free_lock(test_c_thread.exit_event);
    return NULL;
}




#define THREAD_STACK_SIZE       0x1000000
#define PTHREAD_SYSTEM_SCHED_SUPPORTED 1
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

unsigned long
my_PyThread_start_new_thread(void (*func)(void *), void *arg)
{
    pthread_t th;
    int status;
#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
    pthread_attr_t attrs;
#endif
#if defined(THREAD_STACK_SIZE)
    size_t      tss;
#endif

    PyThread_init_thread();

#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
    if (pthread_attr_init(&attrs) != 0)
        return PYTHREAD_INVALID_THREAD_ID;
#endif
#if defined(THREAD_STACK_SIZE)
    PyThreadState *tstate = main_state;
    size_t stacksize = tstate ? tstate->interp->pythread_stacksize : 0;
    tss = (stacksize != 0) ? stacksize : THREAD_STACK_SIZE;
    if (tss != 0) {
        if (pthread_attr_setstacksize(&attrs, tss) != 0) {
            pthread_attr_destroy(&attrs);
            return PYTHREAD_INVALID_THREAD_ID;
        }
    }
#endif
#if defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
    pthread_attr_setscope(&attrs, PTHREAD_SCOPE_SYSTEM);
#endif

    pythread_callback *callback = PyMem_RawMalloc(sizeof(pythread_callback));

    if (callback == NULL) {
      return PYTHREAD_INVALID_THREAD_ID;
    }

    callback->func = func;
    callback->arg = arg;

    status = pthread_create(&th,
#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
                             &attrs,
#else
                             (pthread_attr_t*)NULL,
#endif
                             pythread_wrapper, callback);

#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
    pthread_attr_destroy(&attrs);
#endif

    if (status != 0) {
        PyMem_RawFree(callback);
        return PYTHREAD_INVALID_THREAD_ID;
    }

    pthread_join(th);
    pthread_detach(th);

#if SIZEOF_PTHREAD_T <= SIZEOF_LONG
    return (unsigned long) th;
#else
    return (unsigned long) *(unsigned long *) &th;
#endif
}












int main() {
    pthread_t thread_id, thread_id2, thread_id3, thread_id4;


    // PyThread_type_lock lock = PyThread_allocate_lock();

    Py_Initialize();
    PyObject *a, *b, *c, *d, *e, *f, *g;
    PyObject* threadingModule = NULL;
    PyObject* lockCreator     = NULL;
    PyObject* r     = NULL;
    threadingModule = PyImport_ImportModule("threading");
    lockCreator = PyObject_GetAttrString(threadingModule, "Lock");
    Py_DECREF(threadingModule);
    Py_DECREF(lockCreator);
    main_state = PyThreadState_Get();
    

    PyRun_SimpleString("print(123, flush=True)");

    Py_BEGIN_ALLOW_THREADS
    my_PyThread_start_new_thread(&on_thread_shared3, (void*)NULL);
    // on_thread_shared3((void*)NULL);
    printf("aiaiaa");
    fflush(stdout);
    // pthread_create(&thread_id2, NULL, on_thread_shared4, NULL);
    // // pthread_create(&thread_id, (pthread_attr_t*)NULL, pythread_wrapper, callback);
    // // pthread_detach(thread_id2);
    pthread_cond_wait( &condition_var , &count_mutex);
    // pthread_detach(thread_id2);
    // // PyThread_start_new_thread(on_thread_shared3, &lock);
    // pthread_detach(thread_id2);
    fflush(stdout);

    Py_END_ALLOW_THREADS



    // Py_Finalize();

    exit(0);
}
