#ifndef LOBBYBROWSER_H
#define LOBBYBROWSER_H

#include <string>
#include <memory>
#include <set>

#include <QMainWindow>
#include <QListWidget>

#include "lobbybrowserhandler.h"
#include  "lobbybrowserconnection.h"

namespace Ui {
class LobbyBrowser;
}

class LobbyBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit LobbyBrowser(QWidget *parent = 0, LobbyBrowserHandler *handler = nullptr);
    ~LobbyBrowser();

    void setConnection(const LobbyBrowserConnection& cnctn);

    void setLobbies(const std::set<std::string>& lobbyNames);

private slots:
    void on_refreshButton_clicked();

    void on_joinButton_clicked();

    void on_leaveButton_clicked();

    void on_launchButton_clicked();

    void on_readyButton_clicked();

    void on_notReady_clicked();

    void on_lobbyList_itemClicked(QListWidgetItem *item);

    void on_createLobbyButton_clicked();

private:
    Ui::LobbyBrowser *ui;
    LobbyBrowserHandler *handler;

    bool selected{false};
    std::string currentlySelected;
};

#endif // LOBBYBROWSER_H
