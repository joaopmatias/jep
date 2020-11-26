
# compile using $ python setup.py build_ext --inplace

from Cython.Build import cythonize
from setuptools import Extension, setup
import numpy as np

extension = Extension("py_meh", ["py_meh.pyx"],
                      include_dirs=[np.get_include()], depends=[],
                      libraries=[], library_dirs=[])

setup(ext_modules=cythonize(extension, compiler_directives={'language_level': "3"}))
