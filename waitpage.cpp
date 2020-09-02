#include "waitpage.h"
#include "ui_waitpage.h"

WaitPage::WaitPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WaitPage)
{
    ui->setupUi(this);
}

WaitPage::~WaitPage()
{
    delete ui;
    delete game;
}


void WaitPage::on_pushButton_clicked()
{
    game = new GamePage;
    this->close();
    game->show();
}
