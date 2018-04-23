#include "myimage.h"
#include <QPainter>

MyImage::MyImage(QWidget *parent) :
    QWidget(parent)
{
    image = new QPixmap();
}

MyImage::~MyImage()
{
    delete image;
}

void MyImage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect r(0,0,width(),height());

    painter.drawPixmap(r,*image);//图片平铺
}
