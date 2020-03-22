#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H


class GameHandler
{
public:
    GameHandler();

    virtual void initializeGL() = 0;
    virtual void paintGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
};

#endif // GAMEHANDLER_H
