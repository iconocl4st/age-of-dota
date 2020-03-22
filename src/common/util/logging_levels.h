//
// Created by thallock on 3/17/20.
//

#ifndef PLAYING_LOGGING_LEVELS_H
#define PLAYING_LOGGING_LEVELS_H

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

enum log_levels {
	DEBUG,
	NORMAL,
	WARN,
	ERROR,
};


typedef boost::log::sources::severity_logger_mt<log_levels> Logger;

#endif //PLAYING_LOGGING_LEVELS_H
