#include "PyNative.h"

PyObject* add(PyObject *self, PyObject *args)
{
	// dd means two doubles
	double x, y;
	if (!PyArg_ParseTuple(args, "dd", &x, &y))
	{
		return 0;
	}

	return PyFloat_FromDouble(x+y);
}

static struct PyMethodDef pyNativeMethodsDef[]
{
	{"add", (PyCFunction) add, METH_VARARGS, NULL },
	{ NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef pyNativeModuleDef
{
	PyModuleDef_HEAD_INIT,
	"PyNative",
	"",
	-1,
	pyNativeMethodsDef
};

PyMODINIT_FUNC
PyInit_PyNative(void)
{
	return PyModule_Create(&pyNativeModuleDef);
}