/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <boost/python.hpp>

using namespace boost::python;

#include "cabl.h"

namespace sl
{
namespace cabl
{

using namespace boost::python;

//--------------------------------------------------------------------------------------------------

template <class T>
boost::python::list toPythonList(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    boost::python::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
}

//--------------------------------------------------------------------------------------------------

struct NullDeleter
{
  void operator()(const void*){}
};

//--------------------------------------------------------------------------------------------------

std::shared_ptr<DeviceFactory> deviceFactory()
{
  return std::shared_ptr<DeviceFactory>( &DeviceFactory::instance(), NullDeleter() );
}

list enumerateDevices()
{
  return toPythonList(ClientSingle::enumerateDevices());
}
//--------------------------------------------------------------------------------------------------

BOOST_PYTHON_MODULE(pycabl)
{
  enum_<DeviceDescriptor::Type>("DeviceDescriptorType")
        .value("USB", DeviceDescriptor::Type::USB)
        .value("MIDI", DeviceDescriptor::Type::MIDI)
        .value("HID", DeviceDescriptor::Type::HID)
        .value("Unknown", DeviceDescriptor::Type::Unknown)
        ;
  
  class_<DeviceFactory, std::shared_ptr<DeviceFactory>, boost::noncopyable >("DeviceFactory",no_init)
    .def("instance",&deviceFactory )
    .staticmethod("instance");

  class_<ClientSingle, boost::noncopyable>("ClientSingle")
    .def("enumerateDevices",&enumerateDevices).staticmethod("enumerateDevices")
    .def("run",&ClientSingle::run);
  
  class_<DeviceDescriptor>("DeviceDescriptor", init<std::string, DeviceDescriptor::Type,
    DeviceDescriptor::tVendorId,
    DeviceDescriptor::tProductId>())
      .def("getName",&DeviceDescriptor::getName, return_internal_reference<>())
      ;
}

/*

/this is the variable that will hold a reference to the python function
PyObject *py_callback;

//the following function will invoked from python to populate the call back reference
PyObject *set_py_callback(PyObject *callable)
{
    py_callback = callable;       // Remember new callback
    return Py_None;
}
//Initialize and acquire the global interpreter lock
PyEval_InitThreads();

//Ensure that the current thread is ready to call the Python C API
PyGILState_STATE state = PyGILState_Ensure();

//invoke the python function
boost::python::call<void>(py_callback);

//release the global interpreter lock so other threads can resume execution
PyGILState_Release(state);

*/

//--------------------------------------------------------------------------------------------------

} // cabl
} // sl
