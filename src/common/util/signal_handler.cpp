//
// Created by thallock on 3/18/20.
//

#include "signal_handler.h"

#define BOOST_STACKTRACE_USE_BACKTRACE

#include <signal.h>
#include <boost/stacktrace.hpp>

#include <iostream>
#include <fstream>

void my_signal_handler(int signum) {
	::signal(signum, SIG_DFL);
	boost::stacktrace::safe_dump_to("./backtrace.dump");


	std::ifstream ifs("./backtrace.dump");

	boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
	std::cout << "Crash:\n" << st << std::endl;

	::raise(SIGABRT);
}

void register_signal_handler() {
	::signal(SIGSEGV, &my_signal_handler);
	::signal(SIGABRT, &my_signal_handler);
}