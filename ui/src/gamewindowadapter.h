#ifndef GAMEWINDOWADAPTER_H
#define GAMEWINDOWADAPTER_H

#define MOUSE_LEFT_BUTTON 0
#define MOUSE_MIDDLE_BUTTON 1
#define MOUSE_RIGHT_BUTTON 2

class GameWindowAdapter
{
public:
    GameWindowAdapter();
    virtual ~GameWindowAdapter() = default;

    virtual void initializeGL() = 0;
    virtual void paintGL() = 0;
    virtual void resizeGL(int width, int height) = 0;

    virtual void keyPressEvent() = 0;
    virtual void mousePressEvent(int x, int y, int button) = 0;
    virtual void mouseReleaseEvent(int x, int y, int button) = 0;
    virtual void mouseMoveEvent(int x, int y) = 0;
    virtual void mouseWheel(int amount, int x, int y) = 0;
};

#endif // GAMEWINDOWADAPTER_H
