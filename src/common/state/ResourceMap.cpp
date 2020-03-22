//
// Created by thallock on 3/7/20.
//

#include "ResourceMap.h"

void ResourceMap::operator=(const ResourceMap &map) {
    quantities.clear();
    for (auto &pair : map.quantities) {
        quantities[pair.first] = pair.second;
    }
}
