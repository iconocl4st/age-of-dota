//
// Created by thallock on 3/9/20.
//

#ifndef PLAYING_COLOR_H
#define PLAYING_COLOR_H

namespace aod::client::graphics {

class Color {
public:
	float red;
	float green;
	float blue;

	Color(float r, float g, float b) : red{r}, green{g}, blue{b} {}

	Color() : Color{0, 0, 0} {}
};

}

#endif //PLAYING_COLOR_H
