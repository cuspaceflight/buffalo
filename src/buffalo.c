                     /* * * * * * * * * * * * * * * * * * *
                     *              BUFFALO               *
                     * FLIGHT COMPUTER SIMULATION SYSTEM  *
                     *                                    *
                     *  COPYRIGHT (C)  2021 H. E. FRANKS  *
                     *  CAMBRIDGE UNIVERSITY SPACEFLIGHT  *
                     * * * * * * * * * * * * * * * * * * */

#include <Python.h>

static PyMethodDef buffalomethods[] = {
    {
        "load_atmos",
        (PyCFunction)(void(*)(void))loadAtmos,
        METH_VARARGS,
        "Load atmospheric data."
    },
    {
        "load_alt",
        (PyCFunction)(void(*)(void))loadAlt,
        METH_VARARGS,
        "Load flight altitude data."
    },
    {
        "load_baro",
        (PycFunction)(void(*)(void))loadBaro,
        METH_VARARGS,
        "Load flight barometer data."
    },
    {
        "load_accel",
        (PycFunction)(void(*)(void))loadAccel,
        METH_VARARGS,
        "Load flight accelerometer data."
    },
    {
        "load_gyro",
        (PycFunction)(void(*)(void))loadGyro,
        METH_VARARGS,
        "Load flight gyroscope data."
    },
    {
        "simulate_dummy_sensors"
        (PyCFunction)(void(*)(void))simulateDummySensors,
        METH_VARARGS | METH_KEYWORDS,
        "Simulate flight computer operation with dummy sensor data."
    },
    {
        "simulate_real_sensors"
        (PyCFunction)(void(*)(void))simulateDummySensors,
        METH_VARARGS | METH_KEYWORDS,
        "Simulate flight computer operation with real sensor data."
    }
}

static PyModuleDef buffalomodule = {
    PyModuleDef_HEAD_INIT,
    "buffalo"
    "Flight computer simulation."
    -1,
    buffalomethods
}

PyMODINIT_FUNC PyInit_buffalo() { return PyModule_Create(&buffalomodule); }

