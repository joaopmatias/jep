
These scripts recreate the execution of a simple command showing different behaviors in cpython and jep.

The shell script file includes some scripts to help replicate the issue. Bare in mind that they were written in MacOS with Python installed through Conda, thus may require tweaking to work in other systems.

Dependencies used:
* MacOS Catalina
* java adoptopenjdk 11.0.9
* conda 4.8.3
* python 3.7.7
* scipy 1.5.0
* numpy 1.18.5

The issue occurs when the custom function "meh" is called.

The commands do not display problems except for when "meh(10)" is called through jep.
