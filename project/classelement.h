#ifndef CLASSELEMENT_H
#define CLASSELEMENT_H

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include "workscene.h"
#include "classlines.h"

namespace Ui {
class ClassElement;
}

class ClassElement : public QWidget
{
    Q_OBJECT

public:
    explicit ClassElement(QWidget *parent = nullptr);
    ~ClassElement();
    //WorkScene *main_scene;

private slots:
    void on_operationAddButton_clicked();

    void on_attributeAddButton_clicked();

    void on_pushButton_clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void linePosCheck();

private:
    Ui::ClassElement *ui;
    QPoint offset;
};

extern WorkScene *class_scene;

#endif // CLASSELEMENT_H
