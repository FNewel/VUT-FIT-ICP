#ifndef CLASSLINES_H
#define CLASSLINES_H

#include <QWidget>
#include <QPointer>

class ClassLines {

public:
    QPointer<QWidget> source;
    QPointer<QWidget> target;

    QPoint sourcePos;
    QPoint targetPos;
};

extern bool isClicked;
extern QVector <ClassLines>lines;



#endif // CLASSLINES_H
