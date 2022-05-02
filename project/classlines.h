#ifndef CLASSLINES_H
#define CLASSLINES_H

#include <QWidget>
#include <QPointer>
#include <QGraphicsItem>
#include <QLine>
#include "classelement.h"

class ClassLines {

public:
    ClassElement *source;
    ClassElement *target;

    QPoint sourcePos;
    QPoint targetPos;

    QGraphicsLineItem *lineItem;    // niečo ako last
    QVector <QGraphicsLineItem*> lineItems; // všetky lines
};

extern bool isClicked;
extern QVector <ClassLines*>lines;



#endif // CLASSLINES_H
