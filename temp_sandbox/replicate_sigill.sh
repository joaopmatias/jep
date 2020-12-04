#!/bin/sh

PYTHONHOME=/opt/miniconda3
py_app=python3.7m

${PYTHONHOME}/bin/python setup.py build_ext --inplace

gcc -o c_run -I${PYTHONHOME}/include/${py_app}/ -L${PYTHONHOME}/lib/ -l${py_app} -lm c_run.c

LD_LIBRARY_PATH=${PYTHONHOME}/lib/ PYTHONPATH=. ./c_run

${PYTHONHOME}/bin/python run.py

${PYTHONHOME}/bin/jep run.py
