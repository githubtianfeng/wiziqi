#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

class MyImage : public QWidget
{
    Q_OBJECT
public:
    explicit MyImage(QWidget *parent = 0);
    ~MyImage();

    //若参数为QPixamap*  则在构造函数中不需要new QPixmap 否则需要指定空间 并手动delete
    void setPixmap(QPixmap &p)
    {
        *image = p;
    }


private:
    QPixmap* image;

protected:
    void paintEvent(QPaintEvent* );//重写paintEvent事件
};

#endif // MYIMAGE_H
