#ifndef BLOCK_H
#define BLOCK_H

#include "header.h"

class Block
{
public:
    Block();
    Block(int ID,int X);
    Block(int X);

    int id;
    int x,x1,x2,x3;
    int y,y1,y2,y3;  //组成大块的4个小块坐标

    void calcu();   //根据id和元坐标x,y，计算其他的三个坐标

    void one_left();
    void one_right();
    void one_down();
    void one_up();  //测试用
    void rotate();  //旋转
    void anti_rotate(); //反旋
};

#endif // BLOCK_H
