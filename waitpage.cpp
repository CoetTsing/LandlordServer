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

