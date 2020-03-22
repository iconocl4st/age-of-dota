//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_JSONWRITER_H
#define PLAYING_JSONWRITER_H

#include <string>

class JsonWriter {
public:
	virtual ~JsonWriter() = default;

	virtual void writeKey(const std::string& key) = 0;

	virtual void writeBeginArray(const std::string& key) = 0;
	virtual void writeBeginArray() = 0;
	virtual void writeEndArray() = 0;

	virtual void writeBeginObject(const std::string& key) = 0;
	virtual void writeBeginObject() = 0;
	virtual void writeEndObject() = 0;

	virtual void writeBoolean(const std::string& key, bool) = 0;
	virtual void writeInt(const std::string& key, int) = 0;
	virtual void writeFloat(const std::string& key, float) = 0;
	virtual void writeDouble(const std::string& key, double) = 0;
	virtual void writeString(const std::string& key, const std::string& val) = 0;
	virtual void writeNull(const std::string& key) = 0;

	virtual void writeBoolean(bool) = 0;
	virtual void writeInt(int) = 0;
	virtual void writeFloat(float) = 0;
	virtual void writeDouble(double) = 0;
	virtual void writeString(const std::string& val) = 0;
	virtual void writeNull() = 0;

	virtual void flush() = 0;
};


#endif //PLAYING_JSONWRITER_H
