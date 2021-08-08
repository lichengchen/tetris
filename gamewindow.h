#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "block.h"
#include "setspeedwindow.h"
#include "infowindow.h"
#include <QMainWindow>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);

    bool collision(Block *block);   //碰撞检测，cur_block是否与已有方块重叠
    bool collision(Block *b, Block *b1);  //在双人模式检测两个块的碰撞
    void update_ground();   //更新地面，删除已经完整的行，即计算isblocked[][]

    int player_num;
    int pause;

private:
    Ui::GameWindow *ui;

    int speed;
    int score;
    int block_num;

    int height;     //记录最高块的y，优化碰撞检测和图像绘制
    int **isblocked;
    Block *cur_block;
    Block *next_block;

    QTimer *Timer;

    //下面是为第二位玩家准备的
    Block *cur_block_2;
    Block *next_block_2;
    int block_num_2;

    //音乐
    QSound* sound;
    QSound* s1;
    QSound* s2;

    //QMediaPlayer* sound;

private slots:

    void onTimeOut();

    void NewGame1Action();
    void NewGame2Action();
    void SaveAction();
    void LoadAction();
    void PauseAction();
    int StopAction();
    void RetryAction();
    void SetSpeedAction();
    void ShowInfo();

    void receiveData_to_setSpeed(int);
};

#endif // GAMEWINDOW_H
