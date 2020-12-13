
print("\n*** run_check_threads.py started ***")

# this function also has the option to skip some examples and/or throw and exception
def run(q, throw_exception=True):
    import threading
    s_current = str(threading.current_thread())
    s_main = str(threading.main_thread())
    q.put(s_current)
    q.put(s_main)
    if throw_exception:
        raise Exception("Originated in " + s_current + " with main " + s_main)


import threading
from queue import Queue
q = Queue()
throw_exception = False
inception = threading.Thread(target=run, args=(q, throw_exception))
inception.start()
inception.join()
print("\nMessage sent from: " + q.get())
print("Main thread is: " + q.get())

print("\n* Python threading finished *")

run(q, throw_exception)
print("\nMessage sent from: " + q.get())
print("Main thread is: " + q.get())

print("\n*** run_check_threads.py finished ***")
