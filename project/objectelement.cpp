#include "objectelement.h"
#include "ui_objectelement.h"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QPushButton>

ObjectElement::ObjectElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectElement)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Object Name");

    foreach(ClassElement *classElement, class_scene->classes){
        qDebug() << classElement;
    }

    connect(ui->plusLineButton, &QPushButton::clicked, this, &ObjectElement::increaseLifeLine);
    connect(ui->minusLineButton, &QPushButton::clicked, this, &ObjectElement::decreaseLifeLine);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ObjectElement::deleteObject);


}

ObjectElement::~ObjectElement()
{
    //Remove pointer to this object element on destruction
    seq_scene->objects.removeOne(this);
    if(this->lifeLine){
        delete this->lifeLine;
    }
    delete ui;
}

void ObjectElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}

void ObjectElement::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));
        if (this->lifeLine)
            this->lifeLine->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
    }
}

void ObjectElement::increaseLifeLine()
{
    if (this->lifeLine == nullptr){
        this->lifeLine = createLifeLine(this);
        return;
    }

    QPointF p2Point = this->lifeLine->line().p2();
    p2Point.setY(p2Point.y() + 50);
    QLineF newLifeLine = QLineF(this->lifeLine->line().p1(), p2Point);
    this->lifeLine->setLine(newLifeLine);
}

void ObjectElement::decreaseLifeLine()
{
    if (this->lifeLine == nullptr)
        return;
    QPointF p1Point = this->lifeLine->line().p1();
    QPointF p2Point = this->lifeLine->line().p2();

    if (p2Point.y() - p1Point.y() > 50){
        p2Point.setY(p2Point.y() - 50);
        QLineF newLifeLine = QLineF(this->lifeLine->line().p1(), p2Point);
        this->lifeLine->setLine(newLifeLine);
    }
}

void ObjectElement::deleteObject()
{
    this->deleteLater();
}

QGraphicsLineItem * ObjectElement::createLifeLine(ObjectElement* objectPtr){
    QLine lifeLine = QLine(QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height())), QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height()) + 50));
    QPen dashLine = QPen();
    QGraphicsLineItem *lifeLinePtr = seq_scene->addLine(lifeLine, dashLine);
    return lifeLinePtr;
}
