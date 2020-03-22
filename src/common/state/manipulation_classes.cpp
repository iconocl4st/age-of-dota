//
// Created by thallock on 3/13/20.
//

#include "manipulation_classes.h"

namespace aod::common::state {

static void maybeWrite(JsonWriter *writer, const std::string& field, boost::optional<double> value) {
	std::ostringstream hasField;
	hasField << "has-" << field;
	bool hasValue = !!value;
	writer->writeBoolean(hasField.str(), hasValue);
	if (hasValue) {
		writer->writeDouble(field, value.value());
	}
}

void EntityCreationParams::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeString("entity-spec", spec->name);
	writer->writeKey("location");
	location.send(writer);
	writer->writeDouble("orientation", orientation);
	writer->writeInt("player-number", player);

	writer->writeBoolean("has-capacity", !!capacity);
	if (capacity) {
		writer->writeInt("capacity", capacity.value());
	}
	maybeWrite(writer, "accuracy", accuracy);
	maybeWrite(writer, "collect-speed", collectSpeed);
	maybeWrite(writer, "deposit-speed", depositSpeed);
	maybeWrite(writer, "attack-speed", attackSpeed);
	maybeWrite(writer, "line-of-sight", lineOfSight);
	maybeWrite(writer, "max-health", maxHealth);
	maybeWrite(writer, "movement-speed", movementSpeed);
	maybeWrite(writer, "rotation-speed", rotationSpeed);

	writer->writeEndObject();
}

static void maybeRead(JsonReader *reader, const std::string& field, boost::optional<double>& value) {
	std::ostringstream hasField;
	hasField << "has-" << field;
	bool hasValue = reader->readBoolean(hasField.str());
	if (!hasValue) {
		value = boost::optional<double>{};
		return;
	}
	value = boost::optional<double>{reader->readDouble(field)};
}

void EntityCreationParams::parse(JsonReader *reader, std::shared_ptr<GameSpecification> specification) {
	reader->readBeginObject();
	std::string specName = reader->readString("entity-spec");
	spec = specification->findByName(specName);
	if (spec == nullptr) {
		throw std::runtime_error{"Unable to find entity specification"};
	}
	reader->readKey("location");
	location.parse(reader);
	orientation = reader->readDouble("orientation");
	player = reader->readInt("player-number");

	bool hasCapacity = reader->readBoolean("has-capacity");
	if (hasCapacity) {
		capacity = boost::optional<uint32_t>{reader->readInt("capacity")};
	} else {
		capacity = boost::optional<uint32_t>{};
	}

	maybeRead(reader, "accuracy", accuracy);
	maybeRead(reader, "collect-speed", collectSpeed);
	maybeRead(reader, "deposit-speed", depositSpeed);
	maybeRead(reader, "attack-speed", attackSpeed);
	maybeRead(reader, "line-of-sight", lineOfSight);
	maybeRead(reader, "max-health", maxHealth);
	maybeRead(reader, "movement-speed", movementSpeed);
	maybeRead(reader, "rotation-speed", rotationSpeed);


	reader->readEndObject();
}

void DamageApplicationResult::parse(JsonReader *reader) {
	reader->readBeginObject();
	newHealth = reader->readDouble("new-health");
	unitDies = reader->readBoolean("unit-dies");
	reader->readEndObject();
}

void DamageApplicationResult::send(JsonWriter *writer) const {
	writer->writeBeginObject();
	writer->writeDouble("new-health", newHealth);
	writer->writeBoolean("unit-dies", unitDies);
	writer->writeEndObject();
}


}