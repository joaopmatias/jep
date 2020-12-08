#!/bin/sh

PYTHONHOME=/opt/miniconda3
py_version=python3.7
py_app=${py_version}m
jep_path=${PYTHONHOME}/lib/${py_version}/site-packages/jep
jep_cp=${jep_path}/jep-3.9.1

(cd python && ${PYTHONHOME}/bin/python setup.py build_ext --inplace)

gcc -g -o c_run -I${PYTHONHOME}/include/${py_app}/ -L${PYTHONHOME}/lib/ -l${py_app} -lm c_run.c

PYTHONFAULTHANDLER=1 LD_LIBRARY_PATH=${PYTHONHOME}/lib ./c_run

# (cd python && ${PYTHONHOME}/bin/python t.py)

(cd python && ${PYTHONHOME}/bin/jep run.py)

(cd java  && javac -d . -cp "${jep_path}/*" play/run.java)

(cd java && PYTHONFAULTHANDLER=1 PYTHONPATH=../python java -cp "${jep_path}/*:." play.run)


# Clean
# rm -r python/__pycache__ python/build
# rm python/*.so python/*.c c_run

