/**
 * UML Editor - ICP Project 2022
 * @file activationelement.cpp
 * @brief Source File for the ActivationElement Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/objectelement.h"

#include <QGraphicsRectItem>


//Constructor
ActivationElement::ActivationElement(QWidget *parent) : QWidget(parent)
{

}

//Destructor 
ActivationElement::~ActivationElement()
{
    //Remove pointer to this from the source anchor
    if(this->sourceAnchor)
        this->sourceAnchor->activation = nullptr;
    //Remove pointer to this from the destination anchor
    if(this->destAnchor)
        this->destAnchor->activation = nullptr;
    //Remove pointer to this from the vector of activations
    seq_scene->activations.removeOne(this);
    //Remove the ui rectangle
    if(this->actRect)
        delete this->actRect;
}
