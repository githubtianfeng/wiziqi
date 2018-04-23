#include "widget.h"
#include <QApplication>
#include "loginwigdet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

  //  LoginWigdet login;
  //  QObject:connect(w,SIGNAL(connectSucceed()),w,SLOT(close()));
  //  login.show();

    Widget w;
    w.show();

    return a.exec();
}
