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
}


void WaitPage::on_pushButton_clicked()
{
    game = new GamePage;
    this->close();
    game->show();
}
