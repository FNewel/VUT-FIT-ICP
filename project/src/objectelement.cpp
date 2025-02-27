/**
 * UML Editor - ICP Project 2022
 * @file objectelement.cpp
 * @brief Source File for the ObjectElement Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/objectelement.h"
#include "headers/classelement.h"
#include "ui_objectelement.h"

#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QMap>


bool __gl__msgClicked = false; //First-Second click bool for messages in seq diagram
bool __gl__actClicked = false; //First-Second click bool for activation in seq diagram

ObjectElement::ObjectElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectElement)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Object Name");


    foreach(ClassElement *classElement, class_scene->classes){
        ui->comboBox->addItem(classElement->name);
    }

    //Connect the buttons to slots
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
    //Set mouse offset
    offset = event->pos();
}

void ObjectElement::mouseMoveEvent(QMouseEvent *event)
{
    //If left click, move the element
    if(event->buttons() & Qt::LeftButton) {
        this->move(mapToParent(event->pos() - offset));
        //Move the lifeline and side anchors
        if (this->lifeLine)
            this->lifeLine->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        if (this->leftAnchorProxy)
            this->leftAnchorProxy->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        if (this->rightAnchorProxy)
            this->rightAnchorProxy->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());

        //Move all the anchors
        foreach(QGraphicsProxyWidget * key, this->anchors.keys()){
            key->moveBy(event->pos().x() - offset.x(), event->pos().y() - offset.y());
        }


    }
}

void ObjectElement::increaseLifeLine()
{
    //Create the lifeline if it does not exist
    if (this->lifeLine == nullptr){
        this->lifeLine = createLifeLine(this);

        //Create left anchor
        this->leftAnchor = new MessageAnchor(this);
        this->leftAnchorProxy = seq_scene->addWidget(this->leftAnchor);
        this->leftAnchorProxy->setPos(QPointF(this->pos().x()-8, this->pos().y()+(this->height()-50)));
        //Create right anchor
        this->rightAnchor = new MessageAnchor(this);
        this->rightAnchorProxy = seq_scene->addWidget(this->rightAnchor);
        this->rightAnchorProxy->setPos(QPointF(this->pos().x()+this->width()-7, this->pos().y()+(this->height()-50)));
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

void ObjectElement::decreaseLifeLine()
{
    //If there is no lifeline do nithing
    if (this->lifeLine == nullptr)
        return;
    QPointF p1Point = this->lifeLine->line().p1();
    QPointF p2Point = this->lifeLine->line().p2();

    //IF the lifeline is not shorter than the minimum
    if (p2Point.y() - p1Point.y() > 50){
        //make it shorter
        p2Point.setY(p2Point.y() - 50);
        QLineF newLifeLine = QLineF(this->lifeLine->line().p1(), p2Point);
        this->lifeLine->setLine(newLifeLine);


        //Destroy the last anchor
        QGraphicsProxyWidget *deletedAnchorProxy = this->proxyList.takeLast();;
        this->anchors.remove(deletedAnchorProxy);
        delete deletedAnchorProxy;
    }
}

void ObjectElement::deleteObject()
{
    //Delete later (just in case)
    this->deleteLater();
}

void ObjectElement::updateClasses(){

    //If not yet in comboBox, add
    foreach(ClassElement* classElement, class_scene->classes){
        if(ui->comboBox->findText(classElement->name) == -1)
            ui->comboBox->addItem(classElement->name);

    }
    //Remove all items from comboBox that are no longer valid classes
    //If objects class no longer exists empty the combo box
    bool deletedLastItem = false;
    for(int i = 0; i < ui->comboBox->count(); i++){
        if(deletedLastItem)
            i--;
        bool validClass = false;
        foreach(ClassElement* classElement, class_scene->classes){
            if(ui->comboBox->itemText((i)) == classElement->name)
                validClass = true;
        }

        if (!validClass){
            if(ui->comboBox->currentText() == ui->comboBox->itemText(i))
                ui->comboBox->setCurrentIndex(-1);
            ui->comboBox->removeItem(i);
            deletedLastItem = true;
        }else{
            deletedLastItem = false;
        }
    }
}

//Create new lifeline
QGraphicsLineItem * ObjectElement::createLifeLine(ObjectElement* objectPtr){
    //Draw the dashed line
    QPoint p1 = QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height()));
    QPoint p2 = QPoint(objectPtr->pos().x()+(objectPtr->width()/2), objectPtr->pos().y()+(objectPtr->height()) + 50);
    QLine lifeLine = QLine(p1, p2);
    QPen dashLine = QPen(Qt::DashLine);
    QGraphicsLineItem *lifeLinePtr = seq_scene->addLine(lifeLine, dashLine);
    lifeLinePtr->setZValue(0);

    //Append the first anchor
    MessageAnchor * newAnchor = new MessageAnchor(this);
    QGraphicsProxyWidget * newProxy = seq_scene->addWidget(newAnchor);
    newProxy ->setZValue(2);
    newAnchor->proxy = newProxy;
    newProxy->setPos(p2-QPoint(7,0));
    this->anchors.insert(newProxy, newAnchor);
    this->proxyList.append(newProxy);

    return lifeLinePtr;
}

//If the class of the object has been changed update the messages
void ObjectElement::on_comboBox_currentTextChanged(const QString &arg1)
{

    (void)arg1;
    foreach(SeqMessage *message, seq_scene->messages){
        message->updateMessages();
    }
}

//Update the name of the object
void ObjectElement::on_lineEdit_textChanged(const QString &arg1)
{
    this->name = arg1;
}
