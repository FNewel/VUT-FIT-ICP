#include "objectelement.h"

#include <QGraphicsLineItem>

SeqMessage::SeqMessage(QWidget *parent) : QWidget(parent)
{

}

SeqMessage::~SeqMessage()
{
    this->sourceAnchor->message = nullptr;
    this->destAnchor->message = nullptr;
    if(this->messageLine)
        delete this->messageLine;
}
