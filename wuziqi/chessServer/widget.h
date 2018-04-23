#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    //获取客户端
    int getClientFromList(QObject* p);
    //发送聊天信息给所有客户端
    void sendChatMsgToAllClient(QByteArray msg);
public slots:
    void getNewConnection();
    void readDataFromClient();
    void sendGameRequestToOtherClient(int index,QByteArray msg);//转发游戏申请
    void sendGameResponseToAllClient(QByteArray msg);//发送游戏同意响应
    void sendRowAndColToOtherClient(int index,QByteArray msg);//发送行列号 角色给另一个客户端
    void sendSuccessToOtherClient(QByteArray msg);
    void removeSockets();//清理socket
private:
    QTcpServer* tcpServer;
    QList<QTcpSocket*> clientList;
};

#endif // WIDGET_H
