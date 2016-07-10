#include "StdAfx.h"
#include "OHLCVReader_Py.h"
#include "BoostExport.h"
#include <boost/python.hpp>
#include <boost/thread.hpp>
#include "../Common/ScopedGILRelease.h"
#include "logger.h"

OHLCVReader_Py::OHLCVReader_Py(const Symbol& symbol)
   : m_symbol(symbol),
   m_pthread()
{
}

OHLCVReader_Py::~OHLCVReader_Py(void)
{
   if(m_pthread != 0)
      m_pthread->join();
}

void OHLCVReader_Py::read()
{
   m_pthread = shared_ptr<boost::thread>(new boost::thread(boost::bind(&OHLCVReader_Py::read_impl, this)));
}

void OHLCVReader_Py::read_impl()
{
   try
   {
      ScopedGILAcquire lock;
      initDataFeed();
      object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
      object dictionary = module.attr("__dict__");
      
      PyRun_SimpleString("execfile('CSV_OHLCV.py')");
      object run(module.attr("run"));
      run(ptr(this));
   }
   catch(error_already_set)
   {
      PyErr_Print();
   }
}

void OHLCVReader_Py::push_data(int open, int close, int high, int low, int adj_close, const boost::posix_time::ptime& starttime, Resolution res, int vol)
{
   Py_BEGIN_ALLOW_THREADS

   shared_ptr<OHLCV> p_ohlcv(new OHLCV(open, close, high, low, adj_close, starttime, res, vol));
   datalog << *p_ohlcv;
   m_queue.push(p_ohlcv);

   Py_END_ALLOW_THREADS
}