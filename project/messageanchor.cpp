#include "objectelement.h"
#include "actorelement.h"
#include "ui_messageanchor.h"

#include <QGraphicsLineItem>
#include <QMoveEvent>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPainter>




MessageAnchor::MessageAnchor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageAnchor)
{
    ui->setupUi(this);
    this->setAttribute( Qt::WA_NoMousePropagation );
}

MessageAnchor::~MessageAnchor()
{

    if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement"){
        ObjectElement *parentElement = dynamic_cast<ObjectElement*>(this->parent());
        parentElement->anchors.remove(this->proxy);
        if(!parentElement->proxyList.empty() && parentElement->proxyList.indexOf(this->proxy) != -1)
            parentElement->proxyList.remove(parentElement->proxyList.indexOf(this->proxy));
    }
    else if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ActorElement"){
        ActorElement *parentElement = dynamic_cast<ActorElement*>(this->parent());
        parentElement->anchors.remove(this->proxy);
        if(!parentElement->proxyList.empty() && parentElement->proxyList.indexOf(this->proxy) != -1)
            parentElement->proxyList.remove(parentElement->proxyList.indexOf(this->proxy));
    }


    if (this->message){
        delete this->message;
    }
    if(this->activation)
        delete this->activation;

    if(this->destructionIcon){
        delete this->destructionIcon;
    }


    delete ui;
}

void MessageAnchor::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!this->destructionIcon){
        QBrush brush = QBrush(Qt::black);
        QPen pen = QPen(brush, 3);
        QLine line1 = QLine(20,20,-20,-20);
        QLine line2 = QLine(-20,20,20,-20);
        QGraphicsLineItem * line1Proxy = seq_scene->addLine(line1, pen);
        QGraphicsLineItem * line2Proxy = seq_scene->addLine(line2, pen);
        line1Proxy->setZValue(3);
        line2Proxy->setZValue(3);
        line2Proxy->setParentItem(line1Proxy);
        this->destructionIcon = line1Proxy;
        this->destructionIcon->setPos(this->pos()+QPoint(this->width()/2,this->height()/2));
    }else{
        delete this->destructionIcon;
        this->destructionIcon = nullptr;
    }
}

void MessageAnchor::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(!seq_scene->msgClicked){
            if(this->message == nullptr){
                SeqMessage *newMsg = new SeqMessage(this);
                seq_scene->messages.append(newMsg);
                newMsg->sourceAnchor = this;
                newMsg->sourcePos = this->pos()+QPoint(this->width()/2,this->height()/2);

                 seq_scene->msgClicked = true;
            }
        }else{

            if(this->message == nullptr && seq_scene->messages.last()->sourceAnchor != this){
                SeqMessage *msg = seq_scene->messages.last();
                msg->destAnchor = this;
                msg->destPos = this->pos()+QPoint(this->width()/2,this->height()/2);

                QGraphicsLineItem *newLine = seq_scene->addLine(msg->sourcePos.x(), msg->sourcePos.y(), msg->destPos.x(), msg->destPos.y());
                msg->messageLine = newLine;
                newLine->setZValue(3);
                msg->messageLine->setFlag(QGraphicsItem::ItemIsSelectable);
                msg->setArrow(0);

                //Function combo box only for target objects, not actors
                if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement")
                    msg->addComboBox();

                this->message = msg;
                msg->sourceAnchor->message = msg;


                seq_scene->msgClicked = false;
            }
        }
    }else if(event->button() == Qt::RightButton){
        //Check if currently clicked anchor is not one of the side anchors on objects (they cannot anchor activations)
        bool leftOrRightAnchor = true;
        if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement"){
            ObjectElement *parentElement = dynamic_cast<ObjectElement*>(this->parent());
            foreach(QGraphicsProxyWidget* anchorProxy, parentElement->proxyList){
                if(this->proxy == anchorProxy){
                    leftOrRightAnchor = false;
                }
            }
        }
        else if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ActorElement"){
            ActorElement *parentElement = dynamic_cast<ActorElement*>(this->parent());
            foreach(QGraphicsProxyWidget* anchorProxy, parentElement->proxyList){
                if(this->proxy == anchorProxy){
                    leftOrRightAnchor = false;
                }
            }
        }
        if(leftOrRightAnchor){
            return;
        }

        //First Click
        if(!seq_scene->actClicked){

            //If this anchor does not yet have activation
            if(this->activation == nullptr){
                ActivationElement *newAct = new ActivationElement(this);
                seq_scene->activations.append(newAct);
                newAct->sourceAnchor = this;
                newAct->sourcePos = this->pos() - QPoint(1,1);//Move by 1px so the border loks better with anchors

                seq_scene->actClicked = true;
            }
        //Second Click
        }else{
            if(this->activation == nullptr && seq_scene->activations.last()->sourceAnchor != this){

                //Find last activation in list of activations
                ActivationElement *act = nullptr;
                if(!seq_scene->activations.empty())
                    act = seq_scene->activations.last();



                //If activation has been found AND this anchor is on the same element as source anchor
                if(act != nullptr && this->parent() == act->sourceAnchor->parent()){

                    act->destAnchor = this;
                    act->destPos = this->pos()+QPoint(this->width()+1,this->height()+1);

                    //If source anchor is lower than destination anchor, swap them
                    if(act->destPos.y() < act->sourcePos.y()){
                        act->destAnchor = act->sourceAnchor;
                        act->sourceAnchor = this;

                        act->sourcePos = act->sourceAnchor->pos()- QPoint(1,1);
                        act->destPos = act->destAnchor->pos()+QPoint(this->width()+1,this->height()+1);;

                        this->activation = act;
                        act->destAnchor->activation = act;
                    }

                    //Draw the rectangle
                    QRect activationRect = QRect(act->sourcePos.x(), act->sourcePos.y(), act->destPos.x() - act->sourcePos.x(), act->destPos.y() - act->sourcePos.y());
                    QGraphicsRectItem *newRect = seq_scene->addRect(activationRect);
                    newRect->setBrush(Qt::white);
                    newRect->setZValue(1);
                    act->actRect = newRect;
                    act->actRect->setFlag(QGraphicsItem::ItemIsSelectable);


                    this->activation = act;
                    act->sourceAnchor->activation = act;
                //If the second anchor did not meet specified requirements, remove the starting point of the activation as well
                }else{
                    if(!seq_scene->activations.empty())
                        seq_scene->activations.removeLast();
                    if(act != nullptr)
                        delete act;
                }
                seq_scene->actClicked = false;
            }
        }
    }
}

void MessageAnchor::moveEvent(QMoveEvent *event)
{
    if(this->destructionIcon)
        this->destructionIcon->setPos(this->pos()+QPoint(this->width()/2,this->height()/2));
    if(this->message){
        QLineF newMsgLine;
        if (this->message->sourceAnchor == this){
            newMsgLine = QLineF(event->pos()+QPoint(this->width()/2,this->height()/2), this->message->messageLine->line().p2());
            this->message->sourcePos = event->pos()+QPoint(this->width()/2,this->height()/2);
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        if (this->message->destAnchor == this){
            newMsgLine = QLineF(this->message->messageLine->line().p1(), event->pos()+QPoint(this->width()/2,this->height()/2));
            this->message->destPos = event->pos()+QPoint(this->width()/2,this->height()/2);
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        this->message->messageLine->setLine(newMsgLine);
    }

    if(this->activation){

        if (this->activation->sourceAnchor == this){
            this->activation->sourcePos = event->pos()- QPoint(1,1);

        }
        if (this->activation->destAnchor == this){
            this->activation->destPos = event->pos()+QPoint(this->width()+1,this->height()+1);
        }


        this->activation->actRect->setRect(this->activation->sourcePos.x(), this->activation->sourcePos.y(),
                                           this->activation->destPos.x() - this->activation->sourcePos.x(),
                                           this->activation->destPos.y() - this->activation->sourcePos.y());
    }


}
