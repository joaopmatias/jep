
def eg1(n=10):
    from cython_examples import py_helper
    py_helper(n)
    print('Example 1 finished', flush=True)


def eg2(n=20):
    import numpy as np
    from numpy.random import random
    from scipy.linalg import solve
    from numpy.testing import assert_array_almost_equal
    from numpy import dot
    #tests/test_basic.py::TestSolve::test_random_complex zsh: illegal hardware instruction  jep
    np.random.seed(1234)
    a = random([n, n]) + 1j * random([n, n])
    for i in range(n):
        a[i, i] = 20*(.1+a[i, i])
    for i in range(2):
        b = random([n, 3])
        x = solve(a, b)
        assert_array_almost_equal(dot(a, x), b)
    print('Example 2 finished', flush=True)


def eg3():
    # fails when using libopenblas in conda
    import pickle as pkl
    from pathlib import Path
    f = open(Path(__file__).parent.joinpath('resources/example3.pkl'), 'rb')
    d = pkl.load(f)
    f.close()

    from scipy.linalg._flapack import dgeqrf
    args = d['args']
    kwargs = d['kwargs']
    dgeqrf(*args, **kwargs)
    print('Example 3 finished', flush=True)


def eg4():
    #tests/test_basic.py::TestLstsq::test_random_overdet Fatal Python error: Segmentation fault
    from numpy.testing import assert_, assert_allclose
    from scipy.linalg.tests.test_basic import REAL_DTYPES, direct_lstsq, _eps_cast
    import numpy as np
    from numpy.random import random
    from scipy.linalg import lstsq
    for dtype in REAL_DTYPES:
        for (n, m) in ((20, 15), (200, 2)):
            for lapack_driver in ('gelsd', 'gelss', 'gelsy', None):
                for overwrite in (True, False):
                    a = np.asarray(random([n, m]), dtype=dtype)
                    for i in range(m):
                        a[i, i] = 20 * (0.1 + a[i, i])
                    for i in range(4):
                        b = np.asarray(random([n, 3]), dtype=dtype)
                        # Store values in case they are overwritten later
                        a1 = a.copy()
                        b1 = b.copy()
                        out = lstsq(a1, b1,
                                    lapack_driver=lapack_driver,
                                    overwrite_a=overwrite,
                                    overwrite_b=overwrite)
                        x = out[0]
                        r = out[2]
                        assert_(r == m, 'expected efficient rank %s, '
                                        'got %s' % (m, r))
                        assert_allclose(
                            x, direct_lstsq(a, b, cmplx=0),
                            rtol=25 * _eps_cast(a1.dtype),
                            atol=25 * _eps_cast(a1.dtype),
                            err_msg="driver: %s" % lapack_driver)
    print('Example 4 finished', flush=True)


def eg5():
    #tests/test_basic.py::TestLstsq::test_random_exact Fatal Python error: Segmentation fault
    from numpy.testing import assert_, assert_allclose
    from scipy.linalg.tests.test_basic import REAL_DTYPES, _eps_cast
    import numpy as np
    from numpy.random import random
    from scipy.linalg import lstsq
    from numpy import dot
    for dtype in REAL_DTYPES:
        for n in (20, 200):
            for lapack_driver in ('gelsd', 'gelss', 'gelsy', None):
                for overwrite in (True, False):
                    a = np.asarray(random([n, n]), dtype=dtype)
                    for i in range(n):
                        a[i, i] = 20 * (0.1 + a[i, i])
                    for i in range(4):
                        b = np.asarray(random([n, 3]), dtype=dtype)
                        # Store values in case they are overwritten later
                        a1 = a.copy()
                        b1 = b.copy()
                        out = lstsq(a1, b1,
                                    lapack_driver=lapack_driver,
                                    overwrite_a=overwrite,
                                    overwrite_b=overwrite)
                        x = out[0]
                        r = out[2]
                        assert_(r == n, 'expected efficient rank %s, '
                                        'got %s' % (n, r))
                        if dtype is np.float32:
                            assert_allclose(
                                dot(a, x), b,
                                rtol=500 * _eps_cast(a1.dtype),
                                atol=500 * _eps_cast(a1.dtype),
                                err_msg="driver: %s" % lapack_driver)
                        else:
                            assert_allclose(
                                dot(a, x), b,
                                rtol=1000 * _eps_cast(a1.dtype),
                                atol=1000 * _eps_cast(a1.dtype),
                                err_msg="driver: %s" % lapack_driver)
    print('Example 5 finished', flush=True)


def eg6():
    #tests/test_decomp.py::test_orth_memory_efficiency Fatal Python error: Fatal Python error: Segmentation fault
    from scipy.linalg.tests.test_decomp import _check_orth
    import numpy as np
    # Pick n so that 16*n bytes is reasonable but 8*n*n bytes is unreasonable.
    # Keep in mind that @pytest.mark.slow tests are likely to be running
    # under configurations that support 4Gb+ memory for tests related to
    # 32 bit overflow.
    n = 10*1000*1000
    try:
        _check_orth(n, np.float64, skip_big=True)
    except MemoryError:
        raise AssertionError('memory error perhaps caused by orth regression')
    print('Example 6 finished', flush=True)


def eg7():
    #FAILED tests/test_lapack.py::test_sgesdd_lwork_bug_workaround
    import sys
    import subprocess
    import time
    from numpy.testing import assert_equal
    # Test that SGESDD lwork is sufficiently large for LAPACK.
    #
    # This checks that workaround around an apparent LAPACK bug
    # actually works. cf. gh-5401
    #
    # xslow: requires 1GB+ of memory
    p = subprocess.Popen([sys.executable, '-c',
                          'import numpy as np; '
                          'from scipy.linalg import svd; '
                          'a = np.zeros([9537, 9537], dtype=np.float32); '
                          'svd(a)'],
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    # Check if it an error occurred within 5 sec; the computation can
    # take substantially longer, and we will not wait for it to finish
    for j in range(50):
        time.sleep(0.1)
        if p.poll() is not None:
            returncode = p.returncode
            break
    else:
        # Didn't exit in time -- probably entered computation.  The
        # error is raised before entering computation, so things are
        # probably OK.
        returncode = 0
        p.terminate()
    assert_equal(returncode, 0,
                 "Code apparently failed: " + p.stdout.read())
    print('Example 7 finished', flush=True)


def eg8():
    #tests/test_basic.py::TestSolve::test_random_sym_complex zsh: illegal hardware instruction  jep
    from numpy.random import random
    from numpy import conjugate, dot
    from scipy.linalg import solve
    from numpy.testing import assert_array_almost_equal
    n = 20
    a = random([n, n])
    a = a + 1j * random([n, n])
    for i in range(n):
        a[i, i] = abs(20 * (.1 + a[i, i]))
        for j in range(i):
            a[i, j] = conjugate(a[j, i])
    b = random([n]) + 2j * random([n])
    for i in range(2):
        x = solve(a, b, sym_pos=1)
        assert_array_almost_equal(dot(a, x), b)
    print('Example 8 finished', flush=True)
