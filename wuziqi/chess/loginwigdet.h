#ifndef LOGINWIGDET_H
#define LOGINWIGDET_H

#include <QWidget>
#include <QTcpSocket>

class LoginWigdet : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWigdet(QWidget *parent = 0);

signals:
    void connectSucceed();
public slots:
    void connBtnClicked();
    void connectToHostSuccess();
    void sendMessageToHost();
private:
    QTcpSocket *tcpSocket;
};

#endif // LOGINWIGDET_H
