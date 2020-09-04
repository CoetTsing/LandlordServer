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
    void showCardsCenter();
    void mousePressEvent(QMouseEvent *);
    void hide();
    void ask();
    void whoIsLord();
    void heIsLord();
    void go();
    void check();
    bool test();

private:
    Ui::GamePage *ui;
    QTcpServer* tcpServer = nullptr;
    QTcpSocket* serverSocket1 = nullptr;
    QTcpSocket* serverSocket2 = nullptr;
    int totalplayer = 1;
    QString lord = "";
    int lordplayer;
    int playerId = 1;
    int nowPlayer;
    int previousPlayer;
    bool have[54] = {0};
    QVector<int> cardsAll;
    QVector<int> cardsLord;
    QVector<int> cardsCenter;
    QVector<int> cardsP1;
    QVector<int> cardsP2;
    QVector<int> cardsP3;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);
    QVector<QLabel*> labelsCenter = QVector<QLabel*>(10);
    bool cardsChosen[21] = {0};
    QVector<int> cardsToGo;
    int type;
    int typeRecived;
    int num;

private slots:
    void connectToClient();
    void readInfo1();
    void readInfo2();
    void on_ask_clicked();
    void on_noask_clicked();
    void on_go_clicked();
    void on_nogo_clicked();
};

#endif // GAMEPAGE_H
