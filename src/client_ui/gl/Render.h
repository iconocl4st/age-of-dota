//
// Created by thallock on 2/6/20.
//

#ifndef PLAYING_RENDER_H
#define PLAYING_RENDER_H

#include <QtGui/QOpenGLExtraFunctions>

namespace aod::client::app {
class UiClientContext;
}

namespace aod::client::graphics {

// THE MAIN METHOD
void render(aod::client::app::UiClientContext& context, QOpenGLExtraFunctions *pFunctions);

}

#endif //PLAYING_RENDER_H
