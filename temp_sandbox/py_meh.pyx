cimport numpy as np
cimport cython
import numpy as np

np.import_array()

cimport scipy.linalg.cython_lapack as lapack

cdef FORTRAN = 1

cdef int zmeh(int n) except *:
    cdef np.npy_intp dim[2]
    cdef np.complex128_t [::1,:] capI
    cdef int [::1,:] ipiv
    cdef:
        int info

    # Initialize arrays
    dim[0] = n; dim[1] = n;
    capI = np.PyArray_ZEROS(2, dim, np.NPY_COMPLEX128, FORTRAN)
    ipiv = np.PyArray_ZEROS(2, dim, np.NPY_INT32, FORTRAN)

    # create matrix with two diagonals
    for i in range(n-1):
        capI[i,i] = capI[i,i] + 1
        capI[i,i+1] = capI[i,i+1] + 1

    lapack.zgetrf(&n, &n, &capI[0,0], &n, &ipiv[0,0], &info)
    print(np.asarray(capI))
    print(np.asarray(ipiv))
    print(info)
    return 0;

def meh(n):
    zmeh(n)