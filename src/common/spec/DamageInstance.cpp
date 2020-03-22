//
// Created by thallock on 3/13/20.
//

#include "DamageInstance.h"

void DamageInstance::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeInt("from", from);
	writer->writeKey("origin");
	origin.send(writer);
	writer->writeInt("damage-type", damageType);
	writer->writeDouble("amount", damageAmount);
	writer->writeEndObject();
}

void DamageInstance::parse(JsonReader *reader) {
	reader->readBeginObject();
	from = reader->readInt("from");
	reader->readKey("origin");
	origin.parse(reader);
	damageType = (damage_types::DamageType) reader->readInt("damage-type");
	damageAmount = reader->readDouble("amount");
	reader->readEndObject();
}
