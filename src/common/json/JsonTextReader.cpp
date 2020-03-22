//
// Created by thallock on 2/14/20.
//

# include "JsonTextReader.h"
#include "JsonException.h"


TextJsonReader::TextJsonReader(InputBufferWrapper& in, const std::string &name) : tokenizer{in, name} {}

void TextJsonReader::expect(const std::string &expected) {
	if (tokenizer.current.str() == expected)
		return;
	std::ostringstream exString;
	exString << "Unexpected string. Expected " << expected << ", Found: " << tokenizer.current.str();
	throw JsonException(exString.str());
}


void TextJsonReader::expect(json_token::JsonToken expected) {
	if (tokenizer.currentToken == expected)
		return;
	if (expected == json_token::REAL && tokenizer.currentToken == json_token::INTEGER)
		// close enough...
		return;
	std::ostringstream exString;
	exString << "Unexpected token. Expected " << json_token::getName(expected) << ", Found: "
			 << json_token::getName(tokenizer.currentToken);
	throw JsonException(exString.str());
}

void TextJsonReader::skipComma() {
	if (tokenizer.currentToken == json_token::COMMA)
		tokenizer.next();
}

void TextJsonReader::readKey(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
}

void TextJsonReader::readBeginArray(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	tokenizer.next();
	expect(json_token::BEGIN_ARRAY);
}

void TextJsonReader::readBeginArray() {
	tokenizer.next();
	skipComma();
	expect(json_token::BEGIN_ARRAY);
}

void TextJsonReader::readEndArray() {
	tokenizer.next();
	expect(json_token::END_ARRAY);
}

void TextJsonReader::readBeginObject(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	tokenizer.next();
	expect(json_token::BEGIN_OBJECT);
}

void TextJsonReader::readBeginObject() {
	tokenizer.next();
	skipComma();
	expect(json_token::BEGIN_OBJECT);
}

void TextJsonReader::readEndObject() {
	tokenizer.next();
	expect(json_token::END_OBJECT);
}

bool TextJsonReader::readBoolean(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	return readBoolean();
}

int TextJsonReader::readInt(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	return readInt();
}

float TextJsonReader::readFloat(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	return readFloat();
}

double TextJsonReader::readDouble(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	return readDouble();
}

std::string TextJsonReader::readString(const std::string &key) {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	expect(key);
	tokenizer.next();
	expect(json_token::COLON);
	return readString();
}

bool TextJsonReader::readBoolean() {
	tokenizer.next();
	skipComma();
	switch (tokenizer.currentToken) {
		case json_token::TRUE:
			return true;
		case json_token::FALSE:
			return false;
		default:
			throw JsonException("Expected boolean");
	}
}

int TextJsonReader::readInt() {
	tokenizer.next();
	skipComma();
	expect(json_token::INTEGER);
	return std::stoi(tokenizer.current.str());
}

float TextJsonReader::readFloat() {
	tokenizer.next();
	skipComma();
	expect(json_token::REAL);
	return (float) std::stof(tokenizer.current.str());
}

double TextJsonReader::readDouble() {
	tokenizer.next();
	skipComma();
	expect(json_token::REAL);
	return std::stod(tokenizer.current.str());
}

std::string TextJsonReader::readString() {
	tokenizer.next();
	skipComma();
	expect(json_token::STRING);
	return tokenizer.current.str();
}

bool TextJsonReader::atArrayEnd(json_token::JsonToken alternateToken) {
	tokenizer.next();
	skipComma();
	if (tokenizer.currentToken == json_token::END_ARRAY)
		return true;
	if (tokenizer.currentToken == alternateToken)
		return false;
	throw JsonException("Expected to be at the end of an array.");
}
