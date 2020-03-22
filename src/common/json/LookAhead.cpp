//
// Created by thallock on 2/14/20.
//

#include "LookAhead.h"

#define END_VALUE 0
#define UNINITIALIZED_VALUE 0

LookAhead::LookAhead(InputBufferWrapper& in) :
        peekValue{UNINITIALIZED_VALUE},
        atEnd{false},
        hasPeeked{false},
        in{in} {}

char LookAhead::read() {
    if (hasPeeked) {
        hasPeeked = false;
        char r = peekValue;
        peekValue = UNINITIALIZED_VALUE;
        return r;
    }
    if (atEnd) {
        return 0;
    }
    int read = in.next();
    if (read == EOF) {
        atEnd = true;
        return END_VALUE;
    }
    return (char) read;
}

char LookAhead::peek() {
    if (hasPeeked) {
        return peekValue;
    }
    if (atEnd) {
        return END_VALUE;
    }
    hasPeeked = true;
    int read = in.next();
    if (read == EOF) {
        atEnd = true;
        peekValue = END_VALUE;
    } else {
        peekValue = (char) read;
    }
    return peekValue;
}
