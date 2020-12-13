
These scripts recreate the execution of simple code snippets displaying different behaviors depending on the python executable and environments used. Typically, the results of C file match those of the Jep script.

The shell script file includes some scripts to easily run the examples. Bare in mind that they were written in MacOS with Python installed through Conda, thus may require tweaking to work in other systems.

Dependencies used:
* MacOS Catalina
* java adoptopenjdk 11.0.9
* conda 4.8.3
* python 3.7.7
* scipy 1.5.0
* numpy 1.18.5

Most of the examples were extracted from scipy tests that can be executed as follows
```
$ python
>>> import scipy
>>> scipy.test()
```

The results of the examples were affected by whether scipy was compiled in the machine or installed through binaries
```
pip install --no-binary :all: scipy==1.5.0
```
and were affected by wether `openblas` was installed through conda before compiling
```
conda install libopenblas libgfortran nomlk
```

### Updates:

* December 13: Added a java file calling the code in `run.py` through jep.