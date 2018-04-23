#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPushButton>
#include "mypushbutton.h"
#include <QPixmap>
#include <QMap>
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    void setBtnEnable(bool);

    void start();

    void setRoleflag(int f)
    {
        roleFlag = f;
    }

    int getRoleflag()
    {
        return roleFlag;
    }
    void setRoleflag1(int f)
    {
        roleFlag1 = f;
    }
    int getRoleflag1()
    {
        return roleFlag1;
    }

    int isWin(MyPushButton *p);

    void setCountchess(int c)
    {
        countChess = c;
    }

    int getCountchess()
    {
        return countChess;
    }

public slots:
    void getBtnIandJ();
    void setIcon(int row,int col,int role);
    void startMyTimer(MyPushButton* b);
signals:
    void changeLabelPixmap();
    void success(QString);
    void sendRowAndCol(int,int,int);//行列号、棋子角色
private:
    MyPushButton *btn[30][30];
    bool isMouseItem;//鼠标上是否有棋子
    int roleFlag;//棋子当前角色标志 0:黑 1:白
    int roleFlag1;//玩家自己的角色
    QPixmap* black,*white;
    QMap<MyPushButton*,int> timeIdMap;//闪烁定时器map

    int countChess;//记录几棋子
protected:
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // BOARD_H






