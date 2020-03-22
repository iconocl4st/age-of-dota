#ifndef PLAYEROPTIONS_H
#define PLAYEROPTIONS_H

#include <QWidget>

namespace Ui {
class PlayerOptions;
}

class PlayerOptions : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerOptions(QWidget *parent = 0);
    ~PlayerOptions();

private:
    Ui::PlayerOptions *ui;
};

#endif // PLAYEROPTIONS_H
