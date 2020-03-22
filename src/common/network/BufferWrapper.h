//
// Created by thallock on 2/17/20.
//

#ifndef PLAYING_BUFFERWRAPPER_H
#define PLAYING_BUFFERWRAPPER_H

#include <string>
#include <iostream>
#include <memory>

#include "BufferManager.h"

class OutputBufferWrapper {
public:
    virtual ~OutputBufferWrapper() = default;

    virtual void write(const std::string& string) = 0;
    virtual void flush() = 0;
};


class InputBufferWrapper {
public:
    virtual ~InputBufferWrapper() = default;

    virtual int next() = 0;
};


class SocketOutputBufferWrapper : public OutputBufferWrapper {
private:
	int socket;
	Buffer *currentBuffer;
	std::size_t currentIndex;

public:
	SocketOutputBufferWrapper(int socket);
	~SocketOutputBufferWrapper();

	void write(const std::string& string);
	void flush();
};


class StreamOutputBufferWrapper : public OutputBufferWrapper {
private:
	std::ostream& out;

public:
	StreamOutputBufferWrapper(std::ostream& out);
	~StreamOutputBufferWrapper();

	void write(const std::string& string);
	void flush();
};


class SocketInputBufferWrapper : public InputBufferWrapper {
private:
	int socket;
	Buffer *buffer;
	std::size_t currentIndex;
	std::size_t currentEnd;

	void fillBuffer();
public:
	SocketInputBufferWrapper(int socket);
	~SocketInputBufferWrapper();

	int next();
};


#endif //PLAYING_BUFFERWRAPPER_H
