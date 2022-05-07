/**
 * UML Editor - ICP Project 2022
 * @file classlines.h
 * @brief Header File for the ClassLines Class of class diagram.
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef CLASSLINES_H
#define CLASSLINES_H

#include "classelement.h"

#include <QWidget>
#include <QPointer>
#include <QGraphicsItem>
#include <QLine>

/** 
*   Class Lines Class.
*   Class for the lines connecting the Class Elements in class diagram.
*/
class ClassLines {

public:
    /**
    * Pointer to the source Class Element.
    * Pointer to a Class Element this line originates from.
    */
    ClassElement *source;
    /**
    * Pointer to the target Class Element.
    * Pointer to a Class Element this line leads to.
    */
    ClassElement *target;

    /**
    * Source Position
    * Point in the scene from which the line originates.
    */
    QPoint sourcePos;
    /**
    * Target Position
    * Point in the scene to which the line leads.
    */
    QPoint targetPos;

    /**
    * Arrow type at the source of the line.
    * A number storing the type of arrow at the source of the line.
    * Legend:
    * 0 - Association
    * 1 - Generalization
    * 2 - Composition
    * 3 - Aggregation
    */
    int sourceConnection = 0;
    /**
    * Arrow type at the target of the line.
    * A number storing the type of arrow at the target of the line.
    * Legend:
    * 0 - Association
    * 1 - Generalization
    * 2 - Composition
    * 3 - Aggregation
    */
    int targetConnection = 0;

    /**
    * Pointer to the Ui represenation of the line.
    */
    QGraphicsLineItem *lineItem;
};

/**
* Global bool value for mouse clicks.
* Global variable which holds information about whether the source position of the line has been picked.
*/
extern bool isClicked;
/**
* Global Vector of Class Lines.
*/
extern QVector <ClassLines*>lines;



#endif // CLASSLINES_H
