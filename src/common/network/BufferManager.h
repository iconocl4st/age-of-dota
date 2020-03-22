//
// Created by thallock on 2/18/20.
//

#ifndef PLAYING_BUFFERMANAGER_H
#define PLAYING_BUFFERMANAGER_H

#include <list>
#include <mutex>

class Buffer {
public:
    std::size_t size;
    char *data;
};

class BufferManager {
private:
    std::recursive_mutex mutex;
    std::list<Buffer*> freeBufferObjects;
    std::list<Buffer*> usedBufferObjects;
public:
    Buffer *getBuffer();
    void doneWithBuffer(Buffer *buffer);
};

// Maybe not static?
BufferManager *getBufferManager();


#endif //PLAYING_BUFFERMANAGER_H
