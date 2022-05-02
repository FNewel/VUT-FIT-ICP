#ifndef OBJECTELEMENT_H
#define OBJECTELEMENT_H

#include <QWidget>
#include "workscene.h"

namespace Ui {
class ObjectElement;
}

class ObjectElement : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectElement(QWidget *parent = nullptr);
    QGraphicsLineItem * createLifeLine(ObjectElement *objectPtr);
    ~ObjectElement();

private:
    Ui::ObjectElement *ui;
    QPoint offset;
    QGraphicsLineItem * lifeLine = nullptr;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private slots:

    void increaseLifeLine();
    void decreaseLifeLine();
    void deleteObject();
};



extern WorkScene *class_scene;
extern WorkScene *seq_scene;


#endif // OBJECTELEMENT_H
