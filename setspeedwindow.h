#ifndef SETSPEEDWINDOW_H
#define SETSPEEDWINDOW_H

#include <QMainWindow>

namespace Ui {
class SetSpeedWindow;
}

class SetSpeedWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SetSpeedWindow(int s,QWidget *parent = 0);
    ~SetSpeedWindow();

signals:
    void  sendData(int);
    //void destroyed();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SetSpeedWindow *ui;
};

#endif // SETSPEEDWINDOW_H
