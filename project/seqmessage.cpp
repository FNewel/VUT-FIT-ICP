#include "objectelement.h"
#include "ui_objectelement.h"
#include "classelement.h"

#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <itemobject.h>
#include <QtMath>

#define SYNC_ARROW 0
#define ASYNC_ARROW 1
#define RETURN_ARROW 2

SeqMessage::SeqMessage(QWidget *parent) : QWidget(parent)
{

}

SeqMessage::~SeqMessage()
{
    this->sourceAnchor->message = nullptr;
    this->destAnchor->message = nullptr;
    if(this->messageLine)
        delete this->messageLine;
    if(this->messageName)
        delete this->messageName;
}

void SeqMessage::addComboBox()
{
    this->messageName = new QComboBox();
    this->messageNameProxy = seq_scene->addWidget(messageName);
    this->updateCBoxLoc();

    ObjectElement* parentObject = qobject_cast <ObjectElement*>(this->destAnchor->parent());
    QString className = parentObject->ui->comboBox->currentText();
    ClassElement *classPtr = nullptr;
    foreach(ClassElement *classElement, class_scene->classes){
        if(classElement->name == className){
            classPtr = classElement;
            break;
        }
    }

    if(classPtr){
        foreach(ItemObject *itemObject, classPtr->methods){
            this->messageName->addItem(itemObject->value);
        }
    }

}

void SeqMessage::updateMessages()
{
    ObjectElement* parentObject = qobject_cast <ObjectElement*>(this->destAnchor->parent());
    QString className = parentObject->ui->comboBox->currentText();
    ClassElement *classPtr = nullptr;
    foreach(ClassElement *classElement, class_scene->classes){
        if(classElement->name == className){
            classPtr = classElement;
            break;
        }
    }

    //If not yet in comboBox, add
    if(classPtr){
        foreach(ItemObject *itemObject, classPtr->methods){
            if(this->messageName->findText(itemObject->value) == -1)
                this->messageName->addItem(itemObject->value);

        }

        //Remove all items from comboBox that are no longer valid methods
        //If method name no longer exists empty the combo box
        bool deletedLastItem = false;
        for(int i = 0; i < this->messageName->count(); i++){
            if (deletedLastItem)
                i--;
            bool validMessage = false;
            foreach(ItemObject *itemObject, classPtr->methods){
                if(this->messageName->itemText((i)) == itemObject->value)
                    validMessage = true;
            }
            if (!validMessage){
                if(this->messageName->currentText() == this->messageName->itemText(i))
                    this->messageName->setCurrentIndex(-1);

                this->messageName->removeItem(i);
                deletedLastItem = true;

            }else{
                deletedLastItem = false;
            }
            qDebug() << "Infinite Loop Check: " << i;
        }
    }

}

void SeqMessage::updateCBoxLoc()
{
    if(messageNameProxy)
        messageNameProxy->setPos(this->sourcePos.x()-(this->sourcePos.x()-this->destPos.x())/2 - this->messageName->width()/2, this->sourcePos.y()-(this->sourcePos.y()-this->destPos.y())/2 - 30);
}

void SeqMessage::updateArrowHead()
{
    qreal angle = qAtan2(this->sourcePos.y() - destPos.y(), this->sourcePos.x() - destPos.x());
    qreal degrees = qRadiansToDegrees(angle);
    this->arrowHeadProxy->setRotation(degrees + 180);
    this->arrowHeadProxy->setPos(this->destPos);
}

void SeqMessage::setArrow(int arrowType)
{

    if(arrowType == SYNC_ARROW){
        QPolygon arrowHead;
        arrowHead << QPoint(-10,-10) << QPoint(-10,10) << QPoint(0,0);
        this->arrowHeadProxy = seq_scene->addPolygon(arrowHead, QPen(Qt::black), QBrush(Qt::black));
        this->arrowHeadProxy->setParentItem(this->messageLine);
        this->updateArrowHead();
    }else if(arrowType == ASYNC_ARROW){

    }else if(arrowType == RETURN_ARROW){

    }
}
