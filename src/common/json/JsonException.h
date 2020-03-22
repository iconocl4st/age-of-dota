//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_JSONEXCEPTION_H
#define PLAYING_JSONEXCEPTION_H

#include <exception>
#include <string>

class JsonException: public std::exception
{
private:
	const std::string str;
public:
	JsonException(const std::string str) : str{str} {}

	virtual const char* what() const throw() {
		return str.c_str();
	}
};


#endif //PLAYING_JSONEXCEPTION_H
