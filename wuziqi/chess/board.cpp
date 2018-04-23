#include "board.h"
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMapIterator>
#include <QMessageBox>

Board::Board(QWidget *parent) : QWidget(parent)
{
    this->resize(500,600);

    QGridLayout *mLayout = new QGridLayout;
    mLayout->setSpacing(0);
    int i = 0;
    int j = 0;
    isMouseItem = false;
    black = new QPixmap(":/black.jpg");
    white = new QPixmap(":/white.jpg");
    countChess = 5;//默认是五子棋

    for(;i < 30;i++){
        for(j = 0;j < 30;j++){
            btn[i][j] = new MyPushButton(this);
            btn[i][j]->setRow(i);
            btn[i][j]->setCol(j);
            btn[i][j]->setRoleflag(-1);

            btn[i][j]->setSizePolicy(QSizePolicy::Ignored,
                                     QSizePolicy::Ignored);
            mLayout->addWidget(btn[i][j],i,j,1,1);

            connect(btn[i][j],SIGNAL(clicked()),
                    this,SLOT(getBtnIandJ()));
        }
    }

    this->setLayout(mLayout);
}

Board::~Board()
{
//    int i = 0;
//    int j = 0;
//    for(;i < 30;i++)
//    {
//        for(j = 0;j < 30;j++)
//        {
//            delete btn[i][j];
//        }
//    }
}

/**
 * @brief Board::getBtnIandJ
 * 通过sender获取发起信号者是哪个按钮
   判断按钮上面由有没有图片 若没有，则判断当前角色
   为0：为黑方 为1：为白方
   设置当前按钮角色
   发射信号改变widget的label
   开启定时器 实现闪烁功能
   判断输赢
 */
void Board::getBtnIandJ()
{
    if(roleFlag != getRoleflag1())//如果玩家角色不是当前角色 那就不能点击 直接返回
        return;
    MyPushButton *b = (MyPushButton *)sender();
    if(!b->getFlag())//如果按钮没有图片
    {
        if(!roleFlag)
        {
            //当前角色为黑
            b->setMyIcon(*black);//设置按钮图标
            b->setImage(black);//闪烁图片为黑色
            b->setRoleflag(0);//设置按钮的角色
            emit sendRowAndCol(b->getRow(),b->getCol(),roleFlag);

            roleFlag = 1;//设置下一次的角色为白
        }
        else
        {
            //当前角色为白
            b->setMyIcon(*white);
            b->setImage(white);
            b->setRoleflag(1);
            emit sendRowAndCol(b->getRow(),b->getCol(),roleFlag);

            roleFlag = 0;//设置下一次的角色为黑
        }
  //      emit changeLabelPixmap();//发射信号改变widget的label

        startMyTimer(b);
    }

    //判断输赢
    int win;
    if((win = isWin(b))!= -1)
    {
        QString name;
        if(win == 0)
            name = "black";
        else
            name = "white";
        emit success(QString("%1 win!").arg(name));//发射赢得信号 关闭游戏
    }
}

void Board::setIcon(int row, int col, int role)
{
    if(!role)
    {
        //为黑方 在相应位置设置图标
        btn[row][col]->setIcon(*black);//图标为黑色
        btn[row][col]->setRoleflag(0);//棋子角色为黑色
        btn[row][col]->setImage(black);//闪烁图片为黑色
    }
    else
    {
        //为白方
        btn[row][col]->setIcon(*white);
        btn[row][col]->setRoleflag(1);
        btn[row][col]->setImage(white);
    }
    startMyTimer(btn[row][col]);
}

void Board::startMyTimer(MyPushButton *b)
{
    int id = b->startTimer(500);//开启定时器 实现闪烁功能
    b->setTimeId(id);//设置当前定时器id

//        QMapIterator<MyPushButton*, int> iterator(timeIdMap);
//        while (iterator.hasNext()) {
//            iterator.next();
//            iterator.key()->killTimer(iterator.value());
//            iterator.key()->setMyIcon(*(iterator.key()->getImage()));
//            timeIdMap.remove(iterator.key());
//        }
    //foreach效率更高
    if(timeIdMap.size())
    {
        foreach (MyPushButton *p,timeIdMap.keys()) {
            p->killTimer(timeIdMap.value(p));//删掉之前的定时器 只是让当前的棋子闪烁
            p->setMyIcon(*(p->getImage()));//将之前闪烁的棋子重新设置图片 以免因闪烁空白而造成丢失
            timeIdMap.remove(p);//把之前闪烁的棋子移除timeIdMap
        }
    }
    timeIdMap.insert(b,id);//把当前棋子加入timeIdMap
}

void Board::setBtnEnable(bool f)
{
    int i,j;
    for(i = 0;i < 30;i++)
        for(j = 0;j < 30;j++)
            btn[i][j]->setEnabled(f);
}

void Board::start()
{
    isMouseItem = true;
}

int Board::isWin(MyPushButton *p)
{
    int role = p->getRoleflag();
    //判断横向的
    {
        int count = 1;
        int x = p->getRow(),y = p->getCol();//x、y表示行列号 不是x y的坐标
        while(1)
        {
            if(--y < 0)//判断是否超出边界
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        y = p->getCol();
        while(1)
        {
            if(++y > 29)
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        if(count >= countChess)
            return role;
    }

    //判断纵向的
    {
        int count = 1;
        int x = p->getRow(),y = p->getCol();
        while(1)
        {
            if(--x < 0)//判断是否超出边界
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        x = p->getRow();
        while(1)
        {
            if(++x > 29)
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        if(count >= countChess)
            return role;
    }

    //判断斜方向的
    {
        int count = 1;
        int x = p->getRow(),y = p->getCol();
        while(1)
        {
            if(--y < 0 || --x < 0)//判断是否超出边界
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        x = p->getRow();
        y = p->getCol();
        while(1)
        {
            if(++x > 29 || ++y > 29)
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        if(count >= countChess)
            return role;
    }

    {
        int count = 1;
        int x = p->getRow(),y = p->getCol();
        while(1)
        {
            if(--x < 0 || ++y > 29)//判断是否超出边界
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        x = p->getRow();
        y = p->getCol();
        while(1)
        {
            if(--y < 0 || ++x > 29)
                break;
            if(btn[x][y]->getRoleflag() == role)
                count++;
            else
                break;
        }
        if(count >= countChess)
            return role;
    }

    return -1;
}


void Board::mouseMoveEvent(QMouseEvent *e)
{
    //可以做棋子图片跟随鼠标移动的功能
   if(isMouseItem)
   {
       QPoint p = e->pos();
//       qDebug()<<p<<endl;
   }
}



