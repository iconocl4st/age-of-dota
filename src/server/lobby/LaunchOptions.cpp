//
// Created by thallock on 2/15/20.
//

#include "LaunchOptions.h"

void LaunchOptions::debug(JsonWriter &writer) {
    writer.writeBeginObject();
//    writer.writeInt("width", width);
//    writer.writeInt("height", height);
    writer.writeEndObject();
}
