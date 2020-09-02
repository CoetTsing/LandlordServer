#ifndef WAITPAGE_H
#define WAITPAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WaitPage; }
QT_END_NAMESPACE

class WaitPage : public QMainWindow
{
    Q_OBJECT

public:
    WaitPage(QWidget *parent = nullptr);
    ~WaitPage();

private:
    Ui::WaitPage *ui;
};
#endif // WAITPAGE_H
