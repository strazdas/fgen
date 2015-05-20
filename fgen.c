#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>

#define PY_ARRAY_UNIQUE_SYMBOL

static double distance(double *x1, double *x2, int n)
{
    double d = 0;
    int i;
    for (i = 0; i < n; i++) {
        d += (x1[i] - x2[i]) * (x1[i] - x2[i]);
    }
    return sqrt(d);
}

static double f(double **M, double *rho, double *gamma, double t, double globl, int n, int i)
{
    if (i == 0) {
        return t;
    } else if (i == 1) {
        return globl;
    } else {
        double o1 = rho[i] - distance(M[0], M[i], n);
        return o1 * o1 + t - gamma[i];
    }    
}

static double g(double **M, double t, int n, double *x) 
{
    double sum = 0.0;
    int i;
    for (i = 0; i < n; i++) {
        sum += (M[0][i] - x[i]) * (M[0][i] - x[i]);   
    }
    return sum + t;
}

static double A(double **M, double *rho, double *gamma, double t, double globl, int n, int i)
{
    double d = distance(M[0], M[i], n);
    return d * d + t - f(M, rho, gamma, t, globl, n, i);
}

static PyObject *
fgen_ndtype_call(PyObject *self, PyObject *args)
{
    PyObject *ndf, *x;
    if (!PyArg_UnpackTuple(args, "ndtype_call", 2, 2, &ndf, &x)) {
        return NULL;
    }
    PyObject *M, *rho, *gamma, *t, *globl, *n, *m;
    M = PyObject_GetAttrString(ndf, "M");
    rho = PyObject_GetAttrString(ndf, "rho");
    gamma = PyObject_GetAttrString(ndf, "gamma");
    t = PyObject_GetAttrString(ndf, "t");
    globl = PyObject_GetAttrString(ndf, "globl");
    n = PyObject_GetAttrString(ndf, "n");
    m = PyObject_GetAttrString(ndf, "m");
    double c_t = PyFloat_AsDouble(t);
    double c_globl = PyFloat_AsDouble(globl);
    long c_n = PyInt_AsLong(n);
    long c_m = PyInt_AsLong(m);
    int i, j;
    double **c_M, *c_rho, *c_gamma, *c_x;
    npy_intp dims_M; 
    PyArray_AsCArray(&M, (void **)&c_M, &dims_M, 2, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_rho;
    PyArray_AsCArray(&rho, (void *)&c_rho, &dims_rho, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_gamma;
    PyArray_AsCArray(&gamma, (void *)&c_gamma, &dims_gamma, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_x;
    PyArray_AsCArray(&x, (void *)&c_x, &dims_x, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    double result;
    int inside = 0;
    for (i = 1; i < c_m; i++) {
        if (distance(c_M[i], c_x, c_n) < c_rho[i]) {
            inside = 1;
            double dot = 0;
            for (j = 0; j < c_n; j++) {
                dot += (c_x[j] - c_M[i][j]) * (c_M[0][j] - c_M[i][j]);
            }
            double o1 = dot / distance(c_x, c_M[i], c_n);
            double o2 = (1.0 / (c_rho[i] * c_rho[i])) * A(c_M, c_rho, c_gamma, c_t, c_globl, c_n, i);
            double d = distance(c_x, c_M[i], c_n);
            double o3 = d * d;
            result = (1.0 - (2.0 / c_rho[i]) * o1 + o2) * o3 + f(c_M, c_rho, c_gamma, c_t, c_globl, c_n, i);
            break;
        }
    }
    if (!inside) {
        result = g(c_M, c_t, c_n, c_x) + c_t;
    }
    PyArray_Free(M, (void *)c_M);
    PyArray_Free(rho, (void *)c_rho);
    PyArray_Free(gamma, (void *)c_gamma);
    PyArray_Free(x, (void *)c_x);
    Py_DECREF(M);
    Py_DECREF(rho);
    Py_DECREF(gamma);
    Py_DECREF(t);
    Py_DECREF(globl);
    Py_DECREF(n);
    Py_DECREF(m);
    return Py_BuildValue("f", result);
}

static PyObject *
fgen_dtype_call(PyObject *self, PyObject *args)
{
    PyObject *ndf, *x;
    if (!PyArg_UnpackTuple(args, "dtype_call", 2, 2, &ndf, &x)) {
        return NULL;
    }
    PyObject *M, *rho, *gamma, *t, *globl, *n, *m;
    M = PyObject_GetAttrString(ndf, "M");
    rho = PyObject_GetAttrString(ndf, "rho");
    gamma = PyObject_GetAttrString(ndf, "gamma");
    t = PyObject_GetAttrString(ndf, "t");
    globl = PyObject_GetAttrString(ndf, "globl");
    n = PyObject_GetAttrString(ndf, "n");
    m = PyObject_GetAttrString(ndf, "m");
    double c_t = PyFloat_AsDouble(t);
    double c_globl = PyFloat_AsDouble(globl);
    long c_n = PyInt_AsLong(n);
    long c_m = PyInt_AsLong(m);
    int i, j;
    double **c_M, *c_rho, *c_gamma, *c_x;
    npy_intp dims_M; 
    PyArray_AsCArray(&M, (void **)&c_M, &dims_M, 2, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_rho;
    PyArray_AsCArray(&rho, (void *)&c_rho, &dims_rho, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_gamma;
    PyArray_AsCArray(&gamma, (void *)&c_gamma, &dims_gamma, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    npy_intp dims_x;
    PyArray_AsCArray(&x, (void *)&c_x, &dims_x, 1, PyArray_DescrFromType(PyArray_DOUBLE));
    double result;
    int inside = 0;
    for (i = 1; i < c_m; i++) {
        if (distance(c_M[i], c_x, c_n) < c_rho[i]) {
            inside = 1;
            double dot = 0;
            for (j = 0; j < c_n; j++) {
                dot += (c_x[j] - c_M[i][j]) * (c_M[0][j] - c_M[i][j]);
            }
            double o1 = (2.0 / (c_rho[i] * c_rho[i])) * (dot / distance(c_x, c_M[i], c_n)) - (2.0 / (c_rho[i] * c_rho[i] * c_rho[i])) * A(c_M, c_rho, c_gamma, c_t, c_globl, c_n, i);
            double o2 = 1.0 - (4.0 / c_rho[i]) * (dot / distance(c_x, c_M[i], c_n)) + (3.0 / (c_rho[i] * c_rho[i])) * A(c_M, c_rho, c_gamma, c_t, c_globl, c_n, i);
            double d = distance(c_x, c_M[i], c_n);
            result = o1 * d * d * d + o2 * d * d + f(c_M, c_rho, c_gamma, c_t, c_globl, c_n, i);
            break;
        }
    }
    if (!inside) {
        result = g(c_M, c_t, c_n, c_x) + c_t;
    }
    PyArray_Free(M, (void *)c_M);
    PyArray_Free(rho, (void *)c_rho);
    PyArray_Free(gamma, (void *)c_gamma);
    PyArray_Free(x, (void *)c_x);
    Py_DECREF(M);
    Py_DECREF(rho);
    Py_DECREF(gamma);
    Py_DECREF(t);
    Py_DECREF(globl);
    Py_DECREF(n);
    Py_DECREF(m);
    return Py_BuildValue("f", result);
}

static PyMethodDef FgenMethods[] = {
    {"ndtype_call",  fgen_ndtype_call, METH_VARARGS, "Get a value of an ND-Type function."},
    {"dtype_call", fgen_dtype_call, METH_VARARGS, "Get a value of a D-Type function."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initfgen(void)
{
    PyObject *m;
    m = Py_InitModule("fgen", FgenMethods);
    import_array();
}
