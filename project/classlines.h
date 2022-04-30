#ifndef CLASSLINES_H
#define CLASSLINES_H

#include <QWidget>
#include <QPointer>

namespace Ui {
class ClassElement;
}

class ClassLines {

public:
    QPointer<QWidget> source;
    QPointer<QWidget> target;

    QPoint sourcePos;
    QPoint targetPos;
};

bool isClicked = false;
static QVector <ClassLines>lines;



#endif // CLASSLINES_H
