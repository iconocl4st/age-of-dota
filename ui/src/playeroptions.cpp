#include "playeroptions.h"
#include "ui_playeroptions.h"

PlayerOptions::PlayerOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerOptions)
{
    ui->setupUi(this);
}

PlayerOptions::~PlayerOptions()
{
    delete ui;
}
