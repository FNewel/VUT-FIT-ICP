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

    // 0: Association | 1: Generalization | 2: Composition | 3: Aggregation
    int sourceConnection = 0;
    int targetConnection = 0;

    QGraphicsLineItem *lineItem;    // on samotný
};

extern bool isClicked;
extern QVector <ClassLines*>lines;



#endif // CLASSLINES_H
