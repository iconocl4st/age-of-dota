//
// Created by thallock on 2/14/20.
//

#ifndef PLAYING_JSONTOKENER_H
#define PLAYING_JSONTOKENER_H

#include <sstream>
#include <iostream>
#include "LookAhead.h"

namespace json_token {
    enum JsonToken {
        BEFORE_READING,
        AFTER_READING,

        BEGIN_OBJECT,
        END_OBJECT,
        BEGIN_ARRAY,
        END_ARRAY,
        TRUE,
        FALSE,
        STRING,
        INTEGER,
        REAL,
        NULL_VALUE,
        COLON,
        COMMA,
    };

    std::string getName(JsonToken token);
}

class JsonTokener {
private:
    LookAhead lookAhead;

    void skipWhiteSpace();
    void readString();
    void readNumber(char first);
    void assertString(const std::string& expected);

public:
    std::string name;
    std::ostringstream current;
    json_token::JsonToken currentToken;

    JsonTokener(InputBufferWrapper& in, const std::string& name) :
        lookAhead{in},
        current{},
        currentToken{json_token::BEFORE_READING},
        name{name} {}

    void next();
};


#endif //PLAYING_JSONTOKENER_H
