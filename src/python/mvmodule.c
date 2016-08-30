#include <Python.h>
#include "../../bin/libmv.h"



static PyObject*
matching_from_networkx(PyObject* self, PyObject* args)
{
  const char* name;
  PyObject * graph;
  if (!PyArg_ParseTuple(args, "O", &graph))
    return NULL;

  PyObject * psize;
  psize = PyObject_CallMethod(graph,"number_of_nodes","()");
  int size;
  size = PyInt_AsLong(psize);

  PyObject * pedges;
  pedges = PyObject_CallMethod(graph,"edges","()");
  int edgesL;
  edgesL = PyList_Size(pedges);

  Graph *g = Graph_init (size);

  int i;
  PyObject * cur_edge;
  for(i=0;i<edgesL;i++){
    cur_edge = PyList_GetItem(pedges,i);
    long a,b;
    a = PyInt_AsLong(PyTuple_GetItem(cur_edge,0));
    b = PyInt_AsLong(PyTuple_GetItem(cur_edge,1));
    Graph_add_edge (g, a, b);
  }

  EdgeList *matching = MV_MaximumCardinalityMatching(g);
  EdgeListIterator * current = matching->first;

  int match_num = 0;
  while (current) {
    ++match_num;
    current = current->next;
  }

  current = matching->first;

  PyObject * list =  PyList_New(match_num);
  i =0;
  while(current) {

    PyObject * o = PyTuple_New(2);
    int a,b;
    a = current->value.v1;
    b = current->value.v2;
    PyTuple_SetItem(o, 0, PyInt_FromLong(a));
    PyTuple_SetItem(o, 1, PyInt_FromLong(b));
    PyList_SetItem(list, i, o);
    current = current-> next;
    i++;
  }
  // deallocate
  EdgeList_delete(matching);
  Graph_delete(g);
  //  return mvi;
  //Py_RETURN_NONE;
  return list;
}

static PyMethodDef MVMethods[] =
  {
    {"matching_from_networkx", matching_from_networkx, METH_VARARGS, "Generates a matching from a networkX graph.."},
    {NULL, NULL, 0, NULL}
  };
 
PyMODINIT_FUNC
initmv(void)
{
  (void) Py_InitModule("mv", MVMethods);
}
