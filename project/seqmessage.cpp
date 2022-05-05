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
    if(this->sourceAnchor)
        this->sourceAnchor->message = nullptr;

    if(this->destAnchor)
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

}

void SeqMessage::updateCBoxLoc()
{
    if(messageNameProxy)
        messageNameProxy->setPos(this->sourcePos.x()-(this->sourcePos.x()-this->destPos.x())/2 - this->messageName->width()/2, this->sourcePos.y()-(this->sourcePos.y()-this->destPos.y())/2 - 30);
}

void SeqMessage::updateArrowHead()
{
    if(this->arrowHeadProxy){
        qreal angle = qAtan2(this->sourcePos.y() - destPos.y(), this->sourcePos.x() - destPos.x());
        qreal degrees = qRadiansToDegrees(angle);
        this->arrowHeadProxy->setRotation(degrees + 180);
        this->arrowHeadProxy->setPos(this->destPos);
    }
}

void SeqMessage::setArrow(int arrowType)
{
    if(this->arrowHeadProxy){
        delete this->arrowHeadProxy;
        this->arrowHeadProxy = nullptr;
    }
    QBrush brush = QBrush(Qt::black);
    QPen penSolid = QPen(brush, 3, Qt::SolidLine);
    QPen penDashed = QPen(brush, 3, Qt::DashLine);
    if(arrowType == SYNC_ARROW){
        this->messageLine->setPen(penSolid);
        QPolygon arrowHead;
        arrowHead << QPoint(-10,-10) << QPoint(-10,10) << QPoint(0,0);
        QGraphicsPolygonItem *polygonProxy = seq_scene->addPolygon(arrowHead, penSolid, brush);
        polygonProxy->setZValue(3);
        this->arrowHeadProxy = polygonProxy;
        this->arrowHeadProxy->setParentItem(this->messageLine);
        this->updateArrowHead();
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

    }else if(arrowType == RETURN_ARROW){
        this->setArrow(ASYNC_ARROW);
        this->messageLine->setPen(penDashed);
    }
    this->messageType = arrowType;
}
