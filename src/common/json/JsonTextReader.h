//
// Created by thallock on 3/10/20.
//

#ifndef PLAYING_JSONTEXTREADER_H
#define PLAYING_JSONTEXTREADER_H

#include "JsonTokener.h"
#include "JsonReader.h"

class TextJsonReader: public JsonReader {
private:
	JsonTokener tokenizer;

	void expect(const std::string& expected);
	void expect(json_token::JsonToken expected);
	void skipComma();
public:
	TextJsonReader(InputBufferWrapper& in, const std::string& name);

	~TextJsonReader() = default;

	void readKey(const std::string& key);

	void readBeginArray(const std::string& key);
	void readBeginArray();
	void readEndArray();
	void readBeginObject(const std::string& key);
	void readBeginObject();
	void readEndObject();
	bool readBoolean(const std::string& key);
	int readInt(const std::string& key);
	float readFloat(const std::string& key);
	double readDouble(const std::string& key);
	std::string readString(const std::string& key);
	bool readBoolean();
	int readInt();
	float readFloat();
	double readDouble();
	std::string readString();

	bool atArrayEnd(json_token::JsonToken alternateToken);
};



#endif //PLAYING_JSONTEXTREADER_H
