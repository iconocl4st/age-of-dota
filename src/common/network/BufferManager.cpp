//
// Created by thallock on 2/18/20.
//

#include "BufferManager.h"

#include <cstdlib>

#define BUFFER_SIZE 1024
#define MAX_NUM_BUFFERS 20

Buffer *BufferManager::getBuffer() {
    Buffer *buffer{nullptr};
    std::unique_lock<std::recursive_mutex> lock{mutex};

    if (freeBufferObjects.empty()) {
        buffer = new Buffer;
        buffer->data = (char *)std::malloc(BUFFER_SIZE);
        buffer->size = BUFFER_SIZE;
        usedBufferObjects.push_back(buffer);
        return buffer;
    }

    buffer = freeBufferObjects.front();
    freeBufferObjects.pop_front();
    usedBufferObjects.push_front(buffer);
    return buffer;
}


void BufferManager::doneWithBuffer(Buffer *buffer) {
    std::unique_lock<std::recursive_mutex> lock{mutex};
    usedBufferObjects.remove(buffer);
    freeBufferObjects.push_front(buffer);

    while (freeBufferObjects.size() > MAX_NUM_BUFFERS) {
        Buffer *freeBuffer = freeBufferObjects.back();
        freeBufferObjects.pop_back();
        std::free(freeBuffer->data);
        delete buffer;
    }
}

BufferManager *manager{nullptr};

BufferManager *getBufferManager() {
    if (manager == nullptr) {
        manager = new BufferManager;
    }
    return manager;
}