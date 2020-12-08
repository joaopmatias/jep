# import threading
# import subprocess
# import signal
import sys
# print(sys.path)
print("hoy")
from pathlib import Path
print(Path(Path(".").absolute().parent, "python"))
sys.path.append("/Users/joao.matias/Projects/jep/temp_sandbox/python")
# print(sys.path)
def sig_handler(signum, frame):
    pass
# signal.signal(signal.SIGILL, sig_handler)
# print(threading.current_thread(), flush=True)
# print(threading.main_thread())
# t=threading.main_thread()
from examples import eg1, eg2, eg3, eg4, eg5, eg6, eg7, eg8
try:
    eg1()
    eg2()
    eg3()
    eg4()
    eg5()
    eg6()
    #eg7()
    eg8()
except Exception as e:
    print(e)
