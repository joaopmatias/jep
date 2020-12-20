
print("\n*** run.py started ***")

# this function also has the option to skip some examples and/or throw and exception
def run():
    skip_examples = (1,3,4,5,6,7,8,9)
    throw_exception = False
    import threading
    from examples import eg1, eg2, eg3, eg4, eg5, eg6, eg7, eg8, eg9
    s_current = str(threading.current_thread())
    s_main = str(threading.main_thread())

    print("Originated in " + s_current + " with main " + s_main)
    fcn_list = (eg1, eg2, eg3, eg4, eg5, eg6, eg7, eg8, eg9)
    for i, f in enumerate(fcn_list, start=1):
        if not i in skip_examples:
            f()

def hi():
    print("hello")
# import threading
# import _thread
# from queue import Queue
# q = Queue()
# skip_examples = (1,2,3,6,7,8,9)
# throw_exception = False
# _thread.start_new_thread(run, ())
# inception = threading.Thread(target=run, args=())
# inception.start()
# inception.join()
# inception._stop()
# print("\nMessage sent from: " + q.get())
# print("Main thread is: " + q.get())

print("\n* Python threading finished *")

# run(q, skip_examples, throw_exception)
# print("\nMessage sent from: " + q.get())
# print("Main thread is: " + q.get())

print("\n*** run.py finished ***")
