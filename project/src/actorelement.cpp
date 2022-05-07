/**
 * UML Editor - ICP Project 2022
 * @file actorelement.cpp
 * @brief Source File for the ActorElement Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/actorelement.h"
#include "headers/objectelement.h"
#include "ui_actorelement.h"


#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <qevent.h>


ActorElement::ActorElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActorElement)
{
    ui->setupUi(this);
    //Create a scene inside the little QGraphicsView
    QGraphicsScene *actorScene = new QGraphicsScene();
    ui->graphicsView->setScene(actorScene);
    //Set dimensions of the view
    actorScene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    //Draw the actor
    QPen pen = QPen(QBrush(Qt::black), 3);
    actorScene->addLine(QLine(20,120,45,80), pen); //Left Leg
    actorScene->addLine(QLine(70,120,45,80), pen); //Right Leg
    actorScene->addLine(QLine(45,80,45,40), pen); //Torso
    actorScene->addLine(QLine(20,60,45,40), pen); //Left Arm
    actorScene->addLine(QLine(70,60,45,40), pen); //Right Arm
    actorScene->addEllipse(35,20,20,20, pen); //Head

    //Connect button signals
    connect(ui->plusLineButton, &QPushButton::clicked, this, &ActorElement::increaseLifeLine);
    connect(ui->minusLineButton, &QPushButton::clicked, this, &ActorElement::decreaseLifeLine);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ActorElement::deleteActor);

}

ActorElement::~ActorElement()
{
    //Remove pointer to this object element on destruction
    if(!seq_scene->actors.empty())
        seq_scene->actors.removeOne(this);
    //Remove the life line
    if(this->lifeLine)
        delete this->lifeLine;

    delete ui;
}

QGraphicsLineItem *ActorElement::createLifeLine(ActorElement *actorPtr)
{
    //Create the lifeline below the actor
    QPoint p1 = QPoint(actorPtr->pos().x()+(actorPtr->width()/2), actorPtr->pos().y()+(actorPtr->height()));
    QPoint p2 = QPoint(actorPtr->pos().x()+(actorPtr->width()/2), actorPtr->pos().y()+(actorPtr->height()) + 50);
    QLine lifeLine = QLine(p1, p2);
    QPen dashLine = QPen(Qt::DashLine);
    QGraphicsLineItem *lifeLinePtr = seq_scene->addLine(lifeLine, dashLine);
    //Set the Z value to zero so it appears below other, more important elements
    lifeLinePtr->setZValue(0);

    //Add the first anchor
    MessageAnchor * newAnchor = new MessageAnchor(this);
    QGraphicsProxyWidget * newProxy = seq_scene->addWidget(newAnchor);
    //Set Z value
    newProxy->setZValue(2);
    newAnchor->proxy = newProxy;
    newProxy->setPos(p2-QPoint(7,0));
    //Add to the Dictionary and Vector of this actor element
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);

    return lifeLinePtr;
}


void ActorElement::increaseLifeLine()
{
    //If no lifeline was yet created, create it
    if (this->lifeLine == nullptr){
        this->lifeLine = createLifeLine(this);

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
    newProxy->setZValue(2);
    newAnchor->proxy = newProxy;
    newProxy->setPos(this->lifeLine->pos()+p2Point-QPoint(7,0));
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);
}

void ActorElement::decreaseLifeLine()
{
    //Do nothing if there is not a lifeline
    if (this->lifeLine == nullptr)
        return;

    //Get current points of the lifeline
    QPointF p1Point = this->lifeLine->line().p1();
    QPointF p2Point = this->lifeLine->line().p2();

    //If we are not at the last anchor
    if (p2Point.y() - p1Point.y() > 50){
        //Decrease line length
        p2Point.setY(p2Point.y() - 50);
        QLineF newLifeLine = QLineF(this->lifeLine->line().p1(), p2Point);
        this->lifeLine->setLine(newLifeLine);


        //Destory the anchor
        QGraphicsProxyWidget *deletedAnchorProxy = this->proxyList.takeLast();;
        this->anchors.remove(deletedAnchorProxy);
        delete deletedAnchorProxy;
    }
}

void ActorElement::deleteActor()
{
    //Delete later - just in case to avoid some crashes
    this->deleteLater();
}

void ActorElement::mousePressEvent(QMouseEvent *event)
{
    //Set offset of the mouse relative to the actor element
    offset = event->pos();
}

void ActorElement::mouseMoveEvent(QMouseEvent *event)
{
    //If the lest button was pressed
    if(event->buttons() & Qt::LeftButton) {
        //Move the acotr
        this->move(mapToParent(event->pos() - offset));
        //Move the life line
        if (this->lifeLine)
            this->lifeLine->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());

        //Move the anchors
        foreach(QGraphicsProxyWidget * key, this->anchors.keys()){
            key->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        }


    }
}

void ActorElement::on_lineEdit_textChanged(const QString &arg1)
{
    //Set the name if it has been changed
    this->name = arg1;
}
