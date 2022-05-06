/**
 * UML Editor - ICP Project 2022
 * @file activationelement.cpp
 * @brief popis TODO
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/objectelement.h"

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
    seq_scene->activations.removeOne(this);
    if(this->actRect)
        delete this->actRect;
}
