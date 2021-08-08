#ifndef SETMUSICWINDOW_H
#define SETMUSICWINDOW_H

#include <QMainWindow>

namespace Ui {
class SetMusicWindow;
}

class SetMusicWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SetMusicWindow(QWidget *parent = 0);
    ~SetMusicWindow();

private:
    Ui::SetMusicWindow *ui;
};

#endif // SETMUSICWINDOW_H
