//
// Created by thallock on 3/7/20.
//

#ifndef PLAYING_RESOURCEMAP_H
#define PLAYING_RESOURCEMAP_H

#include <map>

#include "definitions.h"

class ResourceMap {
public:
    std::map<ResourceId, uint32_t> quantities;

    void operator=(const ResourceMap& map);
//    void operator-=(const ResourceMap& map);
//    void operator+=(const ResourceMap& map);

};


#endif //PLAYING_RESOURCEMAP_H
