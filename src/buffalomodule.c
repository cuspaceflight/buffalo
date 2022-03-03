/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                    BUFFALO                                   *
*                       FLIGHT COMPUTER SIMULATION SYSTEM                      *
*                                                                              *
*                  COPYRIGHT (C) 2021 H.E. FRANKS, B.M. ANDREW                 *
*                        CAMBRIDGE UNIVERSITY SPACEFLIGHT                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <math.h>
#include <Python.h>
#include <stdio.h>

#include "state_estimation.h"

typedef struct {
    float time;
    float accel;
    float pressure;
} sensor_data_t;

sensor_data_t* sensor_data;
size_t len_data;

static PyObject* loadData(PyObject* self, PyObject* args)
{
    PyObject* data;
    PyObject* item;
    PyObject* a;
    PyObject* p;
    float t;

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

        t = (float) PyFloat_AsDouble(PyObject_GetAttrString(item, "time"));
        a = PyObject_GetAttrString(item, "accel");
        p = PyObject_GetAttrString(item, "pressure");

        sensor_data_t reading = {
            .time     = t,
            .accel    = (a == Py_None) ? NAN : PyFloat_AsDouble(a),
            .pressure = (p == Py_None) ? NAN : PyFloat_AsDouble(p)
        };
        sensor_data[i] = reading;

        if (PyErr_Occurred()) return NULL;
    }

    state_estimation_init();

    Py_RETURN_NONE;
}

static PyObject* simulate(PyObject* self, PyObject* args)
{
    output_t se;

    PyObject* output = PyList_New(0);

    float prev_time = sensor_data[0].time;
    float prev_print_time = 0;

    for (size_t i = 1; i < len_data; i++) {
        if (PyErr_CheckSignals()) return NULL; /* exit on Ctrl-C */
        sensor_data_t current_reading = sensor_data[i];
       
        gaussian_t accel = { current_reading.accel, 0.3f };

        gaussian_t pressure = { current_reading.pressure, 250.0f };

        se = state_estimation_tick(current_reading.time - prev_time,
                                   &pressure, &accel);

        prev_time = current_reading.time;

        if (current_reading.time - prev_print_time > 0.02) {
            prev_print_time = current_reading.time;
            if (PyList_Append(output,
                              Py_BuildValue("dddd", current_reading.time,
                                            se.h, se.s, se.a)
                             ) == -1)
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

