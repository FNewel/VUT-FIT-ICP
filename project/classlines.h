#ifndef CLASSLINES_H
#define CLASSLINES_H

#include <QWidget>
#include <QPointer>
#include <QGraphicsItem>
#include <QLine>

class ClassLines {

public:
    QGraphicsLineItem *lineItem;

    QPointer<QWidget> source;
    QPointer<QWidget> target;

    QPoint sourcePos;
    QPoint targetPos;
};

extern bool isClicked;
extern QVector <ClassLines*>lines;



#endif // CLASSLINES_H
