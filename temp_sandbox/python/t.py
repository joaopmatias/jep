#import subprocess; subprocess.run(['python', '-c', 'import run']);
import threading
#
def f():
    import run

t = threading.Thread(target=f)
t.start()
t.join()
