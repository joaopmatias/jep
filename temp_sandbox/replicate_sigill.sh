#!/bin/sh

PYTHONHOME=/opt/miniconda3
py_version=python3.7
py_app=${py_version}m
jep_path=${PYTHONHOME}/lib/${py_version}/site-packages/jep
jep_cp=${jep_path}/jep-3.9.1
python_subfolder=${PWD}/src/main/python
java_subfolder=${PWD}/src/main/java
c_subfolder=${PWD}/src/main/c

(cd ${python_subfolder} && ${PYTHONHOME}/bin/python setup.py build_ext --inplace)

echo "\n   C"

gcc -pthread -g ${c_subfolder}/c_run.c -o c_run -I${PYTHONHOME}/include/${py_app}/ -L${PYTHONHOME}/lib/ -l${py_app}

LD_LIBRARY_PATH=${PYTHONHOME}/lib/ PYTHONFAULTHANDLER=1 PYTHONPATH=${python_subfolder} ./c_run

echo "\n   Python"

(cd ${python_subfolder} && PYTHONFAULTHANDLER=1 PYTHONPATH=${python_subfolder} ${PYTHONHOME}/bin/python run.py)

echo "\n   Jep"

(cd ${python_subfolder} && PYTHONFAULTHANDLER=1 PYTHONPATH=${python_subfolder} ${PYTHONHOME}/bin/jep run.py)

echo "\n   Java"

(cd ${java_subfolder} && javac -d . -cp "${jep_path}/*" play/run.java)

(cd ${java_subfolder} && PYTHONFAULTHANDLER=1 PYTHONPATH=${python_subfolder} java -Xss4m -cp "${jep_path}/*:." -Djava.library.path="${jep_path}" play.run)


# Clean
# rm -r ${python_subfolder}/__pycache__ ${python_subfolder}/build c_run.dSYM
# rm ${python_subfolder}/*.so python/*.c c_run
# rm ${python_subfolder}/*.log ../*.log ${java_subfolder}/*.log

