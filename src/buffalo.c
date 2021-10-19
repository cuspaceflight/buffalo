                     /* * * * * * * * * * * * * * * * * * *
                     *              BUFFALO               *
                     * FLIGHT COMPUTER SIMULATION SYSTEM  *
                     *                                    *
                     *  COPYRIGHT (C)  2021 H. E. FRANKS  *
                     *  CAMBRIDGE UNIVERSITY SPACEFLIGHT  *
                     * * * * * * * * * * * * * * * * * * */

#include <Python.h>

#include "flightdata.h"

static PyObject* loadAlt(PyObject* self, PyObject* args)
{
    PyObject* PyList_altitudes;
    if (!PyArg_ParseTuple(args, "O", &PyList_altitudes))
        return NULL;

    size_t len = PyObject_Length(PyList_altitudes);
    if (!len) return NULL; // TODO: Check if list

    free((void*) altitudes);
    altitudes = (float*) malloc(len * sizeof(float));

    for (size_t i = 0; i < len; i++) {
        PyObject* item = PyList_GetItem(PyList_altitudes, i);
        altitudes[i] = (float) PyFloat_AsDouble(item);

        if (PyErr_Occurred()) return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* loadBaro(PyObject* self, PyObject* args)
{
    Py_RETURN_NONE;
}

static PyObject* loadAccel(PyObject* self, PyObject* args)
{
    Py_RETURN_NONE;
}

static PyObject* loadGyro(PyObject* self, PyObject* args)
{
    Py_RETURN_NONE;
}

static PyObject* simulateDummySensors(PyObject* self, PyObject* args,
                                                      PyObject* keywds)
{
    Py_RETURN_NONE;
}

static PyObject* simulateRealSensors(PyObject* self, PyObject* args,
                                                     PyObject* keywds)
{
    Py_RETURN_NONE;
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

