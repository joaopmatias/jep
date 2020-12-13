
print("\n*** run.py started ***")

# this function also has the option to skip some examples and/or throw and exception
def run(q, skip_examples=(), throw_exception=True):
    import threading
    from examples import eg1, eg2, eg3, eg4, eg5, eg6, eg7, eg8
    s_current = str(threading.current_thread())
    s_main = str(threading.main_thread())
    q.put(s_current)
    q.put(s_main)
    fcn_list = (eg1, eg2, eg3, eg4, eg5, eg6, eg7, eg8)
    for i, f in enumerate(fcn_list, start=1):
        if not i in skip_examples:
            f()
    if throw_exception:
        raise Exception("Originated in " + s_current + " with main " + s_main)


import threading
from queue import Queue
q = Queue()
skip_examples = (7,)
throw_exception = False
inception = threading.Thread(target=run, args=(q, skip_examples, throw_exception))
inception.start()
inception.join()
print("\nMessage sent from: " + q.get())
print("Main thread is: " + q.get())

print("\n* Python threading finished *")

run(q, skip_examples, throw_exception)
print("\nMessage sent from: " + q.get())
print("Main thread is: " + q.get())

print("\n*** run.py finished ***")
