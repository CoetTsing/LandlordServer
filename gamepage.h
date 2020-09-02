#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QLabel>

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

private:
    Ui::GamePage *ui;
    bool have[54] = {0};
    QVector<int> cardsAll;
    QVector<int> cardsLord;
    QVector<int> cardsP1;
    QVector<int> cardsP2;
    QVector<int> cardsP3;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);
};

#endif // GAMEPAGE_H
