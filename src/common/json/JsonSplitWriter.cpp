//
// Created by thallock on 2/14/20.
//


#include "JsonSplitWriter.h"

JsonSplitWriter::JsonSplitWriter(JsonWriter& w1, JsonWriter& w2) : w1{w1}, w2{w2} {}

void JsonSplitWriter::writeKey(const std::string &key) {
	w1.writeKey(key);
	w2.writeKey(key);
}

void JsonSplitWriter::writeBeginArray(const std::string &key) {
	w1.writeBeginArray(key);
	w2.writeBeginArray(key);
}

void JsonSplitWriter::writeBeginArray() {
	w1.writeBeginArray();
	w2.writeBeginArray();
}

void JsonSplitWriter::writeEndArray() {
	w1.writeEndArray();
	w2.writeEndArray();
}

void JsonSplitWriter::writeBeginObject(const std::string &key) {
	w1.writeBeginObject(key);
	w2.writeBeginObject(key);
}

void JsonSplitWriter::writeBeginObject() {
	w1.writeBeginObject();
	w2.writeBeginObject();
}

void JsonSplitWriter::writeEndObject() {
	w1.writeEndObject();
	w2.writeEndObject();
}

void JsonSplitWriter::writeBoolean(const std::string &key, bool val) {
	w1.writeBoolean(key, val);
	w2.writeBoolean(key, val);
}

void JsonSplitWriter::writeInt(const std::string &key, int val) {
	w1.writeInt(key, val);
	w2.writeInt(key, val);
}

void JsonSplitWriter::writeFloat(const std::string &key, float val) {
	w1.writeFloat(key, val);
	w2.writeFloat(key, val);
}

void JsonSplitWriter::writeDouble(const std::string &key, double val) {
	w1.writeDouble(key, val);
	w2.writeDouble(key, val);
}

void JsonSplitWriter::writeString(const std::string &key, const std::string &val) {
	w1.writeString(key, val);
	w2.writeString(key, val);
}

void JsonSplitWriter::writeNull(const std::string &key) {
	w1.writeNull(key);
	w2.writeNull(key);
}

void JsonSplitWriter::writeBoolean(bool val) {
	w1.writeBoolean(val);
	w2.writeBoolean(val);
}

void JsonSplitWriter::writeInt(int val) {
	w1.writeInt(val);
	w2.writeInt(val);
}

void JsonSplitWriter::writeFloat(float val) {
	w1.writeFloat(val);
	w2.writeFloat(val);
}

void JsonSplitWriter::writeDouble(double val) {
	w1.writeDouble(val);
	w2.writeDouble(val);
}

void JsonSplitWriter::writeString(const std::string &val) {
	w1.writeString(val);
	w2.writeString(val);
}

void JsonSplitWriter::writeNull() {
	w1.writeNull();
	w2.writeNull();
}

void JsonSplitWriter::flush() {
	w1.flush();
	w2.flush();
}
