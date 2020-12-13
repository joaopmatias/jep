#!/bin/sh

PYTHONHOME=/opt/miniconda3
py_version=python3.7
py_app=${py_version}m
jep_path=${PYTHONHOME}/lib/${py_version}/site-packages/jep
jep_cp=${jep_path}/jep-3.9.1
modules_dir=${PWD}/python

(cd python && ${PYTHONHOME}/bin/python setup.py build_ext --inplace)

echo "\n   C"

gcc -g c_run.c -o c_run -I${PYTHONHOME}/include/${py_app}/ -L${PYTHONHOME}/lib/ -l${py_app} -pthread

LD_LIBRARY_PATH=${PYTHONHOME}/lib/ PYTHONFAULTHANDLER=1 PYTHONPATH=${modules_dir} ./c_run

echo "\n   Python"

(cd python && PYTHONFAULTHANDLER=1 PYTHONPATH=${modules_dir} ${PYTHONHOME}/bin/python run.py)

echo "\n   Jep"

(cd python && PYTHONFAULTHANDLER=1 PYTHONPATH=${modules_dir} ${PYTHONHOME}/bin/jep run.py)

echo "\n   Java"

(cd java  && javac -d . -cp "${jep_path}/*" play/run.java)

(cd java && PYTHONFAULTHANDLER=1 PYTHONPATH=${modules_dir} java -cp "${jep_path}/*:." -Djava.library.path="${jep_path}" play.run)


# Clean
# rm -r python/__pycache__ python/build
# rm python/*.so python/*.c c_run
# rm python/*.log java/*.log

