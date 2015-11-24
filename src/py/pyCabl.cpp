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

void onPadChanged(PyObject* callable, Device::Pad pad_, uint16_t val_, bool shiftKey_)
{
   // Invoke callable, passing a Python object which holds a reference to x
   boost::python::call<void>(callable, pad_, val_, shiftKey_);
}

BOOST_PYTHON_MODULE(pycabl)
{
  enum_<DeviceDescriptor::Type>("DeviceDescriptorType")
        .value("USB", DeviceDescriptor::Type::USB)
        .value("MIDI", DeviceDescriptor::Type::MIDI)
        .value("HID", DeviceDescriptor::Type::HID)
        .value("Unknown", DeviceDescriptor::Type::Unknown)
        ;
  
#define M_PAD_CASE(nPad) value("Pad1##nPad", Device::Pad::Pad##nPad)

 enum_<Device::Pad>("Pad")
  .M_PAD_CASE( 1).M_PAD_CASE( 2).M_PAD_CASE( 3).M_PAD_CASE( 4)
  .M_PAD_CASE( 5).M_PAD_CASE( 6).M_PAD_CASE( 7).M_PAD_CASE( 8)
  .M_PAD_CASE( 9).M_PAD_CASE(10).M_PAD_CASE(11).M_PAD_CASE(12)
  .M_PAD_CASE(13).M_PAD_CASE(14).M_PAD_CASE(15).M_PAD_CASE(16)
  .M_PAD_CASE(17).M_PAD_CASE(18).M_PAD_CASE(19).M_PAD_CASE(20)
  .M_PAD_CASE(21).M_PAD_CASE(22).M_PAD_CASE(23).M_PAD_CASE(24)
  .M_PAD_CASE(25).M_PAD_CASE(26).M_PAD_CASE(27).M_PAD_CASE(28)
  .M_PAD_CASE(29).M_PAD_CASE(30).M_PAD_CASE(31).M_PAD_CASE(32)
  .M_PAD_CASE(33).M_PAD_CASE(34).M_PAD_CASE(35).M_PAD_CASE(36)
  .M_PAD_CASE(37).M_PAD_CASE(38).M_PAD_CASE(39).M_PAD_CASE(40)
  .M_PAD_CASE(41).M_PAD_CASE(42).M_PAD_CASE(43).M_PAD_CASE(44)
  .M_PAD_CASE(45).M_PAD_CASE(46).M_PAD_CASE(47).M_PAD_CASE(48)
  .M_PAD_CASE(49).M_PAD_CASE(50).M_PAD_CASE(51).M_PAD_CASE(52)
  .M_PAD_CASE(53).M_PAD_CASE(54).M_PAD_CASE(55).M_PAD_CASE(56)
  .M_PAD_CASE(57).M_PAD_CASE(58).M_PAD_CASE(59).M_PAD_CASE(60)
  .M_PAD_CASE(61).M_PAD_CASE(62).M_PAD_CASE(63).M_PAD_CASE(64)
  ;
#undef M_PAD_CASE
  
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
