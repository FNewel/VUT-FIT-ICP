/**
 * UML Editor - ICP Project 2022
 * @file seqmessage.cpp
 * @brief Source File for the SeqMessage Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/objectelement.h"
#include "headers/classelement.h"
#include "headers/itemobject.h"
#include "ui_objectelement.h"

#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QtMath>


#define SYNC_ARROW 0
#define ASYNC_ARROW 1
#define RETURN_ARROW 2


SeqMessage::SeqMessage(QWidget *parent) : QWidget(parent)
{

}

SeqMessage::~SeqMessage()
{

    //Remove this as the pointer from the source anchor
    if(this->sourceAnchor)
        this->sourceAnchor->message = nullptr;

    //Remove this as the pointer from the target anchor
    if(this->destAnchor)
          this->destAnchor->message = nullptr;

    //Remove this from the vector of messages
    seq_scene->messages.removeOne(this);

    //Remove the message line
    if(this->messageLine)
        delete this->messageLine;

    //Remove the combo box
    if(this->messageName)
        delete this->messageName;
}

//Add the combo box above the line
void SeqMessage::addComboBox()
{
    //Create
    this->messageName = new QComboBox();
    this->messageNameProxy = seq_scene->addWidget(messageName);
    //Updaqte location and available methods
    this->updateCBoxLoc();
    updateMessages();

}

//Update the available methods for messages
void SeqMessage::updateMessages()
{
    //Only update if combobox actually exists (it does not for messages targeted at actors)
    if(this->messageNameProxy){
        ObjectElement* parentObject = qobject_cast <ObjectElement*>(this->destAnchor->parent());
        QString className = parentObject->ui->comboBox->currentText();
        ClassElement *classPtr = nullptr;
        foreach(ClassElement *classElement, class_scene->classes){
            if(classElement->name == className){
                classPtr = classElement;
                break;
            }
        }

        //Classes with possible generalization pathss
        QVector<ClassElement*> classesGenPossible;
        //Classest hat are actually generalizations
        QVector<ClassElement*> classesGenTrue;


        //Prevents cyclic inheritance crash
        QVector<ClassElement*> visitedVector;



        if(classPtr){
            //Add classptr to both
            classesGenPossible.append(classPtr);
            classesGenTrue.append(classPtr);
            //Call recursive function

            this->findGen(&classesGenPossible,&classesGenTrue, &visitedVector);

        }

        //If not yet in comboBox, add
        foreach(ClassElement* classElement, classesGenTrue){
            foreach(ItemObject *itemObject, classElement->methods){
                if(this->messageName->findText(itemObject->value) == -1)
                    this->messageName->addItem(itemObject->value);

            }
        }

        //Remove all items from comboBox that are no longer valid methods
        //If method name no longer exists empty the combo box
        bool deletedLastItem = false;
        for(int i = 0; i < this->messageName->count(); i++){
            if (deletedLastItem)
                i--;
            bool validMessage = false;
            foreach(ClassElement* classElement, classesGenTrue){
                foreach(ItemObject *itemObject, classElement->methods){
                    if(this->messageName->itemText((i)) == itemObject->value)
                        validMessage = true;
                }
            }
            if (!validMessage){
                if(this->messageName->currentText() == this->messageName->itemText(i))
                    this->messageName->setCurrentIndex(-1);

                this->messageName->removeItem(i);
                deletedLastItem = true;

            }else{
                deletedLastItem = false;
            }
        }
    }

}

void SeqMessage::updateCBoxLoc()
{
    //Update the location of the combo box
    if(messageNameProxy)
        messageNameProxy->setPos(this->sourcePos.x()-(this->sourcePos.x()-this->destPos.x())/2 - this->messageName->width()/2, this->sourcePos.y()-(this->sourcePos.y()-this->destPos.y())/2 - 30);
}

//Update the arrowhead location and rotation
void SeqMessage::updateArrowHead()
{
    if(this->arrowHeadProxy){
        qreal angle = qAtan2(this->sourcePos.y() - destPos.y(), this->sourcePos.x() - destPos.x());
        qreal degrees = qRadiansToDegrees(angle);
        this->arrowHeadProxy->setRotation(degrees + 180);
        this->arrowHeadProxy->setPos(this->destPos);
    }
}

//Update the type of message arrow
void SeqMessage::setArrow(int arrowType)
{
    //Delete current arrowhead
    if(this->arrowHeadProxy){
        delete this->arrowHeadProxy;
        this->arrowHeadProxy = nullptr;
    }
    //Set up burshes and pens
    QBrush brush = QBrush(Qt::black);
    QPen penSolid = QPen(brush, 3, Qt::SolidLine);
    QPen penDashed = QPen(brush, 3, Qt::DashLine);

    //Sync message -> filled arrow, solid line
    if(arrowType == SYNC_ARROW){
        this->messageLine->setPen(penSolid);
        QPolygon arrowHead;
        arrowHead << QPoint(-10,-10) << QPoint(-10,10) << QPoint(0,0);
        QGraphicsPolygonItem *polygonProxy = seq_scene->addPolygon(arrowHead, penSolid, brush);
        polygonProxy->setZValue(3);
        this->arrowHeadProxy = polygonProxy;
        this->arrowHeadProxy->setParentItem(this->messageLine);
        this->updateArrowHead();
    //Async message -> V arrow, solid line
    }else if(arrowType == ASYNC_ARROW){
        this->messageLine->setPen(penSolid);
        QLine line1 = QLine(-10,-10,0,0);
        QLine line2 = QLine(-10,10,0,0);
        QGraphicsLineItem *line1Proxy = seq_scene->addLine(line1, penSolid);
        QGraphicsLineItem *line2Proxy = seq_scene->addLine(line2, penSolid);
        line1Proxy->setZValue(3);
        line2Proxy->setZValue(3);
        line2Proxy->setParentItem(line1Proxy);
        line1Proxy->setPos(this->destPos);
        this->arrowHeadProxy = line1Proxy;
        this->arrowHeadProxy->setParentItem(this->messageLine);
        this->updateArrowHead();

    //Return message -> V arrow, dashed line
    }else if(arrowType == RETURN_ARROW){
        this->setArrow(ASYNC_ARROW);
        this->messageLine->setPen(penDashed);
    }
    this->messageType = arrowType;
}

//Find the methods accesible via generalization
void SeqMessage::findGen(QVector<ClassElement *> *possVector, QVector<ClassElement *> *trueVector, QVector<ClassElement*> *visited)

{
    //End if the vector of possible class elements is empty
    if(possVector->empty()){
        return;
    }

    ClassElement* inspectedClass = possVector->takeLast();
    foreach(ClassLines* line, inspectedClass->lineItems){
        //If thiso bject is source of this line and the target is a generalization, add this item to the vector of generalizations
        if(line->source == inspectedClass && line->targetConnection == 1){

            if(!visited->contains(line->target)){
                possVector->append(line->target);
                trueVector->append(line->target);
                visited->append(line->target);
            }
        }
        //Recursive call
        findGen(possVector, trueVector, visited);

    }
}
