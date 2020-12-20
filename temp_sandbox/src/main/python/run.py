
print("\n*** run.py started ***")

# this function also has the option to skip some examples and/or throw and exception
def run():
    skip_examples = (6,7,8,9)
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

import threading
inception = threading.Thread(target=run, args=())
inception.start()
inception.join()

print("\n* Python threading finished *")

run()

print("\n*** run.py finished ***")
