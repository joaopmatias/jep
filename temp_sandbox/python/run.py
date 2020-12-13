


from examples import run

import threading
from queue import Queue
q = Queue()
inception = threading.Thread(target=run, args=(q,))
inception.start()
inception.join()
print(q.get())

print("Python threading finished")

#run(q)
#print(q.get())
