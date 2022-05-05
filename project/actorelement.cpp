#include "actorelement.h"
#include "ui_actorelement.h"
#include "objectelement.h"

#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <qevent.h>


ActorElement::ActorElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActorElement)
{
    ui->setupUi(this);
    QGraphicsScene *actorScene = new QGraphicsScene();
    ui->graphicsView->setScene(actorScene);
    actorScene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    //90x120 (0,120)->(90,0) midX = 45, midY = 60
    QPen pen = QPen(QBrush(Qt::black), 3);
    actorScene->addLine(QLine(20,120,45,80), pen); //Left Leg
    actorScene->addLine(QLine(70,120,45,80), pen); //Right Leg
    actorScene->addLine(QLine(45,80,45,40), pen); //Torso
    actorScene->addLine(QLine(20,60,45,40), pen); //Left Arm
    actorScene->addLine(QLine(70,60,45,40), pen); //Right Arm
    actorScene->addEllipse(35,20,20,20, pen); //Head

    connect(ui->plusLineButton, &QPushButton::clicked, this, &ActorElement::increaseLifeLine);
    connect(ui->minusLineButton, &QPushButton::clicked, this, &ActorElement::decreaseLifeLine);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ActorElement::deleteActor);

}

ActorElement::~ActorElement()
{
    //Remove pointer to this object element on destruction
    seq_scene->actors.removeOne(this);
    if(this->lifeLine)
        delete this->lifeLine;

    delete ui;
}

QGraphicsLineItem *ActorElement::createLifeLine(ActorElement *actorPtr)
{
    QPoint p1 = QPoint(actorPtr->pos().x()+(actorPtr->width()/2), actorPtr->pos().y()+(actorPtr->height()));
    QPoint p2 = QPoint(actorPtr->pos().x()+(actorPtr->width()/2), actorPtr->pos().y()+(actorPtr->height()) + 50);
    QLine lifeLine = QLine(p1, p2);
    QPen dashLine = QPen(Qt::DashLine);
    QGraphicsLineItem *lifeLinePtr = seq_scene->addLine(lifeLine, dashLine);
    lifeLinePtr->setZValue(0);

    MessageAnchor * newAnchor = new MessageAnchor(this);
    QGraphicsProxyWidget * newProxy = seq_scene->addWidget(newAnchor);
    newProxy->setZValue(2);
    newAnchor->proxy = newProxy;
    newProxy->setPos(p2-QPoint(7,0));
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);

    return lifeLinePtr;
}


void ActorElement::increaseLifeLine()
{
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

void ActorElement::deleteActor()
{
    this->deleteLater();
}

void ActorElement::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}

void ActorElement::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));
        if (this->lifeLine)
            this->lifeLine->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());

        foreach(QGraphicsProxyWidget * key, this->anchors.keys()){
            key->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        }


    }
}

void ActorElement::on_lineEdit_textChanged(const QString &arg1)
{
    this->name = arg1;
}
