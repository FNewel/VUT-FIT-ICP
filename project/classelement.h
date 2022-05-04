#ifndef CLASSELEMENT_H
#define CLASSELEMENT_H

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include "QDebug"
#include <QPointer>
#include <QLine>
#include <QPainter>
#include <QGraphicsLineItem>
#include "workscene.h"
#include "classlines.h"
#include "ui_classelement.h"
#include "itemobject.h"
#include "classlines.h"
#include "objectelement.h"

namespace Ui {
class ClassElement;
}

class ClassElement : public QWidget
{
    Q_OBJECT

public:
    explicit ClassElement(QWidget *parent = nullptr);
    ~ClassElement();

    Ui::ClassElement *ui;

    QString name = "";
    QVector <ItemObject*> attributes;
    QVector <ItemObject*> methods;
    QVector <ClassLines*> lineItems; // všetky lines jemu patriace

private slots:
    void on_operationAddButton_clicked();
    void on_attributeAddButton_clicked();
    void on_pushButton_clicked();
    void on_name_input_textChanged(const QString &arg1);


protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void linePosCheck(ClassLines *lines, bool update);

private:
    QPoint offset;
};

extern WorkScene *class_scene;
extern WorkScene *seq_scene;

#endif // CLASSELEMENT_H
