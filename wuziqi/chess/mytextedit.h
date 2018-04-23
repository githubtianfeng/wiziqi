#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget *parent=0);
    ~MyTextEdit();


signals:
    void sendChatMessage(QString);

protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // MYTEXTEDIT_H



