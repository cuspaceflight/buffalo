/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                    BUFFALO                                   *
*                       FLIGHT COMPUTER SIMULATION SYSTEM                      *
*                                                                              *
*                  COPYRIGHT (C) 2021 H.E. FRANKS, B.M. ANDREW                 *
*                        CAMBRIDGE UNIVERSITY SPACEFLIGHT                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Python.h>

#include "flightdata.h"
#include "fc.h"


static PyObject* loadAlt(PyObject* self, PyObject* args) {
    PyObject* PyList_altitudes;
    if (!PyArg_ParseTuple(args, "O", &PyList_altitudes)) Py_RETURN_NONE;

    // Must be list
    if (!PyList_Check(PyList_altitudes)) Py_RETURN_NONE;

    size_t len = PyObject_Length(PyList_altitudes);
    if (!len) Py_RETURN_NONE;

    // Prevent double-free if this is the first call
    if (altitudes != NULL) free((void*) altitudes);

    altitudes = (float*) malloc(len * sizeof(float));

    for (size_t i = 0; i < len; i++) {
        altitudes[i] = (float) PyFloat_AsDouble(
            PyList_GetItem(PyList_altitudes, i));

        if (PyErr_Occurred()) Py_RETURN_NONE;
    }
    Py_RETURN_NONE;
}

static PyObject* loadBaro(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyObject* loadAccel(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyObject* loadGyro(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

static inline PyObject* simulate()
{
    Py_RETURN_NONE;
}

static PyObject* simulateDummySensors(PyObject* self, PyObject* args,
                                                      PyObject* keywds)
{
    /* PRECALCULATE FLIGHT PROFILE */
    return simulate();
}

static PyObject* simulateRealSensors(PyObject* self, PyObject* args,
                                                     PyObject* keywds)
{
    /* PRECALCULATE FLIGHT PROFILE */
    return simulate();
}


static PyMethodDef buffalomethods[] = {
    {
        "load_alt",
        (PyCFunction)(void(*)(void))loadAlt,
        METH_VARARGS,
        "Load flight altitude data."
    },
    {
        "load_baro",
        (PyCFunction)(void(*)(void))loadBaro,
        METH_VARARGS,
        "Load flight barometer data."
    },
    {
        "load_accel",
        (PyCFunction)(void(*)(void))loadAccel,
        METH_VARARGS,
        "Load flight accelerometer data."
    },
    {
        "load_gyro",
        (PyCFunction)(void(*)(void))loadGyro,
        METH_VARARGS,
        "Load flight gyroscope data."
    },
    {
        "simulate_dummy_sensors",
        (PyCFunction)(void(*)(void))simulateDummySensors,
        METH_VARARGS | METH_KEYWORDS,
        "Simulate flight computer operation with dummy sensor data."
    },
    {
        "simulate_real_sensors",
        (PyCFunction)(void(*)(void))simulateRealSensors,
        METH_VARARGS | METH_KEYWORDS,
        "Simulate flight computer operation with real sensor data."
    }
};

static PyModuleDef buffalomodule = {
    PyModuleDef_HEAD_INIT,
    "buffalo",
    "Flight computer simulation.",
    -1,
    buffalomethods
};

PyMODINIT_FUNC PyInit_buffalo() { return PyModule_Create(&buffalomodule); }

