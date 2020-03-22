//
// Created by thallock on 2/14/20.
//

#include "JsonTokener.h"
#include "JsonException.h"

static bool std_isspace(char c) {
    switch (c) {
        case ' ':
        case '\n':
        case '\r':
        case '\b':
        case '\t':
            return true;
        default:
            return false;
    }
}

static bool std_isdigit(char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
    }
}

static bool is_real_number_char(char c) {
    return (
            c == 'E' ||
            c == 'e' ||
            c == '.'
    );
}

static bool is_number_char(char c) {
    return (
            std_isdigit(c) ||
            is_real_number_char(c) ||
            c == '-' ||
            c == '+'
    );
}


void JsonTokener::readNumber(char c) {
    currentToken = json_token::INTEGER;
    current << c;
    while (is_number_char(lookAhead.peek())) {
        c = lookAhead.read();
        current << c;
        if (is_real_number_char(c)) {
            currentToken = json_token::REAL;
        }
    } ;
}

void JsonTokener::readString() {
    currentToken = json_token::STRING;
    char c;
    while ((c = lookAhead.read()) != '"') {
        current << c;
    }
}

void JsonTokener::assertString(const std::string& expected) {
    char c;

    for (std::string::size_type i = 0; i < expected.size(); i++) {
        c = lookAhead.read();
        if (c == expected[i])
            continue;
        throw JsonException("Found a string different that expected.");
    }
}


void JsonTokener::skipWhiteSpace() {
    while (std_isspace(lookAhead.peek())) {
        lookAhead.read();
    }
}


void JsonTokener::next() {
    current.str("");
    current.clear();
    if (lookAhead.atEnd) {
        currentToken = json_token::AFTER_READING;
        return;
    }
    char c;
    skipWhiteSpace();
    c = lookAhead.read();

    if (is_number_char(c)) {
        readNumber(c);
        return;
    }

    switch (c) {
        case ',':
            currentToken = json_token::COMMA;
            break;
        case ':':
            currentToken = json_token::COLON;
            break;
        case '{':
            currentToken = json_token::BEGIN_OBJECT;
            break;
        case '}':
            currentToken = json_token::END_OBJECT;
            break;
        case '[':
            currentToken = json_token::BEGIN_ARRAY;
            break;
        case ']':
            currentToken = json_token::END_ARRAY;
            break;
        case '"':
            readString();
            break;
        case 't':
            assertString("rue");
            currentToken = json_token::TRUE;
            break;
        case 'f':
            assertString("alse");
            currentToken = json_token::FALSE;
            break;
        case 'n':
            assertString("ull");
            currentToken = json_token::NULL_VALUE;
            break;
        default:
            std::ostringstream exString;
            exString << "Unexpected character: " << c << std::endl;
            throw JsonException{exString.str()};
    }
}

namespace json_token {
    std::string getName(JsonToken token) {
        switch (token) {
            case BEFORE_READING:
                return "BEFORE_READING";
            case AFTER_READING:
                return "AFTER_READING";
            case BEGIN_OBJECT:
                return "BEGIN_OBJECT";
            case END_OBJECT:
                return "END_OBJECT";
            case BEGIN_ARRAY:
                return "BEGIN_ARRAY";
            case END_ARRAY:
                return "END_ARRAY";
            case TRUE:
                return "TRUE";
            case FALSE:
                return "FALSE";
            case STRING:
                return "STRING";
            case INTEGER:
                return "INTEGER";
            case REAL:
                return "REAL";
            case NULL_VALUE:
                return "NULL_VALUE";
            case COLON:
                return "COLON";
            case COMMA:
                return "COMMA";
            default:
                return "ERROR";
        }
    }
}
