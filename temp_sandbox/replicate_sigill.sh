#!/bin/sh

PYTHONHOME=/opt/miniconda3

$PYTHONHOME/bin/python setup.py build_ext --inplace

gcc -o c_meh -I$PYTHONHOME/include/python3.7m/ -L$PYTHONHOME/lib/ -lpython3.7m -lm c_meh.c -framework Python

LD_LIBRARY_PATH=$PYTHONHOME/lib/ PYTHONPATH=. ./c_meh

$PYTHONHOME/bin/python run.py

$PYTHONHOME/bin/jep run.py
