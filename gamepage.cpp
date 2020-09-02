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
    setMouseTracking(true);
    deal();
    showCardsLord();
    showCards();
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

void GamePage::showCardsLord() {
    for (int i = 0; i < 3; i++) {
        labelsLord[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cardsLord[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(72, 97, Qt::KeepAspectRatio);
        labelsLord[i]->setScaledContents(true);
        labelsLord[i]->setPixmap(pic);
        labelsLord[i]->resize(72, 97);
        labelsLord[i]->move(464 + i * 100, 30);
        labelsLord[i]->show();
    }
}

void GamePage::showCards() {
    for (int i = 0; i < cardsP1.size(); i++) {
        labels[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cardsP1[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(143, 193, Qt::KeepAspectRatio);
        labels[i]->setScaledContents(true);
        labels[i]->setText(QString::number(i));
        labels[i]->setPixmap(pic);
        labels[i]->resize(143, 193);
        labels[i]->move(200 + i * 40, 550);
        labels[i]->show();
    }
}

void GamePage::mousePressEvent(QMouseEvent *) {

}
