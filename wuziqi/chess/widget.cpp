#include "widget.h"
#include <QGridLayout>
#include <QTimerEvent>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(800,600);
    this->setWindowTitle("五子棋");

    upEdit = new QTextEdit(this);
    upEdit->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);
    upEdit->setReadOnly(true);

    downEdit = new MyTextEdit(this);
    downEdit->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);

    //edit中的信息发送给服务器
    connect(downEdit,SIGNAL(sendChatMessage(QString)),
            this,SLOT(sendChatMsgToServer(QString)));

    lwiget = new QListWidget(this);
    lwiget->setSizePolicy(QSizePolicy::Ignored,
                          QSizePolicy::Ignored);
    lwiget->addItem("5 chess");
    lwiget->addItem("6 chess");
    lwiget->addItem("7 chess");
    connect(lwiget,SIGNAL(clicked(QModelIndex)),this,SLOT(setCurrentItem(QModelIndex)));

    startBtn = new QPushButton("Start",this);
    startBtn->setSizePolicy(QSizePolicy::Ignored,
                                      QSizePolicy::Ignored);
    connect(startBtn,SIGNAL(clicked()),
            this,SLOT(sendStartGameRequest()));

    timeLbl = new QLabel("Timer: 00:00:00",this);
    timeLbl->setFont(QFont("Times",16));
    timeLbl->setSizePolicy(QSizePolicy::Ignored,
                             QSizePolicy::Ignored);
    currRoleLbl = new QLabel("Current Role:",this);
    currRoleLbl->setSizePolicy(QSizePolicy::Ignored,
                                 QSizePolicy::Ignored);

    black = new QPixmap(":/black.jpg");
    white = new QPixmap(":/white.jpg");

  //  imageLbl = new QLabel(this);
    imageLbl = new MyImage(this);
    imageLbl->setSizePolicy(QSizePolicy::Ignored,
                              QSizePolicy::Ignored);
  //  imageLbl->setPixmap(*black);

    b = new Board(this);
    //设置所有按钮不可用
    b->setBtnEnable(false);
    b->setCountchess(5);//默认为五子棋

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(upEdit,0,0,3,2);
    layout->addWidget(downEdit,3,0,1,2);
    layout->addWidget(lwiget,4,0);
    layout->addWidget(startBtn,4,1);
    layout->addWidget(timeLbl,5,0,1,2);
    layout->addWidget(currRoleLbl,6,0,1,2);
    layout->addWidget(imageLbl,7,0,3,2);
    layout->addWidget(b,0,2,10,6);

    this->setLayout(layout);

    h = m = s = 0;

  //  connect(b,SIGNAL(changeLabelPixmap()),this,SLOT(changeLagel()));

    initNet();//初始化网络

    connect(b,SIGNAL(sendRowAndCol(int,int,int)),this,SLOT(sendRowAndColToServer(int,int,int)));
    connect(b,SIGNAL(success(QString)),this,SLOT(sendSuccessToServer(QString)));


}

Widget::~Widget()
{

}

void Widget::startGameTimer()
{
    b->setBtnEnable(true);//设置按钮可用
    time_Id = this->startTimer(1000);//开启定时器
    b->start();

}

//改变label图片
void Widget::changeLagel()
{
    if(!getRoleflag())
    {
        imageLbl->setPixmap(*white);
        setRoleflag(1);
    }
    else
    {
        imageLbl->setPixmap(*black);
        setRoleflag(0);
    }
    //重新更新label图片 触发paintEvent事件
    imageLbl->update();
}

void Widget::setCurrentItem(QModelIndex q)
{
    lwiget->setCurrentIndex(q);
    currentListRow = q.row();

    if(currentListRow == 0)
    {
        b->setCountchess(5);
    }
    else if(currentListRow == 1)
    {
        b->setCountchess(6);
    }
    else
        b->setCountchess(7);
}

void Widget::OutConnectMsg()
{
    qDebug() << "connect to host success!";
}

void Widget::readMsgFromServer()
{
    //1.读取数据
    QByteArray msg = tcpSocket->readAll();
    qDebug() << "readMsg from server: " << msg;
    //2.数据解析
    if(msg.startsWith("#Chat:"))
    {
        //聊天信息
        upEdit->append(msg.remove(0,6));//删掉消息头#Chat:
    }
    else if(msg.startsWith("#Command:"))
    {
        if(msg.contains("gameStart"))
        {
            if((QMessageBox::information(this,"Game Request",QString("Are you ready %1?").arg(QString(msg.mid(19,6)))
                       ,QMessageBox::Ok,QMessageBox::Cancel))== QMessageBox::Ok)
            {
                //判断是几子棋 并设置角色
                if(msg.contains("5chess"))
                {
                    b->setCountchess(5);
                    currentListRow = 0;
                    lwiget->setCurrentRow(0);
                }
                else if(msg.contains("6chess"))
                {
                    b->setCountchess(6);
                    currentListRow = 1;
                    lwiget->setCurrentRow(1);
                }
                else if(msg.contains("7chess"))
                {
                    b->setCountchess(7);
                    currentListRow = 2;
                    lwiget->setCurrentRow(2);
                }
                //直接将当前角色设置为黑方 自己角色设置为白方即可
                b->setRoleflag(0);
                setRoleflag(1);
                b->setRoleflag1(1);
                startBtn->setEnabled(false);

                imageLbl->setPixmap(*white);
                imageLbl->update();

                //同意开始游戏，发送游戏开始命令给服务器
                tcpSocket->write("#Command:agree");
            }
        }
        else if(msg.contains("agree"))//收到对方同意开始游戏消息
        {
            startGameTimer();
        }
        else if(msg.contains("win"))
        {
            QByteArray arr = msg.remove(0,9);
            if(QMessageBox::information(this,"chess",QString(arr),
                                     QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok)
                this->close();
            else
                b->setEnabled(false);
        }
    }
    else if(msg.startsWith("#XY:"))
    {
        //发送的row col是0-29的整数，因此用split分割 list保存更好
        QByteArray arr = msg.remove(0,4);
        QList<QByteArray> arrList = arr.split(',');
        int row = arrList.at(0).toInt();
        int col = arrList.at(1).toInt();
        int role = arrList.at(2).toInt();
        b->setRoleflag(!role);//给另一个客户端设置当前角色
        b->setIcon(row,col,role);//给对应位置设置图标
    }
}

void Widget::sendChatMsgToServer(QString msg)
{
    QByteArray data("#Chat:");
    data.append(msg);
    qint64 n = tcpSocket->write(data);
    if(n<=0)
        qDebug() << "write msg failture!";
}

void Widget::sendStartGameRequest()
{
    QByteArray msg = QByteArray("#Command:");
    msg.append("gameStart,");
    msg.append(QString::number(b->getCountchess()).append("chess,"));
    msg.append(QString::number(0).append("role"));
    setRoleflag(0);//设置自己角色为黑方
    b->setRoleflag(0);//自当前角色为黑方
    b->setRoleflag1(0);//玩家自己的角色为黑方
    imageLbl->setPixmap(*black);//设置图片
    imageLbl->update();
    startBtn->setEnabled(false);//start按钮置为不可用

    qDebug() << "send start game,msg:" << msg;
    qint64 n = tcpSocket->write(msg);
    if(n <= 0)
        qDebug() << "write msg failture!";
}

void Widget::sendRowAndColToServer(int row, int col, int role)
{
    QByteArray msg("#XY:");
    msg.append(QString::number(row).append(",").append(QString::number(col).append(",").append(QString::number(role))));

    tcpSocket->write(msg);
}

void Widget::sendSuccessToServer(QString msg)
{
    QByteArray arr("#Command:");
    arr.append(msg);
    tcpSocket->flush();//若不flush() 测试会"#XY:8,7,0#Command:black win!" 两个命令连在一起了
    tcpSocket->write(arr);
    qDebug() << "send " << arr;
    if(QMessageBox::information(this,"chess",msg,
                             QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok)
        this->close();
    else
        b->setEnabled(false);
}


void Widget::initNet()
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress::LocalHost,8099);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(OutConnectMsg()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMsgFromServer()));
}

void Widget::timerEvent(QTimerEvent *e)
{
    //游戏开始计时 id
    if(e->timerId() == time_Id){
        if(++s > 59){
            s = 0;
            m++;
            if(m > 59){
                m = 0;
                h++;
                if(h > 24){

                }
            }
        }

        QString hstr = QString("%1").arg(h).rightJustified(2,'0');
        QString mstr = QString("%1").arg(m).rightJustified(2,'0');
        QString sstr = QString("%1").arg(s).rightJustified(2,'0');
        //更新事件标签
        QString str = QString("Timer:%1:%2:%3")
                .arg(hstr).arg(mstr).arg(sstr);
        timeLbl->setText(str);
    }
}
