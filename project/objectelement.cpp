#include "objectelement.h"
#include "ui_objectelement.h"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QMap>

bool __gl__msgClicked = false;
QVector <SeqMessage *>__gl__messages;

ObjectElement::ObjectElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectElement)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Object Name");


    // TODO: idk prečo je tu toto, dám do komentáru
    /*
    foreach(ClassElement *classElement, class_scene->classes){
        qDebug() << classElement;
    }*/

    connect(ui->plusLineButton, &QPushButton::clicked, this, &ObjectElement::increaseLifeLine);
    connect(ui->minusLineButton, &QPushButton::clicked, this, &ObjectElement::decreaseLifeLine);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ObjectElement::deleteObject);


}

ObjectElement::~ObjectElement()
{
    //Remove pointer to this object element on destruction
    seq_scene->objects.removeOne(this);
    if(this->lifeLine)
        delete this->lifeLine;

    //delete this->leftAnchor;
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
        if (this->leftAnchorProxy)
            this->leftAnchorProxy->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        if (this->rightAnchorProxy)
            this->rightAnchorProxy->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());

        foreach(QGraphicsProxyWidget * key, this->anchors.keys()){
            key->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        }


    }
}

void ObjectElement::increaseLifeLine()
{
    if (this->lifeLine == nullptr){
        this->lifeLine = createLifeLine(this);

        this->leftAnchor = new MessageAnchor(this);
        this->leftAnchorProxy = seq_scene->addWidget(this->leftAnchor);
        this->leftAnchorProxy->setPos(QPointF(this->pos().x()-8, this->pos().y()+(this->height()/2)));

        this->rightAnchor = new MessageAnchor(this);
        this->rightAnchorProxy = seq_scene->addWidget(this->rightAnchor);
        this->rightAnchorProxy->setPos(QPointF(this->pos().x()+this->width()-7, this->pos().y()+(this->height()/2)));
        return;
    }

    //Make Line Longer
    QPointF p2Point = this->lifeLine->line().p2();
    p2Point.setY(p2Point.y() + 50);
    QLineF newLifeLine = QLineF(this->lifeLine->line().p1(), p2Point);
    this->lifeLine->setLine(newLifeLine);


    //Attach anchor to the end of the line
    MessageAnchor * newAnchor = new MessageAnchor(this);
    QGraphicsProxyWidget * newProxy = seq_scene->addWidget(newAnchor);
    newAnchor->proxy = newProxy;
    newProxy->setPos(this->lifeLine->pos()+p2Point-QPoint(7,0));
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);
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



        QGraphicsProxyWidget *deletedAnchorProxy = this->proxyList.takeLast();;
        this->anchors.remove(deletedAnchorProxy);
        delete deletedAnchorProxy;
    }
}

void ObjectElement::deleteObject()
{
    this->deleteLater();
}

QGraphicsLineItem * ObjectElement::createLifeLine(ObjectElement* objectPtr){
    QPoint p1 = QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height()));
    QPoint p2 = QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height()) + 50);
    QLine lifeLine = QLine(p1, p2);
    QPen dashLine = QPen(Qt::DashLine);
    QGraphicsLineItem *lifeLinePtr = seq_scene->addLine(lifeLine, dashLine);

    MessageAnchor * newAnchor = new MessageAnchor(this);
    QGraphicsProxyWidget * newProxy = seq_scene->addWidget(newAnchor);
    newAnchor->proxy = newProxy;
    newProxy->setPos(p2-QPoint(7,0));
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);

    return lifeLinePtr;
}
