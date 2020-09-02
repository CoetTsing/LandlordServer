#ifndef WAITPAGE_H
#define WAITPAGE_H

#include <QMainWindow>
#include "gamepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WaitPage; }
QT_END_NAMESPACE

class WaitPage : public QMainWindow
{
    Q_OBJECT

public:
    WaitPage(QWidget *parent = nullptr);
    ~WaitPage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WaitPage *ui;
    GamePage* game;
};
#endif // WAITPAGE_H
