/**
 * UML Editor - ICP Project 2022
 * @file messageanchor.cpp
 * @brief Source File for the MessageAnchor Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/objectelement.h"
#include "headers/actorelement.h"
#include "ui_messageanchor.h"

#include <QGraphicsLineItem>
#include <QMoveEvent>
#include <QGraphicsProxyWidget>
#include <QPainter>


MessageAnchor::MessageAnchor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageAnchor)
{
    ui->setupUi(this);
    //Caused movement of the parent object when clicked on side anchors
    this->setAttribute( Qt::WA_NoMousePropagation );
}

MessageAnchor::~MessageAnchor()
{

    //If the parent is an object
    if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement"){
        ObjectElement *parentElement = dynamic_cast<ObjectElement*>(this->parent());
        //Removed from anchors vector, removed the ui element
        parentElement->anchors.remove(this->proxy);
        if(!parentElement->proxyList.empty() && parentElement->proxyList.indexOf(this->proxy) != -1)
            parentElement->proxyList.remove(parentElement->proxyList.indexOf(this->proxy));
    }
    //IF the parent is an actor
    else if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ActorElement"){
        ActorElement *parentElement = dynamic_cast<ActorElement*>(this->parent());
        parentElement->anchors.remove(this->proxy);
        if(!parentElement->proxyList.empty() && parentElement->proxyList.indexOf(this->proxy) != -1)
            parentElement->proxyList.remove(parentElement->proxyList.indexOf(this->proxy));
    }

    //Remove the activation on this anchor
    if(!seq_scene->activations.empty()){
        if(seq_scene->activations.last()->sourceAnchor == this){
            ActivationElement *actPtr = seq_scene->activations.last();
            seq_scene->activations.removeLast();
            delete actPtr;
        }

    }
    //Remove the message on this anchor
    if(!seq_scene->messages.empty()){
        if(seq_scene->messages.last()->sourceAnchor == this){
            SeqMessage *msgPtr = seq_scene->messages.last();
            seq_scene->messages.removeLast();
            delete msgPtr;
        }

    }
    //Call deconstructors
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

//Desturction Icon Creation
void MessageAnchor::mouseDoubleClickEvent(QMouseEvent *event)
{

    (void)event;

    //Check if this is a sude anchor (destructions cannot be created on side anchors)
    ObjectElement *objectElement = nullptr;
    bool isSideAnchor = false;
    if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement"){
        objectElement = dynamic_cast<ObjectElement*>(this->parent());
        if(this == objectElement->leftAnchor || this == objectElement->rightAnchor)
            isSideAnchor = true;
    }
    if(!isSideAnchor){
        //If there is not a destruction icon, create one
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
            //If there is a destruction icon, delete it
            delete this->destructionIcon;
            this->destructionIcon = nullptr;
        }
    }
}

//Messages and activations
void MessageAnchor::mousePressEvent(QMouseEvent *event)
{
    //MEssages are created using the left button
    if(event->button() == Qt::LeftButton){
        //Check if this this is the source of the message
        if(!seq_scene->msgClicked){
            //If this anchor does not yet have a message
            if(this->message == nullptr){
                //Create a new message, append it to the vector of messages but do not yet draw it
                SeqMessage *newMsg = new SeqMessage(this);
                seq_scene->messages.append(newMsg);
                newMsg->sourceAnchor = this;
                newMsg->sourcePos = this->pos()+QPoint(this->width()/2,this->height()/2);

                //Flip the bool
                seq_scene->msgClicked = true;
            }
        //This is the target anchor
        }else{

            //If this anchor does not have a message and it's not the source anchor of the currently created message
            if(this->message == nullptr && !seq_scene->messages.empty() && seq_scene->messages.last()->sourceAnchor != this){
                
                //Set the target parameters for the line and draw it
                SeqMessage *msg = seq_scene->messages.last();
                msg->destAnchor = this;
                msg->destPos = this->pos()+QPoint(this->width()/2,this->height()/2);

                QGraphicsLineItem *newLine = seq_scene->addLine(msg->sourcePos.x(), msg->sourcePos.y(), msg->destPos.x(), msg->destPos.y());
                msg->messageLine = newLine;
                newLine->setZValue(3);
                msg->messageLine->setFlag(QGraphicsItem::ItemIsSelectable);
                //Synchronous message by default
                msg->setArrow(0);

                //Function combo box only for target objects, not actors
                if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement")
                    msg->addComboBox();

                this->message = msg;
                msg->sourceAnchor->message = msg;

                //Flip the bool
                seq_scene->msgClicked = false;
            }
        }
    //Activation creation on right click
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
        //If it is a side anchor, do not create an activation
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
            bool success = false;
            if(!seq_scene->activations.empty()){

                //Find last activation in list of activations
                ActivationElement *act = nullptr;
                act = seq_scene->activations.last();



                //If There is no activation on this anchor
                //AND an activation was found
                //AND this anchor has the same parent object as the source anchor
                //AND this is not the source anchor
                if(this->activation == nullptr && act != nullptr && act->sourceAnchor != nullptr && this->parent() == act->sourceAnchor->parent() && act->sourceAnchor != this){

                    //side anchor check
                    ObjectElement *objectElement = nullptr;
                    bool isSideAnchor = false;
                    if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement"){
                        objectElement = dynamic_cast<ObjectElement*>(this->parent());
                        if(this == objectElement->leftAnchor || this == objectElement->rightAnchor)
                            isSideAnchor = true;
                    }
                    //AND this is not a side anchor
                    if(!isSideAnchor){
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

                        success = true;
                    }

                }
                //If the second anchor did not meet specified requirements, remove the starting point of the activation as well
                //and remove the pointer from vector of activations
                if(!success){
                    if(!seq_scene->activations.empty())
                        seq_scene->activations.removeLast();
                    if(act != nullptr)
                        delete act;
                }

            }
            //Flip the bool
            seq_scene->actClicked = false;
        }
    }
}

//If the parrent is being moved
void MessageAnchor::moveEvent(QMoveEvent *event)
{
    //If this has a destruction icon, move it
    if(this->destructionIcon)
        this->destructionIcon->setPos(this->pos()+QPoint(this->width()/2,this->height()/2));
    //If this anchor has a message connected to it
    if(this->message){
        QLineF newMsgLine;
        //IF this is the source anchor
        if (this->message->sourceAnchor == this){
            newMsgLine = QLineF(event->pos()+QPoint(this->width()/2,this->height()/2), this->message->messageLine->line().p2());
            this->message->sourcePos = event->pos()+QPoint(this->width()/2,this->height()/2);
            //Update the location of the combo box above the line and the arrowhead
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        //If this is the target anchor
        if (this->message->destAnchor == this){
            newMsgLine = QLineF(this->message->messageLine->line().p1(), event->pos()+QPoint(this->width()/2,this->height()/2));
            this->message->destPos = event->pos()+QPoint(this->width()/2,this->height()/2);
            //Update the location of the combo box above the line and the arrowhead
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        this->message->messageLine->setLine(newMsgLine);
    }

    //If this anchor has an activation attached
    if(this->activation){

        //If source anchor
        if (this->activation->sourceAnchor == this){
            this->activation->sourcePos = event->pos()- QPoint(1,1);

        }
        //If destination anchor
        if (this->activation->destAnchor == this){
            this->activation->destPos = event->pos()+QPoint(this->width()+1,this->height()+1);
        }

        //Change the rectangle (move it)
        this->activation->actRect->setRect(this->activation->sourcePos.x(), this->activation->sourcePos.y(),
                                           this->activation->destPos.x() - this->activation->sourcePos.x(),
                                           this->activation->destPos.y() - this->activation->sourcePos.y());
    }


}
