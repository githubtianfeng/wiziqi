#include "widget.h"
#include <QHostAddress>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(200,100);
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,8099);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(getNewConnection()));
}

Widget::~Widget()
{

}

int Widget::getClientFromList(QObject *p)
{
    for(int i=0;i<clientList.size();i++)
    {
        if(clientList.at(i) == p)
        {
            return i;
        }
    }
    return -1;
}

void Widget::sendChatMsgToAllClient(QByteArray msg)
{
    for(int i=0;i<clientList.size();++i)
    {
        clientList.at(i)->write(msg);
    }
}

void Widget::getNewConnection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();
    clientList.push_back(socket);
    qDebug() << "get a new client!";

    connect(socket,SIGNAL(readyRead()),this,SLOT(readDataFromClient()));

    connect(socket,SIGNAL(disconnected()),this,SLOT(removeSockets()));
  //  connect(socket,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}

void Widget::readDataFromClient()
{
    int index = getClientFromList(sender());
    if(index == -1)
        return;
    //1.读取数据
    QByteArray msg;
    msg = clientList.at(index)->readAll();
    qDebug() << "index:" <<index <<" read:" << msg;
    //2.数据解析     //3.相应处理
    if(msg.startsWith("#Chat:"))
    {
        //聊天信息
        sendChatMsgToAllClient(msg);
    }
    else if(msg.startsWith("#Command:"))
    {
        if(msg.contains("gameStart"))
        {
            //转发给另一个客户端
            sendGameRequestToOtherClient(index,msg);
        }
        else if(msg.contains("agree"))
        {
            //服务器收到某一客户端发来的agree请求，转发给两个客户端
            sendGameResponseToAllClient(msg);
        }
        else if(msg.contains("win"))
        {
            sendSuccessToOtherClient(msg);
        }
    }
    else if(msg.startsWith("#XY:"))
    {
        sendRowAndColToOtherClient(!index,msg);//只有两个客户端 在clientList中下标非0即1
    }

}

void Widget::sendGameRequestToOtherClient(int index, QByteArray msg)
{
    if(clientList.size() < 2)
        return;
    if(!index)
        clientList.at(1)->write(msg);
    else
        clientList.at(0)->write(msg);
}

void Widget::sendGameResponseToAllClient(QByteArray msg)
{
    if(clientList.size() < 2)
        return;
    for(int i=0;i<clientList.size();i++)
    {
        clientList.at(i)->write(msg);
    }
}

void Widget::sendRowAndColToOtherClient(int index,QByteArray msg)
{
    if(clientList.size() < 2)
        return;
    clientList.at(index)->write(msg);
}

void Widget::sendSuccessToOtherClient(QByteArray msg)
{
    int index = getClientFromList(sender());
    if(index == -1)
        return;
    clientList.at(!index)->write(msg);
}

void Widget::removeSockets()
{
    int index = getClientFromList(sender());
    if(index == -1)
        return;
    if(index < clientList.size())
    {
        clientList.at(index)->deleteLater();//先释放资源 再removeAt
        clientList.removeAt(index);
    }
}
