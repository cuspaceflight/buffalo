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
    PyObject* item;
    char* data_type;
    enum sensor_type dtype;
    float t, v;

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

    for (size_t i = 0; i < len_data; i++) {
        item = PyList_GetItem(data, i);

        data_type = PyBytes_AsString(PyUnicode_AsUTF8String(
            PyObject_GetAttrString(item, "data_type")));
        if (PyErr_Occurred() || PyErr_CheckSignals())
            return NULL; /* quit on ^C */

        dtype = strcmp(data_type, "BARO") ? ACCEL : BARO;

        t = (float) PyFloat_AsDouble(PyObject_GetAttrString(item, "time"));
        v = (float) PyFloat_AsDouble(PyObject_GetAttrString(item, "value"));

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
    output_t se;

    PyObject* output = PyList_New(0);

    float prev_time = sensor_data[0].time;
    float prev_print_time = 0;

    for (size_t i = 1; i < len_data; i++) {
        sensor_data_t current_reading = sensor_data[i];
       
        if (current_reading.data_type != BARO) continue;
        se = state_estimation_tick(
            current_reading.time - prev_time, current_reading.value, 250.0f);
        prev_time = current_reading.time;

        if (current_reading.time - prev_print_time > 0.1) {
            prev_print_time = current_reading.time;
            if (PyList_Append(output, Py_BuildValue("ddd", current_reading.time,
                                                    se.h, se.s)) == -1)
                return NULL;
        }
        
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

