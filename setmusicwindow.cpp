#include "setmusicwindow.h"
#include "ui_setmusicwindow.h"

SetMusicWindow::SetMusicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SetMusicWindow)
{
    ui->setupUi(this);
}

SetMusicWindow::~SetMusicWindow()
{
    delete ui;
}
