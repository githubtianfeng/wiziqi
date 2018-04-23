#include "mypushbutton.h"
#include <QTimerEvent>

MyPushButton::MyPushButton(QWidget *parent)
    : QPushButton(parent)
{
    image = NULL;
    flag = 0; //默认按钮无图标
}

MyPushButton::~MyPushButton()
{

}

void MyPushButton::setMyIcon(const QIcon &icon)
{
    if(!flag)
    {
        this->setIcon(icon);
        flag = 1;
    }
}

void MyPushButton::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == time_Id){
        if(flag){
            this->setIcon(QIcon(""));
            flag = 0;
        }else{
            this->setIcon(QIcon(*image));
            flag = 1;
        }
    }
}









