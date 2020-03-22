//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_CONNECTIONBUFFER_H
#define PLAYING_CONNECTIONBUFFER_H


#include <boost/asio/streambuf.hpp>

#define BUFFER_LENGTH 1024

class ConnectionBuffer {

    boost::asio::streambuf inputBuffer;
    boost::asio::streambuf outputBuffer;



};


#endif //PLAYING_CONNECTIONBUFFER_H
