#include "setspeedwindow.h"
#include "ui_setspeedwindow.h"
#include "header.h"

SetSpeedWindow::SetSpeedWindow(int s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SetSpeedWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setValue(s);
    ui->label_speed->setText(QString::number(s));

    setAttribute(Qt::WA_DeleteOnClose);
}

SetSpeedWindow::~SetSpeedWindow()
{
    delete ui;
}

void SetSpeedWindow::on_buttonBox_accepted()
{
    emit sendData(ui->horizontalSlider->value());
    qDebug()<<"send:"<<ui->horizontalSlider->value();
    this->close();
}

void SetSpeedWindow::on_buttonBox_rejected()
{
    this->close();
}
