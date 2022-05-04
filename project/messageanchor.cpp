#include "objectelement.h"
#include "ui_messageanchor.h"

#include <QGraphicsLineItem>
#include <QMoveEvent>
#include <QDebug>




MessageAnchor::MessageAnchor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageAnchor)
{
    ui->setupUi(this);
    this->setAttribute( Qt::WA_NoMousePropagation );
}

MessageAnchor::~MessageAnchor()
{
    if (this->message){
        delete this->message;
    }
    delete ui;
}

void MessageAnchor::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(!__gl__msgClicked){
            if(this->message == nullptr){
                SeqMessage *newMsg = new SeqMessage(this);
                __gl__messages.append(newMsg);
                newMsg->sourceAnchor = this;
                newMsg->sourcePos = this->pos()+QPoint(this->width()/2,this->height()/2);

                 __gl__msgClicked = true;
            }
        }else{

            if(this->message == nullptr && __gl__messages.last()->sourceAnchor != this){
                SeqMessage *msg = __gl__messages.last();
                msg->destAnchor = this;
                msg->destPos = this->pos()+QPoint(this->width()/2,this->height()/2);

                QGraphicsLineItem *newLine = seq_scene->addLine(msg->sourcePos.x(), msg->sourcePos.y(), msg->destPos.x(), msg->destPos.y());
                msg->messageLine = newLine;
                msg->messageLine->setFlag(QGraphicsItem::ItemIsSelectable);
                msg->setArrow(0);

                //Function combo box only for target objects, not actors
                if(QString::fromUtf8(this->parent()->metaObject()->className()) == "ObjectElement")
                    msg->addComboBox();

                this->message = msg;
                msg->sourceAnchor->message = msg;


                __gl__msgClicked = false;
            }
        }
    }else if(event->button() == Qt::RightButton){
        if(!this->destructionIcon){
            qDebug() << "Here";
            QBrush brush = QBrush(Qt::black);
            QPen pen = QPen(brush, 3);
            QLine line1 = QLine(20,20,-20,-20);
            QLine line2 = QLine(-20,20,20,-20);
            QGraphicsLineItem * line1Proxy = seq_scene->addLine(line1, pen);
            QGraphicsLineItem * line2Proxy = seq_scene->addLine(line2, pen);
            line2Proxy->setParentItem(line1Proxy);
            this->destructionIcon = line1Proxy;
            this->destructionIcon->setPos(this->pos()+QPoint(this->width()/2,this->height()/2));
        }else{
            delete this->destructionIcon;
            this->destructionIcon = nullptr;
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
        message->messageLine->setLine(newMsgLine);
    }


}
