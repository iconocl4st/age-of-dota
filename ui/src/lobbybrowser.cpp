#include "lobbybrowser.h"
#include "ui_lobbybrowser.h"

#include <iostream>

LobbyBrowser::LobbyBrowser(QWidget *parent, LobbyBrowserHandler *handler) :
    QMainWindow(parent),
    ui(new Ui::LobbyBrowser),
    handler{handler}
{
    ui->setupUi(this);
    LobbyBrowserConnection defaultConnection;
    setConnection(defaultConnection);
}

LobbyBrowser::~LobbyBrowser()
{
    delete ui;
}

void LobbyBrowser::setLobbies(const std::set<std::string>& lobbyNames) {
    ui->lobbyList->clear();
    for (auto & lobbyName : lobbyNames) {
        ui->lobbyList->addItem(lobbyName.c_str());
    }
    selected = false;
    currentlySelected = "";
}

void LobbyBrowser::setConnection(const LobbyBrowserConnection& cnctn) {
    ui->refreshButton->setEnabled(!cnctn.connected);
    ui->lobbyList->setEnabled(!cnctn.connected);

    ui->joinButton->setEnabled(!cnctn.connected && selected);
    ui->leaveButton->setEnabled(cnctn.connected);
    ui->readyButton->setEnabled(cnctn.connected && !cnctn.ready);
    ui->notReady->setEnabled(cnctn.connected && cnctn.ready);
    ui->launchButton->setEnabled(cnctn.connected && cnctn.allReady);
}


void LobbyBrowser::on_joinButton_clicked()
{
    handler->connect(currentlySelected);
}

void LobbyBrowser::on_leaveButton_clicked()
{
    handler->disconnect();
}

void LobbyBrowser::on_launchButton_clicked()
{
    handler->launch();
}

void LobbyBrowser::on_readyButton_clicked()
{
    handler->setReady(true);
}
void LobbyBrowser::on_notReady_clicked()
{
    handler->setReady(false);
}


void LobbyBrowser::on_refreshButton_clicked()
{
    handler->refreshLobbies();
}

void LobbyBrowser::on_lobbyList_itemClicked(QListWidgetItem *item)
{
    selected = true;
    currentlySelected = item->text().toStdString();
    ui->lobbyName->setText(item->text());
    handler->requestStatusUpdate();
}

void LobbyBrowser::on_createLobbyButton_clicked()
{
    handler->createLobby(ui->newLobbyNameEditor->text().toStdString());
}
