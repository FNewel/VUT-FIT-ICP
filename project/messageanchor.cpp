#include "objectelement.h"
#include "ui_messageanchor.h"

#include <QGraphicsLineItem>
#include <QMoveEvent>




MessageAnchor::MessageAnchor(ObjectElement *parent) :
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
    if(!__gl__msgClicked){
        if(this->message == nullptr){
            SeqMessage *newMsg = new SeqMessage(this);
            __gl__messages.append(newMsg);
            newMsg->sourceAnchor = this;
            newMsg->sourcePos = this->pos()+QPoint(8,8);

             __gl__msgClicked = true;
        }
    }else{

        if(this->message == nullptr & __gl__messages.last()->sourceAnchor != this){
            SeqMessage *msg = __gl__messages.last();
            msg->destAnchor = this;
            msg->destPos = this->pos()+QPoint(8,8);

            QBrush brush = QBrush(Qt::black);
            QPen pen = QPen(brush, 3);
            QGraphicsLineItem *newLine = seq_scene->addLine(msg->sourcePos.x(), msg->sourcePos.y(), msg->destPos.x(), msg->destPos.y(), pen);
            msg->messageLine = newLine;
            msg->addComboBox();
            this->message = msg;
            msg->sourceAnchor->message = msg;
            msg->messageLine->setFlag(QGraphicsItem::ItemIsSelectable);
            __gl__msgClicked = false;
        }
    }


}

void MessageAnchor::moveEvent(QMoveEvent *event)
{
    if(this->message){
        QLineF newMsgLine;
        if (this->message->sourceAnchor == this){
            newMsgLine = QLineF(event->pos()+QPoint(8,8), this->message->messageLine->line().p2());
            this->message->sourcePos = event->pos()+QPoint(8,8);
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        if (this->message->destAnchor == this){
            newMsgLine = QLineF(this->message->messageLine->line().p1(), event->pos()+QPoint(8,8));
            this->message->destPos = event->pos()+QPoint(8,8);
            this->message->updateCBoxLoc();
            this->message->updateArrowHead();
        }
        message->messageLine->setLine(newMsgLine);
    }


}
