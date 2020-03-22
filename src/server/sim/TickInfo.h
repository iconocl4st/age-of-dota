//
// Created by thallock on 3/12/20.
//

#ifndef PLAYING_TICKINFO_H
#define PLAYING_TICKINFO_H


class TickInfo {
public:
	GameTime tickBegin;
	double timeDelta;

	GameTime getTickEnd() {
		return tickBegin + timeDelta;
	}
};


#endif //PLAYING_TICKINFO_H
