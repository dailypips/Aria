// Aria.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "Singleton.h"
#define INIT_LOG
#include "logger.h"
#include "logger_init.h"
#include <boost/date_time.hpp>
#include "../Engine/Engine.h"
#include "../Data/Resolution.h"

using namespace boost;

int main(int argc, char* argv[])
{
   init_log();
   posix_time::ptime start(boost::gregorian::date(2005, 02, 01));

   Singleton<Engine>::instance().init_back_test(start, Day);
   Singleton<Engine>::instance().run();
   
   logging::core::get()->remove_all_sinks();
   return 0;
}

#undef INIT_LOG