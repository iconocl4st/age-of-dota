//
// Created by thallock on 3/11/20.
//

#ifndef PLAYING_CLOSEONEXIT_H
#define PLAYING_CLOSEONEXIT_H


class CloseOnExit {
private:
	int fd;
public:
	CloseOnExit(int fd);
	~CloseOnExit();
};


#endif //PLAYING_CLOSEONEXIT_H
