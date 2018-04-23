#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QPixmap>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget *parent = 0);
    ~MyPushButton();
    void setRow(int x){
        row = x;
    }
    void setCol(int y){
        col = y;
    }
    void setTimeId(int id){
        time_Id = id;
    }
    int getRoleflag()
    {
        return roleFlag;
    }
    void setRoleflag(int f)
    {
        roleFlag = f;
    }

    int getFlag()
    {
        return flag;
    }
    void setMyIcon(const QIcon &icon);
    void setImage(QPixmap* p)
    {
        image = p;
    }
    QPixmap* getImage()
    {
        return image;
    }

    int getRow()
    {
        return row;
    }

    int getCol()
    {
        return col;
    }

protected:
    //重写定时器事件处理函数
    void timerEvent(QTimerEvent *);

private:
    int row,col;//棋子在棋盘的行列号
    QPixmap *image;//闪烁的图片
    int roleFlag; //角色标记 0:黑 1:白
    int flag;//图标标志
    int time_Id;

};

#endif // MYPUSHBUTTON_H





