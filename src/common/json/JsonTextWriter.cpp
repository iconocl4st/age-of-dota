//
// Created by thallock on 2/14/20.
//

#include  "JsonTextWriter.h"
#include "JsonException.h"

#include <sstream>

void TextJsonWriter::indent() {
	for (int i = 0; i < indentation; i++)
		out.write("\t");
}

void TextJsonWriter::writeComma() {
	if (requiresComma)
		out.write(",");
	out.write("\n");
	requiresComma = false;
}

void TextJsonWriter::writeKeyOnly(const std::string &key) {
	out.write("\"");
	out.write(key);
	out.write("\": ");
}

TextJsonWriter::TextJsonWriter(OutputBufferWrapper& out) :
	out{out},
	requiresComma{false},
	indentation{0} {}

void TextJsonWriter::writeKey(const std::string &key) {
	writeComma();
	indent();
	writeKeyOnly(key);
	requiresComma = false;
}

void TextJsonWriter::writeBeginArray(const std::string &key) {
	writeComma();
	indent();
	writeKeyOnly(key);
	out.write("[");
	++indentation;
	requiresComma = false;
}

void TextJsonWriter::writeBeginArray() {
	writeComma();
	indent();
	out.write("[");
	++indentation;
	requiresComma = false;
}

void TextJsonWriter::writeEndArray() {
	--indentation;
	out.write("\n");
	indent();
	out.write("]");
	requiresComma = true;
}

void TextJsonWriter::writeBeginObject(const std::string &key) {
	writeComma();
	indent();
	writeKeyOnly(key);
	out.write("{");
	++indentation;
	requiresComma = false;
}

void TextJsonWriter::writeBeginObject() {
	writeComma();
	indent();
	out.write("{");
	++indentation;
	requiresComma = false;
}

void TextJsonWriter::writeEndObject() {
	--indentation;
	out.write("\n");
	indent();
	out.write("}");
	requiresComma = true;
}

void TextJsonWriter::writeBoolean(const std::string &key, bool val) {
	writeComma();
	indent();
	writeKeyOnly(key);
	if (val) {
		out.write("true");
	} else {
		out.write("false");
	}
	requiresComma = true;
}

void TextJsonWriter::writeInt(const std::string &key, int val) {
	writeComma();
	indent();
	writeKeyOnly(key);
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeFloat(const std::string &key, float val) {
	writeComma();
	indent();
	writeKeyOnly(key);
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeDouble(const std::string &key, double val) {
	writeComma();
	indent();
	writeKeyOnly(key);
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeString(const std::string &key, const std::string &val) {
	std::size_t found = val.find("\"");
	if (found != std::string::npos)
		throw JsonException("json string con not contain the character '\"'");

	writeComma();
	indent();
	writeKeyOnly(key);
	out.write("\"");
	out.write(val);
	out.write("\"");
	requiresComma = true;
}

void TextJsonWriter::writeNull(const std::string &key) {
	writeComma();
	indent();
	writeKeyOnly(key);
	out.write("null");
	requiresComma = true;
}

void TextJsonWriter::writeBoolean(bool val) {
	writeComma();
	indent();
	if (val) {
		out.write("true");
	} else {
		out.write("false");
	}
	requiresComma = true;
}

void TextJsonWriter::writeInt(int val) {
	writeComma();
	indent();
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeFloat(float val) {
	writeComma();
	indent();
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeDouble(double val) {
	writeComma();
	indent();
	std::ostringstream b;
	b << val;
	out.write(b.str());
	requiresComma = true;
}

void TextJsonWriter::writeString(const std::string &val) {
	std::size_t found = val.find("\"");
	if (found != std::string::npos)
		throw JsonException("json string cannot contain the character '\"'");

	writeComma();
	indent();
	out.write("\"");
	out.write(val);
	out.write("\"");
	requiresComma = true;
}

void TextJsonWriter::writeNull() {
	writeComma();
	indent();
	out.write("null");
	requiresComma = true;
}

void TextJsonWriter::flush() {
	out.flush();
}

