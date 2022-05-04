#include "objectelement.h"
#include <QGraphicsRectItem>
ActivationElement::ActivationElement(QWidget *parent) : QWidget(parent)
{

}

ActivationElement::~ActivationElement()
{
    if(this->sourceAnchor)
        this->sourceAnchor->activation = nullptr;
    if(this->destAnchor)
        this->destAnchor->activation = nullptr;
    if(this->actRect)
        delete this->actRect;
}
