#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, GameWindowAdapter *adapter) :
    QOpenGLWidget(parent),
    ui(new Ui::GameWindow),
    adapter(adapter)
{
    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}


void GameWindow::initializeGL() {
    adapter->initializeGL();
}
void GameWindow::paintGL() {
    adapter->paintGL();
}

void GameWindow::resizeGL(int width, int height) {
    adapter->resizeGL(width, height);
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    adapter->keyPressEvent();
}

int GameWindow::getMouseButton(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        return MOUSE_LEFT_BUTTON;
    }
    if (event->button() == Qt::RightButton) {
        return MOUSE_RIGHT_BUTTON;
    }
    if (event->button() == Qt::MiddleButton) {
        return MOUSE_MIDDLE_BUTTON;
    }
    return -1;
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    adapter->mousePressEvent(
        event->x(),
        event->y(),
        getMouseButton(event)
    );
}

void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    adapter->mouseReleaseEvent(
        event->x(),
        event->y(),
        getMouseButton(event)
    );
}

void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    adapter->mouseMoveEvent(event->x(), event->y());
}

void GameWindow::wheelEvent(QWheelEvent *event) {
    adapter->mouseWheel(
        event->angleDelta().y(),
        event->x(),
        event->y()
    );
}

void GameWindow::repaint() {
    this->update();
}
