#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();
    void deal();
    int rollcard();
    static bool cmp(int a, int b);
    void showCardsLord();
    void showCards();
    void mousePressEvent(QMouseEvent *);
    void go();

private:
    Ui::GamePage *ui;
    QTcpServer* tcpServer = nullptr;
    QTcpSocket* serverSocket1 = nullptr;
    QTcpSocket* serverSocket2 = nullptr;
    int totalplayer = 1;
    bool lordExist = false;
    int lordplayer;
    int playerId = 1;
    int nowplayer;
    bool have[54] = {0};
    QVector<int> cardsAll;
    QVector<int> cardsLord;
    QVector<int> cardsP1;
    QVector<int> cardsP2;
    QVector<int> cardsP3;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);

private slots:
    void connectToClient();
    void readInfo1();
    void readInfo2();
};

#endif // GAMEPAGE_H
