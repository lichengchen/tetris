#include "gamewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFont font  = app .font();
    font.setPointSize(10);
    app.setFont(QFont("微软雅黑",10));

    GameWindow w;
    w.show();

    return app.exec();
}
