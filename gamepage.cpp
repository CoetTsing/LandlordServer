#include "gamepage.h"
#include "ui_gamepage.h"
#include <QDebug>
#include <QTime>
#include <QtGlobal>

GamePage::GamePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePage)
{
    ui->setupUi(this);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    deal();
}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::deal() {
    for (int i = 0; i < 54; i++) {
        int a = rollcard();
        if (a == 52)
            cardsAll.push_back(215);
        else if (a == 53)
            cardsAll.push_back(225);
        else {
            if (a < 13)
                cardsAll.push_back((a % 13 + 1) * 10);
            else if (a >= 13 && a < 26)
                cardsAll.push_back((a % 13 + 1) * 10 + 1);
            else if (a >= 26 && a < 39)
                cardsAll.push_back((a % 13 + 1) * 10 + 2);
            else
                cardsAll.push_back((a % 13 + 1) * 10 + 3);
        }
    }
    for (int i = 0; i < 3; i++)
        cardsLord.push_back(cardsAll[i]);
    for (int i = 3; i < 20; i++)
        cardsP1.push_back(cardsAll[i]);
    for (int i = 20; i < 37; i++)
        cardsP2.push_back(cardsAll[i]);
    for (int i = 37; i < 54; i++)
        cardsP3.push_back(cardsAll[i]);
}

int GamePage::rollcard() {
    int a = qrand() % 54;
    if (!have[a]) {
        have[a] = true;
        return a;
    } else
        return rollcard();
}
