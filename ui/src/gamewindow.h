#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "gamewindowadapter.h"

#include <QWidget>
#include <QOpenGLWidget>
#include <QWheelEvent>

namespace Ui {
class GameWindow;
}

class GameWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0, GameWindowAdapter *adapter = 0);
    ~GameWindow();

    void repaint();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

private:
    Ui::GameWindow *ui;
    GameWindowAdapter *adapter;

    int getMouseButton(QMouseEvent *event);
};

#endif // GAMEWINDOW_H
