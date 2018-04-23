#include "mytextedit.h"
#include <QKeyEvent> //键盘事件类

MyTextEdit::MyTextEdit(QWidget *parent)
    :QTextEdit(parent)
{

}

MyTextEdit::~MyTextEdit()
{

}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    //按return键，获取自己的  文字信息
    if(e->key() == Qt::Key_Return || (e->key() == Qt::Key_Return && e->modifiers() == Qt::ControlModifier))
    {
        //捕获 回车键被按下
        //只有回车键被按下或者ctrl+return，发送消息
        emit sendChatMessage(toPlainText());
        this->clear();
        return;
    }
    else
    {
        QTextEdit::keyPressEvent(e);
    }
}









