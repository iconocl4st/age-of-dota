//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_JSONREADER_H
#define PLAYING_JSONREADER_H

#include <string>

#include "JsonTokener.h"


class JsonReader {
public:
    virtual ~JsonReader() = default;

    virtual void readBeginArray(const std::string& key) = 0;
    virtual void readBeginArray() = 0;
    virtual void readEndArray() = 0;

    virtual void readBeginObject(const std::string& key) = 0;
    virtual void readBeginObject() = 0;
    virtual void readEndObject() = 0;

    virtual bool readBoolean(const std::string& key) = 0;
    virtual int readInt(const std::string& key) = 0;
    virtual float readFloat(const std::string& key) = 0;
    virtual double readDouble(const std::string& key) = 0;
    virtual std::string readString(const std::string& key) = 0;

    virtual bool readBoolean() = 0;
    virtual int readInt() = 0;
    virtual float readFloat() = 0;
    virtual double readDouble() = 0;
    virtual std::string readString() = 0;

    virtual bool atArrayEnd(json_token::JsonToken alternateToken) = 0;

    virtual void readKey(const std::string& key) = 0;
};


#endif //PLAYING_JSONREADER_H
