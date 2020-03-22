//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_LOOKAHEAD_H
#define PLAYING_LOOKAHEAD_H

#include "../network/BufferWrapper.h"

class LookAhead {
private:
    InputBufferWrapper& in;

public:
    char peekValue;
    bool atEnd;
    bool hasPeeked;

    LookAhead(InputBufferWrapper& in);

    char read();

    char peek();
};


#endif //PLAYING_LOOKAHEAD_H
