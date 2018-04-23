#include "loginwigdet.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHostAddress>
#include <QLineEdit>
#include <QLabel>

LoginWigdet::LoginWigdet(QWidget *parent) :
    QWidget(parent)
{
    this->resize(200,300);

    QLabel *userLabel = new QLabel("用户名",this);
    QLabel *passwordLabel = new QLabel("密码",this);

    QLineEdit *userEdit = new QLineEdit(this);
    QLineEdit *passwordEdit = new QLineEdit(this);

    QPushButton *btn1 = new QPushButton("确定",this);
    QPushButton *btn2 = new QPushButton("取消",this);

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(userLabel);
    hLayout1->addWidget(userEdit);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(passwordLabel);
    hLayout2->addWidget(passwordEdit);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(btn1);
    hLayout3->addWidget(btn2);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout1);
    mainLayout->addLayout(hLayout2);
    mainLayout->addLayout(hLayout3);

    this->setLayout(mainLayout);

    //连接
    tcpSocket = new QTcpSocket(this);
    connect(btn1,SIGNAL(clicked()),this,SLOT(connBtnClicked()));
    //捕获 网络连接成功信号
    connect(tcpSocket,SIGNAL(connected()),
            this,SLOT(connectToHostSuccess()));
}

void LoginWigdet::connBtnClicked()
{
    tcpSocket->connectToHost(QHostAddress::LocalHost,8099);
}

void LoginWigdet::connectToHostSuccess()
{
    emit connectSucceed();
}

void LoginWigdet::sendMessageToHost()
{

}


