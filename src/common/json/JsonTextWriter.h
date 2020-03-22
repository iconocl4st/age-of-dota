//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_JSONTEXTWRITER_H
#define PLAYING_JSONTEXTWRITER_H


#include "JsonWriter.h"
#include "../network/BufferWrapper.h"

class TextJsonWriter : public JsonWriter {
private:
	OutputBufferWrapper& out;
	bool requiresComma;
	int indentation;

	void indent();
	void writeComma();
	void writeKeyOnly(const std::string& key);

public:
	TextJsonWriter(OutputBufferWrapper& out);
	~TextJsonWriter() = default;

	void writeKey(const std::string& key);

	void writeBeginArray(const std::string& key);
	void writeBeginArray();
	void writeEndArray();

	void writeBeginObject(const std::string& key);
	void writeBeginObject();
	void writeEndObject();

	void writeBoolean(const std::string& key, bool val);
	void writeInt(const std::string& key, int val);
	void writeFloat(const std::string& key, float val);
	void writeDouble(const std::string& key, double val);
	void writeString(const std::string& key, const std::string& val);
	void writeNull(const std::string& key);

	void writeBoolean(bool val);
	void writeInt(int val);
	void writeFloat(float val);
	void writeDouble(double val);
	void writeString(const std::string& val);
	void writeNull();

	void flush();
};



#endif //PLAYING_JSONTEXTWRITER_H
