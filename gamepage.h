#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>

namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

private:
    Ui::GamePage *ui;
};

#endif // GAMEPAGE_H
