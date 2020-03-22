//
// Created by thallock on 2/17/20.
//

#include "BufferWrapper.h"

#include <unistd.h>
#include <sys/socket.h>

SocketOutputBufferWrapper::SocketOutputBufferWrapper(int socket) :
	socket{socket},
	currentBuffer{getBufferManager()->getBuffer()},
	currentIndex{0} {}

SocketOutputBufferWrapper::~SocketOutputBufferWrapper() {
	flush();
	getBufferManager()->doneWithBuffer(currentBuffer);
}

 void SocketOutputBufferWrapper::write(const std::string &string) {
	std::size_t stringIndex = 0;
	std::size_t stringSize = string.size();
	while (stringIndex < stringSize) {
		std::size_t remainingSpace = currentBuffer->size - currentIndex;
		std::size_t remainingCharacters = stringSize - stringIndex;
		std::size_t to_write = std::min(remainingSpace, remainingCharacters);

		for (std::size_t i = 0; i < to_write; i++) {
			currentBuffer->data[currentIndex + i] = string[stringIndex + i];
		}
		stringIndex += to_write;
		currentIndex += to_write;

		if (currentIndex == currentBuffer->size) {
			flush();
		}
	}
}

void SocketOutputBufferWrapper::flush() {
	BufferManager *manager = getBufferManager();
	Buffer *toSend = currentBuffer;
	std::size_t amountToSend = currentIndex;
	currentBuffer = manager->getBuffer();
	currentIndex = 0;

	send(socket, toSend->data, amountToSend, 0);
	manager->doneWithBuffer(toSend);
}

StreamOutputBufferWrapper::StreamOutputBufferWrapper(std::ostream &out) : out{out} {}

StreamOutputBufferWrapper::~StreamOutputBufferWrapper() {
	out.flush(); // Is this all I should do?
}

void StreamOutputBufferWrapper::write(const std::string &string) { out << string; }

void StreamOutputBufferWrapper::flush() { out.flush(); }


void SocketInputBufferWrapper::fillBuffer() {
	currentEnd = read(socket, buffer->data, buffer->size);
	currentIndex = 0;
}

SocketInputBufferWrapper::SocketInputBufferWrapper(int socket) :
	socket{socket},
	buffer{getBufferManager()->getBuffer()},
	currentIndex{0},
	currentEnd{0} {}

SocketInputBufferWrapper::~SocketInputBufferWrapper() {
	getBufferManager()->doneWithBuffer(buffer);
}

int SocketInputBufferWrapper::next() {
	if (currentIndex == currentEnd) {
		fillBuffer();
	}
	return 0xff & buffer->data[currentIndex++];
}
