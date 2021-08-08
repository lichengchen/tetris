#include "block.h"

Block::Block()
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    id=rand()%19;
    x=mx/2;
    y=0;

    calcu();
}

Block::Block(int ID,int X)
{
    id=ID;
    x=X;
    y=0;

    calcu();
}

Block::Block(int X)
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    id=rand()%19;
    x=X;
    y=0;

    calcu();
}

void Block::calcu()
{
    x1=x;
    x2=x;
    x3=x;
    y1=y;
    y2=y;
    y3=y;
    switch (id) {
    case 0:
        x1--;
        y2--;
        x3--;
        y3--;
        break;
    case 1:
        x1--;
        x2++;
        y3--;
        break;
    case 2:
        x3++;
        y1--;
        y2++;
        break;
    case 3:
        x1--;
        x2++;
        y3++;
        break;
    case 4:
        x1--;
        y2++;
        y3--;
        break;
    case 5:
        x1--;
        x2--;
        x3++;
        y1--;
        break;
    case 6:
        x3++;
        y1--;
        y2++;
        y3--;
        break;
    case 7:
        x1--;
        x2++;
        x3++;
        y3++;
        break;
    case 8:
        x3--;
        y1--;
        y2++;
        y3++;
        break;
    case 9:
        x1--;
        x2++;
        x3++;
        y3--;
        break;
    case 10:
        x3++;
        y1--;
        y2++;
        y3++;
        break;
    case 11:
        x1--;
        x2++;
        x3--;
        y3++;
        break;
    case 12:
        x3--;
        y1--;
        y2++;
        y3--;
        break;
    case 13:
        x1--;
        x3--;
        y2++;
        y3--;
        break;
    case 14:
        x1--;
        x3++;
        y2--;
        y3--;
        break;
    case 15:
        x1--;
        x2--;
        y2++;
        y3--;
        break;
    case 16:
        x1++;
        x3--;
        y2--;
        y3--;
        break;
    case 17:
        x1--;
        x2-=2;
        x3++;
        break;
    case 18:
        y1--;
        y2-=2;
        y3++;
        break;
    default:
        qDebug()<<"invalid id:"<<id;
        break;
    }
}

//左右移动函数可保证左右的安全性,而上下不保证
void Block::one_left()
{
    if(x>0&&x1>0&&x2>0&&x3>0){
        x--;
        x1--;
        x2--;
        x3--;
    }
}

void Block::one_right()
{
    if(x<mx-1&&x1<mx-1&&x2<mx-1&&x3<mx-1){
        x++;
        x1++;
        x2++;
        x3++;
    }
}

void Block::rotate()
{
    switch (id) {
    case 0:
        break;
    case 4:
        id=1;
        break;
    case 8:
        id=5;
        break;
    case 12:
        id=9;
        break;
    case 14:
        id=13;
        break;
    case 16:
        id=15;
        break;
    case 18:
        id=17;
        break;
    default:
        id++;
        break;
    }
    calcu();
    while(x<0||x1<0||x2<0||x3<0){
        x++;
        x1++;
        x2++;
        x3++;
    }
    while(x>mx-1||x1>mx-1||x2>mx-1||x3>mx-1){
        x--;
        x1--;
        x2--;
        x3--;
    }
}
void Block::anti_rotate()
{
    switch (id) {
    case 0:
        break;
    case 1:
        id=4;
        break;
    case 5:
        id=8;
        break;
    case 9:
        id=12;
        break;
    case 13:
        id=14;
        break;
    case 15:
        id=16;
        break;
    case 17:
        id=18;
        break;
    default:
        id--;
        break;
    }
    calcu();
    while(x<0||x1<0||x2<0||x3<0){
        x++;
        x1++;
        x2++;
        x3++;
    }
    while(x>mx-1||x1>mx-1||x2>mx-1||x3>mx-1){
        x--;
        x1--;
        x2--;
        x3--;
    }
}

void Block::one_up()
{
    y--;
    y1--;
    y2--;
    y3--;
}

void Block::one_down()
{
    y++;
    y1++;
    y2++;
    y3++;
}




