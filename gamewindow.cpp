#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "math.h"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    connect(ui->action_newGame1,&QAction::triggered,this,&GameWindow::NewGame1Action);
    connect(ui->action_newGame2,&QAction::triggered,this,&GameWindow::NewGame2Action);
    connect(ui->action_save,&QAction::triggered,this,&GameWindow::SaveAction);
    connect(ui->action_load,&QAction::triggered,this,&GameWindow::LoadAction);
    connect(ui->action_pause,&QAction::triggered,this,&GameWindow::PauseAction);
    connect(ui->action_stop,&QAction::triggered,this,&GameWindow::StopAction);
    connect(ui->action_retry,&QAction::triggered,this,&GameWindow::RetryAction);
    connect(ui->action_setSpeed,&QAction::triggered,this,&GameWindow::SetSpeedAction);
    connect(ui->action_files,&QAction::triggered,this,&GameWindow::ShowInfo);

    this->setWindowTitle("Tetris");
    /*
    player = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("F:\\1.mp3"));
    player->setVolume(10);
*/
    sound = new QSound("1.wav");
    sound->setLoops(INFINITY);
    s1=new QSound("biu.wav");
    s2=new QSound("dingling.wav");

    //初值设定
    player_num=0;
    pause=1;
    speed=SPEED;
    score=0;
    block_num=0;
    height=my;
    block_num_2=0;

    //背景空间
    isblocked=new int* [mx];
    for(int i=0;i<mx;i++){
        isblocked[i]= new int [my];
        for(int j=0;j<my;j++){
            isblocked[i][j]=0;
        }
    }

    //定时器与刷新
    Timer = new QTimer( this );
    connect( Timer, SIGNAL(timeout()), SLOT(onTimeOut()));
}

GameWindow::~GameWindow()
{
    delete ui;

    if(isblocked!=NULL){
        for(int i=0;i<mx;i++)
            delete [] isblocked[i];
        delete isblocked;

    }
    if(cur_block!=NULL)
        delete cur_block;
    if(next_block!=NULL)
        delete next_block;
    if(cur_block_2!=NULL)
        delete cur_block_2;
    if(next_block_2!=NULL)
        delete next_block_2;

    delete Timer;

    delete sound;
    delete s1;
    delete s2;
}

//相对起始位置BX BY，画一个位置为x,y(y>=0)单位长度的，边长为单位长度(BlockSize)的矩形
void my_drawRect(int bx,int by,QPainter &painter,int x,int y)
{
    if(y>=0)
        painter.drawRect(bx+x*BlockSize,by+y*BlockSize,BlockSize,BlockSize);
}

//画一个块
void drawBlock(int bx,int by,QPainter &painter,Block* block)
{
    my_drawRect(bx,by,painter,block->x,block->y);
    my_drawRect(bx,by,painter,block->x1,block->y1);
    my_drawRect(bx,by,painter,block->x2,block->y2);
    my_drawRect(bx,by,painter,block->x3,block->y3);
}

//在画面右侧画下一块
void draw_nextBlock(int bx,int by,QPainter &painter,Block* block)
{
    painter.setBrush(QBrush());//设置画刷形式
    painter.drawRect(bx,by,BlockSize*6,BlockSize*6);
    painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));//设置画刷形式

    my_drawRect(bx,by,painter,3,3);
    my_drawRect(bx,by,painter,block->x1-block->x+3,block->y1-block->y+3);
    my_drawRect(bx,by,painter,block->x2-block->x+3,block->y2-block->y+3);
    my_drawRect(bx,by,painter,block->x3-block->x+3,block->y3-block->y+3);
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //painter.setPen(QPen());//设置画笔形式
    //painter.setBrush(QBrush());//设置画刷形式

    if(player_num==1){
        //画墙
        painter.drawRect(BX,BY,BlockSize*mx,BlockSize*my);
        //画方块
        painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));//设置画刷形式
        drawBlock(BX,BY,painter,cur_block);

        //画下一个块（右）
        draw_nextBlock(BX+BlockSize*(mx+2),BY+BlockSize*2,painter,next_block);

        //画已经存在的地面
        painter.setPen(QPen(Qt::blue,4));//设置画笔形式

        if(isblocked!=NULL){
            for(int i=0;i<mx;i++){
                for(int j=height;j<my;j++){
                    if(isblocked[i][j]==1){

                        painter.drawRect(BX+i*BlockSize,BY+j*BlockSize,BlockSize,BlockSize);

                    }
                }
            }
        }

        //画字符
        painter.drawText(BX+BlockSize*(mx+2), BY+30, QStringLiteral("下一个"));
        painter.drawText(BX+BlockSize*(mx+2), BY+150+BlockSize*6, QStringLiteral("得分"));
        painter.drawText(BX+BlockSize*(mx+2), BY+100+BlockSize*6, QStringLiteral("落下的方块数"));

        painter.setPen(QPen());
        painter.drawText(BX+BlockSize*(mx+2)+180, BY+100+BlockSize*6, QString::number(block_num));

        painter.setPen(QPen(Qt::red,4));
        painter.drawText(BX+BlockSize*(mx+2)+180, BY+150+BlockSize*6, QString::number(score));

    }

    else if(player_num==2){
        //画墙
        painter.drawRect(BX,BY,BlockSize*mx,BlockSize*my);
        //画方块
        painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));//设置画刷形式
        drawBlock(BX,BY,painter,cur_block);

        painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));//设置画刷形式
        drawBlock(BX,BY,painter,cur_block_2);


        //画下一个块（左、右）
        draw_nextBlock(BX+BlockSize*(mx+2),BY+BlockSize*2,painter,next_block);
        draw_nextBlock(BX+BlockSize*(mx+2),BY+BlockSize*2+200,painter,next_block_2);



        //画已经存在的地面
        painter.setPen(QPen(Qt::blue,4));//设置画笔形式

        if(isblocked!=NULL){
            for(int i=0;i<mx;i++){
                for(int j=height;j<my;j++){
                    if(isblocked[i][j]==1){
                        painter.drawRect(BX+i*BlockSize,BY+j*BlockSize,BlockSize,BlockSize);
                    }
                }
            }
        }

        //画字符
        painter.setPen(QPen(Qt::red));//设置画笔形式
        painter.drawText(BX+BlockSize*(mx+2), BY+30, QStringLiteral("玩家一    已下落块数："));
        painter.drawText(BX+BlockSize*(mx+2)+180, BY+30, QString::number(block_num_2));

        painter.setPen(QPen(Qt::green));//设置画笔形式
        painter.drawText(BX+BlockSize*(mx+2), BY+230, QStringLiteral("玩家二    已下落块数："));
        painter.drawText(BX+BlockSize*(mx+2)+180, BY+230, QString::number(block_num));

        painter.setPen(QPen(Qt::black));//设置画笔形式
        painter.drawText(BX+BlockSize*(mx+2), BY+500, QStringLiteral("得分："));
        painter.drawText(BX+BlockSize*(mx+2)+180, BY+500, QString::number(score));
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(pause==1)
        return;

    if(player_num==1){
        s1->play();
        switch(event->key()){
        case Qt::Key_Left:{
            cur_block->one_left();
            if(collision(cur_block)==true)
                cur_block->one_right();
            break;
        }

        case Qt::Key_Right:{
            cur_block->one_right();
            if(collision(cur_block)==true)
                cur_block->one_left();
            break;
        }

        case Qt::Key_Up:{
            cur_block->rotate();
            if(collision(cur_block)==true)
                cur_block->anti_rotate();
            break;
        }

        case Qt::Key_Down:{
            while(collision(cur_block)==false){
                cur_block->one_down();
            }
            cur_block->one_up();
            break;
        }
        default:{
            qDebug()<<"Invalid Input";
            break;
        }
        }
    }

    else if(player_num==2){
        s1->play();
        switch(event->key()){
        case Qt::Key_Left:{
            cur_block->one_left();
            if(collision(cur_block)==true||collision(cur_block,cur_block_2)==true)
                cur_block->one_right();
            break;
        }

        case Qt::Key_Right:{
            cur_block->one_right();
            if(collision(cur_block)==true||collision(cur_block,cur_block_2)==true)
                cur_block->one_left();
            break;
        }

        case Qt::Key_Up:{
            cur_block->rotate();
            if(collision(cur_block)==true||collision(cur_block,cur_block_2)==true)
                cur_block->anti_rotate();
            break;
        }

        case Qt::Key_Down:{
            while(collision(cur_block)==false&&collision(cur_block,cur_block_2)==false){
                cur_block->one_down();
            }
            cur_block->one_up();
            break;
        }

        case Qt::Key_A:{
            cur_block_2->one_left();
            if(collision(cur_block_2)==true||collision(cur_block,cur_block_2)==true)
                cur_block_2->one_right();

            break;
        }
        case Qt::Key_D:{
            cur_block_2->one_right();
            if(collision(cur_block_2)==true||collision(cur_block,cur_block_2)==true)
                cur_block_2->one_left();
            break;
        }
        case Qt::Key_W:{
            cur_block_2->rotate();
            if(collision(cur_block_2)==true||collision(cur_block,cur_block_2)==true)
                cur_block_2->anti_rotate();
            break;
        }
        case Qt::Key_S:{
            while(collision(cur_block_2)==false&&collision(cur_block,cur_block_2)==false){
                cur_block_2->one_down();
            }
            cur_block_2->one_up();
            break;
        }
        default:{
            qDebug()<<"Invalid Input";
            break;
        }
        }

        update();
    }
}

bool GameWindow::collision(Block* block)
{
    int x=block->x;
    int y=block->y;
    int x1=block->x1;
    int y1=block->y1;
    int x2=block->x2;
    int y2=block->y2;
    int x3=block->x3;
    int y3=block->y3;

    if(x>mx-1||x1>mx-1||x2>mx-1||x3>mx-1||x<0||x1<0||x2<0||x3<0){
        return true;
    }

    if(y>my-1||y1>my-1||y2>my-1||y3>my-1){
        return true;
    }
    if(y>=0&&y1>=0&&y2>=0&&y3>=0){  //这是因为y<0是合法的
        if(isblocked[x][y]==1||isblocked[x1][y1]==1||isblocked[x2][y2]==1||isblocked[x3][y3]==1){
            return true;
        }
    }
    return false;
}

bool GameWindow::collision(Block* b,Block* b1)
{
    int check[mx][my];
    for(int i=0;i<mx;i++){
        for(int j=0;j<my;j++){
            check[i][j]=0;
        }
    }
    check[b->x][b->y]++;
    check[b->x1][b->y1]++;
    check[b->x2][b->y2]++;
    check[b->x3][b->y3]++;
    check[b1->x][b1->y]++;
    check[b1->x1][b1->y1]++;
    check[b1->x2][b1->y2]++;
    check[b1->x3][b1->y3]++;

    for(int i=0;i<mx;i++){
        for(int j=0;j<my;j++){
            if(check[i][j]>1){
                return true;
            }
        }
    }
    return false;
}

void GameWindow::onTimeOut()
{
    if(player_num==1){
        Timer->stop();
        cur_block->one_down();
        if(collision(cur_block)==true){
            cur_block->one_up();    //此时块“落地”

            s2->play();

            //更新基本信息,height始终应是最小的y
            block_num++;
            if(height>cur_block->y)
                height=cur_block->y;
            if(height>cur_block->y1)
                height=cur_block->y1;
            if(height>cur_block->y2)
                height=cur_block->y2;
            if(height>cur_block->y3)
                height=cur_block->y3;

            //check_gameover();
            if(cur_block->y<0||cur_block->y1<0||cur_block->y2<0||cur_block->y3<0){
                qDebug()<<"GAME OVER~";
                PauseAction();
                int q=QMessageBox::question(NULL, "GameOver~", "你输了！要重新开始嘛！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(q==QMessageBox::Yes){
                    player_num=0;
                    pause=1;
                    score=0;
                    block_num=0;
                    height=0;
                    for(int i=0;i<mx;i++){
                        for(int j=0;j<my;j++){
                            isblocked[i][j]=0;
                        }
                    }
                    delete cur_block;
                    delete next_block;
                    Timer->stop();
                    NewGame1Action();
                }
                else{
                    pause=1;
                }
                return;
            }

            //更新“地面”
            isblocked[cur_block->x][cur_block->y]=1;
            isblocked[cur_block->x1][cur_block->y1]=1;
            isblocked[cur_block->x2][cur_block->y2]=1;
            isblocked[cur_block->x3][cur_block->y3]=1;

            update_ground();

            //更新块
            Block* tmp_Block=cur_block;
            cur_block=next_block;
            delete tmp_Block;
            next_block=new Block();
        }
        Timer->start( 1000/speed );
        update();
    }

    else if(player_num==2){
        Timer->stop();

        cur_block->one_down();
        cur_block_2->one_down();

        if(collision(cur_block)==true){
            cur_block->one_up();    //此时块“落地”
            cur_block_2->one_up();

            s2->play();

            //更新基本信息,height始终应是最小的y
            block_num_2++;
            if(height>cur_block->y)
                height=cur_block->y;
            if(height>cur_block->y1)
                height=cur_block->y1;
            if(height>cur_block->y2)
                height=cur_block->y2;
            if(height>cur_block->y3)
                height=cur_block->y3;

            //check_gameover();
            if(cur_block->y<0||cur_block->y1<0||cur_block->y2<0||cur_block->y3<0){
                qDebug()<<"GAME OVER~";
                PauseAction();
                int q=QMessageBox::question(NULL, "GameOver~", "你输了！要从新开始嘛！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(q==QMessageBox::Yes){
                    player_num=0;
                    pause=1;
                    score=0;
                    block_num=0;
                    height=0;
                    for(int i=0;i<mx;i++){
                        for(int j=0;j<my;j++){
                            isblocked[i][j]=0;
                        }
                    }
                    delete cur_block;
                    delete next_block;
                    delete cur_block_2;
                    delete next_block_2;
                    Timer->stop();
                    NewGame2Action();
                }
                else{
                    pause=1;
                }
                return;
            }

            //更新“地面”
            isblocked[cur_block->x][cur_block->y]=1;
            isblocked[cur_block->x1][cur_block->y1]=1;
            isblocked[cur_block->x2][cur_block->y2]=1;
            isblocked[cur_block->x3][cur_block->y3]=1;

            update_ground();

            //更新块
            Block* tmp_Block=cur_block;
            cur_block=next_block;
            delete tmp_Block;
            next_block=new Block(mx/2+3);

        }
        if(collision(cur_block_2)==true){
            cur_block->one_up();    //此时块“落地”
            cur_block_2->one_up();

            s2->play();

            //更新基本信息,height始终应是最小的y
            block_num++;
            if(height>cur_block_2->y)
                height=cur_block_2->y;
            if(height>cur_block_2->y1)
                height=cur_block_2->y1;
            if(height>cur_block_2->y2)
                height=cur_block_2->y2;
            if(height>cur_block_2->y3)
                height=cur_block_2->y3;

            //check_gameover();
            if(cur_block_2->y<0||cur_block_2->y1<0||cur_block_2->y2<0||cur_block_2->y3<0){
                qDebug()<<"GAME OVER~";
                PauseAction();
                int q=QMessageBox::question(NULL, "GameOver~", "你输了！要重新开始嘛！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(q==QMessageBox::Yes){
                    player_num=0;
                    pause=1;
                    score=0;
                    block_num=0;
                    height=0;
                    for(int i=0;i<mx;i++){
                        for(int j=0;j<my;j++){
                            isblocked[i][j]=0;
                        }
                    }
                    delete cur_block;
                    delete next_block;
                    delete cur_block_2;
                    delete next_block_2;
                    Timer->stop();
                    NewGame2Action();
                }
                else{
                   pause=1;
                }
                return;
            }

            //更新“地面”
            isblocked[cur_block_2->x][cur_block_2->y]=1;
            isblocked[cur_block_2->x1][cur_block_2->y1]=1;
            isblocked[cur_block_2->x2][cur_block_2->y2]=1;
            isblocked[cur_block_2->x3][cur_block_2->y3]=1;

            update_ground();

            //更新块
            Block* tmp_Block=cur_block_2;
            cur_block_2=next_block_2;
            delete tmp_Block;
            next_block_2=new Block(mx/2-3);
        }
        Timer->start( 1000/speed );
        update();
    }
}

void GameWindow::update_ground()
{
    for(int y=my-1;y>=height;y--){   //从下一行往上
        int x;
        for(x=0;x<mx;x++){  //检测一行是不是全满
            if(isblocked[x][y]==0)
                break;
        }
        if(x==mx){  //即上一循环未break,一行全满
            for(int i=0;i<mx;i++){
                isblocked[i][y]=0;  //此行清空
                score++;
                for(int j=y;j>height;j--){   //上面的行下降
                    isblocked[i][j]=isblocked[i][j-1];
                }
                isblocked[i][height]=0;  //最上面一行是空
            }
            height--;
        }
    }
}

void GameWindow::NewGame1Action()
{
    if(player_num==0){
        sound->play();

        pause=0;
        player_num=1;
        speed=SPEED;
        score=0;
        block_num=0;
        height=my;

        next_block=new Block();
        cur_block=new Block((next_block->id+5)%19,mx/2);

        Timer->start( 1000/speed );
        //player->play();
    }
    else{
        if(pause==0)
            PauseAction();
        int q=QMessageBox::question(NULL, "新建游戏", "已有游戏在运行，是否继续新建？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(q==QMessageBox::Yes){
            StopAction();
            NewGame1Action();
        }
        else{
            PauseAction();
        }
    }
}

void GameWindow::NewGame2Action(){
    if(player_num==0){
        sound->play();
        pause=0;
        player_num=2;
        speed=SPEED;
        score=0;
        block_num=0;
        height=my;
        block_num_2=0;

        next_block=new Block();
        next_block->x+=3;
        next_block->calcu();
        cur_block=new Block((next_block->id+5)%19,mx/2+3);

        next_block_2=new Block((cur_block->id+5)%19,mx/2-3);
        cur_block_2=new Block((next_block_2->id+5)%19,mx/2-3);

        Timer->start( 1000/speed );
        //player->play();
    }
    else{
        if(pause==0)
            PauseAction();
        int q=QMessageBox::question(NULL, "新建游戏", "已有游戏在运行，是否继续新建？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(q==QMessageBox::Yes){
            StopAction();
            NewGame2Action();
        }
        else{
            PauseAction();
        }
    }
}

void GameWindow::SaveAction(){
    if(player_num==0)
        return;
    QFile file("mygame.txt");
    if(file.exists()){
        int q=QMessageBox::question(NULL, "保存游戏", "已有存档，保存将覆盖，是否继续？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(q==QMessageBox::Yes){

        }
        else{
           return;
        }
    }

    if (file.open(QIODevice ::WriteOnly | QIODevice ::Text)){
        QTextStream out(&file);
        out<<player_num<<endl<<pause<<endl<<speed<<endl<<score<<endl<<block_num<<endl<<height<<endl;
        for(int i=0;i<mx;i++){
            for(int j=0;j<my;j++){
                out<<isblocked[i][j]<<endl;
            }
        }
        out<<cur_block->id<<endl<<cur_block->x<<endl<<cur_block->y<<endl;
        out<<next_block->id<<endl<<next_block->x<<endl<<next_block->y<<endl;

        if(player_num==2){
            out<<cur_block_2->id<<endl<<cur_block_2->x<<endl<<cur_block_2->y<<endl;
            out<<next_block_2->id<<endl<<next_block_2->x<<endl<<next_block_2->y<<endl;
            out<<block_num_2<<endl;
        }
        QMessageBox::about(NULL, "保存", "保存成功！");;
    }
    file.close();
}

void GameWindow::LoadAction(){
    QFile file("mygame.txt");
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text)){
        QTextStream in(&file);
        StopAction();
        NewGame1Action();
        in>>player_num>>pause>>speed>>score>>block_num>>height;
        for(int i=0;i<mx;i++){
            for(int j=0;j<my;j++){
                in>>isblocked[i][j];
            }
        }
        cur_block=new Block;
        next_block=new Block;
        in>>cur_block->id>>cur_block->x>>cur_block->y;
        in>>next_block->id>>next_block->x>>next_block->y;
        cur_block->calcu();
        next_block->calcu();

        if(player_num==2){
            cur_block_2=new Block;
            next_block_2=new Block;
            in>>cur_block_2->id>>cur_block_2->x>>cur_block_2->y;
            in>>next_block_2->id>>next_block_2->x>>next_block_2->y;
            cur_block_2->calcu();
            next_block_2->calcu();
            in>>block_num_2;
        }
    }
    else{
        QMessageBox::about(NULL, "加载", "未找到可用存档");
    }
    file.close();
}

void GameWindow::PauseAction(){
    if(player_num==0)
        return;

    if(pause==0){
        pause=1;
        Timer->stop();
        sound->stop();
    }
    else{
        pause=0;
        Timer->start( 1000/speed );
        sound->play();
    }
}

int GameWindow::StopAction()
{
    if(player_num==0)
        return 0;

    if(pause==0)
        PauseAction();
    int q=QMessageBox::warning(NULL, "警告", "未保存的游戏将丢失，是否继续？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(q==QMessageBox::No){
        PauseAction();
        return 0;
    }

    player_num=0;
    pause=1;
    score=0;
    block_num=0;
    height=0;
    for(int i=0;i<mx;i++){
        for(int j=0;j<my;j++){
            isblocked[i][j]=0;
        }
    }
    delete cur_block;
    delete next_block;

    Timer->stop();
    sound->stop();
    s1->stop();
    s2->stop();

    update();
    return 1;
}

void GameWindow::RetryAction()
{
    if(player_num==0)
        return;

    int pla=player_num;
    if(StopAction()==1){
        if(pla==1){
            NewGame1Action();
        }
        if(pla==2){
            NewGame2Action();
        }
    }
}

void GameWindow::SetSpeedAction()
{
    if(player_num==0)
        return;

    if(pause==0)
        PauseAction();
    SetSpeedWindow *s =new SetSpeedWindow(speed,this);
    connect(s,SIGNAL(sendData(int)),this,SLOT(receiveData_to_setSpeed(int)));
    connect(s,SIGNAL(destroyed(QObject*)),this,SLOT(PauseAction()));
    s->show();
    update();
}

void GameWindow::receiveData_to_setSpeed(int newSpeed)
{
    speed=newSpeed;
}

void GameWindow::ShowInfo()
{
    InfoWindow *i=new InfoWindow;
    i->show();
}
