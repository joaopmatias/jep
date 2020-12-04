
# compile using $ python setup.py build_ext --inplace

from Cython.Build import cythonize
from setuptools import Extension, setup
import numpy
import os

extension = Extension("cython_examples", ["cython_examples.pyx"],
                      include_dirs=[os.path.join(numpy.__path__[0], 'core', 'include')], depends=[],
                      libraries=[], library_dirs=[])

setup(ext_modules=cythonize(extension, compiler_directives={'language_level': "3"}))
