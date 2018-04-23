#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget> //列表组件
#include "mytextedit.h"

#include "board.h"
#include <QPixmap>
#include "myimage.h"
#include <QTcpSocket>

/**
 * @brief The Widget class
 * 网络部分消息头协议规定：
 * #Chat: 表示聊天信息
 * #Command:gameStart,5chess,0role 询问游戏开始 并规定几子棋、角色 0代表黑 1代表白  规定发起start的一方为黑方 先走
 * #Command:agree 同意开始游戏
 * #XY:5,10,1  表示第几行第几列被点击，1号角色
 * #Command:black win! 黑方赢了
 *
 * 客户端 网络部分用信号与槽函数来完成功能，不能用线程
 * 因为如果多线程，既要在主线程中widget中收数据，又要在子线程中发数据，
 * 同时界面还要操作，主线程和子线程不能使用同一个tcpSocket
 *
 */
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    int getRoleflag()
    {
        return roleFlag;
    }

    void setRoleflag(int f)
    {
        roleFlag = f;
    }

    void setCurrentListRow(int c)
    {
        currentListRow = c;
    }

    /* 网络部分 */
    //初始化网络
    void initNet();

public slots:
    void startGameTimer();

    void changeLagel();

    void setCurrentItem(QModelIndex);

    void OutConnectMsg();//

    void readMsgFromServer();

    void sendChatMsgToServer(QString msg);//发送消息给服务器

    void sendStartGameRequest();//点击start发送游戏开始申请

    void sendRowAndColToServer(int row,int col,int role);//发送行列号给服务器

    void sendSuccessToServer(QString msg);//发送成功信息给服务器
private:
    QTextEdit *upEdit;
    MyTextEdit *downEdit;

    QListWidget *lwiget;
    QPushButton *startBtn;

    QLabel *timeLbl;
    int time_Id;
    int h,m,s;

    QLabel *currRoleLbl;
 //   QLabel *imageLbl;
    MyImage *imageLbl;

    QPixmap *black,*white;
    int roleFlag;//自己的角色 0为黑 1为白
    Board *b;

    int currentListRow;//当前listWidget的选中的行

    /* 网络部分 */
    QTcpSocket* tcpSocket;
protected:
    void timerEvent(QTimerEvent *);
};

#endif // WIDGET_H





