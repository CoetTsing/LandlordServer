#include "gamepage.h"
#include "ui_gamepage.h"
#include <QDebug>
#include <QTime>
#include <QtGlobal>
#include <algorithm>
#include <QMouseEvent>

GamePage::GamePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePage)
{
    ui->setupUi(this);
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->ask->hide();
    ui->noask->hide();
    ui->go->hide();
    ui->nogo->hide();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    setMouseTracking(true);
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 8888);
    connect(tcpServer, &QTcpServer::newConnection, this, &GamePage::connectToClient);
}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::connectToClient() {
    if (totalplayer == 1) {
        serverSocket1 = tcpServer->nextPendingConnection();
        totalplayer++;
        connect(serverSocket1, &QTcpSocket::readyRead, this, &GamePage::readInfo1);
        QString p2 = "02";
        serverSocket1->write(p2.toUtf8().data());
    } else if (totalplayer == 2) {
        serverSocket2 = tcpServer->nextPendingConnection();
        totalplayer++;
        connect(serverSocket2, &QTcpSocket::readyRead, this, &GamePage::readInfo2);
        QString p3 = "03";
        serverSocket2->write(p3.toUtf8().data());
        for (int i = 0; i < 3; i++) {
            labelsLord[i] = new QLabel(this);
            QString path = ":/cards/" + QString::number(1316) + ".png";
            QPixmap pic(path);
            pic.scaled(72, 97, Qt::KeepAspectRatio);
            labelsLord[i]->setScaledContents(true);
            labelsLord[i]->setPixmap(pic);
            labelsLord[i]->resize(72, 97);
            labelsLord[i]->move(464 + i * 100, 30);
            labelsLord[i]->show();
        }
        deal();
        nowPlayer = qrand() % 3 + 1;
        hide();
        QString now = "2" + QString::number(nowPlayer);
        serverSocket1->write(now.toUtf8().data());
        serverSocket2->write(now.toUtf8().data());
        ask();
    }
}

void GamePage::readInfo1() {
    QByteArray tmp = serverSocket1->readAll();
    qDebug() <<tmp;
    while (tmp.size() != 0) {
        if (*tmp.begin() == '2') {
            nowPlayer = *(tmp.begin() + 1) - '0';
            hide();
            lord = "";
            for (auto i = tmp.begin() + 2; i != tmp.end(); i++)
                lord += *i;
            QString now = "2" + QString::number(nowPlayer) + lord;
            serverSocket1->write(now.toUtf8().data());
            serverSocket2->write(now.toUtf8().data());
            ask();
            tmp = tmp.right(tmp.size() - 2 - lord.size());
        } else if (*tmp.begin() == '3') {
            lordplayer = *(tmp.begin() + 1) - '0';
            heIsLord();
            tmp = tmp.right(tmp.size() - 2);
        }
    }
}

void GamePage::readInfo2() {
    QByteArray tmp = serverSocket2->readAll();
    qDebug() <<tmp;
    while (tmp.size() != 0) {
        if (*tmp.begin() == '2') {
            nowPlayer = *(tmp.begin() + 1) - '0';
            hide();
            lord = "";
            for (auto i = tmp.begin() + 2; i != tmp.end(); i++)
                lord += *i;
            QString now = "2" + QString::number(nowPlayer) + lord;
            serverSocket1->write(now.toUtf8().data());
            serverSocket2->write(now.toUtf8().data());
            ask();
            tmp = tmp.right(tmp.size() - 2 - lord.size());
        } else if (*tmp.begin() == '3') {
            lordplayer = *(tmp.begin() + 1) - '0';
            heIsLord();
            tmp = tmp.right(tmp.size() - 2);
        }
    }
}

void GamePage::deal() {
    for (int i = 0; i < 54; i++) {
        int a = rollcard();
        if (a == 52)
            cardsAll.push_back(1215);
        else if (a == 53)
            cardsAll.push_back(1225);
        else {
            if (a < 13)
                cardsAll.push_back(1000 + (a % 13 + 1) * 10);
            else if (a >= 13 && a < 26)
                cardsAll.push_back(1000 + (a % 13 + 1) * 10 + 1);
            else if (a >= 26 && a < 39)
                cardsAll.push_back(1000 + (a % 13 + 1) * 10 + 2);
            else
                cardsAll.push_back(1000 + (a % 13 + 1) * 10 + 3);
        }
    }
    for (int i = 0; i < 3; i++)
        cardsLord.push_back(cardsAll[i]);
    for (int i = 3; i < 20; i++)
        cardsP1.push_back(cardsAll[i]);
    std::sort(cardsP1.begin(), cardsP1.end(), cmp);
    showCards();
    for (int i = 20; i < 37; i++)
        cardsP2.push_back(cardsAll[i]);
    std::sort(cardsP2.begin(), cardsP2.end(), cmp);
    QString cardsToP2 = "1";
    for (auto x: cardsP2)
        cardsToP2 += QString::number(x);
    serverSocket1->write(cardsToP2.toUtf8().data());
    for (int i = 37; i < 54; i++)
        cardsP3.push_back(cardsAll[i]);
    std::sort(cardsP3.begin(), cardsP3.end(), cmp);
    QString cardsToP3 = "1";
    for (auto x: cardsP3)
        cardsToP3 += QString::number(x);
    serverSocket2->write(cardsToP3.toUtf8().data());
}

int GamePage::rollcard() {
    int a = qrand() % 54;
    if (!have[a]) {
        have[a] = true;
        return a;
    } else
        return rollcard();
}

bool GamePage::cmp(int a, int b) {
    return a > b;
}

void GamePage::showCardsLord() {
    for (int i = 0; i < 3; i++) {
        delete labelsLord[i];
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
    for (int i = 0; i < 20; i++) {
        if (labels[i] != nullptr)
            delete labels[i];
        labels[i] = nullptr;
    }
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

void GamePage::showCardsCenter() {
    for (int i = 0; i < 10; i++) {
        if (labelsCenter[i] != nullptr)
            delete labelsCenter[i];
        labelsCenter[i] = nullptr;
    }
    for (int i = 0; i < cardsCenter.size(); i++) {
        labelsCenter[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cardsCenter[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(143, 193, Qt::KeepAspectRatio);
        labelsCenter[i]->setScaledContents(true);
        labelsCenter[i]->setText(QString::number(i));
        labelsCenter[i]->setPixmap(pic);
        labelsCenter[i]->resize(143, 193);
        labelsCenter[i]->move(400 + i * 40, 250);
        labelsCenter[i]->show();
    }
}

void GamePage::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    int n = 21;
    if (x >= 200 && x <= 200 + 40 * cardsP1.size() && y <= 743 && y >= 550)
        n = (x - 200) / 40;
    cardsChosen[n] = !cardsChosen[n];
    for (int i = 0; i < cardsP1.size(); i++) {
        if (cardsChosen[i])
            labels[i]->move(200 + i * 40, 530);
        else
           labels[i]->move(200 + i * 40, 550);
    }
}

void GamePage::hide() {
    if (nowPlayer == 1) {
        ui->label_4->show();
        ui->label_5->hide();
        ui->label_6->hide();
    } else if (nowPlayer == 2) {
        ui->label_5->show();
        ui->label_4->hide();
        ui->label_6->hide();
    } else {
        ui->label_6->show();
        ui->label_4->hide();
        ui->label_5->hide();
    }
}

void GamePage::ask() {
    if (nowPlayer == 1) {
        ui->ask->show();
        ui->noask->show();
    }
}

void GamePage::whoIsLord() {
    if (lord[2] == '1')
        lordplayer = 1;
    else {
        if (lord[1] == '3')
            lordplayer = 3;
        else
            lordplayer = 2;
    }
    heIsLord();
}

void GamePage::heIsLord() {
    nowPlayer = lordplayer;
    previousPlayer = lordplayer;
    hide();
    QString iAmLord = "3" + QString::number(lordplayer);
    serverSocket1->write(iAmLord.toUtf8().data());
    serverSocket2->write(iAmLord.toUtf8().data());
    QString cardsTo = "4";
    for (auto x: cardsLord)
        cardsTo += QString::number(x);
    serverSocket1->write(cardsTo.toUtf8().data());
    serverSocket2->write(cardsTo.toUtf8().data());
    showCardsLord();
    if (lordplayer == 1) {
        for (int i = 0; i < 3; i++)
            cardsP1.push_back(cardsLord[i]);
        std::sort(cardsP1.begin(), cardsP1.end(), cmp);
        showCards();
    }
    QString toGo = "5" + QString::number(lordplayer) + QString::number(lordplayer) + "0";
    serverSocket1->write(toGo.toUtf8().data());
    serverSocket2->write(toGo.toUtf8().data());
    go();
}

void GamePage::go() {
    if (nowPlayer == 1) {
        ui->go->show();
        ui->nogo->setEnabled(true);
        ui->nogo->show();
    }
    if (previousPlayer == 1) {
        ui->nogo->setEnabled(false);
    }
}

void GamePage::check() {
    QVector<int> c;
    for (auto x: cardsToGo)
        c.push_back(x / 10);
    qDebug() << c;
    if (c.size() == 1)
        type = 1;
    else if (c.size() == 2) {
        if (c[0]== c[1])
            type = 2;
        else if (c[0] == 122 && c[1] == 121)
            type = 9;
        else
            type = 0;
    }
    else if (c.size() == 3) {
        if (c[0] == c[1] && c[1] == c[2])
            type = 3;
        else
            type = 0;
    }
    else if (c.size() == 4) {
        if (c[0] == c[1] && c[1] == c[2] && c[2] == c[3])
            type = 8;
        else if ((c[0] == c[1] && c[1] == c[2] && c[2] != c[3])
                 || (c[1] == c[2] && c[2] == c[3] && c[0] != c[1]))
            type = 3;
        else
            type = 0;
    }
    else if (c.size() == 5) {
        if ((c[0] == c[1] && c[1] == c[2] && c[2] != c[3] && c[3] == c[4])
                || (c[1] != c[2] && c[2] == c[3] && c[0] == c[1] && c[3] == c[4]))
            type = 3;
        else if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1)
            type = 4;
        else
            type = 0;
    }
    else if (c.size() == 6) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5])
            type = 5;
        else if ((c[0] == c[1] && c[1] == c[2] && c[2] == c[3] && c[4] == c[5])
                 || (c[0] == c[1] && c[2] == c[3] && c[3] == c[4] && c[4] == c[5]))
            type = 6;
        else if (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5])
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 7) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1)
            type = 4;
        else
            type = 0;
    }
    else if (c.size() == 8) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7])
            type = 5;
        else if ((c[2] == c[3] && c[3] == c[4] && c[4] - c[5] == 1 && c[5] == c[6] && c[6] == c[7])
                 || (c[1] == c[2] && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] == c[6])
                 || (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5]))
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 9) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1 && c[7] == c[8])
            type = 4;
        else if (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7] && c[7] == c[8])
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 10) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1 && c[7] - c[8] == 1 && c[8] - c[9] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7] && c[7] - c[8] == 1 && c[8] == c[9])
            type = 5;
        else if ((c[0] == c[1] && c[2] == c[3] && c[4] == c[5] && c[5] == c[6] && c[6] - c[7] == 1 && c[7] == c[8] && c[8] == c[9])
                 || (c[0] == c[1] && c[2] == c[3] && c[3] == c[4] && c[4] - c[5] == 1 && c[5] == c[6] && c[6] == c[7] && c[8] == c[9])
                 || (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5] && c[6] == c[7] && c[8] == c[9]))
            type = 7;
        else
            type = 0;
    }
}

bool GamePage::test() {
    return true;
}

void GamePage::on_ask_clicked()
{
    lord += "1";
    if (lord.size() == 3)
        whoIsLord();
    else {
        nowPlayer = 2;
        hide();
        QString now = "2" + QString::number(nowPlayer) + lord;
        serverSocket1->write(now.toUtf8().data());
        serverSocket2->write(now.toUtf8().data());
    }
    ui->ask->hide();
    ui->noask->hide();
}

void GamePage::on_noask_clicked()
{
    lord += "0";
    if (lord.size() == 3)
        whoIsLord();
    else {
        nowPlayer = 2;
        hide();
        QString now = "2" + QString::number(nowPlayer) + lord;
        serverSocket1->write(now.toUtf8().data());
        serverSocket2->write(now.toUtf8().data());
    }
    ui->ask->hide();
    ui->noask->hide();
}

void GamePage::on_go_clicked()
{
    cardsToGo.clear();
    for (int i = 0; i < cardsP1.size(); i++)
        if (cardsChosen[i])
            cardsToGo.push_back(cardsP1[i]);
    check();
    qDebug() << type;
    if (type != 0) {
        if (test()) {

            for (auto x: cardsToGo)
                cardsCenter.push_back(x);
            showCardsCenter();
            cardsCenter.clear();
            QVector<int> tmp;
            for (int i = 0; i < cardsP1.size(); i++) {
                if (!cardsChosen[i])
                    tmp.push_back(cardsP1[i]);
                else
                    cardsChosen[i] = false;
            }
            cardsP1.clear();
            for (auto x: tmp)
                cardsP1.push_back(x);
            showCards();
        }
    }
}

void GamePage::on_nogo_clicked()
{

}
