/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                    BUFFALO                                   *
*                       FLIGHT COMPUTER SIMULATION SYSTEM                      *
*                                                                              *
*                  COPYRIGHT (C) 2021 H.E. FRANKS, B.M. ANDREW                 *
*                        CAMBRIDGE UNIVERSITY SPACEFLIGHT                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Python.h>

#include "state_estimation.h"

enum sensor_type { BARO, ACCEL };

typedef struct {
    enum sensor_type data_type;
    float time;
    float value;
} sensor_data_t;

sensor_data_t* sensor_data;
size_t len_data;

static PyObject* loadData(PyObject* self, PyObject* args)
{
    PyObject* data;
    if (!PyArg_ParseTuple(args, "O", &data)) {
        PyErr_SetString(PyExc_RuntimeError, "Error parsing list.");
        return NULL;
    }

    // Must be list
    if (!PyList_Check(data)) {
        PyErr_SetString(PyExc_RuntimeError, "Data is not a list.");
        return NULL;
    }

    len_data = PyObject_Length(data);

    if (!len_data) {
        PyErr_SetString(PyExc_RuntimeError, "List is empty.");
        return NULL;
    }

    // Prevent double-free if this is the first call
    if (sensor_data != NULL) free((void*) sensor_data);

    sensor_data = (sensor_data_t*) malloc(len_data * sizeof(sensor_data_t));

    const char* baro_string = "BARO";

    for (size_t i = 0; i < len_data; i++) {
        PyObject* item = PyList_GetItem(data, i);
        PyObject* strobj = PyObject_GetAttrString(item, "data_type");
        char* data_type = PyUnicode_AsUTF8String(strobj);
        if (PyErr_Occurred() || PyErr_CheckSignals())
            return NULL; /* quit on ^C */

        enum sensor_type dtype = strcmp(data_type, "BARO") ? ACCEL : BARO;

        float t = (float)PyFloat_AsDouble(PyObject_GetAttrString(item, "time"));
        float v = (float)PyFloat_AsDouble(PyObject_GetAttrString(item, "value"));

        sensor_data_t reading = {
            .data_type = dtype,
            .time     = t,
            .value    = v
        };
        sensor_data[i] = reading;

        if (PyErr_Occurred()) return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* simulate(PyObject* self, PyObject* args)
{
    state_estimate_t se;
    state_estimation_init();

    PyObject* output = PyList_New(0);

    for (size_t i = 1; i < len_data; i++) {
        sensor_data_t current_reading = sensor_data[i];
        float prev_time = sensor_data[i-1].time;
        
        se = state_estimation_get_state(current_reading.time -
                                        prev_time);

        switch (current_reading.data_type) {
            case BARO:
                state_estimation_new_pressure(current_reading.value,
                                              0.01 * current_reading.value);
            case ACCEL:
                state_estimation_new_accel(current_reading.value,
                                           0.01 * current_reading.value);
        }

        if (PyList_Append(output, Py_BuildValue(
            "dd", current_reading.time, se.h)) == -1)
            return NULL;
    }

    return output;
}

static PyMethodDef buffalomethods[] = {
    {
        "load_data",
        (PyCFunction)(void(*)(void))loadData,
        METH_VARARGS,
        "Load flight data."
    },
    {
        "simulate",
        (PyCFunction)(void(*)(void))simulate,
        METH_VARARGS,
        "Run flight simulation."
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

